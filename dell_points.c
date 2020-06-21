//
//  dell_points.c
//  SV_v.2.1
//
//  Created by Artem on 06/11/2018.
//  Copyright © 2018 Artem. All rights reserved.
//

#include "Header.h"

struct Graph dellAndConnect(struct CreateGraphInp createIn, int maxDelCnt){
    struct Graph result = CreateGraph(createIn);
    int dellstep = createIn.p % 2 == 0 ? 1 : 2;
    int dellCount = dellstep;
    bool ready = 0;
    while(!ready && dellCount < maxDelCnt){
        backCode code = createAdr(result, result.diam);
        struct ConnectGraphOut tmpRes = dellPoints(result, code, dellCount);
        if(tmpRes.ready){
            ready = 1;
            free(result.graph);
            result = tmpRes.graphCopy;
        }
        dellCount += dellstep;
    }
    if(!ready){
        free(result.graph); result.graph = NULL;
    }
    return result;
}

struct ConnectGraphOut dellPoints(struct Graph inp, backCode code, int count){
    struct ConnectGraphOut result;
    result.ready = 0;
    int adr = decode(inp, code);
    if(adr > inp.pointsAmo)
        printf("error\n");
        printf("code\n");
        for(int i = 0; i < code.size; ++i){
            printf("%2d ", code.code[i]);
        }
    printf(", coded point %d\n", decode(inp, code));
    
    int* tmp = dellPoint(inp, adr);
    if(count == 0){
        struct Graph copy = inp;
        copy.graph = calloc(copy.pointsAmo * copy.p, sizeof(int));
        for(int i = 0; i < copy.pointsAmo * copy.p; ++i)
            copy.graph[i] = inp.graph[i];
        struct ConnectGraphOut res = connectGraphStack(&copy, copy.L);
        printf("connaction from delited poins\n");
        graphPrint(copy);
        printf("\n");
        
        if (!res.ready) free(copy.graph);
        else res.graphCopy = copy;
        return res;
    }


    backCode codeCopy = code;
    codeCopy.code = calloc(codeCopy.size, sizeof(int));
    for(int j = 0; j < codeCopy.size; ++j){
        codeCopy.code[j] = code.code[j];
    }
    for (int i = 0; i < code.size; ++i){
        add(codeCopy, 1, 1, i);
        result = dellPoints(inp, codeCopy, count - 1);
        printf("codeCopy\n");
        for(int i = 0; i < codeCopy.size; ++i){
            printf("%2d ", codeCopy.code[i]);
        }
        printf(", coded point %d\n", decode(inp, codeCopy));
        if(result.ready)
            return result;
        
        free(codeCopy.code);
        codeCopy = code;
        codeCopy.code = calloc(codeCopy.size, sizeof(int));
        for(int j = 0; j < codeCopy.size; ++j){
            codeCopy.code[j] = code.code[j];
        }
    }
    restorePoint(inp, adr, tmp);
    free(tmp);
    return result;
}

int* dellPoint(struct Graph graph, int point){
    if(arcFree(graph, point) == -2)
        return NULL;
    int *result = calloc(graph.p, sizeof(int));
    for(int i = 0; i < graph.p; ++i){
        result[i] = graph.graph[point*graph.p + i];
        if(result[i] != 0){
            int tmpAdr = result[i];
            for(int j = 0; j < graph.p; ++j){
                if(graph.graph[tmpAdr*graph.p + j] == point){
                    graph.graph[tmpAdr*graph.p + j] = 0;
                    graph.graph[point*graph.p + i] = 0;
                    break;
                }
            }
        }
    }
    return result;
}

void restorePoint(struct Graph graph, int adr, int* point){
    if(point == NULL)
        return;
    for(int i = 0; i < graph.p; ++i){
        graph.graph[adr*graph.p + i] = point[i];
        if(point[i] != 0){
            int tmpAdr = point[i], tmpArc = arcFree(graph, tmpAdr);
            if(tmpArc == -1) printf("problam in restore point\n graph whas modifided, point: %2d\n", tmpAdr);
            graph.graph[tmpAdr*graph.p + tmpArc] = adr;
        }
    }
}
