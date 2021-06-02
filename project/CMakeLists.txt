cmake_minimum_required(VERSION 3.16)
project(igraph_test C)

find_package(igraph REQUIRED)
find_package(IGraph)


add_executable(igraph_test igraph_test.c)

#target_link_libraries(igraph_test PRIVATE OpenMP::OpenMP_CXX)
target_link_libraries(igraph_test PUBLIC igraph::igraph)
