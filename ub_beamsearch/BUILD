cc_library(
        name = "beamsearch",
        hdrs = ["beamsearch.hpp"],
        srcs = ["beamsearch.cpp"],
        deps = ["//lib:cpp"],
        visibility = ["//visibility:public"],
        include_prefix = "roadef2018/ub_beamsearch/",
)

cc_binary(
        name = "main",
        srcs = ["main.cpp"],
        deps = [":beamsearch"],
)

