//
// Created by xiehao on 2024/9/13.
//

#include "../../include/gc/mark_copy.h"

MarkCopy::MarkCopy(MemoryChunk *chunk) : m_mem_chunk(chunk) {

}

MarkCopy::~MarkCopy() {

}

// =======
MemoryChunk *MarkCopy::get_mem_chunk() {
    return m_mem_chunk;
}

MarkCopy *MarkCopy::set_mem_chunk(MemoryChunk *chunk) {
    m_mem_chunk = chunk;
    return this;
}

// =======
void MarkCopy::mark_step() {
    PRINT("\t 开始执行标记操作\n");

    /**
     * 模拟可达性析算法标记对象，回收未打标记的对象爱嗯
     */
    list<MemoryCell *> *used_table = get_mem_chunk()->get_used_table();

    list<MemoryCell *>::iterator iterator;
    for (iterator = used_table->begin(); iterator != used_table->end(); iterator++) {
        MemoryCell *cell = *iterator;

        cell->to_string("模拟打标记");

        // 打标记的条件
        if (4 == cell->get_size() || 1 == cell->get_size()) {

            INFO_PRINT("\t 打上标记 \n");

            (*iterator)->set_mark(true);
        } else {
            INFO_PRINT("\t 未打标记\n");
        }
    }
}

void MarkCopy::stop_the_world() {
    INFO_PRINT("\t GC, 暂停所有用户线程\n ");
}

void MarkCopy::memory_sweep_step() {
    // 检测to 区是否有碎片
    if (1 != m_mem_chunk->get_idle_table()->size()) {
        ERROR_PRINT("To区有内存碎片，无法完成GC\n");
        exit(1);
    }

    // 修改cell_start
    get_mem_chunk()->renew_cell_start();
    get_mem_chunk()->set_used_cell_num(0);

    // 释放available_table
    get_mem_chunk()->free_available_table();

    // 空间置换
    get_mem_chunk()->set_available_table(*(get_mem_chunk()->get_idle_table()));

    get_mem_chunk()->get_idle_table()->pop_front();

    get_mem_chunk()->get_idle_table()->push_front(
            new MemoryCell(get_mem_chunk()->get_old_cell_start(), get_mem_chunk()->get_cell_num() / 2));

    // 内存移动
    list<MemoryCell *>::iterator used_ite = get_mem_chunk()->get_used_table()->begin();

    while (used_ite != get_mem_chunk()->get_used_table()->end()) {
        MemoryCell *used_cell = *used_ite;

        used_cell->to_string();

        if (!used_cell->get_mark()) {
            INFO_PRINT("\t 未打标记, 开始释放资源\n");

            delete used_cell;
        } else {
            INFO_PRINT("\t 打了标记，开始处理\n");
            get_mem_chunk()->malloc_after_gc(used_cell);
        }

        used_ite = get_mem_chunk()->get_used_table()->erase(used_ite);
    }
}

// ========
void MarkCopy::run() {
    // 打标记
    mark_step();

    // 暂停所有用户线程
    stop_the_world();

    // 内存交换
    memory_sweep_step();
}

