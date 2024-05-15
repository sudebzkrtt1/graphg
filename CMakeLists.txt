cmake_minimum_required(VERSION 3.27)
project(untitled3)

set(CMAKE_CXX_STANDARD 17)

add_executable(untitled3 main.cpp
        EdgeList.cpp
        EdgeList.h
        Edge.cpp
        Edge.h
        Graph.cpp
        Graph.h)
