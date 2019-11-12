cc_test(
    name="hash",
    srcs=["unit/hash.cpp"],
    copts=["-std=c++1y", "-Iexternal/googletest/googletest/include"],
    deps=[
        "//:narg",
        "@googletest//:gtest",
    ],
    timeout="short",
)


cc_test(
    name="narg",
    srcs=["unit/narg.cpp"],
    copts=["-std=c++1y", "-Iexternal/googletest/googletest/include"],
    deps=[
        "//:narg",
        "@googletest//:gtest",
    ],
    timeout="short",
)
