cc_library(
        name = "cpp",
        hdrs = [
                "instance.hpp",
                "solution.hpp"
        ],
        srcs = [
                "instance.cpp",
                "solution.cpp",
        ],
        deps = ["@benchtools//:info"],
        linkopts = [
                "-pthread",
                "-lboost_filesystem",
                "-lboost_program_options",
        ],
        visibility = ["//visibility:public"],
        include_prefix = "roadef2018/lib/",
)

cc_binary(
        name = "main",
        srcs = ["main.cpp"],
        deps = [
                "//ub_dfs:dfs",
                "//ub_astar:astar",
                "//ub_dpastar:dpastar",
                "//ub_beamsearch:beamsearch",
                "//ub_beamstacksearch:beamstacksearch",
                "//ub_mbastar:mbastar",
        ],
)

cc_binary(
        name = "insinfo_main",
        srcs = ["insinfo_main.cpp"],
        deps = [":cpp"],
)

cc_test(
        name = "test",
        srcs = [
                "tests/waste_test.cpp",
                "tests/add_test.cpp",
                "tests/insertion_test.cpp",
                "tests/defect_test.cpp",
                "tests/integration_test.cpp",
        ],
        deps = [
                ":cpp",
                "//ub_astar:astar",
                "@googletest//:gtest_main",
        ],
        data = ["//instances:instances"],
        timeout = "short",
)

