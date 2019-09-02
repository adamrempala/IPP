/** @file
 * Implementacja klasy wykonującej algorytm Dijkstry
 *
 * @author Adam Rempała <ar406309@students.mimuw.edu.pl>
 * @copyright Adam Rempała
 * @date 17.08.2019
 */

#include <stdio.h>

#include "strdup.h"
#include "newdij.h"

/***FUNKCJE POMOCNICZE***/

/** @brief Ustala optymalnego kandydata.
 * W pojedynku biorą udział dwie konstrukcje – mistrz i pretendent. Lepszy
 * kandydat zostaje mistrzem, gorszy jest usuwany. W przypadku remisu
 * usuwany jest pretendent, a fakt, że mistrz nie wygrał ze wszystkimi,
 * odnotowany jest w boolu.
 * @param[in,out] best      – wskaźnik na wskaźnik na mistrza
 * @param[in] pretendent    – wskaźnik na pretendenta
 * @param[in,out] absolute  – wskaźnik na informację, czy mistrz wygrał ze
 * wszystkimi
 */
void duel(HeapCard pretendent, HeapCard *best, bool *absolute) {
    if (!bigger(pretendent, *best)) {
        if (smaller (pretendent, *best)) {
            *absolute = true;
            deleteCard(*best);
            *best = pretendent;
        }
        else {
            *absolute = false;
            deleteCard(pretendent);
        }
    }
    else {
        deleteCard(pretendent);
    }
}

/** @brief Tworzy nową strukturę i wrzuca na stóg.
 * Na podstawie opisu rozszerzenia tworzy nową strukturę konstrukcji
 * drogi krajowej i wrzuca ją na stóg.
 * @param[in] current       – wskaźnik na strukturę konstrukcji
 * @param[in] iter          – wskaźnik na strukturę z danymi połączenia
 * @param[in,out] card      – wskaźnik na nowo tworzoną strukturę
 * @param[in,out] heap      – wskaźnik na stóg
 * @return Wartość @p true, jeśli operacja przebiegła pomyślnie, wartość
 * @p false, jeśli nie udało się zaalokować pamięci.
 */
bool pushExtended(HeapCard current, CityNext iter, HeapCard card,
             Heap heap) {
    // oldestYear będzie wartością najstarszego odcinka konstrukcji
    int oldestYear = current->year;

    // która może być zaktualizowana
    if (oldestYear > iter->year || oldestYear == 0)
        oldestYear = iter->year;

    // kopiowanie i rozszerzenie konstrukcji drogi krajowej
    Route extended = copyRoute(current->construction);

    if (extended == NULL)
        return false;

    if (!addRight(extended, iter->cityName)) {
        deleteRoute(extended);
        return false;
    }

    // utworzenie nowej struktury konstrukcji i wrzucenie jej na stóg
    card = newCard(iter->cityName, extended,
                   current->dist + iter->length, oldestYear);

    if (card == NULL)
        return false;

    if (!put(&(heap->first), card)) {
        deleteCard(card);
        return false;
    }

    return true;
}

/** @brief Wyjście z Dijkstry.
 * Usuwa wszystkie dane utworzone przez funkcję i zwraca NULL.
 * @param[in] c1    – wskaźnik na strukturę konstrukcji
 * @param[in] c2    – wskaźnik na strukturę konstrukcji
 * @param[in] h     – wskaźnik na stóg
 * @param[in] v     – wskaźnik na listę miast
 * @return NULL
 */
HeapCard dijDeleter(HeapCard c1, HeapCard c2, Heap h, Cities v) {
    deleteCard(c1);
    deleteCard(c2);
    deleteHeap(h);
    deleteList(v);
    return NULL;
}

/***FUNKCJE GLOBALNE***/

/** @brief Kontrola różności nazw miast.
 * @param[in] city1  – wskaźnik na nazwę 1. miasta.
 * @param[in] city2  – wskaźnik na nazwę 2. miasta.
 * @return Wartość @p true, jeśli miasta są różne, @p false
 * w przeciwnym przypadku.
 */
