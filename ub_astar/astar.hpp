#pragma once

#include "roadef2018/lib/instance.hpp"
#include "roadef2018/lib/solution.hpp"

namespace roadef2018
{

struct AStarData
{
    const Instance& ins;
    Solution& sol_best;
    Info info;
};

void sol_astar(AStarData d);
Solution sol_astar(const Instance& ins, Info info = Info());

}

