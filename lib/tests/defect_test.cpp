#include "roadef2018/lib/instance.hpp"
#include "roadef2018/lib/solution.hpp"

#include <gtest/gtest.h>
#include <vector>

using namespace roadef2018;

TEST(Solution, Insertion4CutDefect1)
{
    /**
     *
     * |----------------------------------|
     * |                                  |
     * |                                  |
     * |                                  |
     * |                                  |
     * |                                  |
     * |                                  |
     * |                                  |
     * |                                  |
     * |                                  |
     * |                                  |
     * |-------|---|                      | 1000
     * |   0   |   |                      |
     * |-------|   |                      | 500
     * |       | 1 |                      |
     * |   x   |   |                      | 250
     * |       |   |                      |
     * |-------|---|----------------------|
     *   500 1000 1700
     *
     */

    Logger logger;
    Info info(logger);
    GlobalParam p = GlobalParam::roadef2018();
    std::vector<Defect> vd {
        {.id = 0, .plate_id = 0, .pos = {.x = 500, .y = 248}, .rect = {.w = 2, .h = 2}}
    };
    std::vector<Item> vi {
        {.id = 0, .rect = {500, 1000}, .stack = 0, .sequence = 1},
        {.id = 1, .rect = {700, 1000}, .stack = 0, .sequence = 2},
        {.id = 2, .rect = {1700, 2000}, .stack = 0, .sequence = 3},
    };
    Instance ins(vi, vd, p);
    Solution sol(ins);

    std::vector<Insertion> is {
        {.j1 = -1, .j2 = 0, .df = -1, .x1 = 1000, .y2 = 750, .x3 = 1000, .x1_max = 3500, .y2_max = 3210, .z1 = 0, .z2 = 1},
        {.j1 = 0, .j2 = -1, .df = -1, .x1 = 500, .y2 = 1000, .x3 = 500, .x1_max = 3500, .y2_max = 3210, .z1 = 0, .z2 = 0},
        {.j1 = -1, .j2 = -1, .df = -1, .x1 = 502, .y2 = 250, .x3 = 502, .x1_max = 3500, .y2_max = 3210, .z1 = 1, .z2 = 1},
    };
    EXPECT_EQ(sol.all_valid_insertions(info), is);
    sol.add_item({.j1 = -1, .j2 = 0, .df = -1, .x1 = 1000, .y2 = 750, .x3 = 1000, .x1_max = 3500, .y2_max = 3210, .z1 = 0, .z2 = 1}, info);

    std::vector<Insertion> is_2 {
        {.j1 = 1, .j2 = -1, .df = 2, .x1 = 2000, .y2 = 750, .x3 = 2000, .x1_max = 3500, .y2_max = 3210, .z1 = 0, .z2 = 1},
        {.j1 = 1, .j2 = -1, .df = 2, .x1 = 1700, .y2 = 1000, .x3 = 1700, .x1_max = 3500, .y2_max = 3210, .z1 = 0, .z2 = 0},
        //{.j1 = 1, .j2 = -1, .df = 1, .x1 = 1000, .y2 = 1450, .x3 = 1000, .x1_max = 3500, .y2_max = 3210, .z1 = 0, .z2 = 0}, // TODO
        //{.j1 = 1, .j2 = -1, .df = 1, .x1 = 1000, .y2 = 1750, .x3 = 700, .x1_max = 3500, .y2_max = 3210, .z1 = 0, .z2 = 0}, // TODO
    };
    EXPECT_EQ(sol.all_valid_insertions(info), is_2);
    sol.add_item({.j1 = 1, .j2 = -1, .df = 2, .x1 = 1700, .y2 = 1000, .x3 = 1700, .x1_max = 3500, .y2_max = 3210, .z1 = 0, .z2 = 0}, info);

    Insertion i2 = {.j1 = 2, .j2 = -1, .df = 1, .x1 = 1700, .y2 = 3000, .x3 = 1700, .x1_max = 3500, .y2_max = 3210, .z1 = 0, .z2 = 0};
    std::vector<Insertion> is2 = sol.all_valid_insertions(info);
    EXPECT_NE(std::find(is2.begin(), is2.end(), i2), is2.end());
    sol.add_item(i2, info);
}

