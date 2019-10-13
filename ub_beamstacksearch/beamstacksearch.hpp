#pragma once

#include "roadef2018/lib/instance.hpp"
#include "roadef2018/lib/solution.hpp"

namespace roadef2018
{

struct BeamStackSearchData
{
    const Instance& ins;
    int criterion_id;
    Cpt size;
    Solution& sol_best;
    Info info = Info();
};

void sol_beamstacksearch(BeamStackSearchData d);

}

