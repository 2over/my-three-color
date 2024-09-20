#pragma once

#include "../common.h"
#include "memory_cell.h"
#include <list>

using namespace std;

class MemoryChunk {
private:
    /**
     * 创建Chunk的文件名
     */
    char *m_filename;

    /**
     * 创建Chunk的文件位置
     */
    uint m_line;

    /**
     * 该Chunk的内存大小
     */
    uint m_size;

    /**
     * 以多少字节对齐
     */
    uint m_align_size;

    /**
     * 该Chunk包含多少Cell
     */
    uint m_cell_num;

    /**
     * 被用了的Cell数量
     */
    uint m_used_cell_num;

    /**
     * 当前Cell的起始位置
     * 复制算法会使用到
     */
    uint m_cell_start;

    /**
     * 需要释放内存的数据
     */
private:
    /**
     * 存储数据
     */
    pvoid m_data;

    list<MemoryCell *> m_available_table;
    list<MemoryCell *> m_used_table;

    /**
     * 空闲的内存
     * 复制算法用
     */
    list<MemoryCell *> m_idle_table;

    /**
     * 整理内存时中转用，暂存打了标记的对象，对象移动后释放
     */
    list<MemoryCell *> m_transfer_table;

private:
    MemoryCell *real_malloc(MemoryCell *available_cell, uint cell_num);

public:
    MemoryChunk(uint size, char *filename, uint line);

    ~MemoryChunk();

public:
    char *get_filename();

    uint get_line();

    uint get_size();

    pvoid get_data();

    uint get_align_size();

    uint get_cell_num();

    MemoryChunk *inc_used_cell_num(uint step);

    MemoryChunk *desc_used_cell_num(uint step);

    list<MemoryCell *> *get_available_table();

    list<MemoryCell *> *get_used_table();

    list<MemoryCell *> *get_transfer_table();

    list<MemoryCell *> *get_idle_table();

    uint get_cell_start();

    MemoryChunk *get_cell_start(uint val);

    uint get_new_cell_start();

    uint get_old_cell_start();

    MemoryChunk *renew_cell_start();

    MemoryChunk *set_available_table(list<MemoryCell *> &table);

    MemoryChunk *set_used_cell_num(uint val);

public:
    MemoryCell *malloc(uint size);

    /**
     * 标记-整理算法运行后分配内存
     */
    MemoryCell *malloc_after_gc(MemoryCell *transfer_cell);

    MemoryCell *malloc_after_mark_copy_gc(MemoryCell *used_cell);

    void free_available_table();

    void free_used_table();

public:
    void to_string();

    void print_available_table();

    void print_used_table();

    void print_transfer_table();

    void print_idle_table();

    void print_all_table();

};