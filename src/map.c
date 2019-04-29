#include <stdbool.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>

#include "map.h"
#include "hashtable.h"
#include "road.h"
#include "dijkstra.h"

char* addDigit(char x, char* s, unsigned *msi, size_t *j) {
    if(*(j)+1 >= *msi){
        *msi = 2*(*msi)+1;
        s = realloc (s, sizeof(char)*(*msi));
    }
    s[(*j)++] = x + 48;
    return s;
}

char* addNumber(unsigned x, char* s, unsigned* msi, size_t* j) {
    unsigned div = 1, counter = 0;
    while (x / div > 0) {
        div *= 10;
        counter++;
    }
    for(;counter > 0; counter--) {
        div /=10;
        s = addDigit((x/div)%10, s, msi, j);
    }
	return s;
}

Map* newMap(void) {
	Map *m = (Map*) malloc(sizeof(Map));
	m->bindings = newTable();
	return m;
}

void deleteMap(Map *map) {
	deleteTable(map->bindings);
	for (int i = 1; i < 1000; i++) deleteRoad(map->routes[i]);
	free(map);
}

bool properName(const char* name) {
	while (*name) {
		if ((*name < 32 && *name > 0) || *name == ';') return false;
		name++;
	}
	return true;
}
bool sameCity(const char *city1, const char *city2) {
	if(!strcmp(city1, city2)) return true;
	return false;
}
bool properYear(int year) {
	if (year != 0) return true;
	return false;
}

void newCity(Map *m, const char *city) {
    hashInsert (m->bindings, city);
}


bool addRoad(Map *map, const char *city1, const char *city2, unsigned length, int builtYear) {
	if (map == NULL) return false;
	HashTable* mb;
	mb = map->bindings;
	if (properName(city1) == false) return false;
	if (properName(city2) == false) return false;
	if (properYear(builtYear) == false) return false;
	if (sameCity(city1, city2)) return false;
	if (!exists(mb, city1)) hashInsert(mb, city1);
	if (!exists(mb, city2)) hashInsert(mb, city2);
	if (isNeighbor(hashSearch(mb, city1), city2)) return false;
	if (addNeighbor(mb, city1, city2, length, builtYear)) {
        if (!addNeighbor(mb, city2, city1, length, builtYear)) {
            deleteNeighbor(hashSearch(mb, city1), city2);
            return false;
        }
        else return true;
	}
	else return false;
}

bool repairRoad(Map *map, const char *city1, const char *city2, int repairYear) {
    HashTable *mb;
    if (map == NULL) return false;
    mb = map->bindings;
	if (properName(city1) == false) return false;
	if (properName(city2) == false) return false;
	if (properYear(repairYear) == false) return false;
	if (sameCity(city1, city2)) return false;
	if (!exists(mb, city1)) return false;
	if (!exists(mb, city2)) return false;
	if (!isNeighbor(hashSearch(mb, city1), city2)) return false;
	NeighborList *a, *b;
	a = neighborMatch(hashSearch(mb, city1), city2), b = neighborMatch(hashSearch(mb, city2), city1);
	if (repairYear < a->year || repairYear < b->year) return false;
	else {
        a->year = repairYear;
        b->year = repairYear;
	}
	return true;
}

bool newRoute(Map *map, unsigned routeId, const char *city1, const char *city2) {
    if (map == NULL) return false;
	if (properName(city1) == false) return false;
	if (properName(city2) == false) return false;
	if (sameCity(city1, city2)) return false;
	if (!exists(map->bindings, city1)) return false;
	if (!exists(map->bindings, city2)) return false;
	if (routeId < 1 || routeId > 999) return false;
	if (map->routes[routeId] != NULL) return false;
	Road *r;
	r = dijkstra(map->bindings, city1, city2);
	if(r == NULL) return false;
	map->routes[routeId] = r;
	return true;
}

bool extendRoute(Map *map, unsigned routeId, const char *city) {
    if (map == NULL) return false;
	if (!exists(map->bindings, city)) return false;
	if (routeId < 1 || routeId > 999) return false;
	Road* route;
	route = map->routes[routeId];
	if (route == NULL) return false;
	if (!strcmp(city, roadBeginning(route)) || !strcmp(city, roadEnd(route))) return false;
	Road *r;
	r = dijkstrart(map->bindings, route, city);
	if (r == NULL) return false;
	else free(route);
	route = r;
	return true;
}

bool removeRoad(Map* map, const char* city1, const char* city2) {
    if (map == NULL) return false;
    HashTable *mb;
    Road* mr[1000];
    mb = map->bindings;
    for(int i = 1; i < 1000; i++) mr[i] = map->routes[i];
    unsigned length = (hashSearch(mb, city1))->length;
    int year = (hashSearch(mb, city1))->year;
    if (!exists(mb, city1)) return false;
    if (!exists(mb, city2)) return false;
    if (!isNeighbor(hashSearch(mb, city1), city2)) return false;
    deleteNeighbor(hashSearch(mb, city1), city2);
    deleteNeighbor(hashSearch(mb, city2), city1);
    Road* newroutes[1000];
    for (int i = 1; i < 1000; i++) if (isNeighborRoad(city1, city2, mr[i])) {
        (newroutes[i]) = dijkstranewr(mb, map->routes[i], city1, city2);
        if (newroutes[i] == NULL) {
            if (addRoad(map, city1, city2, length, year)) {};
            for (int j = 1; j < 1000; j++) deleteRoad(newroutes[i]);
            return false;
        }
        overwrite((mr[i]), (newroutes[i]));
    }
    return true;
}

char const* getRouteDescription(Map *map, unsigned routeId) {
    if (map == NULL) return "";
    if (routeId < 1 || routeId > 1000) return "";
    char *s = "";
	if(((map->routes) + routeId) == NULL) return s;
	else {
        unsigned* msi = 0;
        size_t* j = 0;
        addNumber(routeId, s, msi, j);
        Road *r;
        r = map->routes[routeId];
        char *n;
        n = r->name;
        for(size_t i = 0; i < sizeof(n); i++) {
            s[(*j)++] = n[i];
        }
        while(r->next != NULL) {
            s[(*j)++] = ';';
            addNumber(r->distance, s, msi, j);
            addNumber(r->year, s, msi, j);
            s[(*j)++] = ';';
            char *n;
            n = (r->name);
            for(size_t i = 0; i < sizeof(n); i++) {
                s[(*j)++] = n[i];
            }
        }
        s[*j] = '\0';
	}
	return s;
}
