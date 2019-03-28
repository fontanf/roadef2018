#include "roadef2018/ub_astar/astar.hpp"

#include <set>
#include <random>
#include <algorithm>
#include <vector>
#include <queue>

using namespace roadef2018;

void roadef2018::sol_astar(AStarData d)
{
    LOG_FOLD_START(d.info, "simple_astar" << std::endl);

    auto comp = [](const Solution& s1, const Solution& s2) { return s1.waste() >= s2.waste(); };
    std::priority_queue<Solution, std::vector<Solution>, decltype(comp)> q(comp);
    Cpt node_number = 0;

    q.push(Solution(d.ins));
    while (!q.empty()) {
        node_number++;
        LOG_FOLD_START(d.info, "node " << node_number << std::endl);

        if (!d.info.check_time()) {
            LOG_FOLD_END(d.info, "");
            return;
        }

        // Get node
        Solution sol = q.top();
        q.pop();
        LOG_FOLD(d.info, "sol" << std::endl << sol);

        // if found feasible solution
        if (sol.is_complete()) {
            if (!d.sol_best.is_complete() || sol.waste() < d.sol_best.waste())
                d.sol_best.update(sol, d.info, std::stringstream("A*"));
            LOG(d.info, "node number " << node_number << std::endl);
            LOG_FOLD_END(d.info, "");
            LOG_FOLD_END(d.info, "");
            PUT(d.info, "A*.Nodes", node_number);
            return;
        }

        // explore neighbours
        for (const Insertion& i: sol.all_valid_insertions(d.info)) {
            LOG(d.info, i << std::endl);
            Solution neigh_sol(sol);
            neigh_sol.add_item(i, d.info);
            LOG_FOLD(d.info, "neigh_sol" << std::endl << neigh_sol);

            q.push(neigh_sol);
        }
        LOG_FOLD_END(d.info, "");
    }
    LOG(d.info, "ERROR: NO SOLUTION FOUND after " << node_number << " opened nodes" << std::endl);
    LOG_FOLD_END(d.info, "");
}

Solution roadef2018::sol_astar(const Instance& ins, Info info)
{
    Solution sol(ins);
    sol_astar({
            .ins           = ins,
            .sol_best      = sol,
            .info          = info,
            });
    return sol;
}
