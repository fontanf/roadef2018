#include "roadef2018/lib/instance.hpp"
#include "roadef2018/lib/solution.hpp"

#include <gtest/gtest.h>
#include <vector>

using namespace roadef2018;

#ifdef TOTO

TEST(Solution, Waste_1)
{
    /*
       Waste at the end of the first 1cut = 5x5 = 25
       +-------------------------------------------+
       |                                           |
       |  waste here                               |
       |  ^                                        |
       +--+----+                                   |
       |       |                                   |
       |       |                                   |
       |  5x5  |                                   |
       |       |                                   |
       |       +----+                              |
       |       |    |                              |
       |       |3x3 |                              |
       +------------+------------------------------+
    */

    Logger logger;
    Info info(logger);
    GlobalParam p {
        .nbplates  = 100,
        .platesize = {.w = 50, .h = 10},
        .min1cut   = 1,
        .max1cut   = 50,
        .min2cut   = 1,
        .minwaste  = 5,
    };
    std::vector<Defect> vd;

    std::vector<Item> vi {
        {.id = 0, .rect = {5, 5}, .stack = 0, .sequence = 0},
        {.id = 1, .rect = {3, 3}, .stack = 0, .sequence = 1},
    };
    Instance ins(vi, vd, p);
    Solution sol(ins);

    sol.add_item({.j = 0, .xj = 0, .yj = 0, .oj = InLength, .f = 0, .df = -1, .dc = 1, .xk = 5, .yk = 5, .x1 = 0, .y2 = 0, .j2 = -1}, info);
    sol.add_item({.j = 1, .xj = 5, .yj = 0, .oj = InLength, .f = 1, .df = 0, .dc = 1, .xk = 8, .yk = 3, .x1 = 0, .y2 = 0, .j2 = -1}, info);


    EXPECT_EQ(sol.waste(), 46);
}

TEST(Solution, Waste_2a)
{
    /*
       waste at the end of the second 2cut, waste = 2x3 = 6
       +------------------------------------------------+
       |                                                |
       |                                                |
       +--------+                                       |
       |     5x1|                                       |
       |        |                                       |
       +-----+--+                                       |
       |     |                                          |
       |  3x3| < waste here                             |
       |     |                                          |
       +-----+--+                                       |
       |        |                                       |
       |        |                                       |
       |    5x5 |                                       |
       |        |                                       |
       +--------+---------------------------------------+

    */

    Logger logger;
    Info info(logger);
    GlobalParam p = GlobalParam::roadef2018();
    std::vector<Defect> vd;

    std::vector<Item> vi {
        {.id = 0, .rect = {500, 500}, .stack = 0, .sequence = 0},
        {.id = 1, .rect = {300, 300}, .stack = 0, .sequence = 1},
        {.id = 2, .rect = {100, 500}, .stack = 0, .sequence = 2},
        {.id = 3, .rect = {100, 500}, .stack = 0, .sequence = 3},
    };
    Instance ins(vi, vd, p);
    Solution sol(ins);

    sol.add_item({.j = 0, .xj = 0, .yj = 0, .oj = InLength, .f = 0, .df = -1, .dc = 2, .xk = 500, .yk = 500, .x1 = 0, .y2 = 0, .j2 = -1}, info);
    sol.add_item({.j = 1, .xj = 0, .yj = 500, .oj = InLength, .f = 2, .df = 1, .dc = 2, .xk = 300, .yk = 800, .x1 = 0, .y2 = 0, .j2 = -1}, info);
    sol.add_item({.j = 2, .xj = 0, .yj = 800, .oj = InLength, .f = 2, .df = 1, .dc = 2, .xk = 500, .yk = 900, .x1 = 0, .y2 = 0, .j2 = -1}, info);

    EXPECT_EQ(sol.waste(), 300*200);
}

