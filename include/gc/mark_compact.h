//
// Created by xiehao on 2024/9/6.
//


#pragma once

#include "../common.h"
#include "interface/garbage_collect.h"
#include "../memory/memory_chunk.h"

class MarkCompact : public GarbageCollect {
private:
    MemoryChunk *m_mem_chunk;

public:
    MarkCompact(MemoryChunk *chunk);

    ~MarkCompact();

private:
    void memory_compact_before();

    void memory_compact_after();

public:
    MemoryChunk *get_mem_chunk();

    MarkCompact *set_mem_chunk(MemoryChunk *chunk);

private:
    void mark_step();

    void clean_step();

    void memory_compact_step();

    void data_compact_step();

public:
    void run() override;
};