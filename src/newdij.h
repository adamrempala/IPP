/** @file
 * Interfejs klasy wykonującej algorytm Dijkstry
 *
 * @author Adam Rempała <ar406309@students.mimuw.edu.pl>
 * @copyright Adam Rempała
 * @date 17.08.2019
 */

#include "heap.h"
#include "htab.h"

#ifndef NEWDIJ_H
#define NEWDIJ_H

/** @brief Kontrola różności nazw miast.
 * @param[in] city1  – wskaźnik na nazwę 1. miasta.
 * @param[in] city2  – wskaźnik na nazwę 2. miasta.
 * @return Wartość @p true, jeśli miasta są różne, @p false
 * w przeciwnym przypadku.
 */
bool sameCity(const char *city1, const char *city2);


/** @brief Znajduje najkrótszą drogę od miasta do krańca drogi krajowej.
 * Za pomocą algorytmu Dijkstry wyszukuje najkrótszą drogę (w przypadku
 * konfliktu tę, która ma nowszy najstarszy względem ostatniego remontu
 * lub budowy odcinek) od miasta wskazanego do początku lub końca drogi, o ile
 * rozszerzenie drogi krajowej jestmożliwe i da się to zrobić jednoznacznie.
 * @param[in] hash      – wskaźnik na hashmapę
 * @param[in] city1     – wskaźnik na nazwę miasta
 * @param[in] forbidden – wskaźnik na drogę krajową, która ma być rozszerzona.
 * @return Wskaźnik na strukturę przechowującą opis najlepszej drogi lub NULL,
 * jeśli nie da sięjej jednoznacznie określić lub nie udało się zaalokować
 * pamięci.
 */
HeapCard doubleDijkstra(Hash hash, const char *city1, Route forbidden);

/** @brief Znajduje najkrótszą drogę.
 * Za pomocą algorytmu Dijkstry wyszukuje najkrótszą drogę (w przypadku
 * konfliktu tę, która ma nowszy najstarszy względem ostatniego remontu
 * lub budowy odcinek) od pierwszego miasta do drugiego, o ile ustalenie takiej
 * drogi krajowej jest możliwe i da się to zrobić jednoznacznie. Funkcja używana
 * zarówno w przypadku tworzenia nowej drogi krajowej, jak i też zastąpienia
 * usuwanego odcinka innym fragmentem drogi (wówczas funkcja troszczy się o to,
 * żeby nie było pętli oraz samoprzecięć)
 * @param[in] hash      – wskaźnik na hashmapę
 * @param[in] city1     – wskaźnik na nazwę pierwszego miasta
 * @param[in] city2     – wskaźnik na nazwę drugiego miasta
 * @param[in] forbidden – wskaźnik na drogę krajową do nadpisania lub NULL.
 * @param[in] rmRoad    – czy mamy do czynienia z nadpisywaniem odcinka
 * @return Wskaźnik na strukturę przechowującą opis najlepszej drogi lub NULL,
 * jeśli nie da się jej jednoznacznie określić lub nie udało się zaalokować
 * pamięci.
 */
HeapCard dijkstra(Hash hash, const char *city1, const char *city2,
                  Route forbidden, bool rmRoad);

#endif
