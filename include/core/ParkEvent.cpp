//
// Created by xiehao on 2024/9/29.
//

#include "ParkEvent.h"
#include "Thread.h"

void ParkEvent::park() {
    // 这里为什么枷锁? 因为条件变量wait必须先加锁
    pthread_mutex_lock(_lock);

    _state = 1;

    _owner->set_state(INITIALIZED);

    pthread_cond_wait(_cond, _lock);
    pthread_mutex_unlock(_lock);
}

void ParkEvent::unpark() {

    pthread_mutex_lock(_lock);
    _state = 0;
    pthread_cond_signal(_cond);

    pthread_mutex_unlock(_lock);
}

void ParkEvent::lock() {
    pthread_mutex_lock(_lock);
    _state = 1;
    pthread_cond_wait(_cond, _lock);
    pthread_mutex_unlock(_lock);
}

void ParkEvent::unlock() {
    pthread_mutex_lock(_lock);

    _state = 0;

    pthread_cond_signal(_cond);
    pthread_mutex_unlock(_lock);
}