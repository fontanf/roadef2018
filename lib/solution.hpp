#pragma once

#include "roadef2018/lib/instance.hpp"

#include <sstream>

namespace roadef2018
{

/******************************** SolutionNode ********************************/

typedef int16_t SolutionNodeId;

/**
 * Note: shortcuts
 *
 * f father
 * c child
 * d depth
 *
 * l left
 * r right
 * b bottom
 * t top
 *
 * j Item or item id
 * k Defect or defect id
 * p Plate id
 * i Insertion
 *
 */

/**
 * Depth :
 * * -1: root
 * *  0: plate
 * *  1: 1-cut
 * *  2: 2-cut
 * *  3: 3-cut
 * *  4: 4-cut
 */
typedef int16_t Depth;

struct SolutionNode
{
    SolutionNodeId f; // father for 2-cuts and 3-cuts, -plate-1 for 1-cuts
    Length p;         // x      for 1-cuts and 3-cuts, y        for 2-cuts

    bool operator==(const SolutionNode& node) const;
    bool operator!=(const SolutionNode& node) const { return !(*this == node); }
};

std::ostream& operator<<(std::ostream &os, const SolutionNode& node);

/******************************** SolutionItem ********************************/

struct SolutionItem
{
    ItemId j;
    SolutionNodeId node; // Note that two items may belong to the same node.

    bool operator==(const SolutionItem& item) const;
    bool operator!=(const SolutionItem& item) const { return !(*this == item); }
};

std::ostream& operator<<(std::ostream &os, const SolutionItem& node);

/********************************* Insertion **********************************/

struct Insertion
{
    ItemId j1; // item 1
    ItemId j2; // item 2
    Depth  df; // father's depth
    Length x1; // position of the next 1-cut
    Length y2; // position of the next 2-cut
    Length x3; // position of the next 3-cut
    Length x1_max;
    Length y2_max;
    Cpt    z1;
    Cpt    z2;

    bool operator==(const Insertion& i) const;
    bool operator!=(const Insertion& i) const { return !(*this == i); }
};

std::ostream& operator<<(std::ostream &os, const Insertion& ins);
std::ostream& operator<<(std::ostream &os, const std::vector<Insertion>& is);

/**************************** EnhancedSolutionNode ****************************/

/*
 * Structure used for CSV export
 */
struct EnhancedSolutionNode
{
    SolutionNodeId id;
    SolutionNodeId f;
    Depth d;
    PlateId p;
    Length l, r, b, t;
    std::vector<SolutionNodeId> children; 
    ItemId j;
    bool rotate;
};

std::ostream& operator<<(std::ostream &os, const EnhancedSolutionNode& n);

/********************************** CutInfo ***********************************/

struct CutInfo
{
    SolutionNodeId node;
    ItemId n;
    Length l, b, r, t;

    bool operator==(const CutInfo& c) const;
    bool operator!=(const CutInfo& c) const { return !(*this == c); }
};

std::ostream& operator<<(std::ostream &os, const CutInfo& ins);

/*********************************** Front ************************************/

struct Front
{
    PlateId p;
    Length x1_prev, x3_curr, x1_curr;
    Length y2_prev, y2_curr;
    Cpt z1, z2;
};

std::ostream& operator<<(std::ostream &os, const Front& front);

/********************************** Solution **********************************/

struct ItemRotateX
{
    ItemId j;
    bool rotate;
    Length x;
};

class Solution
{

public:

    Solution(const Instance& ins);
    Solution(const Solution& solution);
    Solution& operator=(const Solution& solution);
    ~Solution() { };
    Solution& assign(const Solution& solution, const std::vector<ItemId>& id);

    bool operator==(const std::vector<SolutionNode>& nodes);

    inline const Instance& instance() const { return instance_; }

    inline ItemId  item_number()            const { return items_.size(); }
    inline double  item_percentage()        const { return (double)item_number() / instance().item_number(); }

    inline PlateId plate_number()           const { return plate_number_; }
    inline double  plate_double()           const { return (double)width() / instance().global_param().platesize.w; }

    inline Area    waste()                  const { return waste_; }
    inline double  waste_percentage()       const { return (double)waste() / area(); }
    inline double  waste_ratio()            const { return (double)waste() / item_area(); }
    inline Area    area()                   const { return current_area_; }
    inline double  mean_area()              const { return (double)area() / item_number(); }
    inline double  mean_item_area()         const { return (double)item_area() / item_number(); }
    inline double  mean_remaining_area()    const { return (double)remaining_area() / (instance().item_number() - item_number()); }
    inline double  remaining_area()         const { return instance().item_area() - item_area(); }
    inline Area    item_area()              const { return item_area_; }

