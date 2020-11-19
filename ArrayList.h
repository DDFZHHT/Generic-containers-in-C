/* 静态顺序表 - ArrayList
 * 在此头文件中，给出了成员函数的声明，并且为struct array_list * 定义了别名 ArrayList。
 *
 * This header file includes declarations of member functions
 * and defines a name "ArrayList" for struct array_list *.
 */

#ifndef ARRAY_LIST_H
#define ARRAY_LIST_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "Error.h"

#define ARRAY_LIST_DEFAULT_CAPACITY (1 << 10)
#define ARRAY_LIST_DEFAULT_ELEM_SIZE sizeof(int)

typedef struct array_list* ArrayList;
typedef struct array_list_iter* ArrayListIter;

// 初始化一个新表
// Initializes a new ArrayList.
struct array_list* ArrayListCreate(size_t capacity, size_t elem_size);

// 释放表的空间并置为空指针
// Frees memory of list a and set a to NULL.
void ArrayListDelete(struct array_list **a);

size_t ArrayListGetElemSize(const struct array_list *a);

size_t ArrayListGetCapacity(const struct array_list *a);

size_t ArrayListGetLength(const struct array_list *a);

bool ArrayListIsEmpty(const struct array_list *a);

bool ArrayListIsFull(const struct array_list *a);

// 插入一个元素
// Inserts x into list a on the position pos.
bool ArrayListInsertElem(struct array_list *a, size_t pos, const void *x);

// 删除一个元素
// Removes a's element on the position pos.
bool ArrayListRemoveElem(struct array_list *a, size_t pos);

// 按位置取元素
// Gets an element on the position pos.
bool ArrayListGetElem(const struct array_list *a, size_t pos, void *x);

// 修改一个元素
// Modifies an element on the position pos.
bool ArrayListSetElem(const struct array_list *a, size_t pos, const void *x);

// 清空表中元素
// Clears all elements of list a.
bool ArrayListClear(struct array_list *a);

// 以x的值把表填满
// Fills list a with x's value.
bool ArrayListFill(struct array_list *a, const void *x);

// 按值查找位置（顺序查找）
// Finds an a's element which equals x with comp() and then returns its position.
size_t ArrayListFind(const struct array_list *a, const void *x,
                     int (*comp)(const void *, const void *));

// 排序（冒泡排序）
// Sorts list a by bubble sort with comp().
bool ArrayListSort(const struct array_list *a,
                   int (*comp)(const void *, const void *));

// 新初始化一个指定位置的ArrayList迭代器
// Creates a new iterator points to current_pos of list a.
struct array_list_iter* ArrayListIterCreate(const struct array_list *a,
                                            size_t pos);

// 销毁迭代器并置为空指针
// Destroys *it and sets *it to NULL.
void ArrayListIterDelete(struct array_list_iter **it);

// 新建一个ArrayList的first迭代器
// Creates an iterator points to the first iterator position.
struct array_list_iter* ArrayListIterFirst(const struct array_list *a);

// 新建一个ArrayList的last迭代器
// Creates an iterator points to the last iterator position.
struct array_list_iter* ArrayListIterLast(const struct array_list *a);

// 检查迭代器是否存在Next位置
// Checks if the next position of it exists.
bool ArrayListIterHasNext(const struct array_list_iter *it);

// 令迭代器移动到Next位置
// Moves the iterator to its next position.
bool ArrayListIterNext(struct array_list_iter *it);

// 获取Next位置的元素
// Gets the element at the next position of it.
bool ArrayListIterGetNext(const struct array_list_iter *it, void *x);

// 修改Next位置的元素
// Modifies the element at the next position of it.
bool ArrayListIterSetNext(const struct array_list_iter *it, const void *x);

// 检查迭代器是否存在Prev位置
// Checks if the prev position of it exists.
bool ArrayListIterHasPrev(const struct array_list_iter *it);

// 令迭代器移动到Prev位置
// Moves the iterator to its prev position.
bool ArrayListIterPrev(struct array_list_iter *it);

// 获取Prev位置的元素
// Gets the element at the prev position of it.
bool ArrayListIterGetPrev(const struct array_list_iter *it, void *x);

// 修改Prev位置的元素
// Modifies the element at the prev position of it.
bool ArrayListIterSetPrev(const struct array_list_iter *it, const void *x);

#ifdef __cplusplus
}
#endif

#endif      // ArrayList.h
