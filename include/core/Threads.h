//
// Created by xiehao on 2024/9/27.
//

#ifndef MY_THREE_COLOR_THREADS_H
#define MY_THREE_COLOR_THREADS_H

#include "../common.h"
#include "../../share/vm/oops/OopDesc.h"

class Thread;

class Threads {

private:
    static queue<oop>       _satb_mark_queue; // g1写屏障记录的是oop
    static queue<void*>     _dirty_card_queue; // cms写屏障记录的是dirty page

private:
    static map<pthread_t, Thread*> _threads_map;
public:
    static queue<oop>& satb_mark_queue() {
        return _satb_mark_queue;
    }

    static map<pthread_t, Thread*>* threads_map() {
        return &_threads_map;
    }

    static map<pthread_t, Thread*> threads() {
        return _threads_map;
    }

    static void add_current_thread(pthread_t t, Thread* thread);

    static void init_2();

    static void build_oop_maps();

    static Thread* current_thread() {
        return threads()[pthread_self()];
    }


};
#endif //MY_THREE_COLOR_THREADS_H
