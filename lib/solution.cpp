#include "roadef2018/lib/solution.hpp"
#include "roadef2018/lib/instance.hpp"

#include <string>
#include <fstream>
#include <iomanip>
#include <locale>

using namespace roadef2018;

/******************************** SolutionNode ********************************/

bool SolutionNode::operator==(const SolutionNode& node) const
{
    return ((f == node.f) && (p == node.p));
}

std::ostream& roadef2018::operator<<(std::ostream &os, const SolutionNode& node)
{
    os << "f " << node.f << " p " << node.p;
    return os;
}

/******************************** SolutionItem ********************************/

bool SolutionItem::operator==(const SolutionItem& item) const
{
    return ((j == item.j) && (node == item.node));
}

std::ostream& roadef2018::operator<<(std::ostream &os, const SolutionItem& item)
{
    os << "j " << item.j << " node " << item.node;
    return os;
}

/********************************* Insertion **********************************/

bool Insertion::operator==(const Insertion& i) const
{
    return (
               (j1 == i.j1) && (j2 == i.j2) && (df == i.df)
            && (x1 == i.x1) && (y2 == i.y2) && (x3 == i.x3)
            && (x1_max == i.x1_max) && (y2_max == i.y2_max)
            && (z1 == i.z1) && (z2 == i.z2));
}

std::ostream& roadef2018::operator<<(std::ostream &os, const Insertion& i)
{
    os
        << "j1 " << i.j1 << " j2 " << i.j2 << " df " << i.df
        << " x1 " << i.x1 << " y2 " << i.y2 << " x3 " << i.x3
        << " x1_max " << i.x1_max << " y2_max " << i.y2_max
        << " z1 " << i.z1 << " z2 " << i.z2;
    return os;
}

std::ostream& roadef2018::operator<<(std::ostream &os, const std::vector<Insertion>& is)
{
    std::copy(is.begin(), is.end(), std::ostream_iterator<Insertion>(os, "\n"));
    return os;
}

/********************************** CutInfo ***********************************/

bool CutInfo::operator==(const CutInfo& c) const
{
    return ((node == c.node) && (n == c.n)
            && (l == c.l) && (r == c.r)
            && (b == c.b) && (t == c.t));
}

std::ostream& roadef2018::operator<<(std::ostream &os, const CutInfo& c)
{
    os << "node " << c.node << " n " << c.n
        << " l " << c.l << " b " << c.b
        << " r " << c.r << " t " << c.t;
    return os;
}

/*********************************** Front ************************************/

std::ostream& roadef2018::operator<<(std::ostream &os, const Front& front)
{
    os << "p " << front.p
        << " x1_prev " << front.x1_prev << " x3_curr " << front.x3_curr
        << " x1_curr " << front.x1_curr
        << " y2_prev " << front.y2_prev << " y2_curr " << front.y2_curr
        << " z1 " << front.z1 << " z2 " << front.z2;
    return os;
}

/********************************** Solution **********************************/

Solution::Solution(const Instance& ins): instance_(ins)
{
    // Initialize pos_stack_
    for (StackId i=0; i<instance().stack_number(); ++i)
        pos_stack_.push_back(0);
    x1_max_ = instance().global_param().platesize.w;
    y2_max_ = instance().global_param().platesize.h;
    lb_width_ = (instance().item_area()-1) / instance().global_param().platesize.h + 1;
}

Solution::Solution(const Solution& solution):
    instance_(solution.instance_),
    nodes_(solution.nodes_),
    pos_stack_(solution.pos_stack_),
    items_(solution.items_),
    plate_number_(solution.plate_number_),
    item_area_(solution.item_area_),
    current_area_(solution.current_area_),
    waste_(solution.waste_),
    width_(solution.width_),
    lb_width_(solution.lb_width_),
    curr_cut_(solution.curr_cut_),
    prev_cut_(solution.prev_cut_),
    x1_max_(solution.x1_max_),
    y2_max_(solution.y2_max_),
    z1_(solution.z1_),
    z2_(solution.z2_),
    df_min_(solution.df_min_),
    yy_(solution.yy_)
{ }

Solution& Solution::operator=(const Solution& solution)
{
    if (this != &solution) {
        nodes_        = solution.nodes_;
        items_        = solution.items_;
        pos_stack_    = solution.pos_stack_;
        plate_number_ = solution.plate_number_;
        item_area_    = solution.item_area_;
        current_area_ = solution.current_area_;
        waste_        = solution.waste_;
        width_        = solution.width_;
        lb_width_     = solution.lb_width_;
        prev_cut_     = solution.prev_cut_;
        curr_cut_     = solution.curr_cut_;
        x1_max_       = solution.x1_max_;
        y2_max_       = solution.y2_max_;
        z1_           = solution.z1_;
        z2_           = solution.z2_;
        df_min_       = solution.df_min_;
        yy_           = solution.yy_;
    }
    return *this;
}

Solution& Solution::assign(const Solution& sol, const std::vector<ItemId>& id)
{
    assert(sol.is_complete());
    assert(instance().item_number() == sol.item_number());

    nodes_ = sol.nodes_;

    items_.clear();
    for (ItemPos j=0; j<sol.item_number(); ++j) {
        items_.push_back({
                .j = id[j],
                .node = sol.item(j).node,
        });
    }
    pos_stack_ = {};

    plate_number_ = sol.plate_number_;
    item_area_    = sol.item_area_;
    current_area_ = sol.current_area_;
    waste_        = sol.waste_;
    width_        = sol.width_;
    lb_width_     = sol.lb_width_;
    prev_cut_     = sol.prev_cut_;
    curr_cut_     = sol.curr_cut_;
    x1_max_       = sol.x1_max_;
    y2_max_       = sol.y2_max_;
    z1_           = sol.z1_;
    z2_           = sol.z2_;
    df_min_       = sol.df_min_;
    yy_           = sol.yy_;

    return *this;
}

bool Solution::operator==(const std::vector<SolutionNode>& nodes)
{
    if (nodes.size() != nodes_.size())
        return false;
    for (SolutionNodeId node_id=0; node_id<(SolutionNodeId)nodes_.size(); ++node_id)
        if (nodes_[node_id] != nodes[node_id])
            return false;
    return true;
}

std::ostream& roadef2018::operator<<(std::ostream &os, const Solution& solution)
{
    os << "n " << solution.item_number()
        << " m " << solution.plate_number() << std::endl;
    os << "item_area " << solution.item_area()
        << " current_area " << solution.area() << std::endl;
    os
        << "width " << solution.width()
        << " waste " << solution.waste()
        << " lb_width " << solution.lb_width()
        << std::endl;
    os << "x1_max " << solution.x1_max() << " y2_max " << solution.y2_max()
        << " z1 " << solution.z1() << " z2 " << solution.z2() << std::endl;
    os << "pos_stack" << std::flush;
    for (StackId s=0; s<solution.instance().stack_number(); ++s)
        os << " " << solution.pos_stack(s);
    os << std::endl;
    for (Depth d=0; d<=3; ++d) {
        SolutionNodeId idc = solution.curr_cut(d).node;
        os << "curr_cut(" << d << ") node " << idc;
        if (idc != -1)
            os
                << " n " << solution.curr_cut(d).n
                << " p " << solution.node(idc).p;
        os << " - " << solution.curr_cut(d) << std::endl;
        SolutionNodeId idp = solution.prev_cut(d).node;
        os << "prev_cut(" << d << ") node " << idp;
        if (idp != -1)
            os
                << " n " << solution.prev_cut(d).n
                << " p " << solution.node(idp).p;
        os << " - " << solution.prev_cut(d) << std::endl;
    }
    os << std::endl;

    for (SolutionNodeId id=0; id<(SolutionNodeId)solution.nodes().size(); ++id) {
        const SolutionNode& node = solution.node(id);
        os << "id " << id
            << "\tf " << node.f
            << "\tp " << node.p << std::endl;
    }
    os << std::endl;

    for (const SolutionItem& item: solution.items())
        os << "j " << item.j << "\tnode " << item.node << std::endl;

    return os;
}

Solution roadef2018::algorithm_end(const Solution& sol, Info& info)
{
    double t = info.elapsed_time();
    PUT(info, "Solution.Waste", sol.waste());
    PUT(info, "Solution.Time", t);
    VER(info, "---" << std::endl
            << "Waste: " << sol.waste() << std::endl
            << "Waste (%): " << sol.waste_percentage()*100 << std::endl
            << "Width: " << sol.width() << std::endl
            << "Plates: " << sol.plate_double() << std::endl
            << "Time: " << t << std::endl);
    return sol;
}

