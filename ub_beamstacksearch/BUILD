cc_library(
        name = "beamstacksearch",
        hdrs = ["beamstacksearch.hpp"],
        srcs = ["beamstacksearch.cpp"],
        deps = ["//lib:cpp"],
        visibility = ["//visibility:public"],
        include_prefix = "roadef2018/ub_beamstacksearch/",
)

cc_binary(
        name = "main",
        srcs = ["main.cpp"],
        deps = [":beamstacksearch"],
)

