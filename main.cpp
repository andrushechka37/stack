#include <stdio.h>
#include <stdlib.h>
#include <math.h>
// if compilliation
// 12 param
typedef int elem_t;

const int start_capacity = 2;
const int realloc_const = 2;

enum errors {
    pop_with_0_size = 1,
    stk_zero_pointer = 2,
    size_bigger_capacity = 3,
    size_below_zero = 4,
    capacity_below_zero = 5,
    dead_canary = 6
};
struct stack {
    int left_canary;

    elem_t * data;
    int size;
    int capacity;

    int right_canary;
};
int verificator_of_stack(stack * stk, const char *file, int line, const char *func);
#define verify(stk) verificator_of_stack(&stk, __FILE__, __LINE__, __func__)


void dump_stk(stack * stk, const char *file, int line, const char * func);

int stack_compression(stack * stk) {
    verify(*stk);
    stk->data = (elem_t *)realloc(stk->data, sizeof(elem_t) * (((stk->capacity) / realloc_const) + 1));
    stk->capacity = (stk->capacity / realloc_const) + 1;
    verify(*stk);
    return 0;
}

int stack_extension(stack * stk) {
    verify(*stk);
    printf("adress of data was %p\n", &stk->data);
    stk->data = (elem_t *) realloc(stk->data, sizeof(elem_t) * (stk->capacity * realloc_const));
    printf("adress of data now %p\n", &stk->data);
    stk->capacity *= realloc_const;
    verify(*stk);
    return 0;
}

int stack_pop(stack * stk, elem_t * value) {
    verify(*stk);
    if (stk->size == 0) {
        printf("pop with zero size");
        return -1;
    }

    *value = stk->data[--stk->size];
    stk->data[stk->size] = -999;
    if (stk->size < (stk->capacity) / 2) {
        stack_compression(stk);
    }
    verify(*stk);
    return 0;
}

int stack_push(stack * stk, elem_t value) {
    verify(*stk);
    if (stk->size == stk->capacity) {
        printf("push call reaalock\n");
        stack_extension(stk);
    }
    stk->data[stk->size++] = value;
    verify(*stk);
    return 0;
}
int stack_ctor(stack * stk) {
    stk->left_canary = 0xDEADBEEF;
    stk->right_canary = 0xDEADBEEF;
    stk->capacity = start_capacity;
    stk->size = 0;
    stk->data = (elem_t *)calloc(start_capacity, sizeof(elem_t));
    return 0;
}



int main(void) {
    stack stk = {};
    stack_ctor(&stk);
    for(int i = 0; i < 15; i++) {
        stack_push(&stk, i);
    }
    dump_stk(&stk, " ", 1," ");
    for(int i = 15; i > 1; i--) {
        int t = i;
        stack_pop(&stk, &t);
        printf("%d\n", t);
        dump_stk(&stk, " ", 1," ");
    }

}



void dump_stk(stack * stk, const char *file, int line, const char * func) {
    printf("----------------------------------------------\n");
    printf("stack [%p]\n", stk);
    printf("called from file %s\n", file);
    printf("called from function %s\n", func);
    printf("called from line %d\n", line);
    printf("size = %d\n", stk->size);
    printf("capacity = %d\n", stk->capacity);
    printf("data [%p]\n", &stk->data);
    for (int i = 0; i < stk->capacity; i++) {
        if (stk->data[i] == -999) {
            printf("*[%d] = -999(POISON)\n", i);
        } else {
            printf(" [%d] = %d\n", i, stk->data[i]);
        }
    }
}



int verificator_of_stack(stack * stk, const char *file, int line, const char * func) {
    if(stk->left_canary != 0xDEADBEEF) {
        printf("left canary of struct died");
        dump_stk(stk, file, line, func);
        exit(-1);
        return dead_canary;
    }

    if(stk->right_canary != 0xDEADBEEF) {
        printf("right canary of struct died");
        dump_stk(stk, file, line, func);
        exit(-1);
        return dead_canary;
    }

    if(stk == NULL) {
        printf("stk_zero_pointer");
        dump_stk(stk, file, line, func);
        exit(-1);
        return stk_zero_pointer;
    }

    if (stk->size > stk->capacity) {
        printf("size_bigger_capacity");
        dump_stk(stk, file, line, func);
        exit(-1);
        return size_bigger_capacity;
    }

    if (stk->capacity < 0) {
        printf("capacity_below_zero");
        dump_stk(stk, file, line, func);
        exit(-1);
        return capacity_below_zero;
    }

    if (stk->size < 0) {
        printf("size_below_zero\n");
        dump_stk(stk, file, line, func);
        exit(-1);
        return size_below_zero;
    }
    //if(//it was called from pop)
    //data 0
    return 0;
}
