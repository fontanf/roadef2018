#pragma once

#include "roadef2018/lib/instance.hpp"
#include "roadef2018/lib/solution.hpp"

#include <vector>
#include <chrono>
#include <omp.h>

namespace roadef2018
{

struct BeamSearchData
{
    const Instance& ins;
    Info& info;
    double time_limit;
    bool break_symetries;
    int criterion_id;
    float growing_factor;
    Solution& best_solution;
    Cpt& sol_number;
    omp_lock_t& writelock;
};

void sol_beamsearch(BeamSearchData d);

}

