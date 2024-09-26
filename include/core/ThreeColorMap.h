//
// Created by xiehao on 2024/9/24.
//

#ifndef MY_THREE_COLOR_THREECOLORMAP_H
#define MY_THREE_COLOR_THREECOLORMAP_H

#include "../common.h"
#include "../../share/vm/oops/OopDesc.h"

class Thread;

class ThreeColorMap {
private:
    queue<oop>  _white_table;
    queue<oop>  _gray_table;
    queue<oop>  _black_table;

public:
    queue<oop>& white_table() {
        return _white_table;
    }

    queue<oop>& gray_table() {
        return _gray_table;
    }

    queue<oop>& black_table() {
        return _black_table;
    }
};
#endif //MY_THREE_COLOR_THREECOLORMAP_H
