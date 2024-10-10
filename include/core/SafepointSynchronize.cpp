//
// Created by xiehao on 2024/9/30.
//

#include "SafepointSynchronize.h"
#include "../common.h"
#include "Threads.h"
#include "Thread.h"
#include "ParkEvent.h"

SafepointSynchronize::SynchronizeState volatile SafepointSynchronize::_state = SafepointSynchronize::_not_synchronized;

volatile int SafepointSynchronize::_waiting_to_block = 0;
void *SafepointSynchronize::_polling_page = NULL;

static volatile int PageArmed = 0;      // safepoint polling page is RO | RW vs PROT_NONE

void SafepointSynchronize::begin() {
    INFO_PRINT("安全点生效\n");

    _state = _synchronizing;

    _waiting_to_block = Threads::threads_map()->size();

    SafepointSynchronize::make_polling_page_unreadable();

    PageArmed = 1;

    // 等待所有线程进入安全点
    while(0 != _waiting_to_block) {
        INFO_PRINT("\t [main thread]等待所有用户线程进入安全点\n");
        sleep(1);
    }
    INFO_PRINT("\t [main thread] 所有用户线程进入安全点,退出\n");

    _state = _synchronized;
}

void SafepointSynchronize::end() {
    if (PageArmed) {
        INFO_PRINT("安全点失效\n");

        SafepointSynchronize::make_polling_page_readable();

        PageArmed = 0;
    }

    resume_all_thread();
    _state = _not_synchronized;
}

void SafepointSynchronize::init() {
    _polling_page = mmap(
            NULL,       // 映射区的开始地址,设置为NULL或0表示由系统决定
            4,          // 申请的内存大小按照内存页对其，这里直接调用函数获取内存页大小
            PROT_READ | PROT_WRITE | PROT_EXEC, // 映射的内存区的权限: 可读可写可执行
            MAP_ANONYMOUS | MAP_PRIVATE,    // 映射对象的类型
            -1,
            0
            );
}

void SafepointSynchronize::resume_all_thread() {
    map<pthread_t, Thread*>::iterator ite;
    for (ite = Threads::threads_map()->begin(); ite != Threads::threads_map()->end(); ite++) {
        Thread* thread = ite->second;
        INFO_PRINT("\t [%s] 安全点结束,被唤醒\n", thread->name().c_str());

        thread->_ParkEvent->unpark();
    }
}

