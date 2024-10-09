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
    string          _name;
    ThreadState     _state;
    bool            _interrupted;
    Stack*          _stack;

private:
    pthread_t       _tid[1];
    thread_fun      _entry_point;
    void*           _args;

private:
    queue<oop>      _satb_mark_queue; // g1写屏障记录的是oop
    queue<void*>    _dirty_card_queue; // cms写屏障记录的是dirty page
public:
    queue<oop>& satb_mark_queue() {
        return _satb_mark_queue;
    }

    Stack* stack() {
        return _stack;
    }

    void set_interrupted(bool v) {
        _interrupted = v;
    }

    bool interrupted() {
        return _interrupted;
    }

    void set_state(ThreadState state) {
        _state = state;
    }

    ThreadState get_state() {
        return _state;
    }

    Thread(thread_fun entry_point, void* args, char* name);

    Thread(thread_fun entry_point, void* args, int thread_index);

    ~Thread();

public:
    ParkEvent* _ParkEvent; // for synchronized()

    ParkEvent* _MutexEvent; // for native internal Mutex/Monitor

private:
    Monitor* _startThread_lock; // sync parent and child in thread creation

public:
    Monitor* startThread_lock() {
        return _startThread_lock;
    }

public:
    pthread_t tid() {
        return *_tid;
    }

    thread_fun entry_point() {
        return _entry_point;
    }

    string name() {
        return _name;
    }

public:
    void run();

    static void SpinAcquire(volatile int* addr, Thread* self);
    static void SpinRelease(volatile int *addr, Thread* self);


};
#endif //MY_THREE_COLOR_THREAD_H
