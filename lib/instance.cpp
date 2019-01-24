#include "roadef2018/lib/instance.hpp"

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <boost/algorithm/string.hpp>

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

std::string Item::to_string() const
{
    return "item id: " + std::to_string(this->id)
        + "\tdim: "+std::to_string(this->rect.w) + "," + std::to_string(this->rect.h)
        + "\tpos: "+std::to_string(this->sequence) + " in stack " +std::to_string(this->stack);
}

/******************************************************************************/

std::string Defect::to_string() const
{
    return "defect id: " + std::to_string(this->id)
        + "\ton plate " + std::to_string(this->plate_id)
        + "\tat pos (" + std::to_string(this->pos.x) + "," + std::to_string(this->pos.y)
        + ")\tsize (" + std::to_string(this->rect.w) + "," + std::to_string(this->rect.h) + ")";
}

/******************************************************************************/

std::string GlobalParam::to_string() const
{
    return std::to_string(this->nbplates) + " plates of size ("
        + std::to_string(this->platesize.w) + "," + std::to_string(this->platesize.h)
        + ") min1cut:" + std::to_string(this->min1cut)
        + ", max1cut:" + std::to_string(this->max1cut)
        + ", min2cut:" + std::to_string(this->min2cut)
        + ", minwaste:" + std::to_string(this->minwaste);
}

/******************************************************************************/

Instance::Instance(
        std::string batch_filename,
        std::string defects_filename,
        std::string global_param_filename)
{
    Logger logger("", true);
    Info info(logger);

    // read batch file
    std::ifstream f_batch {batch_filename};
    std::string tmp;
    std::vector<std::string> line;
    getline(f_batch, tmp);
    if (!f_batch) {
        LOG(info, "ERROR: I CANNOT OPEN " << batch_filename << std::endl);
        throw FileNotFoundException();
    }
    while (f_batch.good()) {
        getline(f_batch,tmp);
        boost::split(line, tmp, [](char c) {return c == ';';});
        if (line.size() == 5) {
            Item item = {
                .id       = std::stoi(line[0]),
                .rect     = {.w = std::stoi(line[1]), .h = std::stoi(line[2])},
                .stack    = std::stoi(line[3]),
                .sequence = std::stoi(line[4])};
            this->items_.push_back(item);
            if (item.stack >= (StackId)batches_.size()) { // if item is in a new batch
                batches_.push_back(std::vector<Item>());
            }
            batches_[item.stack].push_back(item); // add this item to its batch
        }
    }

    // read defects file
    plate_defects_.resize(100, std::vector<Defect>());
    std::ifstream f_defects {defects_filename};

    if (!f_defects) {
        LOG(info, "ERROR: I CANNOT OPEN " << defects_filename << std::endl);
        throw FileNotFoundException();
    }

    getline(f_defects, tmp);
    while (f_defects.good()) {
        getline(f_defects, tmp);
        boost::split(line, tmp, [](char c) {return c == ';';});
        if (line.size() == 6) {
            Coord     coord  {.x  = std::stoi(line[2]), .y        = std::stoi(line[3])};
            Rectangle rect   {.w  = std::stoi(line[4]), .h        = std::stoi(line[5])};
            Defect    defect {.id = std::stoi(line[0]), .plate_id = std::stoi(line[1]),
                             .pos = coord, .rect = rect};
            this->defects_.push_back(defect);
            plate_defects_[std::stoi(line[1])].push_back(defect);
        }
    }

    // read global parameters file
    std::vector<Length> data;
    std::ifstream f_global {global_param_filename};

    if (!f_global) {
        LOG(info, "ERROR: I CANNOT OPEN " << global_param_filename << std::endl);
        throw FileNotFoundException();
    }

    getline(f_global, tmp);
    for (int i = 0; i < 7; i++) {
        getline(f_global, tmp);
        boost::split(line, tmp, [](char c) {return c == ';';});
        data.push_back(std::stoi(line[1]));
    }

    Rectangle rect{data.at(1), data.at(2)};
    this->global_param_ = {
        .nbplates  = data.at(0),
        .platesize = rect,
        .min1cut   = data.at(3),
        .max1cut   = data.at(4),
        .min2cut   = data.at(5),
        .minwaste  = data.at(6)};

    fill_stack_pred();
    compute_item_surface();
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

    compute_item_surface();
    fill_stack_pred();
}

Instance::Instance(const Instance& ins)
{
    items_         = ins.items_;
    defects_       = ins.defects_;
    global_param_  = ins.global_param_;
    plate_defects_ = ins.plate_defects_;
    batches_       = ins.batches_;
    item_surface_  = ins.item_surface_;
    stack_pred_    = ins.stack_pred_;
}

std::string Instance::to_string() const
{
    return "instance description: " + std::to_string(this->items_.size()) + " items\t "
        + std::to_string(this->defects_.size()) + " defects\n"
        + this->global_param_.to_string();
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
                //std::cout << "stack_pred[" << s2 << "] == " << s1 << std::endl;
                break;
            }
        }
    }
}

void Instance::compute_item_surface()
{
    item_surface_ = 0;
    for (ItemId j=0; j<item_number(); ++j)
        item_surface_ += item(j).rect.area();
}

