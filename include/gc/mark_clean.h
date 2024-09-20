//
// Created by xiehao on 2024/9/6.
//

#pragma once

#include "interface/garbage_collect.h"
#include "../memory/memory_chunk.h"

using namespace std;

class MarkClean : public GarbageCollect {
private:
    MemoryChunk *m_mem_chunk;

public:
    MarkClean(MemoryChunk *mem_chunk);

    ~MarkClean();

public:
    MemoryChunk *get_mem_chunk();

    MarkClean *set_mem_chunk(MemoryChunk *chunk);

private:
    void mark_step();

    void clean_step();

public:
    void run() override;
};
