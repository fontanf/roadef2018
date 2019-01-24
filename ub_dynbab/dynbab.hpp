#pragma once

#include "roadef2018/lib/instance.hpp"
#include "roadef2018/lib/solution.hpp"

#include <chrono>
#include <omp.h>

namespace roadef2018
{

struct DynBabData
{
    const Instance& ins;
    Info& info;
    double time_limit;
    double growth_factor;
    int criterion_id;
    Solution& best_solution;
    Cpt& sol_number;
    omp_lock_t& writelock;
};

void sol_dynbab(DynBabData d);

}

