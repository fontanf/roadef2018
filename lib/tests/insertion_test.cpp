#include "roadef2018/lib/instance.hpp"
#include "roadef2018/lib/solution.hpp"

#include <gtest/gtest.h>

using namespace roadef2018;

TEST(Solution, InsertionDFm1I)
{
    /**
     *
     * |--------------------------------------------------| 3210
     * |                                                  |
     * |                                                  |
     * |                                                  |
     * |                                                  |
     * |                                                  |
     * |                                                  |
     * |                                                  |
     * |                                                  |
     * |-------------------|                              | 500
     * |         0         | 500                          |
     * |-------------------|------------------------------|
     *                   1000                           6000
     */

    Info info;
    GlobalParam p = GlobalParam::roadef2018();
    std::vector<Defect> vd;

    std::vector<Item> vi {
        {.id = 0, .rect = {500, 1000}, .stack = 0, .sequence = 0},
    };
    Instance ins(vi, vd, p);
    Solution sol(ins);

    std::vector<Insertion> is {
        {.j1 = 0, .j2 = -1, .df = -1, .x1 = 1000, .y2 = 500, .x3 = 1000, .x1_max = 3500, .y2_max = 3210, .z1 = 0, .z2 = 0},
        {.j1 = 0, .j2 = -1, .df = -1, .x1 = 500, .y2 = 1000, .x3 = 500, .x1_max = 3500, .y2_max = 3210, .z1 = 0, .z2 = 0},
    };

    EXPECT_EQ(sol.all_valid_insertions(info), is);
}

TEST(Solution, InsertionDfm1II)
{
    Info info;
    GlobalParam p = GlobalParam::roadef2018();
    std::vector<Defect> vd;

    std::vector<Item> vi {
        {.id = 0, .rect = {3000, 3210}, .stack = 0, .sequence = 1},
        {.id = 1, .rect = {3000, 3210}, .stack = 0, .sequence = 2},
        {.id = 2, .rect = {3000, 3210}, .stack = 0, .sequence = 3},
    };
    Instance ins(vi, vd, p);
    Solution sol(ins);

    Insertion i0 = {.j1 = 0, .j2 = -1, .df = -1, .x1 = 3000, .y2 = 3210, .x3 = 3000, .x1_max = 3500, .y2_max = 3210, .z1 = 0, .z2 = 0};
    std::vector<Insertion> is0 = sol.all_valid_insertions(info);
    EXPECT_NE(std::find(is0.begin(), is0.end(), i0), is0.end());
    sol.add_item(i0, info);

    Insertion i1 = {.j1 = 1, .j2 = -1, .df = 0, .x1 = 6000, .y2 = 3210, .x3 = 6000, .x1_max = 6000, .y2_max = 3210, .z1 = 0, .z2 = 0};
    std::vector<Insertion> is1 = sol.all_valid_insertions(info);
    EXPECT_NE(std::find(is1.begin(), is1.end(), i1), is1.end());
    sol.add_item(i1, info);

    Insertion i2 = {.j1 = 2, .j2 = -1, .df = -1, .x1 = 3000, .y2 = 3210, .x3 = 3000, .x1_max = 3500, .y2_max = 3210, .z1 = 0, .z2 = 0};
    std::vector<Insertion> is2 = sol.all_valid_insertions(info);
    EXPECT_NE(std::find(is2.begin(), is2.end(), i2), is2.end());
    sol.add_item(i2, info);
}

TEST(Solution, InsertionDf0)
{
    /**
     *
     * |--------|---------------------------------------| 3210
     * |        |                                       |
     * |        |                                       |
     * |        |                                       |
     * |        |                                       |
     * |    0   |                                       |
     * |        |------------------|                    | 1000
     * |        |                  |                    |
     * |        |        1         |                    |
     * |        |                  |                    |
     * |--------|------------------|--------------------|
     *        1000               2500                 6000
     */

    Info info;
    GlobalParam p = GlobalParam::roadef2018();
    std::vector<Defect> vd;

    std::vector<Item> vi {
        {.id = 0, .rect = {1000, 3210}, .stack = 0, .sequence = 1},
        {.id = 1, .rect = {1000, 1500}, .stack = 0, .sequence = 2},
    };
    Instance ins(vi, vd, p);
    Solution sol(ins);

    Insertion i0 = {.j1 = 0, .j2 = -1, .df = -1, .x1 = 1000, .y2 = 3210, .x3 = 1000, .x1_max = 3500, .y2_max = 3210, .z1 = 0, .z2 = 0};
    std::vector<Insertion> is0 = sol.all_valid_insertions(info);
    EXPECT_NE(std::find(is0.begin(), is0.end(), i0), is0.end());
    sol.add_item(i0, info);

    std::vector<Insertion> is {
        //{.j1 = 1, .j2 = -1, .df = 2, .x1 = 2500, .y2 = 3210, .x3 = 2500, .x1_max = 3500, .y2_max = 3210, .z1 = 0, .z2 = 0},
        //{.j1 = 1, .j2 = -1, .df = 2, .x1 = 2000, .y2 = 3210, .x3 = 2000, .x1_max = 3500, .y2_max = 3210, .z1 = 0, .z2 = 0},
        {.j1 = 1, .j2 = -1, .df = 0, .x1 = 2500, .y2 = 1000, .x3 = 2500, .x1_max = 4500, .y2_max = 3210, .z1 = 0, .z2 = 0},
        {.j1 = 1, .j2 = -1, .df = 0, .x1 = 2000, .y2 = 1500, .x3 = 2000, .x1_max = 4500, .y2_max = 3210, .z1 = 0, .z2 = 0},
    };
    EXPECT_EQ(sol.all_valid_insertions(info), is);
}

TEST(Solution, InsertionDf1I)
{
    /**
     *
     * |--------|----------------------------------------|
     * |        |                                        |
     * |        |                                        |
     * |        |                                        |
     * |        |                                        |
     * |        |------------------|                     | 2100
     * |        |                  |                     |
     * |   0    |                  |                     |
     * |        |        2         |                     |
     * |        |                  |                     |
     * |        |------------------|----------|          | 1000
     * |        |                             |          |
     * |        |             1               |          |
     * |        |                             |          |
     * |--------|-----------------------------|----------|
     * 0      1000               3000       4000       6000
     *
     */

    Info info;
    GlobalParam p = GlobalParam::roadef2018();
    std::vector<Defect> vd;

    std::vector<Item> vi {
        {.id = 0, .rect = {1000, 3210}, .stack = 0, .sequence = 1},
        {.id = 1, .rect = {1000, 3000}, .stack = 0, .sequence = 2},
        {.id = 2, .rect = {1100, 2000}, .stack = 0, .sequence = 3},
    };
    Instance ins(vi, vd, p);
    Solution sol(ins);

    Insertion i0 = {.j1 = 0, .j2 = -1, .df = -1, .x1 = 1000, .y2 = 3210, .x3 = 1000, .x1_max = 3500, .y2_max = 3210, .z1 = 0, .z2 = 0};
    std::vector<Insertion> is0 = sol.all_valid_insertions(info);
    EXPECT_NE(std::find(is0.begin(), is0.end(), i0), is0.end());
    sol.add_item(i0, info);

    Insertion i1 = {.j1 = 1, .j2 = -1, .df = 0, .x1 = 4000, .y2 = 1000, .x3 = 4000, .x1_max = 4500, .y2_max = 3210, .z1 = 0, .z2 = 0};
    std::vector<Insertion> is1 = sol.all_valid_insertions(info);
    EXPECT_NE(std::find(is1.begin(), is1.end(), i1), is1.end());
    sol.add_item(i1, info);

    std::vector<Insertion> is {
        {.j1 = 2, .j2 = -1, .df = 1, .x1 = 4000, .y2 = 2100, .x3 = 3000, .x1_max = 4500, .y2_max = 3210, .z1 = 0, .z2 = 0},
        {.j1 = 2, .j2 = -1, .df = 1, .x1 = 4000, .y2 = 3000, .x3 = 2100, .x1_max = 4500, .y2_max = 3210, .z1 = 0, .z2 = 0},
    };

    EXPECT_EQ(sol.all_valid_insertions(info), is);
}

