#include "roadef2018/ub_dfs/dfs.hpp"
#include "roadef2018/ub_astar/astar.hpp"
#include "roadef2018/ub_dpastar/dpastar.hpp"
#include "roadef2018/ub_beamsearch/beamsearch.hpp"
#include "roadef2018/ub_beamstacksearch/beamstacksearch.hpp"
#include "roadef2018/ub_mbastar/mbastar.hpp"

#include <iomanip>

using namespace roadef2018;

void run(std::string algo, const Instance& ins, Solution& sol, Info info)
{
    std::stringstream ss(algo);
    std::istream_iterator<std::string> begin(ss);
    std::istream_iterator<std::string> end;
    std::vector<std::string> vstrings(begin, end);

    std::map<std::string, std::string> args;
    for (auto it=std::next(vstrings.begin());
            it!=vstrings.end() && std::next(it)!=vstrings.end();
            it=std::next(std::next(it)))
        args[*it] = *std::next(it);

    if (vstrings[0] == "DFS") {
        sol_dfs({
                .ins           = ins,
                .sol_best      = sol,
                .info          = info
                });
    } else if (vstrings[0] == "A*") {
        sol_astar({
                .ins           = ins,
                .sol_best      = sol,
                .info          = info
                });
    } else if (vstrings[0] == "MBA*") {
        auto it = args.find("f");
        double f = (it != args.end())? std::stod(it->second): 1.5;
        it = args.find("c");
        int c = (it != args.end())? std::stoi(it->second): 1;
        sol_mbastar({
                .ins           = ins,
                .growth_factor = f,
                .criterion_id  = c,
                .sol_best      = sol,
                .info          = info
                });
    } else if (vstrings[0] == "BS") {
        auto it = args.find("f");
        double f = (it != args.end())? std::stod(it->second): 1.5;
        it = args.find("c");
        int c = (it != args.end())? std::stoi(it->second): 1;
        sol_beamsearch({
                .ins           = ins,
                .growth_factor = f,
                .criterion_id  = c,
                .sol_best      = sol,
                .info          = info
                });
    } else if (vstrings[0] == "BSS") {
        auto it = args.find("c");
        int c = (it != args.end())? std::stoi(it->second): 1;
        it = args.find("s");
        int s = (it != args.end())? std::stoi(it->second): 1024;
        sol_beamstacksearch({
                .ins           = ins,
                .criterion_id  = c,
                .size          = s,
                .sol_best      = sol,
                .info          = info
                });
    } else if (vstrings[0] == "DPA*") {
        auto it = args.find("s");
        double s = (it != args.end())? std::stod(it->second): -1;
        if (ins.stack_number() == 1) {
            sol_dpastar_1({
                    .ins           = ins,
                    .sol_best      = sol,
                    .info          = info
                    });
        } else if (ins.stack_number() == 2) {
            sol_dpastar_2({
                    .ins           = ins,
                    .sol_best      = sol,
                    .info          = info
                    });
        } else if (ins.state_number() <= s) {
            sol_dpastar({
                    .ins           = ins,
                    .sol_best      = sol,
                    .info          = info
                    });
        }
    } else {
        VER(info, "WARNING: unknown algorithm \"" << vstrings[0] << "\"" << std::endl);
    }
}

int main(int argc, char *argv[])
{
    namespace po = boost::program_options;

    // Parse program options
    std::string globalparam_path = "instances/global_param.csv";
    std::string    instance_path = "";
    std::string certificate_path = "";

    std::string output_path = "";
    std::string    log_path = "";
    int log_levelmax = 999;

    std::string team_id = "J28";

    double time_limit = std::numeric_limits<double>::infinity();
    std::vector<std::string> algorithms = {
        "MBA* c 0 f 1.33",
        "MBA* c 1 f 1.33",
        "MBA* c 0 f 1.5",
        "MBA* c 1 f 1.5",
        "DPA* s -1",
    };
    int seed;

    po::options_description desc("Allowed options");
    desc.add_options()
        (",h", "Produce help message")
        ("name", "Team ID")
        (",t", po::value<double>(&time_limit), "Time limit in seconds\n  ex: 3600")
        (",a", po::value<std::vector<std::string>>(&algorithms)->multitoken(), "Algorithms\n  ex \"MBA* c 1 f 1.5\"")
        (",s", po::value<int>(&seed), "Seed (not used)")

        (",p", po::value<std::string>(&instance_path)->required(), "Instance path\n  ex: \"instances/A1\"")
        (",o", po::value<std::string>(&certificate_path), "Certificate path\n  ex: \"solutions_3600/A15_solution.csv\"")

        (",g", po::value<std::string>(&globalparam_path), "Global param path\n  default: \"instances/global_param.csv\"")
        (",i", po::value<std::string>(&output_path), "Output path\n  ex: \"output.ini\"")
        (",v", "Verbose")
        (",l", po::value<std::string>(&log_path), "Log path")
        (",w", "Write log in stderr")
        ;
    po::variables_map vm;
    boost::program_options::store(
            boost::program_options::command_line_parser(argc, argv)
            .options(desc)
            .style(
                boost::program_options::command_line_style::unix_style |
                boost::program_options::command_line_style::allow_long_disguise)
            .run(),
            vm);
    if (vm.count("name")) {
        std::cout << team_id << std::endl;;
        return 1;
    }
    if (vm.count("-h")) {
        std::cout << desc << std::endl;;
        return 1;
    }
    try {
        po::notify(vm);
    } catch (po::required_option e) {
        std::cout << desc << std::endl;;
        return 1;
    }

    Instance ins(instance_path + "_batch.csv", instance_path + "_defects.csv", globalparam_path);

    Info info = Info()
        .set_verbose(vm.count("-v"))
        .set_timelimit(time_limit)
        .set_certfile(certificate_path)
        .set_outputfile(output_path)
        .set_onlywriteattheend(false)
        .set_logfile(log_path)
        .set_log2stderr(vm.count("-w"))
        .set_loglevelmax(log_levelmax)
        ;

    Solution sol(ins);

    VER(info, std::left << std::setw(6) << "Sol");
    VER(info, std::left << std::setw(22) << "Algorithm");
    VER(info, std::left << std::setw(12) << "Waste");
    VER(info, std::left << std::setw(11) << "Waste (%)");
    VER(info, std::left << std::setw(9) << "Width");
    VER(info, std::left << std::setw(9) << "Plates");
    VER(info, "Time");
    VER(info, std::endl);

    std::vector<std::thread> threads;
    for (Cpt i=0; i<(Cpt)algorithms.size(); ++i) {
        std::string s = algorithms[i];
        std::replace(s.begin(), s.end(), ' ', '-');
        threads.push_back(std::thread(run,
                    algorithms[i], std::ref(ins), std::ref(sol), Info(info, true, s)));
    }
    for (std::thread& thread: threads)
        thread.join();

    algorithm_end(sol, info);
    info.write_ini(output_path); // Write output file
    return 0;
}

