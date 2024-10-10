//
// Created by xiehao on 2024/9/6.
//

#include "../../include/gc/mark_clean.h"
#include "../../share/vm/compiler/OopMapSet.h"
#include "../../share/vm/compiler/OopMap.h"
#include "../../include/core/ThreeColorMap.h"
#include "../../share/vm/code/CompressedReadStream.h"
#include "../../share/vm/code/CompressedWriteStream.h"
#include "../../share/vm/oops/OopDesc.h"
#include "../../include/core/Thread.h"


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

extern OopMapSet oopMapSet;

extern ThreeColorMap threeColorMap;

void MarkClean::mark_from_oops() {
    INFO_PRINT("[vm thread] 开始标记Root\n");

//    INFO_PRINT("------------------------------%d\n", oopMapSet.om_count());
    for (int i = 0; i < oopMapSet.om_count(); i++) {
        OopMap *map = oopMapSet.om_data()[i];

        CompressedReadStream *stream = new CompressedReadStream(map->write_stream()->buffer(),
                                                                map->write_stream()->position());
        INFO_PRINT("\t [vm thread] 开始处理线程%s的Root oop: %d\n",
                   map->thread()->name().c_str(), map->write_stream()->position());
//        INFO_PRINT("------------------------------%d\n", stream->position());
        for (int j = 0; j < stream->position(); ++j) {
            oop o = oop(stream->read(j));

            INFO_PRINT("\t\t [vm thread]对象%c(%d) 加入gray table\n", o->sign(), o->field_oops().capacity());

            threeColorMap.gray_table().push(o);
        }
    }

    INFO_PRINT("[vm thread]完成标记Root\n");
}

void MarkClean::clean(MemoryChunk *chunk) {
    PRINT("开始执行清理操作\n");

    bool flag = false;

    list<MemoryCell *>::iterator iterator;

    for (iterator = chunk->get_used_table()->begin(); iterator != chunk->get_used_table()->end();) {
        MemoryCell *cell = *iterator;

        oop o = oop(cell->ptr());
        cell->to_string("检测是否打了标记");

        // 释放未打标记的对象
        if (!cell->get_mark()) {
            INFO_PRINT("\t 对象%c: 未打标记，清理之\n", o->sign());

            // 处理used_table
            iterator = chunk->get_used_table()->erase(iterator);

            // 处理available_table
            chunk->get_available_table()->push_front(new MemoryCell(cell->get_start(), cell->get_size()));

            chunk->desc_used_cell_num(cell->get_size());

            // 释放对象
            cell->zero();

            delete cell;

            flag = true;
        } else {
            INFO_PRINT("\t 对象%c: 打了标记, 无法回收\n", o->sign());
            iterator++;
        }
    }

    PRINT("清理操作结束\n");
}