TEST(Solution, InsertionDf1II)
{
    /**
     *
     * |--------|----------------------------------------| 3210
     * |        |                                        |
     * |        |                                        |
     * |        |                                        |
     * |        |--------------------------|             | 2100
     * |        |                          |             |
     * |   0    |            2             |             |
     * |        |                          |             |
     * |        |--------------------------|--|          | 1000
     * |        |                             |          |
     * |        |             1               |          |
     * |        |                             |          |
     * |--------|-----------------------------|----------|
     *        1000                      3990 4000       6000
     */

    Info info;
    GlobalParam p = GlobalParam::roadef2018();
    std::vector<Defect> vd;

    std::vector<Item> vi {
        {.id = 0, .rect = {1000, 3210}, .stack = 0, .sequence = 1},
        {.id = 1, .rect = {1000, 3000}, .stack = 0, .sequence = 2},
        {.id = 2, .rect = {1100, 2990}, .stack = 0, .sequence = 3},
    };
    Instance ins(vi, vd, p);
    Solution sol(ins);

    Insertion i0 = {.j1 = 0, .j2 = -1, .df = -1, .x1 = 1000, .y2 = 3210, .x3 = 1000, .x1_max = 3500, .y2_max = 3210, .z1 = 0, .z2 = 0};
    std::vector<Insertion> is0 = sol.all_valid_insertions(info);
    EXPECT_NE(std::find(is0.begin(), is0.end(), i0), is0.end());
    sol.add_item(i0, info);

    Insertion i1 = {.j1 = 1, .j2 = -1, .df = 0, .x1 = 4000, .y2 = 1000, .x3 = 4000, .x1_max = 4500, .y2_max = 3210, .z1 = 0, .z2 = 0};
    std::vector<Insertion> is1 = sol.all_valid_insertions(info);
    EXPECT_NE(std::find(is1.begin(), is1.end(), i1), is1.end());
    sol.add_item(i1, info);

    std::vector<Insertion> is {
        {.j1 = 2, .j2 = -1, .df = 1, .x1 = 4020, .y2 = 2100, .x3 = 3990, .x1_max = 4500, .y2_max = 3210, .z1 = 1, .z2 = 0},
        //{.j1 = 2, .j2 = -1, .df = 0, .x1 = 5100, .y2 = 2990, .x3 = 5100, .x1_max = 6000, .y2_max = 3210, .z1 = 0, .z2 = 0},
    };
    EXPECT_EQ(sol.all_valid_insertions(info), is);
}

TEST(Solution, InsertionDf1III)
{
    /**
     *
     * |--------|----------------------------------------| 3210
     * |        |                                        |
     * |        |                                        |
     * |        |                                        |
     * |        |-----------------------------|          | 2100
     * |        |                             |          |
     * |   0    |            2                |          |
     * |        |                             |          |
     * |        |--------------------------|--|          | 1000
     * |        |                          |             |
     * |        |             1            |             |
     * |        |                          |             |
     * |--------|--------------------------|-------------|
     *        1000                      4000 4010       6000
     *
     */

    Info info;
    GlobalParam p = GlobalParam::roadef2018();
    std::vector<Defect> vd;
    std::vector<Item> vi {
        {.id = 0, .rect = {1000, 3210}, .stack = 0, .sequence = 1},
        {.id = 1, .rect = {1000, 3000}, .stack = 0, .sequence = 2},
        {.id = 2, .rect = {1100, 3010}, .stack = 0, .sequence = 3},
    };
    Instance ins(vi, vd, p);

    Solution sol(ins);

    Insertion i0 = {.j1 = 0, .j2 = -1, .df = -1, .x1 = 1000, .y2 = 3210, .x3 = 1000, .x1_max = 3500, .y2_max = 3210, .z1 = 0, .z2 = 0};
    std::vector<Insertion> is0 = sol.all_valid_insertions(info);
    EXPECT_NE(std::find(is0.begin(), is0.end(), i0), is0.end());
    sol.add_item(i0, info);

    Insertion i1 = {.j1 = 1, .j2 = -1, .df = 0, .x1 = 4000, .y2 = 1000, .x3 = 4000, .x1_max = 4500, .y2_max = 3210, .z1 = 0, .z2 = 0};
    std::vector<Insertion> is1 = sol.all_valid_insertions(info);
    EXPECT_NE(std::find(is1.begin(), is1.end(), i1), is1.end());
    sol.add_item(i1, info);

    std::vector<Insertion> is {
        {.j1 = 2, .j2 = -1, .df = 1, .x1 = 4030, .y2 = 2100, .x3 = 4010, .x1_max = 4500, .y2_max = 3210, .z1 = 1, .z2 = 0},
        //{.j1 = 2, .j2 = -1, .df = 0, .x1 = 5100, .y2 = 3010, .x3 = 5100, .x1_max = 6000, .y2_max = 3210, .z1 = 0, .z2 = 0},
    };
    EXPECT_EQ(sol.all_valid_insertions(info), is);
}

TEST(Solution, InsertionDf1IV)
{
    /**
     *
     * |--------|----------------------------------------| 3210
     * |        |                                        |
     * |        |                                        |
     * |        |                                        |
     * |        |-----------------------------|          | 2100
     * |        |                             |          |
     * |   0    |            2                |          |
     * |        |                             |          |
     * |        |--------------------------|--|          | 1000
     * |        |                          |             |
     * |        |             1            |             |
     * |        |                          |             |
     * |--------|--------------------------|-------------|
     *        1000                      4000 4020       6000
     *
     */

    Info info;
    GlobalParam p = GlobalParam::roadef2018();
    std::vector<Defect> vd;
    std::vector<Item> vi {
        {.id = 0, .rect = {1000, 3210}, .stack = 0, .sequence = 1},
        {.id = 1, .rect = {1000, 3000}, .stack = 0, .sequence = 2},
        {.id = 2, .rect = {1100, 3020}, .stack = 0, .sequence = 3},
    };
    Instance ins(vi, vd, p);

    Solution sol(ins);

    Insertion i0 = {.j1 = 0, .j2 = -1, .df = -1, .x1 = 1000, .y2 = 3210, .x3 = 1000, .x1_max = 3500, .y2_max = 3210, .z1 = 0, .z2 = 0};
    std::vector<Insertion> is0 = sol.all_valid_insertions(info);
    EXPECT_NE(std::find(is0.begin(), is0.end(), i0), is0.end());
    sol.add_item(i0, info);

    Insertion i1 = {.j1 = 1, .j2 = -1, .df = 0, .x1 = 4000, .y2 = 1000, .x3 = 4000, .x1_max = 4500, .y2_max = 3210, .z1 = 0, .z2 = 0};
    std::vector<Insertion> is1 = sol.all_valid_insertions(info);
    EXPECT_NE(std::find(is1.begin(), is1.end(), i1), is1.end());
    sol.add_item(i1, info);

    std::vector<Insertion> is {
        {.j1 = 2, .j2 = -1, .df = 1, .x1 = 4020, .y2 = 2100, .x3 = 4020, .x1_max = 4500, .y2_max = 3210, .z1 = 0, .z2 = 0},
        //{.j1 = 2, .j2 = -1, .df = 0, .x1 = 5100, .y2 = 3020, .x3 = 5100, .x1_max = 6000, .y2_max = 3210, .z1 = 0, .z2 = 0},
    };
    EXPECT_EQ(sol.all_valid_insertions(info), is);
}

