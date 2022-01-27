load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
load(
    "@bazel_tools//tools/build_defs/repo:git.bzl",
    "git_repository",
    "new_git_repository",
)

gtest_version = "release-1.11.0"

# Fetch gtest
# see https://google.github.io/googletest/quickstart-bazel.html
http_archive(
    name = "com_google_googletest",
    strip_prefix = "googletest-" + gtest_version,
    urls = [
        "https://github.com/google/googletest/archive/" + gtest_version + ".zip",
    ],
)

# Bazel needs a dependency on rules_cc repo (enrlov: isn't this built in?)
# see https://google.github.io/googletest/quickstart-bazel.html
http_archive(
    name = "rules_cc",
    strip_prefix = "rules_cc-40548a2974f1aea06215272d9c2b47a14a24e556",
    urls = ["https://github.com/bazelbuild/rules_cc/archive/40548a2974f1aea06215272d9c2b47a14a24e556.zip"],
)

git_repository(
    name = "bazel_clang_tidy",
    commit = "9871a95dbb150dc595aa91355fe99c500196cf3c",
    remote = "https://github.com/erenon/bazel_clang_tidy",
)

http_archive(
    name = "rules_python",
    sha256 = "954aa89b491be4a083304a2cb838019c8b8c3720a7abb9c4cb81ac7a24230cea",
    url = "https://github.com/bazelbuild/rules_python/releases/download/0.4.0/rules_python-0.4.0.tar.gz",
)

load("@rules_python//python:pip.bzl", "pip_install")

pip_install(
    name = "pip",
    #python_interpreter_target = "@python_interpreter//:python_bin",
    requirements = "//doxygen:requirements.txt",
)

new_git_repository(
    name = "mcss",
    build_file = "@//:external/m.css.BUILD",
    commit = "b6559265ce825c7cedc8a7e27770c8f11a938ba9",
    remote = "https://github.com/oliverlee/m.css/",
)
