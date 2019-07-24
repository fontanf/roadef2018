#include "roadef2018/lib/instance.hpp"
#include "roadef2018/lib/solution.hpp"

#include <string>
#include <iostream>
#include <fstream>
#include <vector>

using namespace roadef2018;

std::ostream& roadef2018::operator<<(std::ostream &os, Coord xy)
{
    os << xy.x << " " << xy.y;
    return os;
}

std::ostream& roadef2018::operator<<(std::ostream &os, Rectangle r)
{
    os << r.w << " " << r.h;
    return os;
}

std::ostream& roadef2018::operator<<(std::ostream &os, const Item& it)
{
    os
        << "id " << it.id
        << " length " << it.rect.w
        << " height " << it.rect.h
        << " stack " << it.stack
        << " sequence " << it.sequence;
    return os;
}

/******************************************************************************/

std::ostream& roadef2018::operator<<(std::ostream &os, const Defect& d)
{
    os
        << "defect_id " << d.id
        << " plate_id " << d.plate_id
        << " x " << d.pos.x
        << " y " << d.pos.y
        << " width " << d.rect.w
        << " height " << d.rect.h;
    return os;
}

/******************************************************************************/

std::ostream& roadef2018::operator<<(std::ostream &os, const GlobalParam& global_param)
{
    os
        << "n " << global_param.nbplates
        << " width " << global_param.platesize.w
        << " height " << global_param.platesize.h
        << " min1cut " << global_param.min1cut
        << " max1cut " << global_param.max1cut
        << " min2cut " << global_param.min2cut
        << " minwaste " << global_param.minwaste;
    return os;
}

/******************************************************************************/

std::vector<std::string> split(std::string line)
{
    std::vector<std::string> v;
    std::stringstream ss(line);
    std::string tmp;
    while (getline(ss, tmp, ';'))
        v.push_back(tmp);
    return v;
}

Instance::Instance(
        std::string batch_filename,
        std::string defects_filename,
        std::string global_param_filename)
{
    std::string tmp;
    std::vector<std::string> line;

    // read batch file
    std::ifstream f_batch(batch_filename);
    getline(f_batch, tmp);
    while (getline(f_batch, tmp)) {
        line = split(tmp);
        Item item = {
            .id       = (ItemId)std::stol(line[0]),
            .rect     = {.w = std::stol(line[1]), .h = std::stol(line[2])},
            .stack    = (StackId)std::stol(line[3]),
            .sequence = (SeqId)std::stol(line[4])};
        items_.push_back(item);
        if (item.stack >= (StackId)batches_.size()) // if item is in a new batch
            batches_.push_back(std::vector<Item>());
        batches_[item.stack].push_back(item); // add this item to its batch
    }

    // read defects file
    plate_defects_.resize(100, std::vector<Defect>());
    std::ifstream f_defects(defects_filename);
    getline(f_defects, tmp);
    while (getline(f_defects, tmp)) {
        line = split(tmp);
        Coord     coord  {.x  = std::stol(line[2]), .y = std::stol(line[3])};
        Rectangle rect   {.w  = std::stol(line[4]), .h = std::stol(line[5])};
        Defect    defect {.id = (DefectId)std::stol(line[0]), .plate_id = (PlateId)std::stol(line[1]), .pos = coord, .rect = rect};
        defects_.push_back(defect);
        plate_defects_[std::stol(line[1])].push_back(defect);
    }

    // read global parameters file
    std::vector<Length> data;
    std::ifstream f_global(global_param_filename);
    getline(f_global, tmp);
    for (int i=0; i<7; ++i) {
        getline(f_global, tmp);
        line = split(tmp);
        data.push_back(std::stol(line[1]));
    }
    Rectangle rect{data.at(1), data.at(2)};
    global_param_ = {
        .nbplates  = (PlateId)data.at(0),
        .platesize = rect,
        .min1cut   = data.at(3),
        .max1cut   = data.at(4),
        .min2cut   = data.at(5),
        .minwaste  = data.at(6)};

    fill_stack_pred();
    compute_item_area();
}