TEST(Solution, InsertionDf1V)
{
    /**
     *
     * |--------|---------------------------------|------| 3210
     * |        |                                 |      |
     * |        |                                 |      |
     * |        |               3                 |      |
     * |        |                                 |      |
     * |        |-----------------------------|---|      | 2100
     * |        |                             |          |
     * |   0    |              2              |          |
     * |        |                             |          |
     * |        |--------------------------|--|          | 1000
     * |        |                          |             |
     * |        |             1            |             |
     * |        |                          |             |
     * |--------|--------------------------|-------------|
     *        1000                      4000 4010 4050  6000
     *
     */

    Info info;
    GlobalParam p = GlobalParam::roadef2018();
    std::vector<Defect> vd;
    std::vector<Item> vi {
        {.id = 0, .rect = {1000, 3210}, .stack = 0, .sequence = 1},
        {.id = 1, .rect = {1000, 3000}, .stack = 0, .sequence = 2},
        {.id = 2, .rect = {1100, 3010}, .stack = 0, .sequence = 3},
        {.id = 3, .rect = {1110, 3050}, .stack = 0, .sequence = 3},
    };
    Instance ins(vi, vd, p);

    Solution sol(ins);

    Insertion i0 = {.j1 = 0, .j2 = -1, .df = -1, .x1 = 1000, .y2 = 3210, .x3 = 1000, .x1_max = 3500, .y2_max = 3210, .z1 = 0, .z2 = 0};
    std::vector<Insertion> is0 = sol.all_valid_insertions(info);
    EXPECT_NE(std::find(is0.begin(), is0.end(), i0), is0.end());
    sol.add_item(i0, info);

    Insertion i1 = {.j1 = 1, .j2 = -1, .df = 0, .x1 = 4000, .y2 = 1000, .x3 = 4000, .x1_max = 4500, .y2_max = 3210, .z1 = 0, .z2 = 0};
    std::vector<Insertion> is1 = sol.all_valid_insertions(info);
    EXPECT_NE(std::find(is1.begin(), is1.end(), i1), is1.end());
    sol.add_item(i1, info);

    Insertion i2 = {.j1 = 2, .j2 = -1, .df = 1, .x1 = 4030, .y2 = 2100, .x3 = 4010, .x1_max = 4500, .y2_max = 3210, .z1 = 1, .z2 = 0};
    std::vector<Insertion> is2 = sol.all_valid_insertions(info);
    EXPECT_NE(std::find(is2.begin(), is2.end(), i2), is2.end());
    sol.add_item(i2, info);

    std::vector<Insertion> is {
        {.j1 = 3, .j2 = -1, .df = 1, .x1 = 4050, .y2 = 3210, .x3 = 4050, .x1_max = 4500, .y2_max = 3210, .z1 = 0, .z2 = 0},
        {.j1 = 3, .j2 = -1, .df = 0, .x1 = 5140, .y2 = 3050, .x3 = 5140, .x1_max = 6000, .y2_max = 3210, .z1 = 0, .z2 = 0},
    };
    EXPECT_EQ(sol.all_valid_insertions(info), is);
}

TEST(Solution, InsertionDf1MinWasteI)
{
    /**
     *
     * |-------------------------------------------------|
     * |                                                 |
     * |                                                 |
     * |---------|-----------|----------|                | 3000
     * |         |           |          |                |
     * |    1    |     2     |    3     |                |
     * |         |           |          |                |
     * |---------|---------|-|----------|                | 1500
     * |                   |                             |
     * |         0         |                             |
     * |                   |                             |
     * |-------------------|-----------------------------|
     *          1000    2000 2010      3010
     */

    Info info;
    GlobalParam p = GlobalParam::roadef2018();
    std::vector<Defect> vd;

    std::vector<Item> vi {
        {.id = 0, .rect = {1500, 2000}, .stack = 0, .sequence = 1},
        {.id = 1, .rect = {1000, 1500}, .stack = 0, .sequence = 2},
        {.id = 2, .rect = {1010, 1500}, .stack = 0, .sequence = 3},
        {.id = 3, .rect = {1000, 1500}, .stack = 0, .sequence = 4},
    };
    Instance ins(vi, vd, p);
    Solution sol(ins);

    Insertion i0 = {.j1 = 0, .j2 = -1, .df = -1, .x1 = 2000, .y2 = 1500, .x3 = 2000, .x1_max = 3500, .y2_max = 3210, .z1 = 0, .z2 = 0};
    std::vector<Insertion> is0 = sol.all_valid_insertions(info);
    EXPECT_NE(std::find(is0.begin(), is0.end(), i0), is0.end());
    sol.add_item(i0, info);

    Insertion i1 = {.j1 = 1, .j2 = -1, .df = 1, .x1 = 2000, .y2 = 3000, .x3 = 1000, .x1_max = 3500, .y2_max = 3210, .z1 = 0, .z2 = 0};
    std::vector<Insertion> is1 = sol.all_valid_insertions(info);
    EXPECT_NE(std::find(is1.begin(), is1.end(), i1), is1.end());
    sol.add_item(i1, info);

    Insertion i2 = {.j1 = 2, .j2 = -1, .df = 2, .x1 = 2030, .y2 = 3000, .x3 = 2010, .x1_max = 3500, .y2_max = 3210, .z1 = 1, .z2 = 0};
    std::vector<Insertion> is2 = sol.all_valid_insertions(info);
    EXPECT_NE(std::find(is2.begin(), is2.end(), i2), is2.end());
    sol.add_item(i2, info);

    std::vector<Insertion> is {
        {.j1 = 3, .j2 = -1, .df = 2, .x1 = 3010, .y2 = 3000, .x3 = 3010, .x1_max = 3500, .y2_max = 3210, .z1 = 0, .z2 = 0},
        {.j1 = 3, .j2 = -1, .df = 0, .x1 = 3530, .y2 = 1000, .x3 = 3530, .x1_max = 5530, .y2_max = 3210, .z1 = 0, .z2 = 0},
        {.j1 = 3, .j2 = -1, .df = 0, .x1 = 3030, .y2 = 1500, .x3 = 3030, .x1_max = 5530, .y2_max = 3210, .z1 = 0, .z2 = 0},
    };
    EXPECT_EQ(sol.all_valid_insertions(info), is);
}

TEST(Solution, InsertionDf1MinWasteII)
{
    /**
     *
     * |-----------------------|----------| 3210
     * |                       |          |
     * |           2           |          |
     * |                       |          |
     * |---------------------|-|          | 2010
     * |                     |            |
     * |          1          |            |
     * |                     |            |
     * |-------------------|-|            | 1000
     * |                   |              |
     * |         0         |              |
     * |                   |              |
     * |-------------------|--------------|
     *                  2000 2005 2010
     */

    Info info;
    GlobalParam p = GlobalParam::roadef2018();
    std::vector<Defect> vd;

    std::vector<Item> vi {
        {.id = 0, .rect = {1000, 2000}, .stack = 0, .sequence = 0},
        {.id = 1, .rect = {1010, 2005}, .stack = 0, .sequence = 1},
        {.id = 2, .rect = {1200, 2010}, .stack = 0, .sequence = 2},
    };
    Instance ins(vi, vd, p);
    Solution sol(ins);

    Insertion i0 = {.j1 = 0, .j2 = -1, .df = -1, .x1 = 2000, .y2 = 1000, .x3 = 2000, .x1_max = 3500, .y2_max = 3210, .z1 = 0, .z2 = 0};
    std::vector<Insertion> is0 = sol.all_valid_insertions(info);
    EXPECT_NE(std::find(is0.begin(), is0.end(), i0), is0.end());
    sol.add_item(i0, info);

    Insertion i1 = {.j1 = 1, .j2 = -1, .df = 1, .x1 = 2025, .y2 = 2010, .x3 = 2005, .x1_max = 3500, .y2_max = 3210, .z1 = 1, .z2 = 0};
    std::vector<Insertion> is1 = sol.all_valid_insertions(info);
    EXPECT_NE(std::find(is1.begin(), is1.end(), i1), is1.end());
    sol.add_item(i1, info);

    std::vector<Insertion> is {
        {.j1 = 2, .j2 = -1, .df = 2, .x1 = 3205, .y2 = 3010, .x3 = 3205, .x1_max = 3500, .y2_max = 3210, .z1 = 0, .z2 = 0},
        {.j1 = 2, .j2 = -1, .df = 1, .x1 = 2030, .y2 = 3210, .x3 = 2010, .x1_max = 3500, .y2_max = 3210, .z1 = 1, .z2 = 0},
        {.j1 = 2, .j2 = -1, .df = 0, .x1 = 4035, .y2 = 1200, .x3 = 4035, .x1_max = 5525, .y2_max = 3210, .z1 = 0, .z2 = 0},
        //{.j1 = 2, .j2 = -1, .df = 0, .x1 = 3225, .y2 = 2010, .x3 = 3225, .x1_max = 5525, .y2_max = 3210, .z1 = 0, .z2 = 0},
    };
    EXPECT_EQ(sol.all_valid_insertions(info), is);
}

