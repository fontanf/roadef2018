#pragma once

#include "roadef2018/lib/instance.hpp"
#include "roadef2018/lib/solution.hpp"

namespace roadef2018
{

struct DPAStarData
{
    const Instance& ins;
    Solution& sol_best;
    Info info = Info();
};

void sol_dpastar_1(DPAStarData d);
void sol_dpastar_2(DPAStarData d);
void sol_dpastar(DPAStarData d);

}

