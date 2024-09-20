//
// Created by xiehao on 2024/9/5.
//
#include "../../include/common.h"
#include "../../include/memory/memory_cell.h"
#include "../../include/memory/memory_chunk.h"

MemoryCell::MemoryCell(uint start, uint size) : m_start(start), m_size(size) {
    m_end = m_start + m_size;
    m_mark = false;
}

MemoryCell::MemoryCell(MemoryCell &cell) {
    m_start = cell.m_start;
    m_end = cell.m_end;
    m_size = cell.m_size;

    m_mark = false;

    m_transfer_object = true;
}

MemoryCell::~MemoryCell() {

    switch (DEFAULT_GC_TYPE) {
        case GC_MARK_CLEAN: {
            INFO_PRINT("[调用析构函数%s] 释放资源\n", __func__);
            break;
        }
        case GC_MARK_COPY: {
            INFO_PRINT("[调用析构函数%s] 释放资源\n", __func__);
            to_string("\t 释放的cell信息");

            break;
        }

        case GC_MARK_COLLECT: {
            INFO_PRINT("[调用析构函数%s]释放资源\n", __func__);
            break;
        }
    }
}

uint MemoryCell::get_start() {
    return m_start;
}

MemoryCell *MemoryCell::set_start(uint val) {
    m_start = val;
    return this;
}

uint MemoryCell::get_end() {
    return m_end;
}

MemoryCell *MemoryCell::set_end(uint val) {
    m_end = val;
    return this;
}

uint MemoryCell::get_size() {
    return m_size;
}

MemoryCell *MemoryCell::set_size(uint val) {
    m_size = val;
    return this;
}

MemoryCell *MemoryCell::inc_start(uint step) {
    m_start += step;
    if (m_start > m_end) {
        ERROR_PRINT("cell end overflow\n");
        exit(1);
    }

    return this;
}

MemoryCell *MemoryCell::desc_start(uint step) {
    m_start -= step;
    if (m_start < 0) {
        ERROR_PRINT("cell start overflow\n");
        exit(1);
    }

    return this;
}


MemoryCell *MemoryCell::inc_end(uint step) {
    m_end += step;
    return this;
}

MemoryCell *MemoryCell::desc_end(uint step) {
    m_end -= step;
    return this;
}

MemoryCell *MemoryCell::inc_size(uint step) {
    m_size += step;
    return this;
}

MemoryCell *MemoryCell::desc_size(uint step) {
    m_size -= step;
    if (m_size < 0) {
        ERROR_PRINT("cell size overflow\n");
        exit(1);
    }

}

bool MemoryCell::get_mark() {
    return m_mark;
}

MemoryCell *MemoryCell::set_mark(bool val) {
    m_mark = val;
    return this;
}

pvoid MemoryCell::get_belong_chunk() {
    return m_belong_chunk;
}

MemoryCell *MemoryCell::set_belong_chunk(pvoid chunk) {
    m_belong_chunk = chunk;
    return this;
}

bool MemoryCell::get_transfer_object() {
    return m_transfer_object;
}

MemoryCell *MemoryCell::set_transfer_object(bool val) {
    m_transfer_object = val;
    return this;
}

// =================

void MemoryCell::to_string() {
    PRINT("[Cell信息]start = %d, end = %d, size =%d\n", m_start, m_end, m_size);
}

void MemoryCell::to_string(char *msg) {
    PRINT("[Cell信息]%s:start = %d, end = %d, size =%d\n", msg, m_start, m_end, m_size);
}

pvoid MemoryCell::ptr() {
    if (NULL == m_belong_chunk) {
        return NULL;
    }

    MemoryChunk *chunk = (MemoryChunk *) m_belong_chunk;
    return (pvoid) ((ulong) chunk->get_data() + get_start() * chunk->get_align_size());
}





