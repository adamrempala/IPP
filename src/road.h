/** @file
 * Interfejs drogi
 *
 * @author Adam Rempa³a <ar406309@students.mimuw.edu.pl>
 * @date 20.03.2019
 */

#ifndef ROAD_H
#define ROAD_H

#include <stdbool.h>

/**
 * Struktura przechowujÂ¹ca drogê. Zawiera imiê, odleg³oœæ do nastêpnego elementu,
 * ogóln¹ odleg³oœæ (tylko dla pierwszego elementu i tylko dla nowej drogi), rok remontu,
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

/** @brief Zapocz¹tkowuje tworzenie drogi.
 * Tworzy nowÂ¹, pustÂ¹ strukturÃª zawieraj¹c¹ jedno miasto bez jakichkolwiek innych danych.
 * @param[in] name  â€“ wskaÅ¸nik na napis reprezentujÂ¹cy nazwÃª miasta;
 * @return WskaÅ¸nik na utworzonÂ¹ strukturÃª lub NULL, gdy nie udaÂ³o siÃª
 * zaalokowaÃ¦ pamiÃªci.
 */

Road *startRoad(const char* name); //uwalamy, jak NULL

/** @brief Zapocz¹tkowuje tworzenie drogi.
 * Tworzy nowÂ¹, pustÂ¹ strukturÃª zawieraj¹c¹ jedno miasto bez jakichkolwiek innych danych.
 * @param[in] city  â€“ wskaÅ¸nik na napis reprezentujÂ¹cy nazwÃª miasta;
 * @param[in] dist  â€“ odleg³oœæ do kolejnego miasta;
 * @param[in] yearBuilt  â€“ wskaÅ¸nik na napis reprezentujÂ¹cy nazwÃª miasta;
 * @return WskaÅ¸nik na utworzonÂ¹ strukturÃª lub NULL, gdy nie udaÂ³o siÃª
 * zaalokowaÃ¦ pamiÃªci.
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