TEST(Solution, Insertion4CutDefect2)
{
    /**
     *
     * |----------------------------------|
     * |                                  |
     * |                                  |
     * |                                  |
     * |                                  |
     * |                                  |
     * |                                  |
     * |                                  |
     * |                                  |
     * |                                  |
     * |                                  |
     * |-------|                          | 1400
     * |       |                          |
     * |   0   |---|                      | 1000
     * |-------|   |                      |
     * |   x   |   |                      | 900
     * |       |   |                      |
     * |       | 1 |                      |
     * |   x   |   |                      | 300
     * |       |   |                      |
     * |-------|---|----------------------|
     *   500 1000 1700
     *
     */

    Logger logger;
    Info info(logger);
    GlobalParam p = GlobalParam::roadef2018();
    std::vector<Defect> vd {
        {.id = 0, .plate_id = 0, .pos = {.x = 500, .y = 298}, .rect = {.w = 2, .h = 2}},
        {.id = 1, .plate_id = 0, .pos = {.x = 500, .y = 898}, .rect = {.w = 2, .h = 2}},
    };
    std::vector<Item> vi {
        {.id = 0, .rect = {500, 1000}, .stack = 0, .sequence = 0},
        {.id = 1, .rect = {700, 1000}, .stack = 0, .sequence = 1},
    };
    Instance ins(vi, vd, p);
    Solution sol(ins);

    std::vector<Insertion> is {
        {.j1 = -1, .j2 = 0, .df = -1, .x1 = 1000, .y2 = 800, .x3 = 1000, .x1_max = 3500, .y2_max = 3210, .z1 = 0, .z2 = 1},
        {.j1 = 0, .j2 = -1, .df = -1, .x1 = 500, .y2 = 1000, .x3 = 500, .x1_max = 3500, .y2_max = 3210, .z1 = 0, .z2 = 0},
        {.j1 = -1, .j2 = -1, .df = -1, .x1 = 502, .y2 = 300, .x3 = 502, .x1_max = 3500, .y2_max = 3210, .z1 = 1, .z2 = 1},
    };
    EXPECT_EQ(sol.all_valid_insertions(info), is);
    sol.add_item({.j1 = -1, .j2 = 0, .df = -1, .x1 = 1000, .y2 = 750, .x3 = 1000, .x1_max = 3500, .y2_max = 3210, .z1 = 0, .z2 = 1}, info);

    std::vector<Insertion> is_2 {
        {.j1 = 1, .j2 = -1, .df = 2, .x1 = 2000, .y2 = 800, .x3 = 2000, .x1_max = 3500, .y2_max = 3210, .z1 = 0, .z2 = 1},
        {.j1 = 1, .j2 = -1, .df = 2, .x1 = 1700, .y2 = 1400, .x3 = 1700, .x1_max = 3500, .y2_max = 3210, .z1 = 0, .z2 = 1},
        //{.j1 = -1, .j2 = 1, .df = 1, .x1 = 1000, .y2 = 1600, .x3 = 1000, .x1_max = 3500, .y2_max = 3210, .z1 = 0, .z2 = 1}, // TODO
        //{.j1 = -1, .j2 = 1, .df = 1, .x1 = 1000, .y2 = 1900, .x3 = 700, .x1_max = 3500, .y2_max = 3210, .z1 = 0, .z2 = 1}, // TODO
    };
    EXPECT_EQ(sol.all_valid_insertions(info), is_2);
}