void Solution::update(const Solution& sol, Info& info, const std::stringstream& algorithm)
{
    info.output->mutex_sol.lock();

    if (!is_complete() || sol.waste() < waste()) {
        const std::vector<EnhancedSolutionNode>& res = sol.enhanced_nodes(info);
        if (!sol.check_intersection_defects(info, res) ) {
            VER(info, "WARNING: unfeasible solution found" << std::endl);
            return;
        }

        info.output->sol_number++;
        *this = sol;
        double t = info.elapsed_time();
        std::string sol_str = "Solution" + std::to_string(info.output->sol_number);
        PUT(info, sol_str + ".Waste", sol.waste());
        PUT(info, sol_str + ".Time", t);
        PUT(info, sol_str + ".Algorithm", algorithm.str());

        VER(info, std::left << std::setw(6) << info.output->sol_number);
        VER(info, std::left << std::setw(22) << algorithm.str());
        VER(info, std::left << std::setw(12) << sol.waste());
        VER(info, std::left << std::setw(11) << 100*sol.waste_percentage());
        VER(info, std::left << std::setw(9) << sol.width());
        VER(info, std::left << std::setw(9) << sol.plate_double());
        VER(info, t << std::endl);

        if (!info.output->onlywriteattheend) {
            info.write_ini();
            export_csv(info, res);
        }
    }

    info.output->mutex_sol.unlock();
}

/********************************** add_item **********************************/

void Solution::update_prev_cuts_and_curr_cuts(Depth df, ItemId n)
{
    switch (df) {
    case -1: {
        prev_cut_[0] = curr_cut(0);
        curr_cut_[0] = {.node = static_cast<SolutionNodeId>(-plate_number()),
            .n = n, .l = 0, .b = 0};
        for (Depth d=1; d<=3; ++d) {
            prev_cut_[d].node = -1;
            curr_cut_[d] = {
                .node = static_cast<SolutionNodeId>(node_number() + d - 1),
                .n = n, .l = 0, .b = 0};
        }
        break;
    } case 0: {
        curr_cut_[0].n += n;
        prev_cut_[1] = curr_cut(1);
        curr_cut_[1] = {.node = node_number(), .n = n, .l = x1_prev(), .b = 0};
        prev_cut_[2].node = -1;
        curr_cut_[2] = {.node = static_cast<SolutionNodeId>(node_number() + 1),
            .n = n, .l = x1_prev(), .b = 0};
        prev_cut_[3].node = -1;
        curr_cut_[3] = {.node = static_cast<SolutionNodeId>(node_number() + 2),
            .n = n, .l = x1_prev(), .b = 0};
        break;
    } case 1: {
        curr_cut_[0].n += n;
        curr_cut_[1].n += n;
        prev_cut_[2] = curr_cut(2);
        curr_cut_[2] = {.node = node_number(), .n = n, .l = x1_prev(), .b = y2_prev()};
        prev_cut_[3].node = -1;
        curr_cut_[3] = {.node = static_cast<SolutionNodeId>(node_number() + 1),
            .n = n, .l = x1_prev(), .b = y2_prev()};
        break;
    } case 2: {
        curr_cut_[0].n += n;
        curr_cut_[1].n += n;
        curr_cut_[2].n += n;
        prev_cut_[3] = curr_cut(3);
        curr_cut_[3] = {.node = node_number(), .n = n, .l = x3_curr(), .b = y2_prev()};
        break;
    } default: {
        assert(false);
        break;
    }
    }
}

SolutionNodeId Solution::add_item(const Insertion& i, Info& info, bool break_symetries)
{
    LOG_FOLD_START(info, "add_item " << i << std::endl);
    assert(i.df == -1 || i.x1 >= x1_curr());

    Length h = instance().global_param().platesize.h;
    Length w = instance().global_param().platesize.w;

    // Update plate_number_
    if (i.df == -1)
        plate_number_++;

    // Update items_, items_area_ and pos_stack_
    ItemId n = 0; // number of added items
    SolutionNodeId id = nodes().size() + 2 - i.df;
    if (i.df == -1)
        id--;
    if (i.j1 != -1) {
        items_.push_back({.j = i.j1, .node = id});
        item_area_ += instance().item(i.j1).rect.area();
        n++;
        pos_stack_[instance().item(i.j1).stack]++;
    }
    if (i.j2 != -1) {
        items_.push_back({.j = i.j2, .node = id});
        item_area_ += instance().item(i.j2).rect.area();
        n++;
        pos_stack_[instance().item(i.j2).stack]++;
    }

    // Update yy_
    if (i.df != 2)
        yy_.clear();
    if (i.j1 == -1 && i.j2 != -1) {
        bool rotate = (i.x3 - coord(i.df).x == instance().item(i.j2).rect.w)? false: true;
        assert(i.x3 - coord(i.df).x == instance().item(i.j2).width(rotate));
        yy_.push_back({.j = i.j2, .rotate = rotate, .x = i.x3});
    }

    // Update df_min_
    // TODO testing needed
    df_min_ = -1;
    if (break_symetries) {
        if (i.j1 == -1 && i.j2 == -1) { // add defect
            if (i.df >= 1 && i.x1 == x1_curr()) {
                df_min_ = 1;
            }
            if (i.df == 2 && i.y2 == y2_curr()) {
                df_min_ = 2;
            }
        } else if (i.j1 == -1 && i.j2 != -1) { // add item above defect
            // If df < 2 and y2 - hj doesn't intersect a defect, then we force to
            // add the next item in the same 2-cut.
            if (i.df < 2) {
                bool rotate = (i.x3 - coord(i.df).x == instance().item(i.j2).rect.w)? false: true;
                assert(i.x3 - coord(i.df).x == instance().item(i.j2).width(rotate));
                Length w = instance().global_param().platesize.w;
                if (y_intersects_defect(i.x3, w,
                            i.y2 - instance().item(i.j2).height(rotate),
                            plate_number()-1) == -1)
                    df_min_ = 2;
            }
        } else if (i.j1 != -1 && i.j2 != -1) { // add 2 items
            // If df < 2 and both y2-hj1 and y2-hj2 intersect a defect, then we
            // force to add the next item in the same 2-cut.
            if (i.df < 2) {
                if (instance().item(i.j1).stack == instance().item(i.j2).stack) {
                    bool rotate1 = (i.x3 - coord(i.df).x == instance().item(i.j1).rect.w)? false: true;
                    assert(i.x3 - coord(i.df).x == instance().item(i.j1).width(rotate1));
                    Length w = instance().global_param().platesize.w;
                    if (y_intersects_defect(i.x3, w,
                                y2_prev(i.df) + instance().item(i.j1).height(rotate1),
                                plate_number()-1) == -1)
                        df_min_ = 2;
                } else {
                    bool rotate1 = (i.x3 - coord(i.df).x == instance().item(i.j1).rect.w)? false: true;
                    bool rotate2 = (i.x3 - coord(i.df).x == instance().item(i.j2).rect.w)? false: true;
                    assert(i.x3 - coord(i.df).x == instance().item(i.j1).width(rotate1));
                    assert(i.x3 - coord(i.df).x == instance().item(i.j2).width(rotate2));
                    Length w = instance().global_param().platesize.w;
                    if (      y_intersects_defect(i.x3, w,
                                i.y2 - instance().item(i.j1).height(rotate1),
                                plate_number()-1) == -1
                            || y_intersects_defect(i.x3, w,
                                i.y2 - instance().item(i.j2).height(rotate2),
                                plate_number()-1) == -1)
                        df_min_ = 2;
                }
            }
        }
    }

    // Update prev_cut_ and curr_cut_
    update_prev_cuts_and_curr_cuts(i.df, n);

    // Update nodes_
    SolutionNodeId f = (i.df == -1)? -plate_number(): curr_cut(i.df).node;
    Depth d = (i.df == -1)? 0: i.df;
    SolutionNodeId c = nodes_.size() - 1;
    do {
        c++;
        nodes_.push_back({.f = f});
        f = c;
        d++;
    } while (d != 3);

    // Update x1_max_, y2_max_, z1_ and z2_
    x1_max_ = i.x1_max;
    y2_max_ = i.y2_max;
    z1_ = i.z1;
    z2_ = i.z2;

    // Update rights and lefts
    curr_cut_[0].r = i.x1;
    if (curr_cut_[0].t < i.y2)
        curr_cut_[0].t = i.y2;
    curr_cut_[1].r = i.x1;
    curr_cut_[1].t = i.y2;
    curr_cut_[2].r = i.x3;
    curr_cut_[2].t = i.y2;
    curr_cut_[3].r = i.x3;
    curr_cut_[3].t = i.y2; // i.y4?
    nodes_[curr_cut(1).node].p = i.x1;
    nodes_[curr_cut(2).node].p = i.y2;
    nodes_[curr_cut(3).node].p = i.x3;

    assert(curr_cut(0).node == -1 || curr_cut(0).l < curr_cut(0).r);
    assert(curr_cut(0).node == -1 || curr_cut(0).r - curr_cut(0).l
            >= instance().global_param().minwaste);
    assert(curr_cut(1).node == -1 || curr_cut(1).l < curr_cut(1).r);
    assert(curr_cut(1).node == -1 || curr_cut(1).r - curr_cut(1).l
            >= instance().global_param().minwaste);
    assert(curr_cut(2).node == -1 || curr_cut(2).l < curr_cut(2).r);
    assert(curr_cut(2).node == -1 || curr_cut(2).r - curr_cut(2).l
            >= instance().global_param().minwaste);
    assert(curr_cut(3).node == -1 || curr_cut(3).l < curr_cut(3).r);
    assert(curr_cut(3).node == -1 || curr_cut(3).r - curr_cut(3).l
            >= instance().global_param().minwaste);

    // Update current_area_, waste_ and width_
    current_area_ = (is_complete())? (w * h * (plate_number() - 1)) + (x1_curr() * h):
        (w * h * (plate_number_ - 1))
        + x1_prev() * h
        + (x1_curr() - x1_prev()) * y2_prev()
        + (x3_curr() - x1_prev()) * (y2_curr() - y2_prev());
    waste_ = current_area_ - item_area_;
    width_ = (plate_number_ - 1) * w + x1_curr();
    lb_width_ = std::max(width_, (waste_ + instance().item_area() - 1) / h + 1);

    LOG_FOLD_END(info, "");
    return c;
}

