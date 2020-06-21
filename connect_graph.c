//
//  connect_graph.c
//  Scientific_work_v2.0
//
//  Created by Artem on 18/08/2018.
//  Copyright © 2018 Artem. All rights reserved.
//

#include "Header.h"

struct ConnectGraphOut connectGraph(struct ConnectGraphInp inp){
    unsigned int maxL = inp.bestL > inp.graph.L ? inp.bestL : inp.graph.L;
    bool root = inp.point == 0 && inp.arc == 0;
    int workPoint = inp.point, workArc = inp.arc;
    struct ConnectGraphOut result;
    result.arcAdres = 0;
    result.delPoints = 0;
    result.L = 0;
    result.ready = 0;
    if(root){
        workPoint = 1;
        workArc = arcFree(inp.graph, workPoint);
        while(workArc == -1 && workPoint < inp.graph.pointsAmo){
            ++workPoint;
            workArc = arcFree(inp.graph, workPoint);
        }
    }
    if(workPoint >= inp.graph.pointsAmo && workArc == -1){
        bool tmpProblem = 0;
        for(int i = 1; i < inp.graph.pointsAmo && !tmpProblem; ++i){
            short *tmpDist = dist(inp.graph, i);
            for(int j = 1; j < inp.graph.pointsAmo && !tmpProblem; ++j)
                tmpProblem = tmpDist[j] > inp.graph.diam;
            free(tmpDist);
        }
        if(tmpProblem){
            result.graphCopy =  inp.graph;
            result.graphCopy.graph = calloc(result.graphCopy.pointsAmo * result.graphCopy.p, sizeof(int));
            for(unsigned int i = 0; i < result.graphCopy.pointsAmo * result.graphCopy.p; ++i)
                result.graphCopy.graph[i] = inp.graph.graph[i];
        }
        result.ready = !tmpProblem;
        result.delPoints = tmpProblem;
        return result;
    }
    short *distanse = dist(inp.graph, workPoint);
    struct SortOut sortGr = sortNgrow(distanse, inp.graph, maxL);
    if(sortGr.size == 1){
        result.L = sortGr.dist[0];
        free(distanse);
        free(sortGr.dist);
        free(sortGr.points);
        return result;
    }
    if(sortGr.size < 1)
        printf("\nEbat kopat\n");
    int *ckleMax = calloc(sortGr.size, sizeof(int));
    int connectAdr;
    for(int i = 0; i < sortGr.size; ++i){
        int tmpAdr = sortGr.points[i], tmpDist = sortGr.dist[i];
        connectAdr = tmpAdr* inp.graph.p + arcFree(inp.graph, tmpAdr);
        inp.graph.graph[connectAdr] = workPoint;
        inp.graph.graph[workPoint*inp.graph.p + workArc] = tmpAdr;
        int newPoint, newArc;
        newPoint = workPoint;
        if(arcFree(inp.graph, newPoint) == -1){
            for(newPoint = workPoint+1; newPoint < inp.graph.pointsAmo && arcFree(inp.graph, newPoint) == -1; ++newPoint);
            newArc = arcFree(inp.graph, newPoint);
        }
        else{
            newArc = arcFree(inp.graph, newPoint);
        }
        // ==
        free(distanse);
        free(sortGr.dist);
        free(sortGr.points);
        // ==
        struct ConnectGraphInp newInp = inp;
        newInp.point = newPoint;
        newInp.arc = newArc;
        struct ConnectGraphOut rekOut = connectGraph(newInp);  // <--rek
        result.ready = rekOut.ready;
        if(result.ready){
            //free(distanse);
            //free(sortGr.dist);
            //free(sortGr.points);
            free(ckleMax);
            return result;
        }
        if(rekOut.delPoints && !result.delPoints){
            if(tmpDist == maxL && inp.graph.L != maxL){
                free(rekOut.graphCopy.graph);
                ckleMax[i] = maxL;
            }
            else{
                result.graphCopy = rekOut.graphCopy;
                result.delPoints = 1;
            }
        }
        if(rekOut.L != 0)
            {ckleMax[i] = rekOut.L;}
        inp.graph.graph[connectAdr] = 0;
        inp.graph.graph[workPoint*inp.graph.p + workArc] = 0;
        distanse = dist(inp.graph, workPoint);
        sortGr = sortNgrow(distanse, inp.graph, maxL);
        
    }
    if(result.delPoints){
        free(distanse);
        free(sortGr.dist);
        free(sortGr.points);
        free(ckleMax);
        return result;
    }
    int maxLout = 0;
    for(int i = 0; i < sortGr.size; ++i)
        {maxLout = maxLout >= ckleMax[i] ? maxLout : ckleMax[i];}
    result.L = maxLout;
    if(root){
        if(maxLout < maxL){
            free(inp.graph.graph);
            inp.graph = readGraphFile("bestL.txt");
        }
    }
    free(distanse);
    free(sortGr.dist);
    free(sortGr.points);
    free(ckleMax);
    //if(!result.delPoints && !result.ready && result.L == 0)
    //    printf("\nHuinya kakaya-to\n");

    return result;
}
