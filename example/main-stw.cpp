//
// Created by xiehao on 2024/9/27.
//
#include "../include/common.h"
#include "../include/core/ThreeColorMap.h"
#include "../include/core/Thread.h"
#include "../include/memory/memory_pool.h"
#include "../include/core/SafepointSynchronize.h"
#include "../share/vm/oops/Klass.h"
#include "../share/vm/runtime/StackValue.h"

ThreeColorMap threeColorMap;

MemoryPool memPool;

MemoryChunk *memChunk = memPool.new_chunk(1024);

void *thread_do(void *arg) {
    Thread *Self = static_cast<Thread *>(arg);

    for (int i = 0; i < 1000000; i++) {
        INFO_PRINT("%s->%d\n", Self->name().c_str(), i);

        sleep(1);

        SafepointSynchronize::insert_safepoint();
    }
    return 0;
}

/**
 * A
 * B
 * C
 * D
 *  | - F
 *  | - G
 * E
 *  | - H
 *      | - I
 *      | - J
 *          | - K
 */
oop d = (new Klass)->allocate_instance('D');
oop e = (new Klass)->allocate_instance('E');
oop h = (new Klass)->allocate_instance('H');
oop j = (new Klass)->allocate_instance('J');

void* t1_do(void *arg) {
    Thread* Self = static_cast<Thread*>(arg);

    // 模拟没有被引用的GC Root oop
    (new Klass)->allocate_instance('A');
    (new Klass)->allocate_instance('B');
    (new Klass)->allocate_instance('C');

    d->field_oops().push_back((new Klass)->allocate_instance('F'));
    d->field_oops().push_back((new Klass)->allocate_instance('G'));

    INFO_PRINT("%d\n", d->field_oops().capacity());

    e->field_oops().push_back(h);

    h->field_oops().push_back((new Klass)->allocate_instance('I'));

    h->field_oops().push_back(j);

    j->field_oops().push_back((new Klass)-> allocate_instance('K'));

    // 模拟数据入栈
    StackValue* v1 = new StackValue(T_INT, 1);
    StackValue* v2 = new StackValue(T_BYTE, 2);
    StackValue* v3 = new StackValue(T_OBJECT, d);
    StackValue* v4 = new StackValue(T_CHAR, 3);
    StackValue* v5 = new StackValue(T_OBJECT, e);

    Self->stack()->push
    Self->stack()->push_heap(v2);

}
