/** @file
 * Interfejs klasy przechowującej drogi krajowe
 *
 * @author Adam Rempała <ar406309@students.mimuw.edu.pl>
 * @copyright Adam Rempała
 * @date 17.08.2019
 */

#include "cities.h"
#include "rmake.h"

#ifndef ROUTE_H
#define ROUTE_H

struct RouteCity;

/**
 * Wskaźnik na strukturę RouteCity.
 */
typedef struct RouteCity *Stop;

/**
 * Struktura przechowująca element drogi krajowej.
 */
struct RouteCity {
    char* name; ///< Nazwa miasta należącego do drogi krajowej
    Stop left; ///< Wskaźnik na strukurę miasta drogi krajowej po lewej
    Stop right; ///< Wskaźnik na strukurę miasta drogi krajowej po prawej
};

/**
 * Struktura przechowująca drogę krajową.
 */
struct Path {
    Stop first;///< Wskaźnik na strukurę pierwszego miasta drogi krajowej
    Stop last; ///< Wskaźnik na strukurę ostatniego miasta drogi krajowej
};

/**
 * Wskaźnik na strukturę Path.
 */
typedef struct Path* Route;

/** @brief Zwraca liczbę nieujemną.
 * Jeśli liczba nie jest typu unsigned, zwraca 0; w przeciwnym wypadku zwraca
 * wartość odpowiadającą napisowi symbolizujacemu liczbę.
 * @param[in] numberText    – wskaźnik na napis symbolizujący liczbę
 * @return Liczba dodatnia lub 0, jeśli liczba w napisie nie jest typu unsigned.
 */
unsigned correctUnsigned(char *numberText);

/** @brief Zwraca liczbę całkowitą (rok).
 * Jeśli liczba nie jest typu int, zwraca 0; w przeciwnym wypadku zwraca wartość
 * odpowiadającą napisowi symbolizujacemu liczbę.
 * @param[in] numberText    – wskaźnik na napis symbolizujący liczbę
 * @return Liczba całkowita lub 0, jeśli liczba w napisie nie jest typu int.
 */
int iYear(char *numberText);

/** @brief Długość napisu odpowiadającego liczbie.
 * @param[in] x – długość odcinka
 * @return Liczba nieujemna określająca długość napisu.
 */
int numberLength(long long x);

/** @brief Transkrypcja liczby na napis.
 * @param[in] x  – liczba.
 * @return Wskaźnik na napis odpowiadający liczbie.
 */
char *numberToChar(long long x);

/** @brief Tworzy nową strukturę.
 * Tworzy nową drogę krajową, zawierającą tylko jeden element –
 * miasto wskazane przez napis.
 * @param[in] name  – wskaźnik na nazwę pierwszego miasta drogi.
 * @return Wskaźnik na utworzoną strukturę lub NULL, gdy nie udało się
 * zaalokować pamięci.
 */
Route startRoute(const char* name); //uwalamy, jak NULL

/** @brief Tworzy nową strukturę.
 * Tworzy nowy element drogi krajowe, zawierającą tylko jeden element –
 * miasto wskazane przez napis.
 * @param[in] name  – wskaźnik na nazwę miasta.
 * @return Wskaźnik na utworzoną strukturę lub NULL, gdy nie udało się
 * zaalokować pamięci.
 */
Stop newStop(const char* name);

/** @brief Dodaje miasto.
 * Dodaje do drogi krajowej miasto z prawej strony.
 * @param[in,out] r     – wskaźnik na drogę
 * @param[in] cityName  – nazwa miasta.
 * @return Wartość @p true, jeśli operacja przebiegła pomyślnie, @p false,
 * jeśli nie udało się zaalokować pamięci.
 */
bool addRight(Route r, const char *city);

/** @brief Usuwa miasto.
 * Usuwa ostatni element drogi krajowej.
 * @param[in,out] r – wskaźnik na drogę.
 */
void deleteRight(Route r);

/** @brief Dodaje miasto.
 * Dodaje do drogi krajowej miasto z lewej strony.
 * @param[in,out] r     – wskaźnik na drogę
 * @param[in] cityName  – nazwa miasta.
 * @return Wartość @p true, jeśli operacja przebiegła pomyślnie, @p false,
 * jeśli nie udało się zaalokować pamięci.
 */
bool addLeft(Route r, const char *city);

/** @brief Usuwa miasto.
 * Usuwa pierwszy element drogi krajowej.
 * @param[in,out] r – wskaźnik na drogę.
 */
void deleteLeft(Route r);

