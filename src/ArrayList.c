/* 静态顺序表 - ArrayList
 * 在源文件中定义了struct array_list 结构体，使得结构体成员对外隐藏起来，无法被访问。
 * 此外也包括成员函数的实现。
 *
 * In this source file we defined struct array
 * so that the members of struct array_list are hidden to outside
 * and then other files can't change them.
 * Also, there are the implementations of functions in this file.
 */

#include "ArrayList.h"

struct array_list {
    void **data;        // 数据域       pointer to data
    size_t elem_size;   // 元素大小     size of single element
    size_t capacity;    // 最大容量     max capacity
    size_t length;      // 当前元素个数 current num of elements
};

struct array_list_iter {
    void **ptr_to_elem;             // 指向当前元素   pointer to current element
    struct array_list *ptr_to_list; // 记录所对应的表 pointer to the array list
};

// 初始化一个新表
struct array_list* ArrayListCreate(size_t capacity, size_t elem_size) {
    struct array_list *a = (struct array_list *)malloc(sizeof(struct array_list));
    if (NULL == a)          // 空间分配失败
        goto ALLOC_FAILED;  // memory alloc failed
    a->data = (void **)malloc(capacity * sizeof(void *));
    if (NULL == a->data)    // 空间分配失败
        goto ALLOC_FAILED;  // memory alloc failed
    a->elem_size = elem_size > 0 ? elem_size : ARRAY_LIST_DEFAULT_ELEM_SIZE;// 避免容量和 elem_size 为 0 的情况
    a->capacity = capacity > 0 ? capacity : ARRAY_LIST_DEFAULT_CAPACITY;    // capacity or elem_size == zero is not allowed
    a->length = 0;
    return a;

    ALLOC_FAILED:   // 防止内存泄漏
    free(a);        // to avoid memory leak
    PRINT_ERR_MSG(ERR_MSG_OUT_OF_MEMORY);
    return NULL;
}

// 释放表的空间并置为空指针
void ArrayListDelete(struct array_list **a) {   // 为了在 free() 后把 a 置为NULL，传参为二级指针，即对指针 a 取地址
    if (NULL != *a) {                           // to set pointer a = NULL after free(), parameter is **a
        void **p, **p_end = (*a)->data + (*a)->length;
        for (p = (*a)->data; p < p_end; p++)
            free(*p);
        free((*a)->data);
    }
    free(*a);
    *a = NULL;
}

size_t ArrayListGetElemSize(const struct array_list *a) {
    if (NULL == a) {
        PRINT_ERR_MSG(ERR_MSG_NULL_POINTER);
        return ERROR_SIZE;
    }
    return a->elem_size;
}

size_t ArrayListGetCapacity(const struct array_list *a) {
    if (NULL == a) {
        PRINT_ERR_MSG(ERR_MSG_NULL_POINTER);
        return ERROR_SIZE;
    }
    return a->capacity;
}

size_t ArrayListGetLength(const struct array_list *a) {
    if (NULL == a) {
        PRINT_ERR_MSG(ERR_MSG_NULL_POINTER);
        return ERROR_SIZE;
    }
    return a->length;
}

bool ArrayListIsEmpty(const struct array_list *a) {
    if (NULL == a) {
        PRINT_ERR_MSG(ERR_MSG_NULL_POINTER);
        return false;
    }
    return a->length == 0;
}

bool ArrayListIsFull(const struct array_list *a) {
    if (NULL == a) {
        PRINT_ERR_MSG(ERR_MSG_NULL_POINTER);
        return false;
    }
    return a->length >= a->capacity;
}

// 插入一个元素
bool ArrayListInsertElem(struct array_list *a, size_t pos, const void *x) {
    if (NULL == a || NULL == x) {
        PRINT_ERR_MSG(ERR_MSG_NULL_POINTER);
        return false;
    } else if (pos > a->length || ArrayListIsFull(a)) { // 表满或位置错误时，不能插入
        PRINT_ERR_MSG(ERR_MSG_INDEX_OUT_OF_RANGE);      // can't insert when array is full or position is wrong
        return false;                                   // 可插入的位置：0~length，共 (length+1)个
    }                                                   // from 0 to length, there are (length+1) positions can insert
    void *tmp = malloc(a->elem_size);
    if (NULL == tmp) {
        PRINT_ERR_MSG(ERR_MSG_OUT_OF_MEMORY);
        return false;
    }                               // 把后半部分元素向后移一个位置
    memcpy(tmp, x, a->elem_size);   // move the latter half part of array backward one position
    memmove(a->data + pos + 1, a->data + pos, (a->length - pos) * sizeof(void *));
    a->data[pos] = tmp;             // 再把 x 写入空出来的位置
    a->length++;                    // and then write x to the empty position
    return true;
}

