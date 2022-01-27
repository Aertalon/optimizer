load(
    "@local_config//:defs.bzl",
    "PROJECT_DEFAULT_COPTS",
)

# FIXME
filegroup(
    name = "headers",
    srcs = glob(["src/**/*.hpp"]),
    visibility = ["@mcss//:__pkg__"],
)

cc_binary(
    name = "optimizer",
    srcs = ["src/main.cpp"],
    deps = [
        "//src/convopt",
        "//src/dualnumbers",
    ],
)
