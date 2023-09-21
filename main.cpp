#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// smart realloc with gisteresis

typedef int elem_t;

const int start_capacity = 80;
const int realloc_const = 2;

enum errors {
    pop_with_0_size = 1,
    stk_zero_pointer = 2,
    size_bigger_capacity = 3,
    size_below_zero = 4,
    capacity_below_zero = 5
};
#define STACK_DUMP(stk) dump_stk(&stk, __FILE__, __LINE__, __func__)
struct stack {
    elem_t * data;
    int size;
    int capacity;
};

void dump_stk(stack * stk, char * file, int line, const char * func) {
    printf("stack [%p]\n", stk);
    printf("called from file %s\n", file);
    printf("called from function %s\n", func);
    printf("called from line %d\n", line);
    printf("size = %d\n", stk->size);
    printf("capacity = %d\n", stk->capacity);
    printf("data [%p]\n", &stk->data);
    for (int i = 0; i < stk->capacity; i++) {
        printf("[%d] = %d\n", i, stk->data[i]);
    }
}
int verificator_of_stack(stack * stk) {
    if(stk == NULL) {
        printf("stk_zero_pointer");
        return stk_zero_pointer;
    }

    if (stk->size > stk->capacity) {
        printf("size_bigger_capacity");
        return size_bigger_capacity;
    }

    if (stk->capacity < 0) {
        printf("capacity_below_zero");
        return capacity_below_zero;
    }

    if (stk->size < 0) {
        printf("size_below_zero");
        return size_below_zero;
    }
    //if(//it was called from pop)
    return 0;
}

int stack_compression(stack * stk) {
    stk->data = (elem_t *)realloc(stk->data, ((stk->capacity) / realloc_const) + 1);
    return 0;
}

int stack_extension(stack * stk) {
    printf("adress of data was %p", &stk->data);
    stk->data = (elem_t *) realloc(stk->data, stk->capacity * realloc_const);
    if(stk->data == NULL) {
        printf("gg");
        return -1;
    }
    printf("adress of data now %p", &stk->data);
    stk->capacity *= realloc_const;
    return 0;
}

int stack_pop(stack * stk, elem_t * value) {
    if (verificator_of_stack(stk) != 0) {
        return -1;
    }

    if (stk->size == 0) {
        printf("pop with zero size");
        return -1;
    }

    *value = stk->data[--stk->size];
    stk->data[stk->size] = NAN;
    if (stk->size < (stk->capacity) / 2) {
        stack_compression(stk);
    }
    return 0;
}

int stack_push(stack * stk, elem_t value) {
    //if stack is full call extension
    if (stk->size == stk->capacity) {
        printf("push call reaalock");
        stack_extension(stk);
    }
    // if (verificator_of_stack(stk) != 0) {
    //     return -1;
    // }
    stk->data[stk->size++] = value;
    return 0;
}
int stack_ctor(stack * stk) {
    stk->capacity = start_capacity;
    stk->size = 0;
    stk->data = (elem_t *)calloc(start_capacity, sizeof(elem_t));
    stk->data[1] = 78;
    stk->data[4] = NAN;
    return 0;
}



int main(void) {
    stack stk = {};
    stack_ctor(&stk);
    stack_push(&stk, 12);
    stack_push(&stk, 36);
    for(int i = 0; i < 20; i++) {
        stack_push(&stk, i);
    }
    STACK_DUMP(stk);

}