TEST(Solution, Waste_2b)
{
    /*
       waste at the end of the second 2 cut and at the end of the first 2cut = 2x3 + 1x5 = 11
       waste = 5*6 + 3*8 + 1*5 = 30 + 24 + 5 = 59
       +------------------------------------------------+
       |                                                |
       |                                                |
       +---------+                                      |
       |     6x1 |                                      |
       |         |                                      |
       +-----+---+                                      |
       |     |                                          |
       |  3x3| < waste here                             |
       |     |                                          |
       +-----+--+                                       |
       |        |                                       |
       |        +> waste here                           |
       |    5x5 |                                       |
       |        |                                       |
       +--------+---------------------------------------+

          0
          |
          1
          |
          2
        / | \
       3  4  5

    */

    Logger logger;
    Info info(logger);
    GlobalParam p = GlobalParam::roadef2018();
    std::vector<Defect> vd;

    std::vector<Item> vi {
        {.id = 0, .rect = {500, 500}, .stack = 0, .sequence = 0},
        {.id = 1, .rect = {300, 300}, .stack = 0, .sequence = 1},
        {.id = 2, .rect = {600, 100}, .stack = 0, .sequence = 2},
        {.id = 3, .rect = {600, 100}, .stack = 0, .sequence = 3},
    };
    Instance ins(vi, vd, p);
    Solution sol(ins);

    sol.add_item({.j = 0, .xj = 0, .yj = 0, .oj = InLength, .f = 0, .df = -1, .dc = 2, .xk = 500, .yk = 500, .x1 = 0, .y2 = 0, .j2 = -1}, info);
    sol.add_item({.j = 1, .xj = 0, .yj = 500, .oj = InLength, .f = 2, .df = 1, .dc = 2, .xk = 300, .yk = 800, .x1 = 0, .y2 = 0, .j2 = -1}, info);
    EXPECT_EQ(sol.waste(), 300*200);
    sol.add_item({.j = 2, .xj = 0, .yj = 800, .oj = InLength, .f = 2, .df = 1, .dc = 2, .xk = 600, .yk = 900, .x1 = 0, .y2 = 0, .j2 = -1}, info);
    EXPECT_EQ(sol.waste(), 500*100 + 300*300);
}

TEST(Solution, Waste_3)
{
    /*
       waste at the end of the second 3cut (waste: 2x1 = 2)
       +-----------------------------------------------------------+
       |                                                           |
       |                                                           |
       |                                                           |
       |                                                           |
       |                                                           |
       |                                                           |
       |                                                           |
       |                                                           |
       |                                                           |
       |                                                           |
       +-----------------+                                         |
       +-------1x6-------+                                         |
       |     |   ^ waste here                                      |
       |     +---+-+     |                                         |
       |     |     |     |                                         |
       | 2x5 |     |     |                                         |
       |     | 2x4 | 2x5 |                                         |
       |     |     |     |                                         |
       +-----+-----+-----+-----------------------------------------+
    */

    Logger logger;
    Info info(logger);
    GlobalParam p = GlobalParam::roadef2018();
    std::vector<Defect> vd;

    std::vector<Item> vi {
        {.id = 0, .rect = {200, 500}, .stack = 0, .sequence = 0},
        {.id = 1, .rect = {200, 400}, .stack = 0, .sequence = 1},
        {.id = 2, .rect = {200, 500}, .stack = 0, .sequence = 2},
        {.id = 3, .rect = {600, 100}, .stack = 0, .sequence = 3},
        {.id = 4, .rect = {600, 100}, .stack = 0, .sequence = 4},
    };
    Instance ins(vi, vd, p);
    Solution sol(ins);

    sol.add_item({.j = 0, .xj = 0, .yj = 0, .oj = InLength, .f = 0, .df = -1, .dc = 3, .xk = 200, .yk = 500, .x1 = 0, .y2 = 0, .j2 = -1}, info);
    sol.add_item({.j = 1, .xj = 200, .yj = 0, .oj = InLength, .f = 3, .df = 1, .dc = 3, .xk = 400, .yk = 400, .x1 = 0, .y2 = 0, .j2 = -1}, info);
    sol.add_item({.j = 2, .xj = 400, .yj = 0, .oj = InLength, .f = 3, .df = 1, .dc = 3, .xk = 600, .yk = 500, .x1 = 0, .y2 = 0, .j2 = -1}, info);
    sol.add_item({.j = 3, .xj = 0, .yj = 500, .oj = InLength, .f = 2, .df = 1, .dc = 2, .xk = 600, .yk = 600, .x1 = 0, .y2 = 0, .j2 = -1}, info);

    EXPECT_EQ(sol.waste(), 20000);
}

TEST(Solution, Waste_A1_positive)
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

    Logger logger;
    Info info(logger);
    Instance inst = Instance("data/dataset_A/A1_batch.csv", "data/dataset_A/A1_defects.csv", "data/global_param.csv");
    Solution sol(inst);

    sol.add_item({.j = 0, .xj = 0, .yj = 0, .oj = InLength, .f = 0, .df = -1, .dc = 1, .xk = 758, .yk = 1578, .x1 = 0, .y2 = 0, .j2 = -1}, info);
    sol.add_item({.j = 1, .xj = 758, .yj = 0, .oj = InLength, .f = 1, .df = 0, .dc = 3, .xk = 2308, .yk = 738, .x1 = 0, .y2 = 0, .j2 = -1}, info);
    LOG(info, sol << std::endl);

    EXPECT_EQ(sol.waste(), 758*(3210-1578));
}