/**************************** all_valid_insertions ****************************/

std::vector<Insertion> Solution::all_valid_insertions(Info& info, bool break_symetries) const
{
    LOG_FOLD_START(info, "all_valid_insertions" << std::endl);

    std::vector<Insertion> res;

    Depth df_min = df_min_;
    for (Depth df=2; df>=df_min; --df) {
        if (node_number() == 0)
            df = -1;
        LOG(info, "df " << df << std::endl);

        if (break_symetries && !check_symetries(df, info))
            break;

        Coord c = coord(df);
        LOG(info, "x " << c.x << " y " << c.y << std::endl);

        // Try adding an item
        for (StackId s=0; s<instance().stack_number(); ++s) {
            if (pos_stack_[s] == instance().stack_size(s))
                continue;
            StackId sp = instance().stack_pred(s);
            if (sp != -1 && pos_stack_[sp] <= pos_stack_[s])
                continue;

            ItemId j = instance().item(s, pos_stack_[s]).id;
            bool b = instance().item(j).rect.w > instance().item(j).rect.h;
            insertion_1_item(res, j, !b, df, df_min, info);
            insertion_1_item(res, j,  b, df, df_min, info);
            //insertion_1_item(res, j, false, df, df_min, info);
            //insertion_1_item(res, j, true, df, df_min, info);

            // Try adding it with a second item
            LOG(info, "try adding with a second item" << std::endl);
            for (StackId s2=s; s2<instance().stack_number(); ++s2) {
                ItemId j2 = -1;
                if (s2 == s) {
                    if (pos_stack_[s2] + 1 == instance().stack_size(s2))
                        continue;
                    StackId sp2 = instance().stack_pred(s2);
                    if (sp2 != -1 && pos_stack_[sp2] <= pos_stack_[s2])
                        continue;
                    j2 = instance().item(s2, pos_stack_[s2] + 1).id;
                } else {
                    if (pos_stack_[s2] == instance().stack_size(s2))
                        continue;
                    StackId sp2 = instance().stack_pred(s2);
                    if (                    (sp2 == s && pos_stack_[sp2] + 1 <= pos_stack_[s2])
                            || (sp2 != -1 && sp2 != s && pos_stack_[sp2]     <= pos_stack_[s2]))
                        continue;
                    j2 = instance().item(s2, pos_stack_[s2]).id;
                }

                // To break symetries, the item with the smallest id is always
                // placed at the bottom.
                // This doesn't create precedency issues since all the
                // predecessors of an item have smaller id.
                if (j2 < j) {
                    ItemId tmp = j;
                    j = j2;
                    j2 = tmp;
                }
                if (instance().item(j).rect.w == instance().item(j2).rect.w)
                    insertion_2_items(res, j, false, j2, false, df, df_min, info);
                if (instance().item(j).rect.w == instance().item(j2).rect.h)
                    insertion_2_items(res, j, false, j2, true,  df, df_min, info);
                if (instance().item(j).rect.h == instance().item(j2).rect.w)
                    insertion_2_items(res, j, true,  j2, false, df, df_min, info);
                if (instance().item(j).rect.h == instance().item(j2).rect.h)
                    insertion_2_items(res, j, true,  j2, true,  df, df_min, info);
            }
        }

        const std::vector<Defect>& defects = instance().defects(last_plate(df));
        for (ItemId k=0; k<(DefectId)defects.size(); ++k)
            if (defects[k].right() > c.x && defects[k].top() > c.y)
                insertion_defect(res, defects[k], df, info);
    }

    DBG(
        LOG_FOLD_START(info, "res" << std::endl);
        for (const Insertion& i: res)
            LOG(info, i << std::endl);
        LOG_FOLD_END(info, "");
    );

    LOG_FOLD_END(info, "");
    return res;
}

DefectId Solution::rect_intersects_defect(Info& info,
        Length l, Length r, Length b, Length t, PlateId p) const
{
    LOG_FOLD_START(info, "rect_intersects_defects l " << l << " r " << r << " b " << b << " t " << t << " p " << p << std::endl);
    assert(l <= r);
    assert(b <= t);
    for (const Defect& k: instance().defects(p)) {
        Length lk = k.pos.x;
        Length rk = k.pos.x + k.rect.w;
        assert(lk <= rk);
        LOG(info, " k " << k.id << " l " << lk << " r " << rk);
        if (lk >= r)
            continue;
        if (l >= rk)
            continue;
        Length bk = k.pos.y;
        Length tk = k.pos.y + k.rect.h;
        assert(b <= t);
        LOG(info, " b " << bk << " t " << tk);
        if (bk >= t)
            continue;
        if (b >= tk)
            continue;
        LOG_FOLD_END(info, " * ");
        return k.id;
    }
    LOG_FOLD_END(info, " -1 ");
    return -1;
}

bool Solution::dominates(Front f1, Front f2, const GlobalParam& global_param)
{
    if (f1.p < f2.p) return true;
    if (f1.p > f2.p) return false;
    if (f1.z1 == 0 && f1.x1_curr != global_param.platesize.w
            && !(f2.z1 == 0 && f1.x1_curr == f2.x1_curr))
        f1.x1_curr += global_param.minwaste;
    if (f1.z2 == 0 && f2.y2_curr != global_param.platesize.h
            && !(f2.z2 == 0 && f1.y2_curr == f2.y2_curr))
        f1.y2_curr += global_param.minwaste;
    if (f2.y2_curr != global_param.platesize.h && f1.x1_prev > f2.x1_prev) return false;
    if (f1.x1_curr > f2.x1_curr) return false;
    if        (f2.y2_prev <  f1.y2_prev) { if (f1.x1_curr > f2.x3_curr) return false;
    } else if (f2.y2_prev <  f1.y2_curr) { if (f1.x3_curr > f2.x3_curr) return false;
    } else  /* f2.y2_prev <= h */        { if (f1.x1_prev > f2.x3_curr) return false; }
    if        (f2.y2_curr <  f1.y2_prev) { if (f1.x1_curr > f2.x1_prev) return false;
    } else if (f2.y2_curr <  f1.y2_curr) { if (f1.x3_curr > f2.x1_prev) return false;
    } else  /* f2.y2_curr <= h */        { /* if (f1.x1_prev > f2.x1_prev) return false */; }
    return true;
}

Area Solution::waste(const Insertion& i) const
{
    Length w = instance().global_param().platesize.w;
    Length h = instance().global_param().platesize.h;
    PlateId p = (i.df == -1)? plate_number_: plate_number_ - 1;
    Front f = front(i);
    ItemId n = item_number();
    Area item_area = item_area_;
    if (i.j1 != -1) {
        n++;
        item_area += instance().item(i.j1).rect.area();
    }
    if (i.j2 != -1) {
        n++;
        item_area += instance().item(i.j2).rect.area();
    }
    Area current_area = (n == instance().item_number())? (w * h * (p - 1)) + (f.x1_curr * h):
        (w * h * (p - 1))
        + f.x1_prev * h
        + (f.x1_curr - f.x1_prev) * f.y2_prev
        + (f.x3_curr - f.x1_prev) * (f.y2_curr - f.y2_prev);
    return current_area - item_area;
}

