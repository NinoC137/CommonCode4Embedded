#ifndef _STACK_H__
#define _STACK_H__

#include "stdio.h"

typedef struct stack_t
{
    void* arr;          //泛型数组
    size_t capacity;    //数组的容量
    size_t size;        //逻辑元素的个数
    size_t elem_size;   //每个元素的内存大小
} stack_t;

stack_t* stack_create(size_t elem_size);
void stack_free(stack_t* s);
void stack_push(stack_t* s, void* elem);
void stack_pop(stack_t* s, void* result);

#endif