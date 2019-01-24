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
        deps = [
                "@benchtools//:info",
        ],
        linkopts = [
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
                "//ub_astar:libastar",
                "//ub_astarhistorycuts:libastarhistorycuts",
                "//ub_dfs:libdfs",
                "//ub_dynbab:libdynbab",
                "//ub_beamsearch:libbeamsearch",
                "//ub_beamstacksearch:libbeamstacksearch",
        ],
        copts = [
                "-fopenmp",
        ],
        linkopts = [
                "-fopenmp",
                "-static",
        ],
)

cc_binary(
        name = "test_instance",
        srcs = ["test_instance.cpp"],
        deps = [
        ],
)

cc_binary(
        name = "test_io_instance",
        srcs = ["test_io_instance.cpp"],
        deps = [
        ],
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
                "//ub_astar:libastar",
                "@googletest//:gtest_main",
        ],
        data = ["//instances:instances"],
        timeout = "short",
)

cc_binary(
        name = "test_main",
        srcs = ["test_main.cpp"],
        deps = [
                "cpp",
                "//ub_astar:libastar",
        ],
)

