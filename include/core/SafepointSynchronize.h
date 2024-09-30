//
// Created by xiehao on 2024/9/30.
//

#ifndef MY_THREE_COLOR_SAFEPOINTSYNCHRONIZE_H
#define MY_THREE_COLOR_SAFEPOINTSYNCHRONIZE_H


#include "../common.h"

class SafepointSynchronize{
public:
    enum SynchronizeState {
        _not_synchronized   = 0,            // Threads not synchronized at a safepoint
        // Keep this value 0. See the comment in do_call_back()
        _synchronizing      = 1,            // Synchronizing in progress
        _synchronized       = 2             // All Java threads are stopped at a safepoint. Only VM thread is running

    };

    static volatile int _waiting_to_block;
    static void* _polling_page;
private:
    static volatile SynchronizeState _state;    // Threads might read this flag directly,without acquireing the Threads_lock
public:
    inline static bool is_at_safepoint() {
        return _state == _synchronized;
    }

    inline static bool is_synchronizing() {
        return _state == _synchronizing;
    }

public:
    static void init();

    static void begin();
    static void end();
    static void resume_all_thread();

public:
    static void insert_safepoint() {
        // hotspot 源码: LIR_Assembler::return_op
        // test %eax, os::_poling_page
        *(char*)_polling_page = 0;
    }

    static void make_polling_page_unreadable(void) {
        if (mprotect((char *)_polling_page, 4, PROT_NONE) != 0) {
            ERROR_PRINT("Could not disable polling page\n");
        }
    };

    static void make_polling_page_readable(void) {
        if (mprotect((char*) _polling_page, 4, PROT_READ | PROT_WRITE) != 0) {
            ERROR_PRINT("Could not enable polling page\n");
        }
    }
};


#endif //MY_THREE_COLOR_SAFEPOINTSYNCHRONIZE_H
