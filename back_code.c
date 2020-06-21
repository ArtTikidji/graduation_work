//
//  back_code.c
//  SV_v.2.1
//
//  Created by Artem on 06/11/2018.
//  Copyright © 2018 Artem. All rights reserved.
//

#include "Header.h"

// альтернативный способ доступа к вершинам графа
// используется при переборе вариантов удаления вершин

backCode createAdr(struct Graph graph, int deep){
    backCode result;
    result.L = graph.L;
    result.p = graph.p;
    result.size = deep;
    result.code = calloc(result.size, sizeof(int));
    result.code[0] = 0;
    result.code[1] = result.L != 1 ? 2 : 1;
    for(int i = 2; i < result.size; ++i){
        result.code[i] = 1;
    }
    return result;
}

int decode(struct Graph graph, backCode code){
    int result;
    int tmpCode = code.code[0];
    result = code.L != 1 ? (1 + tmpCode) * graph.p : graph.p + tmpCode;
    printf("\ntmp res, %2d\n", result);
    for(int i = 1; i < code.size; ++i){
        tmpCode = code.code[i];
        result += tmpCode;
        printf("tmp res, %2d\n", result);
        result = graph.graph[result];
        result *= graph.p;
    }
    result = result/graph.p;
    return result;
}

void add(backCode code, int addend, bool fixed, int pos){
    if(pos >= code.size)
        return; // error
    int *tmpCode = code.code;
    int tmpP = code.p, tmpL = code.L;
    int minValPos = 1, maxValPos = tmpP;
    if(pos == 0){
        minValPos = 0;
        if(tmpL != 1)
            maxValPos = tmpL;
    }
    if(pos == 1  &&  tmpL != 1){
        minValPos = 2;
    }
    int sumVal = addend + tmpCode[pos];
    int area =  maxValPos - minValPos;
    tmpCode[pos] = sumVal % (area) + minValPos;
    if(!fixed && sumVal >= area){
        add(code, sumVal/area, fixed, pos+1);
    }
};

