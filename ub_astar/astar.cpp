#include "roadef2018/ub_astar/astar.hpp"

#include <set>
#include <random>
#include <algorithm>
#include <vector>
#include <queue>

using namespace roadef2018;

Solution roadef2018::ub_astar(const Instance& ins, Info& info)
{
    LOG(info, LOG_FOLD_START << " simple_astar" << std::endl);

    auto comp = [](const Solution& s1, const Solution& s2) { return s1.waste() >= s2.waste(); };
    std::priority_queue<Solution, std::vector<Solution>, decltype(comp)> q(comp);
    Cpt nb_nodes = 0;

    q.push(Solution(ins));
    while (!q.empty()) {
        // Get node
        Solution sol = q.top();
        q.pop();
        nb_nodes++;
        LOG(info, LOG_FOLD_START << " node " << nb_nodes << std::endl);
        LOG(info, LOG_FOLD_START << " sol" << std::endl << sol << LOG_FOLD_END << std::endl);

        // if found feasible solution
        if (sol.is_complete()) {
            LOG(info, "nb nodes " << nb_nodes << std::endl);
            LOG(info, LOG_FOLD_END << std::endl);
            LOG(info, LOG_FOLD_END << std::endl);
            VER(info, "Nodes: " << nb_nodes << std::endl);
            return algorithm_end(sol, info);
        }

        // explore neighbours
        for (const Insertion& i: sol.all_valid_insertions(info)) {
            LOG(info, i << std::endl);
            Solution neigh_sol(sol);
            neigh_sol.add_item(i, info);
            LOG(info, LOG_FOLD_START << " neigh_sol" << std::endl << neigh_sol << LOG_FOLD_END << std::endl);

            q.push(neigh_sol);
        }
        LOG(info, LOG_FOLD_END << std::endl);
    }
    LOG(info, "ERROR: NO SOLUTION FOUND after " << nb_nodes << " opened nodes" << std::endl);
    LOG(info, LOG_FOLD_END << std::endl);
    return Solution(ins);
}

