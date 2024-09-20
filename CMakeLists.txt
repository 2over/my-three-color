cmake_minimum_required(VERSION 3.16)
project(my-three-color)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_FLAGS "${CAMKE_CXX_FLAGS} -std=c++11 -pthread")

add_executable(my-three-color
        main.cpp
        include/memory/memory_cell.h
        include/memory/memory_chunk.h
        include/memory/memory_pool.h
        src/memory/memory_chunk.cpp
        src/memory/memory_pool.cpp
        include/common.h
        include/gc/ConcurrentMark.h
        include/gc/gc_factory.h
        include/gc/mark_clean.h
        include/gc/mark_compact.h
        include/gc/mark_copy.h
        include/gc/interface/garbage_collect.h
        include/gc/ConcurrentMark.h src/gc/ConcurrentMark.cpp include/core/Thread.h share/vm/oops/OopDesc.h)