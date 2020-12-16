package(default_visibility = ["//test:__subpackages__"])
cc_binary(
    name = "FastRFS",
    srcs = ["FastRFS.cpp"],
    deps = [":FastRFTripartitionScorer", 
            ":RFSupportAnalysis",        
            "@phylonaut",
            "@phylokit",
    	    "@com_github_google_glog//:glog"
        ]
)

cc_library(
    name = "FastRFTripartitionScorer",
    hdrs = ["FastRFTripartitionScorer.hpp"],
    srcs = ["FastRFTripartitionScorer.cpp"],
    deps = ["@phylonaut", "@phylokit", "@com_github_google_glog//:glog"]
)

cc_library(
    name = "RFSupportAnalysis",
    hdrs = ["RFSupportAnalysis.hpp"],
    srcs = ["RFSupportAnalysis.cpp"],
    deps = ["@phylonaut", "@phylokit"]
)