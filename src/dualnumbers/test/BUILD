load("@rules_cc//cc:defs.bzl", "cc_test")

cc_test(
    name = "test",
    size = "small",
    srcs = ["dualnumbers_test.cpp"],
    deps = [
        "//src/dualnumbers",
        "@com_google_googletest//:gtest_main",
    ],
)
