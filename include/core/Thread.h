//
// Created by xiehao on 2024/9/20.
//

#ifndef MY_THREE_COLOR_THREAD_H
#define MY_THREE_COLOR_THREAD_H

#include "../common.h"
#include "../../share/vm/oops/OopDesc.h"
class Monitor;
class ParkEvent;
class Stack;

enum ThreadState {
    ALLOCATED,              // Memory has benn allocated but not initialized
    INITIALIZED,            // The thread hash been initialized but yet started
    RUNNABLE,               // Has been started and is runnable, but not necessarily running
    MONITOR_WAIT,           // Waiting on a contented monitor lock
    CONDVAR_WAIT,           // Waiting on a condition variable
    OBJECT_WAIT,            // Waiting on an Object.wait() call
    BREAKPOINTED,           // Suspended at breakpoint
    SLEEPING,               // Thread.sleep()
    ZOMBIE,                 // ALL done, but not reclaimed yet
};

class Thread {
private:
    strj g
};
#endif //MY_THREE_COLOR_THREAD_H
