#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "priqueue.h"
#include "road.h"
#include "strdup.h"

Queue *newQueue (){
    return NULL;
}

bool isEmpty(Queue *q) {
    return q;
}

void deleteQueue(Queue *q) {
    if(q != NULL) {
        deleteQueue(q->next);
        free(q->elem);
        deleteRoad(q->road);
        free(q);
    }
}

bool addToQueue(Queue *q, const char *h, unsigned dist, Road* road) {
    if (q == NULL) {
        q = (Queue*)malloc(sizeof(Queue));
        if(q == NULL) return false;
        q->elem = strdup(h);
        q->distanse = dist;
        q->road = road;
        return true;
    }
    else if (q->distanse >= dist) {
        Queue *q1 = (Queue*)malloc(sizeof(Queue));
        if(q1 == NULL) return false;
        q1->elem = q->elem;
        q1->distanse = q->distanse;
        q1->road = q->road;
        q1->next = q->next;
        q->elem = strdup(h);
        q->distanse = dist;
        q->next = q1;
        q->road = road;
        return true;
    }
    else if (q->next == NULL) {
        Queue *q1 = (Queue*)malloc(sizeof(Queue));
        if(q1 == NULL) return false;
        q1->elem = strdup(h);
        q1->distanse = dist;
        q1->road = road;
        q->next = q1;
        return true;
    }
    else if (q->next->distanse >= dist) {
        Queue *q1 = (Queue*)malloc(sizeof(Queue));
        if(q1 == NULL) return false;
        q1->elem = strdup(h);
        q1->distanse = dist;
        q1->next = q->next;
        q1->road = road;
        q->next = q1;
        return true;
    }
    else return addToQueue(q->next, h, dist, road);
}
char* getMin(Queue *q) {
    return q->elem;
}

unsigned getMinPrior(Queue *q){
    return q->distanse;
}
Road* getMinRoad(Queue *q) {
    return q->road;
}

Road* removeMin(Queue *q) {
    if(q->next == NULL){
        Road *r = q->road;
        free(q);
        return r;
    }
    else{
        Queue *q1;
        Road *r;
        q1 = q->next;
        r = q->road;
        free(q->elem);
        q->elem = strdup(q1->elem);
        q->next = q1->next;
        q->distanse = q1->distanse;
        q->road = q1->road;
        free(q1->elem);
        free(q1);
        return r;
    }
}
