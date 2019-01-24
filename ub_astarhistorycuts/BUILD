cc_library(
        name = "libastarhistorycuts",
        hdrs = ["astarhistorycuts.hpp"],
        srcs = ["astarhistorycuts.cpp"],
        deps = ["//lib:cpp"],
        visibility = ["//visibility:public"],
        include_prefix = "roadef2018/ub_astarhistorycuts/",
)

cc_binary(
        name = "main",
        srcs = ["main.cpp"],
        deps = [":libastarhistorycuts"],
)

