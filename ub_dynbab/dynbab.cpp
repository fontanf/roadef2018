#include "roadef2018/ub_dynbab/dynbab.hpp"

#include <set>
#include <random>
#include <algorithm>
#include <vector>

using namespace roadef2018;

void roadef2018::sol_dynbab(DynBabData d)
{
    LOG(d.info, LOG_FOLD_START << " dynstar" << std::endl);

    Cpt node_number = 0;
    std::chrono::high_resolution_clock::time_point t1 = d.info.t1;

    for (Cpt q_sizemax=2; ; q_sizemax=q_sizemax*d.growth_factor) {
        if (q_sizemax == (Cpt)(q_sizemax*d.growth_factor))
            q_sizemax++;
        if (q_sizemax > 5000000) {
            break;
        }
        LOG(d.info, LOG_FOLD_START << " q_sizemax " << q_sizemax << std::endl);

        std::multiset<Solution, SolutionCompare> q(SolutionCompare(d.criterion_id));
        q.insert(Solution(d.ins));

        bool end = true;
        while (!q.empty()) {
            std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
            if (time_span.count() > d.time_limit) {
                LOG(d.info, LOG_FOLD_END << std::endl);
                break;
            }
            node_number++;
            LOG(d.info, LOG_FOLD_START << " node_number " << node_number << std::endl);

            while ((Cpt)q.size() > q_sizemax) {
                end = false;
                q.erase(std::prev(q.end()));
            }

            Solution sol_cur(*q.begin());
            q.erase(q.begin());

            LOG(d.info, LOG_FOLD_START << " solution" << std::endl << sol_cur << std::endl << LOG_FOLD_END << std::endl);

            LOG(d.info, "item_number " << sol_cur.item_number() << " / " << d.ins.item_number() << std::endl);
            if (sol_cur.item_number() < 3 * d.ins.item_number() / 4) {
                LOG(d.info, "if" << std::endl);
                for (const Insertion& i: sol_cur.all_valid_insertions(d.info, true)) {
                    LOG(d.info, i << std::endl);
                    Solution sol_tmp(sol_cur);
                    sol_tmp.add_item(i, d.info);
                    if (d.best_solution.is_complete() && sol_tmp.waste() >= d.best_solution.waste()) {
                        LOG(d.info, " waste cut ×" << std::endl);
                        continue;
                    }
                    LOG(d.info, " add" << std::endl);
                    q.insert(sol_tmp);
                }
            } else {
                LOG(d.info, "else" << std::endl);
                std::vector<Insertion> neigh = sol_cur.all_valid_insertions(d.info, true);
                while (!neigh.empty()) {
                    Solution best_cur_sol(sol_cur);
                    bool b = false; // becomes 'true' when a first best_cur_sol is found
                    for (Insertion i: neigh) {
                        LOG(d.info, i << std::endl);
                        Solution sol_tmp(sol_cur);
                        sol_tmp.add_item(i, d.info);

                        LOG(d.info, " waste " << sol_tmp.waste() << " lb_area " << sol_tmp.area());

                        if (d.best_solution.is_complete() && sol_tmp.waste() >= d.best_solution.waste()) {
                            LOG(d.info, " ×" << std::endl);
                            continue;
                        }

                        if (sol_tmp.is_complete()
                                && (!d.best_solution.is_complete()
                                    || sol_tmp.waste() < d.best_solution.waste())) {
                            omp_set_lock(&d.writelock);
                            if (!d.best_solution.is_complete()
                                    || sol_tmp.waste() < d.best_solution.waste())
                                d.best_solution.update(sol_tmp, d.info, d.sol_number,
                                        "dynbab " + std::to_string(d.criterion_id)
                                        + " " + std::to_string(d.growth_factor)
                                        + " " + std::to_string(q_sizemax));
                            omp_unset_lock(&d.writelock);
                        }

                        if ((!b) || (sol_tmp.area() < best_cur_sol.area())) {
                            LOG(d.info, " new best");
                            if (!b) {
                                LOG(d.info, " b = true"); b = true;
                            } else {
                                LOG(d.info, " add previous best");
                                q.insert(best_cur_sol);
                            }
                            best_cur_sol = sol_tmp;

                        } else {
                            LOG(d.info, " add");
                            q.insert(sol_tmp);
                        }
                        LOG(d.info, std::endl);
                    }

                    if (!b)
                        break;

                    sol_cur = best_cur_sol;
                    LOG(d.info, LOG_FOLD_START << " solution" << std::endl << sol_cur << std::endl << LOG_FOLD_END << std::endl);
                    neigh = sol_cur.all_valid_insertions(d.info, true);
                }
            }
            LOG(d.info, LOG_FOLD_END << std::endl);
        }
        if (end)
            return;
        LOG(d.info, LOG_FOLD_END << std::endl);
    }
    LOG(d.info, LOG_FOLD_END << std::endl);
}