TEST(Solution, InsertionDf2I)
{
    /**
     *
     * |--------|----|-----------------------------------| 3210
     * |        |    |                                   |
     * |        |    |                                   |
     * |        |    |                                   |
     * |        | 2  |----|                              | 2010
     * |        |    |    |                              |
     * |   0    |    |  3 |                              |
     * |        |----|----|--------|                     | 1000
     * |        |                  |                     |
     * |        |         1        |                     |
     * |        |                  |                     |
     * |--------|------------------|---------------------| 0
     *        1000 1500  2200    3000                  6000
     */

    Info info;
    GlobalParam p = GlobalParam::roadef2018();
    std::vector<Defect> vd;

    std::vector<Item> vi {
        {.id = 0, .rect = {1000, 3210}, .stack = 0, .sequence = 1},
        {.id = 1, .rect = {1000, 2000}, .stack = 0, .sequence = 2},
        {.id = 2, .rect = {500, 2210}, .stack = 0, .sequence = 3},
        {.id = 3, .rect = {700, 1010}, .stack = 0, .sequence = 4},
    };
    Instance ins(vi, vd, p);
    Solution sol(ins);

    Insertion i0 = {.j1 = 0, .j2 = -1, .df = -1, .x1 = 1000, .y2 = 3210, .x3 = 1000, .x1_max = 3500, .y2_max = 3210, .z1 = 0, .z2 = 0};
    std::vector<Insertion> is0 = sol.all_valid_insertions(info);
    EXPECT_NE(std::find(is0.begin(), is0.end(), i0), is0.end());
    sol.add_item(i0, info);

    Insertion i1 = {.j1 = 1, .j2 = -1, .df = 0, .x1 = 3000, .y2 = 1000, .x3 = 3000, .x1_max = 4500, .y2_max = 3210, .z1 = 0, .z2 = 0};
    std::vector<Insertion> is1 = sol.all_valid_insertions(info);
    EXPECT_NE(std::find(is1.begin(), is1.end(), i1), is1.end());
    sol.add_item(i1, info);

    Insertion i2 = {.j1 = 2, .j2 = -1, .df = 1, .x1 = 3000, .y2 = 3210, .x3 = 1500, .x1_max = 4500, .y2_max = 3210, .z1 = 0, .z2 = 0};
    std::vector<Insertion> is2 = sol.all_valid_insertions(info);
    EXPECT_NE(std::find(is2.begin(), is2.end(), i2), is2.end());
    sol.add_item(i2, info);

    std::vector<Insertion> is {
        //{.j1 = 3, .j2 = -1, .df = 2, .x1 = 3000, .y2 = 3210, .x3 = 2510, .x1_max = 4500, .y2_max = 3210, .z1 = 0, .z2 = 0},
        {.j1 = 3, .j2 = -1, .df = 2, .x1 = 3000, .y2 = 3210, .x3 = 2200, .x1_max = 4500, .y2_max = 3210, .z1 = 0, .z2 = 0},
    };
    EXPECT_EQ(sol.all_valid_insertions(info), is);
}

TEST(Solution, InsertionDf2II)
{
    /**
     *
     * |--------|----------------------------------------| 3210
     * |        |                                        |
     * |        |                                        |
     * |        |----|                                   | 2010
     * |        |    |----|                              | 2000
     * |        |    |    |                              |
     * |   0    | 2  |  3 |                              |
     * |        |----|----|--------|                     | 1000
     * |        |                  |                     |
     * |        |         1        |                     |
     * |        |                  |                     |
     * |--------|------------------|---------------------| 0
     *        1000 1500  2200    3000                  6000
     */

    Info info;
    GlobalParam p = GlobalParam::roadef2018();
    std::vector<Defect> vd;

    std::vector<Item> vi {
        {.id = 0, .rect = {1000, 3210}, .stack = 0, .sequence = 1},
        {.id = 1, .rect = {1000, 2000}, .stack = 0, .sequence = 2},
        {.id = 2, .rect = {500, 1010}, .stack = 0, .sequence = 3},
        {.id = 3, .rect = {700, 1000}, .stack = 0, .sequence = 4},
    };
    Instance ins(vi, vd, p);
    Solution sol(ins);

    Insertion i0 = {.j1 = 0, .j2 = -1, .df = -1, .x1 = 1000, .y2 = 3210, .x3 = 1000, .x1_max = 3500, .y2_max = 3210, .z1 = 0, .z2 = 0};
    std::vector<Insertion> is0 = sol.all_valid_insertions(info);
    EXPECT_NE(std::find(is0.begin(), is0.end(), i0), is0.end());
    sol.add_item(i0, info);

    Insertion i1 = {.j1 = 1, .j2 = -1, .df = 0, .x1 = 3000, .y2 = 1000, .x3 = 3000, .x1_max = 4500, .y2_max = 3210, .z1 = 0, .z2 = 0};
    std::vector<Insertion> is1 = sol.all_valid_insertions(info);
    EXPECT_NE(std::find(is1.begin(), is1.end(), i1), is1.end());
    sol.add_item(i1, info);

    Insertion i2 = {.j1 = 2, .j2 = -1, .df = 1, .x1 = 3000, .y2 = 2010, .x3 = 1500, .x1_max = 4500, .y2_max = 3210, .z1 = 0, .z2 = 0};
    std::vector<Insertion> is2 = sol.all_valid_insertions(info);
    EXPECT_NE(std::find(is2.begin(), is2.end(), i2), is2.end());
    sol.add_item(i2, info);

    std::vector<Insertion> is {
        {.j1 = 3, .j2 = -1, .df = 2, .x1 = 3000, .y2 = 2010, .x3 = 2500, .x1_max = 4500, .y2_max = 3210, .z1 = 0, .z2 = 0},
        {.j1 = 3, .j2 = -1, .df = 2, .x1 = 3000, .y2 = 2030, .x3 = 2200, .x1_max = 4500, .y2_max = 3210, .z1 = 0, .z2 = 1},
    };
    EXPECT_EQ(sol.all_valid_insertions(info), is);
}

TEST(Solution, InsertionDf2III)
{
    /**
     *
     * |--------|----------------------------------------| 3210
     * |        |                                        |
     * |        |                                        |
     * |        |    |----|                              | 2010
     * |        |----|    |                              | 2000
     * |        |    |    |                              |
     * |   0    | 2  |  3 |                              |
     * |        |----|----|--------|                     | 1000
     * |        |                  |                     |
     * |        |         1        |                     |
     * |        |                  |                     |
     * |--------|------------------|---------------------| 0
     *        1000 1500  2200    3000                  6000
     */

    Info info;
    GlobalParam p = GlobalParam::roadef2018();
    std::vector<Defect> vd;

    std::vector<Item> vi {
        {.id = 0, .rect = {1000, 3210}, .stack = 0, .sequence = 1},
        {.id = 1, .rect = {1000, 2000}, .stack = 0, .sequence = 2},
        {.id = 2, .rect = {500, 1000}, .stack = 0, .sequence = 3},
        {.id = 3, .rect = {700, 1010}, .stack = 0, .sequence = 4},
    };
    Instance ins(vi, vd, p);
    Solution sol(ins);

    Insertion i0 = {.j1 = 0, .j2 = -1, .df = -1, .x1 = 1000, .y2 = 3210, .x3 = 1000, .x1_max = 3500, .y2_max = 3210, .z1 = 0, .z2 = 0};
    std::vector<Insertion> is0 = sol.all_valid_insertions(info);
    EXPECT_NE(std::find(is0.begin(), is0.end(), i0), is0.end());
    sol.add_item(i0, info);

    Insertion i1 = {.j1 = 1, .j2 = -1, .df = 0, .x1 = 3000, .y2 = 1000, .x3 = 3000, .x1_max = 4500, .y2_max = 3210, .z1 = 0, .z2 = 0};
    std::vector<Insertion> is1 = sol.all_valid_insertions(info);
    EXPECT_NE(std::find(is1.begin(), is1.end(), i1), is1.end());
    sol.add_item(i1, info);

    Insertion i2 = {.j1 = 2, .j2 = -1, .df = 1, .x1 = 3000, .y2 = 2000, .x3 = 1500, .x1_max = 4500, .y2_max = 3210, .z1 = 0, .z2 = 0};
    std::vector<Insertion> is2 = sol.all_valid_insertions(info);
    EXPECT_NE(std::find(is2.begin(), is2.end(), i2), is2.end());
    sol.add_item(i2, info);

    std::vector<Insertion> is {
        {.j1 = 3, .j2 = -1, .df = 2, .x1 = 3000, .y2 = 2000, .x3 = 2510, .x1_max = 4500, .y2_max = 3210, .z1 = 0, .z2 = 0},
        {.j1 = 3, .j2 = -1, .df = 2, .x1 = 3000, .y2 = 2030, .x3 = 2200, .x1_max = 4500, .y2_max = 3210, .z1 = 0, .z2 = 1},
    };
    EXPECT_EQ(sol.all_valid_insertions(info), is);
}

