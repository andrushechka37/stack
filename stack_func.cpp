#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "main.h"


int stack_ctor(stack * stk) {
    stk->left_canary = 0xDEADBEEF;
    stk->right_canary = 0xDEADBEEF;
    stk->capacity = start_capacity;
    stk->size = 0;
    stk->data = (elem_t *)calloc(sizeof(elem_t) * start_capacity + 2 * sizeof(canary_t), 1);
    can(*stk, 'l');
    *(canary_t *)(stk->data + (stk->capacity)) = 0xDEADBEEF;
    stk->hash_data = calc_data(*stk);
    stk->hash_stack = calc_stack(*stk);
    stk->data = (elem_t*)&(((canary_t*)stk->data)[-1]);
    return 0;
}

int stack_push(stack * stk, elem_t value) {
    //verify(*stk);
    if (stk->size + 2 >= stk->capacity) {
        printf("push call realloc\n");
        stack_extension(stk);
    }
    stk->data[stk->size++] = value;
    stk->hash_data = calc_data(*stk);
    stk->hash_stack = calc_stack(*stk);
    //verify(*stk);
    return 0;
}

int stack_pop(stack * stk, elem_t * value) {
    verify(*stk);
    if (stk->size == 0) {
        printf("pop with zero size");
        exit(-1);
        return -1;
    }

    *value = stk->data[--stk->size];
    stk->data[stk->size] = -999;
    stk->hash_data = calc_data(*stk);
    stk->hash_stack = calc_stack(*stk);
    if (stk->size < (stk->capacity) / 2) {
        stack_compression(stk);
    }
    stk->hash_data = calc_data(*stk);
    stk->hash_stack = calc_stack(*stk);
    verify(*stk);
    return 0;
}

int stack_extension(stack * stk) {
    //verify(*stk);
    stk->data = (elem_t*)&(((canary_t*)stk->data)[-1]);
    printf("adress of data was %p\n", stk->data);
    stk->data = (elem_t *) realloc(stk->data, sizeof(elem_t) * (stk->capacity * realloc_const) + 2 * sizeof(canary_t));
    stk->data = (elem_t*)&(((canary_t*)stk->data)[1]);
    printf("adress of data now %p\n", stk->data);
    can(*stk, 'r');
    stk->capacity *= realloc_const;
    stk->hash_data = calc_data(*stk);
    stk->hash_stack = calc_stack(*stk);
    //verify(*stk);
    return 0;
}

int stack_compression(stack * stk) {
    verify(*stk);
    stk->data = (elem_t *)&(((canary_t*)stk->data)[-1]);
    stk->data = (elem_t *)realloc(stk->data, sizeof(elem_t) * (((stk->capacity) / realloc_const)) + 2 * sizeof(canary_t));
    stk->data = (elem_t*)&(((canary_t*)stk->data)[1]);
    can(*stk, 's');
    stk->capacity = (stk->capacity / realloc_const);
    stk->hash_data = calc_data(*stk);
    stk->hash_stack = calc_stack(*stk);
    verify(*stk);
    return 0;
}

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

        *(canary_t *)(stk->data + (stk->capacity) * realloc_const) = 0xDEADBEEF;
    }
    if (type == 's') {
        int fatnees_of_canary = sizeof(canary_t) / sizeof(elem_t);
        for (int i = 0; i < fatnees_of_canary; i++) {
            stk->data[stk->capacity + i] = 0;
        }
        *(canary_t *)(stk->data + (stk->capacity) / realloc_const) = 0xDEADBEEF;
    }
    return 0;
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
    if(check_hash(*stk) != 0) {
        printf("wrong hash\n");
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

void dump_stk(stack * stk, const char *file, int line, const char * func) {
    printf("----------------------------------------------\n");
    printf("stack [%p]\n", stk);
    printf("called from file %s\n", file);
    printf("called from function %s\n", func);
    printf("called from line %d\n", line);
    printf("size = %d\n", stk->size);
    printf("capacity = %d\n", stk->capacity);
    printf("data hash is %d\n", stk->hash_data);
    printf("struct hash is %d\n", stk->hash_stack);
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
    printf("----------------------------------------------\n");
}

int hash_counter_stack(stack * stk) {
    return stk->capacity + stk->size;
}
int hash_counter_data(stack * stk) {
    int sum = 0;
    for(int i = 0; i <= stk->size; i++) {
        if(stk->data[i] != -999) {
            sum += stk->data[i];
        }
    }
    return sum;
}
int hash_check(stack * stk) {
    if (stk->hash_data != hash_counter_data(stk)) {
        printf("%d", hash_counter_data(stk));
        printf(" trouble with data\n");
        return -1;
    }
    if (stk->hash_stack != hash_counter_stack(stk)) {
        printf("trouble with stach struct\n");
        return 1;
    }
    return 0;
}

int stack_dtor(stack * stk) {
    free((elem_t *)&(((canary_t*)stk->data)[-1]));
    return 0;
}