TEST(Solution, Insertion4CutDefect3)
{
    /**
     *
     * |----------------------------------|
     * |                                  |
     * |                                  |
     * |                                  |
     * |                                  |
     * |                                  |
     * |                                  |
     * |                                  |
     * |-------|                          | 1800
     * |       |                          |
     * |   0   |                          |
     * |-------|                          |
     * |   x   |                          | 1300
     * |       |---|                      | 1000
     * |       |   |                      |
     * |   x   |   |                      | 900
     * |       |   |                      |
     * |       | 1 |                      |
     * |   x   |   |                      | 300
     * |       |   |                      |
     * |-------|---|----------------------|
     *   500 1000 1700
     *
     */

    Logger logger;
    Info info(logger);
    GlobalParam p = GlobalParam::roadef2018();
    std::vector<Defect> vd {
        {.id = 0, .plate_id = 0, .pos = {.x = 500, .y = 298}, .rect = {.w = 2, .h = 2}},
        {.id = 1, .plate_id = 0, .pos = {.x = 500, .y = 898}, .rect = {.w = 2, .h = 2}},
        {.id = 2, .plate_id = 0, .pos = {.x = 500, .y = 1298}, .rect = {.w = 2, .h = 2}},
    };
    std::vector<Item> vi {
        {.id = 0, .rect = {500, 1000}, .stack = 0, .sequence = 0},
        {.id = 1, .rect = {700, 1000}, .stack = 0, .sequence = 1},
    };
    Instance ins(vi, vd, p);
    Solution sol(ins);

    std::vector<Insertion> is {
        {.j1 = -1, .j2 = 0, .df = -1, .x1 = 1000, .y2 = 800, .x3 = 1000, .x1_max = 3500, .y2_max = 3210, .z1 = 0, .z2 = 1},
        {.j1 = 0, .j2 = -1, .df = -1, .x1 = 500, .y2 = 1000, .x3 = 500, .x1_max = 3500, .y2_max = 3210, .z1 = 0, .z2 = 0},
        {.j1 = -1, .j2 = -1, .df = -1, .x1 = 502, .y2 = 300, .x3 = 502, .x1_max = 3500, .y2_max = 3210, .z1 = 1, .z2 = 1},
    };
    EXPECT_EQ(sol.all_valid_insertions(info), is);
    sol.add_item({.j1 = -1, .j2 = 0, .df = -1, .x1 = 1000, .y2 = 750, .x3 = 1000, .x1_max = 3500, .y2_max = 3210, .z1 = 0, .z2 = 1}, info);

    std::vector<Insertion> is_2 {
        {.j1 = 1, .j2 = -1, .df = 2, .x1 = 2000, .y2 = 800, .x3 = 2000, .x1_max = 3500, .y2_max = 3210, .z1 = 0, .z2 = 1},
        {.j1 = 1, .j2 = -1, .df = 2, .x1 = 1700, .y2 = 1800, .x3 = 1700, .x1_max = 3500, .y2_max = 3210, .z1 = 0, .z2 = 1},
        //{.j1 = -1, .j2 = 1, .df = 1, .x1 = 1000, .y2 = 2000, .x3 = 1000, .x1_max = 3500, .y2_max = 3210, .z1 = 0, .z2 = 1}, // TODO
        //{.j1 = -1, .j2 = 1, .df = 1, .x1 = 1000, .y2 = 2300, .x3 = 700, .x1_max = 3500, .y2_max = 3210, .z1 = 0, .z2 = 1}, // TODO
    };
    EXPECT_EQ(sol.all_valid_insertions(info), is_2);
}

TEST(Solution, Insertion4CutDefect4)
{
    /**
     *
     * |----------------------------------|
     * |                                  |
     * |                                  |
     * |                                  |
     * |                                  |
     * |                                  |
     * |                                  |
     * |                                  |
     * |                                  |
     * |                                  |
     * |                                  |
     * |---|-------|                      | 1000
     * |   |   1   |                      |
     * |   |-------|                      | 500
     * | 0 |       |                      |
     * |   |   x   |                      | 250
     * |   |       |                      |
     * |---|-------|----------------------|
     *    500    2000
     *
     */

    Logger logger;
    Info info(logger);
    GlobalParam p = GlobalParam::roadef2018();
    std::vector<Defect> vd {
        {.id = 0, .plate_id = 0, .pos = {.x = 1000, .y = 248}, .rect = {.w = 2, .h = 2}}
    };
    std::vector<Item> vi {
        {.id = 0, .rect = {500, 1000}, .stack = 0, .sequence = 0},
        {.id = 1, .rect = {500, 1500}, .stack = 0, .sequence = 1},
    };
    Instance ins(vi, vd, p);
    Solution sol(ins);

    Insertion i0 = {.j1 = 0, .j2 = -1, .df = -1, .x1 = 500, .y2 = 1000, .x3 = 500, .x1_max = 3500, .y2_max = 3210, .z1 = 0, .z2 = 0};
    std::vector<Insertion> is0 = sol.all_valid_insertions(info);
    EXPECT_NE(std::find(is0.begin(), is0.end(), i0), is0.end());
    sol.add_item(i0, info);

    std::vector<Insertion> is {
        {.j1 = -1, .j2 = 1, .df = 2, .x1 = 2000, .y2 = 1000, .x3 = 2000, .x1_max = 3500, .y2_max = 3210, .z1 = 0, .z2 = 0},
        {.j1 = 1, .j2 = -1, .df = 2, .x1 = 1000, .y2 = 1500, .x3 = 1000, .x1_max = 3500, .y2_max = 3210, .z1 = 0, .z2 = 0},
        {.j1 = -1, .j2 = -1, .df = 2, .x1 = 1002, .y2 = 1000, .x3 = 1002, .x1_max = 3500, .y2_max = 3210, .z1 = 1, .z2 = 0},
        {.j1 = 1, .j2 = -1, .df = 1, .x1 = 500, .y2 = 2500, .x3 = 500, .x1_max = 3500, .y2_max = 3210, .z1 = 0, .z2 = 0},
    };
    EXPECT_EQ(sol.all_valid_insertions(info), is);
}

