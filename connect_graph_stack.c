//
//  connect_graph_stack.c
//  Scientific_work_v2.0
//
//  Created by Artem on 31/08/2018.
//  Copyright © 2018 Artem. All rights reserved.
//

#include "Header.h"

struct ConnectGraphOut connectGraphStack(struct Graph *graph, unsigned int bestL){
    struct ConnectGraphOut result;
    result.arcAdres = 0;
    result.delPoints = 0;
    result.L = 0;
    result.ready = 0;
    
    time_t prevTime = clock();
    int stepCnt = 0;
    time_t stepSize = CLOCKS_PER_SEC, eps = CLOCKS_PER_SEC;
    stepSize *= 60;
    
    time_t begin_havyTime = 0, end_havyTime = 0;
    FILE *havyTimeFile;
//    havyTimeFile = fopen("havy_time.txt", "a");
    
    int maxL = bestL > graph->L ? bestL : graph->L;
    struct myStack stack = createStack(graph->pointsAmo * graph->p);
    int workPoint = 1, nextPoint = 1, workPos = 0;
    int workArc = arcFree(*graph, workPoint), nextArc = -1;
    bool upInTree = 0;
    for(workPoint = 1;workPoint < graph->pointsAmo && workArc <= 0; ++workPoint, workArc = arcFree(*graph, workPoint))
        {workArc = arcFree(*graph, workPoint);}
//    end_havyTime = clock();
    short *distanse = dist(*graph, workPoint);
    struct SortOut sortGr = sortNgrow(distanse, *graph, maxL); //           <====
    struct SortOut isomorph = del_isomophism(sortGr, *graph, workPoint);
    free(sortGr.dist);
    free(sortGr.points);
    sortGr = isomorph;
    
    int connectPoint = sortGr.points[0], fullAdr = connectPoint * graph->p;
    fullAdr += arcFree(*graph, connectPoint);
    
    graph->graph[fullAdr] = workPoint;
    graph->graph[workPoint*graph->p + workArc] = connectPoint;

    pushStack(&stack, workPoint, workArc, workPos, fullAdr, sortGr, distanse);
    for(nextPoint = workPoint, nextArc = arcFree(*graph, nextPoint); nextPoint < graph->pointsAmo && nextArc == -1; ++nextPoint, nextArc = arcFree(*graph, nextPoint));
    while(stack.size != 0 && !result.ready){
        if((stack.size - 1) == 5){
            begin_havyTime = end_havyTime;
            end_havyTime = clock();
        }
        if((stack.size - 1) == 5 && begin_havyTime != 0){
            havyTimeFile = fopen("subTree_time.txt", "a");
            double time = ((double)end_havyTime - (double)begin_havyTime)/CLOCKS_PER_SEC;
            fprintf(havyTimeFile, "%d %d %f\n", graph->pointsAmo, (stack.size - 1), time);
            fclose(havyTimeFile);
        }
        workPos = 0;
        if(upInTree){
            free(distanse);         distanse = NULL;
            free(sortGr.dist);      sortGr.dist = NULL;
            free(sortGr.points);    sortGr.points = NULL;
            
            time_t tmpTime = clock();
            if ((tmpTime - prevTime) > (stepSize - eps) && (tmpTime  - prevTime) < (stepSize + eps))
                prevTime = printStackState(stack, &stepCnt);
            
            popStack(&stack, &nextPoint, &nextArc, &workPos, &fullAdr, &sortGr, &distanse);
            if(graph->graph[fullAdr] == nextPoint && graph->graph[nextPoint * graph->p + nextArc] == fullAdr/graph->p){
                graph->graph[fullAdr] = 0;
                graph->graph[nextPoint * graph->p + nextArc] = 0;
            }
            ++workPos;
            upInTree = 0;
            
        }
        
        workPoint = nextPoint;
        workArc = nextArc;
        
        if(workPos == 0){
            distanse = dist(*graph, workPoint);
            sortGr = sortNgrow(distanse, *graph, maxL); //                  <======
            if(stack.size < 5){
                struct SortOut isomorph = del_isomophism(sortGr, *graph, workPoint);
                free(sortGr.dist);
                free(sortGr.points);
                sortGr = isomorph;
            }
        }
        if(workPos >= sortGr.size){
            upInTree = 1;
            continue;
        }
        if(workPos == sortGr.size - 1 && sortGr.dist[sortGr.size - 1] < maxL-1){
            //result.L = workPos == sortGr.size - 1 && sortGr.dist[workPos] > result.L - 1 ? sortGr.dist[workPos]+1 : result.L;
            result.L = result.L < maxL ? maxL : result.L;
            //printf("L = %2d\n", result.L);
            upInTree = 1;
            continue;
        }
        connectPoint = sortGr.points[workPos];
        fullAdr = connectPoint * graph->p + arcFree(*graph, connectPoint);
        graph->graph[workPoint * graph->p + workArc] = connectPoint;
        graph->graph[fullAdr] = workPoint;
        
        for(nextPoint = workPoint;nextPoint < graph->pointsAmo && arcFree(*graph, nextPoint) < 0; ++nextPoint);
        nextArc = arcFree(*graph, nextPoint);
        
        if(nextPoint == graph->pointsAmo || nextArc < 0){
            result.ready = 1;
            for(int i = 1; i < graph->pointsAmo && result.ready; ++i){ // достигнут лист дерева вариантов
                if(arcFree(*graph, i) == -2)
                    continue;
                short *tmpDist = dist(*graph, i);
                for(int j = 1; j < graph->pointsAmo && result.ready; ++j){
                    if(arcFree(*graph, j) != -2){
                        result.ready = tmpDist[j] <= graph->diam; // проверка выполнения условия диаметра
                    }
                    else{
                        printf("not exist \n");
                    }
                }
                free(tmpDist);
            }
            if(result.ready){
//                fclose(havyTimeFile);
                return result;
            }
            else{
                result.delPoints = 1;
                if(result.L == maxL && graph->L != maxL){ // минимальная длинна образовавшегося цикла совпадает с базовой длинной уже построенного графа (прекратить перебор, а после вернуть уже построенный граф)
                    break;
                }
                if(graph->graph[fullAdr] == workPoint && graph->graph[workPoint * graph->p + workArc] == fullAdr/graph->p){ // разрыв построенной на последнем шаге дуги
                    graph->graph[fullAdr] = 0;
                    graph->graph[workPoint * graph->p + workArc] = 0;
                }
                upInTree = 1;
                continue;
            }
        }
        pushStack(&stack, workPoint, workArc, workPos, fullAdr, sortGr, distanse);
    }
    if(!result.ready && result.L == graph->L){
        result.delPoints = 1;
    }
    
    if(maxL != graph->L && result.L < maxL){
        free(graph->graph);
        *graph = readGraphFile("bestL.txt");
        result.L = graph->L;
        result.delPoints = 0;
    }
//    fclose(havyTimeFile);
    return result;
}