    inline Length  width()                  const { return width_; }
    inline Length  lb_width()               const { return lb_width_; }

    inline bool is_complete() const { return item_number() == instance_.item_number(); }

    bool check_symetries(Depth df, Info& info) const;
    std::vector<EnhancedSolutionNode> enhanced_nodes(Info& info) const;
    bool check_intersection_defects(Info& info, const std::vector<EnhancedSolutionNode>& nodes) const;
    static void export_csv(Info& info, const std::vector<EnhancedSolutionNode>& nodes);

    SolutionNodeId add_item(const Insertion& ins, Info& info, bool break_symetries = true);

    void update(const Solution& sol, Info& info, const std::stringstream& algorithm);

    std::string to_string(SolutionNodeId node_id, std::string str="") const;
    std::string branchingviz_string() const;
    std::string branchingviz_string(SolutionNodeId id, std::vector<EnhancedSolutionNode> nodes) const;
    std::string to_string(Insertion ins);

    std::vector<Insertion> all_valid_insertions(Info& info, bool break_symetries = true) const;

    /**
     * For unit tests
     */
    const std::vector<SolutionNode>& nodes() const { return nodes_; }
    const SolutionNode& node(SolutionNodeId id) const { return nodes_[id]; }
    SolutionNodeId node_number() const { return nodes_.size(); }
    const SolutionItem& item(ItemId j) const { return items_[j]; }
    const std::vector<SolutionItem>& items() const { return items_; }
    const std::vector<ItemId>& stacks() const { return pos_stack_; }

    const std::array<CutInfo, 4>& prev_cut() const { return prev_cut_; };
    const std::array<CutInfo, 4>& curr_cut() const { return curr_cut_; };
    inline const CutInfo& prev_cut(Depth d) const { return prev_cut_[d]; };
    inline const CutInfo& curr_cut(Depth d) const { return curr_cut_[d]; };

    inline Length x1_curr() const { return (curr_cut(1).node == -1)? 0: node(curr_cut(1).node).p; }
    inline Length x1_prev() const { return (prev_cut(1).node == -1)? 0: node(prev_cut(1).node).p; }
    inline Length y2_curr() const { return (curr_cut(2).node == -1)? 0: node(curr_cut(2).node).p; }
    inline Length y2_prev() const { return (prev_cut(2).node == -1)? 0: node(prev_cut(2).node).p; }
    inline Length x3_curr() const { return (curr_cut(3).node == -1)? x1_prev(): node(curr_cut(3).node).p; }
    inline Length x3_prev() const { return (prev_cut(3).node == -1)? x1_prev(): node(prev_cut(3).node).p; }
    inline Front front() const { return {.p = static_cast<PlateId>(plate_number()-1), .x1_prev = x1_prev(), .x3_curr = x3_curr(), .x1_curr = x1_curr(), .y2_prev = y2_prev(), .y2_curr = y2_curr(), .z1 = z1(), .z2 = z2()}; }

    inline Length x1_max() const { return x1_max_; }
    inline Length y2_max() const { return y2_max_; }
    inline Cpt z1() const { return z1_; }
    inline Cpt z2() const { return z2_; }

    std::vector<ItemId> pos_stack() const { return pos_stack_; }
    ItemId pos_stack(StackId s) const { return pos_stack_[s]; }

    static bool dominates(Front f1, Front f2, const GlobalParam& global_param);

private:

    /**
     * Attributes
     */

    const Instance& instance_;

    std::vector<SolutionNode> nodes_ = {};

    /**
     * pos_stack_[s] == k iff the solution contains items 0 to k-1 in the
     * sequence of stack s.
     */
    std::vector<ItemId> pos_stack_ = {};

    /**
     * items_[j].j is the jth item of the the solution.
     * items_[j].node is the index of its associated node of nodes_.
     */
    std::vector<SolutionItem> items_ = {};

    PlateId plate_number_ = 0;
    Area item_area_       = 0;
    Area current_area_    = 0;
    Area waste_           = 0;
    Length width_         = 0;
    Length lb_width_      = -1;

    std::array<CutInfo, 4> curr_cut_ {{{.node = -1}, {.node = -1}, {.node = -1}, {.node = -1}}};
    std::array<CutInfo, 4> prev_cut_ {{{.node = -1}, {.node = -1}, {.node = -1}, {.node = -1}}};

