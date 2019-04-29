#ifndef DIJKSTRA_H
#define DIJKSTRA_h

#include "priqueue.h"
#include "road.h"
#include "hashtable.h"
Road *dijkstra(HashTable *h, const char* city1, const char* city2); //NULL uwala

Road *dijkstrart(HashTable *h, Road* route, const char* city);

Road *dijkstranewr(HashTable *h, Road* route, const char* city1, const char* city2);

#endif
