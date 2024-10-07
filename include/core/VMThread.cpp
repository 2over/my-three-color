//
// Created by ziya on 2024/10/6.
//

#include "VMThread.h"
#include "Thread.h"
#include "SafepointSynchronize.h"
#include "../../include/gc/gc_factory.h"

extern MemoryChunk memChunk;

void VMThread::run() {
    if (!SafepointSynchronize::is_at_safepoint()) {
        ERROR_PRINT("[vm thread] 未启用安全点,禁止打标记\n");
        return ;
    }

    GCFactory::get_instance()->minor_run(DEFAULT_GC_TYPE, memChunk);
}
