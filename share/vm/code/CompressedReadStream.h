//
// Created by ziya on 2024/10/6.
//

#ifndef MY_THREE_COLOR_COMPRESSEDREADSTREAM_H
#define MY_THREE_COLOR_COMPRESSEDREADSTREAM_H

#include "CompressedStream.h"

class CompressedReadStream : public CompressedStream {
public:
    long read() { return _buffer[_position++]; }
    long read(int index) {
        return _buffer[index];
    }

public:
    CompressedReadStream(long* buffer, int position = 0):CompressedStream(buffer, position = 0) {

    }
};


#endif //MY_THREE_COLOR_COMPRESSEDREADSTREAM_H
