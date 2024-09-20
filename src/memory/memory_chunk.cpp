//
// Created by xiehao on 2024/9/6.
//

#include "../../include/common.h"
#include "../../include/memory/memory_chunk.h"
#include "../../include/gc/gc_factory.h"
#include <string.h>

MemoryChunk::MemoryChunk(uint size, char *filename, uint line) : m_size(size), m_filename(filename), m_line(line) {
    m_align_size = ALIGN_SIZE;
    m_cell_num = ((size - 1) / m_align_size) + 1;
    m_size = m_cell_num * m_align_size;

    this->m_data = calloc(m_size, sizeof(byte));
    if (NULL == this->m_data) {
        ERROR_PRINT("分配内存失败\n");
        exit(1);
    }

    switch (DEFAULT_GC_TYPE) {
        case GC_MARK_CLEAN:
        case GC_MARK_COLLECT: {
            m_used_cell_num = 0;

            m_available_table.push_front(new MemoryCell(0, m_cell_num));

            print_available_table();
            break;
        }
        case GC_MARK_COPY: {
            m_used_cell_num = 0;
            m_cell_start = 0;

            m_available_table.push_front(new MemoryCell(0, m_cell_num / 2));
            m_idle_table.push_front(new MemoryCell(m_cell_num / 2, m_cell_num / 2));

            print_available_table();
            print_idle_table();
            break;
        }
    }


}

MemoryChunk::~MemoryChunk() {
    PRINT("[调用析构函数%s]释放资源\n", __func__);

    if (m_data) {
        PRINT("\t 释放资源,申请内存位置( %s:%d ),内存大小:%d 字节\n", m_filename, m_line, m_size);
        free(m_data);
    }

    free_available_table();
    free_used_table();
}

// =================
MemoryCell *MemoryChunk::real_malloc(MemoryCell *available_cell, uint cell_num) {
#ifdef DEBUG
    /**
     * 计算出内存地址
     */
    pvoid ret = (pvoid) ((ulong) get_data() + available_cell->get_start() * get_align_size());

    INFO_PRINT("[真正分配内存]Data起始地址=%X, cell_start =%d, ret =%X\n", get_data(), available_cell->get_start(),
               ret);
#endif

    /**
     * 创建used cell并加入list
     */
    MemoryCell *used_cell = new MemoryCell(available_cell->get_start(), cell_num);

    used_cell->set_belong_chunk(this);

    m_used_table.push_front(used_cell);

    /**
     * 处理cell信息
     */
    available_cell->inc_start(cell_num)->desc_size(cell_num);

    /**
     * 更新used_cell_num
     */
    inc_used_cell_num(cell_num);

    /**
     * 如果Chunk用光了，就清空available_table
     */
    switch (DEFAULT_GC_TYPE) {
        case GC_MARK_COPY: {
            if (0 == available_cell->get_size()) {
                free_available_table();
            }
            break;
        }
        case GC_MARK_COLLECT: {
            if (m_cell_num == m_used_cell_num) {
                free_available_table();
            }
            break;
        }
    }


    return used_cell;
}

// ===========
pvoid MemoryChunk::get_data() {
    return this->m_data;
}

char *MemoryChunk::get_filename() {
    return m_filename;
}

uint MemoryChunk::get_line() {
    return m_line;
}

uint MemoryChunk::get_size() {
    return m_size;
}

uint MemoryChunk::get_align_size() {
    return m_align_size;
}

uint MemoryChunk::get_cell_num() {
    return m_cell_num;
}

MemoryChunk *MemoryChunk::inc_used_cell_num(uint step) {
    m_used_cell_num += step;

    if (m_used_cell_num > m_cell_num) {
        ERROR_PRINT("cell of chunk size overflow\n");
        exit(1);
    }

    return this;
}

MemoryChunk *MemoryChunk::desc_used_cell_num(uint step) {
    m_used_cell_num -= step;
    if (m_used_cell_num < 0) {
        ERROR_PRINT("cell of chunk size overflow\n");
        exit(1);
    }
    return this;
}

list<MemoryCell *> *MemoryChunk::get_available_table() {
    return &m_available_table;
}

list<MemoryCell *> *MemoryChunk::get_used_table() {
    return &m_used_table;
}

