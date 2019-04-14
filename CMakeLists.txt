cmake_minimum_required(VERSION 3.0)

project(LL-SynAutomate)

set(LL_LIB_SRCS
    lib/automate.cpp
	lib/symbol.cpp
)

set(LL_LIB_HDRS
    lib/automate.hpp
	lib/symbol.hpp
)

add_library(LL-SynAutomate-lib ${LL_LIB_HDRS} ${LL_LIB_SRCS})

include_directories(lib)

set(LL_TEST_HDRS
    test/testautomate.hpp
)

set(LL_TEST_SRCS
	test/main.cpp
        test/testautomate.cpp
)

add_executable(LL-SynAutomate-test ${LL_TEST_HDRS} ${LL_TEST_SRCS})
target_link_libraries(LL-SynAutomate-test PRIVATE LL-SynAutomate-lib)
