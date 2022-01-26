load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

gtest_version = "release-1.11.0"

# Fetch gtest
# see https://google.github.io/googletest/quickstart-bazel.html
http_archive(
  name = "com_google_googletest",
  urls = [
      "https://github.com/google/googletest/archive/" + gtest_version + ".zip"],
  strip_prefix = "googletest-" + gtest_version,
)

# Bazel needs a dependency on rules_cc repo (enrlov: isn't this built in?)
# see https://google.github.io/googletest/quickstart-bazel.html
http_archive(
  name = "rules_cc",
  urls = ["https://github.com/bazelbuild/rules_cc/archive/40548a2974f1aea06215272d9c2b47a14a24e556.zip"],
  strip_prefix = "rules_cc-40548a2974f1aea06215272d9c2b47a14a24e556",
)