TEST(Solution, Insertion4CutDefect5)
{
    /**
     *
     * |----------------------------------|
     * |                                  |
     * |                                  |
     * |                                  |
     * |                                  |
     * |                                  |
     * |                                  |
     * |                                  |
     * |                                  |
     * |   |-------|                      | 1300
     * |---|   1   |                      | 1000
     * |   |-------|                      |
     * |   |   x   |                      | 800
     * |   |       |                      |
     * | 0 |       |                      |
     * |   |   x   |                      | 250
     * |   |       |                      |
     * |---|-------|----------------------|
     *    500    2000
     *
     */

    Logger logger;
    Info info(logger);
    GlobalParam p = GlobalParam::roadef2018();
    std::vector<Defect> vd {
        {.id = 0, .plate_id = 0, .pos = {.x = 1000, .y = 248}, .rect = {.w = 2, .h = 2}},
        {.id = 1, .plate_id = 0, .pos = {.x = 1000, .y = 798}, .rect = {.w = 2, .h = 2}},
    };
    std::vector<Item> vi {
        {.id = 0, .rect = {500, 1000}, .stack = 0, .sequence = 0},
        {.id = 1, .rect = {500, 1500}, .stack = 0, .sequence = 1},
    };
    Instance ins(vi, vd, p);
    Solution sol(ins);

    Insertion i0 = {.j1 = 0, .j2 = -1, .df = -1, .x1 = 500, .y2 = 1000, .x3 = 500, .x1_max = 3500, .y2_max = 3210, .z1 = 0, .z2 = 0};
    std::vector<Insertion> is0 = sol.all_valid_insertions(info);
    EXPECT_NE(std::find(is0.begin(), is0.end(), i0), is0.end());
    sol.add_item(i0, info);

    std::vector<Insertion> is {
        {.j1 = -1, .j2 = 1, .df = 2, .x1 = 2000, .y2 = 1300, .x3 = 2000, .x1_max = 3500, .y2_max = 3210, .z1 = 0, .z2 = 1},
        {.j1 = 1, .j2 = -1, .df = 2, .x1 = 1000, .y2 = 1500, .x3 = 1000, .x1_max = 3500, .y2_max = 3210, .z1 = 0, .z2 = 0},
        {.j1 = -1, .j2 = -1, .df = 2, .x1 = 1002, .y2 = 1000, .x3 = 1002, .x1_max = 3500, .y2_max = 3210, .z1 = 1, .z2 = 0},
        {.j1 = 1, .j2 = -1, .df = 1, .x1 = 500, .y2 = 2500, .x3 = 500, .x1_max = 3500, .y2_max = 3210, .z1 = 0, .z2 = 0},
    };
    EXPECT_EQ(sol.all_valid_insertions(info), is);
}

