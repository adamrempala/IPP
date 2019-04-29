#ifndef PRIQUEUE_H
#define PRIQUEUE_H
#include <stdbool.h>
#include <string.h>
#include "road.h"
#include "hashtable.h"
/*
typedef struct Queue{
    HashElement* elem;
    unsigned priority;
    struct Queue* left;
    struct Queue* right;
} Queue;
*/
typedef struct Queue {
    char* elem;
    unsigned distanse;
    struct Queue *next;
    Road *road;
} Queue;

Queue *newQueue (void);
bool isEmpty(Queue *q);
void deleteQueue(Queue *q);
bool addToQueue(Queue *q, const char *h, unsigned dist, Road* road);
char* getMin(Queue *q);
unsigned getMinPrior(Queue *q);
Road* getMinRoad(Queue *q);
Road* removeMin(Queue *q);
Road* copyRoad(Road *r);
#endif
