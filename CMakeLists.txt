cmake_minimum_required(VERSION 2.6)
project(GigEV4Linux)

include_directories(. /usr/include /usr/include/c++/4.3)
set(CMAKE_CXX_FLAGS "-W -Wall -g")
find_package(Boost REQUIRED thread)

find_library(VIGRA_LIBRARY vigraimpex)
add_executable(camtest Main.cpp Gvsp.h Gvcp.h)
target_link_libraries(camtest ${Boost_THREAD_LIBRARY} ${VIGRA_LIBRARY})
