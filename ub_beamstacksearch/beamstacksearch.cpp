#include "roadef2018/ub_beamstacksearch/beamstacksearch.hpp"

using namespace roadef2018;

void roadef2018::sol_beamstacksearch(BeamStackSearchData d)
{
    std::chrono::high_resolution_clock::time_point t1 = d.info.t1;

    std::vector<std::multiset<Solution, SolutionCompare>> stacks;
    stacks.push_back(std::multiset<Solution, SolutionCompare>(SolutionCompare(d.criterion_id)));
    stacks[0].insert(Solution(d.ins));
    for (Cpt depth=0;;) {
        // Check time
        std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
        if (time_span.count() > d.time_limit) {
            LOG(d.info, LOG_FOLD_END << std::endl);
            return;
        }

        while (!stacks[depth].empty() &&
                ((Cpt)stacks.size() <= depth+1 || (Cpt)stacks[depth+1].size() < d.size)) {
            Solution sol_cur(*stacks[depth].begin());
            stacks[depth].erase(stacks[depth].begin());

            //for (ItemId j=0; j<sol_cur.item_number(); ++j)
                //std::cout << sol_cur.item(j).j << " ";
            //std::cout << std::endl;

            for (const Insertion& i: sol_cur.all_valid_insertions(d.info, true)) {
                Solution sol_tmp(sol_cur);
                sol_tmp.add_item(i, d.info);

                if (d.best_solution.is_complete()
                        && sol_tmp.waste() >= d.best_solution.waste())
                    continue;

                if (sol_tmp.is_complete()
                        && (!d.best_solution.is_complete()
                            || sol_tmp.waste() < d.best_solution.waste())) {
                    omp_set_lock(&d.writelock);
                    if (!d.best_solution.is_complete() || sol_tmp.waste() < d.best_solution.waste()) {
                        if (!sol_tmp.check_intersection_defects(d.info) ) {
                            std::cout << "ERROR SOLUTION FOUND\n";
                            d.best_solution.update(sol_tmp, d.info, d.sol_number, "DSS " + std::to_string(d.size));
                            exit(1);
                        }
                        d.best_solution.update(sol_tmp, d.info, d.sol_number,
                                "BSS (" + std::to_string(d.criterion_id) + ", " + std::to_string(d.size) + ")");
                    }
                    omp_unset_lock(&d.writelock);
                }

                if (!sol_tmp.is_complete()) {
                    while ((Cpt)stacks.size() <= depth+2)
                        stacks.push_back(std::multiset<Solution, SolutionCompare>(SolutionCompare(d.criterion_id)));
                    stacks[depth+1].insert(sol_tmp);
                }
            }
        }

        if (depth != 0 && (Cpt)stacks[depth].size() < d.size && !stacks[depth-1].empty()) {
            depth--;
        } else {
            depth++;
        }
    }
}