bool Solution::check_symetries(Depth df, Info& info) const
{
    LOG_FOLD_START(info, "check_symetries df " << df << std::endl);
    // If we want to open a new d-cut, if the current d-cut and the preivous
    // d-cut don't contain defects, then we compute the lowest index of the
    // items of each cut.
    // If the lowest index of the current cut is smaller than the lowest index
    // of the previous cut and the 2 cuts can be exchanged without violating
    // the precedences, then we don't consider this solution.

    if (plate_number_ == 0) {
        LOG_FOLD_END(info, "no item");
        return true;
    }
    if (prev_cut_[df+1].node == -1) {
        LOG_FOLD_END(info, "no previous " << df+1 << "-cut");
        return true;
    }

    LOG(info, "prev_cut[" << df+1 <<"] " << prev_cut(df+1) << std::endl);
    LOG(info, "curr_cut[" << df+1 <<"] " << curr_cut(df+1) << std::endl);

    // Check defects
    PlateId p = plate_number() - 1;
    switch (df) {
    case -1: {
        LOG_FOLD_END(info, "return true");
        return true;
        // Note: if you want to remove the return, you need to check defect
        // intersections.

        PlateId p0 = plate_number_ - 2;
        Length x1 = 0;
        Length x2 = std::max(prev_cut(0).r, curr_cut(0).r);
        Length y1 = 0;
        Length y2 = std::max(prev_cut(0).t, curr_cut(0).t);

        DefectId k1 = rect_intersects_defect(info, x1, x2, y1, y2, p);
        if (k1 >= 0) {
            LOG_FOLD_END(info, "contains defect");
            return true;
        }
        DefectId k2 = rect_intersects_defect(info, x1, x2, y1, y2, p0);
        if (k2 >= 0) {
            LOG_FOLD_END(info, "contains defect");
            return true;
        }
        break;
    } case 0: {
        LOG_FOLD_END(info, "return true");
        return true;
        // Note: if you want to remove the return, you need to check defect
        // intersections.

        Length x1 = prev_cut_[1].l;
        Length x2 = curr_cut_[1].r;
        Length y1 = 0;
        Length y2 = std::max(prev_cut(1).t, curr_cut(1).t);

        DefectId k = rect_intersects_defect(info, x1, x2, y1, y2, p);
        if (k != -1) {
            LOG_FOLD_END(info, "contains defect");
            return true;
        }
        break;
    } case 1: {
        Length x1 = curr_cut_[2].l;
        Length x2 = std::max(prev_cut(2).r, curr_cut(2).r);
        Length y1 = prev_cut_[2].b;
        Length y2 = curr_cut_[2].t;

        Length y_new = prev_cut(2).b + curr_cut(2).t - curr_cut(2).b;
        Length w = instance().global_param().platesize.w;
        DefectId k0 = y_intersects_defect(curr_cut(2).l, w, y_new, p);
        if (k0 != -1) {
            LOG_FOLD_END(info, "intersects defect");
            return true;
        }

        DefectId k = rect_intersects_defect(info, x1, x2, y1, y2, p);
        if (k != -1) {
            LOG_FOLD_END(info, "contains defect");
            return true;
        }
        break;
    } case 2: {
        Length x1 = prev_cut_[3].l;
        Length x2 = curr_cut_[3].r;
        Length y1 = curr_cut_[3].b;
        Length y2 = std::max(prev_cut(3).t, curr_cut(3).t);

        Length x_new = prev_cut(3).l + curr_cut(3).r - curr_cut(3).l;
        DefectId k0 = x_intersects_defect(x_new, p);
        if (k0 != -1) {
            LOG_FOLD_END(info, "intersects defect");
            return true;
        }

        DefectId k = rect_intersects_defect(info, x1, x2, y1, y2, p);
        if (k != -1) {
            LOG_FOLD_END(info, "contains defect");
            return true;
        }
        break;
    } default: {
        assert(false);
    }
    }

    // Compute min
    ItemId jmin_curr = instance().item_number();
    LOG(info, "curr_cut_[" << df+1 << "]");
    ItemId n1 = curr_cut(df+1).n;
    ItemId n2 = curr_cut(df+1).n + prev_cut(df+1).n;
    assert(n1 != 0);
    assert(n2 > n1);
    assert(n2 <= item_number());
    for (auto item=items().rbegin(); item<items().rbegin()+n1; ++item) {
        LOG(info, " " << item->j);
        if (jmin_curr > item->j)
            jmin_curr = item->j;
    }
    LOG(info, " jmin_curr " << jmin_curr << std::endl);

    ItemId jmin_prev = instance().item_number();
    LOG(info, "prev_cut_[" << df+1 << "]");
    for (auto item=items().rbegin()+n1; item<items().rbegin()+n2; ++item) {
        LOG(info, " " << item->j);
        if (jmin_prev > item->j)
            jmin_prev = item->j;
    }
    LOG(info, " jmin_prev " << jmin_prev << std::endl);

    if (jmin_prev < jmin_curr) {
        LOG_FOLD_END(info, "jmin_prev < jmin_curr");
        return true;
    }

    // Check precedences
    for (auto item_curr=items().rbegin(); item_curr<items().rbegin()+n1;
            ++item_curr) {
        for (auto item_prev=items().rbegin()+n1;
                item_prev<items().rbegin()+n2; ++item_prev) {
            if (item_prev->j < item_curr->j
                    && instance().item(item_prev->j).stack
                    == instance().item(item_curr->j).stack) {
                LOG_FOLD_END(info, item_prev->j << " precedes " << item_curr->j);
                return true;
            }
        }
    }

    LOG_FOLD_END(info, "");
    return false;
}

Coord Solution::coord(Depth df) const
{
    switch (df) {
    case -1: {
        return {0, 0};
    } case 0: {
        return {x1_curr(), 0};
    } case 1: {
        return {x1_prev(), y2_curr()};
    } case 2: {
        return {x3_curr(), y2_prev()};
    } default: {
        assert(false);
        return {-1, -1};
    }
    }
}

PlateId Solution::last_plate(Depth df) const
{
    if (df == -1) {
        return (item_number() == 0)? 0: plate_number();
    } else {
        return plate_number()-1;
    }
}

Front Solution::front(const Insertion& i) const
{
    switch (i.df) {
    case -1: {
        return {.p = last_plate(i.df),
            .x1_prev = 0, .x3_curr = i.x3, .x1_curr = i.x1,
            .y2_prev = 0, .y2_curr = i.y2,
            .z1 = i.z1, .z2 = i.z2};
    } case 0: {
        return {.p = last_plate(i.df),
            .x1_prev = x1_curr(), .x3_curr = i.x3, .x1_curr = i.x1,
            .y2_prev = 0, .y2_curr = i.y2,
            .z1 = i.z1, .z2 = i.z2};
    } case 1: {
        return {.p = last_plate(i.df),
            .x1_prev = x1_prev(), .x3_curr = i.x3, .x1_curr = i.x1,
            .y2_prev = y2_curr(), .y2_curr = i.y2,
            .z1 = i.z1, .z2 = i.z2};
    } case 2: {
        return {.p = last_plate(i.df),
            .x1_prev = x1_prev(), .x3_curr = i.x3, .x1_curr = i.x1,
            .y2_prev = y2_prev(), .y2_curr = i.y2,
            .z1 = i.z1, .z2 = i.z2};
    } default: {
        assert(false);
        return {.p = -1,
            .x1_prev = -1, .x3_curr = -1, .x1_curr = -1,
            .y2_prev = -1, .y2_curr = -1,
            .z1 = -1, .z2 = -1};
    }
    }
}

Length Solution::x1_prev(Depth df) const
{
    switch (df) {
    case -1: {
        return 0;
    } case 0: {
        return x1_curr();
    } case 1: {
        return x1_prev();
    } case 2: {
        return x1_prev();
    } default: {
        assert(false);
        return -1;
    }
    }
}

Length Solution::x1_max(Depth df) const
{
    switch (df) {
    case -1: {
        return std::min(
                coord(df).x + instance().global_param().max1cut,
                instance().global_param().platesize.w);
    } case 0: {
        return std::min(
                coord(df).x + instance().global_param().max1cut,
                instance().global_param().platesize.w);
    } case 1: {
        Length x = x1_max_;
        for (const Defect& k: instance().defects(plate_number()-1))
            if (k.bottom() < y2_curr() && k.top() > y2_curr())
                if (k.left() > x1_prev())
                    if (x > k.left())
                        x = k.left();
        return x;
    } case 2: {
        return x1_max_;
    } default: {
        return -1;
    }
    }
}

Length Solution::y2_prev(Depth df) const
{
    switch (df) {
    case -1: {
        return 0;
    } case 0: {
        return 0;
    } case 1: {
        return y2_curr();
    } case 2: {
        return y2_prev();
    } default: {
        assert(false);
        return -1;
    }
    }
}

Length Solution::y2_max(Depth df, Length x3) const
{
    Length y = (df == 2)? y2_max_: instance().global_param().platesize.h;
    for (const Defect& k: instance().defects(last_plate(df)))
        if (k.left() < x3 && k.right() > x3)
            if (k.bottom() > y2_prev(df))
                if (y > k.bottom())
                    y = k.bottom();
    return y;
}

