//
// Created by xiehao on 2024/9/27.
//
#include "../include/common.h"
#include "../include/core/ThreeColorMap.h"
#include "../include/core/Thread.h"
#include "../include/memory/memory_pool.h"
#include "../include/core/SafepointSynchronize.h"
#include "../share/vm/oops/Klass.h"
#include "../include/container/stack.h"
#include "../share/vm/runtime/StackValue.h"
#include "../include/core/ParkEvent.h"
#include "../include/core/Threads.h"
#include "../include/core/VMThread.h"
#include "../include/gc/mark_clean.h"

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

    Self->stack()->push(v1);
    Self->stack()->push(v2);
    Self->stack()->push(v3);
    Self->stack()->push(v4);
    Self->stack()->push(v5);

    // 模拟程序运行
    for (int i = 0; i < 1000000; ++i) {
        INFO_PRINT("%s->%d\n", Self->name().c_str(), i);
        sleep(1);
        SafepointSynchronize::insert_safepoint();
    }

    return 0;
}
/**
 * 用于模拟运行期间
 * @param arg
 * @return
 */
void* t2_do(void* arg) {
    Thread* Self = static_cast<Thread *>(arg);
    Self->_ParkEvent->park();

    INFO_PRINT("线程%s: ");
    return 0;
}

void user_thread_work() {
    Thread* t1 = new Thread(t1_do, NULL, 1);
    Thread* t2 = new Thread(thread_do, NULL, 2);

    t1->run();
    t2->run();

    getchar();
}

int main() {
    Threads::init_2();
    SafepointSynchronize::init();

    int v;
    while (true) {
        INFO_PRINT("请输入命令: \n");
        cin >> v;
        switch(v) {
            case 1: {
                user_thread_work();
                break;
            }
            case 2: {
                SafepointSynchronize::begin();
                break;
            }
            case 3: {
                SafepointSynchronize::end();
                break;
            }
            case 4: {
                /**
                 * 开始遍历线程栈
                 * 1 main线程去做这件事情
                 * 2 线程池去做这件事情
                 */
                Threads::build_oop_maps();
                break;
            }
            case 5: {
                VMThread::run();
                break;
            }
            case 6: {
                MarkClean::mark_from_oops();
            }
        }
    }
}
