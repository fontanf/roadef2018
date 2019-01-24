#include "roadef2018/lib/instance.hpp"
#include "roadef2018/lib/solution.hpp"

#include "roadef2018/opt_astar/astar.hpp"

using namespace roadef2018;

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    std::vector<Item> vi {
        {.id = 0, .rect = {3000, 1000}, .stack = 0, .sequence = 0},
        {.id = 1, .rect = {3000, 2210}, .stack = 1, .sequence = 0}};
    std::vector<Defect> vd;
    GlobalParam p = GlobalParam::roadef2018();
    Logger logger("log.txt");
    Info info(logger);
    Instance ins(vi, vd, p);
    Solution sol(opt_simple_astar(ins, info));
    std::cout << "waste " << sol.waste() << std::endl;
    std::cout << "lb_area " << sol.lb_area() << std::endl;
    std::cout << sol << std::endl;
}