void Solution::insertion_1_item(std::vector<Insertion>& res,
        ItemId j, bool rotate, Depth df,
        Depth& df_min, Info& info) const
{
    LOG_FOLD_START(info, "insertion_1_item"
            << " j " << j << " rotate " << rotate << " df " << df << std::endl);
    assert(-1 <= df); assert(df <= 3);

    // Check defect intersection
    PlateId p = (df == -1)? plate_number_: plate_number_ - 1;
    Coord   c = coord(df);
    Length  x = c.x + instance().item(j).width(rotate);
    Length  y = c.y + instance().item(j).height(rotate);
    Length  w = instance().global_param().platesize.w;
    Length  h = instance().global_param().platesize.h;
    if (x > w || y > h) {
        LOG_FOLD_END(info, "too wide/high");
        return;
    }

    DefectId k = rect_intersects_defect(info, c.x, x, c.y, y, p);
    if (k >= 0) {
        // Try adding the item above the defect
        LOG_FOLD_END(info, "intersects defect");
        insertion_1_item_4cut(res, k, j, rotate, df, df_min, info);
        return;
    }

    Insertion i {
        .j1 = j, .j2 = -1, .df = df,
        .x1 = x, .y2 = y, .x3 = x,
        .x1_max = x1_max(df), .y2_max = y2_max(df, x), .z1 = 0, .z2 = 0};
    LOG(info, i << std::endl);

    if (df >= 1)
        insertion_item_update_x1_z1(info, i);
    if (df == 2 && !insertion_item_update_y2_z2(info, i))
        return;
    if (!compute_width(info, i))
        return;
    if (!compute_height(info, i))
        return;

    // Check dominance
    for (auto it=res.begin(); it!=res.end();) {
        if ((it->j1 == i.j1 && it->j2 == -1)
                || (it->j1 == -1 && it->j2 == i.j1)) {
            LOG(info, "f_i  " << front(i) << std::endl);
            LOG(info, "f_it " << front(*it) << std::endl);
            if (dominates(front(i), front(*it), instance().global_param())) {
                LOG(info, "dominates " << *it << std::endl);
                *it = res.back();
                res.pop_back();
            } else if (dominates(front(*it), front(i), instance().global_param())) {
                LOG_FOLD_END(info, "dominated by " << *it);
                return;
            } else {
                ++it;
            }
        } else {
            ++it;
        }
    }
    res.push_back(i);

    if (df == 2 && i.x1 == x1_curr() && i.y2 == y2_curr()) {
        df_min = 2;
    } else if (df >= 1 && i.x1 == x1_curr() && df_min < 1) {
        df_min = 1;
    } else if (df_min < 0) {
        df_min = 0;
    }

    LOG_FOLD_END(info, "");
}

void Solution::insertion_1_item_4cut(std::vector<Insertion>& res,
            DefectId k, ItemId j, bool rotate, Depth df,
            Depth& df_min, Info& info) const
{
    LOG_FOLD_START(info, "insertion_1_item_4cut"
            << " k " << k << " j " << j << " rotate " << rotate << " df " << df << std::endl);
    assert(-1 <= df); assert(df <= 3);

    Length minwaste = instance().global_param().minwaste;
    Length w = instance().global_param().platesize.w;
    Length h = instance().global_param().platesize.h;
    Coord  c = coord(df);
    Length x = c.x + instance().item(j).width(rotate);
    Length y = std::max(
            instance().defect(k).top(),
            c.y + minwaste) + instance().item(j).height(rotate);
    LOG(info, "y " << y << std::endl);
    if (x > w || y > h) {
        LOG_FOLD_END(info, "too wide/high");
        return;
    }

    Insertion i {
        .j1 = -1, .j2 = j, .df = df,
        .x1 = x, .y2 = y, .x3 = x,
        .x1_max = x1_max(df), .y2_max = y2_max(df, x), .z1 = 0, .z2 = 1};
    LOG(info, i << std::endl);

    if (df >= 1)
        insertion_item_update_x1_z1(info, i);
    if (df == 2 && !insertion_defect_update_y2_z2(info, i))
        return;
    if (!compute_width(info, i))
        return;
    if (!compute_height(info, i))
        return;

    // Check dominance
    for (auto it=res.begin(); it!=res.end();) {
        if ((it->j1 == i.j2 && it->j2 == -1)
                || (it->j1 == -1 && it->j2 == i.j2)) {
            LOG(info, "f_i  " << front(i) << std::endl);
            LOG(info, "f_it " << front(*it) << std::endl);
            if (dominates(front(i), front(*it), instance().global_param())) {
                LOG(info, "dominates " << *it << std::endl);
                *it = res.back();
                res.pop_back();
            } else if (dominates(front(*it), front(i), instance().global_param())) {
                LOG_FOLD_END(info, "dominated by " << *it);
                return;
            } else {
                ++it;
            }
        } else {
            ++it;
        }
    }

    res.push_back(i);

    if (df == 2 && i.x1 == x1_curr() && i.y2 == y2_curr()) {
        df_min = 2;
    } else if (df >= 1 && i.x1 == x1_curr() && df_min < 1) {
        df_min = 1;
    } else if (df_min < 0) {
        df_min = 0;
    }

    LOG_FOLD_END(info, "");
}

void Solution::insertion_2_items(std::vector<Insertion>& res,
        ItemId j1, bool rotate1, ItemId j2, bool rotate2, Depth df,
        Depth& df_min, Info& info) const
{
    LOG_FOLD_START(info, "insertion_2_items"
            << " j1 " << j1 << " rotate1 " << rotate1
            << " j2 " << j2 << " rotate2 " << rotate2
            << " df " << df << std::endl);
    assert(-1 <= df); assert(df <= 3);

    // Check defect intersection
    PlateId p = (df == -1)? plate_number_: plate_number_ - 1;
    Length  w = instance().global_param().platesize.w;
    Length  h = instance().global_param().platesize.h;
    Coord   c = coord(df);
    Length  x = c.x + instance().item(j1).width(rotate1);
    Length  y = c.y + instance().item(j1).height(rotate1)
                    + instance().item(j2).height(rotate2);
    if (x > w || y > h) {
        LOG_FOLD_END(info, "too wide/high");
        return;
    }
    if (rect_intersects_defect(info, c.x, x, c.y, y, p) >= 0) {
        LOG_FOLD_END(info, "intersects defect");
        return;
    }

    Insertion i {
        .j1 = j1, .j2 = j2, .df = df,
        .x1 = x, .y2 = y, .x3 = x,
        .x1_max = x1_max(df), .y2_max = y2_max(df, x), .z1 = 0, .z2 = 2};
    LOG(info, i << std::endl);

    if (df >= 1)
        insertion_item_update_x1_z1(info, i);
    if (df == 2 && !insertion_2_items_update_y2_z2(info, i))
        return;
    if (!compute_width(info, i))
        return;
    if (!compute_height(info, i))
        return;

    // Check dominance
    for (auto it=res.begin(); it!=res.end();) {
        if (it->j1 == i.j1 && it->j2 == i.j2) {
            LOG(info, "f_i  " << front(i) << std::endl);
            LOG(info, "f_it " << front(*it) << std::endl);
            if (dominates(front(i), front(*it), instance().global_param())) {
                LOG(info, "dominates " << *it << std::endl);
                *it = res.back();
                res.pop_back();
            } else if (dominates(front(*it), front(i), instance().global_param())) {
                LOG_FOLD_END(info, "dominated by " << *it);
                return;
            } else {
                ++it;
            }
        } else {
            ++it;
        }
    }

    res.push_back(i);

    if (df == 2 && i.x1 == x1_curr() && i.y2 == y2_curr()) {
        df_min = 2;
    } else if (df >= 1 && i.x1 == x1_curr() && df_min < 1) {
        df_min = 1;
    } else if (df_min < 0) {
        df_min = 0;
    }

    LOG_FOLD_END(info, "");
}

