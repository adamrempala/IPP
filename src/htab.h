/** @file
 * Interfejs klasy przechowującej hashmapę
 *
 * @author Adam Rempała <ar406309@students.mimuw.edu.pl>
 * @copyright Adam Rempała
 * @date 17.08.2019
 */


#include <malloc.h>
#include <math.h>

#include "cities.h"

#ifndef HASHTABLE_H
#define HASHTABLE_H

/**
 * Struktura przechowująca hasło z hashmapy.
 */
struct HashItem {
    char* key; ///< wskaźnik na napis – klucz
    Cities value; ///< wskaźnik na listę połączeń
};

/**
 * Wskaźnik na strukturę HashItem.
 */
typedef struct HashItem* Term;

/**
 * Struktura przechowująca hashmapę.
 */
struct Hashtable {
    unsigned base; ///< liczba bazowa, na podst. której tworzona jest tablica
    unsigned size; ///< rozmiar tablicy
    unsigned numberOfElements; ///< liczba elementów w tablicy
    Term* items; ///< tablica haseł
};

/**
 * Wskaźnik na strukturę Hashtable
 */
typedef struct Hashtable *Hash;


/** @brief Tworzy nową strukturę.
 * Tworzy nową strukturę niezawierającą żadnych haseł, ale mającą swój rozmiar.
 * @return Wskaźnik na utworzoną strukturę lub NULL, gdy nie udało się
 * zaalokować pamięci.
 */
Hash newTable();

/** @brief Usuwa strukturę.
 * Usuwa hashmapę,ze wszystkimi odniesieniami, które zawiera.
 * @param[in] table – wskaźnik na hashmapę.
 */
void deleteTable(Hash table);

/** @brief Wprowadzenie hasła do hashmapy.
 * Wprowadza do hashmapy hasło z listą połączeń.
 * @param[in,out] table – wskaźnik na hashmapę.
 * @param[in] key       – wskaźnik na klucz.
 * @param[in] value     – wskaźnik na liste połączeń.
 * @return Wartość @p true w przypadku pomyślnego przebiegu, wartość @p false,
 * jeśli nie udało się zalokować pamięci.
 */
bool insertHash(Hash table, const char *key, Cities value);

/** @brief Wyszukiwanie listy połączeń
 * Na podstawie klucza szuka w hashmapie listy połączeń z danym miastem.
 * @param[in] table – wskaźnik na hashmapę.
 * @param[in] key       – wskaźnik na klucz (miasto).
 * @return Wskaźnik na listę połączeń lub NULL, jeśli nie udało się
 * znaleźć hasła.
 */
Cities searchHash(Hash table, const char *key);

/** @brief Usuwa hasło
 * Usuwa hasło z hashmapy. Funkcja jest używana tylko w przypadku usuwania
 * całej tablicy, czyli @ref deleteTable. Funkcja nic nie zwraca.
 * @param[in,out] table – wskaźnik na hashmapę.
 * @param[in] key       – wskaźnik na klucz (miasto).
 */
void deleteHash(Hash table, const char *key);

#endif
