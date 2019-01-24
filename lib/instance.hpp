/**
 * Contains instance data structures and functions to parse instances.
 */

#pragma once

#include "info.hpp"

#include <string>
#include <vector>
#include <exception>

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/program_options.hpp>

namespace roadef2018
{

typedef int_fast64_t ItemId;
typedef int_fast64_t ItemPos;
typedef int_fast64_t Length;
typedef int_fast64_t Area;
typedef int_fast64_t StackId;
typedef int_fast64_t Sequence;
typedef int_fast64_t DefectId;
typedef int_fast64_t PlateId;
typedef int_fast64_t NbPlates;
typedef int_fast64_t NodeId;
typedef int_fast64_t NodeType;
typedef int_fast64_t Cpt;
typedef int_fast64_t PositionInStack;
typedef int_fast64_t Seed;

enum Orientation { InLength, InHeight };

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

    void rotate() { std::swap(w, h); }
    Rectangle rotate() const { return {h, w}; }
    Area area() const { return w*h; }
    bool in_length() const { return w >= h; }
};

std::ostream& operator<<(std::ostream &os, Rectangle r);

/******************************************************************************/

struct Item
{
    ItemId id;
    Rectangle rect;
    StackId stack;
    Sequence sequence;

    Length length() const { return std::max(rect.w, rect.h); }
    Length width()  const { return std::min(rect.w, rect.h); }

    Length width(Orientation o) const
    {
        return (o == InLength)?
            std::max(rect.w, rect.h):
            std::min(rect.w, rect.h);
    }

    Length height(Orientation o) const
    {
        return (o == InLength)?
            std::min(rect.w, rect.h):
            std::max(rect.w, rect.h);
    }

    std::string to_string() const;
};

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

    std::string to_string() const;
};

/******************************************************************************/

struct GlobalParam
{
    NbPlates nbplates;
    Rectangle platesize;
    Length min1cut;
    Length max1cut;
    Length min2cut;
    Length minwaste;

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

    std::string to_string() const;
};


class FileNotFoundException: public std::exception
{
  virtual const char* what() const throw()
  {
    return "File not found exception";
  }
};


/******************************************************************************/

/**
 * Instance class
 *
 * Constructor is given file names (items file, defects file and global data file). 
 * It parses the files and creates vectors of Items and Defects and fills a Global Param object.
 */
class Instance 
{

public:

    /**
     * Create instances from files.
     */
    Instance(
            std::string batch_filename,
            std::string defects_filename,
            std::string global_param_filename);

    /**
     * Create instance manually.
     */
    Instance(
            const std::vector<Item>& items,
            const std::vector<Defect>& defects,
            const GlobalParam& global_param);

    /**
     * Copy constructor.
     */
    Instance(const Instance& ins);

    /**
     * Getters
     */

    inline const GlobalParam& global_param() const { return global_param_; };

    inline ItemId             item_number()  const { return items_.size(); }
    inline Area               item_surface() const { return item_surface_; }
    inline Area               mean_area()    const { return item_surface_ / item_number(); }
    inline const Item&        item(ItemId j) const { return items_[j]; }

    inline StackId            stack_number()               const { return batches_.size(); }
    inline ItemId             stack_size(StackId s)        const { return batches_[s].size(); }
    inline const Item&        item(StackId s, ItemPos pos) const { return batches_[s][pos]; }

    inline DefectId           defect_number()    const { return defects_.size(); }
    inline const Defect&      defect(DefectId k) const { return defects_[k]; }

    inline const std::vector<Item>&   stack(StackId s)    const { return batches_[s]; }
    inline const std::vector<Defect>& defects(PlateId id) const { return plate_defects_[id]; }

    inline const std::vector<Defect>& all_defects() const { return defects_; }

    StackId stack_pred(StackId s) const { return stack_pred_[s]; }

    std::string to_string() const;

private:

    std::vector<Item>   items_;
    std::vector<Defect> defects_;
    GlobalParam         global_param_;

    std::vector<std::vector<Defect>> plate_defects_;
    std::vector<std::vector<Item>>   batches_;

    Area item_surface_;


    void compute_item_surface();

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

}

