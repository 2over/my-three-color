//
// Created by xiehao on 2024/9/26.
//

#include "Klass.h"
#include "../../../include/common.h"
#include "../../../include/memory/memory_pool.h"
#include "OopDesc.h"

extern MemoryPool memPool;
extern MemoryChunk *memChunk;

OopDesc *Klass::allocate_instance() {
    MemoryCell *cell = memChunk->malloc(16);

    OopDesc *o = static_cast<OopDesc *>(cell->ptr());
    o->set_mem_cell(cell);

    return o;
}

OopDesc *Klass::allocate_instance(char c) {
    MemoryCell *cell = memChunk->malloc(sizeof(OopDesc));

    OopDesc *o = static_cast<OopDesc *>(cell->ptr());
    o->set_mem_cell(cell);
    o->set_sign(c);

    // TODO 如果gc正在运行，新创建的对象直接加入黑色队列，打上标记
    return o;
}
