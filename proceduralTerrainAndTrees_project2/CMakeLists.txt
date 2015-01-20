cmake_minimum_required(VERSION 2.8)

project( PTAT )
set( PTAT_SRCS src/proceduralTerrainAndTrees_project2.cpp )
include_directories(./inc/)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
find_package(OpenMesh REQUIRED)
add_executable( ptat ${PTAT_SRCS} ${PTAT_HDRS})
target_link_libraries(ptat ${OpenMesh_LIBRARY})

project( PROCEDURAL_TREES )
set( PROCEDURAL_TREES_SRCS src/3DTurtle.cpp src/Lsystem.cpp src/TurtleTest.cpp)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
add_definitions(-std=c++11)
find_package(OpenMesh REQUIRED)

include_directories(./inc/ /usr/include/eigen3/ /usr/include/eigen2/)
add_executable( procedural_trees ${PROCEDURAL_TREES_SRCS} ${PROCEDURAL_TREES_HDRS})
target_link_libraries(procedural_trees ${OpenMesh_LIBRARY})

