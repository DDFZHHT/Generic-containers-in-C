#include <stdio.h>
#include <ctype.h>

#include "ArrayList.h"

#define COMMANDS_SIZE 128

void AddCommands(void (*commands[])(struct array_list **a));

void ShowHelp(struct array_list **dummy);
void Quit(struct array_list **dummy);

void Create(struct array_list **a);
void Destroy(struct array_list **a);
void ShowStatus(struct array_list **a);

void Clear(struct array_list **a);
void Fill(struct array_list **a);

void InsertElem(struct array_list **a);
void RemoveElem(struct array_list **a);
void GetElem(struct array_list **a);
void SetElem(struct array_list **a);

void FindElem(struct array_list **a);
void ReplaceElem(struct array_list **a);

void SortAscending(struct array_list **a);
void Traverse(struct array_list **a);
void TraverseBackward(struct array_list **a);

void VisInt(const void *x);
int CmpInt(const void *a, const void *b);

int main(void) {
    ArrayList list_int_ = NULL;
    void (*commands[COMMANDS_SIZE])(struct array_list **a);

    AddCommands(commands);
    ShowHelp(NULL);
    for (; ; ) {
        unsigned char op = toupper(getchar());
        if (commands[op] != NULL)
            commands[op](&list_int_);
    }
    return 0;
}

void AddCommands(void (*commands[])(struct array_list **a)) {
    size_t i;
    for (i = 0; i < COMMANDS_SIZE; i++)
        commands[i] = NULL;
    commands['H'] = ShowHelp;
    commands['Q'] = Quit;

    commands['N'] = Create;
    commands['D'] = Destroy;
    commands['L'] = ShowStatus;

    commands['C'] = Clear;
    commands['F'] = Fill;

    commands['I'] = InsertElem;
    commands['R'] = RemoveElem;
    commands['G'] = GetElem;
    commands['M'] = SetElem;

    commands['S'] = FindElem;
    commands['P'] = ReplaceElem;

    commands['A'] = SortAscending;
    commands['T'] = Traverse;
    commands['B'] = TraverseBackward;
}

void ShowHelp(struct array_list **dummy) {
    puts("静态线性表 (ArrayList) 测试程序。请输入命令，不区分大小写：\n");
    puts("H - 显示帮助 (Help)，");
    puts("Q - 退出程序 (Quit)，\n");
    puts("N [size] - 新建 (New) 长度为size的表，");
    puts("D        - 释放表所占空间 (Destroy)，");
    puts("L        - 显示表长 (Length) 以及其他信息，\n");
    puts("C       - 清空表 (Clear)，");
    puts("F [val] - 以val的值填充 (Fill)，\n");
    puts("I [pos][val] - 在位置pos插入值为val的元素 (Insert)，");
    puts("R [pos]      - 删除位于pos的元素 (Remove)，");
    puts("G [pos]      - 按位置取元素 (Get)，");
    puts("M [pos][val] - 修改元素 (Modify)，\n");
    puts("S [val]         - 按值查找首次出现的元素 (Search)，");
    puts("P [val][newval] - 按值查找所有与之相等元素并替换 (rePlace)，\n");
    puts("A - 升序排序 (sort Ascending)，");
    puts("T - 正序遍历 (Traverse)，");
    puts("B - 逆序遍历 (traverse Backward)。");
}

void Quit(struct array_list **dummy) {
    puts("感谢使用，再见！");
    exit(0);
}

void ShowStatus(struct array_list **a) {
    if (ERROR_SIZE == ArrayListGetLength(*a)) {
        puts("无效的表。");
    } else {
        printf("length = %lu, capacity = %lu, element size = %lu byte(s)\n",
               ArrayListGetLength(*a), ArrayListGetCapacity(*a), ArrayListGetElemSize(*a));
        if (ArrayListIsEmpty(*a))
            puts("表为空。");
        if (ArrayListIsFull(*a))
            puts("表已满。");
    }
}

