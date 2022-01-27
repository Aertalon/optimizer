exports_files(
    [".clang-tidy"],
    visibility = ["//:__subpackages__"],
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
