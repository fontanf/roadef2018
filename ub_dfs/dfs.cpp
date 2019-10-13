#include "roadef2018/ub_dfs/dfs.hpp"

using namespace roadef2018;

void sol_dfs_rec(DFSData& d, const Solution& sol_curr)
{
    LOG_FOLD_START(d.info, "opt_simple_dfs_rec" << std::endl);
    LOG_FOLD(d.info, "sol_curr" << std::endl << sol_curr);

    if (!d.info.check_time()) {
        LOG_FOLD_END(d.info, "");
        return;
    }

    if (d.sol_best.is_complete() && d.sol_best.waste() <= sol_curr.waste())
        return;

    if (sol_curr.is_complete())
        if (!d.sol_best.is_complete() || sol_curr.waste() < d.sol_best.waste())
            d.sol_best.update(sol_curr, d.info, std::stringstream("DFS"));

    std::vector<Solution> sols;
    for (const Insertion& i: sol_curr.all_valid_insertions(d.info)) {
        LOG(d.info, i << std::endl);
        sols.push_back(sol_curr);
        sols.back().add_item(i, d.info);
    }

    sort(sols.begin(), sols.end(), [](const Solution& s1, const Solution& s2) -> bool { return s1.waste() < s2.waste(); });

    for (const Solution& sol: sols)
        sol_dfs_rec(d, sol);

    LOG_FOLD_END(d.info, "");
}

void roadef2018::sol_dfs(DFSData d)
{
    Solution sol(d.ins);
    sol_dfs_rec(d, sol);
}

