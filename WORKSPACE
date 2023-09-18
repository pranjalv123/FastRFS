load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

git_repository(
    name = "com_github_bazelbuild_platforms",
    commit = "4ad40ef271da8176d4fc0194d2089b8a76e19d7b",
    remote = "https://github.com/bazelbuild/platforms"
)

git_repository(
    name = "com_github_nelhage_rules_boost",
    commit = "4ab574f9a84b42b1809978114a4664184716f4bf",
    remote = "https://github.com/nelhage/rules_boost",
    shallow_since = "1570056263 -0700",
)

load("@com_github_nelhage_rules_boost//:boost/boost.bzl", "boost_deps")

boost_deps()

http_archive(
    name = "com_github_gflags_gflags",
    sha256 = "34af2f15cf7367513b352bdcd2493ab14ce43692d2dcd9dfc499492966c64dcf",
    strip_prefix = "gflags-2.2.2",
    urls = ["https://github.com/gflags/gflags/archive/v2.2.2.tar.gz"],
)

http_archive(
    name = "com_github_google_glog",
    sha256 = "122fb6b712808ef43fbf80f75c52a21c9760683dae470154f02bddfc61135022",
    strip_prefix = "glog-0.6.0",
    urls = ["https://github.com/google/glog/archive/v0.6.0.zip"],
)

git_repository(
    name = "catch2",
    commit = "c5538476052dfe9d3ff2325198b1a8f42fc10669",
    remote = "https://github.com/evanmoran/catch2-bazel/",
    shallow_since = "1530732979 -0700",
)

git_repository(
    name = "phylokit",
    commit = "aac94d561d24cb747a1e8381ffa2da0a0a0d72b6",
    remote = "https://github.com/pranjalv123/phylokit/",
)

git_repository(
    name = "phylonaut",
    commit = "f4d3fd75911651a5e62e2f286368add4f047d79c",
    remote = "https://github.com/pranjalv123/phylonaut/",
)