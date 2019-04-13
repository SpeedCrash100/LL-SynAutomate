cmake_minimum_required(VERSION 3.0)

project(LL-SynAutomate)

set(LL_SRCS
    automate.cpp
    main.cpp
)

set(LL_HDRS
    automate.hpp
)

add_executable(LL-SynAutomate ${LL_HDRS} ${LL_SRCS})
