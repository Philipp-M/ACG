cmake_minimum_required(VERSION 2.8)

project( PTAT )
set( PTAT_SRCS src/proceduralTerrainAndTrees_project2.cpp src/DrainageNetworkTree.cpp)
include_directories(./inc/)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
add_definitions(-std=c++11)
find_package(OpenMesh REQUIRED)
add_executable( ptat ${PTAT_SRCS} ${PTAT_HDRS})
target_link_libraries(ptat ${OpenMesh_LIBRARY})