void Solution::insertion_defect(std::vector<Insertion>& res,
            const Defect& k, Depth df, Info& info) const
{
    LOG_FOLD_START(info, "insertion_defect"
            << " k " << k.id << " df " << df << std::endl);
    assert(-1 <= df); assert(df <= 3);

    // Check defect intersection
    Length minwaste = instance().global_param().minwaste;
    Length w = instance().global_param().platesize.w;
    Length h = instance().global_param().platesize.h;
    Coord  c = coord(df);
    Length x = std::max(k.right(), c.x + minwaste);
    Length y = std::max(k.top(),   c.y + minwaste);
    if (x > w || y > h) {
        LOG_FOLD_END(info, "too wide/high");
        return;
    }

    Insertion i {
        .j1 = -1, .j2 = -1, .df = df,
        .x1 = x, .y2 = y, .x3 = x,
        .x1_max = x1_max(df), .y2_max = y2_max(df, x), .z1 = 1, .z2 = 1};
    LOG(info, i << std::endl);

    if (df >= 1)
        insertion_defect_update_x1_z1(info, i);
    if (df == 2 && !insertion_defect_update_y2_z2(info, i))
        return;
    if (!compute_width(info, i))
        return;
    if (!compute_height(info, i))
        return;

    // Check dominance
    for (auto it=res.begin(); it!=res.end();) {
        if (it->j1 == -1 && it->j2 == -1) {
            LOG(info, "f_i  " << front(i) << std::endl);
            LOG(info, "f_it " << front(*it) << std::endl);
            if (dominates(front(i), front(*it), instance().global_param())) {
                LOG(info, "dominates " << *it << std::endl);
                *it = res.back();
                res.pop_back();
            } else if (dominates(front(*it), front(i), instance().global_param())) {
                LOG_FOLD_END(info, "dominated by " << *it);
                return;
            } else {
                ++it;
            }
        } else {
            ++it;
        }
    }

    res.push_back(i);

    LOG_FOLD_END(info, "");
}

void Solution::insertion_item_update_x1_z1(Info& info, Insertion& i) const
{
    (void)info;
    LOG(info, "i.x3 " << i.x3 << " x1_curr() " << x1_curr() << std::endl);
    Length minwaste = instance().global_param().minwaste;
    if (i.x3 + minwaste <= x1_curr()) {
        i.x1 = x1_curr();
        i.z1 = z1();
    } else if (i.x3 < x1_curr()) { // x - minwaste < i.xk < x
        if (z1() == 0) {
            i.x1 = x1_curr() + minwaste;
            i.z1 = 1;
        } else {
            i.x1 = i.x3 + minwaste;
            i.z1 = 1;
        }
    } else if (i.x3 == x1_curr()) {
        i.z1 = 0;
    } else { // x1_curr() < i.x3
        if (z1() == 0 && i.x3 < x1_curr() + minwaste) {
            i.x1 = i.x3 + minwaste;
            i.z1 = 1;
        } else {
            i.x1 = i.x3;
            i.z1 = 0;
        }
    }
}

void Solution::insertion_defect_update_x1_z1(Info& info, Insertion& i) const
{
    (void)info;
    Length minwaste = instance().global_param().minwaste;
    if (i.x3 <= x1_curr()) {
        i.x1 = x1_curr();
        i.z1 = z1();
    } else { // x1_curr() <= i.x3
        if (z1() == 0 && x1_curr() + minwaste > i.x3) {
            i.x1 = i.x3 + minwaste;
            i.z1 = 1;
        } else { // last_1cut_x_ == 1)
            i.x1 = i.x3;
            i.z1 = 1;
        }
    }
}

bool Solution::insertion_item_update_y2_z2(Info& info, Insertion& i) const
{
    LOG(info, "i.y2 " << i.y2 << " y2_curr() " << y2_curr() << std::endl);
    Length minwaste = instance().global_param().minwaste;
    if (i.y2 + minwaste <= y2_curr()) {
        i.y2 = y2_curr();
        i.z2 = z2();
    } else if (i.y2 < y2_curr()) { // y_curr() - minwaste < i.y4 < y_curr()
        if (z2() == 2) {
            LOG_FOLD_END(info, "too high, y2_curr() " << y2_curr() << " i.y2 " << i.y2 << " i.z2 " << i.z2);
            return false;
        } else if (z2() == 0) {
            i.y2 = y2_curr() + minwaste;
            i.z2 = 1;
        } else { // z2() == 1
            i.y2 = i.y2 + minwaste;
            i.z2 = 1;
        }
    } else if (i.y2 == y2_curr()) {
        if (z2() == 2) {
            i.y2 = y2_curr();
            i.z2 = 2;
        } else {
            i.y2 = y2_curr();
            i.z2 = 0;
        }
        return true;
    } else if (y2_curr() < i.y2 && i.y2 < y2_curr() + minwaste) {
        if (z2() == 2) {
            LOG_FOLD_END(info, "too high, y2_curr() " << y2_curr() << " i.y2 " << i.y2 << " i.z2 " << i.z2);
            return false;
        } else if (z2() == 0) {
            i.y2 = i.y2 + minwaste;
            i.z2 = 1;
        } else { // z2() == 1
            i.y2 = i.y2;
            i.z2 = 0;
        }
    } else { // y2_curr() + minwaste <= i.y2
        if (z2() == 2) {
            LOG_FOLD_END(info, "too high, y2_curr() " << y2_curr() << " i.y2 " << i.y2 << " i.z2 " << i.z2);
            return false;
        } else {
            i.y2 = i.y2;
            i.z2 = 0;
        }
    }
    return true;
}

bool Solution::insertion_2_items_update_y2_z2(Info& info, Insertion& i) const
{
    LOG(info, "i.y2 " << i.y2 << " y2_curr() " << y2_curr() << " z2() " << z2() << std::endl);
    Length minwaste = instance().global_param().minwaste;
    i.z2 = 2;
    if (i.y2 < y2_curr()) {
        LOG_FOLD_END(info, "too high, y2_curr() " << y2_curr() << " i.y2 " << i.y2 << " i.z2 " << i.z2);
        return false;
    } else if (i.y2 == y2_curr()) {
        i.y2 = y2_curr();
    } else if (y2_curr() < i.y2 && i.y2 < y2_curr() + minwaste) {
        if (z2() == 2) {
            LOG_FOLD_END(info, "too high, y2_curr() " << y2_curr() << " i.y2 " << i.y2 << " i.z2 " << i.z2);
            return false;
        } else if (z2() == 0) {
            LOG_FOLD_END(info, "too high, y2_curr() " << y2_curr() << " i.y2 " << i.y2 << " i.z2 " << i.z2);
            return false;
        } else { // z2() == 1
            i.y2 = i.y2;
        }
    } else { // y2_curr() + minwaste <= i.y2
        if (z2() == 2) {
            LOG_FOLD_END(info, "too high, y2_curr() " << y2_curr() << " i.y2 " << i.y2 << " i.z2 " << i.z2);
            return false;
        } else {
            i.y2 = i.y2;
        }
    }
    return true;
}

bool Solution::insertion_defect_update_y2_z2(Info& info, Insertion& i) const
{
    (void)info;
    Length minwaste = instance().global_param().minwaste;
    if (i.y2 <= y2_curr()) {
        i.y2 = y2_curr();
        i.z2 = z2();
    } else if (y2_curr() < i.y2 && i.y2 < y2_curr() + minwaste) {
        if (z2() == 2) {
            LOG_FOLD_END(info, "too high, y2_curr() " << y2_curr() << " i.y2 " << i.y2 << " i.z2 " << i.z2);
            return false;
        } else if (z2() == 0) {
            i.y2 = y2_curr() + minwaste;
            i.z2 = 1;
        } else { // z2() == 1
            i.y2 = i.y2;
            i.z2 = 1;
        }
    } else {
        if (z2() == 2) {
            LOG_FOLD_END(info, "too high, y2_curr() " << y2_curr() << " i.y2 " << i.y2 << " i.z2 " << i.z2);
            return false;
        } else {
            i.y2 = i.y2;
            i.z2 = 1;
        }
    }
    return true;
}

bool Solution::compute_width(Info& info, Insertion& i) const
{
    Length minwaste = instance().global_param().minwaste;
    Length w = instance().global_param().platesize.w;
    PlateId p = (i.df == -1)? plate_number_: plate_number_ - 1;

    // Increase x1 if it intersects a defect.
    for (;;) {
        DefectId k = x_intersects_defect(i.x1, p);
        if (k == -1)
            break;
        i.x1 = (i.z1 == 0)?
            std::max(instance().defect(k).right(), i.x1 + minwaste):
            instance().defect(k).right();
        i.z1 = 1;
    }

    // Increase width if too close from border
    if (i.x1 < w && i.x1 + minwaste > w) {
        if (i.z1 == 1) {
            i.x1 = w;
            i.z1 = 0;
        } else { // i.z1 == 0
            LOG(info, i << std::endl);
            LOG_FOLD_END(info, "too long w - minwaste < i.x1 < w and i.z1 == 0");
            return false;
        }
    }

    // Check max width
    if (i.x1 > i.x1_max) {
        LOG(info, i << std::endl);
        LOG_FOLD_END(info, "too long i.x1 > i.x1_max");
        return false;
    }

    LOG(info, "width OK" << std::endl);
    return true;
}

