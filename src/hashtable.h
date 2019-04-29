#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stdbool.h>

typedef struct NeighborList{
	char* name;
	unsigned length;
	int year;
	struct NeighborList* next;
} NeighborList;

typedef struct {
    char *key;
    NeighborList *value;
} HashElement;

typedef struct {
	int size0;
    int size;
    int count;
    HashElement **elements;
} HashTable;

HashTable *newTable();

void deleteTable(HashTable *t);

void hashInsert(HashTable *t, const char *key);

bool exists(HashTable *t, const char *key);

NeighborList* hashSearch(HashTable *t, const char *key);

void hashDelete(HashTable *t, const char *key);

void deleteNeighbor(NeighborList *n, const char *m);

bool addNeighbor(HashTable *t, const char *key, const char *name_, unsigned length_, int year_);

bool isNeighbor(NeighborList *l, const char *n);

NeighborList* neighborMatch(NeighborList *l, const char *n);

#endif
