#include "roadef2018/ub_beamsearch/beamsearch.hpp"

using namespace roadef2018;

void roadef2018::sol_beamsearch(BeamSearchData d)
{
    Cpt node_number = 0;
    std::chrono::high_resolution_clock::time_point t1 = d.info.t1;

    for (Cpt B=2; B<100000; B*=d.growing_factor) { // for each Beam size
        std::multiset<Solution, SolutionCompare> nodes(SolutionCompare(d.criterion_id));
        nodes.insert(Solution(d.ins));
        while (nodes.size() > 0) { // for each level of the tree
            // if time over
            std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
            if (time_span.count() > d.time_limit) {
                LOG(d.info, LOG_FOLD_END << std::endl);
                break;
            }

            // compute list of child nodes
            std::multiset<Solution, SolutionCompare> children(SolutionCompare(d.criterion_id));
            for (const Solution& s: nodes) {
                for (const Insertion& i: s.all_valid_insertions(d.info, d.break_symetries)) {
                    Solution sol_tmp(s);
                    sol_tmp.add_item(i, d.info);
                    node_number++;
                    if (d.best_solution.is_complete() && sol_tmp.waste() >= d.best_solution.waste()) {
                        LOG(d.info, " waste cut ×" << std::endl);
                        continue;
                    }
                    if (sol_tmp.is_complete() && (!d.best_solution.is_complete() || sol_tmp.waste() < d.best_solution.waste())) {
                        omp_set_lock(&d.writelock);
                        if (!d.best_solution.is_complete() || sol_tmp.waste() < d.best_solution.waste()) {
                            if (!sol_tmp.check_intersection_defects(d.info) ) {
                                std::cout << "ERROR SOLUTION FOUND\n";
                                d.best_solution.update(sol_tmp, d.info, d.sol_number, "BeamSearchi " + std::to_string(B));
                                exit(1);
                            }
                            d.best_solution.update(sol_tmp, d.info, d.sol_number, "BeamSearchi " + std::to_string(B));
                        }
                        omp_unset_lock(&d.writelock);
                    } else if (!sol_tmp.is_complete()) {
                        children.insert(sol_tmp);
                        while ((Cpt)children.size() > B) { // if children too big, remove one element
                            children.erase(std::prev(children.end()));
                        }
                    }
                }
            }

            // if children size is bigger than B, discard elements
            // while ((Cpt)children.size() > B)
            //     children.erase(std::prev(children.end()));

            nodes.swap(children);
        }
    }
}

