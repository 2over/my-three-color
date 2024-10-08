//
// Created by ziya on 2024/10/6.
//

#include "OopMap.h"
#include "../code/CompressedWriteStream.h"

OopMap::OopMap(Thread *t) {
    _thread = t;
    set_write_stream(new CompressedWriteStream(64));
}

void OopMap::set_xxx(long v) {
    write_stream()->write_long(v);
}