#pragma once

#include "../common.h"

using namespace std;

class MemoryCell {
private:
    uint m_start;
    uint m_end;

    /**
     * Cell的数量,每个Cell占8字节
     */
    uint m_size;
    bool m_mark;

    /**
     * 是否是中转对象
     *
     * 在GC标记阶段会将原对象放入Chunk的transfer_table中(因为之前malloc返回的指针指向的是这个对象，这个对象释放了旧指针就失效了)
     * 然后生成一个克隆对象放入Chunk的used_table中用于后续的内存释放与整理(如果是多线程，GC阶段需要STW,否则内存的数据会被其他线程覆盖掉)
     * 这个属性就是表示是否是这个克隆对象
     */

    bool m_transfer_object;
private:
    /**
     * 这里实际类型位MemoryChunk,因为C++语言不允许互为成员，所以用pvoid代替
     */
    pvoid m_belong_chunk;

public:
    MemoryCell(uint start, uint size);

    MemoryCell(MemoryCell &cell);

    ~MemoryCell();

public:
    uint get_start();

    MemoryCell *set_start(uint val);

    uint get_end();

    MemoryCell *set_end(uint val);

    uint get_size();

    MemoryCell *set_size(uint val);

    bool get_mark();

    MemoryCell *set_mark(bool val);

    pvoid get_belong_chunk();

    MemoryCell *set_belong_chunk(pvoid chunk);

    bool get_transfer_object();

    MemoryCell *set_transfer_object(bool val);

    /**
     * m_start以step为步长递增
     *
     */
    MemoryCell *inc_start(uint step);

    MemoryCell *desc_start(uint step);

    MemoryCell *inc_end(uint step);

    MemoryCell *desc_end(uint step);

    MemoryCell *inc_size(uint step);

    MemoryCell *desc_size(uint step);

    void zero();

public:
    void to_string();

    void to_string(char *msg);

    pvoid ptr();
};
