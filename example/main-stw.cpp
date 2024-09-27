//
// Created by xiehao on 2024/9/27.
//
#include "../include/common.h"
#include "../include/core/ThreeColorMap.h"
#include "../include/memory/memory_pool.h"

ThreeColorMap threeColorMap;

MemoryPool memPool;

MemoryChunk* memChunk = memPool.new_chunk(1024);

void* thread_do(void* arg) {
    Thread* Self = static_cast<Thread *>(arg);

    for (int i = 0; i < 1000000; i++) {
        INFO_PRINT("%s->%d\n", Self->name().)
    }
}