TEST(Solution, InsertionDf2IV)
{
    /**
     *
     * |--------|----------------------------------------| 3210
     * |        |         |--------|                     | 3000
     * |        |         |        |                     |
     * |        |---------|        |                     | 2500
     * |        |         |    3   |                     |
     * |        |    2    |        |                     |
     * |   0    |         |        |                     |
     * |        |---------|--------|                     | 1000
     * |        |                  |                     |
     * |        |         1        |                     |
     * |        |                  |                     |
     * |--------|------------------|---------------------| 0
     *        1000      2000     3000                  6000
     */

    Info info;
    GlobalParam p = GlobalParam::roadef2018();
    std::vector<Defect> vd;

    std::vector<Item> vi {
        {.id = 0, .rect = {1000, 3210}, .stack = 0, .sequence = 1},
        {.id = 1, .rect = {1000, 2000}, .stack = 0, .sequence = 2},
        {.id = 2, .rect = {1000, 1500}, .stack = 0, .sequence = 3},
        {.id = 3, .rect = {1000, 2000}, .stack = 0, .sequence = 4},
    };
    Instance ins(vi, vd, p);
    Solution sol(ins);

    Insertion i0 = {.j1 = 0, .j2 = -1, .df = -1, .x1 = 1000, .y2 = 3210, .x3 = 1000, .x1_max = 3500, .y2_max = 3210, .z1 = 0, .z2 = 0};
    std::vector<Insertion> is0 = sol.all_valid_insertions(info);
    EXPECT_NE(std::find(is0.begin(), is0.end(), i0), is0.end());
    sol.add_item(i0, info);

    Insertion i1 = {.j1 = 1, .j2 = -1, .df = 0, .x1 = 3000, .y2 = 1000, .x3 = 3000, .x1_max = 4500, .y2_max = 3210, .z1 = 0, .z2 = 0};
    std::vector<Insertion> is1 = sol.all_valid_insertions(info);
    EXPECT_NE(std::find(is1.begin(), is1.end(), i1), is1.end());
    sol.add_item(i1, info);

    Insertion i2 = {.j1 = 2, .j2 = -1, .df = 1, .x1 = 3000, .y2 = 2500, .x3 = 2000, .x1_max = 4500, .y2_max = 3210, .z1 = 0, .z2 = 0};
    std::vector<Insertion> is2 = sol.all_valid_insertions(info);
    EXPECT_NE(std::find(is2.begin(), is2.end(), i2), is2.end());
    sol.add_item(i2, info);

    std::vector<Insertion> is {
        {.j1 = 3, .j2 = -1, .df = 2, .x1 = 4000, .y2 = 2500, .x3 = 4000, .x1_max = 4500, .y2_max = 3210, .z1 = 0, .z2 = 0},
        {.j1 = 3, .j2 = -1, .df = 2, .x1 = 3000, .y2 = 3000, .x3 = 3000, .x1_max = 4500, .y2_max = 3210, .z1 = 0, .z2 = 0},
    };
    EXPECT_EQ(sol.all_valid_insertions(info), is);
}

TEST(Solution, InsertionDf2V)
{
    /**
     *
     * |-------------------------------------------------|
     * |   |----|                                        | 3000
     * |---|    |                                        | 2500
     * |   |    |                                        |
     * | 1 |  2 |                                        |
     * |   |    |                                        |
     * |---|----|                                        | 1500
     * |        |                                        |
     * |        |                                        |
     * |    0   |                                        |
     * |        |                                        |
     * |--------|----------------------------------------| 0
     *    500 1000
     */

    Info info;
    GlobalParam p = GlobalParam::roadef2018();
    std::vector<Defect> vd;

    std::vector<Item> vi {
        {.id = 0, .rect = {1000, 1500}, .stack = 0, .sequence = 1},
        {.id = 1, .rect = {500, 1000}, .stack = 0, .sequence = 2},
        {.id = 2, .rect = {500, 1500}, .stack = 0, .sequence = 3},
    };
    Instance ins(vi, vd, p);
    Solution sol(ins);

    Insertion i0 = {.j1 = 0, .j2 = -1, .df = -1, .x1 = 1000, .y2 = 1500, .x3 = 1000, .x1_max = 3500, .y2_max = 3210, .z1 = 0, .z2 = 0};
    std::vector<Insertion> is0 = sol.all_valid_insertions(info);
    EXPECT_NE(std::find(is0.begin(), is0.end(), i0), is0.end());
    sol.add_item(i0, info);

    Insertion i1 = {.j1 = 1, .j2 = -1, .df = 1, .x1 = 1000, .y2 = 2500, .x3 = 500, .x1_max = 3500, .y2_max = 3210, .z1 = 0, .z2 = 0};
    std::vector<Insertion> is1 = sol.all_valid_insertions(info);
    EXPECT_NE(std::find(is1.begin(), is1.end(), i1), is1.end());
    sol.add_item(i1, info);

    std::vector<Insertion> is {
        {.j1 = 2, .j2 = -1, .df = 2, .x1 = 2000, .y2 = 2500, .x3 = 2000, .x1_max = 3500, .y2_max = 3210, .z1 = 0, .z2 = 0},
        {.j1 = 2, .j2 = -1, .df = 2, .x1 = 1000, .y2 = 3000, .x3 = 1000, .x1_max = 3500, .y2_max = 3210, .z1 = 0, .z2 = 0},
    };
    EXPECT_EQ(sol.all_valid_insertions(info), is);
}

TEST(Solution, InsertionX1)
{
    /**
     *
     * |-------------------------------------------------|
     * |                                                 |
     * |                                                 |
     * |                                                 |
     * |                                                 |
     * |                                                 |
     * |                                                 |
     * |                                                 |
     * |                                                 |
     * |                                                 |
     * |-------------------|                             | 1500
     * |         2         |                             |
     * |------------------||                             | 1000
     * |        1         |                              |
     * |-----------------||                              | 500
     * |       0         |                               |
     * |-----------------|-------------------------------|
     *                 980
     *                  990
     *                   1000
     *
     */

    Info info;
    GlobalParam p = GlobalParam::roadef2018();
    std::vector<Defect> vd;

    std::vector<Item> vi {
        {.id = 0, .rect = {500, 980}, .stack = 0, .sequence = 1},
        {.id = 1, .rect = {500, 990}, .stack = 1, .sequence = 1},
        {.id = 2, .rect = {500, 1000}, .stack = 2, .sequence = 1},
    };
    Instance ins(vi, vd, p);
    Solution sol(ins);

    Insertion i0 = {.j1 = 0, .j2 = -1, .df = -1, .x1 = 980, .y2 = 500, .x3 = 980, .x1_max = 3500, .y2_max = 3210, .z1 = 0, .z2 = 0};
    std::vector<Insertion> is0 = sol.all_valid_insertions(info);
    EXPECT_NE(std::find(is0.begin(), is0.end(), i0), is0.end());
    sol.add_item(i0, info);

    Insertion i1 = {.j1 = 1, .j2 = -1, .df = 1, .x1 = 1010, .y2 = 1000, .x3 = 990, .x1_max = 3500, .y2_max = 3210, .z1 = 1, .z2 = 0};
    std::vector<Insertion> is1 = sol.all_valid_insertions(info);
    EXPECT_NE(std::find(is1.begin(), is1.end(), i1), is1.end());
    sol.add_item(i1, info);

    std::vector<Insertion> is {
        {.j1 = 2, .j2 = -1, .df = 2, .x1 = 1990, .y2 = 1000, .x3 = 1990, .x1_max = 3500, .y2_max = 3210, .z1 = 0, .z2 = 0},
        {.j1 = 2, .j2 = -1, .df = 1, .x1 = 1020, .y2 = 1500, .x3 = 1000, .x1_max = 3500, .y2_max = 3210, .z1 = 1, .z2 = 0},
        {.j1 = 2, .j2 = -1, .df = 1, .x1 = 1010, .y2 = 2000, .x3 = 500, .x1_max = 3500, .y2_max = 3210, .z1 = 1, .z2 = 0},
    };
    EXPECT_EQ(sol.all_valid_insertions(info), is);
}

