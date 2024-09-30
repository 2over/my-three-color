//
// Created by xiehao on 2024/9/30.
//

#ifndef MY_THREE_COLOR_STACK_H
#define MY_THREE_COLOR_STACK_H

#include <stdio.h>
#include <stdlib.h>
#include "../common.h"

class Stack {
private:
    long*   _data;
    int     _top;
    int     _maxSize;

public:
    Stack(int size) {
        _top = 0;
        _maxSize = size;

        _data = new long[size];
    }

    ~Stack() {
        delete _data;
    }

public:
    int size() {
        return _top;
    }

public:
    void push(long val) {
        if (_top > _maxSize -1) {
            printf("栈越界\n");

            exit(-1);
        }

        _data[_top++] = val;
    }

    void push(void* val) {
        if (_top > _maxSize - 1) {
            printf("栈越界\n");
            exit(-1);
        }

        _data[_top++] = (long)val;
    }

    void pop() {
        if (_top < 0) {
            printf("栈已经到底\n");

            exit(-1);
        }

        _top -= 1;
    }

    long popData() {
        if (_top < 0) {
            printf("栈已经到底\n");
            exit(-1);
        }

        long r = _data[--_top];
        return r;
    }

    long top() {
        return _data[_top - 1];
    }

    long* data_addr() {
        return _data;
    }

    long* data_current_addr() {
        return _data + _top;
    }

public:
    void print(const char *str) {
        printf("%s: top=%d, max=%d\n", str, _top, _maxSize);
    };
};


#endif //MY_THREE_COLOR_STACK_H
