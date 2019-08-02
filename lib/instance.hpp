#pragma once

#include "benchtools/info.hpp"

#include <string>
#include <vector>
#include <exception>
#include <thread>

namespace roadef2018
{

typedef int16_t ItemId;
typedef int16_t ItemPos;
typedef int64_t Length;
typedef int64_t Area;
typedef int16_t StackId;
typedef int16_t SeqId;
typedef int16_t DefectId;
typedef int16_t PlateId;
typedef int64_t Cpt;
typedef int64_t Seed;

class Solution;

struct Coord
{
    Length x;
    Length y;
};

std::ostream& operator<<(std::ostream &os, Coord xy);

struct Rectangle
{
    Length w;
    Length h;

    Area area() const { return w*h; }
};

std::ostream& operator<<(std::ostream &os, Rectangle r);

/******************************************************************************/

struct Item
{
    ItemId id;
    Rectangle rect;
    StackId stack;
    SeqId sequence;

    Length width (bool rotate) const { return (!rotate)? rect.w: rect.h; }
    Length height(bool rotate) const { return (!rotate)? rect.h: rect.w; }
};

std::ostream& operator<<(std::ostream &os, const Item& it);

/******************************************************************************/

struct Defect
{
    DefectId id;
    PlateId plate_id;
    Coord pos;
    Rectangle rect;

    Length left()   const { return pos.x; }
    Length right()  const { return pos.x + rect.w; }
    Length top()    const { return pos.y + rect.h; }
    Length bottom() const { return pos.y; }
};

std::ostream& operator<<(std::ostream &os, const Defect& d);

/******************************************************************************/

struct GlobalParam
{
    PlateId nbplates = 100;
    Rectangle platesize = {6000, 3210};
    Length min1cut = 100;
    Length max1cut = 3500;
    Length min2cut = 100;
    Length minwaste = 20;

    static GlobalParam roadef2018()
    {
        return {
            .nbplates = 100,
            .platesize = {.w = 6000, .h = 3210},
            .min1cut = 100,
            .max1cut = 3500,
            .min2cut = 100,
            .minwaste = 20};
    }
};

std::ostream& operator<<(std::ostream &os, const GlobalParam& global_param);


/******************************************************************************/

class Instance 
{

public:

    /**
     * Create instances from files
     */
    Instance(
            std::string batch_filename,
            std::string defects_filename,
            std::string global_param_filename);

    /**
     * Create instance manually
     */
    Instance(
            const std::vector<Item>& items,
            const std::vector<Defect>& defects,
            const GlobalParam& global_param);

    /**
     * Copy constructor
     */
    Instance(const Instance& ins);

    Instance(const Solution& sol, std::vector<ItemId>& id);
    void exchange(ItemId j1, ItemId j2, std::vector<ItemId>& id);

    /**
     * Getters
     */

    inline const GlobalParam& global_param() const { return global_param_; }

    inline ItemId   item_number()         const { return items_.size(); }
    inline StackId  stack_number()        const { return batches_.size(); }
    inline ItemId   stack_size(StackId s) const { return batches_[s].size(); }
    inline DefectId defect_number()       const { return defects_.size(); }

    inline Area item_area() const { return item_area_; }
    inline Area mean_area() const { return item_area_ / item_number(); }

    inline const Item&   item(ItemId j)               const { return items_[j]; }
    inline const Item&   item(StackId s, ItemPos pos) const { return batches_[s][pos]; }
    inline const Defect& defect(DefectId k)           const { return defects_[k]; }

    inline const std::vector<Item>&              items()             const { return items_; }
    inline const std::vector<Item>&              stack(StackId s)    const { return batches_[s]; }
    inline const std::vector<std::vector<Item>>& stacks()            const { return batches_; }
    inline const std::vector<Defect>&            defects()           const { return defects_; }
    inline const std::vector<Defect>&            defects(PlateId id) const { return plate_defects_[id]; }

    StackId stack_pred(StackId s) const { return stack_pred_[s]; }

    Cpt state_number() const;

private:

    std::vector<Item>   items_;
    std::vector<Defect> defects_;
    GlobalParam         global_param_;

    std::vector<std::vector<Defect>> plate_defects_;
    std::vector<std::vector<Item>>   batches_;

    Area item_area_;


    void compute_item_area();

    /**
     * If stacks s1 < s2 < s3 contain identical items in the same order, then
     * stack_pred[s1] = -1, stack_pred[s2] = s1 and stack_pred[s3] = s2.
     */
    std::vector<StackId> stack_pred_;

    /**
     * Return true iff s1 and s2 contains identical objects in the same order.
     */
    bool equals(StackId s1, StackId s2);

    void fill_stack_pred();

};

std::ostream& operator<<(std::ostream &os, const Instance& ins);

}

