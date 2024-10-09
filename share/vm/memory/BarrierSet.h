//
// Created by xiehao on 2024/10/8.
//

#ifndef MY_THREE_COLOR_BARRIERSET_H
#define MY_THREE_COLOR_BARRIERSET_H

#include "../oops/OopDesc.h"

class BarrierSet {
public:
    void write_ref_field_pre(oop old_val) {
        write_ref_field_pre_work(old_val);
    }

protected:
    virtual void write_ref_field_pre_work(oop old_val) {

    }
};


#endif //MY_THREE_COLOR_BARRIERSET_H
