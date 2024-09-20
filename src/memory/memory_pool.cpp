//
// Created by xiehao on 2024/9/6.
//


#include "../../include/memory/memory_pool.h"

MemoryPool::~MemoryPool()
{
    INFO_PRINT("[调用析构函数]%s\n", __func__);

    free_chunks();
}

// ===========

MemoryChunk *MemoryPool::new_chunk(uint mem_size)
{
    MemoryChunk *mem_chunk = new MemoryChunk(mem_size, __FILE__, __LINE__);
    this->m_chunks.push_front(mem_chunk);

    return mem_chunk;
}

// =========

void MemoryPool::print_chunks()
{
    INFO_PRINT("[打印未释放的内存]开始\n");

    list<MemoryChunk *>::iterator iterator;

    for (iterator = m_chunks.begin(); iterator != m_chunks.end(); iterator++) {
        MemoryChunk *chunk = *iterator;

        INFO_PRINT("\t [未释放的内存]申请位置:( %s:%d ), 内存大小:%d 字节\n", chunk->get_filename(), chunk->get_line(), chunk->get_size());
    }

    INFO_PRINT("[打印未释放的内存]结束\n");
}

void MemoryPool::free_chunks()
{
    list<MemoryChunk *>::iterator iterator;
    for (iterator = m_chunks.begin(); iterator != m_chunks.end(); iterator++) {
        delete (*iterator);
    }

    // TODO 网上说clear时会调用每个元素的析构函数，但是测试发现并不会
    m_chunks.clear();
}

