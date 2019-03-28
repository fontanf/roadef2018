#include "roadef2018/ub_dpastar/dpastar.hpp"

#include <set>
#include <random>
#include <algorithm>
#include <vector>
#include <queue>

using namespace roadef2018;

bool call_history_1(
        std::vector<std::vector<Front>>& history,
        const Solution& sol, Info& info)
{
    std::vector<Front>& list = history[sol.pos_stack(0)];

    // Check if front is dominated
    for (Front f: list) {
        if (Solution::dominates(f, sol.front(), sol.instance().global_param())) {
            LOG(info, "history " << sol.pos_stack(0) << " " << sol.pos_stack(1) << " " << sol.front() << std::endl);
            LOG(info, "dominated by " << f << std::endl);
            return false;
        }
    }

    // Remove dominated fronts in list
    for (auto it=list.begin(); it != list.end();) {
        if (Solution::dominates(sol.front(), *it, sol.instance().global_param())) {
            *it = list.back();
            list.pop_back();
        } else {
            ++it;
        }
    }

    // Add front
    LOG(info, "history " << sol.pos_stack(0) << " " << sol.pos_stack(1) << " " << sol.front() << std::endl);
    list.push_back(sol.front());

    return true;
}

void roadef2018::sol_dpastar_1(DPAStarData d)
{
    LOG_FOLD_START(d.info, "DPA* 1" << std::endl);
    assert(d.ins.stack_number() == 1);

    std::multiset<Solution, SolutionCompare> q(SolutionCompare(2));
    Cpt node_number = 0;
    Cpt q_sizemax = 0;

    // Create history cut object
    std::vector<std::vector<Front>> history(d.ins.stack(0).size()+1);

    q.insert(Solution(d.ins));
    while (!q.empty()) {
        node_number++;
        LOG_FOLD_START(d.info, "node " << node_number << std::endl);

        // Get node
        Solution sol(*q.begin());
        q.erase(q.begin());
        LOG_FOLD(d.info, "sol" << std::endl << sol);
        if (q_sizemax < (Cpt)q.size())
            q_sizemax = q.size();

        // If found feasible solution
        if (sol.is_complete()) {
            if (!d.sol_best.is_complete() || sol.waste() < d.sol_best.waste())
                d.sol_best.update(sol, d.info, std::stringstream("DPA*"));
            LOG(d.info, "node number " << node_number << std::endl);
            LOG_FOLD_END(d.info, "");
            LOG_FOLD_END(d.info, "");
            PUT(d.info, "DPA*.Nodes", node_number);
            PUT(d.info, "DPA*.QueueMaxSize", q_sizemax);
            return;
        }

        // Explore neighbours
        for (const Insertion& i: sol.all_valid_insertions(d.info, false)) {
            LOG(d.info, i << std::endl);
            Solution sol_tmp(sol);
            sol_tmp.add_item(i, d.info, false);
            LOG_FOLD(d.info, "sol_tmp" << std::endl << sol_tmp);

            if (d.sol_best.is_complete() && sol_tmp.waste() >= d.sol_best.waste()) {
                LOG(d.info, " waste cut ×" << std::endl);
                continue;
            }

            if (i.j1 != -1 || i.j2 != -1) {
                if (!call_history_1(history, sol_tmp, d.info)) {
                    LOG(d.info, " history cut x" << std::endl);
                    continue;
                }
            }

            q.insert(sol_tmp);
        }

        if (d.sol_best.is_complete() && !q.empty())
            while (std::prev(q.end())->waste() >= d.sol_best.waste())
                q.erase(std::prev(q.end()));

        LOG_FOLD_END(d.info, "");
    }
    LOG_FOLD_END(d.info, "");
}

/******************************************************************************/

bool call_history_2(
        std::vector<std::vector<std::vector<Front>>>& history,
        const Solution& sol, Info& info)
{
    std::vector<Front>& list = history[sol.pos_stack(0)][sol.pos_stack(1)];

    // Check if front is dominated
    for (Front f: list) {
        if (Solution::dominates(f, sol.front(), sol.instance().global_param())) {
            LOG(info, "history " << sol.pos_stack(0) << " " << sol.pos_stack(1) << " " << sol.front() << std::endl);
            LOG(info, "dominated by " << f << std::endl);
            return false;
        }
    }

    // Remove dominated fronts in list
    for (auto it=list.begin(); it != list.end();) {
        if (Solution::dominates(sol.front(), *it, sol.instance().global_param())) {
            *it = list.back();
            list.pop_back();
        } else {
            ++it;
        }
    }

    // Add front
    LOG(info, "history " << sol.pos_stack(0) << " " << sol.pos_stack(1) << " " << sol.front() << std::endl);
    list.push_back(sol.front());

    return true;
}