// 删除一个元素
bool ArrayListRemoveElem(struct array_list *a, size_t pos) {
    if (NULL == a) {
        PRINT_ERR_MSG(ERR_MSG_NULL_POINTER);
        return false;
    } else if (pos >= a->length) {                  // 可删除的位置：0~(length-1)，共 length 个
        PRINT_ERR_MSG(ERR_MSG_INDEX_OUT_OF_RANGE);  // from 0 to (length-1), there are length positions can remove
        return false;
    }
    free(a->data[pos]);
    memmove(a->data + pos, a->data + pos + 1, (a->length - pos) * sizeof(void *));  // 把后半部分元素向前移一个位置
    a->length--;                                        // move the latter half part of array forward one position
    return true;
}

// 按位置取元素
bool ArrayListGetElem(const struct array_list *a, size_t pos, void *x) {
    if (NULL == a || NULL == x) {
        PRINT_ERR_MSG(ERR_MSG_NULL_POINTER);
        return false;
    } else if (pos >= a->length) {
        PRINT_ERR_MSG(ERR_MSG_INDEX_OUT_OF_RANGE);
        return false;
    }
    memcpy(x, a->data[pos], a->elem_size);
    return true;
}

// 修改一个元素
bool ArrayListSetElem(const struct array_list *a, size_t pos, const void *x) {
    if (NULL == a || NULL == x) {
        PRINT_ERR_MSG(ERR_MSG_NULL_POINTER);
        return false;
    } else if (pos >= a->length) {
        PRINT_ERR_MSG(ERR_MSG_INDEX_OUT_OF_RANGE);
        return false;
    }
    memcpy(a->data[pos], x, a->elem_size);
    return true;
}

// 清空表中元素
bool ArrayListClear(struct array_list *a) {
    if (NULL == a) {
        PRINT_ERR_MSG(ERR_MSG_NULL_POINTER);
        return false;
    }
    void **p, **p_end = a->data + a->length;
    for (p = a->data; p < p_end; p++)
        free(*p);
    a->length = 0;
    return true;
}

// 以x值填满表中元素
bool ArrayListFill(struct array_list *a, const void *x) {
    if (NULL == a || NULL == x) {
        PRINT_ERR_MSG(ERR_MSG_NULL_POINTER);
        return false;
    }
    void **p, **p_end = a->data + a->length;// 从0到length-1，修改元素的值
    for (p = a->data; p < p_end; p++)       // from 0 to (length-1)，change the value of elements
        memcpy(*p, x, a->elem_size);
    for (p_end = a->data + a->capacity; p < p_end; p++) {
        *p = malloc(a->elem_size);          // 从length到capacity-1，新分配空间插入元素
        if (NULL == *p) {                   // from length to (capacity-1), alloc new space and insert elements
            PRINT_ERR_MSG(ERR_MSG_OUT_OF_MEMORY);
            return false;
        }
        memcpy(*p, x, a->elem_size);
        a->length++;
    }
    return true;
}

// 按值查找位置（顺序查找）
size_t ArrayListFind(const struct array_list *a, const void *x,
                     int (*comp)(const void *, const void *)) {
    if (NULL == a || NULL == x || NULL == comp) {
        PRINT_ERR_MSG(ERR_MSG_NULL_POINTER);
        return ERROR_SIZE;
    }
    void **p, **p_end = a->data + a->length;
    for (p = a->data; p < p_end; p++) {
        if (0 == comp(x, *p))
            return p - (a->data);
    }
    return NOT_FOUND;
}

// 排序（冒泡排序）
bool ArrayListSort(const struct array_list *a,
                   int (*comp)(const void *, const void *)) {
    if (NULL == a || NULL == comp) {
        PRINT_ERR_MSG(ERR_MSG_NULL_POINTER);
        return false;
    }
    size_t i, j;
    void *tmp;
    for (i = 0; i < a->length; i++) {
        for (j = 0; j < a->length - i - 1; j++) {       // 升序排序
            if (comp(a->data[j], a->data[j+1]) > 0) {   // elem[j] > elem[j+1]
                tmp = a->data[j];
                a->data[j] = a->data[j+1];
                a->data[j+1] = tmp;
            }
        }
    }
    return true;
}

// 新初始化一个指定位置的ArrayList迭代器
struct array_list_iter* ArrayListIterCreate(const struct array_list *a,
                                            size_t pos) {
    if (NULL == a) {
        PRINT_ERR_MSG(ERR_MSG_NULL_POINTER);
        return NULL;
    } else if (pos > a->length) {
        PRINT_ERR_MSG(ERR_MSG_INDEX_OUT_OF_RANGE);
        return NULL;        // 合法范围：0~length，共(length+1)个位置
    }
    struct array_list_iter* it = (struct array_list_iter *)
                                 malloc(sizeof (struct array_list_iter));
    if (NULL == it) {
        PRINT_ERR_MSG(ERR_MSG_OUT_OF_MEMORY);
        return NULL;
    }
    it->ptr_to_elem = a->data + pos;
    it->ptr_to_list = (struct array_list *)a;
    return it;
}

