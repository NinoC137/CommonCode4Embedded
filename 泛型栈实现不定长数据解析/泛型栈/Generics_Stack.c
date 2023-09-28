#include "Generics_Stack.h"

#include "string.h"
#include "assert.h"
#include "stdlib.h"

stack_t* stack_create(size_t elem_size){
    stack_t* s = malloc(sizeof(stack_t));

    s->arr = malloc(elem_size * 10);    //初始化时至少开辟出能容纳10个泛型元素的内存区

    s->elem_size = elem_size;
    s->size = 0;
    s->capacity = 10;

    return s;
}

void stack_free(stack_t* s){
    free(s->arr);
    free(s);
}

void stack_push(stack_t*s, void* elem){
    //首先判读是否需要扩容
    if(s->size == s->capacity){
        s->capacity <<= 1;  //内存空间*2
        s->arr = realloc(s->arr, s->elem_size * s->capacity);
    }

    //将元素存入内存区
    char* char_ptr = s->arr; //转换为单字节指针, 便于运算后移动一定数量的内存
    char_ptr += s->elem_size * s->size;
    memcpy(char_ptr, elem, s->elem_size);
    s->size++;
}

void stack_pop(stack_t* s, void* result){
    //判断是否为空栈
    if(s->size == 0){
        fprintf(stderr,"Error! you can't pop elements from an empty stack.\r\n");
        exit(1);
    }
    s->size--;
    char* char_ptr = s->arr;
    char_ptr += s->elem_size * s->size;
    memcpy(result, char_ptr, s->elem_size);
}