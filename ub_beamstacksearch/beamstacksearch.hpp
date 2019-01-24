#pragma once

#include "roadef2018/lib/instance.hpp"
#include "roadef2018/lib/solution.hpp"

#include <vector>
#include <chrono>
#include <omp.h>

namespace roadef2018
{

struct BeamStackSearchData
{
    const Instance& ins;
    Info& info;
    double time_limit;
    int criterion_id;
    Cpt size;
    Solution& best_solution;
    Cpt& sol_number;
    omp_lock_t& writelock;
};

void sol_beamstacksearch(BeamStackSearchData d);

}

