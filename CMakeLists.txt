cmake_minimum_required(VERSION 2.6)
project(GigEV4Linux)

set(SOURCES src/Main.cpp
            src/Gvsp.cpp
            src/Gvcp.cpp
            src/GigEVision.cpp
            src/GenICam.cpp)

set(HEADERS src/Gvsp.h
            src/Gvcp.h
            src/GenICam.h
            src/GigEVision.h)


include_directories(. /usr/include /usr/include/c++/4.3 thirdparty/GenICam/include)
add_definitions(-DGENICAM_ROOT_PATH=\"${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/GenICam/\")

set(CMAKE_CXX_FLAGS "-W -Wall -Wno-unknown-pragmas -Wno-attributes -g")
find_package(Boost REQUIRED thread filesystem)

find_library(ZIP_LIBRARY zip REQUIRED)
find_library(PNG_LIBRARY png REQUIRED)
find_library(GENICAM_BASE_LIBRARY genicam_base REQUIRED thirdparty/GenICam/lib64)
find_library(GENICAM_API_LIBRARY genicam_api REQUIRED thirdparty/GenICam/lib64)
#find_library(GENICAM_MATHPARSER_LIBRARY genicam_base REQUIRED thirdparty/GenICam/lib64)
add_executable(camtest ${SOURCES} ${HEADERS})

target_link_libraries(camtest ${Boost_THREAD_LIBRARY} ${Boost_FILESYSTEM_LIBRARY} ${ZIP_LIBRARY}
                              ${PNG_LIBRARY} ${GENICAM_BASE_LIBRARY} ${GENICAM_API_LIBRARY})