TEST(Solution, InsertionX2)
{
    /**
     * Test from a bugged solution of instanace A5.
     */

    Info info;
    GlobalParam p = GlobalParam::roadef2018();
    std::vector<Defect> vd;

    std::vector<Item> vi {
        {.id = 0, .rect = {431, 658}, .stack = 0, .sequence = 1},
        {.id = 1, .rect = {431, 1170}, .stack = 0, .sequence = 2},
        {.id = 2, .rect = {303, 1054}, .stack = 0, .sequence = 3},
        {.id = 3, .rect = {568, 1399}, .stack = 0, .sequence = 4},
        {.id = 4, .rect = {545, 872}, .stack = 0, .sequence = 5},
    };
    Instance ins(vi, vd, p);
    Solution sol(ins);

    Insertion i0 = {.j1 = 0, .j2 = -1, .df = -1, .x1 = 431, .y2 = 658, .x3 = 431, .x1_max = 3500, .y2_max = 3210, .z1 = 0, .z2 = 0};
    std::vector<Insertion> is0 = sol.all_valid_insertions(info);
    EXPECT_NE(std::find(is0.begin(), is0.end(), i0), is0.end());
    sol.add_item(i0, info);

    Insertion i1 = {.j1 = 1, .j2 = -1, .df = 2, .x1 = 862, .y2 = 1170, .x3 = 862, .x1_max = 3500, .y2_max = 3210, .z1 = 0, .z2 = 0};
    std::vector<Insertion> is1 = sol.all_valid_insertions(info);
    EXPECT_NE(std::find(is1.begin(), is1.end(), i1), is1.end());
    sol.add_item(i1, info);

    Insertion i2 = {.j1 = 2, .j2 = -1, .df = 1, .x1 = 862, .y2 = 2224, .x3 = 303, .x1_max = 3500, .y2_max = 3210, .z1 = 0, .z2 = 0};
    std::vector<Insertion> is2 = sol.all_valid_insertions(info);
    EXPECT_NE(std::find(is2.begin(), is2.end(), i2), is2.end());
    sol.add_item(i2, info);

    Insertion i3 = {.j1 = 3, .j2 = -1, .df = 2, .x1 = 891, .y2 = 2569, .x3 = 871, .x1_max = 3500, .y2_max = 3210, .z1 = 1, .z2 = 0};
    std::vector<Insertion> is3 = sol.all_valid_insertions(info);
    EXPECT_NE(std::find(is3.begin(), is3.end(), i3), is3.end());
    sol.add_item(i3, info);

    Insertion i4 = {.j1 = 4, .j2 = -1, .df = 1, .x1 = 892, .y2 = 3114, .x3 = 872, .x1_max = 3500, .y2_max = 3210, .z1 = 1, .z2 = 0};
    std::vector<Insertion> is4 = sol.all_valid_insertions(info);
    EXPECT_NE(std::find(is4.begin(), is4.end(), i4), is4.end());
    sol.add_item(i4, info);

    EXPECT_EQ(sol.width(), 892);
}

TEST(Solution, InsertionX3)
{
    /**
     *
     * |------------------------|------------------------|
     * |                        |                        |
     * |                        |                        |
     * |                        |                        |
     * |                        |                        |
     * |                        |                        |
     * |                        |                        |
     * |                        |-----------------------|| 1000
     * |           0            |                       ||
     * |                        |            1          ||
     * |                        |                       ||
     * |                        |----------------------||| 1000
     * |                        |                      | |
     * |                        |           0          | |
     * |                        |                      | |
     * |------------------------|----------------------|-|
     *                        3000                   5960
     *                                                5970
     */

    Info info;
    GlobalParam p = GlobalParam::roadef2018();
    std::vector<Defect> vd;

    std::vector<Item> vi {
        {.id = 0, .rect = {3000, 3210}, .stack = 0, .sequence = 1},
        {.id = 1, .rect = {1000, 2960}, .stack = 1, .sequence = 1},
        {.id = 2, .rect = {1000, 2970}, .stack = 2, .sequence = 1},
    };
    Instance ins(vi, vd, p);
    Solution sol(ins);

    Insertion i0 = {.j1 = 0, .j2 = -1, .df = -1, .x1 = 3000, .y2 = 3210, .x3 = 3000, .x1_max = 3500, .y2_max = 3210, .z1 = 0, .z2 = 0};
    std::vector<Insertion> is0 = sol.all_valid_insertions(info);
    EXPECT_NE(std::find(is0.begin(), is0.end(), i0), is0.end());
    sol.add_item(i0, info);
    EXPECT_EQ(sol.x1_curr(), 3000);

    Insertion i1 = {.j1 = 1, .j2 = -1, .df = 0, .x1 = 5960, .y2 = 1000, .x3 = 5960, .x1_max = 6000, .y2_max = 3210, .z1 = 0, .z2 = 0};
    std::vector<Insertion> is1 = sol.all_valid_insertions(info);
    EXPECT_NE(std::find(is1.begin(), is1.end(), i1), is1.end());
    sol.add_item(i1, info);
    EXPECT_EQ(sol.x1_prev(), 3000);

    std::vector<Insertion> is {
        {.j1 = 2, .j2 = -1, .df = 1, .x1 = 6000, .y2 = 2000, .x3 = 5970, .x1_max = 6000, .y2_max = 3210, .z1 = 0, .z2 = 0},
    };
    EXPECT_EQ(sol.all_valid_insertions(info), is);
}

TEST(Solution, InsertionY1)
{
    /**
     *
     * |-------------------------------------------------|
     * |                                                 |
     * |                                                 |
     * |                                                 |
     * |                                                 |
     * |                                                 |
     * |                                                 |
     * |                                                 |
     * |       |---|                                     | 1000
     * |   |---|   |                                     | 990
     * |---|   |   |                                     | 980
     * |   |   |   |                                     |
     * |   | 1 | 2 |                                     |
     * | 0 |   |   |                                     |
     * |   |   |   |                                     |
     * |   |   |   |                                     |
     * |---|---|---|-------------------------------------|
     *   500 1000 1500
     *
     */

    Info info;
    GlobalParam p = GlobalParam::roadef2018();
    std::vector<Defect> vd;

    std::vector<Item> vi {
        {.id = 0, .rect = {500, 980}, .stack = 0, .sequence = 1},
        {.id = 1, .rect = {500, 990}, .stack = 1, .sequence = 1},
        {.id = 2, .rect = {500, 1000}, .stack = 2, .sequence = 1},
    };
    Instance ins(vi, vd, p);
    Solution sol(ins);

    Insertion i0 = {.j1 = 0, .j2 = -1, .df = -1, .x1 = 500, .y2 = 980, .x3 = 500, .x1_max = 3500, .y2_max = 3210, .z1 = 0, .z2 = 0};
    std::vector<Insertion> is0 = sol.all_valid_insertions(info);
    EXPECT_NE(std::find(is0.begin(), is0.end(), i0), is0.end());
    sol.add_item(i0, info);

    Insertion i1 = {.j1 = 1, .j2 = -1, .df = 2, .x1 = 1000, .y2 = 1010, .x3 = 1000, .x1_max = 3500, .y2_max = 3210, .z1 = 0, .z2 = 1};
    std::vector<Insertion> is1 = sol.all_valid_insertions(info);
    EXPECT_NE(std::find(is1.begin(), is1.end(), i1), is1.end());
    sol.add_item(i1, info);

    std::vector<Insertion> is {
        {.j1 = 2, .j2 = -1, .df = 2, .x1 = 2000, .y2 = 1010, .x3 = 2000, .x1_max = 3500, .y2_max = 3210, .z1 = 0, .z2 = 1},
        {.j1 = 2, .j2 = -1, .df = 2, .x1 = 1500, .y2 = 1020, .x3 = 1500, .x1_max = 3500, .y2_max = 3210, .z1 = 0, .z2 = 1},
        {.j1 = 2, .j2 = -1, .df = 1, .x1 = 1000, .y2 = 1510, .x3 = 1000, .x1_max = 3500, .y2_max = 3210, .z1 = 0, .z2 = 0},
        {.j1 = 2, .j2 = -1, .df = 1, .x1 = 1000, .y2 = 2010, .x3 = 500, .x1_max = 3500, .y2_max = 3210, .z1 = 0, .z2 = 0},
    };
    EXPECT_EQ(sol.all_valid_insertions(info), is);
}