// 销毁迭代器并置为空指针
void ArrayListIterDelete(struct array_list_iter **it) {
    free(*it);
    *it = NULL;
}

// 新建一个ArrayList的first迭代器
struct array_list_iter* ArrayListIterFirst(const struct array_list *a) {
    if (NULL == a) {
        PRINT_ERR_MSG(ERR_MSG_NULL_POINTER);
        return NULL;
    }
    return ArrayListIterCreate(a, 0);
}

// 新建一个ArrayList的last迭代器
struct array_list_iter* ArrayListIterLast(const struct array_list *a) {
    if (NULL == a) {
        PRINT_ERR_MSG(ERR_MSG_NULL_POINTER);
        return NULL;
    }
    return ArrayListIterCreate(a, a->length);
}

// 检查迭代器是否存在Next位置
bool ArrayListIterHasNext(const struct array_list_iter *it) {
    if (NULL == it) {
        PRINT_ERR_MSG(ERR_MSG_NULL_POINTER);
        return false;
    }
    return it->ptr_to_elem < it->ptr_to_list->data + it->ptr_to_list->length
        && it->ptr_to_elem >= it->ptr_to_list->data;
}

// 令迭代器移动到Next位置
bool ArrayListIterNext(struct array_list_iter *it) {
    if (NULL == it) {
        PRINT_ERR_MSG(ERR_MSG_NULL_POINTER);
        return false;
    } else if (!ArrayListIterHasNext(it)) {
        PRINT_ERR_MSG(ERR_MSG_INDEX_OUT_OF_RANGE);
        return false;
    }
    it->ptr_to_elem++;
    return true;
}

// 获取Next位置的元素
bool ArrayListIterGetNext(const struct array_list_iter *it, void *x) {
    if (NULL == x || NULL == it) {
        PRINT_ERR_MSG(ERR_MSG_NULL_POINTER);
        return false;
    } else if (!ArrayListIterHasNext(it)) {
        PRINT_ERR_MSG(ERR_MSG_INDEX_OUT_OF_RANGE);
        return false;
    }
    memcpy(x, *(it->ptr_to_elem), it->ptr_to_list->elem_size);
    return true;
}

// 修改Next位置的元素
bool ArrayListIterSetNext(const struct array_list_iter *it, const void *x) {
    if (NULL == x || NULL == it) {
        PRINT_ERR_MSG(ERR_MSG_NULL_POINTER);
        return false;
    } else if (!ArrayListIterHasNext(it)) {
        PRINT_ERR_MSG(ERR_MSG_INDEX_OUT_OF_RANGE);
        return false;
    }
    memcpy(*(it->ptr_to_elem), x, it->ptr_to_list->elem_size);
    return true;
}

// 检查迭代器是否存在Prev位置
bool ArrayListIterHasPrev(const struct array_list_iter *it) {
    if (NULL == it) {
        PRINT_ERR_MSG(ERR_MSG_NULL_POINTER);
        return false;
    }
    return it->ptr_to_elem > it->ptr_to_list->data
        && it->ptr_to_elem <= it->ptr_to_list->data + it->ptr_to_list->length;
}

// 令迭代器移动到Prev位置
bool ArrayListIterPrev(struct array_list_iter *it) {
    if (NULL == it) {
        PRINT_ERR_MSG(ERR_MSG_NULL_POINTER);
        return false;
    } else if (!ArrayListIterHasPrev(it)) {
        PRINT_ERR_MSG(ERR_MSG_INDEX_OUT_OF_RANGE);
        return false;
    }
    it->ptr_to_elem--;
    return true;
}

// 获取Prev位置的元素
bool ArrayListIterGetPrev(const struct array_list_iter *it, void *x) {
    if (NULL == x || NULL == it) {
        PRINT_ERR_MSG(ERR_MSG_NULL_POINTER);
        return false;
    } else if (!ArrayListIterHasPrev(it)) {
        PRINT_ERR_MSG(ERR_MSG_INDEX_OUT_OF_RANGE);
        return false;
    }
    memcpy(x, *(it->ptr_to_elem - 1), it->ptr_to_list->elem_size);
    return true;
}

// 修改Prev位置的元素
bool ArrayListIterSetPrev(const struct array_list_iter *it, const void *x) {
    if (NULL == x || NULL == it) {
        PRINT_ERR_MSG(ERR_MSG_NULL_POINTER);
        return false;
    } else if (!ArrayListIterHasPrev(it)) {
        PRINT_ERR_MSG(ERR_MSG_INDEX_OUT_OF_RANGE);
        return false;
    }
    memcpy(*(it->ptr_to_elem - 1), x, it->ptr_to_list->elem_size);
    return true;
}
