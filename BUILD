load("//bazel:rules.bzl", "opt_cc_library")

exports_files(
    [".clang-tidy"],
    visibility = ["//:__subpackages__"],
)

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
        "src/impl/base_fn.hpp",
        "src/impl/series.hpp",
        "src/math.hpp",
        "src/spaces.hpp",
        "src/stdx/traits.hpp",
    ],
    visibility = ["@mcss//:__pkg__"],
)

opt_cc_library(
    name = "optimizer",
    hdrs = [":headers"],
    visibility = ["//visibility:public"],
    deps = [
        "@gcem",
    ],
)
