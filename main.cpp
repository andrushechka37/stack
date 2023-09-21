#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// smart realloc with gisteresis

typedef int elem_t;

const int start_capacity = 25;
const int realloc_const = 2;

enum errors {
    pop_with_0_size = 1,
    stk_zero_pointer = 2,
    size_bigger_capacity = 3,
    size_below_zero = 4,
    capacity_below_zero = 5
};

struct stack {
    elem_t * data;
    int size;
    int capacity;
};


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
    void * error_status = realloc(stk, ((stk->capacity) / realloc_const) + 1);
    return 0;
}

int stack_extension(stack * stk) {
    void* error_status = realloc(stk, (stk->capacity) * realloc_const);
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
    if (verificator_of_stack(stk) != 0) {
        return -1;
    }

    if (stk->size == stk->capacity) {
        stack_extension(stk);
    }
    stk->data[stk->size++] = value;
    return 0;
}
int stack_ctor(stack * stk) {
    stk->capacity = start_capacity;
    stk->size = 0;
    stk->data = (elem_t *)calloc(start_capacity, sizeof(elem_t));
    return 0;
}



int main(void) {
    stack stk = {};
    stack_ctor(&stk);

}
