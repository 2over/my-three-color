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

public:
    vector<oop>& field_oops() {
        return _field_oops;
    }

    // Fast access to barrier set
    static BarrierSet* bs() {
        return _bs;
    }

    static void set_bs(BarrierSet* bs) {
        _bs = bs;
    }

    char gc_mark() {
        return _gc_mark;
    }

    void set_gc_mark(char v) {
        _gc_mark = v;
    }

    void set_sign(char c) {
        _sign = c;
    }

    char sign() {
        return _sign;
    }

    MemoryCell* mem_cell() {
        return _mem_cell;
    }

    void set_mem_cell(MemoryCell* cell) {
        _mem_cell = cell;
    }

};


#endif //MY_THREE_COLOR_OOPDESC_H
