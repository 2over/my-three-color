//
// Created by xiehao on 2024/9/6.
//

#include "../common.h"
#include "../memory/memory_pool.h"


typedef enum {
    GC_STATUS_CLOSED,
    GC_STATUS_WORKING
} GC_STATUS;

class GCFactory {
private:
    GC_STATUS m_status;

public:
    static GCFactory *get_instance() {
        static GCFactory factory;
        return &factory;
    }

public:
    GC_STATUS get_status();

    GCFactory *set_status(GC_STATUS status);

public:
    void minor_run(GC_TYPE type, MemoryChunk *mem_chunk);

    void major_run(GC_TYPE type, MemoryChunk *mem_chunk);

    void full_run(GC_TYPE type, MemoryPool *memoryPool);
};