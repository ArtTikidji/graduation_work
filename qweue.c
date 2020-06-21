//
//  qweue.c
//  Scientific_work_v2.0
//
//  Created by Artem on 18/08/2018.
//  Copyright © 2018 Artem. All rights reserved.
//

#include "Header.h"

int* creatr_qweue(int n){
    int *result = calloc(n+2, sizeof(unsigned int));
    result[0] = n+2;
    result[1] = 2;
    return result;
}

void push_qweue(int *qweue, int inf){
    int last_point = qweue[1];
    qweue[1] += 1;
    qweue[last_point] = inf;
}

int pop_qweue(int *qweue){
    if (qweue == NULL || qweue[1] == 2)
        return -1;
    //unsigned int last_point = qweue[1];
    int result = qweue[2];
    for(int i = 2; i<qweue[1]; ++i)
        qweue[i] = qweue[i+1];
    qweue[qweue[1]] = 0;
    if(qweue[1] > 1)
        --qweue[1];
    return result;
};
