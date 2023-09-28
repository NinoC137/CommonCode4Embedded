#include "Generics_Stack.h"
#include "assert.h"

int main(void){
    stack_t* s = stack_create(sizeof(int)); //因为内部是通过分配内存的方式进行了内存区的开辟,所以这里声明为指针

    for(int i = 0; i < 20; i++){
        stack_push(s, &i);
        if(i < 10) assert(s->capacity == 10);
        else assert(s->capacity == 20);
    }

    int result;
    for(int i = 0; i < 20; i++){
        stack_pop(s, &result);
        printf("pop a number: %d\r\n", result);
    }

    stack_free(s);

    return 0;
}