exports_files(
    [".clang-tidy"],
    visibility = ["//:__subpackages__"],
)

filegroup(
    name = "headers",
    srcs = [
        "src/convopt.hpp",
        "src/dualnumbers.hpp",
        "src/math.hpp",
        "src/spaces.hpp",
    ],
    visibility = ["@mcss//:__pkg__"],
)

cc_library(
    name = "optimizer",
    hdrs = [":headers"],
    visibility = ["//visibility:public"],
)
