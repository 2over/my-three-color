//
// Created by xiehao on 2024/10/8.
//

#ifndef MY_THREE_COLOR_G1SATBCARDTABLEMODREFBS_H
#define MY_THREE_COLOR_G1SATBCARDTABLEMODREFBS_H

#include "../../memory/CardTableModRefBSForCTRS.h"

class G1SATBCardTableModRefBS : public CardTableModRefBSForCTRS {
public:
    virtual void write_ref_field_pre_work(oop old_val) {
        inline_write_ref_field_pre(old_val);
    }

    void inline_write_ref_field_pre(oop old_val) {
        write_ref_field_pre_static(old_val);
    }

    void write_ref_field_pre_static(oop old_val) {
        enqueue(old_val);
    }

    void enqueue(oop old_val);

};


#endif //MY_THREE_COLOR_G1SATBCARDTABLEMODREFBS_H
