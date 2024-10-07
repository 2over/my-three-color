//
// Created by xiehao on 2024/9/6.
//

#pragma once

#ifndef MY_THREE_COLOR_MARK_CLEAN_H
#define MY_THREE_COLOR_MARK_CLEAN_H

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
    static void mark_from_oops();
    static void clean(MemoryChunk*);
};

#endif //MY_THREE_COLOR_MARK_CLEAN_H