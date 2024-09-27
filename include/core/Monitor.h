//
// Created by xiehao on 2024/9/27.
//

#ifndef MY_THREE_COLOR_MONITOR_H
#define MY_THREE_COLOR_MONITOR_H


#include "Thread.h"

class Monitor {
private:
    Thread*     _owner;
public:
    void wait(Thread* t);
    void notify(Thread* t);
    void notify_all(Thread* t);

};


#endif //MY_THREE_COLOR_MONITOR_H
