#include <stdio.h>
#include <stdlib.h>
#include <math.h>
// if compilliation
//dtor
typedef int elem_t;
typedef unsigned long long canary_t;
const int start_capacity = 3;
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
    unsigned long long left_canary;

    elem_t * data;
    int size;
    int capacity;

    unsigned long long  right_canary;
};
int put_canary(stack * stk, char type) {
    if (type == 'l') {
        ((canary_t*)stk->data)[0] = 0xDEADBEEF;
        stk->data = (elem_t*)&(((canary_t*)stk->data)[1]);
    }

    if (type == 'r') {
        //*(canary_t *)(stk->data + (stk->capacity / 2) - 2) = 0;
        int fatnees_of_canary = sizeof(canary_t) / sizeof(elem_t);
        for (int i = 0; i < fatnees_of_canary; i++) {
            stk->data[stk->capacity + i] = 0;
        }

        *(canary_t *)(stk->data + stk->capacity) = 0xDEADBEEF;
    }
    return 0;
}

int verificator_of_stack(stack * stk, const char *file, int line, const char *func);

#define verify(stk) verificator_of_stack(&stk, __FILE__, __LINE__, __func__)
#define can(stk, t) put_canary(&stk, t)

void dump_stk(stack * stk, const char *file, int line, const char * func);

int stack_compression(stack * stk) {
    verify(*stk);
    stk->data = (elem_t *)&(((canary_t*)stk->data)[-1]);
    stk->data = (elem_t *)realloc(stk->data, sizeof(elem_t) * (((stk->capacity) / realloc_const)) + 2 * sizeof(canary_t));
    stk->data = (elem_t*)&(((canary_t*)stk->data)[1]);
    stk->capacity = (stk->capacity / realloc_const);
    can(*stk, 'r');
    verify(*stk);
    return 0;
}

int stack_extension(stack * stk) {
    verify(*stk);
    stk->data = (elem_t*)&(((canary_t*)stk->data)[-1]);
    printf("adress of data was %p\n", stk->data);
    stk->data = (elem_t *) realloc(stk->data, sizeof(elem_t) * (stk->capacity * realloc_const) + 2 * sizeof(canary_t));
    stk->data = (elem_t*)&(((canary_t*)stk->data)[1]);
    printf("adress of data now %p\n", stk->data);
    stk->capacity *= realloc_const;
    can(*stk, 'r');
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
    if (stk->size + 2 >= stk->capacity) {
        printf("push call realloc\n");
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
    stk->data = (elem_t *)calloc(sizeof(elem_t) * start_capacity + 2 * sizeof(canary_t), 1);
    can(*stk, 'l');
    can(*stk, 'r');
    return 0;
}



int main(void) {
    stack stk = {};
    stack_ctor(&stk);
    for(int i = 3; i < 15; i++) {
        stack_push(&stk, i);
    }
    dump_stk(&stk, " ", 1," ");
    for(int i = 15; i > 5; i--) {
        int t = i;
        stack_pop(&stk, &t);
        printf("%d\n", t);
        dump_stk(&stk, " ", 1," ");
    }
    for(int i = 3; i < 15; i++) {
        stack_push(&stk, i);
    }
    dump_stk(&stk, " ", 1," ");
    for(int i = 15; i > 5; i--) {
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
    printf("data [%p]\n", stk->data);
    printf("left canary of array = %llu\n", ((canary_t*)stk->data)[-1]);
    for (int i = 0; i < stk->capacity; i++) {
        if (stk->data[i] == -999) {
            printf("*[%d] = -999(POISON)\n", i);
        } else {
            printf(" [%d] = %d\n", i, stk->data[i]);
        }
    }
    printf("right canary of array = %llu\n", *(canary_t *)(stk->data + stk->capacity));
}



int verificator_of_stack(stack * stk, const char *file, int line, const char * func) {
    bool error = 0;
    if(stk->left_canary != 0xDEADBEEF) {
        printf("left canary of struct died\n");
        error = 1;
    }

    if(stk->right_canary != 0xDEADBEEF) {
        printf("right canary of struct died\n");
        error = 1;
    }

    if(((canary_t*)stk->data)[-1] != 0xDEADBEEF) {
        printf("left canary of array died\n");
        error = 1;
    }

    if(*(canary_t *)(stk->data + stk->capacity) != 0xDEADBEEF) {
        printf("right canary of array died\n");
        error = 1;
    }

    if(stk == NULL) {
        printf("stk_zero_pointer\n");
        error = 1;
    }

    if (stk->size > stk->capacity) {
        printf("size_bigger_capacity\n");
        error = 1;
    }

    if (stk->capacity < 0) {
        printf("capacity_below_zero\n");
        error = 1;
    }

    if (stk->size < 0) {
        printf("size_below_zero\n");
        error = 1;
    }
    if(error == 1) {
        dump_stk(stk, file, line, func);
        exit(-1);
    }
    //if(//it was called from pop)
    //data 0
    return 0;
}