bool Solution::compute_height(Info& info, Insertion& i) const
{
    Length minwaste = instance().global_param().minwaste;
    Length h = instance().global_param().platesize.h;
    PlateId p = (i.df == -1)? plate_number_: plate_number_ - 1;
    bool y2_fixed = (i.z2 == 2 || (i.df == 2 && z2() == 2));

    for (;;) {
        bool found = false;

        // Increase y2 if it intersects a defect.
        DefectId k = y_intersects_defect(x1_prev(i.df), i.x1, i.y2, p);
        if (k != -1) {
            if (y2_fixed) {
                LOG_FOLD_END(info, "y2_fixed");
                return false;
            }
            i.y2 = (i.z2 == 0)?
                std::max(instance().defect(k).top(), i.y2 + minwaste):
                instance().defect(k).top();
            i.z2 = 1;
            found = true;
        }

        // Increase y2 if an item 'on top of its 3-cut' intersects a defect.
        if (i.df == 2) {
            for (auto jox: yy_) {
                ItemId      j = jox.j;
                bool   rotate = jox.rotate;
                Length      r = jox.x;
                Length      l = r - instance().item(j).width(rotate);
                Length      t = i.y2;
                Length      b = i.y2 - instance().item(j).height(rotate);
                LOG(info, "j " << j << " l " << l << " r " << r << " b " << b << " t " << t << std::endl);
                DefectId k = rect_intersects_defect(info, l, r, b, t, p);
                if (k >= 0) {
                    if (y2_fixed) {
                        LOG_FOLD_END(info, "y2_fixed");
                        return false;
                    }
                    i.y2 = (i.z2 == 0)?
                        std::max(instance().defect(k).top() + instance().item(j).height(rotate),
                                i.y2 + minwaste):
                        instance().defect(k).top() + instance().item(j).height(rotate);
                    i.z2 = 1;
                    found = true;
                }
            }
        }
        if (i.j1 == -1 && i.j2 != -1) {
            Length l = coord(i.df).x;
            Length r = i.x3;
            bool rotate = (r - l == instance().item(i.j2).rect.w)? false: true;
            assert(r - l == instance().item(i.j2).width(rotate));
            Length t = i.y2;
            Length b = i.y2 - instance().item(i.j2).height(rotate);
            DefectId k = rect_intersects_defect(info, l, r, b, t, p);
            if (k >= 0) {
                if (y2_fixed) {
                    LOG_FOLD_END(info, "y2_fixed");
                    return false;
                }
                i.y2 = (i.z2 == 0)?
                    std::max(instance().defect(k).top()
                               + instance().item(i.j2).height(rotate),
                             i.y2 + minwaste):
                    instance().defect(k).top()
                        + instance().item(i.j2).height(rotate);
                i.z2 = 1;
                found = true;
            }
        }
        LOG(info, "found " << found << std::endl);
        if (!found)
            break;
    }

    // Now check plate's height
    if (i.y2 < h && i.y2 + minwaste > h) {
        if (i.z2 == 1) {
            i.y2 = h;
            i.z2 = 0;

            if (i.df == 2) {
                for (auto jox: yy_) {
                    ItemId      j = jox.j;
                    bool   rotate = jox.rotate;
                    Length      r = jox.x;
                    Length      l = r - instance().item(j).width(rotate);
                    Length      t = i.y2;
                    Length      b = i.y2 - instance().item(j).height(rotate);
                    DefectId k = rect_intersects_defect(info, l, r, b, t, p);
                    if (k >= 0) {
                        LOG_FOLD_END(info, "too high");
                        return false;
                    }
                }
            }

            if (i.j1 == -1 && i.j2 != -1) {
                Length l = coord(i.df).x;
                Length r = i.x3;
                bool rotate = (r - l == instance().item(i.j2).rect.w)? false: true;
                assert(r - l == instance().item(i.j2).width(rotate));
                Length t = i.y2;
                Length b = i.y2 - instance().item(i.j2).height(rotate);
                DefectId k = rect_intersects_defect(info, l, r, b, t, p);
                if (k >= 0) {
                    LOG_FOLD_END(info, "too high");
                    return false;
                }
            }

        } else { // i.z2 == 0 or i.z2 == 2
            LOG(info, i << std::endl);
            LOG_FOLD_END(info, "too high");
            return false;
        }
    }

    if (i.y2 > i.y2_max) {
        LOG(info, i << std::endl);
        LOG_FOLD_END(info, "too high");
        return false;
    }

    LOG(info, "height OK" << std::endl);
    return true;
}

DefectId Solution::y_intersects_defect(
        Length l, Length r, Length y, PlateId plate) const
{
    DefectId k_min = -1;
    for (const Defect& k: instance().defects(plate)) {
        if (k.right() <= l || k.left() >= r)
            continue;
        if (k.bottom() >= y || k.top() <= y)
            continue;
        if (k_min == -1 || k.left() < instance().defect(k_min).left())
            k_min = k.id;
    }
    return k_min;
}

DefectId Solution::x_intersects_defect(Length x, PlateId plate) const
{
    for (const Defect& k: instance().defects(plate))
        if (k.left() < x && k.right() > x)
            return k.id;
    return -1;
}

/*********************************** export ***********************************/

std::ostream& roadef2018::operator<<(std::ostream &os, const EnhancedSolutionNode& n)
{
    os
        << "id " << n.id
        << " f " << n.f
        << " d " << n.d
        << " p " << n.p
        << " l " << n.l
        << " r " << n.r
        << " b " << n.b
        << " t " << n.t
        << " j " << n.j
        << " rotate " << n.rotate;
    return os;
}

std::ostream& print(std::ostream& os,
        const std::vector<EnhancedSolutionNode>& res,
        SolutionNodeId id, std::string tab)
{
    os << tab << res[id] << std::endl;
    for (SolutionNodeId c: res[id].children)
        print(os, res, c, tab + "  ");
    return os;
}

bool empty(const std::vector<EnhancedSolutionNode>& v, SolutionNodeId f_v)
{
    if (v[f_v].children.size() == 0)
        return (v[f_v].j < 0);
    for (SolutionNodeId c: v[f_v].children)
        if (!empty(v, c))
            return false;
    return true;
}

SolutionNodeId sort(std::vector<EnhancedSolutionNode>& res,
        const std::vector<EnhancedSolutionNode> v,
        SolutionNodeId gf_res, SolutionNodeId f_v)
{
    SolutionNodeId id_res = res.size();
    res.push_back(v[f_v]);
    res[id_res].id = id_res;
    res[id_res].f  = gf_res;
    res[id_res].children = {};
    if (empty(v, f_v)) {
        res[id_res].j = -1;
    } else {
        for (SolutionNodeId c_v: v[f_v].children) {
            SolutionNodeId c_id_res = sort(res, v, id_res, c_v);
            res[id_res].children.push_back(c_id_res);
        }
    }
    return id_res;
}

