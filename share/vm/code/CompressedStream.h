//
// Created by ziya on 2024/10/6.
//

#ifndef MY_THREE_COLOR_COMPRESSEDSTREAM_H
#define MY_THREE_COLOR_COMPRESSEDSTREAM_H


class CompressedStream {
protected:
    long*   _buffer;
    int     _position;

public:
    CompressedStream(long* buffer, int position = 0) {
        _buffer     = buffer;
        _position   = position;
    }

    long* buffer() const        {   return _buffer;  }

    // Positioning
    int  position() const               { return _position; }
    void set_position(int position)     { _position = position; }
};


#endif //MY_THREE_COLOR_COMPRESSEDSTREAM_H