    /**
     * x1_max_ is the maximum position of the current 1-cut.
     * Used when otherwise, one of its 2-cut would intersect a defect.
     */
    Length x1_max_ = -1;

    /**
     * y2_max_ is the maximum position of the current 2-cut.
     * Used when otherwise, one of its 3-cut would intersect a defect.
     */
    Length y2_max_ = -1;

    /**
     * z1_
     * * 0: to increase the width of the last 1-cut, it is necessary to add at
     * least the minimum waste.
     * * 1: the width of the last 1-cut can be increased by any value.
     */
    Cpt z1_ = 0;

    /**
     * z2_
     * * 0: to increase the height of the last 2-cut, it is necessary to add at
     * least the minimum waste.
     * * 1: the height of the last 2-cut can be increased by any value.
     * * 2: the height of the last 2-cut cannot be increased (case where it
     * contains of 4-cut with 2 items).
     */
    Cpt z2_ = 0;

    Cpt df_min_ = -1;

    /**
     * Contains the list of items (id, rotation, right cut position) inserted
     * above a defect between the previous and the current 2-cut.
     */
    std::vector<ItemRotateX> yy_ = {};


    /**
     * Private methods
     */

    /**
     * all_valid_insertions
     */
    void update_prev_cuts_and_curr_cuts(Depth df, ItemId n);

    /**
     * Insertion of one item.
     */
    void insertion_1_item(std::vector<Insertion>& res,
            ItemId j, bool rotate, Depth df,
            Depth& df_min, Info& info) const;
    /**
     * Insertion of one item above a defect.
     */
    void insertion_1_item_4cut(std::vector<Insertion>& res,
            DefectId k, ItemId j, bool rotate, Depth df,
            Depth& df_min, Info& info) const;
    /**
     * Insertion of two items.
     */
    void insertion_2_items(std::vector<Insertion>& res,
            ItemId j1, bool rotate1, ItemId j2, bool rotate2, Depth df,
            Depth& df_min, Info& info) const;
    /**
     * Insertion of a defect.
     */
    void insertion_defect(std::vector<Insertion>& res,
            const Defect& k, Depth df, Info& info) const;

    /**
     * Coordinates of the bottom left side of a new insertion at depth df.
     */
    Coord coord(Depth df) const;
    PlateId last_plate(Depth df) const;
    Length x1_prev(Depth df) const;
    Length y2_prev(Depth df) const;
    Length x1_max(Depth df) const;
    Length y2_max(Depth df, Length x3) const;
    Front front(const Insertion& i) const;
    Area waste(const Insertion& i) const;

    /**
     * Intersections
     */
    bool rect_intersection(
            Coord c1, Rectangle r1, Coord c2, Rectangle r2) const;
    DefectId rect_intersects_defect(Info& info,
            Length l, Length r, Length b, Length t, PlateId p) const;
    DefectId x_intersects_defect(
            Length x, PlateId plate) const;
    DefectId y_intersects_defect(
            Length l, Length r, Length y, PlateId plate) const;

    /**
     * Compute i.x1 and i.z1 depending on x3 and x1_curr().
     */
    void insertion_item_update_x1_z1(Info& info, Insertion& i) const;
    void insertion_defect_update_x1_z1(Info& info, Insertion& i) const;

    /**
     * Update i.x1 depending on defect intersections.
     * Return false if infeasible.
     */
    bool compute_width(Info& info, Insertion& i) const;

    /**
     * Compute i.y2 and i.z2 depending on y4 and y2_curr().
     */
    bool insertion_item_update_y2_z2(Info& info, Insertion& i) const;
    bool insertion_2_items_update_y2_z2(Info& info, Insertion& i) const;
    bool insertion_defect_update_y2_z2(Info& info, Insertion& i) const;

    /**
     * Update i.y2 depending on defect intersections.
     * Return false if infeasible.
     */
    bool compute_height(Info& info, Insertion& i) const;

    /**
     * export
     */
    SolutionNodeId add_nodes(
            std::vector<EnhancedSolutionNode>& res,
            Cpt j_pos, SolutionNodeId id) const;

};

std::ostream& operator<<(std::ostream &os, const Solution& solution);

Solution algorithm_end(const Solution& sol, Info& info);

struct SolutionCompare
{
    SolutionCompare(int comparator_id): id(comparator_id) {  }
    double value(const Solution& s);
    bool operator()(const Solution& s1, const Solution& s2);
    int id;
};

}