/** @brief Usuwa drogę krajową.
 * Usuwa całą drogę krajową ze wszystkimi powiązanymi elementami.
 * @param[in] r – wskaźnik na drogę krajową.
 */
void deleteRoute(Route r);

/** @brief Sprawdza przynależność miasta.
 * Szuka miasta w drodze krajowej i daje znać, czy
 * miasto należy do drogi, czy nie.
 * @param[in] r – wskaźnik na drogę krajową.
 * @param[in] city – wskaźnik na miasto.
 * @return Wartość @p true, jeśli miasto należy do drogi krajowej,
 * wartość @p false w przeciwnym przypadku.
 */
bool isPart(Route r, const char *city);

/** @brief Łączy dwie drogi krajowe.
 * Złącza dwie drogi krajowe ze sobą w podanej kolejności i czyści wskaźnik
 * drugiej. Wobec tego pierwsza staje się drogą połączoną z obu. Nie robi nic,
 * jeśli nazwy ostatniego miasta pierwszej drogi i pierwszego drugiej są różne.
 * @param[in,out] r1 – wskaźnik na pierwszą drogę krajową.
 * @param[in] r2     – wskaźnik na drugą drogę krajową
 * @return Wskaźnik na połączoną drogę lub NULL, jeśli drogi
 * nie mogą zostać połączone z powodu różnych nazw krańcowych miast.
 */
Route merge2Routes(Route r1, Route r2);

/** @brief Zastępuje odcinek drogi krajowej jednym lub więcej.
 * Szuka miasta, które jest lewym miastem zastępowanego odcinka. Między to
 * miasto a miasto po prawej wkleja miasta z drogi drugiej. Czyści wskaźnik
 * drugiej i odtąd nie można się do niego odwoływać. Wobec tego pierwsza staje
 * się drogą połączoną z obu. Nie robi nic, jeśli nazwy miast w miejscach
 * sklejeń są różne.
 * @param[in,out] r1        – wskaźnik na pierwszą drogę krajową.
 * @param[in] overwriting   – wskaźnik na rozszerzenie drogi krajowej.
 * @param[in] city          – wskaźnik na lewe miasto zastępowanego odcinka.
 * @return Wskaźnik na połączoną drogę lub NULL, jeśli drogi nie mogą zostać
 * połączone z powodu różnych nazw krańcowych miast.
 */
Route replaceRoute(Route r1, Route overwriting, const char *city);

/** @brief Sprawdza, czy podany odcinek należy do odcinka drogi krajowej.
 * Sprawdza, czy odcinek między dwoma miastami (w kolejności ich podania) należy
 * do odcinka drogi krajowej od wskazanego elementu w prawo.
 * @param[in] s         – wskaźnik na element drogi krajowej.
 * @param[in] first     – wskaźnik na nazwę pierwszego miasta.
 * @param[in] second    – wskaźnik na nazwę drugiego miasta.
 * @return Wartość @p true, gdy odcinek istnieje, wartość @p false
 * w przeciwnym przypadku.
 */
bool existsPath(Stop s, const char *first, const char *second);

/** @brief Sprawdza, czy miasto jest na skraju drogi krajowej.
 * @param[in] r         – wskaźnik na drogę krajową.
 * @param[in] city      – wskaźnik na nazwę  miasta.
 * @return Wartość @p true, gdy miasto jest początkiem lub końcem drogi
 * krajowej, wartość @p false w przeciwnym przypadku.
 */
bool isBeginningOrEnd(Route r, const char *city);

/** @brief Sprawdza, czy droga krajowa zawiera mniej niż 3 miasta.
 * @param[in] r         – wskaźnik na drogę krajową.
 * @return Wartość @p true, gdy droga krajowa zawiera mniej niż 3 miasta,
 * wartość @p false w przeciwnym przypadku.
 */
bool singleRoad(Route r);

/** @brief Kopiuje drogę krajową.
 * Tworzy identyczną drogę krajową.
 * @param[in] r         – wskaźnik na drogę krajową.
 * @return Wskaźnik na nową drogę krajową lub NULL, jeśli nie udało się
 * zaalokować pamięci.
 */
Route copyRoute(Route r);

/** @brief Kopiuje lustrzanie drogę krajową.
 * Tworzy lustrzane odbicie drogi krajowej.
 * @param[in] r         – wskaźnik na wskaźnik na drogę krajową.
 * @return Wskaźnik na lustrzane odbicie drogi krajoewj lub NULL, jeśli nie
 * udało się zaalokować pamięci.
 */
Route revRoute(Route r);

#endif
