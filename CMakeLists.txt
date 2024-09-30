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
        include/gc/ConcurrentMark.h src/gc/ConcurrentMark.cpp include/core/Thread.h share/vm/oops/OopDesc.h share/vm/compiler/OopMapSet.cpp share/vm/compiler/OopMap.h share/vm/utilities/BasicType.h share/vm/memory/ResourceObj.h share/vm/compiler/OopMapSet.h include/core/ThreeColorMap.h include/core/ThreeColorMap.cpp include/container/Array.h include/container/Container.h share/vm/oops/Klass.cpp share/vm/oops/Klass.h include/core/Threads.h example/main-stw.cpp include/core/Thread.cpp include/core/Monitor.cpp include/core/Monitor.h include/core/ParkEvent.cpp include/core/ParkEvent.h include/core/SafepointSynchronize.cpp include/core/SafepointSynchronize.h)