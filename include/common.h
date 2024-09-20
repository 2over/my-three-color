#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

using namespace std;

typedef void *pvoid;
typedef unsigned char byte;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long ulong;

typedef union {
    long l_dummy;
    double d_dummy;
    void *p_dummy;
} Align;

#define  ALIGN_SIZE     (sizeof(Align))

typedef enum {
    GC_MARK_CLEAN,
    GC_MARK_COLLECT,
    GC_MARK_COPY,
    GC_G1,
} GC_TYPE;

#define DEFAULT_GC_TYPE GC_MARK_CLEAN
/**
 * 自定义打印输出
 */

#define INFO_OUTPUT         3
#define WARNING_OUTPUT      2
#define DEBUG_OUTPUT        1
#define ERROR_OUTPUT        0

#define DEBUG
#define DEBUG_LEVEL         INFO_OUTPUT

#define PRINT(info, ...) do { \
    printf("[Info] (%s:%d->%s):" info"", __FILE__,__LINE__,__FUNCTION__,##__VA_ARGS__); \
}while(0)

#define INFO_PRINT(info, ...) do { \
    if (DEBUG_LEVEL >= INFO_OUTPUT) { \
        printf("[Info] (%s:%d->%s):" info"", __FILE__,__LINE__,__FUNCTION__,##__VA_ARGS__); \
    } \
}while(0)

#define WARNING_PRINT(info, ...) do { \
    if (DEBUG_LEVEL >= WARNING_OUTPUT) { \
        printf("[Warning] (%s:%d->%s):" info"", __FILE__,__LINE__,__FUNCTION__, ##__VA_ARGS__); \
    } \
}while(0)

#define DEBUG_PRINT(info, ...) do { \
    if (DEBUG_LEVEL >= DEBUG_OUTPUT) { \
        printf("[Debug] (%s:%d->%s):" info"", __FILE__,__FUNCITON__, __LINE__, ##__VA_ARGS__);  \
    }\
}while(0)

#define ERROR_PRINT(info, ...) do \
{                                 \
    if (DEBUG_LEVEL >=  ERROR_OUTPUT) { \
        printf("[Error] %s:%d->%s):" info"",__FILE__,__LINE__,__FUNCTION__,##__VA_ARGS__);} \
}while(0)

#define NULL_POINTER(ptr) do { \
    if (NULL == ptr) {         \
        printf("[Error] (%s:%d->%s):空指针\n", __FILE__, __LINE__, __FUNCTION__)                           \
    } \
}while(0)




