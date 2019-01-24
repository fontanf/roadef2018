#include "roadef2018/ub_astarhistorycuts/astarhistorycuts.hpp"

#include <set>
#include <random>
#include <algorithm>
#include <vector>
#include <queue>

using namespace roadef2018;

bool call_history_1(
        std::vector<std::vector<Front>>& history,
        Solution sol, Info& info)
{
    std::vector<Front>& list = history[sol.pos_stack(0)];
    // check if front is dominated
    for (Front f: list) {
        if (Solution::dominates(f, sol.front(), sol.instance().global_param())) { // if front is dominated
            LOG(info, "history " << sol.pos_stack(0) << " " << sol.pos_stack(1) << " " << sol.front() << std::endl);
            LOG(info, "dominated by " << f << std::endl);
            return false;
        }
    }

    // remove dominated fronts in list
    for (auto it=list.begin(); it != list.end();) {
        if (Solution::dominates(sol.front(), *it, sol.instance().global_param())) {
            *it = list.back();
            list.pop_back();
        } else {
            ++it;
        }
    }

    // add front
    LOG(info, "history " << sol.pos_stack(0) << " " << sol.pos_stack(1) << " " << sol.front() << std::endl);
    list.push_back(sol.front());

    return true;
}

Solution roadef2018::sol_astar_historycuts_1(const Instance& ins, Info& info)
{
    LOG(info, LOG_FOLD_START << " simple_astar" << std::endl);
    assert(ins.stack_number() == 1);

    auto comp = [](const Solution& s1, const Solution& s2) { return s1.waste() >= s2.waste(); };
    std::priority_queue<Solution, std::vector<Solution>, decltype(comp)> q(comp);
    Cpt nb_nodes = 0;

    /* create history cut object */
    std::vector<std::vector<Front>> history(ins.stack(0).size()+1);

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

            if (i.j1 != -1 || i.j2 != -1) {
                if (!call_history_1(history, neigh_sol, info)) {
                    LOG(info, " history cut x" << std::endl);
                    continue;
                }
            }

            q.push(neigh_sol);
        }
        LOG(info, LOG_FOLD_END << std::endl);
    }
    LOG(info, "ERROR: NO SOLUTION FOUND after " << nb_nodes << " opened nodes" << std::endl);
    LOG(info, LOG_FOLD_END << std::endl);
    return Solution(ins);
}

/******************************************************************************/

bool call_history_2(
        std::vector<std::vector<std::vector<Front>>>& history,
        Solution sol, Info& info)
{
    std::vector<Front>& list = history[sol.pos_stack(0)][sol.pos_stack(1)];
    // check if front is dominated
    for (Front f: list) {
        if (Solution::dominates(f, sol.front(), sol.instance().global_param())) { // if front is dominated
            LOG(info, "history " << sol.pos_stack(0) << " " << sol.pos_stack(1) << " " << sol.front() << std::endl);
            LOG(info, "dominated by " << f << std::endl);
            return false;
        }
    }

    // remove dominated fronts in list
    for (auto it=list.begin(); it != list.end();) {
        if (Solution::dominates(sol.front(), *it, sol.instance().global_param())) {
            *it = list.back();
            list.pop_back();
        } else {
            ++it;
        }
    }

    // add front
    LOG(info, "history " << sol.pos_stack(0) << " " << sol.pos_stack(1) << " " << sol.front() << std::endl);
    list.push_back(sol.front());

    return true;
}

Solution roadef2018::sol_astar_historycuts_2(const Instance& ins, Info& info)
{
    LOG(info, LOG_FOLD_START << " simple_astar" << std::endl);
    assert(ins.stack_number() == 2);

    auto comp = [](const Solution& s1, const Solution& s2) { return s1.waste() >= s2.waste(); };
    std::priority_queue<Solution, std::vector<Solution>, decltype(comp)> q(comp);
    Cpt nb_nodes = 0;

    /* create history cut object */
    std::vector<std::vector<std::vector<Front>>> history;
    for (StackId i=0; i<=(StackId)ins.stack(0).size(); ++i)
        history.push_back(std::vector<std::vector<Front>>(ins.stack(1).size()+1));

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

            if (i.j1 != -1 || i.j2 != -1) {
                if (!call_history_2(history, neigh_sol, info)) {
                    LOG(info, " history cut x" << std::endl);
                    continue;
                }
            }

            q.push(neigh_sol);
        }
        LOG(info, LOG_FOLD_END << std::endl);
    }
    LOG(info, "ERROR: NO SOLUTION FOUND after " << nb_nodes << " opened nodes" << std::endl);
    LOG(info, LOG_FOLD_END << std::endl);
    return Solution(ins);
}

/******************************************************************************/

bool call_history(
        std::map<std::vector<ItemId>, std::vector<Front>>& history,
        Solution sol, Info& info)
{
    auto list = history.find(sol.pos_stack());
    if (list == history.end()) {
        history[sol.pos_stack()] = {};
    }

    // check if front is dominated
    for (Front f: list->second) {
        if (Solution::dominates(f, sol.front(), sol.instance().global_param())) { // if front is dominated
            LOG(info, "history " << sol.pos_stack(0) << " " << sol.pos_stack(1) << " " << sol.front() << std::endl);
            LOG(info, "dominated by " << f << std::endl);
            return false;
        }
    }

    // remove dominated fronts in list
    for (auto it=list->second.begin(); it != list->second.end();) {
        if (Solution::dominates(sol.front(), *it, sol.instance().global_param())) {
            *it = list->second.back();
            list->second.pop_back();
        } else {
            ++it;
        }
    }

    // add front
    LOG(info, "history " << sol.pos_stack(0) << " " << sol.pos_stack(1) << " " << sol.front() << std::endl);
    list->second.push_back(sol.front());

    return true;
}

Solution roadef2018::sol_astar_historycuts(const Instance& ins, Info& info)
{
    LOG(info, LOG_FOLD_START << " simple_astar" << std::endl);

    auto comp = [](const Solution& s1, const Solution& s2) { return s1.waste() >= s2.waste(); };
    std::priority_queue<Solution, std::vector<Solution>, decltype(comp)> q(comp);
    Cpt nb_nodes = 0;

    /* create history cut object */
    std::map<std::vector<ItemId>, std::vector<Front>> history;

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
        for (const Insertion& i: sol.all_valid_insertions(info, false)) {
            LOG(info, i << std::endl);
            Solution neigh_sol(sol);
            neigh_sol.add_item(i, info);
            LOG(info, LOG_FOLD_START << " neigh_sol" << std::endl << neigh_sol << LOG_FOLD_END << std::endl);

            if (i.j1 != -1 || i.j2 != -1) {
                if (!call_history(history, neigh_sol, info)) {
                    LOG(info, " history cut x" << std::endl);
                    continue;
                }
            }

            q.push(neigh_sol);
        }
        LOG(info, LOG_FOLD_END << std::endl);
    }
    LOG(info, "ERROR: NO SOLUTION FOUND after " << nb_nodes << " opened nodes" << std::endl);
    LOG(info, LOG_FOLD_END << std::endl);
    return Solution(ins);
}

