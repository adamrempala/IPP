#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <malloc.h>
#include <math.h>

#include "hashtable.h"
#include "prime.h"
#include "strdup.h"


NeighborList* neighborMatch(NeighborList *l, const char *n) {
    if (l == NULL) return NULL;
    if(!strcmp((l->name), n)) return l;
    return neighborMatch(l->next, n);
}

static void deleteNeighborList(NeighborList *l) {
    if (l == NULL) return;
	deleteNeighborList(l->next);
	free(l->name);
    free(l);
}

bool isNeighbor(NeighborList *l, const char *n) {
    if (l == NULL) return false;
    if(!strcmp((l->name), n)) return true;
    return isNeighbor(l->next, n);
}

void deleteNeighbor(NeighborList *n, const char *m){
    if ((n->next) == NULL) {
        deleteNeighborList(n);
        n = (NeighborList*)malloc(sizeof(NeighborList));
    }
    else if (strcmp((n->next->name), m)) deleteNeighbor(n->next, m);
    else if ((n->next->next) == NULL) deleteNeighborList(n->next);
    else {
        NeighborList *l1, *l2, *l3;
        l1 = n, l2 = n->next, l3 = n->next->next;
        l1->next = l3;
        l2->next = NULL;
        deleteNeighborList(l2);
    }
}

static HashElement *newElement(const char *k, NeighborList* value) {
    NeighborList *i = (NeighborList*)malloc(sizeof(NeighborList));
    if (i != NULL) {
        i->name = strdup(value->name);
        i->year = value->year;
        i->length= value->length;
        i->next = value->next;
    }
    HashElement *h = (HashElement*)malloc(sizeof(HashElement));
    if (h != NULL) {
        h->key = strdup(k);
        h->value = i;
    }
    return h;
}
static HashTable *newTableS(int size0) {
	HashTable* t = (HashTable*)malloc(sizeof(HashTable));
	t->size0 = size0;
	t->size = next_prime(t->size0);
	t->count = 0;
	t->elements = (HashElement**)calloc((size_t)t->size, sizeof(HashElement*));
	return t;
}

HashTable *newTable() {
    return newTableS(53);
}

static void deleteElement(HashElement *d) {

	if (d == NULL) return;
	free(d->key);
	deleteNeighborList(d->value);
	free(d);
}

void deleteTable(HashTable *t) {

	for (int i =0; i < t->size; i++) {
		HashElement* elem = t->elements[i];
		if(elem != NULL) {
			deleteElement(elem);
		}
	}
	free(t->elements);
	free(t);
}

static int hashing(const char *w, int a, int s) {

	long hash = 0;
	const int w_length = strlen(w);
	for (int i=0; i < w_length; i++) {
		hash += (long long)pow(a, w_length- (i+1)) * w[i];
		hash = hash % s;
	}
	return (int)hash;
}

static int getHash(const char *w, int buckets, int trial) {
	const int hash_1 = hashing(w, 151, buckets);
	const int hash_2 = hashing(w, 641, buckets);
	return (hash_1 + (trial * (hash_2 + 1))) % buckets;
}

static HashElement DELETED = {NULL, NULL};

static void hashReplace(HashTable *t, const char *key, NeighborList *value) {
	HashElement* elem = newElement(key, value);
	if (elem->key == NULL) {
	    deleteElement(elem);
	}
    int indic = getHash(elem->key, t->size, 0);
    HashElement* curry = t->elements[indic];
    int i = 1;
    while (curry != NULL) {
		if (curry != &DELETED) {
    			if (!strcmp(curry->key, key)) {
                		deleteElement(curry);
                		t->elements[indic] = elem;
                		return;
			}
		}
        	indic = getHash(elem->key, t->size, i);
        	curry = t->elements[indic];
        	i++;
    t->elements[indic] = elem;
    t->count++;
	}
}
bool addNeighbor(HashTable *t, const char *key, const char *name_, unsigned length_, int year_) {
	NeighborList* l, *m;
	m = (NeighborList*)malloc(sizeof(NeighborList));
	l = hashSearch(t, key);
	if (m == NULL) return false;
	if (l == NULL) {
		m->length = length_;
		m->name = (char*)name_;
		m->year = year_;
		hashReplace(t, key, m);
		return true;
	}
	l = (hashSearch(t, key));
	m->length = l->length;
	m->name = l->name;
	m->next = l->next;
	m->year = l->year;
	l->length = length_;
	l->name = strdup(name_);
	l->next = m;
	l->year = year_;
	hashReplace(t, key, l);
	return true;
}


static void resizeTable(HashTable *t, int size_) {
	if(size_ < 53) return;
	HashTable *newt = newTableS(size_);
	for(int i = 0; i < t->size; i++) {
		HashElement *elem = t->elements[i];
		if(elem != &DELETED && elem != NULL) {
			hashReplace(newt, elem->key, elem->value);
		}
	}
	t->size0 = newt->size0;
	t->count = newt->count;
	int tmp = t->size;
	t->size = newt->size;
	newt->size = tmp;
	HashElement **tmpe = t->elements;
	t->elements = newt->elements;
	newt->elements = tmpe;
	deleteTable(newt);
}

static void biggerTable(HashTable *t) {
	int size = t->size0 * 2;
	resizeTable(t, size);
}

void hashInsert(HashTable *t, const char *key) {
	if (t->count * 100 / t->size > 65) {
		biggerTable(t);
	}
	HashElement* elem = (HashElement*)malloc(sizeof(HashElement));
	elem->key = strdup(key);
	if (elem->key == NULL) {
	    deleteElement(elem);
	}
    int indic = getHash(elem->key, t->size, 0);

    HashElement* curry = t->elements[indic];
    int i = 1;
    while (curry != NULL) {
        indic = getHash(elem->key, t->size, i);
        curry = t->elements[indic];
        i++;
    }
    t->elements[indic] = elem;
    t->count++;
}

bool exists(HashTable *t, const char *key){
	if (hashSearch(t, key) == NULL) return false;
	return true;
}

NeighborList* hashSearch(HashTable *t, const char *key) {
    int indic = getHash(key, t->size, 0);
    HashElement *elem = t->elements[indic];
    int i = 1;
    while (elem != NULL) {
    	if (elem != &DELETED) {
    		if (!strcmp(elem->key, key)) {
			return elem->value;
        	}
	}
        indic = getHash(key, t->size, i);
        elem = t->elements[indic];
        i++;
    }
    return NULL;
}

void hashDelete(HashTable *t, const char *key) {
	int indic = getHash(key, t->size, 0);
    HashElement *elem = t->elements[indic];
    int i = 1;
    while (elem != NULL) {
    	if (elem != &DELETED) {
    		if (!strcmp(elem->key, key)) {
            	t->elements[indic] = &DELETED;
        	}
	}
    	indic = getHash(key, t->size, i);
        elem = t->elements[indic];
        i++;
    }
    (t->count)--;
}