#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <memory.h>
#include <sys/mman.h>
#include <signal.h>

#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <list>
#include <map>
#include <sstream>
#include <queue>

using namespace std;

typedef void * pvoid;
typedef unsigned char byte;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long ulong;

typedef union {
    long l_dummy;
    double d_dummy;
    void * p_dummy;
}Align;
#define ALIGN_SIZE (sizeof(Align))
#define MinOopMapAllocation 0x50

typedef enum {
    GC_MARK_CLEAN,
    GC_MARK_COLLECT,
    GC_MARK_COPY,
    GC_G1,
}GC_Type;

#define UseParallelGC false
#define UseG1GC false
#define UseSerialGC false
#define UseConcMarkSweepGC false
#define UseAdaptiveSizePolicy false

#define DEFAULT_GC_TYPE GC_MARK_CLEAN
typedef void* (*thread_fun)(void *);
// JavaThreadState keeps track of which part of the code a thread is executing in. This
// information is needed by the safepoint code.
//
// There are 4 essential states:
//
//  _thread_new         : Just started, but not executed init. code yet (most likely still in OS init code)
//  _thread_in_native   : In native code. This is a safepoint region, since all oops will be in jobject handles
//  _thread_in_vm       : Executing in the vm
//  _thread_in_Java     : Executing either interpreted or compiled Java code (or could be in a stub)
//
// Each state has an associated xxxx_trans state, which is an intermediate state used when a thread is in
// a transition from one state to another. These extra states makes it possible for the safepoint code to
// handle certain thread_states without having to suspend the thread - making the safepoint code faster.
//
// Given a state, the xxx_trans state can always be found by adding 1.
//
enum JavaThreadState {
    _thread_uninitialized     =  0, // should never happen (missing initialization)
    _thread_new               =  2, // just starting up, i.e., in process of being initialized
    _thread_new_trans         =  3, // corresponding transition state (not used, included for completness)
    _thread_in_native         =  4, // running in native code
    _thread_in_native_trans   =  5, // corresponding transition state
    _thread_in_vm             =  6, // running in VM
    _thread_in_vm_trans       =  7, // corresponding transition state
    _thread_in_Java           =  8, // running in Java or in stub code
    _thread_in_Java_trans     =  9, // corresponding transition state (not used, included for completness)
    _thread_blocked           = 10, // blocked in vm
    _thread_blocked_trans     = 11, // corresponding transition state
    _thread_max_state         = 12  // maximum thread state+1 - used for statistics allocation
};

typedef unsigned char      jubyte;
typedef unsigned short     jushort;
typedef unsigned int       juint;
typedef int                jint;
typedef unsigned long long julong;
typedef long               jlong;
typedef unsigned char      jboolean;
typedef unsigned short     jchar;
typedef short              jshort;
typedef float              jfloat;
typedef double             jdouble;
typedef signed char        jbyte;

inline int low (long value)      { return int(value); }
inline int high(long value)      { return int(value >> 32);}

// the fancy casts are a hopefully portable way
// to do unsigned 32 to 64 bit type conversion
inline void set_low(jlong* value, jint low) {
    *value &= (jlong)0xffffffff << 32;
    *value |= (jlong)(julong)(juint)low;
}

inline void set_high(jlong* value, jint high) {
    *value &= (jlong)(julong)(juint)0xffffffff;
    *value |= (jlong)high << 32;
}

inline jlong jlong_from(jint h, jint l) {
    jlong result = 0; // initialization to avoid warning
    set_high(&result, h);
    set_low(&result, l);
    return result;
}

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
        printf("[Info] (%s:%d->%s):" info"",__FILE__,__LINE__,__FUNCITON__,##__VA_ARGS__); \
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

#define ERROR_PRINT(info, ...) do { if (DEBUG_LEVEL >=  ERROR_OUTPUT) { printf("[Error] %s:%d->%s):" info"",__FILE__,__LINE,__FUNCTION__,##__VA_ARGS__);}}while(0)
#define assert(cond, msg) { if (!(cond)) { fprintf(stderr, "assert fails %s %d:%s\n", __FILE__, __LINE__,msg); abort();}}

#define WRITE(str) do { \
    write(STDOUT_FILENO, str, strlen(str)); \
}while(0)

#define ASSERT_ZERO(status, str) do { \
    if (0 != status) {                \
        WRITE(str);                   \
        exit(-1);                                  \
    }\
}while(0)




