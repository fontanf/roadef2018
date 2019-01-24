cc_library(
        name = "libbeamsearch",
        hdrs = ["beamsearch.hpp"],
        srcs = ["beamsearch.cpp"],
        deps = ["//lib:cpp"],
        copts = ["-fopenmp"],
        linkopts = ["-fopenmp"],
        visibility = ["//visibility:public"],
        include_prefix = "roadef2018/ub_beamsearch/",
)

cc_binary(
        name = "main",
        srcs = ["main.cpp"],
        deps = [":libbeamsearch"],
        linkopts = ["-fopenmp"],
)

