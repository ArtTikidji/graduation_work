//
//  first_step.c
//  Scientific_work_v2.0
//
//  Created by Artem on 18/08/2018.
//  Copyright © 2018 Artem. All rights reserved.
//

#include "Header.h"

unsigned int firstStep(unsigned int p, unsigned int diam){
    unsigned int bestL = 0, bestAmo = 0;
    for(int L = 3; L <= 2*diam + 1; ++L){
        unsigned int tmpK = 1;
        int tmp1 = L - 1 - (p-1)*(int)pow(p-2, tmpK-1);
        int tmp2 = L - 1 - (p-1)*(1 + (tmpK-1)*(p-2));
        while(tmp2 >= 0 && 2*tmpK < tmp2)
            ++tmpK;
        struct CreateGraphInp creatInp;
        creatInp.diam = diam;
        creatInp.k = tmpK;
        creatInp.L = L;
        creatInp.p = p;
        struct Graph graph = CreateGraph(creatInp);
        if(graph.pointsAmo < bestAmo){
            free(graph.graph);
            break;
        }
        struct ConnectGraphInp connectInp;
        connectInp.arc = 0;
        connectInp.bestL = L;
        connectInp.graph = graph;
        connectInp.point = 0;
        
        time_t startTime = clock();
        struct ConnectGraphOut connectResult = connectGraphStack(&graph, L);
        time_t endTime = clock();
        
        if(!connectResult.ready && connectResult.L != 0 && connectResult.L < L){
            free(graph.graph);
            break;
        }
        if(!connectResult.ready && connectResult.delPoints){
            struct Graph dellRes = dellAndConnect(creatInp, graph.pointsAmo - bestAmo);
            if(dellRes.graph != NULL){
                free(graph.graph);
                graph = dellRes;
                connectResult.ready = 1;
            }
        }
        if(connectResult.ready){
            char *filename = malloc(sizeof("Cykle0.txt"));
            sprintf(filename, "Cykle%d.txt", L);
            long clik = endTime - startTime, down = CLOCKS_PER_SEC;
            double time = (double)clik/down;
            writeGraphFile(graph, time , filename);
        }
        if(connectResult.ready && graph.pointsAmo >= bestAmo){
            bestL = graph.L;
            bestAmo = graph.pointsAmo;
            writeGraphFile(graph, 0, "bestL.txt");
        }

        free(graph.graph);
    }
    return bestL;
}
