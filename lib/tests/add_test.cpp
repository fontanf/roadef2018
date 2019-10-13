#include "roadef2018/lib/instance.hpp"
#include "roadef2018/lib/solution.hpp"

#include <gtest/gtest.h>

using namespace roadef2018;

TEST(Solution, AddItem1)
{
    /**
     * |--------------------------|
     * |                          |
     * |                          |
     * |                          |
     * |                          |
     * |                          |
     * |                          |
     * |   |---|                  | 300
     * |---|   |                  | 200
     * | 0 | 1 |                  |
     * |---|---|------------------|
     * 0  300 700
     *
     *      0    3
     *      |    |
     *      1    4
     *      |    |
     *      2    5
     *
     */

    std::vector<Item> vi {
        {.id = 0, .rect = {200, 300}, .stack = 0, .sequence = 1},
        {.id = 1, .rect = {300, 400}, .stack = 1, .sequence = 1}};
    std::vector<Defect> vd;
    GlobalParam p = GlobalParam::roadef2018();
    Info info;
    Instance ins(vi, vd, p);

    Solution sol(ins);
    EXPECT_EQ(sol.plate_number(), 0);
    EXPECT_EQ(sol.item_number(), 0);
    EXPECT_EQ(sol.waste(), 0);

    // Add item 0
    sol.add_item({.j1 = 0, .j2 = -1, .df = -1, .x1 = 300, .y2 = 200, .x3 = 300, .x1_max = 3500, .y2_max = 3210, .z1 = 0, .z2 = 0}, info);
    std::vector<SolutionNode> nodes {
        {.f = -1, .p = 300},
        {.f =  0, .p = 200},
        {.f =  1, .p = 300},
    };
    std::vector<SolutionItem> items {
        {.j = 0, .node = 2},
    };
    EXPECT_EQ(sol.nodes(), nodes);
    EXPECT_EQ(sol.items(), items);
    CutInfo c0 {.node = -1, .n = 1, .l = 0, .b = 0, .r = 300, .t = 200};
    CutInfo c1 {.node = 0, .n = 1, .l = 0, .b = 0, .r = 300, .t = 200};
    CutInfo c2 {.node = 1, .n = 1, .l = 0, .b = 0, .r = 300, .t = 200};
    CutInfo c3 {.node = 2, .n = 1, .l = 0, .b = 0, .r = 300, .t = 200};
    EXPECT_EQ(sol.curr_cut(0), c0);
    EXPECT_EQ(sol.curr_cut(1), c1);
    EXPECT_EQ(sol.curr_cut(2), c2);
    EXPECT_EQ(sol.curr_cut(3), c3);
    EXPECT_EQ(sol.prev_cut(0).node, -1);
    EXPECT_EQ(sol.prev_cut(1).node, -1);
    EXPECT_EQ(sol.prev_cut(2).node, -1);
    EXPECT_EQ(sol.prev_cut(3).node, -1);
    EXPECT_EQ(sol.z1(), 0);
    EXPECT_EQ(sol.z2(), 0);
    EXPECT_EQ(sol.x1_max(), 3500);
    EXPECT_EQ(sol.y2_max(), 3210);
    EXPECT_EQ(sol.plate_number(), 1);
    EXPECT_EQ(sol.item_number(), 1);
    EXPECT_EQ(sol.waste(), 0);

    // Add item 1;
    sol.add_item({.j1 = 1, .j2 = -1, .df = 0, .x1 = 700, .y2 = 300, .x3 = 700, .x1_max = 3800, .y2_max = 3210, .z1 = 0, .z2 = 0}, info);
    std::vector<SolutionNode> nodes2 {
        {.f = -1, .p = 300},
        {.f =  0, .p = 200},
        {.f =  1, .p = 300},
        {.f = -1, .p = 700},
        {.f =  3, .p = 300},
        {.f =  4, .p = 700},
    };
    std::vector<SolutionItem> items2 {
        {.j = 0, .node = 2},
        {.j = 1, .node = 5},
    };
    EXPECT_EQ(sol.nodes(), nodes2);
    EXPECT_EQ(sol.items(), items2);
    CutInfo c0_1 {.node = -1, .n = 2, .l = 0, .b = 0, .r = 700, .t = 300};
    CutInfo c1_1 {.node = 3, .n = 1, .l = 300, .b = 0, .r = 700, .t = 300};
    CutInfo c2_1 {.node = 4, .n = 1, .l = 300, .b = 0, .r = 700, .t = 300};
    CutInfo c3_1 {.node = 5, .n = 1, .l = 300, .b = 0, .r = 700, .t = 300};
    CutInfo c1_1_prev {.node = 0, .n = 1, .l = 0, .b = 0, .r = 300, .t = 200};
    EXPECT_EQ(sol.curr_cut(0), c0_1);
    EXPECT_EQ(sol.curr_cut(1), c1_1);
    EXPECT_EQ(sol.curr_cut(2), c2_1);
    EXPECT_EQ(sol.curr_cut(3), c3_1);
    EXPECT_EQ(sol.prev_cut(1), c1_1_prev);
    EXPECT_EQ(sol.prev_cut(0).node, -1);
    EXPECT_EQ(sol.prev_cut(2).node, -1);
    EXPECT_EQ(sol.prev_cut(3).node, -1);
    EXPECT_EQ(sol.z1(), 0);
    EXPECT_EQ(sol.z2(), 0);
    EXPECT_EQ(sol.x1_max(), 3800);
    EXPECT_EQ(sol.y2_max(), 3210);
    EXPECT_EQ(sol.plate_number(), 1);
    EXPECT_EQ(sol.item_number(), 2);
    EXPECT_EQ(sol.waste(), 3210*700 - 200*300 - 300*400);
}

