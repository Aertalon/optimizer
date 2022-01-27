exports_files(
    [".clang-tidy"],
    visibility = ["//:__subpackages__"],
)

cc_binary(
    name = "optimizer",
    srcs = ["src/main.cpp"],
    deps = [
        "//src/convopt",
        "//src/dualnumbers",
    ],
)
