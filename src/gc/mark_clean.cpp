//
// Created by xiehao on 2024/9/6.
//

#include "../../include/gc/mark_clean.h"

MarkClean::MarkClean(MemoryChunk *mem_chunk) : m_mem_chunk(mem_chunk) {
    INFO_PRINT("%s\n", __func__);
}

MarkClean::~MarkClean() {
    INFO_PRINT("[调用析构函数%s]释放资源\n", __func__);
}

// =========
MemoryChunk *MarkClean::get_mem_chunk() {
    return m_mem_chunk;
}

MarkClean *MarkClean::set_mem_chunk(MemoryChunk *chunk) {
    m_mem_chunk = chunk;
    return this;
}

// =========
void MarkClean::mark_step() {
    PRINT("\t 开始执行标记操作\n");

    /**
     * 模拟可达性分析算法标记对象，回收未打标记的对象
     */

    list<MemoryCell *> *used_table = get_mem_chunk()->get_used_table();

    list<MemoryCell *>::iterator iterator;
    for (iterator = used_table->begin(); iterator != used_table->end(); iterator++) {
        MemoryCell *cell = *iterator;

        if (cell->get_size() == 4) {
            (*iterator)->set_mark(true);
        }
    }
}

void MarkClean::clean_step() {
    PRINT("\t 开始执行清理操作\n");

    bool flag = false;

    list<MemoryCell *>::iterator iterator;

    for (iterator = get_mem_chunk()->get_used_table()->begin(); iterator != get_mem_chunk()->get_used_table()->end();) {
        MemoryCell *cell = *iterator;

        cell->to_string("检测是否打了标记");

        /* 释放未打标记的对象 */
        if (!cell->get_mark()) {
            INFO_PRINT("\t 未打标记\n");

            /* 处理used_table */
            iterator = get_mem_chunk()->get_used_table()->erase(iterator);

            /* 处理available_table */
            get_mem_chunk()->get_available_table()->push_front(new MemoryCell(cell->get_start(), cell->get_size()));

            get_mem_chunk()->desc_used_cell_num(cell->get_size());

            /* 释放对象 */
            delete cell;

            flag = true;

        } else {
            INFO_PRINT("\t 打了标记,无法回收\n");
            iterator++;
        }
    }

    if (!flag) {
        ERROR_PRINT("[GC后]没有可回收的内存，无法分配内存，执行FullGC\n");

        exit(1);
    }
}

// ==========

void MarkClean::run() {
    PRINT("[标记-清除算法]开始运行\n");

    if (NULL == get_mem_chunk()) {
        ERROR_PRINT("\t 请传入需要释放的Chunk\n");

        exit(1);
    }

    mark_step();

    clean_step();

    get_mem_chunk()->print_all_table();
    PRINT("[标记-清除算法]结束运行\n");
}