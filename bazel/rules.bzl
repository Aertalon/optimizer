load("@rules_cc//cc:defs.bzl", "cc_library", "cc_test")
load(
    "@local_config//:defs.bzl",
    "PROJECT_DEFAULT_COPTS",
)

def opt_cc_library(**kwargs):
    cc_library(
        copts = PROJECT_DEFAULT_COPTS,
        **kwargs
    )

def opt_cc_test(name, **kwargs):
    full_deps = [
        "//:optimizer",
        "@ut",
    ]

    if "deps" in kwargs:
        full_deps = full_deps + kwargs.deps

    if "srcs" in kwargs:
        test_srcs = kwargs.srcs
    else:
        test_srcs = [name + "_test.cpp"]

    cc_test(
        name = name,
        srcs = test_srcs,
        copts = PROJECT_DEFAULT_COPTS,
        deps = full_deps,
        **kwargs
    )