void Create(struct array_list **a) {
    size_t initSize;
    scanf("%lu", &initSize);
    ArrayListDelete(a);
    *a = ArrayListCreate(initSize, sizeof(int));
    if(*a != NULL)
        puts("创建成功。");
}

void Destroy(struct array_list **a) {
    ArrayListDelete(a);
    puts("已释放表。");
}

void Clear(struct array_list **a) {
    if (ArrayListClear(*a)) {
        puts("已清空表。");
    } else {
        puts("无法清空：无效的表。");
    }
}

void Fill(struct array_list **a) {
    int x;
    scanf("%d", &x);
    if (!ArrayListFill(*a, &x)) {
        puts("填充失败。");
    }
}

void InsertElem(struct array_list **a) {
    size_t i;
    int x;
    scanf("%lu%d", &i, &x);
    if (ArrayListInsertElem(*a, i, &x)) {
        printf("插入成功：已将值为 %d 的元素插入到 %lu 号位置。\n", x, i);
    } else {
        puts("插入失败。");
    }
}

void RemoveElem(struct array_list **a) {
    size_t i;
    scanf("%lu", &i);
    if (ArrayListRemoveElem(*a, i)) {
        printf("删除成功：已删除 %lu 号位置的元素。\n", i);
    } else {
        puts("删除失败。");
    }
}

void GetElem(struct array_list **a) {
    size_t i;
    int x;
    scanf("%lu", &i);
    if (ArrayListGetElem(*a, i, &x)) {
        printf("The element at position %lu is %d.\n", i, x);
    } else {
        puts("查询失败。");
    }
}

void SetElem(struct array_list **a) {
    size_t i;
    int x;
    scanf("%lu%d", &i, &x);
    if (ArrayListSetElem(*a, i, &x)) {
        puts("修改成功。");
    } else {
        puts("修改失败。");
    }
}

void FindElem(struct array_list **a) {
    int x;
    scanf("%d", &x);
    size_t idx = ArrayListFind(*a, &x, CmpInt);
    switch (idx) {
    case ERROR_SIZE:
        puts("List not exist!");
        break;
    case NOT_FOUND:
        puts("Not found!");
        break;
    default:
        printf("index of %d is %lu.\n", x, idx);
        break;
    }
}

void ReplaceElem(struct array_list **a) {
    int val, newval, tmp;
    scanf("%d%d", &val, &newval);
    ArrayListIter it;
    for (it = ArrayListIterFirst(*a); ArrayListIterHasNext(it); ArrayListIterNext(it)) {
        ArrayListIterGetNext(it, &tmp);
        if (0 == CmpInt(&val, &tmp))
            ArrayListIterSetNext(it, &newval);
    }
    ArrayListIterDelete(&it);
}

void SortAscending(struct array_list **a) {
    if (ArrayListSort(*a, CmpInt)) {
        puts("已排序。");
    } else {
        puts("排序失败：无效的表。");
    }
}

void Traverse(struct array_list **a) {
    putchar('[');
    int tmp;
    ArrayListIter it = ArrayListIterFirst(*a);
    while (ArrayListIterHasNext(it)) {
        ArrayListIterGetNext(it, &tmp);
        VisInt(&tmp);
        ArrayListIterNext(it);
    }
    ArrayListIterDelete(&it);
    puts("]");
}

void TraverseBackward(struct array_list **a) {
    putchar('[');
    int tmp;
    ArrayListIter it;
    for (it = ArrayListIterLast(*a); ArrayListIterHasPrev(it); ArrayListIterPrev(it)) {
        ArrayListIterGetPrev(it, &tmp);
        VisInt(&tmp);
    }
    ArrayListIterDelete(&it);
    puts("]");
}

void VisInt(const void *x) {
    printf("%d ", *(int *)x);
}

int CmpInt(const void *a, const void *b) {
    if (*(int *)a < *(int *)b) {
        return -1;
    } else if (*(int *)a > *(int *)b) {
        return 1;
    } else {
        return 0;
    }
}
