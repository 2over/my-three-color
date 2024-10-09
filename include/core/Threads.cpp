//
// Created by xiehao on 2024/10/9.
//

#include "Threads.h"
#include "../common.h"
#include "Thread.h"
#include "ParkEvent.h"
#include "SafepointSynchronize.h"
#include "../container/stack.h"
#include "../../share/vm/runtime/StackValue.h"
#include "../../share/vm/compiler/OopMapSet.h"
#include "../../share/vm/compiler/OopMap.h"
#include "../../share/vm/oops/OopDesc.h"

OopMapSet oopMapSet;
map<pthread_t, Thread *> Threads::_threads_map;

queue<oop> Threads::_satb_mark_queue;

void sighandler(int sig) {
    if (SIGSEGV == sig) {
        INFO_PRINT("开始暂停所有用户线程\n");

        Thread *thread = Threads::threads()[pthread_self()];

        INFO_PRINT("\t [%s] 进入阻塞\n", thread->name().c_str());

        // STW
        SafepointSynchronize::_waiting_to_block--;

        thread->_ParkEvent->park();
    }
}

void Threads::init_2() {
    if (SIG_ERR != signal(SIGSEGV, sighandler)) {
        INFO_PRINT("注册信号处理成功: SIGSEGV\n");
    }
}

void Threads::add_current_thread(pthread_t t, Thread *thread) {
    _threads_map[t] = thread;
}

void Threads::build_oop_maps() {
    if (!SafepointSynchronize::is_at_safepoint()) {
        ERROR_PRINT("[main thread] 未启用安全点,禁止执行创建OopMap操作\n");
        return;
    }

    map<pthread_t, Thread *>::iterator ite;

    INFO_PRINT("[main thread] 开始生成 OopMap\n");
    for (ite = Threads::threads_map()->begin(); ite != Threads::threads_map()->end(); ite++) {
        Thread *thread = ite->second;
        Stack *stack = thread->stack();

        OopMap *map = new OopMap(thread);
        oopMapSet.add_gc_map(map);

        INFO_PRINT("\t [main thread] 开始遍历线程%s的栈， 栈数据:%d\n", thread->name().c_str(), stack->size());

        for (int i = 0; i < stack->size(); ++i) {
            StackValue *v = (StackValue *) stack->popData();
            oop o = oop(v->get_ptr());

            if (T_OBJECT == v->type()) {
                map->set_xxx(long(v->get_ptr()));

                INFO_PRINT("\t\t index=%d的位置为对象%c(field size=%d), 写入OopMap\n",
                           i, o->sign(), o->field_oops().capacity(), v->get_ptr());
            }
        }
    }
}