#include "roadef2018/lb_dynstar/astar.hpp"

using namespace roadef2018;

int main(int argc, char *argv[])
{
    namespace po = boost::program_options;

    // Parse program options
    std::string batch_filename = "";
    std::string globalparam_filepath = "data/global_param.csv";
    std::string       batch_filepath = "data/dataset_A/A1_batch.csv";
    std::string     defects_filepath = "data/dataset_A/A1_defects.csv";

    std::string output_file = "";
    std::string cert_file = "";
    std::string log_file = "log.txt";

    double time_limit = 600;
    char dataset = 'A';
    int  id = 1;
    int seed = 0;

    po::options_description desc("Allowed options");
    desc.add_options()
        ("help,h", "produce help message")
        ("dataset,d", po::value<char>(&dataset)->required(), "set dataset id (A or B)")
        ("id,i", po::value<int>(&id), "set instance id (1, 2, ...)")
        ("verbose,v", "enable verbosity")
        ("time-limit,t", po::value<double>(&time_limit), "set log file")
        ("output-file,o", po::value<std::string>(&output_file), "set output file")
        ("cert-file,c", po::value<std::string>(&cert_file), "set certificate output file")
        ("log2stderr", "write log in stderr")
        ("logfile,l", po::value<std::string>(&log_file), "set log file")
        ("seed,s", po::value<int>(&seed), "set seed")
        ;
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
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

    globalparam_filepath = "data/global_param.csv";
    batch_filepath   = std::string("data/dataset_") + dataset + "/" + dataset + std::to_string(id) + "_batch.csv";
    defects_filepath = std::string("data/dataset_") + dataset + "/" + dataset + std::to_string(id) + "_defects.csv";
    Instance ins(batch_filepath, defects_filepath, globalparam_filepath);

    Logger logger(log_file, vm.count("log2stderr"));
    Info info(logger, true);
    info.only_write_at_the_end = false;
    info.cert_file = std::string("checker/instances_checker/") + dataset
        + std::to_string(id) + "_solution.csv";
    info.only_write_at_the_end = false;
    srand(seed);

    Solution sol = sol_dynstar(ins, info, time_limit);

    info.write_ini(output_file); // Write output file
    return 0;
}

