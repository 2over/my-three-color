//
// Created by ziya on 2024/10/6.
//

#ifndef MY_THREE_COLOR_COMPRESSEDWRITESTREAM_H
#define MY_THREE_COLOR_COMPRESSEDWRITESTREAM_H

#include "CompressedStream.h"
#include "../../../include/common.h"

class CompressedWriteStream : public CompressedStream {
private:
    bool full() {
        return _position >= _size;
    }

    void store(long b) {
        _buffer[_position++] = b;
    }

    void write(long b) {
        if (full()) grow();
        store(b);
    }

    void grow();

protected:
    int _size;
public:
    CompressedWriteStream(int initial_size) : CompressedStream(NULL, 0) {
        _buffer = (long *) calloc(8, initial_size);
        _size = initial_size;
        _position = 0;
    }

    CompressedWriteStream(long *buffer, int initial_size, int position = 0) : CompressedStream(buffer, position) {
        _size = initial_size;
    }

    void write_long(long v) {
        write(v);
    }
};


#endif //MY_THREE_COLOR_COMPRESSEDWRITESTREAM_H
