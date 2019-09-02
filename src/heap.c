/** @file
 * Implementacja klasy przechowującej konstrukcję dróg krajowych
 *
 * @author Adam Rempała <ar406309@students.mimuw.edu.pl>
 * @copyright Adam Rempała
 * @date 17.08.2019
 */

#include "strdup.h"
#include "heap.h"

/***FUNKCJA POMOCNICZA***/

/** @brief Usuwa cały podstóg
 * Usuwa cały podstóg ze wszystkimi przyporządkowanymi elementami, w tym
 * konstrukcjami drogi.
 * @param[in] elem – wskaźnik na element na szczycie podstogu
 */
void deleteHeapEl(HeapEl elem) {
    if (elem != NULL) {
        deleteHeapEl(elem->l);
        deleteHeapEl(elem->r);
        deleteCard(elem->routeCard);
        free(elem);
    }
}

/***FUNKCJE GLOBALNE***/

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
bool bigger(HeapCard card1, HeapCard card2) {
    if (card1 != NULL && card2 == NULL)
        return false;
    
    if (card1 == NULL && card2 != NULL)
        return true;
    
    if (card1 == NULL && card2 == NULL)
        return false;
    
    if (card1->dist > card2->dist)
        return true;
    
    if (card1->dist < card2->dist)
        return false;
    
    if (card1->year < card2->year)
        return true;
    
    return false;
}

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
bool smaller(HeapCard card1, HeapCard card2) {
    if (card1 != NULL && card2 == NULL)
        return true;
    
    if (card1 == NULL && card2 != NULL)
        return false;
    
    if (card1 == NULL && card2 == NULL)
        return false;
    
    if (card1->dist < card2->dist)
        return true;
    
    if (card1->dist > card2->dist)
        return false;
    
    if (card1->year > card2->year)
        return true;
    
    return false;
}

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
                 unsigned int lengthSum, int oldestYr) {
    HeapCard newCard = NULL;
    newCard = (HeapCard)malloc(sizeof(struct VisitCard));
    
    if (newCard == NULL)
        return newCard;
    
    newCard->lastCity = NULL;
    newCard->lastCity = strdup(last);
    
    if (newCard->lastCity == NULL) {
        free(newCard);
        newCard = NULL;
        return newCard;
    }

    newCard->construction = route;
    
    if (newCard->construction == NULL) {
        free(newCard->lastCity);
        free(newCard);
        newCard = NULL;
        return newCard;
    }
    
    newCard->dist = lengthSum;
    newCard->year = oldestYr;
    return newCard;
}

/** @brief Usuwa strukturę konstruowanej drogi krajowej.
 * @param[in] card  – wskaźnik na strukturę konstrukcji drogi krajowej
 */
void deleteCard(HeapCard card) {
    if (card != NULL) {
        deleteRoute(card->construction);
        free(card->lastCity);
        free(card);
    }
}

/** @brief Zwraca konstrukcję drogi krajowej.
 * Zwraca wskaźnik na konstrukcję drogi krajowej.
 * @param[in] card – wskaźnik na strukturę konstrukcji drogi krajowej.
 * @return Wskaźnik na konstrukcję lub NULL, jeśli nie udało się
 * zaalokować pamięci.
 */
Route getDescription(HeapCard card) {
    return card->construction;
}

/** @brief Zwraca nazwę ostatniego miasta.
 * Zwraca wskaźnik na nazwę ostatniego miasta w konstruowanej drodze krajowej.
 * @param[in] card – wskaźnik na strukturę konstrukcji drogi krajowej
 * @return Wskaźnik na nazwę lub NULL, jeśli nie udao się zaalokować pamięci.
 */
char *getLastCity(HeapCard card) {
    return card->lastCity;
}

/** @brief Tworzy nową strukturę.
 * Tworzy nową, pustą strukturę niezawierającą żadnych elementów.
 * @return Wskaźnik na utworzoną strukturę lub NULL, gdy nie udało się
 * zaalokować pamięci.
 */
Heap newHeap() {
    Heap heap = NULL;
    heap = (Heap)malloc(sizeof(struct Priqueue));
    
    if (heap == NULL)
        return heap;
    
    heap->first = NULL;
    return heap;
}

/** @brief Pobiera rozmiar (pod)stogu.
 * @param[in] elem – wskaźnik na element stogu.
 * @return Wartość równa rozmiarowi stogu.
 */
unsigned getSize(HeapEl elem) {
    if (elem == NULL)
        return 0;
    
    return elem->size;
}

