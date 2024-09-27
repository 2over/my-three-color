#include "Thread.h"
#include "Threads.h"

//
// Created by xiehao on 2024/9/27.
//
volatile int CreateThreadLock = 0;

void* thread_run(void* arg) {
    Thread* t = static_cast<Thread *>(arg);

    Threads::add_current_thread(pthread_self(), t);

    Monitor* sync = t->startThread_lock();
    sync->wait(t;)
}

