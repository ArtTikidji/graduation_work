//
//  main.c
//  SV_v.2.1
//
//  Created by Artem on 22/10/2018.
//  Copyright © 2018 Artem. All rights reserved.
//

#include "Header.h"



int main(int argc, const char * argv[]) {
    unsigned  int p, diam;
    FILE *fInp;
    // чтение входных параметров:
    // степень однородности и диаметр графа
    fInp = fopen("input.txt", "r");
    fscanf(fInp ,"Deg = %d\nDiam =  %d", &p, &diam);
    fclose(fInp);
    
    unsigned int maxL = firstStep(p, diam);
    
    long clok = 0;
    struct Graph graph = mainStep(p, diam, maxL,  &clok);
    double time = clok * 1.0/CLOCKS_PER_SEC;
    
    writeGraphFile(graph, time, "outputs.txt");
    free(graph.graph);
    return 0;
}