Instance::Instance(
            const std::vector<Item>& items,
            const std::vector<Defect>& defects,
            const GlobalParam& global_param):
    items_(items), defects_(defects), global_param_(global_param)
{
    // Fill plate_defects_
    plate_defects_.resize(100, std::vector<Defect>());
    for (const Defect& d: defects_)
        plate_defects_[d.plate_id].push_back(d);

    // Fill batches_
    StackId stack_max = -1;
    for (const Item& item: items_)
        stack_max = std::max(stack_max, item.stack);
    batches_.resize(stack_max+1, std::vector<Item>());
    for (const Item& item: items_)
        batches_[item.stack].push_back(item);

    compute_item_area();
    fill_stack_pred();
}

Instance::Instance(const Instance& ins)
{
    items_         = ins.items_;
    defects_       = ins.defects_;
    global_param_  = ins.global_param_;
    plate_defects_ = ins.plate_defects_;
    batches_       = ins.batches_;
    item_area_     = ins.item_area_;
    stack_pred_    = ins.stack_pred_;
}

Instance::Instance(const Solution& sol, std::vector<ItemId>& id)
{
    assert(sol.is_complete());
    const Instance& ins = sol.instance();
    id.resize(ins.item_number());
    batches_.push_back({});
    for (ItemId j=0; j<ins.item_number(); ++j) {
        const Item& item = ins.item(sol.item(j).j);
        id[j] = sol.item(j).j;
        Item it = {
            .id       = j,
            .rect     = item.rect,
            .stack    = 0,
            .sequence = static_cast<SeqId>(j+1)
        };
        items_.push_back(it);
        batches_[0].push_back(it);
    }
    defects_       = ins.defects_;
    global_param_  = ins.global_param_;
    plate_defects_ = ins.plate_defects_;
    item_area_     = ins.item_area_;
    stack_pred_ = std::vector<StackId>(stack_number(), -1);
}

void Instance::exchange(ItemId j1, ItemId j2, std::vector<ItemId>& id)
{
    assert(stack_pred(j1) == -1);
    assert(stack_pred(j2) == -1);
    assert(stack_number() == item_number());
    ItemId j_tmp = id[j1];
    id[j1] = id[j2];
    id[j2] = j_tmp;
    Rectangle tmp = items_[j1].rect;
    items_[j1].rect = items_[j2].rect;
    items_[j2].rect = tmp;
}

std::ostream& roadef2018::operator<<(std::ostream &os, const Instance& ins)
{
    os
        << "item number " << ins.item_number()
        << " defect number " << ins.defect_number()
        << " global param: " << std::endl << ins.global_param();

    os << "Items" << std::endl;
    for (const Item& item: ins.items())
        os << item << std::endl;

    os << "Stacks" << std::endl;
    for (StackId i=0; i<ins.stack_number(); ++i) {
        os << "Stack " << i << std::endl;
        for (const Item& item: ins.stack(i))
            os << item << std::endl;
    }

    os << "Defects" << std::endl;
    for (const Defect& defect: ins.defects())
        os << defect << std::endl;

    os << "Defects by plates" << std::endl;
    for (PlateId p=0; p<ins.global_param().nbplates; ++p) {
        os << "Plate " << p << std::endl;
        for (const Defect& defect: ins.defects(p))
            os << defect << std::endl;
    }

    return os;
}

bool Instance::equals(StackId s1, StackId s2)
{
    if (stack_size(s1) != stack_size(s2))
        return false;
    for (ItemId j=0; j<stack_size(s1); ++j) {
        const Item& j1 = item(s1, j);
        const Item& j2 = item(s2, j);
        if (j1.rect.w != j2.rect.w || j1.rect.h != j2.rect.h)
            return false;
    }
    return true;
}

void Instance::fill_stack_pred()
{
    stack_pred_ = std::vector<StackId>(stack_number(), -1);
    for (StackId s2=1; s2<stack_number(); ++s2) {
        for (StackId s1=s2-1; s1>=0; --s1) {
            if (equals(s1, s2)) {
                stack_pred_[s2] = s1;
                break;
            }
        }
    }
}

void Instance::compute_item_area()
{
    item_area_ = 0;
    for (ItemId j=0; j<item_number(); ++j)
        item_area_ += item(j).rect.area();
}

Cpt Instance::state_number() const
{
    Cpt val = 1;
    for (StackId s=0; s<stack_number(); ++s) {
        if (val > LLONG_MAX / (stack_size(s) + 1))
            return LLONG_MAX;
        val *= (stack_size(s) + 1);
    }
    return val;
}

