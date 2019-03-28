#include "roadef2018/lib/instance.hpp"
#include "roadef2018/lib/solution.hpp"

#include <gtest/gtest.h>
#include <vector>

using namespace roadef2018;

TEST(Solution, Waste1)
{
    /*
     * |-------------------------------------------|
     * |                                           |
     * |                                           |
     * |                                           |
     * |-------|                                   |
     * | 50x50 |                                   |
     * |       |----|                              |
     * |       | 30 |                              |
     * |       |x30 |                              |
     * +-------|----|------------------------------|
     */

    Info info;
    GlobalParam p = GlobalParam::roadef2018();
    std::vector<Defect> vd;

    std::vector<Item> vi {
        {.id = 0, .rect = {50, 50}, .stack = 0, .sequence = 1},
        {.id = 1, .rect = {30, 30}, .stack = 0, .sequence = 2},
        {.id = 2, .rect = {40, 40}, .stack = 0, .sequence = 3},
    };
    Instance ins(vi, vd, p);
    Solution sol(ins);

    Insertion i0 {.j1 = 0, .j2 = -1, .df = -1,
            .x1 = 50, .y2 = 50, .x3 = 50,
            .x1_max = 3500, .y2_max = 3210,
            .z1 = 0, .z2 = 0};
    std::vector<Insertion> is0 = sol.all_valid_insertions(info);
    EXPECT_NE(std::find(is0.begin(), is0.end(), i0), is0.end());
    sol.add_item(i0, info);
    EXPECT_EQ(sol.waste(), 0);
    EXPECT_EQ(sol.width(), 50);

    Insertion i1 {.j1 = 1, .j2 = -1, .df =  2,
            .x1 = 80, .y2 = 50, .x3 = 80,
            .x1_max = 3500, .y2_max = 3210,
            .z1 = 0, .z2 = 0};
    std::vector<Insertion> is1 = sol.all_valid_insertions(info);
    sol.add_item(i1, info);
    EXPECT_EQ(sol.waste(), 20*30);
    EXPECT_EQ(sol.width(), 80);
}

TEST(Solution, Waste2)
{
    /*
     * +------------------------------------------------+
     * |                                                |
     * |                                                |
     * +--------+                                       |
     * |  500   |                                       |
     * | x100   |                                       |
     * +-----+--+                                       |
     * |     |                                          |
     * | 300 |                                          |
     * |x300 |                                          |
     * +-----+--+                                       |
     * |        |                                       |
     * |   500  |                                       |
     * |  x500  |                                       |
     * |        |                                       |
     * +--------+---------------------------------------+
     *
     */

    Info info;
    GlobalParam p = GlobalParam::roadef2018();
    std::vector<Defect> vd;

    std::vector<Item> vi {
        {.id = 0, .rect = {500, 500}, .stack = 0, .sequence = 1},
        {.id = 1, .rect = {300, 300}, .stack = 0, .sequence = 2},
        {.id = 2, .rect = {100, 500}, .stack = 0, .sequence = 3},
        {.id = 3, .rect = {100, 500}, .stack = 0, .sequence = 4},
    };
    Instance ins(vi, vd, p);
    Solution sol(ins);

    Insertion i0 {.j1 = 0, .j2 = -1, .df = -1,
            .x1 = 500, .y2 = 500, .x3 = 500,
            .x1_max = 3500, .y2_max = 3210,
            .z1 = 0, .z2 = 0};
    std::vector<Insertion> is0 = sol.all_valid_insertions(info);
    EXPECT_NE(std::find(is0.begin(), is0.end(), i0), is0.end());
    sol.add_item(i0, info);
    EXPECT_EQ(sol.waste(), 0);
    EXPECT_EQ(sol.width(), 500);

    Insertion i1 {.j1 = 1, .j2 = -1, .df = 1,
            .x1 = 500, .y2 = 800, .x3 = 300,
            .x1_max = 3500, .y2_max = 3210,
            .z1 = 0, .z2 = 0};
    std::vector<Insertion> is1 = sol.all_valid_insertions(info);
    sol.add_item(i1, info);
    EXPECT_EQ(sol.waste(), 0);
    EXPECT_EQ(sol.width(), 500);

    Insertion i2 = {.j1 = 2, .j2 = -1, .df = 1,
        .x1 = 500, .y2 = 900, .x3 = 500,
        .x1_max = 3500, .y2_max = 3210,
        .z1 = 0, .z2 = 0};
    std::vector<Insertion> is2 = sol.all_valid_insertions(info);
    EXPECT_NE(std::find(is2.begin(), is2.end(), i2), is2.end());
    sol.add_item(i2, info);
    EXPECT_EQ(sol.waste(), 300*200);
}

