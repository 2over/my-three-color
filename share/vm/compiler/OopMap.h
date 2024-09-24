//
// Created by xiehao on 2024/9/24.
//

#ifndef MY_THREE_COLOR_OOPMAP_H
#define MY_THREE_COLOR_OOPMAP_H

#include "../memory/ResourceObj.h"
#include "../utilities/BasicType.h"

class CompressedWriteStream;
class Thread;

class OopMap {
private:
    CompressedWriteStream* _write_stream;

    // 用于表示这个OopMap属于哪个线程栈的，为了打印日志用
    Thread* _thread;

public:
    CompressedWriteStream* write_stream() const { return _write_stream; }
    void set_write_stream(CompressedWriteStream* value) { _write_stream = value; }

public:
    OopMap(Thread* t);

public:
    void set_xxx(long v);

    Thread* thread() {
        return _thread;
    }

};
#endif //MY_THREE_COLOR_OOPMAP_H
