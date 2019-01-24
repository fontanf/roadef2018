#include "roadef2018/ub_dynbab/dynbab.hpp"
#include "roadef2018/ub_astarhistorycuts/astarhistorycuts.hpp"
#include "roadef2018/ub_beamsearch/beamsearch.hpp"
#include "roadef2018/ub_beamstacksearch/beamstacksearch.hpp"

#include <iomanip>

using namespace roadef2018;

int main(int argc, char *argv[])
{
    namespace po = boost::program_options;

    // Parse program options
    std::string globalparam_path = "instances/global_param.csv";
    std::string    instance_path = "";
    std::string certificate_path = "";

    std::string output_path = "";
    std::string    log_path = "";

    std::string team_id = "J28";

    double time_limit = 9999999999999;
    int seed;

    po::options_description desc("Allowed options");
    desc.add_options()
        (",h", "Produce help message")
        ("name", "Team ID")
        (",t", po::value<double>(&time_limit), "Time limit in seconds\n  ex: 3600")
        (",s", po::value<int>(&seed), "Seed (not used)")

        (",p", po::value<std::string>(&instance_path)->required(), "Instance path\n  ex: \"instances/A1\"")
        (",o", po::value<std::string>(&certificate_path), "Certificate path\n  ex: \"solutions_3600/A15_solution.csv\"")

        (",g", po::value<std::string>(&globalparam_path), "Global param path\n  default: \"instances/global_param.csv\"")
        (",i", po::value<std::string>(&output_path), "Output path\n  ex: \"output.ini\"")
        (",v", "")
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
    if (vm.count("help")) {
        std::cout << desc << std::endl;;
        return 1;
    }
    try {
        po::notify(vm);
    } catch (po::required_option e) {
        std::cout << desc << std::endl;;
        return 1;
    }
    time_limit--;

    Instance ins(instance_path + "_batch.csv", instance_path + "_defects.csv", globalparam_path);

    Logger logger(log_path, vm.count("log2stderr"));
    Info info(logger, true);
    info.only_write_at_the_end = false;
    info.cert_file = certificate_path;
    boost::filesystem::path p(certificate_path);
    boost::filesystem::create_directories(p.parent_path());

    Solution sol(ins);
    Cpt sol_number = 0;
    omp_lock_t writelock;
    omp_init_lock(&writelock);

    VER(info, std::left << std::setw(6) << "Sol");
    VER(info, std::left << std::setw(27) << "Algorithm");
    VER(info, std::left << std::setw(13) << "Waste");
    VER(info, std::left << std::setw(11) << "Waste (%)");
    VER(info, "Time");
    VER(info, std::endl);

    #pragma omp parallel sections // starts a new team
    {
        #pragma omp section
        {
            sol_dynbab({
                    .ins             = ins,
                    .info            = info,
                    .time_limit      = time_limit,
                    .growth_factor   = 1.5,
                    .criterion_id   = 0,
                    .best_solution  = sol,
                    .sol_number     = sol_number,
                    .writelock      = writelock
                    });
        }
        #pragma omp section
        {
            sol_dynbab({
                    .ins             = ins,
                    .info            = info,
                    .time_limit      = time_limit,
                    .growth_factor   = 1.5,
                    .criterion_id    = 1,
                    .best_solution  = sol,
                    .sol_number     = sol_number,
                    .writelock      = writelock
                    });
        }
        #pragma omp section
        {
            sol_dynbab({
                    .ins             = ins,
                    .info            = info,
                    .time_limit      = time_limit,
                    .growth_factor   = 3.0,
                    .criterion_id   = 0,
                    .best_solution  = sol,
                    .sol_number     = sol_number,
                    .writelock      = writelock
                    });
        }
        #pragma omp section
        {
            sol_dynbab({
                    .ins             = ins,
                    .info            = info,
                    .time_limit      = time_limit,
                    .growth_factor   = 3.0,
                    .criterion_id    = 1,
                    .best_solution  = sol,
                    .sol_number     = sol_number,
                    .writelock      = writelock
                    });
        }
        #pragma omp section
        {
            if (ins.stack_number() <= 2) {
                Solution sol_tmp(ins);
                Info info_tmp(info);
                info_tmp.verbose = false;
                if (ins.stack_number() == 1) {
                    sol_tmp = sol_astar_historycuts_1(ins, info_tmp);
                } else if (ins.stack_number() == 2) {
                    sol_tmp = sol_astar_historycuts_2(ins, info_tmp);
                }
                omp_set_lock(&writelock);
                if (!sol.is_complete() || sol_tmp.waste() < sol.waste())
                    sol.update(sol_tmp, info, sol_number, "A*");
                omp_unset_lock(&writelock);
            }
        }
    }
    algorithm_end(sol, info);
    info.write_ini(output_path); // Write output file
    omp_destroy_lock(&writelock);
    return 0;
}

