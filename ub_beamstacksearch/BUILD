cc_library(
        name = "libbeamstacksearch",
        hdrs = ["beamstacksearch.hpp"],
        srcs = ["beamstacksearch.cpp"],
        deps = ["//lib:cpp"],
        copts = ["-fopenmp"],
        linkopts = ["-fopenmp"],
        visibility = ["//visibility:public"],
        include_prefix = "roadef2018/ub_beamstacksearch/",
)

cc_binary(
        name = "main",
        srcs = ["main.cpp"],
        deps = [":libbeamstacksearch"],
        linkopts = ["-fopenmp"],
)

