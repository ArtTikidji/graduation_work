//
//  basic_functions.c
//  Scientific_work_v2.0
//
//  Created by Artem on 13/08/2018.
//  Copyright © 2018 Artem. All rights reserved.
//

#include "Header.h"

int arcFree(struct Graph inp, int point){
    int result = -1;
    if (point == inp.pointsAmo)
        return result;
    for (int i = 0; result == -1 && i < inp.p; ++i)
        if(inp.graph[point*inp.p + i] == 0)
            result = i;
    
    if(result == 0){
        bool exist = 0;
        for(int i = 0; !exist && i < inp.p; ++i)
            exist = inp.graph[point*inp.p + i] != 0;
        result = exist ? 0 : -2;
    }
    return result;
}

struct Graph CreateGraph(struct CreateGraphInp inp){
    struct Graph result;
    result.diam = inp.diam;
    result.L = inp.L;
    result.p = inp.p;
    int branchSz = ((int)(pow(inp.p-1, inp.diam-inp.k)) - 1)/(inp.p-2);
    result.pointsAmo = inp.L == 1 ? 1 + inp.p*branchSz : inp.L + inp.L*(inp.p - 2)*branchSz;
    ++result.pointsAmo;
    result.graph = (unsigned int*)calloc(result.pointsAmo * inp.p, sizeof(unsigned int));
    unsigned int *tmpGraph = result.graph;
    unsigned int lastFreePoint=inp.L+1;
    if(inp.L != 1){
        for(int i = 1; i < inp.L; ++i){
            tmpGraph[inp.p*i + 1] = i + 1;
            tmpGraph[inp.p*(i+1)] = i;
        }
        tmpGraph[1*inp.p] = inp.L;
        tmpGraph[inp.L*inp.p + 1] = 1;
    }
    for(int i = 1; i <= inp.L; ++i){
        unsigned int workPoint = arcFree(result, i) == -2 ? 0 : arcFree(result, i),  lvlSz = inp.p - workPoint;
        for(int j = arcFree(result, i) == -2 ? 0 : arcFree(result, i); j < result.p && j >= 0; ++j){
            tmpGraph[inp.p*i + j] = lastFreePoint;
            tmpGraph[lastFreePoint * inp.p] = i;
            ++lastFreePoint;
        }
        workPoint = tmpGraph[i*inp.p + workPoint];
        for(int deep = 1; deep < inp.diam-inp.k; ++deep){
            for(int iterPoint = 0; iterPoint < lvlSz; ++iterPoint){
                for(int iterP = 1; iterP < inp.p; ++iterP){
                    tmpGraph[workPoint*inp.p + iterP] = lastFreePoint;
                    tmpGraph[lastFreePoint*inp.p] = workPoint;
                    ++lastFreePoint;
                }
                ++workPoint;
            }
            lvlSz *= inp.p - 1;
        }
    }
    return result;
}

void graphPrint(struct Graph inp){
    for(int i = 0; i < inp.pointsAmo; ++i){
        printf("\n%3d |", i);
        for(int j = 0; j < inp.p; ++j)
            printf(" %3d", inp.graph[i*inp.p + j]);
    }
    printf("\n");
}

short* dist(struct Graph graph, int point){
    short *result = calloc(graph.pointsAmo, sizeof(short));
    if(point == 0)
        return result;
    int workPoint = point;
    int *qweue = creatr_qweue(graph.pointsAmo);
    push_qweue(qweue, workPoint);
    workPoint = pop_qweue(qweue);
    while (workPoint != -1){
        for(int i = 0; i < graph.p; ++i){
            int tmpPoint = graph.graph[workPoint*graph.p + i];
            if(tmpPoint > graph.pointsAmo){
                printf("Error\n");
                for(int j = 0; j < graph.pointsAmo; ++j){
                    printf("%2d : ", j);
                    for(int k = 0; k < graph.p; ++k){
                        printf("%2d ", graph.graph[j * graph.p + k]);
                    }
                    printf("\n");
                }
            }
            if(arcFree(graph, tmpPoint) != -2 && result[tmpPoint] == 0){
                result[tmpPoint] = result[workPoint]+1;
                push_qweue(qweue, tmpPoint);
            }
        }
        workPoint = pop_qweue(qweue);
    }
    free(qweue);
    result[point] = 0;
    return result;
}

int comp(const void *a, const void *b){
    comp_record *ka = (comp_record *) a;
    comp_record *kb = (comp_record *) b;
    return kb->dist - ka->dist;
}

