//
// Created by xiehao on 2024/9/6.
//

#include "../../include/common.h"
#include "../../include/gc/gc_factory.h"
#include "../../include/gc/mark_clean.h"
#include "../../include/gc/mark_compact.h"
#include "../../include/gc/mark_copy.h"


GC_STATUS GCFactory::get_status() {
    return m_status;
}

GCFactory *GCFactory::set_status(GC_STATUS status) {
    m_status = status;
    return this;
}

// ========

void GCFactory::minor_run(GC_TYPE type, MemoryChunk *mem_chunk) {
    INFO_PRINT("%s\n", __func__);

    if (GC_STATUS_WORKING == get_status()) {
        WARNING_PRINT("GC正在运行，禁止重复运行\n");

        return;
    }

    set_status(GC_STATUS_WORKING);

    switch (type) {
        case GC_MARK_CLEAN:
            (new MarkClean(mem_chunk))->run();
            break;
        case GC_MARK_COLLECT:
            (new MarkCompact(mem_chunk))->run();
            break;
        case GC_MARK_COPY:
            (new MarkCopy(mem_chunk))->run();
            break;
        case GC_G1:
            break;
    }

    set_status(GC_STATUS_CLOSED);
}


void GCFactory::major_run(GC_TYPE type, MemoryChunk *mem_chunk) {

}

void GCFactory::full_run(GC_TYPE type, MemoryPool *memoryPool) {

}
