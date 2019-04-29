/** @file
 * Interfejs drogi
 *
 * @author Adam Rempa�a <ar406309@students.mimuw.edu.pl>
 * @date 20.03.2019
 */

#ifndef ROAD_H
#define ROAD_H

#include <stdbool.h>

/**
 * Struktura przechowuj¹ca drog�. Zawiera imi�, odleg�o�� do nast�pnego elementu,
 * og�ln� odleg�o�� (tylko dla pierwszego elementu i tylko dla nowej drogi), rok remontu,
 * najstarszy rok dla drogi (tylko dla pierwszego elementu) oraz przekierowanie
 * na kolejny element.
 */
typedef struct Road{
    char* name;
    int distance;
    int generalDistance;
    int year;
    int oldestYear;
    struct Road* next;
} Road;

/** @brief Zapocz�tkowuje tworzenie drogi.
 * Tworzy now¹, pust¹ strukturê zawieraj�c� jedno miasto bez jakichkolwiek innych danych.
 * @param[in] name  – wskaŸnik na napis reprezentuj¹cy nazwê miasta;
 * @return WskaŸnik na utworzon¹ strukturê lub NULL, gdy nie uda³o siê
 * zaalokowaæ pamiêci.
 */

Road *startRoad(const char* name); //uwalamy, jak NULL

/** @brief Zapocz�tkowuje tworzenie drogi.
 * Tworzy now¹, pust¹ strukturê zawieraj�c� jedno miasto bez jakichkolwiek innych danych.
 * @param[in] city  – wskaŸnik na napis reprezentuj¹cy nazwê miasta;
 * @param[in] dist  – odleg�o�� do kolejnego miasta;
 * @param[in] yearBuilt  – wskaŸnik na napis reprezentuj¹cy nazwê miasta;
 * @return WskaŸnik na utworzon¹ strukturê lub NULL, gdy nie uda³o siê
 * zaalokowaæ pamiêci.
 */
Road *addPart(Road* r, const char* city, int dist, int yearBuilt); //uwalamy, jak nic sie nie zmienia

bool isPart(Road *r, const char* city);

bool isInnerPart(Road *r, const char* city);

void deleteRoad(Road *r);

Road* merge2Roads(Road *r1, Road *r2); //uwalamy, jak NULL

Road* revRoad(Road *r); //uwalamy, jak nic sie nie zmienia

char* roadBeginning(Road *r);

char* roadEnd(Road *r);

Road* copyRoad(Road *r); //uwala, jak jest NULL

bool isNeighborRoad(const char* city1, const char* city2, Road* r);

const char* first(const char* city1, const char* city2, Road *r);

void overwrite(Road* r1, Road* r2);

#endif
