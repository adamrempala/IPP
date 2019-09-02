/** @file
 * Implementacja modułu pomocniczego konstrukcji odcinka drogi
 *
 * @author Adam Rempała <ar406309@students.mimuw.edu.pl>
 * @copyright Adam Rempała
 * @date 28.08.2019
 */

#include <stdlib.h>
#include <malloc.h>

#include "rmake.h"

/** @brief Usuwa strukturę.
 * Usuwa strukturę ze wszystkimi danymi z nią związanymi, w szczególności
 * kolejnymi elementami stosu. Nic nie robi, jeśli mamy do czynienia z NULL-em.
 * @param[in] d – wskaźnik na usuwaną strukturę.
 */
void deleteData(Data d) {
    if (d != NULL) {
        deleteData(d->next);
        free(d->start);
        free(d->end);
        free(d);
    }
}

/** @brief Tworzy nową strukturę.
 * Tworzy nową pustą strukturę danych do budowy odcinkóœ drogi krajowej.
 * @return Wskaźnik na utworzoną strukturę lub NULL, gdy nie udało się
 * zaalokować pamięci.
 */
Quest newQuest() {
    Quest q = NULL;
    q = (Quest)malloc(sizeof(struct RouteData));
    
    if (q == NULL)
        return q;
    
    q->first = NULL;
    return q;
}

/** @brief Usuwa strukturę.
 * Usuwa strukturę ze wszystkimi danymi z nią związanymi. Nic nie robi,
 * jeśli mamy do czynienia z NULL-em.
 * @param[in] q – wskaźnik na usuwaną strukturę.
 */
void deleteQuest(Quest q) {
    deleteData(q->first);
    free(q);
}

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
             unsigned length, int year, bool toBuild) {
    Data d = NULL;
    d = (Data)malloc(sizeof(struct PathData));
    
    if (d == NULL)
        return false;
    
    d->start = NULL;
    d->end = NULL;
    d->start = strdup(start);
    d->end = strdup(end);
    
    if (d->start == NULL || d->end == NULL) {
        free(d->start);
        free(d->end);
        free(d);
        return false;
    }
    
    d->length = length;
    d->year = year;
    d->toBuild = toBuild;
    d->next = q->first;
    q->first = d;
    return true;
}

/** @brief Pobiera dane ze stosu.
 * Pobiera strukturę z danymi ze stosu, usuwając ją z jego szczytu.
 * @param[in,out] q – wskaźnik na zlecenie
 * @return Struktura z danymi lub NULL, jeśli stos jest pusty.
 */
Data popData(Quest q) {
    if (q->first == NULL)
        return NULL;
    
    Data d = q->first;
    q->first = q->first->next;
    d->next = NULL;
    return d;
}
