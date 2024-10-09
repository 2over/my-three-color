//
// Created by ziya on 2024/10/6.
//

#include "CompressedWriteStream.h"

void CompressedWriteStream::grow() {
    long *old_buffer = _buffer;
    long *_new_buffer = (long *) calloc(8, _size * 2);

    memcpy(_new_buffer, _buffer, _position * 8);
    _buffer = _new_buffer;
    _size   = _size * 2;

    delete old_buffer;
}