TEST(Solution, InsertionY2)
{
    /**
     *
     * |-------------------------------------------------| 3210
     * |   |-------|                                     | 3200
     * |   |       |                                     |
     * |---|       |                                     | 3000
     * |   |       |                                     |
     * |   |   1   |                                     |
     * | 0 |       |                                     |
     * |   |       |                                     |
     * |   |       |                                     |
     * |---|-------|-------------------------------------|
     *   1000    2000
     *
     */

    Info info;
    GlobalParam p = GlobalParam::roadef2018();
    std::vector<Defect> vd;

    std::vector<Item> vi {
        {.id = 0, .rect = {1000, 3000}, .stack = 0, .sequence = 1},
        {.id = 1, .rect = {1000, 3200}, .stack = 1, .sequence = 1},
    };
    Instance ins(vi, vd, p);
    Solution sol(ins);

    Insertion i0 = {.j1 = 0, .j2 = -1, .df = -1, .x1 = 1000, .y2 = 3000, .x3 = 1000, .x1_max = 3500, .y2_max = 3210, .z1 = 0, .z2 = 0};
    std::vector<Insertion> is0 = sol.all_valid_insertions(info);
    EXPECT_NE(std::find(is0.begin(), is0.end(), i0), is0.end());
    sol.add_item(i0, info);

    std::vector<Insertion> is {
        {.j1 = 1, .j2 = -1, .df = 0, .x1 = 4200, .y2 = 1000, .x3 = 4200, .x1_max = 4500, .y2_max = 3210, .z1 = 0, .z2 = 0},
    };
    EXPECT_EQ(sol.all_valid_insertions(info), is);
}

TEST(Solution, Insertion4CutDfm1)
{
    /**
     *
     * |-------------------------------------------------|
     * |                                                 |
     * |                                                 |
     * |                                                 |
     * |                                                 |
     * |                                                 |
     * |                                                 |
     * |                                                 |
     * |                                                 |
     * |--------|                                        | 600
     * |        |                                        |
     * |   1    |--------|                               | 500
     * |--------|        |                               | 400
     * |        |        |                               |
     * |   0    |   2    |                               |
     * |        |        |                               |
     * |--------|--------|-------------------------------|
     *        1000      2001
     *
     */

    Info info;
    GlobalParam p = GlobalParam::roadef2018();
    std::vector<Defect> vd;

    std::vector<Item> vi {
        {.id = 0, .rect = {400, 1000}, .stack = 0, .sequence = 0},
        {.id = 1, .rect = {200, 1000}, .stack = 1, .sequence = 0},
        {.id = 2, .rect = {500, 1001}, .stack = 0, .sequence = 1},
        {.id = 3, .rect = {700, 1002}, .stack = 0, .sequence = 2},
        {.id = 4, .rect = {590, 1003}, .stack = 1, .sequence = 1},
    };
    Instance ins(vi, vd, p);
    Solution sol(ins);

    Insertion i0 = {.j1 = 0, .j2 = 1, .df = -1, .x1 = 1000, .y2 = 600, .x3 = 1000, .x1_max = 3500, .y2_max = 3210, .z1 = 0, .z2 = 2};
    std::vector<Insertion> is0 = sol.all_valid_insertions(info);
    EXPECT_NE(std::find(is0.begin(), is0.end(), i0), is0.end());
    sol.add_item(i0, info);

    std::vector<Insertion> is2 {
        {.j1 = 2, .j2 = -1, .df = 2, .x1 = 2001, .y2 = 600, .x3 = 2001, .x1_max = 3500, .y2_max = 3210, .z1 = 0, .z2 = 2},
        //{.j1 = 2, .j2 = -1, .df = 1, .x1 = 1021, .y2 = 1100, .x3 = 1001, .x1_max = 3500, .y2_max = 3210, .z1 = 1, .z2 = 0},
        //{.j1 = 2, .j2 = -1, .df = 1, .x1 = 1000, .y2 = 1601, .x3 = 500, .x1_max = 3500, .y2_max = 3210, .z1 = 0, .z2 = 0},
        //{.j1 = 4, .j2 = -1, .df = 1, .x1 = 1023, .y2 = 1190, .x3 = 1003, .x1_max = 3500, .y2_max = 3210, .z1 = 1, .z2 = 0},
        //{.j1 = 4, .j2 = -1, .df = 1, .x1 = 1000, .y2 = 1603, .x3 = 590, .x1_max = 3500, .y2_max = 3210, .z1 = 0, .z2 = 0},
    };
    EXPECT_EQ(sol.all_valid_insertions(info), is2);
}

TEST(Solution, Insertion4CutDf0)
{
    /**
     *
     * |-------|-------------------------------------------------|
     * |       |                                                 |
     * |       |                                                 |
     * |       |                                                 |
     * |       |                                                 |
     * |       |                                                 |
     * |       |                                                 |
     * |       |                                                 |
     * |   0   |                                                 |
     * |       |--------|                                        | 600
     * |       |        |                                        |
     * |       |   2    |--------|                               | 500
     * |       |--------|        |                               | 400
     * |       |        |        |                               |
     * |       |   1    |    3   |                               |
     * |       |        |        |                               |
     * |-------|--------|--------|-------------------------------|
     *       1000     2000     3001
     *
     */

    Info info;
    GlobalParam p = GlobalParam::roadef2018();
    std::vector<Defect> vd;

    std::vector<Item> vi {
        {.id = 0, .rect = {1000, 3210}, .stack = 2, .sequence = 0},
        {.id = 1, .rect = {400, 1000}, .stack = 0, .sequence = 0},
        {.id = 2, .rect = {200, 1000}, .stack = 1, .sequence = 0},
        {.id = 3, .rect = {500, 1001}, .stack = 0, .sequence = 1},
        {.id = 4, .rect = {700, 1002}, .stack = 0, .sequence = 2},
        {.id = 5, .rect = {590, 1003}, .stack = 1, .sequence = 1},
    };
    Instance ins(vi, vd, p);
    Solution sol(ins);

    Insertion i0 = {.j1 = 0, .j2 = -1, .df = -1, .x1 = 1000, .y2 = 3210, .x3 = 1000, .x1_max = 3500, .y2_max = 3210, .z1 = 0, .z2 = 0};
    std::vector<Insertion> is0 = sol.all_valid_insertions(info);
    EXPECT_NE(std::find(is0.begin(), is0.end(), i0), is0.end());
    sol.add_item(i0, info);

    Insertion i1 = {.j1 = 1, .j2 = 2, .df = 0, .x1 = 2000, .y2 = 600, .x3 = 2000, .x1_max = 4500, .y2_max = 3210, .z1 = 0, .z2 = 2};
    std::vector<Insertion> is1 = sol.all_valid_insertions(info);
    EXPECT_NE(std::find(is1.begin(), is1.end(), i1), is1.end());
    sol.add_item(i1, info);

    std::vector<Insertion> is2 {
        {.j1 = 3, .j2 = -1, .df = 2, .x1 = 3001, .y2 = 600, .x3 = 3001, .x1_max = 4500, .y2_max = 3210, .z1 = 0, .z2 = 2},
        //{.j1 = 3, .j2 = -1, .df = 1, .x1 = 2021, .y2 = 1100, .x3 = 2001, .x1_max = 4500, .y2_max = 3210, .z1 = 1, .z2 = 0},
        //{.j1 = 3, .j2 = -1, .df = 1, .x1 = 2000, .y2 = 1601, .x3 = 1500, .x1_max = 4500, .y2_max = 3210, .z1 = 0, .z2 = 0},
        //{.j1 = 5, .j2 = -1, .df = 1, .x1 = 2023, .y2 = 1190, .x3 = 2003, .x1_max = 4500, .y2_max = 3210, .z1 = 1, .z2 = 0},
        //{.j1 = 5, .j2 = -1, .df = 1, .x1 = 2000, .y2 = 1603, .x3 = 1590, .x1_max = 4500, .y2_max = 3210, .z1 = 0, .z2 = 0},
    };
    EXPECT_EQ(sol.all_valid_insertions(info), is2);
}