TEST(Solution, Insertion4CutDefect6)
{
    /**
     *
     * |----------------------------------|
     * |                                  |
     * |                                  |
     * |                                  |
     * |                                  |
     * |                                  |
     * |                                  |
     * |                                  |
     * |                                  |
     * |                                  |
     * |                                  |
     * |                                  |
     * |                                  |
     * |-------|                          | 1000
     * |   1   |   x                      | 750
     * |-------|                          | 500
     * |   0   |                          |
     * |-------|--------------------------|
     *       1000
     *
     */

    Logger logger;
    Info info(logger);
    GlobalParam p = GlobalParam::roadef2018();
    std::vector<Defect> vd {
        {.id = 0, .plate_id = 0, .pos = {.x = 1250, .y = 748}, .rect = {.w = 2, .h = 2}},
    };
    std::vector<Item> vi {
        {.id = 0, .rect = {500, 1000}, .stack = 0, .sequence = 0},
        {.id = 1, .rect = {500, 1000}, .stack = 0, .sequence = 1},
        {.id = 2, .rect = {500, 1500}, .stack = 0, .sequence = 2},
    };
    Instance ins(vi, vd, p);
    Solution sol(ins);

    Insertion i0 = {.j1 = 0, .j2 = 1, .df = -1, .x1 = 1000, .y2 = 1000, .x3 = 1000, .x1_max = 3500, .y2_max = 3210, .z1 = 0, .z2 = 2};
    std::vector<Insertion> is0 = sol.all_valid_insertions(info);
    EXPECT_NE(std::find(is0.begin(), is0.end(), i0), is0.end());
    sol.add_item(i0, info);

    std::vector<Insertion> is {
        {.j1 = 2, .j2 = -1, .df = 2, .x1 = 2500, .y2 = 1000, .x3 = 2500, .x1_max = 3500, .y2_max = 3210, .z1 = 0, .z2 = 2},
        {.j1 = -1, .j2 = -1, .df = 2, .x1 = 1252, .y2 = 1000, .x3 = 1252, .x1_max = 3500, .y2_max = 3210, .z1 = 1, .z2 = 2},
        //{.j1 = 2, .j2 = -1, .df = 1, .x1 = 1500, .y2 = 1500, .x3 = 1500, .x1_max = 3500, .y2_max = 3210, .z1 = 0, .z2 = 0},
        //{.j1 = 2, .j2 = -1, .df = 1, .x1 = 1000, .y2 = 2500, .x3 = 500, .x1_max = 3500, .y2_max = 3210, .z1 = 0, .z2 = 0},
    };
    EXPECT_EQ(sol.all_valid_insertions(info), is);
}

TEST(Solution, Insertion4CutDefect7)
{
    /**
     * The defect is very close to the side, therefore yj = minwaste.
     *
     * |----------------------------------|
     * |                                  |
     * |                                  |
     * |                                  |
     * |                                  |
     * |                                  |
     * |                                  |
     * |                                  |
     * |                                  |
     * |                                  |
     * |                                  |
     * |-------|                          |
     * |   0   |                          |
     * |-------|                          |
     * |   x                              | 10
     * |----------------------------------|
     *   500
     *
     */

    Logger logger;
    Info info(logger);
    GlobalParam p = GlobalParam::roadef2018();
    std::vector<Defect> vd {
        {.id = 0, .plate_id = 0, .pos = {.x = 248, .y = 8}, .rect = {.w = 2, .h = 2}}
    };
    std::vector<Item> vi {
        {.id = 0, .rect = {500, 1000}, .stack = 0, .sequence = 0},
    };
    Instance ins(vi, vd, p);
    Solution sol(ins);

    std::vector<Insertion> is {
        {.j1 = -1, .j2 = 0, .df = -1, .x1 = 1000, .y2 = 520, .x3 = 1000, .x1_max = 3500, .y2_max = 3210, .z1 = 0, .z2 = 1},
        {.j1 = -1, .j2 = 0, .df = -1, .x1 = 500, .y2 = 1020, .x3 = 500, .x1_max = 3500, .y2_max = 3210, .z1 = 0, .z2 = 1},
        {.j1 = -1, .j2 = -1, .df = -1, .x1 = 250, .y2 = 20, .x3 = 250, .x1_max = 3500, .y2_max = 3210, .z1 = 1, .z2 = 1},
    };
    EXPECT_EQ(sol.all_valid_insertions(info), is);
}

