//
// Created by xiehao on 2024/9/6.
//

#pragma once

#include "memory_chunk.h"
#include <iostream>
#include <list>

using namespace std;

class MemoryPool {
    /**
     * 所有需要释放内存的成员
     */
private:
    list<MemoryChunk *> m_chunks;

public:
    ~MemoryPool();

public:
    /**
     * 创建新的chunk
     */
    MemoryChunk *new_chunk(uint mem_size);

public:
    /**
     * 打印所有的Chunk
     */
    void print_chunks();

    /**
     * 释放所有Chunk占用的内存
     */
    void free_chunks();
};