void roadef2018::sol_dpastar_2(DPAStarData d)
{
    LOG_FOLD_START(d.info, "DPA* 2" << std::endl);
    assert(d.ins.stack_number() == 2);

    std::multiset<Solution, SolutionCompare> q(SolutionCompare(2));
    Cpt node_number = 0;
    Cpt q_sizemax = 0;

    // Create history cut object
    std::vector<std::vector<std::vector<Front>>> history;
    for (StackId i=0; i<=(StackId)d.ins.stack(0).size(); ++i)
        history.push_back(std::vector<std::vector<Front>>(d.ins.stack(1).size()+1));

    q.insert(Solution(d.ins));
    while (!q.empty()) {
        node_number++;
        LOG_FOLD_START(d.info, "node " << node_number << std::endl);

        // Get node
        Solution sol(*q.begin());
        q.erase(q.begin());
        LOG_FOLD(d.info, "sol" << std::endl << sol);
        if (q_sizemax < (Cpt)q.size())
            q_sizemax = q.size();

        // If found feasible solution
        if (sol.is_complete()) {
            if (!d.sol_best.is_complete() || sol.waste() < d.sol_best.waste())
                d.sol_best.update(sol, d.info, std::stringstream("DPA*"));
            LOG(d.info, "node number " << node_number << std::endl);
            LOG_FOLD_END(d.info, "");
            LOG_FOLD_END(d.info, "");
            PUT(d.info, "DPA*.Nodes", node_number);
            PUT(d.info, "DPA*.QueueMaxSize", q_sizemax);
            return;
        }

        // Explore neighbours
        for (const Insertion& i: sol.all_valid_insertions(d.info, false)) {
            LOG(d.info, i << std::endl);
            Solution sol_tmp(sol);
            sol_tmp.add_item(i, d.info, false);
            LOG_FOLD(d.info, "sol_tmp" << std::endl << sol_tmp);

            if (d.sol_best.is_complete() && sol_tmp.waste() >= d.sol_best.waste()) {
                LOG(d.info, " waste cut ×" << std::endl);
                continue;
            }

            if (i.j1 != -1 || i.j2 != -1) {
                if (!call_history_2(history, sol_tmp, d.info)) {
                    LOG(d.info, " history cut x" << std::endl);
                    continue;
                }
            }

            q.insert(sol_tmp);
        }

        if (d.sol_best.is_complete() && !q.empty())
            while (std::prev(q.end())->waste() >= d.sol_best.waste())
                q.erase(std::prev(q.end()));

        LOG_FOLD_END(d.info, "");
    }
    LOG_FOLD_END(d.info, "");
}

/******************************************************************************/

bool call_history(
        std::map<std::vector<ItemId>, std::vector<Front>>& history,
        const Solution& sol, Info& info)
{
    auto list = history.insert({sol.pos_stack(), {}}).first;

    // Check if front is dominated
    for (Front f: list->second) {
        if (Solution::dominates(f, sol.front(), sol.instance().global_param())) {
            LOG(info, "history " << sol.pos_stack(0) << " " << sol.pos_stack(1) << " " << sol.front() << std::endl);
            LOG(info, "dominated by " << f << std::endl);
            return false;
        }
    }

    // Remove dominated fronts in list
    for (auto it=list->second.begin(); it != list->second.end();) {
        if (Solution::dominates(sol.front(), *it, sol.instance().global_param())) {
            *it = list->second.back();
            list->second.pop_back();
        } else {
            ++it;
        }
    }

    // Add front
    LOG(info, "history " << sol.pos_stack(0) << " " << sol.pos_stack(1) << " " << sol.front() << std::endl);
    list->second.push_back(sol.front());

    return true;
}

void roadef2018::sol_dpastar(DPAStarData d)
{
    LOG_FOLD_START(d.info, "DPA*" << std::endl);

    std::multiset<Solution, SolutionCompare> q(SolutionCompare(2));
    Cpt node_number = 0;
    Cpt q_sizemax = 0;

    // Create history cut object
    std::map<std::vector<ItemId>, std::vector<Front>> history;

    q.insert(Solution(d.ins));
    while (!q.empty()) {
        node_number++;
        LOG_FOLD_START(d.info, "node " << node_number << std::endl);

        // Get node
        Solution sol(*q.begin());
        q.erase(q.begin());
        LOG_FOLD(d.info, "sol" << std::endl << sol);
        if (q_sizemax < (Cpt)q.size())
            q_sizemax = q.size();

        // If found feasible solution
        if (sol.is_complete()) {
            if (!d.sol_best.is_complete() || sol.waste() < d.sol_best.waste())
                d.sol_best.update(sol, d.info, std::stringstream("DPA*"));
            LOG(d.info, "node number " << node_number << std::endl);
            LOG_FOLD_END(d.info, "");
            LOG_FOLD_END(d.info, "");
            PUT(d.info, "DPA*.Nodes", node_number);
            PUT(d.info, "DPA*.QueueMaxSize", q_sizemax);
            return;
        }

        // Explore neighbours
        for (const Insertion& i: sol.all_valid_insertions(d.info, false)) {
            LOG(d.info, i << std::endl);
            Solution sol_tmp(sol);
            sol_tmp.add_item(i, d.info, false);
            LOG_FOLD(d.info, "sol_tmp" << std::endl << sol_tmp);

            if (d.sol_best.is_complete() && sol_tmp.waste() >= d.sol_best.waste()) {
                LOG(d.info, " waste cut ×" << std::endl);
                continue;
            }

            if (i.j1 != -1 || i.j2 != -1) {
                if (!call_history(history, sol_tmp, d.info)) {
                    LOG(d.info, " history cut x" << std::endl);
                    continue;
                }
            }

            q.insert(sol_tmp);
        }

        if (d.sol_best.is_complete() && !q.empty())
            while (std::prev(q.end())->waste() >= d.sol_best.waste())
                q.erase(std::prev(q.end()));

        while (q.size() > 1000000)
            q.erase(std::prev(q.end()));

        LOG_FOLD_END(d.info, "");
    }
    LOG_FOLD_END(d.info, "");
}