std::vector<EnhancedSolutionNode> Solution::enhanced_nodes(Info& info) const
{
    assert(is_complete());
    Length w = instance().global_param().platesize.w;
    Length h = instance().global_param().platesize.h;

    std::vector<SolutionNodeId> plates;
    std::vector<EnhancedSolutionNode> res(nodes().size());
    for (PlateId p=0; p<plate_number(); ++p) {
        SolutionNodeId id = res.size();
        plates.push_back(id);
        res.push_back({.id = id, .f = -1, .d = 0, .p = p,
                .l = 0, .r = w, .b = 0, .t = h, .children = {}, .j = -1});
    }
    for (SolutionNodeId id=0; id<(SolutionNodeId)nodes().size(); ++id) {
        SolutionNodeId f = (node(id).f >= 0)? node(id).f: plates[(-node(id).f)-1];
        Depth d = res[f].d + 1;
        res[id].id = id;
        res[id].f  = f;
        res[id].d  = d;
        res[id].p  = res[f].p;
        if (d == 1 || d == 3) {
            res[id].r  = node(id).p;
            res[id].l  = (res[f].children.size() == 0)?
                res[f].l:
                res[res[f].children.back()].r;
            res[id].b  = res[f].b;
            res[id].t  = res[f].t;
        } else { // d == 2
            res[id].t  = node(id).p;
            res[id].b  = (res[f].children.size() == 0)?
                res[f].b:
                res[res[f].children.back()].t;
            res[id].l  = res[f].l;
            res[id].r  = res[f].r;
        }
        res[f].children.push_back(id);
    }

    for (SolutionNodeId f=0; f<(SolutionNodeId)(nodes().size()+plates.size()); ++f) {
        SolutionNodeId nb = res[f].children.size();
        if (nb == 0)
            continue;
        SolutionNodeId c_last = res[f].children.back();
        if ((res[f].d == 0 || res[f].d == 2) && res[f].r != res[c_last].r) {
            if (res[f].r - res[c_last].r < instance().global_param().minwaste) {
                res[c_last].r = res[f].r;
            } else {
                SolutionNodeId id = res.size();
                res.push_back({.id = id, .f = f,
                        .d = static_cast<Depth>(res[f].d+1), .p = res[f].p,
                        .l = res[c_last].r, .r = res[f].r,
                        .b = res[f].b,      .t = res[f].t,
                        .children = {}, .j = -1});
                res[f].children.push_back(id);
            }
        } else if ((res[f].d == 1 || res[f].d == 3) && res[f].t != res[c_last].t) {
            if (res[f].t - res[c_last].t < instance().global_param().minwaste) {
                res[c_last].t = res[f].t;
            } else {
                SolutionNodeId id = res.size();
                res.push_back({.id = id, .f = f,
                        .d = static_cast<Depth>(res[f].d+1), .p = res[f].p,
                        .l = res[f].l,      .r = res[f].r,
                        .b = res[c_last].t, .t = res[f].t,
                        .children = {}, .j = -1});
                res[f].children.push_back(id);
            }
        }
    }

    for (SolutionNodeId id=0; id<(SolutionNodeId)res.size(); ++id)
        res[id].j  = -1;

    for (Cpt j_pos=0; j_pos<item_number(); ++j_pos) {
        ItemId         j  = items_[j_pos].j;
        SolutionNodeId id = items_[j_pos].node;
        Length         wj = instance().item(j).rect.w;
        Length         hj = instance().item(j).rect.h;
        if (res[id].children.size() > 0) { // Second item of the 4-cut
            res[res[id].children[1]].j = j; // Alone in the 3-cut
        } else if ((res[id].t - res[id].b == hj && res[id].r - res[id].l == wj)
                || (res[id].t - res[id].b == wj && res[id].r - res[id].l == hj)) {
            res[id].j = j;
            continue;
        } else {
            Length t = (res[id].r - res[id].l == wj)? hj: wj;
            DefectId k = rect_intersects_defect(info,
                    res[id].l, res[id].r, res[id].b, res[id].b + t, res[id].p);
            if (k == -1) { // First item of a 4-cut.
                SolutionNodeId c1 = res.size();
                res.push_back({.id = c1, .f = id,
                        .d = static_cast<Depth>(res[id].d+1), .p = res[id].p,
                        .l = res[id].l, .r = res[id].r,
                        .b = res[id].b, .t = res[id].b + t,
                        .children = {}, .j = j});
                res[id].children.push_back(c1);
                SolutionNodeId c2 = res.size();
                res.push_back({.id = c2, .f = id,
                        .d = static_cast<Depth>(res[id].d+1), .p = res[id].p,
                        .l = res[id].l, .r = res[id].r,
                        .b = res[id].b + t, .t = res[id].t,
                        .children = {}, .j = -1});
                res[id].children.push_back(c2);
            } else {
                SolutionNodeId c1 = res.size();
                res.push_back({.id = c1, .f = id,
                        .d = static_cast<Depth>(res[id].d+1), .p = res[id].p,
                        .l = res[id].l, .r = res[id].r,
                        .b = res[id].b, .t = res[id].t - t,
                        .children = {}, .j = -1});
                res[id].children.push_back(c1);
                SolutionNodeId c2 = res.size();
                res.push_back({.id = c2, .f = id,
                        .d = static_cast<Depth>(res[id].d+1), .p = res[id].p,
                        .l = res[id].l, .r = res[id].r,
                        .b = res[id].t - t, .t = res[id].t,
                        .children = {}, .j = j});
                res[id].children.push_back(c2);
            }
        }
    }

    // Set j to -2 for intermediate nodes and to -3 for residual
    for (EnhancedSolutionNode& n: res)
        if (n.j == -1 && n.children.size() != 0)
            n.j = -2;

    // Sort nodes
    std::vector<EnhancedSolutionNode> res2;
    for (SolutionNodeId c: plates)
        sort(res2, res, -1, c);

    if (res2.rbegin()->j == -1)
        res2.rbegin()->j = -3;

    //print(std::cout, res2, 0, "");
    return res2;
}

bool Solution::rect_intersection(Coord c1, Rectangle r1, Coord c2, Rectangle r2) const
{
    return c1.x + r1.w > c2.x
        && c2.x + r2.w > c1.x
        && c1.y + r1.h > c2.y
        && c2.y + r2.h > c1.y;
}

bool Solution::check_intersection_defects(Info& info,
        const std::vector<EnhancedSolutionNode>& nodes) const
{
    std::vector<bool> items(instance().item_number(), false);

    for (EnhancedSolutionNode n: nodes) {
        // TODO check tree consistency

        // check cut intersects defect
        for (Defect d: instance().defects(n.p)) { // for each defect in the plate
            if (
                       (n.l > d.pos.x && n.l < d.pos.x+d.rect.w && n.b < d.pos.y+d.rect.h && n.t > d.pos.y)
                    || (n.r > d.pos.x && n.r < d.pos.x+d.rect.w && n.b < d.pos.y+d.rect.h && n.t > d.pos.y)
                    || (n.b > d.pos.y && n.b < d.pos.y+d.rect.h && n.l < d.pos.x+d.rect.w && n.r > d.pos.x)
                    || (n.t > d.pos.y && n.t < d.pos.y+d.rect.h && n.l < d.pos.x+d.rect.w && n.r > d.pos.x)) {
                VER(info, "Node " << n << " cut intersects defect " << d << std::endl);
                return false;
            }
        }

        if (n.j >= 0) {
            // TODO check item order
            // check that there is not more than one copy of the item
            if (items[n.j]) {
                VER(info, "Item " << n.j << " produced more that one time" << std::endl);
                return false;
            }
            items[n.j] = true;

            // check if item intersects defect
            for (Defect d: instance().defects(n.p)) { // for each defect in the plate
                if (rect_intersection({.x = n.l, .y = n.b}, {.w = n.r-n.l, .h = n.t-n.b}, d.pos, d.rect)) {
                    VER(info, "Node " << n << " intersects defect " << d << std::endl);
                    return false;
                }
            }

        } else if (n.j == -1) {
            // check minimum waste constraint
            if (
                       n.r - n.l < instance().global_param().minwaste
                    || n.t - n.b < instance().global_param().minwaste) {
                VER(info, "Node " << n << " violates minwaste constraint" << std::endl);
                return false;
            }
        }

        if (n.d == 0) {
             if (
                           n.l != 0 || n.r != instance().global_param().platesize.w
                        || n.b != 0 || n.t != instance().global_param().platesize.h) {
                 VER(info, "Node " << n << " incorrect dimensions" << std::endl);
                 return false;
             }
         } else if (n.d == 1 && n.j != -1 && n.j != -3) {
             if (n.r - n.l < instance().global_param().min1cut) {
                 VER(info, "Node " << n << " violates min1cut constraint" << std::endl);
                 return false;
             }
             if (n.r - n.l > instance().global_param().max1cut) {
                 VER(info, "Node " << n << " violates max1cut constraint" << std::endl);
                 return false;
             }
         } else if (n.d == 2 && n.j != -1) {
             if (n.t - n.b < instance().global_param().min2cut) {
                 VER(info, "Node " << n << " violates min2cut constraint" << std::endl);
                 return false;
             }
         }
    }

    // check that all items are produced
    for (ItemId j=0; j<instance().item_number(); ++j) {
        if (!items[j]) {
            VER(info, "Item " << j << " has not been produced" << std::endl);
            return false;
        }
    }

    return true;
}

void Solution::export_csv(Info& info,
        const std::vector<EnhancedSolutionNode>& nodes)
{
    std::ofstream f{info.output->certfile};
    f << "PLATE_ID;NODE_ID;X;Y;WIDTH;HEIGHT;TYPE;CUT;PARENT\n";
    for (const EnhancedSolutionNode& n: nodes) {
        f
            << n.p << ";"
            << n.id << ";"
            << n.l << ";"
            << n.b << ";"
            << n.r - n.l << ";"
            << n.t - n.b << ";"
            << n.j << ";"
            << n.d << ";";
        if (n.f != -1)
            f << n.f;
        f << std::endl;
    }
    f.close();
}

/*********************************** Compare **********************************/

double SolutionCompare::value(const Solution& s)
{
    switch(id) {
    case 0:
        if (s.area() == 0)
            return 0;
        return s.waste_percentage();
    case 1:
        if (s.area() == 0)
            return 0;
        if (s.item_number() == 0)
            return 1;
        return s.waste_percentage() / s.mean_item_area();
    case 2:
        return s.waste();
    case 3:
        if (s.area() == 0)
            return 0;
        return s.waste_ratio();
    case 4:
        if (s.area() == 0)
            return 0;
        if (s.item_number() == 0)
            return 1;
        return s.waste_ratio() / s.mean_item_area();
    }
    return 0;
}

bool SolutionCompare::operator()(const Solution& s1, const Solution& s2)
{
    return value(s1) < value(s2);
}

