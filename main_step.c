//
//  main_step.c
//  Scientific_work_v2.0
//
//  Created by Artem on 26/08/2018.
//  Copyright © 2018 Artem. All rights reserved.
//

#include "Header.h"

struct Graph mainStep(unsigned int p, unsigned int diam, unsigned int bestL, long *time){
    struct CreateGraphInp creatIn;
    creatIn.diam = diam;
    creatIn.k = 0;
    creatIn.L = 1;
    creatIn.p = p;
    struct Graph result = CreateGraph(creatIn), *graph = &result;
    printf("\n mass in mainStep\n");
    for(int j = 0; j < graph->pointsAmo; ++j){
        printf("%2d : ", j);
        for(int k = 0; k < graph->p; ++k){
            printf("%2d ", graph->graph[j * graph->p + k]);
        }
        printf("\n");
    }
    struct ConnectGraphInp connectGr;
    connectGr.point = 0;
    connectGr.arc = 0;
    connectGr.bestL = bestL;
    connectGr.graph = result;
    time_t startTime = clock();
    //struct ConnectGraphOut connectRes = connectGraph(connectGr);
    struct ConnectGraphOut connectRes = connectGraphStack(&result, bestL);
    time_t endTime = clock();
    *time = endTime - startTime;
    if(connectRes.ready)
        return result;
    free(result.graph);
    result = readGraphFile("bestL.txt");
    return result;
}
