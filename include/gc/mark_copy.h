//
// Created by xiehao on 2024/9/13.
//
#pragma once

#include "interface/garbage_collect.h"
#include "../memory/memory_chunk.h"

class MarkCopy: public GarbageCollect {

private:
    MemoryChunk *m_mem_chunk;

public:
    MarkCopy(MemoryChunk *chunk);
    ~MarkCopy();

public:
    MemoryChunk *get_mem_chunk();

    MarkCopy *set_mem_chunk(MemoryChunk *chunk);

private:
    void mark_step();
    void stop_the_world();
    void memory_sweep_step();

public:
    void run() override ;
};