TEST(Solution, AddItem2)
{
    /**
     *
     * |--------------------------|
     * |                          |
     * |                          |
     * |                          |
     * |                          |
     * |                          |
     * |                          |
     * |------|                   | 800
     * |      |                   |
     * |      |                   |
     * |  0   |                   |
     * |      |                   |
     * |      |                   |
     * |---|--|                   | 200
     * |   |                      |
     * |   |                      |
     * | 1 |---|                  | 100
     * |   | 2 |                  |
     * |---|---|------------------|
     * 0  300 700
     *       500
     *
     *       0
     *      / \
     *     1   4
     *    / \  |
     *   2   3 5
     *
     */

    std::vector<Item> vi {
        {.id = 0, .rect = {500, 600}, .stack = 0, .sequence = 1},
        {.id = 1, .rect = {200, 300}, .stack = 1, .sequence = 1},
        {.id = 2, .rect = {100, 400}, .stack = 2, .sequence = 1}};
    std::vector<Defect> vd;
    GlobalParam p = GlobalParam::roadef2018();
    Info info;
    Instance ins(vi, vd, p);
    Solution sol(ins);

    // Add item 1
    sol.add_item({.j1 = 1, .j2 = -1, .df = -1, .x1 = 300, .y2 = 200, .x3 = 300, .x1_max = 3500, .y2_max = 3210, .z1 = 0, .z2 = 0}, info);
    std::vector<SolutionNode> nodes1 {
        {.f = -1, .p = 300},
        {.f =  0, .p = 200},
        {.f =  1, .p = 300},
    };
    std::vector<SolutionItem> items1 {
        {.j = 1, .node = 2},
    };
    EXPECT_EQ(sol.nodes(), nodes1);
    EXPECT_EQ(sol.items(), items1);

    // Add item 2
    sol.add_item({.j1 = 2, .j2 = -1, .df = 2, .x1 = 700, .y2 = 200, .x3 = 700, .x1_max = 3500, .y2_max = 3210, .z1 = 0, .z2 = 0}, info);
    std::vector<SolutionNode> nodes2 {
        {.f = -1, .p = 700},
        {.f =  0, .p = 200},
        {.f =  1, .p = 300},
        {.f =  1, .p = 700},
    };
    std::vector<SolutionItem> items2 {
        {.j = 1, .node = 2},
        {.j = 2, .node = 3},
    };
    EXPECT_EQ(sol.nodes(), nodes2);
    EXPECT_EQ(sol.items(), items2);
    CutInfo c0_2 {.node = -1, .n = 2, .l = 0, .b = 0, .r = 700, .t = 200};
    CutInfo c1_2 {.node = 0, .n = 2, .l = 0, .b = 0, .r = 700, .t = 200};
    CutInfo c2_2 {.node = 1, .n = 2, .l = 0, .b = 0, .r = 700, .t = 200};
    CutInfo c3_2 {.node = 3, .n = 1, .l = 300, .b = 0, .r = 700, .t = 200};
    CutInfo c3_2_prev {.node = 2, .n = 1, .l = 0, .b = 0, .r = 300, .t = 200};
    EXPECT_EQ(sol.curr_cut(0), c0_2);
    EXPECT_EQ(sol.curr_cut(1), c1_2);
    EXPECT_EQ(sol.curr_cut(2), c2_2);
    EXPECT_EQ(sol.curr_cut(3), c3_2);
    EXPECT_EQ(sol.prev_cut(3), c3_2_prev);
    EXPECT_EQ(sol.prev_cut(0).node, -1);
    EXPECT_EQ(sol.prev_cut(1).node, -1);
    EXPECT_EQ(sol.prev_cut(2).node, -1);
    EXPECT_EQ(sol.z1(), 0);
    EXPECT_EQ(sol.z2(), 0);
    EXPECT_EQ(sol.x1_max(), 3500);
    EXPECT_EQ(sol.y2_max(), 3210);
    EXPECT_EQ(sol.plate_number(), 1);
    EXPECT_EQ(sol.item_number(), 2);
    EXPECT_EQ(sol.waste(), 100*400);

    // Add item 0
    sol.add_item({.j1 = 0, .j2 = -1, .df = 1, .x1 = 700, .y2 = 800, .x3 = 500, .x1_max = 3500, .y2_max = 3210, .z1 = 0, .z2 = 0}, info);
    std::vector<SolutionNode> nodes0 {
        {.f = -1, .p = 700},
        {.f =  0, .p = 200},
        {.f =  1, .p = 300},
        {.f =  1, .p = 700},
        {.f =  0, .p = 800},
        {.f =  4, .p = 500},
    };
    std::vector<SolutionItem> items0 {
        {.j = 1, .node = 2},
        {.j = 2, .node = 3},
        {.j = 0, .node = 5},
    };
    EXPECT_EQ(sol.nodes(), nodes0);
    EXPECT_EQ(sol.items(), items0);
    CutInfo c0_0 {.node = -1, .n = 3, .l = 0, .b = 0, .r = 700, .t = 800};
    CutInfo c1_0 {.node = 0, .n = 3, .l = 0, .b = 0, .r = 700, .t = 800};
    CutInfo c2_0 {.node = 4, .n = 1, .l = 0, .b = 200, .r = 500, .t = 800};
    CutInfo c3_0 {.node = 5, .n = 1, .l = 0, .b = 200, .r = 500, .t = 800};
    CutInfo c2_0_prev {.node = 1, .n = 2, .l = 0, .b = 0, .r = 700, .t = 200};
    EXPECT_EQ(sol.curr_cut(0), c0_0);
    EXPECT_EQ(sol.curr_cut(1), c1_0);
    EXPECT_EQ(sol.curr_cut(2), c2_0);
    EXPECT_EQ(sol.curr_cut(3), c3_0);
    EXPECT_EQ(sol.prev_cut(2), c2_0_prev);
    EXPECT_EQ(sol.prev_cut(0).node, -1);
    EXPECT_EQ(sol.prev_cut(1).node, -1);
    EXPECT_EQ(sol.prev_cut(3).node, -1);
    EXPECT_EQ(sol.z1(), 0);
    EXPECT_EQ(sol.z2(), 0);
    EXPECT_EQ(sol.x1_max(), 3500);
    EXPECT_EQ(sol.y2_max(), 3210);
    EXPECT_EQ(sol.plate_number(), 1);
    EXPECT_EQ(sol.item_number(), 3);
    EXPECT_EQ(sol.x1_curr(), 700);
    EXPECT_EQ(sol.width(), 700);
    EXPECT_EQ(sol.area(), 700*3210);
    EXPECT_EQ(sol.item_area(), 300*200 + 100*400 + 500*600);
    EXPECT_EQ(sol.waste(), 700*3210 - 300*200 - 100*400 - 500*600);
}

