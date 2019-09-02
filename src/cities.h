/** @file
 * Interfejs klasy przechowującej listę połączeń z miastami
 *
 * @author Adam Rempała <ar406309@students.mimuw.edu.pl>
 * @copyright Adam Rempała
 * @date 17.08.2019
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "strdup.h"

#ifndef CITIES_H
#define CITIES_H

struct CityElts;

/**
 * Wskaźnik na strukturę CityElts.
 */
typedef struct CityElts* CityNext;

/**
 * Struktura przechowująca element listy połączeń do sąsiednich miast.
 */
struct CityElts {
    char* cityName; ///< Nazwa miasta, do którego prowadzi odcinek
    unsigned length; ///< Długość odcinka
    int year; ///< Rok ostatniego remontu lub budowy, jeśli remontu nie było
    CityNext next; ///< wskaźnik na następną strukturę połączenia
};

/**
 * Struktura wskazująca na pierwszy element listy połączeń do sąsiednich miast.
 */
struct CityList {
    CityNext first; ///< wskaźnik na pierwszą strukturę połączenia
};

/**
 * Wskaźnik na strukturę CityList.
 */
typedef struct CityList* Cities;


/** @brief Tworzy nową strukturę.
 * Tworzy nową strukturę zawierającą wskaźnik na NULL-a (pusta lista).
 * @return Wskaźnik na pustą listę lub NULL, jeśli nie udało się zaalokować
 * pamięci.
 */
Cities newList();

/** @brief Usuwa listę
 * Usuwa wszystko, co jest związane ze wskazaną listą.
 * @param[in] cityList    – wskaźnik na listę
 */
void deleteList(Cities cityList);

/** @brief Szukanie połączenia
 * Na podstawie nazwy miasta szuka połączenia we wszystkich elementach listy,
 * począwszy od wskazanego.
 * @param[in] cityElt   – wskaźnik na element listy
 * @param[in] city      – wskaźnik na nazwę szukanego miasta
 * @return Wskaźnik na szukane połączenie lub NULL, jeśli nie udało się go
 * znaleźć.
 */
CityNext findElt(CityNext cityElt, const char *city);

/** @brief Sprawdzanie, czy połączenie istnieje
 * Na podstawie nazwy miasta sprawdza, czy na liście, począwszy od podanego
 * elementu, istnieje połączenie do danego miasta.
 * @param[in] cityElt     – wskaźnik na element listy
 * @param[in] city  – wskaźnik na nazwę szukanego miasta
 * @return wartość @p true, jeśli istnieje, w przeciwnym wypadku @p false.
 */
bool contains(CityNext cityElt, const char *city);

/** @brief Dodanie połączenia do listy
 * Dodaje na początek listy połączenie.
 * @param[in,out] cityList  – wskaźnik na listę
 * @param[in] cityName      – wskaźnik na nazwę miasta, do którego prowadzi nowy odcinek
 * @param[in] length        – długość odicnka
 * @param[in] year          – rok ostatniego remontu lub budowy
 * @return Wartość @p true, jeśli udało się dodać miasto lub @p false, jeśli
 * nie udało się zaalokować pamięci.
 */
bool addCity(Cities cityList, const char *cityName, unsigned length, int year);

/** @brief Usuwa połączenie z listy
 * Usuwa z listy wskaźnik na połączenie, chyba że dany nie istnieje.
 * @param[in,out] cityList  – wskaźnik na listę.
 * @param[in] cityName      – wskaźnik na nazwę miasta, do którego prowadzi odcinek.
 * @return Wartość @p true, jeśli udało się usunąć wskaźnik na połączenie
 * lub @p false, jeśli wskaźnik nie istnieje.
 */
bool deleteCity(Cities cityList, const char *cityName);

/** @brief Długość odcinka
 * Podaje informację o długości odcinka lub 0, jeśli odcinek nie istnieje.
 * @param[in,out] cityList     – wskaźnik na listę
 * @param[in] city             – wskaźnik na nazwę miasta, do którego prowadzi odcinek
 * @return Liczba nieujemna opisująca długość odcinka lub 0, gdy odcinek
 * nie istnieje.
 */
unsigned getLength(Cities cityList, const char *city);

/** @brief Rok ostatniego remontu odcinka
 * Podaje informację o roku ostatniego remontu lub budowy odcinka lub
 * zwraca 0, jeśli odcinek nie istnieje.
 * @param[in,out] cityList     – wskaźnik na listę
 * @param[in] city             – wskaźnik na nazwę miasta, do którego prowadzi odcinek
 * @return Liczba niezerowa opisująca rok ostatniego remontu lub budowy odcinka
 * lub 0, gdy odcinek nie istnieje.
 */
int getYear(Cities cityList, const char *city);

#endif // CITIES_H