TEST(Solution, IntegrationA1_1_test)
{
    /**
     * integration A1 (first item)
     *
     *
     +------+-------+----------------------------+
     |      |       |                            |
     |      |       |                            |
     |      |   4   |                            |
     |   1  |       |                            |
     |      +-------++                           |
     |      |        |                           |
     +------+        |                           |
     |      |        |                           |
     |      |    3   |                           |
     |      |        |                           |
     |  0   |        |                           |
     |      +------+-+                           |
     |      |   2  |                             |
     +-------------+-----------------------------+
     *
     *
     */

    Logger logger;
    Info info(logger);
    Instance inst = Instance("data/dataset_A/A1_batch.csv", "data/dataset_A/A1_defects.csv", "data/global_param.csv");
    EXPECT_EQ(inst.item_number(), 5);
    Solution sol = Solution(inst);
    LOG(info, sol << std::endl);
    // for (const Insertion& i: sol.all_valid_insertions(info)) {
    //     std::cerr << i << std::endl; // logger do not print in logs for some reason, so i use here std::cerr
    // }
    // ##################### ITEM 0
    bool found = false;
    LOG(info, "searching optimal position of item 0 ... ");
    for (const Insertion& i: sol.all_valid_insertions(info)) {
        if ( i.xj == 0 && i.yj == 0 && i.dc == 2 && i.xk == 758 && i.yk == 1578 ) {
            found = true;
            sol.add_item(i,info);
            break;
        }
    }
    EXPECT_EQ(found,true);
    LOG(info, "OK\n");

    // ##################### ITEM 1
    found = false;
    LOG(info, "searching optimal position of item 1 ... ");
    for (const Insertion& i: sol.all_valid_insertions(info)) {
        if ( i.xj == 0 && i.yj == 1578 && i.dc == 2 && i.xk == 738 && i.yk == 3128 ) {
            found = true;
            sol.add_item(i,info);
            break;
        }
    }
    EXPECT_EQ(found,true);
    LOG(info, "OK\n");
    LOG(info, "checking waste lower bound ... ");
    EXPECT_EQ(sol.waste(), 20*1550);

    // ##################### ITEM 2
    found = false;
    LOG(info, "searching optimal position of item 2 ... ");
    for (const Insertion& i: sol.all_valid_insertions(info)) {
        if (i.xj == 758 && i.yj == 0 && i.dc == 2 && i.xk == 758+581 && i.yk == 276) {
            found = true;
            sol.add_item(i,info);
            break;
        }
    }
    EXPECT_EQ(found, true);
    LOG(info, "OK\n");
    LOG(info, "checking waste lower bound ... ");
    EXPECT_EQ(sol.waste(), 20*1550+(3210-3128)*758);

    // ##################### ITEM 3
    found = false;
    LOG(info, "searching optimal position of item 3 ... ");
    for (const Insertion& i: sol.all_valid_insertions(info)) {
        if (i.xj == 758 && i.yj == 276 && i.dc == 2 && i.xk == 758+781 && i.yk == 276+1396 ) {
            found = true;
            sol.add_item(i,info);
            break;
        }
    }
    EXPECT_EQ(found,true);
    LOG(info, "OK\n");
    LOG(info, "checking waste lower bound ... ");
    EXPECT_EQ(sol.waste(), 20*1550+(3210-3128)*758+(781-581)*276);

    // ##################### ITEM 4
    found = false;
    LOG(info, "searching optimal position of item 4 ... ");
    for (const Insertion& i: sol.all_valid_insertions(info)) {
        if (i.xj == 758 && i.yj == 1672 && i.dc == 2 && i.xk == 758+648 && i.yk == 1672+1426) {
            found = true;
            sol.add_item(i,info);
            break;
        }
    }
    EXPECT_EQ(found,true);
    LOG(info, "OK\n");

    LOG(info, "checking metrics of solution ... ");
    EXPECT_EQ(sol.waste(), 425486);
    EXPECT_EQ(sol.lb_area(), 1539);
    LOG(info, "OK\n");

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

    Logger logger;
    Info info(logger);
    GlobalParam p = GlobalParam::roadef2018();
    std::vector<Defect> vd;

    std::vector<Item> vi {
        {.id = 0, .rect = {1500, 2000}, .stack = 0, .sequence = 0},
        {.id = 1, .rect = {1710, 1995}, .stack = 1, .sequence = 1},
    };
    Instance ins(vi, vd, p);
    Solution sol(ins);
    sol.add_item({.j = 0, .xj = 0, .yj = 0, .oj = InLength, .f = 0, .df = -1, .dc = 2, .xk = 2000, .yk = 1500, .x1 = 0, .y2 = 0, .j2 = -1}, info);
    sol.add_item({.j = 1, .xj = 0, .yj = 1500, .oj = InLength, .f = 2, .df = 1, .dc = 2, .xk = 1995, .yk = 3210, .x1 = -2020, .y2 = 0, .j2 = -1}, info);
    EXPECT_EQ(sol.waste(), 20*1500 + 25*1710);
    EXPECT_EQ(sol.width(), 2020);
}

#endif