list<MemoryCell *> *MemoryChunk::get_transfer_table() {
    return &m_transfer_table;
}

list<MemoryCell *> *MemoryChunk::get_idle_table() {
    return &m_idle_table;
}

uint MemoryChunk::get_cell_start() {
    return m_cell_start;
}

MemoryChunk *MemoryChunk::get_cell_start(uint val) {
    m_cell_start = val;
    return this;
}

uint MemoryChunk::get_new_cell_start() {
    return (0 == m_cell_start) ? m_cell_num / 2 : 0;
}

uint MemoryChunk::get_old_cell_start() {
    return get_new_cell_start();
}

MemoryChunk *MemoryChunk::renew_cell_start() {
    m_cell_start = get_new_cell_start();
    return this;
}

MemoryChunk *MemoryChunk::set_available_table(list<MemoryCell *> &table) {
    m_available_table = table;
    return this;
}

MemoryChunk *MemoryChunk::set_used_cell_num(uint val) {
    m_used_cell_num = val;
    return this;
}

// ========
MemoryCell *MemoryChunk::malloc(uint size) {
    MemoryCell *ret = NULL;
    if (0 == size) {
        ERROR_PRINT("申请的内存大小不得等于0\n");
        exit(1);
    }

    uint cell_num = ((size - 1) / m_align_size) + 1;
    if (cell_num > m_cell_num) {
        ERROR_PRINT("需要的内存(%d字节)超过最大可用内存(%d字节)\n", size, m_size);
        exit(1);
    }

    /**
     * 遍历available_table查找满足条件的MemoryCell
     */
    list<MemoryCell *>::iterator available_iterator;
    for (available_iterator = m_available_table.begin();
         available_iterator != m_available_table.end(); available_iterator++) {
        MemoryCell *cell = *available_iterator;
        if (cell->get_size() >= cell_num) {
            cell->to_string("找到了满足条件的cell");

            ret = real_malloc(cell, cell_num);
            break;
        }
    }

    if (NULL == ret) {
        ERROR_PRINT("没有满足条件的Chunk(cell_num=%d),无法分配内存，执行GC\n", cell_num);
        GCFactory::get_instance()->minor_run(DEFAULT_GC_TYPE, this);
        ret = malloc(size);
    }

    print_all_table();
    return ret;
}

MemoryCell *MemoryChunk::malloc_after_gc(MemoryCell *transfer_cell) {
    MemoryCell *ret = NULL;

    pvoid ptr_from = NULL;
    pvoid ptr_to = NULL;

    list<MemoryCell *>::iterator available_iterator;
    for (available_iterator = m_available_table.begin();
         available_iterator != m_available_table.end(); available_iterator++) {
        MemoryCell *cell = *available_iterator;

        if (cell->get_size() >= transfer_cell->get_size()) {
            cell->to_string("找到了满足条件的Cell");
            /* 原地址:数据的位置 */
            ptr_from = transfer_cell->ptr();

            /* 调整 transfer cell后插入used_table */
            transfer_cell->set_start(cell->get_start());
            transfer_cell->set_end(transfer_cell->get_start() + transfer_cell->get_size());

            /* 新地址，需要将数据拷贝过来 */
            ptr_to = transfer_cell->ptr();

            INFO_PRINT("开始拷贝内存: ptr_from=%X, ptr_to=%X, size=%d\n", ptr_from, ptr_to,
                       transfer_cell->get_size() * get_align_size());

            memcpy(ptr_to, ptr_from, transfer_cell->get_size() * get_align_size());

            m_used_table.push_front(transfer_cell);

            /* 处理Cell信息 */
            cell->inc_start(transfer_cell->get_size())->desc_size(transfer_cell->get_size());

            /* 更新used_cell_num */
            inc_used_cell_num(transfer_cell->get_size());

            ret = transfer_cell;

            /**
             * 如果Chunk用光了，就清空available_table
             */
            switch (DEFAULT_GC_TYPE) {
                case GC_MARK_COLLECT: {
                    if (m_cell_num == m_used_cell_num) {
                        free_available_table();
                    }
                    break;
                }
                case GC_MARK_COPY: {
                    if (0 == cell->get_size()) {
                        free_available_table();
                    }
                    break;
                }
                case GC_MARK_CLEAN: {
                    if (m_cell_num == m_used_cell_num) {
                        free_available_table();
                    }
                    break;
                }
            }


            break;
        }
    }

    return ret;
}

