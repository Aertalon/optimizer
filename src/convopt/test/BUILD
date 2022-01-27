load("@rules_cc//cc:defs.bzl", "cc_test")

cc_test(
    name = "test",
    size = "small",
    srcs = ["convopt_test.cpp"],
    deps = [
        "//src/convopt",
        "//src/dualnumbers",
        "//src/spaces",
        "@com_google_googletest//:gtest_main",
    ],
)
