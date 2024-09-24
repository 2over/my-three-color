//
// Created by xiehao on 2024/9/20.
//


#include "../../include/gc/ConcurrentMark.h"
#include "../../include/common.h"
#include "../../include/memory/memory_cell.h"
#include "../../share/vm/compiler/OopMapSet.h"

extern OopMapSet oopMapSet;

extern ThreeColorMap threeColorMap;

extern oop e;

static void mark_child(oop parent) {
    INFO_PRINT("[vm thread] 开始处理 对象%c 属性, size =%d\n", parent->sign(), parent->field_oops().capacity());
}