TEST(Solution, Insertion4CutDefect8)
{
    /**
     * If we add item 2, then yk = 3000 => 3210
     * However, in this case, item 0 contains a defect.
     * Therefore, it should not be possible to insert item 1.
     *
     * |----------------------------------| 3210
     * |   x                              | 3205
     * |           |---|                  | 3180
     * |       |---)   |                  | 3170
     * |       |   |   |                  |
     * |       |   |   |                  |
     * |       |   |   |                  |
     * |       |   |   |                  |
     * |       |   |   |                  |
     * |       | 1 | 2 |                  |
     * |       |   |   |                  |
     * |-------|   |   |                  |
     * |   0   |   |   |                  |
     * |-------|   |   |                  |
     * |       |   |   |                  |
     * |   x   |   |   |                  | 250
     * |       |   |   |                  |
     * |-------|---|---|------------------|
     *       1000 1200 1400
     *
     */

    Logger logger;
    Info info(logger);
    GlobalParam p = GlobalParam::roadef2018();
    std::vector<Defect> vd {
        {.id = 0, .plate_id = 0, .pos = {.x = 498, .y = 248}, .rect = {.w = 2, .h = 2}},
        {.id = 1, .plate_id = 0, .pos = {.x = 498, .y = 3205}, .rect = {.w = 2, .h = 2}},
    };
    std::vector<Item> vi {
        {.id = 0, .rect = {500, 1000}, .stack = 0, .sequence = 1},
        {.id = 1, .rect = {200, 3170}, .stack = 0, .sequence = 2},
        {.id = 2, .rect = {200, 3180}, .stack = 0, .sequence = 3},
    };
    Instance ins(vi, vd, p);
    Solution sol(ins);

    Insertion i0 = {.j1 = -1, .j2 = 0, .df = -1, .x1 = 1000, .y2 = 750, .x3 = 1000, .x1_max = 3500, .y2_max = 3210, .z1 = 0, .z2 = 1};
    std::vector<Insertion> is0 = sol.all_valid_insertions(info);
    EXPECT_NE(std::find(is0.begin(), is0.end(), i0), is0.end());
    sol.add_item(i0, info);

    Insertion i1 = {.j1 = 1, .j2 = -1, .df = 2, .x1 = 1200, .y2 = 3170, .x3 = 1200, .x1_max = 3500, .y2_max = 3210, .z1 = 0, .z2 = 0};
    std::vector<Insertion> is1 = sol.all_valid_insertions(info);
    EXPECT_NE(std::find(is1.begin(), is1.end(), i1), is1.end());
    sol.add_item(i1, info);

    std::vector<Insertion> is {
        {.j1 = 2, .j2 = -1, .df = 0, .x1 = 4380, .y2 = 200, .x3 = 4380, .x1_max = 4700, .y2_max = 3210, .z1 = 0, .z2 = 0},
        {.j1 = 2, .j2 = -1, .df = 0, .x1 = 1400, .y2 = 3180, .x3 = 1400, .x1_max = 4700, .y2_max = 3210, .z1 = 0, .z2 = 0},
    };
    EXPECT_EQ(sol.all_valid_insertions(info), is);
}

TEST(Solution, InsertionCutOnDefect1)
{
    /**
     *
     * |----------------------------------|
     * |                                  |
     * |                                  |
     * |                                  |
     * |                                  |
     * |                                  |
     * |                                  |
     * |                                  |
     * |                                  |
     * |                                  |
     * |                                  |
     * |   x   |-------|                  | 1000
     * |       |       |                  |
     * |       |       |                  |
     * |-------|   1   |                  | 500
     * |   0   |       |                  |
     * |       |       |                  |
     * |-------|-------|------------------|
     *       1000    2000
     *
     */

    Logger logger;
    Info info(logger);
    GlobalParam p = GlobalParam::roadef2018();
    std::vector<Defect> vd {
        {.id = 0, .plate_id = 0, .pos = {.x = 500, .y = 995}, .rect = {.w = 10, .h = 10}}
    };
    std::vector<Item> vi {
        {.id = 0, .rect = {500, 1000}, .stack = 0, .sequence = 1},
        {.id = 1, .rect = {1000, 1000}, .stack = 0, .sequence = 2},
    };
    Instance ins(vi, vd, p);
    Solution sol(ins);

    Insertion i0 = {.j1 = 0, .j2 = -1, .df = -1, .x1 = 1000, .y2 = 500, .x3 = 1000, .x1_max = 3500, .y2_max = 3210, .z1 = 0, .z2 = 0};
    std::vector<Insertion> is0 = sol.all_valid_insertions(info);
    EXPECT_NE(std::find(is0.begin(), is0.end(), i0), is0.end());
    sol.add_item(i0, info);

    std::vector<Insertion> is {
        {.j1 = 1, .j2 = -1, .df = 2, .x1 = 2000, .y2 = 1020, .x3 = 2000, .x1_max = 3500, .y2_max = 3210, .z1 = 0, .z2 = 1},
        {.j1 = -1, .j2 = 1, .df = 1, .x1 = 1000, .y2 = 2005, .x3 = 1000, .x1_max = 3500, .y2_max = 3210, .z1 = 0, .z2 = 1},
        {.j1 = -1, .j2 = -1, .df = 1, .x1 = 1000, .y2 = 1005, .x3 = 510, .x1_max = 3500, .y2_max = 3210, .z1 = 0, .z2 = 1},
    };
    EXPECT_EQ(sol.all_valid_insertions(info), is);
}

