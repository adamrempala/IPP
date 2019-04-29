#include <stdbool.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>

#include "road.h"
#include "strdup.h"

Road *startRoad(const char* name) {
    Road *r = (Road*)malloc(sizeof(Road));
    if (r == NULL) return r;
    r->name = strdup(name);
    if (r->name == NULL) {
        free(r);
        return r;
    }
    r->generalDistance = 0;
    return r;
}
int older(int year, Road *r) {
    if(r->oldestYear == 0) return year;
    else {
        if(year < r->oldestYear) return year;
        else return r->oldestYear;
    }
}
Road *addPart(Road* r, const char* city, int dist, int yearBuilt) {
    Road *r1 = (Road*)malloc(sizeof(Road));
    if (r1 == NULL) return r;
    r1->name = strdup(city);
    if (r1-> name == NULL) {
        free(r1);
        return r;
    }
    r1->distance = dist;
    r1->generalDistance = dist + r->generalDistance;
    r1->year = yearBuilt;
    r1->oldestYear = older(yearBuilt, r);
    r1->next = r;
    return r1;
}

bool isPart(Road *r, const char* city) {
	while (r != NULL) {
		if (!strcmp(r->name, city)) return true;
	}
	return false;
}

bool isInnerPart(Road *r, const char* city) {
    if(r == NULL) return false;
    if(r->next == NULL) return false;
    if(r->next->next == NULL) return false;
    r = r->next;
    while (r->next->next != NULL) {
        if (!strcmp(r->name, city)) return true;
        r = r->next;
    }
    return false;
}
void deleteRoad(Road *r) {
    if(r != NULL) {
        deleteRoad(r->next);
        if (r->name != NULL) free(r->name);
        free(r);
    }
}

Road *revRoad(Road *r) {
    Road *s = startRoad(r->name), *t, *u;
    if(s == NULL) return r;
    t = r;
    while (t->next != NULL) {
        u = s;
        s = addPart(s, t->next->name, t->distance, t->year);
        if (u == s) {
            deleteRoad(s);
            return r;
        }
        t = t->next;
    }
    deleteRoad(r);
    return s;
}

char* roadBeginning(Road *r) {
    return r->name;
}

char* roadEnd(Road *r) {
    while(r->next != NULL) r = r->next;
    return r->name;
}

Road* merge2Roads(Road *r1, Road *r2) {
    Road *g, *u;
    g = startRoad(roadBeginning(r1));
    while (r1->next != NULL) {
        u = g;
        g = addPart(g, r1->next->name, r1->distance, r1->year);
        if (u == g) {
            deleteRoad(g);
            return NULL;
        }
        r1 = r1->next;
    }
    while (r2->next != NULL) {
        u = g;
        g = addPart(g, r2->next->name, r2->distance, r2->year);
        if (u == g) {
            deleteRoad(g);
            return NULL;
        }
        r2 = r2->next;
    }
    deleteRoad(r1);
    deleteRoad(r2);
    u = g;
    g = revRoad(g);
    if (u == g) {
        deleteRoad(g);
        return NULL;
    }
    return g;
}

Road* copyRoad(Road* r) {
    Road *s = (Road*)malloc(sizeof(Road)), *t;
    t = s;
    if (s == NULL) return s;
    *(s->name) = *(r->name);
    while (r->next != NULL) {
        s->distance = r->distance;
        s->generalDistance = r->generalDistance;
        s->year = r->year;
        s->oldestYear = r->oldestYear;
        s->next = (Road*)malloc(sizeof(Road));
        if (s->next == NULL) {
            deleteRoad(t);
            return NULL;
        }
        s = s->next;
        r = r->next;
        s->name = strdup(r->name);
        if (s->name == NULL) {
            deleteRoad(t);
            return NULL;
        }
    }
    return t;
}

bool isNeighborRoad(const char* city1, const char* city2, Road* r) {
    if (r == NULL) return false;
    while (r->next == NULL) {
        if (!strcmp(city1, r->name) && !strcmp(city2, r->next->name)) return true;
        if (!strcmp(city2, r->name) && !strcmp(city1, r->next->name)) return true;
    }
    return false;
}

const char* first(const char* city1, const char* city2, Road *r) {
    while (r != NULL) {
        if(!strcmp(city1, r->name)) return city1;
        if(!strcmp(city2, r->name)) return city2;
        r = r->next;
    }
    return NULL;
}

void overwrite(Road* r1, Road* r2) {
    Road *r, *rx, *rz;
    char* rnam;
    r = r1;
    int oy = r2->oldestYear;
    while (strcmp(r1->name, roadBeginning(r2))) r1 = r1->next;
    r1->distance = r2->distance;
    r1->oldestYear = r2->oldestYear;
    r->oldestYear = oy;
    rz = r1;
    rx = r1->next;
    r1->next = r2;
    rnam = roadEnd(r2);
    while (strcmp(r1->name, rnam)) r1 = r1->next;
    r1->distance = rx->distance;
    r1->oldestYear = r2->oldestYear;
    r1->next = rx->next;
    rx->next = NULL;
    deleteRoad(rz);
}
