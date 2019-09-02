/** @file
 * Interfejs klasy przechowującej konstrukcję dróg krajowych
 *
 * @author Adam Rempała <ar406309@students.mimuw.edu.pl>
 * @copyright Adam Rempała
 * @date 17.08.2019
 */

#include <stdlib.h>
#include <malloc.h>
#include <string.h>

#include "route.h"

#ifndef HEAP_H
#define HEAP_H

/**
 * Struktura przechowująca aktualny etap konstrukcji drogi
 * krajowej, używany w stogu.
 */
struct VisitCard {
    char *lastCity; ///< nazwa ostatniego miasta po prawej stronie opisu
    Route construction; ///< wskaźnik na konstrukcję drogi krajowej
    unsigned int dist; ///< długość konstrukcji
    int year; ///< najstarszy rok odcinka należącego do konstrukcji
};

/**
 * Wskaźnik na strukturę VisitCard
 */
typedef struct VisitCard* HeapCard;

/**
 * Struktura będąca elementem stogu.
 */
struct PriQueueEl;

/**
 * Wskaźnik na strukturę PriQueueEl.
 */
typedef struct PriQueueEl* HeapEl;

/**
 * Struktura będąca elementem stogu.
 */
struct PriQueueEl {
    HeapCard routeCard; ///< wskaźnik na strukturę konstrukcji drogi krajowej
    HeapEl l; ///< wskaźnik na lewego syna
    HeapEl r; ///< wskaźnik na prawego syna
    unsigned size; ///< rozmiar (pod)stogu
};

/**
 * Struktura będąca stogiem. W tym stogu na jego szczycie
 * jest element odpowiadając najkrótszej, ewentualnie najnowszej
 * z najkrótszych, konstrukcji drogi.
 */
struct Priqueue {
    HeapEl first; ///< wskaźnik na szczyt stogu
};

/**
 * Wskaźnik na strukturę Priqueue.
 */
typedef struct Priqueue* Heap;


/** @brief Sprawdza, czy pierwsza droga jest dłuższa.
 * Odpowiada na pytanie, czy druga struktura jest gorsza od pierwszej
 * (w tym,że jest NULLem; im mniejsza, tym lepsza, a gdy są równe,
 * decydujący jest rok).
 * @param[in] card1    – wskaźnik na 1. strukturę konstrukcji drogi krajowej.
 * @param[in] card2    – wskaźnik na 2. strukturę konstrukcji drogi krajowej.
 * @return Wartość @p true, jeśli pierwsza konstrukcja drogi krajowej jest
 * dłuższa lub najstarszy jej odcinek jest starszy, wartość @p false
 * w przeciwnym przypadku.
 */
bool bigger(HeapCard card1, HeapCard card2);

/** @brief Sprawdza, czy pierwsza droga jest krótsza.
 * Odpowiada na pytanie, czy druga struktura jest gorsza od pierwszej (w tym,
 * że jest NULLem; im mniejsza, tym lepsza, a gdy są równe, decydujący
 * jest rok).
 * @param[in] card1    – wskaźnik na 1. strukturę konstrukcji drogi krajowej.
 * @param[in] card2    – wskaźnik na 2. strukturę konstrukcji drogi krajowej.
 * @return Wartość @p true, jeśli pierwsza konstrukcja drogi krajowej jest
 * krótsza lub najstarszy jej odcinek jest nowszy, wartość @p false
 * w przeciwnym przypadku.
 */
bool smaller(HeapCard card1, HeapCard card2);

/** @brief Tworzy nową strukturę.
 * Tworzy nową strukturę, kopiując opis konstrukcji drogi krajowej, nazwę
 * ostatniego miasta i przyjmując parametry najstarszego roku oraz łącznej
 * długości odcinków.
 * @param[in] last      – wskaźnik na nazwę ostatniego miasta konstrukcji
 * @param[in] route     – wskaźnik na konstrukcję
 * @param[in] lengthSum – łączna długość odcinków konstruowanej drogi krajowej
 * @param[in] oldestYr  – najstarszy rok odcinka konstruowanej drogi krajowej
 * @return Wskaźnik na utworzoną strukturę lub NULL, gdy nie udało się
 * zaalokować pamięci.
 */
HeapCard newCard(const char *last, Route route,
                 unsigned int lengthSum, int oldestYr);

/** @brief Usuwa strukturę konstruowanej drogi krajowej.
 * @param[in] card  – wskaźnik na strukturę konstrukcji drogi krajowej
 */
void deleteCard(HeapCard card);

/** @brief Zwraca nazwę ostatniego miasta.
 * Zwraca wskaźnik na nazwę ostatniego miasta w konstruowanej drodze krajowej.
 * @param[in] card – wskaźnik na strukturę konstrukcji drogi krajowej
 * @return Wskaźnik na nazwę lub NULL, jeśli nie udao się zaalokować pamięci.
 */
char *getLastCity(HeapCard card);

/** @brief Zwraca konstrukcję drogi krajowej.
 * Zwraca wskaźnik na konstrukcję drogi krajowej.
 * @param[in] card – wskaźnik na strukturę konstrukcji drogi krajowej.
 * @return Wskaźnik na konstrukcję lub NULL, jeśli nie udało się
 * zaalokować pamięci.
 */
Route getDescription(HeapCard card);

/** @brief Tworzy nową strukturę.
 * Tworzy nową, pustą strukturę niezawierającą żadnych elementów.
 * @return Wskaźnik na utworzoną strukturę lub NULL, gdy nie udało się
 * zaalokować pamięci.
 */
Heap newHeap();

/** @brief Pobiera rozmiar (pod)stogu.
 * @param[in] elem – wskaźnik na element stogu.
 * @return Wartość równa rozmiarowi stogu.
 */
unsigned getSize(HeapEl elem);

/** @brief Podpatrza element na szczycie stogu.
 * @param[in] heap – wskaźnik na stóg.
 * @return Wskaźnik na element na szczycie stogu lub NULL,
 * jeżeli stóg jest pusty.
 */
HeapCard getMax(Heap heap);

/** @brief Wrzuca element do (pod)stogu.
 * Jest to funkcja rekurencyjna, która dodaje element do stogu i podstogów tak,
 * żeby każdy ojciec miał konstruowaną drogę krótszą lub równą i niestarszą od
 * synów.
 * @param[in,out] elem – wskaźnik na wskaźnik na element stogu.
 * @param[in] card     – wskaźnik na strukturę konstrukcji
 * @return Wartość @p true, jeśli operacja przebiegła pomyślnie, wartość
 * @p false, jeśli nie udało się zaalokować pamięci.
 */
bool put(HeapEl *elem, HeapCard card);

/** @brief Usuwa element ze szczytu (pod)stogu.
 * Jest to funkcja rekurencyjna, która usuwa element ze szczytu (pod)stogu
 * i dokonuje korekt, żeby każdy ojciec miał konstruowaną drogę krótszą
 * lub równą i niestarszą od synów. Co ważne, nie czyści ona samego elementu
 * przyporządkowanego do elementu na szczycie stogu, bo jest on wcześniej
 * zawsze przechwycony przez @ref getMax.
 * @param[in,out] elem – wskaźnik na wskaźnik na element stogu.
 */
void removeMax(HeapEl *elem);

/** @brief Usuwa cały stóg.
 * Usuwa cały stóg ze wszystkimi przyporządkowanymi elementami,
 * w tym konstrukcjami drogi.
 * @param[in] heap – wskaźnik na stóg
 */
void deleteHeap(Heap heap);

#endif
