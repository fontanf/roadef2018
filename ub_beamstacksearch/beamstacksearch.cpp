#include "roadef2018/ub_beamstacksearch/beamstacksearch.hpp"

using namespace roadef2018;

void roadef2018::sol_beamstacksearch(BeamStackSearchData d)
{
    std::vector<std::multiset<Solution, SolutionCompare>> stacks;
    stacks.push_back(std::multiset<Solution, SolutionCompare>(SolutionCompare(d.criterion_id)));
    stacks[0].insert(Solution(d.ins));
    for (Cpt depth=0;;) {

        if (!d.info.check_time()) {
            LOG_FOLD_END(d.info, "");
            break;
        }

        while (!stacks[depth].empty() &&
                ((Cpt)stacks.size() <= depth+1 || (Cpt)stacks[depth+1].size() < d.size)) {
            Solution sol_cur(*stacks[depth].begin());
            stacks[depth].erase(stacks[depth].begin());

            for (const Insertion& i: sol_cur.all_valid_insertions(d.info, true)) {
                Solution sol_tmp(sol_cur);
                sol_tmp.add_item(i, d.info);

                if (d.sol_best.is_complete()
                        && sol_tmp.waste() >= d.sol_best.waste())
                    continue;

                if (sol_tmp.is_complete()) {
                    if (!d.sol_best.is_complete() || sol_tmp.waste() < d.sol_best.waste()) {
                        std::stringstream ss;
                        ss << "BSS " << d.criterion_id << " " << d.size;
                        d.sol_best.update(sol_tmp, d.info, ss);
                    }
                } else {
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

