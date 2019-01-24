#include "roadef2018/lib/instance.hpp"

#include <iostream>

using namespace roadef2018;

int main(int argc, char* argv[]) 
{
    std::cout << "##### TEST LOADING INSTANCES ####" << "\n\n";
    if (argc < 4) 
    {
        std::cout << "USAGE: " << argv[0] << " BATCH.CSV DEFECTS.CSV GLOBAL_PARAM.CSV" << std::endl;
        return 1;
    }
    Instance inst = Instance(argv[1], argv[2], argv[3]);
    std::cout << inst.to_string() << std::endl;
    return 0;
}