TEST(Solution, Insertion4CutDf1)
{
    /**
     *
     * |-------|-------------------------------------------------|
     * |       |                                                 |
     * |       |                                                 |
     * |       |                                                 |
     * |       |                                                 |
     * |       |                                                 |
     * |       |--------|                                        | 1600
     * |       |   5    |--------|                               | 1500
     * |       |--------|        |                               | 1400
     * |   0   |        |        |                               |
     * |       |   2    |    3   |                               |
     * |       |        |        |                               |
     * |       |--------|--------|                               |
     * |       |                 |                               |
     * |       |        1        |                               |
     * |       |                 |                               |
     * |-------|-----------------|-------------------------------|
     *       1000     2000     3000
     */

    Info info;
    GlobalParam p = GlobalParam::roadef2018();
    std::vector<Defect> vd;

    std::vector<Item> vi {
        {.id = 0, .rect = {3210, 1000}, .stack = 0, .sequence = 1},
        {.id = 1, .rect = {1000, 2000}, .stack = 1, .sequence = 1},
        {.id = 2, .rect = {400, 1000}, .stack = 2, .sequence = 1},
        {.id = 3, .rect = {500, 1001}, .stack = 2, .sequence = 2},
        {.id = 4, .rect = {700, 1002}, .stack = 2, .sequence = 3},
        {.id = 5, .rect = {200, 1000}, .stack = 3, .sequence = 1},
        {.id = 6, .rect = {590, 1003}, .stack = 3, .sequence = 2},
    };
    Instance ins(vi, vd, p);
    Solution sol(ins);

    Insertion i0 = {.j1 = 0, .j2 = -1, .df = -1, .x1 = 1000, .y2 = 3210, .x3 = 1000, .x1_max = 3500, .y2_max = 3210, .z1 = 0, .z2 = 0};
    std::vector<Insertion> is0 = sol.all_valid_insertions(info);
    EXPECT_NE(std::find(is0.begin(), is0.end(), i0), is0.end());
    sol.add_item(i0, info);

    Insertion i1 = {.j1 = 1, .j2 = -1, .df = 0, .x1 = 3000, .y2 = 1000, .x3 = 3000, .x1_max = 4500, .y2_max = 3210, .z1 = 0, .z2 = 0};
    std::vector<Insertion> is1 = sol.all_valid_insertions(info);
    EXPECT_NE(std::find(is1.begin(), is1.end(), i1), is1.end());
    sol.add_item(i1, info);

    Insertion i2 = {.j1 = 2, .j2 = 5, .df = 1, .x1 = 3000, .y2 = 1600, .x3 = 2000, .x1_max = 4500, .y2_max = 3210, .z1 = 0, .z2 = 2};
    std::vector<Insertion> is2 = sol.all_valid_insertions(info);
    EXPECT_NE(std::find(is2.begin(), is2.end(), i2), is2.end());
    sol.add_item(i2, info);

    std::vector<Insertion> is {
        {.j1 = 3, .j2 = -1, .df = 2, .x1 = 3021, .y2 = 1600, .x3 = 3001, .x1_max = 4500, .y2_max = 3210, .z1 = 1, .z2 = 2},
        //{.j1 = 3, .j2 = -1, .df = 1, .x1 = 3000, .y2 = 2100, .x3 = 2001, .x1_max = 4500, .y2_max = 3210, .z1 = 0, .z2 = 0},
        //{.j1 = 3, .j2 = -1, .df = 1, .x1 = 3000, .y2 = 2601, .x3 = 1500, .x1_max = 4500, .y2_max = 3210, .z1 = 0, .z2 = 0},
        //{.j1 = 6, .j2 = -1, .df = 1, .x1 = 3000, .y2 = 2190, .x3 = 2003, .x1_max = 4500, .y2_max = 3210, .z1 = 0, .z2 = 0},
        //{.j1 = 6, .j2 = -1, .df = 1, .x1 = 3000, .y2 = 2603, .x3 = 1590, .x1_max = 4500, .y2_max = 3210, .z1 = 0, .z2 = 0},
    };
    EXPECT_EQ(sol.all_valid_insertions(info), is);
}

TEST(Solution, Insertion4CutDf2)
{
    /**
     *
     * |-------|--------------------------|
     * |       |                          |
     * |       |                          |
     * |       |        |--------|        | 2600
     * |       |--------|   4    |--------| 2500
     * |       |        |--------|        | 2400
     * |       |        |        |        |
     * |       |    2   |   3    |        |
     * |       |        |        |        |
     * |   0   |--------|--------|--------| 2000
     * |       |                          |
     * |       |                          |
     * |       |                          |
     * |       |            1             |
     * |       |                          |
     * |       |                          |
     * |       |                          |
     * |-------|--------------------------|
     *       3000     4000     5000      6000
     *
     */

    Info info;
    GlobalParam p = GlobalParam::roadef2018();
    std::vector<Defect> vd;

    std::vector<Item> vi {
        {.id = 0, .rect = {3000, 3210}, .stack = 2, .sequence = 1},
        {.id = 1, .rect = {2000, 3000}, .stack = 3, .sequence = 1},
        {.id = 2, .rect = {500, 1000}, .stack = 4, .sequence = 1},
        {.id = 3, .rect = {400, 1000}, .stack = 0, .sequence = 1},
        {.id = 4, .rect = {200, 1000}, .stack = 1, .sequence = 1},
        {.id = 5, .rect = {500, 1000}, .stack = 0, .sequence = 2},
        {.id = 6, .rect = {700, 1000}, .stack = 0, .sequence = 3},
        {.id = 7, .rect = {590, 1000}, .stack = 1, .sequence = 2},
    };
    Instance ins(vi, vd, p);
    Solution sol(ins);

    Insertion i0 = {.j1 = 0, .j2 = -1, .df = -1, .x1 = 3000, .y2 = 3210, .x3 = 3000, .x1_max = 3500, .y2_max = 3210, .z1 = 0, .z2 = 0};
    std::vector<Insertion> is0 = sol.all_valid_insertions(info);
    EXPECT_NE(std::find(is0.begin(), is0.end(), i0), is0.end());
    sol.add_item(i0, info);

    Insertion i1 = {.j1 = 1, .j2 = -1, .df = 0, .x1 = 6000, .y2 = 2000, .x3 = 6000, .x1_max = 6000, .y2_max = 3210, .z1 = 0, .z2 = 0};
    std::vector<Insertion> is1 = sol.all_valid_insertions(info);
    EXPECT_NE(std::find(is1.begin(), is1.end(), i1), is1.end());
    sol.add_item(i1, info);

    Insertion i2 = {.j1 = 2, .j2 = -1, .df = 1, .x1 = 6000, .y2 = 2500, .x3 = 4000, .x1_max = 6000, .y2_max = 3210, .z1 = 0, .z2 = 0};
    std::vector<Insertion> is2 = sol.all_valid_insertions(info);
    EXPECT_NE(std::find(is2.begin(), is2.end(), i2), is2.end());
    sol.add_item(i2, info);

    Insertion i3 = {.j1 = 3, .j2 = 4, .df = 2, .x1 = 6000, .y2 = 2600, .x3 = 5000, .x1_max = 6000, .y2_max = 3210, .z1 = 0, .z2 = 2};
    std::vector<Insertion> is3 = sol.all_valid_insertions(info);
    EXPECT_NE(std::find(is3.begin(), is3.end(), i3), is3.end());
    sol.add_item(i3, info);

    std::vector<Insertion> is {
        {.j1 = 5, .j2 = -1, .df = 2, .x1 = 6000, .y2 = 2600, .x3 = 6000, .x1_max = 6000, .y2_max = 3210, .z1 = 0, .z2 = 2},
    };
    EXPECT_EQ(sol.all_valid_insertions(info), is);
}