struct SortOut sortNgrow (short* dist,struct Graph graph, unsigned int maxL){
    struct SortOut result;
    int pointsSz = 0;
    comp_record *tmpMass = calloc(graph.pointsAmo + 1, sizeof(comp_record)), tmpLast = {0, 0};
    for(int i = 1; i < graph.pointsAmo; ++i){
        if(arcFree(graph, i) >= 0){ // есть ли у вершины не проведенные ребра
            if(dist[i] >= maxL-1){
                tmpMass[pointsSz].point = i;
                tmpMass[pointsSz].dist = dist[i];
                ++pointsSz;
            }
            else
                if(dist[i] > tmpLast.dist){
                    tmpLast.point = i;
                    tmpLast.dist = dist[i];
                }
        }
    }
    if(tmpLast.point != 0){
        tmpMass[pointsSz++] = tmpLast;
    }
    qsort(tmpMass, graph.pointsAmo, sizeof(comp_record), comp);
    result.points = calloc(pointsSz, sizeof(int));
    result.dist = calloc(pointsSz, sizeof(sizeof(int)));
    result.size = pointsSz;
    for(int i = 0; i < pointsSz; ++i){
        if(arcFree(graph, tmpMass[i].point) == -1)
            printf("worning pos: %2d, point: %2d ", i,tmpMass[i].point);
        result.points[i] = tmpMass[i].point;
        result.dist[i] = tmpMass[i].dist;
    }
    free(tmpMass);
    return result;
}

int prevRootFind(struct Graph graph, int point){
    int result = point, prev = graph.graph[point * graph.p];
    while (prev > graph.L) {
        result = prev;
        prev = graph.graph[result * graph.p];
    }
    return result;
}

struct SortOut del_isomophism(struct SortOut inp, struct Graph graph, int workPoint){
    struct SortOut result;
    int *sumAll = calloc(inp.size, sizeof(int)),
        *sumSubRoot = calloc(inp.size, sizeof(int)),
        *connectSubRoot = calloc(inp.size, sizeof(int)),
        *sumConnectSubRoot = calloc(inp.size, sizeof(int));
    int workSubRoot = prevRootFind(graph, workPoint);
    short *tmpDist, *tmpSubRootDist;
    for(int i = 0; i < inp.size; ++i){
        connectSubRoot[i] = prevRootFind(graph, inp.points[i]);
        tmpDist = dist(graph, inp.points[i]);
        tmpSubRootDist = dist(graph, connectSubRoot[i]);
        
        sumSubRoot[i] = tmpDist[workSubRoot];
        for(int j = 1; j < graph.pointsAmo; ++j){
            sumAll[i] += tmpDist[j];
            sumConnectSubRoot[i] += tmpSubRootDist[j];
        }
        free(tmpDist);
        free(tmpSubRootDist);
    }
    int size = 1;
    int *isomorph = calloc(inp.size, sizeof(int));
    isomorph[0] = 0;
    for(int i = 0; i < inp.size; ++i){
        bool nov = 1;
        for(int j = 0; j < size && nov; ++j){
            nov = sumAll[i] != sumAll[isomorph[j]]
             || inp.dist[i] != inp.dist[isomorph[j]]
             || sumSubRoot[i] != sumSubRoot[isomorph[j]]
             || sumConnectSubRoot[i] != sumConnectSubRoot[isomorph[j]];
            //printf("%d=%d and %d=%d and %d=%d\n", sumAll[i], sumAll[isomorph[j]], inp.dist[i], inp.dist[isomorph[j]], sumSubRoot[i], sumSubRoot[isomorph[j]]);
        }
        if(nov)
            isomorph[size++] = i;
    }
    result.size = size;
    result.points = calloc(size, sizeof(int));
    result.dist = calloc(size, sizeof(int));
    
    for(int i = 0; i < size; ++i){
        result.dist[i] = inp.dist[isomorph[i]];
        result.points[i] = inp.points[isomorph[i]];
    }
    
    free(isomorph);
    free(sumAll);
    free(sumSubRoot);
    free(sumConnectSubRoot);
    free(connectSubRoot);
    return result;
}

bool pointExist(struct Graph graph, int point){
    bool result = 0;
    for(int i = 0; i < graph.p && !result; ++i)
        result = result || graph.graph[point * graph.p + i];
    return result;
}

struct Graph readGraphFile(char *filname){
    struct Graph result = {NULL, 0, 0, 0, 0};
    FILE *fIn;
    fIn = fopen(filname, "r");
    fscanf(fIn, "%d", &result.diam);
    fscanf(fIn, "%d", &result.p);
    fscanf(fIn, "%d", &result.L);
    fscanf(fIn, "%d", &result.pointsAmo);
    result.graph = calloc(result.pointsAmo, sizeof(unsigned int));
    for(int i = 0; i < result.pointsAmo; ++i){
        for(int j = 0; j < result.p; ++j)
            fscanf(fIn, "%d", &result.graph[i * result.p + j]);
    }
    fclose(fIn);
    return result;
}

void writeGraphFile(struct Graph inp, double time, char *filname){
    FILE *fOut;
    fOut = fopen(filname, "w");
    fprintf(fOut, "%2d \n", inp.diam);
    fprintf(fOut, "%2d \n", inp.p);
    fprintf(fOut, "%2d \n", inp.L);
    fprintf(fOut, "%2d \n", inp.pointsAmo);
    for(int i = 0; i < inp.pointsAmo; ++i){
        for(int j = 0; j < inp.p; ++j)
            fprintf(fOut, "%3d ", inp.graph[i * inp.p + j]);
        fprintf(fOut, "\n");
    }
    fprintf(fOut, "%2.8f \n", time);
    fclose(fOut);
}
