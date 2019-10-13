#include "roadef2018/ub_mbastar/mbastar.hpp"

#include <set>

using namespace roadef2018;

void roadef2018::sol_mbastar(MBAStarData d)
{
    LOG_FOLD_START(d.info, "MBA*" << std::endl);
    Cpt node_number = 0;

    SolutionCompare comp(d.criterion_id);
    for (Cpt q_sizemax=2; q_sizemax < 100000000; q_sizemax=q_sizemax*d.growth_factor) {
        if (q_sizemax == (Cpt)(q_sizemax*d.growth_factor))
            q_sizemax++;
        LOG_FOLD_START(d.info, "q_sizemax " << q_sizemax << std::endl);

        std::multiset<Solution, SolutionCompare> q(comp);
        q.insert(Solution(d.ins));

        while (!q.empty()) {
            if (!d.info.check_time()) {
                LOG_FOLD_END(d.info, "");
                goto mbastarend;
            }
            node_number++;
            LOG_FOLD_START(d.info, "node_number " << node_number << std::endl);

            Solution sol_cur(*q.begin());
            q.erase(q.begin());

            LOG_FOLD(d.info, "solution" << std::endl << sol_cur);

            for (const Insertion& i: sol_cur.all_valid_insertions(d.info, true)) {
                LOG(d.info, i << std::endl);
                Solution sol_tmp(sol_cur);
                sol_tmp.add_item(i, d.info);
                //if (d.sol_best.is_complete() && sol_tmp.lb_width() >= d.sol_best.width()) {
                if (d.sol_best.is_complete() && sol_tmp.waste() >= d.sol_best.waste()) {
                    LOG(d.info, " waste cut ×" << std::endl);
                    continue;
                }

                if (sol_tmp.is_complete()) {
                    if (!d.sol_best.is_complete() || sol_tmp.waste() < d.sol_best.waste()) {
                        std::stringstream ss;
                        ss << "MBA* " << d.criterion_id << " " << d.growth_factor << " " << q_sizemax;
                        d.sol_best.update(sol_tmp, d.info, ss);
                    }
                } else {
                    LOG(d.info, " add" << std::endl);
                    if ((Cpt)q.size() < q_sizemax || comp(sol_tmp, *(std::prev(q.end())))) {
                        q.insert(sol_tmp);
                        if ((Cpt)q.size() > q_sizemax)
                            q.erase(std::prev(q.end()));
                    }
                }
            }

            LOG_FOLD_END(d.info, "");
        }
        LOG_FOLD_END(d.info, "");
        std::stringstream ss;
        ss << "MBA* " << d.criterion_id << " " << d.growth_factor;
        PUT(d.info, ss.str(), "QueueMaxSize", q_sizemax);
    }
mbastarend:

    std::stringstream ss;
    ss << "MBA* " << d.criterion_id << " " << d.growth_factor;
    PUT(d.info, ss.str(), "Nodes", node_number);
    LOG_FOLD_END(d.info, "");
}