/** @brief Podpatrza element na szczycie stogu.
 * @param[in] heap – wskaźnik na stóg.
 * @return Wskaźnik na element na szczycie stogu lub NULL,
 * jeżeli stóg jest pusty.
 */
HeapCard getMax(Heap heap) {
    if (heap == NULL)
        return NULL;
    
    if (heap->first == NULL)
        return NULL;
    
    return heap->first->routeCard;
}

/** @brief Wrzuca element do (pod)stogu.
 * Jest to funkcja rekurencyjna, która dodaje element do stogu i podstogów tak,
 * żeby każdy ojciec miał konstruowaną drogę krótszą lub równą i niestarszą od
 * synów.
 * @param[in,out] elem – wskaźnik na wskaźnik na element stogu.
 * @param[in] card     – wskaźnik na strukturę konstrukcji
 * @return Wartość @p true, jeśli operacja przebiegła pomyślnie, wartość
 * @p false, jeśli nie udało się zaalokować pamięci.
 */
bool put(HeapEl *elem, HeapCard card) {
    //jeśli pod wskazanym adresem nie ma nic, wrzucamy do niego naszą strukturę
    if (*elem == NULL) {
        HeapEl newHeapEl = (HeapEl)malloc(sizeof(struct PriQueueEl));
        
        if (newHeapEl == NULL) {
            deleteCard(card);
            return false;
        }
        
        newHeapEl->routeCard = card;
        newHeapEl->l = NULL;
        newHeapEl->r = NULL;
        newHeapEl->size = 1;
        *elem = newHeapEl;
        return true;
    }
    else {
        HeapEl *hl = &(*elem)->l, *hr = &(*elem)->r;
        
        // dążymy do tego, by stóg był zbalansowany
        if (getSize(*hl) <= getSize(*hr)) {
            // zamiana, jeśli wrzucany element jest lepszy
            if (smaller(card, (*elem)->routeCard)) {
                HeapCard tmp = (*elem)->routeCard;
                (*elem)->routeCard = card;
                card = tmp;
            }
            
            (*elem)->size++;
            if (!put(hl, card))
                return false;
            
            return true;
        }
        else {
            // zamiana, jeśli wrzucany element jest lepszy
            if (smaller(card, (*elem)->routeCard)) {
                HeapCard tmp = (*elem)->routeCard;
                (*elem)->routeCard = card;
                card = tmp;
            }
            
            (*elem)->size++;
            
            // rekurencyjnie robi dalej
            if (!put(hr, card))
                return false;
            
            return true;
        }
    }
}

/** @brief Usuwa element ze szczytu (pod)stogu
 * Jest to funkcja rekurencyjna, która usuwa element ze szczytu (pod)stogu
 * i dokonuje korekt, żeby każdy ojciec miał konstruowaną drogę krótszą
 * lub równą i niestarszą od synów. Co ważne, nie czyści ona samego elementu
 * przyporządkowanego do elementu na szczycie stogu, bo jest on wcześniej
 * zawsze przechwycony przez @ref getMax.
 * @param[in,out] elem – wskaźnik na wskaźnik na element stogu.
 */
void removeMax(HeapEl *elem) {
    if (*elem != NULL) {
        (*elem)->size--;
        
        if ((*elem)->l == NULL && (*elem)->r == NULL) {
            free((*elem));
            *elem = NULL;
        }
        else if ((*elem)->r == NULL) {
            HeapEl e;
            e = (*elem)->l;
            free(*elem);
            *elem = e;
        }
        else if ((*elem)->l == NULL) {
            HeapEl e;
            e = (*elem)->r;
            free(*elem);
            *elem = e;
        }
        else {
            // odwrotnie analogicznie do put, wyciągamy do góry
            // element w większego podstogu
            HeapEl *hl = &(*elem)->l, *hr = &(*elem)->r;
            
            if (!bigger((*hl)->routeCard, (*hr)->routeCard)) {
                (*elem)->routeCard = (*hl)->routeCard;
                removeMax(hl);
            }
            else {
                (*elem)->routeCard = (*hr)->routeCard;
                removeMax(hr);
            }
        }
    }
}

/** @brief Usuwa cały stóg
 * Usuwa cały stóg ze wszystkimi przyporządkowanymi elementami,
 * w tym konstrukcjami drogi.
 * @param[in] heap – wskaźnik na stóg
 */
void deleteHeap(Heap heap) {
    deleteHeapEl(heap->first);
    free(heap);
    heap = NULL;
}
