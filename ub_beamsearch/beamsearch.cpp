#include "roadef2018/ub_beamsearch/beamsearch.hpp"

using namespace roadef2018;

void roadef2018::sol_beamsearch(BeamSearchData d)
{
    Cpt node_number = 0;
    SolutionCompare comp(d.criterion_id);

    for (Cpt q_sizemax=2; ; q_sizemax*=d.growth_factor) { // for each Beam size
        if (q_sizemax == (Cpt)(q_sizemax*d.growth_factor))
            q_sizemax++;

        std::multiset<Solution, SolutionCompare> q(SolutionCompare(d.criterion_id));
        std::multiset<Solution, SolutionCompare> children_2(SolutionCompare(d.criterion_id));
        q.insert(Solution(d.ins));

        bool end = true;
        while (!q.empty()) {

            if (!d.info.check_time()) {
                LOG_FOLD_END(d.info, "");
                break;
            }

            // Compute list of child nodes
            std::multiset<Solution, SolutionCompare> children(SolutionCompare(d.criterion_id));
            children.swap(children_2);
            while (!q.empty()) {
                Solution s(*q.begin());
                q.erase(q.begin());
                for (const Insertion& i: s.all_valid_insertions(d.info, true)) {
                    Solution sol_tmp(s);
                    sol_tmp.add_item(i, d.info);
                    node_number++;

                    if (d.sol_best.is_complete() && sol_tmp.waste() >= d.sol_best.waste()) {
                        LOG(d.info, " waste cut ×" << std::endl);
                        continue;
                    }

                    if (sol_tmp.is_complete()) {
                        if (!d.sol_best.is_complete() || sol_tmp.waste() < d.sol_best.waste()) {
                            std::stringstream ss;
                            ss << "BS " << d.criterion_id << " " << d.growth_factor << " " << q_sizemax;
                            d.sol_best.update(sol_tmp, d.info, ss);
                        }
                    } else if (!sol_tmp.is_complete()) {
                        if (i.j1 == -1 && i.j2 == -1) {
                            if (!q.empty() && comp(sol_tmp, *std::prev(q.end()))) {
                                q.insert(sol_tmp);
                                end = false;
                                q.erase(std::prev(q.end()));
                            }
                        } else if (i.j1 != -1 && i.j2 != -1) {
                            children_2.insert(sol_tmp);
                            if ((Cpt)children_2.size() > q_sizemax) { // if children too big, remove one element
                                end = false;
                                children_2.erase(std::prev(children_2.end()));
                            }
                        } else {
                            children.insert(sol_tmp);
                            if ((Cpt)children.size() > q_sizemax) { // if children too big, remove one element
                                end = false;
                                children.erase(std::prev(children.end()));
                            }
                        }
                    }

                }
            }
            q.swap(children);
        }
        if (end)
            break;
    }
}