TEST(Solution, Waste3)
{
    /*
     * +-----------------------------------------------------------+
     * |                                                           |
     * |                                                           |
     * |                                                           |
     * |                                                           |
     * |                                                           |
     * |                                                           |
     * |                                                           |
     * |                                                           |
     * |                                                           |
     * |                                                           |
     * +-----------------+                                         |
     * |       1x6       |                                         |
     * +-----------------+                                         |
     * |     |     |     |                                         |
     * |     +-----+     |                                         |
     * |     |     |     |                                         |
     * | 2x5 |     | 2x5 |                                         |
     * |     | 2x4 |     |                                         |
     * |     |     |     |                                         |
     * +-----+-----+-----+-----------------------------------------+
     */

    Info info;
    GlobalParam p = GlobalParam::roadef2018();
    std::vector<Defect> vd;

    std::vector<Item> vi {
        {.id = 0, .rect = {200, 500}, .stack = 0, .sequence = 1},
        {.id = 1, .rect = {200, 400}, .stack = 0, .sequence = 1},
        {.id = 2, .rect = {200, 500}, .stack = 0, .sequence = 2},
        {.id = 3, .rect = {600, 100}, .stack = 0, .sequence = 3},
        {.id = 4, .rect = {600, 100}, .stack = 0, .sequence = 5},
    };
    Instance ins(vi, vd, p);
    Solution sol(ins);

    Insertion i0 {.j1 = 0, .j2 = -1, .df = -1,
            .x1 = 200, .y2 = 500, .x3 = 200,
            .x1_max = 3500, .y2_max = 3210,
            .z1 = 0, .z2 = 0};
    std::vector<Insertion> is0 = sol.all_valid_insertions(info);
    EXPECT_NE(std::find(is0.begin(), is0.end(), i0), is0.end());
    sol.add_item(i0, info);
    EXPECT_EQ(sol.waste(), 0);
    EXPECT_EQ(sol.width(), 200);

    Insertion i1 {.j1 = 1, .j2 = -1, .df = 2,
            .x1 = 400, .y2 = 500, .x3 = 400,
            .x1_max = 3500, .y2_max = 3210,
            .z1 = 0, .z2 = 0};
    std::vector<Insertion> is1 = sol.all_valid_insertions(info);
    sol.add_item(i1, info);
    EXPECT_EQ(sol.waste(), 20000);
    EXPECT_EQ(sol.width(), 400);

    Insertion i2 = {.j1 = 2, .j2 = -1, .df = 2,
        .x1 = 600, .y2 = 500, .x3 = 600,
        .x1_max = 3500, .y2_max = 3210,
        .z1 = 0, .z2 = 0};
    std::vector<Insertion> is2 = sol.all_valid_insertions(info);
    EXPECT_NE(std::find(is2.begin(), is2.end(), i2), is2.end());
    sol.add_item(i2, info);
    EXPECT_EQ(sol.waste(), 20000);
    EXPECT_EQ(sol.width(), 600);

    Insertion i3 = {.j1 = 3, .j2 = -1, .df = 1,
        .x1 = 600, .y2 = 600, .x3 = 600,
        .x1_max = 3500, .y2_max = 3210,
        .z1 = 0, .z2 = 0};
    std::vector<Insertion> is3 = sol.all_valid_insertions(info);
    EXPECT_NE(std::find(is3.begin(), is3.end(), i3), is3.end());
    sol.add_item(i3, info);
    EXPECT_EQ(sol.waste(), 20000);
    EXPECT_EQ(sol.width(), 600);
}

TEST(Solution, WasteA1)
{
    /* test A1 with two objects (waste positive)

     0
     |
     1      Plate
     | \
     2  3   1
        |
        4   2
        |
        5   3

    */

    Info info;
    Instance ins = Instance("instances/A1_batch.csv", "instances/A1_defects.csv", "instances/global_param.csv");
    Solution sol(ins);

    Insertion i0 {.j1 = 0, .j2 = -1, .df = -1,
            .x1 = 758, .y2 = 1578, .x3 = 758,
            .x1_max = 3500, .y2_max = 3210,
            .z1 = 0, .z2 = 0};
    std::vector<Insertion> is0 = sol.all_valid_insertions(info);
    EXPECT_NE(std::find(is0.begin(), is0.end(), i0), is0.end());
    sol.add_item(i0, info);
    EXPECT_EQ(sol.waste(), 0);
    EXPECT_EQ(sol.width(), 758);

    Insertion i1 {.j1 = 1, .j2 = -1, .df = 0,
            .x1 = 2308, .y2 = 738, .x3 = 2308,
            .x1_max = 4258, .y2_max = 3210,
            .z1 = 0, .z2 = 0};
    std::vector<Insertion> is1 = sol.all_valid_insertions(info);
    sol.add_item(i1, info);
    EXPECT_EQ(sol.waste(), 758*(3210-1578));
    EXPECT_EQ(sol.width(), 2308);
}

