//
//  Header.h
//  Scientific_work_v2.0
//
//  Created by Artem on 13/08/2018.
//  Copyright © 2018 Artem. All rights reserved.
//

#ifndef Header_h
#define Header_h
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

struct Graph{
    unsigned int *graph;
    unsigned int p; // (ожидаемая) степень однородности графа 
    unsigned int diam; // ожедаемый диаметр графа
    unsigned int pointsAmo; // колличество вершин графа
    unsigned int L; // обхват графа
};

struct CreateGraphInp{
    unsigned int p;
    unsigned int diam;
    unsigned int L;
    int k;
};

// входная структура рекурсивной функции
// соеденения вершин
struct ConnectGraphInp{
    struct Graph graph;
    unsigned int point;
    int arc;
    unsigned int bestL;
};

// структура возвращаемая рекурсвной функцией 
// соеденения вершин
struct ConnectGraphOut{
    struct Graph graphCopy;
    bool delPoints;
    bool ready;
    unsigned int arcAdres; // адрес ребра, образованного на 
    // на данном шаге программы
    int L;
};

struct SortOut{
    int size;
    int *dist;
    int *points;
};
struct myStack{
    unsigned int *stackPoints;
    int *stackArc;
    int *stackPos;
    int *stackFullAdr;
    int size;
    struct SortOut *sortRez;
    short **dist;
};

typedef struct {
    int point;
    int dist;
} comp_record;

typedef struct {
    int *code;
    int size;
    int p;
    int L;
} backCode;

/* === stack === */
struct myStack createStack(unsigned int size);
void pushStack(struct myStack *stack, int point, int arc, int position, int adres, struct SortOut sortRez, short *dist);
void popStack(struct myStack *stack, int *point, int *arc, int *position, int *adres, struct SortOut *sortRez, short **dist);
// так как программа может выполняться крайне долго
// для оценки времени выполнения программы в файл печатается
// содержимое стека (соотсетствующего дереву вариантов)
time_t printStackState(struct myStack stack, int *stapCnt);

/* === qweue === */
int* creatr_qweue(int n);
void push_qweue(int *qweue, int inf);
int pop_qweue(int *qweue);

/* === basic functions ===*/
// проверка возможности "протянуть" ребро к вершине point
int arcFree(struct Graph inp, int point);
struct Graph CreateGraph(struct CreateGraphInp inp);
void graphPrint(struct Graph inp);
// реализация алгоритма поиска в ширину для нахождения 
// расстояния от вершины point до всех остальных
short* dist(struct Graph graph, int point);
// сортировка вершин по расстоянию 
struct SortOut sortNgrow(short* dist,struct Graph graph, unsigned int maxL);
// проверяет, существует ли вершина point в графе
bool pointExist(struct Graph graph, int point);
struct Graph readGraphFile(char *filname);
void writeGraphFile(struct Graph inp, double time, char *filname);
// Удаляет изоморфные варианты соеденения вершин (применяется
// только на верхних уровнях дерева вариантов)
struct SortOut del_isomophism(struct SortOut inp, struct Graph graph, int workPoint);

/* === back code === */
// альтернативный подход к адресации вершин графа
// (древовидной структуры) применяется для удобства
// удаления вершин
backCode createAdr(struct Graph graph, int deep);
int decode(struct Graph graph, backCode code);
void add(backCode code, int addend, bool fixed, int pos);

/* === del Points === */
// переюирает колличество и расположение удаляемых вершин
// после чего соединяет оставшиеся вершины
struct Graph dellAndConnect(struct CreateGraphInp createIn, int maxDelCnt);
int* dellPoint(struct Graph graph, int point);
void restorePoint(struct Graph graph, int adr, int* point);
struct ConnectGraphOut dellPoints(struct Graph inp, backCode code, int count);

/* === connect graph === */
// рекурсивная функция соеденеиня вершин
struct ConnectGraphOut connectGraph(struct ConnectGraphInp inp);
// не рекурсивня (а основанная на стеке) функция соеденения вершин
struct ConnectGraphOut connectGraphStack(struct Graph *graph, unsigned int bestL);

/* === first step === */
// первый этап работы программы связан с поиском оптимального
// обхвата графа, и нахождением промежуточных результатов 
unsigned int firstStep(unsigned int p, unsigned int diam);

/* === main step === */
// основной этап работы программы -- попытка построить граф
// Мура, а вслучае неудачи -- вернуть наилучший предварительный
// результат
struct Graph mainStep(unsigned int p, unsigned int diam, unsigned int bestL, long *time);
#endif /* Header_h */
