//
// Created by xiehao on 2024/9/25.
//

#ifndef MY_THREE_COLOR_ARRAY_H
#define MY_THREE_COLOR_ARRAY_H

#include "../common.h"
#include "Container.h"

class Array : public Container {
private:
    bool full(int step = 0) {
        return (_position + step) >= _size;
    }

    void grow() {
        u_char *old_buffer = _buffer;
        u_char *_new_buffer = (u_char *) calloc(1, _size * 2);
        memcpy(_new_buffer, _buffer, _position);
        _buffer = _new_buffer;
        _size = _size * 2;

        delete old_buffer;
    }

protected:
    int _size;

public:
    Array(int initial_size) : Container(NULL, 0) {
        _buffer = (u_char *) calloc(1, initial_size);
        _size = initial_size;
        _position = 0;
    }

    Array(u_char *buffer, int initial_size, int position = 0) : Container(buffer, position) {
        _size = initial_size;
    }

    void write_long(jlong value);

    void write_ptr(void *value);

    void change_long(int index, long value);

    void reset(int step);

    jlong read_long(juint);
};

#endif //MY_THREE_COLOR_ARRAY_H
