//
// Created by xiehao on 2024/9/25.
//

#ifndef MY_THREE_COLOR_CONTAINER_H
#define MY_THREE_COLOR_CONTAINER_H

#include "../common.h"

class Container {
protected:
    u_char *_buffer;
    int _position;

public:
    Container(u_char *buffer, int position = 0) {
        _buffer = buffer;
        _position = position;
    }

    u_char *buffer() const { return _buffer; }

    // Positioning
    int position() const { return _position; }

    void set_position(int position) { _position = position; }

};

#endif //MY_THREE_COLOR_CONTAINER_H