TEST(Solution, WasteIntegrationA1)
{
    /**
     * +------+-------+----------------------------+
     * |      |       |                            |
     * |      |       |                            |
     * |      |   4   |                            |
     * |   1  |       |                            |
     * |      +-------++                           |
     * |      |        |                           |
     * +------+        |                           |
     * |      |        |                           |
     * |      |    3   |                           |
     * |      |        |                           |
     * |  0   |        |                           |
     * |      +------+-+                           |
     * |      |   2  |                             |
     * +-------------+-----------------------------+
     */

    Info info;
    Instance ins = Instance("instances/A1_batch.csv", "instances/A1_defects.csv", "instances/global_param.csv");
    EXPECT_EQ(ins.item_number(), 5);
    Solution sol = Solution(ins);

    Insertion i0 {.j1 = 0, .j2 = -1, .df = -1,
            .x1 = 758, .y2 = 1578, .x3 = 758,
            .x1_max = 3500, .y2_max = 3210,
            .z1 = 0, .z2 = 0};
    std::vector<Insertion> is0 = sol.all_valid_insertions(info);
    EXPECT_NE(std::find(is0.begin(), is0.end(), i0), is0.end());
    sol.add_item(i0, info);
    EXPECT_EQ(sol.waste(), 0);
    EXPECT_EQ(sol.width(), 758);

    Insertion i1 {.j1 = 1, .j2 = -1, .df = 1,
            .x1 = 758, .y2 = 3128, .x3 = 738,
            .x1_max = 3500, .y2_max = 3210,
            .z1 = 0, .z2 = 0};
    std::vector<Insertion> is1 = sol.all_valid_insertions(info);
    sol.add_item(i1, info);
    EXPECT_EQ(sol.waste(), 0);
    EXPECT_EQ(sol.width(), 758);

    Insertion i2 = {.j1 = 2, .j2 = -1, .df = 0,
        .x1 = 1339, .y2 = 276, .x3 = 1339,
        .x1_max = 4258, .y2_max = 3210,
        .z1 = 0, .z2 = 0};
    std::vector<Insertion> is2 = sol.all_valid_insertions(info);
    EXPECT_NE(std::find(is2.begin(), is2.end(), i2), is2.end());
    sol.add_item(i2, info);
    EXPECT_EQ(sol.waste(), 20*1550+(3210-3128)*758);
    EXPECT_EQ(sol.width(), 1339);

    Insertion i3 = {.j1 = 3, .j2 = -1, .df = 1,
        .x1 = 1539, .y2 = 1672, .x3 = 1539,
        .x1_max = 4258, .y2_max = 3210,
        .z1 = 0, .z2 = 0};
    std::vector<Insertion> is3 = sol.all_valid_insertions(info);
    EXPECT_NE(std::find(is3.begin(), is3.end(), i3), is3.end());
    sol.add_item(i3, info);
    EXPECT_EQ(sol.waste(), 20*1550+(3210-3128)*758+(781-581)*276);
    EXPECT_EQ(sol.width(), 1539);

    Insertion i4 = {.j1 = 4, .j2 = -1, .df = 1,
        .x1 = 1539, .y2 = 3098, .x3 = 1406,
        .x1_max = 4258, .y2_max = 3210,
        .z1 = 0, .z2 = 0};
    std::vector<Insertion> is4 = sol.all_valid_insertions(info);
    EXPECT_NE(std::find(is4.begin(), is4.end(), i4), is4.end());
    sol.add_item(i4, info);
    EXPECT_EQ(sol.width(), 1539);
    EXPECT_EQ(sol.waste(), 425486);
    EXPECT_EQ(sol.lb_width(), 1539);
}

TEST(Solution, WasteSolutionEnd)
{
    /**
     * Waste = 20*1500 + 25*1710
     *
     * |-----------|----------------------|
     * |           |                      |
     * |           |                      |
     * |           |                      |
     * |           |                      |
     * |     1     |                      |
     * |           |                      |
     * |           |                      |
     * |           |                      |
     * |-----------||                     | 1500
     * |            |                     |
     * |            |                     |
     * |            |                     |
     * |     0      |                     |
     * |            |                     |
     * |            |                     |
     * |            |                     |
     * |------------|---------------------|
     *           1995
     *            2000
     *
     */

    Info info;
    GlobalParam p = GlobalParam::roadef2018();
    std::vector<Defect> vd;

    std::vector<Item> vi {
        {.id = 0, .rect = {1500, 2000}, .stack = 0, .sequence = 0},
        {.id = 1, .rect = {1710, 1995}, .stack = 1, .sequence = 1},
    };
    Instance ins(vi, vd, p);
    Solution sol(ins);

    Insertion i0 {.j1 = 0, .j2 = -1, .df = -1,
            .x1 = 2000, .y2 = 1500, .x3 = 2000,
            .x1_max = 3500, .y2_max = 3210,
            .z1 = 0, .z2 = 0};
    std::vector<Insertion> is0 = sol.all_valid_insertions(info);
    EXPECT_NE(std::find(is0.begin(), is0.end(), i0), is0.end());
    sol.add_item(i0, info);

    Insertion i1 {.j1 = 1, .j2 = -1, .df = 1,
            .x1 = 2020, .y2 = 3210, .x3 = 1995,
            .x1_max = 3500, .y2_max = 3210,
            .z1 = 1, .z2 = 0};
    std::vector<Insertion> is1 = sol.all_valid_insertions(info);
    sol.add_item(i1, info);
    EXPECT_EQ(sol.waste(), 20*1500 + 25*1710);
    EXPECT_EQ(sol.width(), 2020);
}