MemoryCell *MemoryChunk::malloc_after_mark_copy_gc(MemoryCell *used_cell) {
    return malloc_after_gc(used_cell);
}

void MemoryChunk::free_available_table() {
    PRINT("[释放available_table]开始\n");

    list<MemoryCell *>::iterator tmp;
    for (tmp = m_available_table.begin(); tmp != m_available_table.end(); tmp++) {
        delete (*tmp);
    }

    m_available_table.clear();

    switch (DEFAULT_GC_TYPE) {
        case GC_MARK_COPY: {
            PRINT("[释放available_table]结束\n");

            break;
        }
        case GC_MARK_COLLECT: {
            break;
        }
    }
}

void MemoryChunk::free_used_table() {
    PRINT("[释放used_table]开始\n");

    list<MemoryCell *>::iterator tmp;
    for (tmp = m_used_table.begin(); tmp != m_used_table.end(); tmp++) {
        delete (*tmp);
    }
    m_available_table.clear();
}


// =========
void MemoryChunk::to_string() {
    PRINT("[打印used_table]开始, size = %d\n", get_used_table()->size());

    list<MemoryCell *>::iterator tmp;
    for (tmp = m_used_table.begin(); tmp != m_used_table.end(); tmp++) {
        PRINT("\t %X: cell_start=%d, cell_end=%d, cell_size=%d\n",
              *tmp, (*tmp)->get_start(), (*tmp)->get_end(), (*tmp)->get_size());
    }

    PRINT("打印used_table]结束\n");
}

void MemoryChunk::print_available_table() {
    PRINT("[打印available_table]开始,  size=%d\n", get_available_table()->size());
    list<MemoryCell *>::iterator tmp;

    for (tmp = m_available_table.begin(); tmp != m_available_table.end(); tmp++) {
        PRINT("\t %X: cell_start=%d, cell_end=%d, cell_size=%d\n", *tmp, (*tmp)->get_start(), (*tmp)->get_end(),
              (*tmp)->get_size());
    }

    PRINT("[打印available_table]结束\n");
}

void MemoryChunk::print_used_table() {
    PRINT("[打印used_table]开始, size=%d\n", get_used_table()->size());
    list<MemoryCell *>::iterator tmp;

    for (tmp = m_used_table.begin(); tmp != m_used_table.end(); tmp++) {
        PRINT("\t %X: cell_start=%d, cell_end=%d, cell_size=%d\n", *tmp, (*tmp)->get_start(), (*tmp)->get_end(),
              (*tmp)->get_size());
    }

    PRINT("[打印used_table]结束\n");
}

void MemoryChunk::print_transfer_table() {
    PRINT("[打印transfer_table]开始, size=%d\n", get_transfer_table()->size());

    list<MemoryCell *>::iterator tmp;

    for (tmp = m_transfer_table.begin(); tmp != m_transfer_table.end(); tmp++) {
        PRINT("\t %X: cell_start=%d, cel_end=%d, cell_size=%d\n",
              *tmp, (*tmp)->get_start(), (*tmp)->get_end(), (*tmp)->get_size());
    }

    PRINT("[打印transfer_table]结束\n");
}

void MemoryChunk::print_idle_table() {
    PRINT("[打印idle_table]开始: size=%d\n", get_idle_table()->size());

    list<MemoryCell *>::iterator tmp;

    for (tmp = m_idle_table.begin(); tmp != m_idle_table.end(); tmp++) {
        PRINT("\t %X: cell_start=%d, cell_end=%d, cell_size=%d\n", *tmp, (*tmp)->get_start(), (*tmp)->get_end(),
              (*tmp)->get_size());
    }

    PRINT("[打印idle_table]结束\n");
}

void MemoryChunk::print_all_table() {


    switch (DEFAULT_GC_TYPE) {
        case GC_MARK_CLEAN: {
            print_available_table();

            print_used_table();
            break;
        }
        case GC_MARK_COPY: {
            print_available_table();

            print_used_table();

            print_transfer_table();

            print_idle_table();
            break;
        }
        case GC_MARK_COLLECT: {

            print_available_table();

            print_used_table();

            print_transfer_table();
            break;
        }
    }

}
