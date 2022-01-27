load(
    "@local_config//:defs.bzl",
    "PROJECT_DEFAULT_COPTS",
)

filegroup(
    name = "headers",
    srcs = [
        "src/concepts.hpp",
        "src/convopt.hpp",
        "src/dualnumbers.hpp",
        "src/math.hpp",
        "src/spaces.hpp",
        "src/stdx/traits.hpp",
    ],
    visibility = ["@mcss//:__pkg__"],
)

cc_library(
    name = "optimizer",
    hdrs = [":headers"],
    visibility = ["//visibility:public"],
)
