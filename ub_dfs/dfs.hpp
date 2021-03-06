#pragma once

#include "roadef2018/lib/instance.hpp"
#include "roadef2018/lib/solution.hpp"

namespace roadef2018
{

struct DFSData
{
    const Instance& ins;
    Solution& sol_best;
    Info info = Info();
};

void sol_dfs(DFSData d);

}