bool sameCity(const char *city1, const char *city2) {
    if(!strcmp(city1, city2))
        return true;

    return false;
}

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
HeapCard doubleDijkstra(Hash hash, const char *city1, Route forbidden) {
    bool absolute = false; // czy najlepszy wynik jest bezwzględnie najlepszy
    Heap heap = newHeap();

    if (heap == NULL)
        return NULL;

    HeapCard card = NULL, best = NULL;
    card = newCard(city1, startRoute(city1), 0, 0); // wyjście Dijkstry

    if (card == NULL)
        return dijDeleter(NULL, NULL, heap, NULL);

    Cities visited = newList();

    if (visited == NULL)
        return dijDeleter(NULL, card, heap, NULL);

    if (!put(&(heap->first), card))
        return dijDeleter(NULL, NULL, heap, visited);

    while (getMax(heap) != NULL) {
        HeapCard current = getMax(heap); // element wzięty ze szczytu stogu
        removeMax(&heap->first);
        char *currentName = getLastCity(current);

        if (currentName == NULL)
            return dijDeleter(NULL, current, heap, visited);
        /*
         * W przypadku gdy koncem odcinka jest szukane miasto,
         * sprawdzamy, czy droga kraj. jest optymalna. Jesli nie zachodza
         * petle, robimy przeszukiwanie, o ile:
         * – przeszukiwanie od danego miasta nie było już robione wcześniej
         * – miasto należy do drogi krajowej, która jest poszerzana
         */
        if (isBeginningOrEnd(forbidden, currentName))
            duel(current, &best, &absolute);
        else if (!contains(visited->first, currentName)
                     && (!isPart(forbidden, currentName))) {
            //od miasta currentName jest to pierwsze i ostatnie przeszukanie
            addCity(visited, currentName, 0, 0);
            //bierze listę połączeń z hashmapy
            Cities linkList = searchHash(hash, currentName);

            if (linkList != NULL) {
                CityNext iterLinkedCity = linkList->first;

                // dla każdego połączenia
                while (iterLinkedCity != NULL) {
                    HeapCard card = NULL;

                    // wrzucamy zaktualizowaną konstrukcję na stóg
                    if (!pushExtended(current, iterLinkedCity, card, heap))
                        return dijDeleter(best, current, heap, visited);

                    iterLinkedCity = iterLinkedCity->next;
                }
            }
            deleteCard(current);
        }
        else deleteCard(current);
    }

    deleteList(visited);
    deleteHeap(heap);

    // sprawdzamy, czy najlepszy wariant ustalony jest jednoznacznie
    if (!absolute) {
        deleteCard(best);
        best = NULL;
    }
    return best;
}

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
             Route forbidden, bool rmRoad) {
    bool absolute = false; // czy najlepszy wynik jest bezwzględnie najlepszy
    Heap heap = newHeap();

    if (heap == NULL)
        return NULL;

    HeapCard card = NULL, best = NULL;
    card = newCard(city1, startRoute(city1), 0, 0); // wyjście Dijkstry

    if (card == NULL)
        return dijDeleter(NULL, NULL, heap, NULL);

    Cities visited = NULL;
    visited = newList();

    if (visited == NULL)
        return dijDeleter(NULL, card, heap, NULL);

    if (!put(&(heap->first), card))
        return dijDeleter(NULL, NULL, heap, visited);

    while (getMax(heap) != NULL) {
        HeapCard current = getMax(heap); // element wzięty ze szczytu stogu
        removeMax(&heap->first);
        char *currentName = NULL;
        currentName = getLastCity(current);

        if (currentName == NULL)
            return dijDeleter(NULL, current, heap, visited);

        /*
         * W przypadku gdy koncem odcinka jest szukane miasto,
         * sprawdzamy, czy droga kraj. jest optymalna. Jesli nie zachodza
         * petle, robimy przeszukiwanie, o ile:
         * – przeszukiwanie od danego miasta nie było już robione wcześniej
         * – miasto należy do drogi krajowej, do której ewentualnie tworzony
         *   jest objazd, chyba że jest to miasto początkowe
         */
        if (sameCity(currentName, city2))
            duel(current, &best, &absolute);
        else if (!contains(visited->first, currentName)
                && ((sameCity(currentName, city1))
                || !isPart(forbidden, currentName))) {
            //od miasta currentName jest to pierwsze i ostatnie przeszukanie
            addCity(visited, currentName, 0, 0);
            //bierze listę połączeń z hashmapy
            Cities linkList = searchHash(hash, currentName);

            if (linkList != NULL) {
                CityNext iterLinkedCity = linkList->first;

                // dla każdego połączenia, z zastrzeżeniem, że jeśli droga
                // jest nadpisywana, nie uzględniamy nadpisywanego połączenia
                while (iterLinkedCity != NULL) {
                    if (!rmRoad || !sameCity(currentName, city1)
                        || !sameCity(iterLinkedCity->cityName, city2)) {
                        HeapCard card = NULL;

                        // wrzucamy zaktualizowaną konstrukcję na stóg
                        if (!pushExtended(current, iterLinkedCity, card, heap))
                            return dijDeleter(best, current, heap, visited);
                    }
                    iterLinkedCity = iterLinkedCity->next;
                }
            }
            deleteCard(current);
        }
        else {
            deleteCard(current);
        }
    }
    deleteList(visited);
    deleteHeap(heap);

    // sprawdzamy, czy najlepszy wariant ustalony jest jednoznacznie
    if (!absolute) {
        deleteCard(best);
        best = NULL;
    }
    return best;
}
