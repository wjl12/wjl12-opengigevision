cmake_minimum_required(VERSION 2.6)
project(GigEV4Linux)

include_directories(. /usr/include /usr/include/c++/4.3)
set(CMAKE_CXX_FLAGS "-W -Wall -g")
find_package(Boost REQUIRED thread filesystem)

find_library(VIGRA_LIBRARY vigraimpex REQUIRED)
find_library(ZIP_LIBRARY zip REQUIRED)
add_executable(camtest Main.cpp Gvsp.h Gvcp.h GenICam.h)

target_link_libraries(camtest ${Boost_THREAD_LIBRARY} ${Boost_FILESYSTEM_LIBRARY} ${ZIP_LIBRARY} ${VIGRA_LIBRARY})
