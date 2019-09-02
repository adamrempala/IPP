/** @file
 * Interfejs modułu pomocniczego konstrukcji odcinka drogi dla funkcji @ref makeANewRoute
 *
 * @author Adam Rempała <ar406309@students.mimuw.edu.pl>
 * @copyright Adam Rempała
 * @date 28.08.2019
 */

#include <stdbool.h>

#include "strdup.h"

#ifndef RMAKE_H
#define RMAKE_H



struct PathData;

/**
 * Wskaźnik na strukturę PathData.
 */
typedef struct PathData* Data;

/**
 * Struktura przechowująca dane odcinka.
 */
struct PathData {
    char *start; ///< wskaźnik na nazwę pierwszego miasta
    char *end; ///< wskaźnik na nazwę drugiego miasta
    unsigned length; ///< długość odcinka
    int year; ///< rok ostatniego remontu lub budowy
    Data next; ///< wskaźnik na kolejny element stosu
    bool toBuild; ///< informacja, czy odcinek jest do budowy, czy remontu
};

/**
 * Struktura przechowująca dane zleconej do zbudowania drogi krajowej.
 */
struct RouteData {
    Data first; ///< wskaźnik na pierwszą strukturę z danymi
};

/**
 * Wskaźnik na strukturę RouteData.
 */
typedef struct RouteData* Quest;

/** @brief Usuwa strukturę.
 * Usuwa strukturę ze wszystkimi danymi z nią związanymi, w szczególności
 * kolejnymi elementami stosu. Nic nie robi, jeśli mamy do czynienia z NULL-em.
 * @param[in] d – wskaźnik na usuwaną strukturę.
 */
void deleteData(Data d);

/** @brief Tworzy nową strukturę.
 * Tworzy nową pustą strukturę danych do budowy odcinkóœ drogi krajowej.
 * @return Wskaźnik na utworzoną strukturę lub NULL, gdy nie udało się
 * zaalokować pamięci.
 */
Quest newQuest();

/** @brief Usuwa strukturę.
 * Usuwa strukturę ze wszystkimi danymi z nią związanymi. Nic nie robi,
 * jeśli mamy do czynienia z NULL-em.
 * @param[in] q – wskaźnik na usuwaną strukturę.
 */
void deleteQuest(Quest q);

/** @brief Dorzuca dane do stosu.
 * Tworzy i ustawia strukturę z danymi na wskaźnik zlecenia, spychając kolejną
 * strukturę na drugie miejsce. ze wszystkimi danymi z nią związanymi,
 * w szczególności kolejnymi elementami stosu.
 * @param[in,out] q     – wskaźnik na zlecenie
 * @param[in] start     – wskaźnik na nazwę pierwszego miasta
 * @param[in] end       – wskaźnik na nazwę drugiego miasta
 * @param[in] length    – długość odcinka
 * @param[in] year      – rok ostatniego remontu lub budowy
 * @param[in] toBuild   – informacja, czy odcinek jest do budowy, czy remontu
 * @return Wartość @p true, jeśli operacja przebiegła pomyślnie, wartość
 * @p false, jeśli nie udało się zaalokować pamięci.
 */
bool pushData(Quest q, char *start, char *end,
             unsigned length, int year, bool toBuild);

/** @brief Pobiera dane ze stosu.
 * Pobiera strukturę z danymi ze stosu, usuwając ją z jego szczytu.
 * @param[in,out] q – wskaźnik na zlecenie
 * @return Struktura z danymi lub NULL, jeśli stos jest pusty.
 */
Data popData(Quest q);

#endif // STRDUP_H

