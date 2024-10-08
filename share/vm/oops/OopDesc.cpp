//
// Created by xiehao on 2024/10/8.
//
#include "OopDesc.h"
#include "../memory/BarrierSet.h"
#include "../gc_implementation/g1/G1SATBCardTableModRefBS.h"

BarrierSet *OopDesc::_bs = new G1SATBCardTableModRefBS;

oop OopDesc::oop_store(int index, oop new_oop) {
    oop old_oop = field_oops()[index];

    // 模拟写屏障
    bs()->write_ref_field_pre(old_oop);

    field_oops()[index] = new_oop;

    return old_oop;
}

void OopDesc::oop_store(oop new_oop) {
    field_oops().push_back(new_oop);
}