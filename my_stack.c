//
//  my_stack.c
//  Scientific_work_v2.0
//
//  Created by Artem on 31/08/2018.
//  Copyright © 2018 Artem. All rights reserved.
//

#include "Header.h"


time_t printStackState(struct myStack stack, int *stapCnt){
    ++ *stapCnt;
    FILE *fstate;
    fstate = fopen("tree_state.txt", "w");
    fprintf(fstate, "step: %3d\n", *stapCnt);
    fprintf(fstate, "treeDeep: %2d\n", stack.size);
    for(int i = 0; i < stack.size; ++i){
        fprintf(fstate, "deep = %2d, position: %2d/%2d\n", i+1, stack.stackPos[i], stack.sortRez[i].size);
    }
    fclose(fstate);
    time_t result = clock();
    return result;
}

struct myStack createStack(unsigned int size){
    struct myStack result;
    result.size = 0;
    result.stackArc = calloc(size, sizeof(int));
    result.stackPoints = calloc(size, sizeof(int));
    result.stackPos = calloc(size, sizeof(unsigned int));
    result.stackFullAdr = calloc(size, sizeof(int));
    result.sortRez = calloc(size, sizeof(struct SortOut));
    result.dist = calloc(size, sizeof(short *));
    return result;
}

void pushStack(struct myStack *stack, int point, int arc, int position, int adres, struct SortOut sortRez, short *dist){
    unsigned int workPos = stack->size;
    stack->stackArc[workPos] = arc;
    stack->stackPoints[workPos] = point;
    stack->stackPos[workPos] = position;
    stack->stackFullAdr[workPos] = adres;
    stack->sortRez[workPos] = sortRez;
    stack->dist[workPos] = dist;
    ++stack->size;
}

void popStack(struct myStack *stack, int *point, int *arc, int *position, int *adres, struct SortOut *sortRez, short **dist){
    unsigned int workPos = --stack->size;
    *point = stack->stackPoints[workPos];
    *arc = stack->stackArc[workPos];
    *position = stack->stackPos[workPos];
    *adres = stack->stackFullAdr[workPos];
    *sortRez = stack->sortRez[workPos];
    *dist = stack->dist[workPos];
}


