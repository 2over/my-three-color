//
// Created by xiehao on 2024/9/20.
//


#include "../../include/gc/ConcurrentMark.h"
#include "../../include/common.h"
#include "../../include/memory/memory_cell.h"
#include "../../share/vm/compiler/OopMapSet.h"
#include "../../include/core/ThreeColorMap.h"
#include "../../share/vm/compiler/OopMapSet.h"
#include "../../share/vm/oops/Klass.h"
#include "../../include/core/Threads.h"


extern OopMapSet oopMapSet;

extern ThreeColorMap threeColorMap;

extern oop e;

static void mark_child(oop parent) {
    INFO_PRINT("[vm thread] 开始处理 对象%c 属性, size =%d\n", parent->sign(), parent->field_oops().capacity());

    if (0 == parent->field_oops().capacity()) {
        INFO_PRINT("\t [vm thread] 对象%c 没有属性, 标记为黑色\n", parent->sign());

        parent->set_gc_mark(true);
        parent->mem_cell()->set_mark(true);
        threeColorMap.black_table().push(parent);

        return;
    }

    for (int i = 0; i < parent->field_oops().capacity(); i++) {
        oop o = parent->field_oops().at(i);

        // 模拟少标
        if ('F' == o->sign()) {
            INFO_PRINT("\n\n [vm thread] 模拟少标:断开D->F,建立E->F(F不加入到灰色队列)\n\n");

            // D(灰色)断开了对F(白色)的引用
            // 添加一层写屏障
            parent->oop_store(0, (new Klass)-> allocate_instance('L'));

            // E(黑色)建立了对F(白色)的引用，F是E的第二个属性
            e->field_oops().push_back(o);

            continue;
        }

        INFO_PRINT("\t [vm thread] 属性: %c->%c, 标记为灰色\n", parent->sign(), o->sign());

        threeColorMap.gray_table().push(o);
    }

    // 标记为和死
    INFO_PRINT("\t [vm thread] 对象%c 属性遍历完了，标记为黑色\n", parent->sign());

    parent->set_gc_mark(true);
    parent->mem_cell()->set_mark(true);
    threeColorMap.black_table().push(parent);
}

void ConcurrentMark::mark() {
    INFO_PRINT("[vm thread] 开始并发标记 size=%d\n", threeColorMap.gray_table().size());

    if (0 == threeColorMap.gray_table().size()) {
        INFO_PRINT("[vm thread]完成并发标记 size =%d\n", threeColorMap.gray_table().size());
        return ;
    }

    // 为了后面的正常运行，一轮扫描完成需要清空gray table，不清空会重复扫描
    for (int i = threeColorMap.gray_table().size() - 1; i >= 0; i--) {
        oop o = threeColorMap.gray_table().front();

        // 扫过的oop要删除
        threeColorMap.gray_table().pop();

        mark_child(o);

    }

    // 上一轮扫描结束需要晴空gray table ，否则会重复扫描
    mark();
}

void ConcurrentMark::final_mark() {
    INFO_PRINT("%d\n", Threads::satb_mark_queue().size());

    for (int i = Threads::satb_mark_queue().size() - 1; i >= 0; i--) {
        oop o = Threads::satb_mark_queue().front();

        // 扫描过的oop要删除
        Threads::satb_mark_queue().pop();

        INFO_PRINT("%d\n", Threads::satb_mark_queue().size());
        if (0 == o->field_oops().capacity()) {
            INFO_PRINT("\t [vm thread] 对象%c 没有属性，标记为黑色\n", o->sign());

            o->set_gc_mark(true);
            o->mem_cell()->set_mark(true);
            threeColorMap.black_table().push(o);

            break;
        }
    }
}