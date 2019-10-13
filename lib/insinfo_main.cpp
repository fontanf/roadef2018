#include "roadef2018/lib/instance.hpp"

#include <boost/accumulators/statistics/variance.hpp>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <math.h>

using namespace roadef2018;

double mean(std::vector<double> const& data)
{
    double sum = 0;
    for (double val: data)
        sum += val;
    return sum / data.size();
}

double stddev(std::vector<double> const& data)
{
    auto stats = std::make_pair(0.0,0.0);
    stats = std::accumulate(data.begin(), data.end(), stats,
                            [](std::pair<double,double> stats, double x) {
                                stats.first += x;
                                stats.second += x * x;
                                return stats;
                            });
    return sqrt((stats.second - pow(stats.first, 2.0) / data.size()) / data.size());
}

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    // Parse program options
    std::string globalparam_path = "instances/global_param.csv";

    std::vector<std::string> instance_pathes;
    for (int i=1; i<=20; ++i)
        instance_pathes.push_back("instances/A" + std::to_string(i));
    instance_pathes.push_back("");
    for (int i=1; i<=15; ++i)
        instance_pathes.push_back("instances/B" + std::to_string(i));
    instance_pathes.push_back("");
    for (int i=1; i<=15; ++i)
        instance_pathes.push_back("instances/X" + std::to_string(i));

    std::cout << "Item number,Stack number,Mean stack size,Stack size SD,Total item area,Mean item area,Item area SD,Item area SD %,Mean stack area SD" << std::endl;

    for (std::string instance_path: instance_pathes) {
        if (instance_path == "") {
            std::cout << std::endl;
            continue;
        }

        Instance ins(instance_path + "_batch.csv", instance_path + "_defects.csv", globalparam_path);

        std::vector<double> item_areas;
        for (ItemId j=0; j<ins.item_number(); ++j)
            item_areas.push_back(ins.item(j).rect.area());

        std::vector<double> stack_sizes;
        for (StackId s=0; s<ins.stack_number(); ++s)
            stack_sizes.push_back(ins.stack_size(s));

        std::vector<double> stack_mean_areas;
        for (StackId s=0; s<ins.stack_number(); ++s) {
            stack_mean_areas.push_back(0);
            for (ItemPos jj=0; jj<ins.stack_size(s); ++jj)
                stack_mean_areas.back() += ins.item(s, jj).rect.area();
            stack_mean_areas.back() /= ins.stack_size(s);
        }

        std::cout
            << instance_path << ","
            << ins.item_number() << ","
            << ins.stack_number() << ","
            << (double)ins.item_number() / ins.stack_number() << ","
            << (int)stddev(stack_sizes) << ","
            << ins.item_area() << ","
            << (int)mean(item_areas) << ","
            << (int)stddev(item_areas) << ","
            << stddev(item_areas) / mean(item_areas) * 100 << ","
            << (int)stddev(stack_mean_areas) << std::endl;
    }

    return 0;
}

