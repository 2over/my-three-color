//
// Created by xiehao on 2024/10/8.
//

#include "G1SATBCardTableModRefBS.h"
#include "../../../../include/core/Threads.h"

void G1SATBCardTableModRefBS::enqueue(oop old_val) {
    Threads::satb_mark_queue().push(old_val);
}