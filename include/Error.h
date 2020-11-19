/* 在此头文件中，定义了函数返回状态错误码、以及异常处理（显示错误信息）
 * This header file includes error codes of function's return value
 * and macros for printing error messages.
 */

#ifndef ERROR_H
#define ERROR_H

#include <stdio.h>
#include <stdint.h>

#define ERROR_SIZE SIZE_MAX
#define NOT_FOUND (SIZE_MAX-1)

#define ERR_MSG_NULL_POINTER       "null pointer"
#define ERR_MSG_INDEX_OUT_OF_RANGE "index out of range"
#define ERR_MSG_OUT_OF_MEMORY      "out of memory"

#define PRINT_ERR_MSG(MSG_STR)                                                                      \
do {                                                                                                \
    fprintf(stderr, "\nError: %s\nat function %s(%s:%d)\n", MSG_STR, __func__, __FILE__, __LINE__); \
} while (0)

#endif      //Error.h
