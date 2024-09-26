//
// Created by xiehao on 2024/9/26.
//

#ifndef MY_THREE_COLOR_KLASS_H
#define MY_THREE_COLOR_KLASS_H

class OopDesc;

class Klass {
public:
    OopDesc* allocate_instance();
    OopDesc* allocate_instance(char c);
};


#endif //MY_THREE_COLOR_KLASS_H
