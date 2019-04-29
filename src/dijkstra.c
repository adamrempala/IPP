#include "dijkstra.h"

#include <stdlib.h>
#include <string.h>

Road* dijkstra(HashTable *h, const char* city1, const char* city2) {
    Road* rf, *ctrl;
    Queue *q = newQueue();
    Road *rs = startRoad(city2);
    if (rs == NULL) return rs;
    bool proper = false;
    addToQueue(q, city2, 0, rs);
    while (!isEmpty(q)) {
        char* s;
        unsigned gl = getMinPrior(q);
        Road* nRoad;
        s = getMin(q);
        nRoad = getMinRoad(q);
        NeighborList *l;
        l = hashSearch(h, s);
        if (!strcmp(s, city1)) {
            if(rf == NULL) {
                rf = nRoad;
                proper = true;
            }
            else if (rf->generalDistance > nRoad->generalDistance) {
                rf = nRoad;
                proper = true;
            }
            else if (rf->generalDistance == nRoad->generalDistance) {
                if (rf->oldestYear > nRoad->oldestYear) {
                    rf = nRoad;
                    proper = true;
                }
                else if (rf->oldestYear == nRoad->oldestYear) {
                    proper = false;
                }
            }
        }
        else while (l != NULL) {
            if(!isPart(q->road, l->name)) {
                nRoad = copyRoad(q->road);
                if (nRoad == NULL) {
                    deleteQueue(q);
                    return nRoad;
                }
                ctrl = nRoad;
                nRoad = addPart(nRoad, l->name, l->length, l->year);
                if (ctrl == nRoad) {
                    deleteQueue(q);
                    deleteRoad(nRoad);
                    return nRoad;
                }
                if(!addToQueue(q, l->name, gl + l->length, nRoad)) {
                    deleteQueue(q);
                    deleteRoad(nRoad);
                    return nRoad;
                }
            }
            l = l->next;
        }
        nRoad = removeMin(q);
        if (nRoad != rf) deleteRoad(nRoad);
    }
    if (proper == true) return rf;
    else{
        if(rf != NULL) deleteRoad(rf);
        return NULL;
    }
}

Road *dijkstrart(HashTable *h, Road* route, const char* city) {
    Road* rf, *ctrl, *u;
    Queue *q = newQueue();
    Road *rs;
    rs = startRoad(city);
    if (rs == NULL) return rs;
    bool proper = false;
    addToQueue(q, city, 0, rs);
    while (!isEmpty(q)) {
        char* s;
        unsigned gl = getMinPrior(q);
        Road* nRoad;
        s = getMin(q);
        nRoad = getMinRoad(q);
        NeighborList *l;
        l = hashSearch(h, s);
        if (!isInnerPart(route, s)){
            if (!strcmp(s, roadBeginning(route)) || !strcmp(s, roadEnd(route))) {
                if(rf == NULL) {
                    rf = nRoad;
                    proper = true;
                }
                else if (rf->generalDistance > nRoad->generalDistance) {
                    rf = nRoad;
                    proper = true;
                }
                else if (rf->generalDistance == nRoad->generalDistance) {
                    if (rf->oldestYear > nRoad->oldestYear) {
                        rf = nRoad;
                        proper = true;
                    }
                    else if (rf->oldestYear == nRoad->oldestYear) {
                        proper = false;
                    }
                }
            }
            else while (l != NULL) {
                if(!isPart(q->road, l->name)) {
                    nRoad = copyRoad(q->road);
                    if (nRoad == NULL) {
                        deleteQueue(q);
                        return nRoad;
                    }
                    ctrl = nRoad;
                    nRoad = addPart(nRoad, l->name, l->length, l->year);
                    if (ctrl == nRoad) {
                        deleteQueue(q);
                        deleteRoad(nRoad);
                        return nRoad;
                    }
                    if(!addToQueue(q, l->name, gl + l->length, nRoad)) {
                        deleteQueue(q);
                        deleteRoad(nRoad);
                        return nRoad;
                    }
                }
                l = l->next;
            }
        }
        nRoad = removeMin(q);
        if (nRoad != rf) deleteRoad(nRoad);
    }
    if (proper == true) {
        if (!strcmp(roadBeginning(rf), roadEnd(route))) {
            return merge2Roads(route, rf);
        }
        else if (!strcmp(roadBeginning(rf), roadBeginning(route))) {
            u = rf;
            rf = revRoad(rf);
            if (u == rf) {
                deleteRoad(rf);
                return NULL;
            }
            return merge2Roads(rf, route);
        }
    }
    else{
        if(rf != NULL) deleteRoad(rf);
        return NULL;
    }
}

Road* dijkstranewr(HashTable *h, Road* route, const char* city1, const char* city2) {
    Road* rf, *ctrl;
    Queue *q = newQueue();
    Road *rs = startRoad(city2);
    if (rs == NULL) return rs;
    bool proper = false;
    addToQueue(q, city2, 0, rs);
    while (!isEmpty(q)) {
        char* s;
        unsigned gl = getMinPrior(q);
        Road* nRoad;
        s = getMin(q);
        nRoad = getMinRoad(q);
        NeighborList *l;
        l = hashSearch(h, s);
        if ((!isInnerPart(route, s) && strcmp(s, roadBeginning(route)) && strcmp(s, roadEnd(route)))
             || (!strcmp(s, city1))) {
            if (!strcmp(s, city1)) {
                if(rf == NULL) {
                    rf = nRoad;
                    proper = true;
                }
                else if (rf->generalDistance > nRoad->generalDistance) {
                    rf = nRoad;
                    proper = true;
                }
                else if (rf->generalDistance == nRoad->generalDistance) {
                    if (rf->oldestYear > nRoad->oldestYear) {
                        rf = nRoad;
                        proper = true;
                    }
                    else if (rf->oldestYear == nRoad->oldestYear) {
                        proper = false;
                    }
                }
            }
            else while (l != NULL) {
                if(!isPart(q->road, l->name)) {
                    nRoad = copyRoad(q->road);
                    if (nRoad == NULL) {
                        deleteQueue(q);
                        return nRoad;
                    }
                    ctrl = nRoad;
                    nRoad = addPart(nRoad, l->name, l->length, l->year);
                    if (ctrl == nRoad) {
                        deleteQueue(q);
                        deleteRoad(nRoad);
                        return nRoad;
                    }
                    if(!addToQueue(q, l->name, gl + l->length, nRoad)) {
                        deleteQueue(q);
                        deleteRoad(nRoad);
                        return nRoad;
                    }
                }
                l = l->next;
            }
        }
        nRoad = removeMin(q);
        if (nRoad != rf) deleteRoad(nRoad);
    }
    if (proper == true) return rf;
    else{
        if(rf != NULL) deleteRoad(rf);
        return NULL;
    }
}
