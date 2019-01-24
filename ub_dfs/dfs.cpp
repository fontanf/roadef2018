#include "roadef2018/ub_dfs/dfs.hpp"

#include <set>
#include <random>
#include <algorithm>
#include <vector>
#include <queue>

using namespace roadef2018;

void ub_dfs_rec(const Solution& sol_curr, Solution& sol_best, Cpt& sol_number, Info& info)
{
    LOG(info, LOG_FOLD_START << " opt_simple_dfs_rec" << std::endl);
    LOG(info, LOG_FOLD_START << " sol_curr" << std::endl << sol_curr << LOG_FOLD_END << std::endl);

    if (sol_best.is_complete() && sol_best.waste() <= sol_curr.waste())
        return;

    if (sol_curr.is_complete())
        if (!sol_best.is_complete() || sol_curr.waste() < sol_best.waste())
            sol_best.update(sol_curr, info, sol_number);

    std::vector<Solution> sols;
    for (const Insertion& i: sol_curr.all_valid_insertions(info)) {
        LOG(info, i << std::endl);
        sols.push_back(sol_curr);
        sols.back().add_item(i, info);
    }

    sort(sols.begin(), sols.end(), [](const Solution& s1, const Solution& s2) -> bool { return s1.waste() < s2.waste(); });

    for (const Solution& sol: sols)
        ub_dfs_rec(sol, sol_best, sol_number, info);

    LOG(info, LOG_FOLD_END << std::endl);
}

Solution roadef2018::ub_dfs(const Instance& ins, Info& info)
{
    Solution sol(ins);
    Solution sol_best(ins);
    Cpt sol_number = 0;
    ub_dfs_rec(sol, sol_best, sol_number, info);
    return sol_best;
}

