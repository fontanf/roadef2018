#include "roadef2018/lib/instance.hpp"
#include "roadef2018/lib/solution.hpp"
#include "roadef2018/ub_astar/astar.hpp"

#include <gtest/gtest.h>

using namespace roadef2018;

TEST(Solution, C1)
{
    Info info;
    Instance ins = Instance("instances/C1_batch.csv", "instances/C1_defects.csv", "instances/global_param.csv");
    Solution sol = sol_astar(ins, info);
    EXPECT_EQ(sol.waste(), 0);
    EXPECT_EQ(sol.width(), 6000);
}

TEST(Solution, C2)
{
    Info info;
    Instance ins = Instance("instances/C2_batch.csv", "instances/C2_defects.csv", "instances/global_param.csv");
    Solution sol = sol_astar(ins, info);
    EXPECT_EQ(sol.waste(), 210*5700);
    EXPECT_EQ(sol.width(), 5700);
}

TEST(Solution, C3)
{
    Info info;
    Instance ins = Instance("instances/C3_batch.csv", "instances/C3_defects.csv", "instances/global_param.csv");
    Solution sol = sol_astar(ins, info);
    EXPECT_EQ(sol.waste(), 0);
    EXPECT_EQ(sol.width(), 6000);
}

