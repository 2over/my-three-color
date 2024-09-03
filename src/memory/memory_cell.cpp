//
// Created by 87766 on 2024/9/2.
//

#include "../../include/memory/memory_cell.h"
#include "../../include/common.h"
#include "../../include/memory/memory_cell.h"
#include "../../include/memory/memory_chunk.h"

MemoryCell::MemoryCell(uiint start, uint size):m_start(start), m_size(size)
{
    m_end = m_start + m_size;
    m_mark = false;
}

MemoryCell::MemoryCell(MemoryCell &cell)
{
    m_start = cell.m_start;
    m_end == cell.m_end;
    m_size = cell.m_size;
    m_mark = false;
    m_transfer_object = true;
}