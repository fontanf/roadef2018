cc_library(
        name = "libdynbab",
        hdrs = ["dynbab.hpp"],
        srcs = ["dynbab.cpp"],
        deps = ["//lib:cpp"],
        copts = ["-fopenmp"],
        linkopts = ["-fopenmp"],
        visibility = ["//visibility:public"],
        include_prefix = "roadef2018/ub_dynbab/",
)

cc_binary(
        name = "main",
        srcs = ["main.cpp"],
        deps = [":libdynbab"],
)

