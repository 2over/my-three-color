//
// Created by xiehao on 2024/9/20.
//

#ifndef MY_THREE_COLOR_OOPDESC_H
#define MY_THREE_COLOR_OOPDESC_H

#include "../../../include/common.h"
#include "../../../include/container/Array.h"

class MemoryCell;

class BarrierSet;

typedef class OopDesc *oop;

class OopDesc {
private:
    void*           _mark;
    void*           _klass_point;
    char            _gc_mark;
    MemoryCell*     _mem_cell;

    // 用于标识这是哪个对象
    char            _sign;
    // 用来存储所有oop属性
    vector<oop>     _field_oops;

    // Fast access to barrier set. Must be initialized
    static BarrierSet* _bs;

public:
    oop oop_store(int index, oop new_oop);
    void oop_store(oop new_oop);



};


#endif //MY_THREE_COLOR_OOPDESC_H
