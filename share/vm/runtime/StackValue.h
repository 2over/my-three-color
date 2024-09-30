//
// Created by xiehao on 2024/9/30.
//

#ifndef MY_THREE_COLOR_STACKVALUE_H
#define MY_THREE_COLOR_STACKVALUE_H

#include "../utilities/BasicType.h"

class StackValue {
private:
    BasicType   _type;
    void*       _val;

public:
    StackValue(BasicType type, int v) {
        _type = type;
        _val = reinterpret_cast<void*>(v);
    }

    StackValue(BasicType type, long v) {
        _type = type;
        _val = reinterpret_cast<void*>(v);
    }
    StackValue(BasicType type, void* v) {
        _type = type;
        _val = v;
    }

public:
    BasicType type() {
        return _type;
    }

    long get_long() {
        return reinterpret_cast<long>(_val);
    }

    void* get_ptr() {
        return _val;
    }
};


#endif //MY_THREE_COLOR_STACKVALUE_H
