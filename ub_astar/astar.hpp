#pragma once

#include "roadef2018/lib/instance.hpp"
#include "roadef2018/lib/solution.hpp"

#include <chrono>

namespace roadef2018
{

/**
 * simple A* algorithm (no specific cuts). Used to test branching methods
 */
Solution ub_astar(const Instance& ins, Info& info);

}

