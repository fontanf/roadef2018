#pragma once

#include "roadef2018/lib/instance.hpp"
#include "roadef2018/lib/solution.hpp"

#include <chrono>

namespace roadef2018
{

Solution sol_astar_historycuts_1(const Instance& ins, Info& info);
Solution sol_astar_historycuts_2(const Instance& ins, Info& info);
Solution sol_astar_historycuts(const Instance& ins, Info& info);

}

