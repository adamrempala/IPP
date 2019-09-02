/** @file
 * Implementacja klasy przechowującej drogi krajowe
 *
 * @author Adam Rempała <ar406309@students.mimuw.edu.pl>
 * @copyright Adam Rempała
 * @date 17.08.2019
 */

#include "htab.h"
#include "route.h"

/** @brief Zwraca liczbę nieujemną
 * Jeśli liczba nie jest typu unsigned, zwraca 0; w przeciwnym wypadku zwraca
 * wartość odpowiadającą napisowi symbolizujacemu liczbę.
 * @param[in] numberText    – wskaźnik na napis symbolizujący liczbę
 * @return Liczba dodatnia lub 0, jeśli liczba w napisie nie jest typu unsigned.
 */
unsigned correctUnsigned(char *numberText) {
    //Unsigned ma maksymalnie 10 cyfr
    //zmienna informuje nas, czy musimy się obawiać przekroczenia
    //wartości maksymalnej, jeśli liczba ma 10 cyfr
    bool noOverflow = false;
    
    //pomijamy zera wiodące
    while (*numberText == '0')
        numberText++;
    
    unsigned sigma = 0;
    char max[10] = "4294967295";
    
    if (strlen(numberText) > 10)
        return 0;
    else if (strlen(numberText) < 10) {
        for (unsigned i = 0; i < strlen(numberText); i++) {
            if (numberText[i] < 48 || numberText[i] > 57)
                return 0;
            
            sigma = 10 * sigma - 48 + numberText[i];
        }
        return sigma;
    }
    else {
        for (int i = 0; i < 10; i++) {
            if ((numberText[i] < 48 || numberText[i] > max[i]) && !noOverflow)
                return 0;
            
            sigma = 10 * sigma - 48 + numberText[i];
            
            if (numberText[i] < max[i])
                noOverflow = true;
        }
        return sigma;
    }
}

/** @brief Zwraca liczbę całkowitą (rok)
 * Jeśli liczba nie jest typu int, zwraca 0; w przeciwnym wypadku zwraca wartość
 * odpowiadającą napisowi symbolizujacemu liczbę.
 * @param[in] numberText    – wskaźnik na napis symbolizujący liczbę
 * @return Liczba całkowita lub 0, jeśli liczba w napisie nie jest typu int.
 */
int iYear(char *numberText) {
    //zmienne i działanie analogicznie jak w przypadku unsigneda
    //ze względu na różnice między modułem wartości minimalnej i maksymalnej
    //należy rozważyć dwa przypadki
    bool noOverflow = false;
    bool isPositive = true;
    
    if (*numberText == '-') {
        isPositive = false;
        numberText++;
    }
    
    while (*numberText == '0')
        numberText++;
    
    long long sigma = 0;
    char maxPositive[10] = "2147483647", maxNegative[10] = "2147483648";
    
    if (strlen(numberText) > 10)
        return 0;
    else if (strlen(numberText) < 10) {
        for (size_t i = 0; i < strlen(numberText); i++) {
            if (numberText[i] < 48 || numberText[i] > 57)
                return 0;
            
            sigma = 10 * sigma - 48 + numberText[i];
        }
        if (!isPositive)
            return (int)-sigma;
        
        return (int)sigma;
    }
    else if (isPositive){
        for (int i = 0; i < 10; i++) {
            if ((numberText[i] < 48 || numberText[i] > maxPositive[i]) && !noOverflow)
                return 0;
            
            sigma = 10 * sigma - 48 + numberText[i];
            
            if (numberText[i] < maxPositive[i]) noOverflow = true;
        }
        
        return (int)sigma;
    }
    else {
        for (int i = 0; i < 10; i++) {
            if ((numberText[i] < 48 || numberText[i] > maxNegative[i]) && !noOverflow)
                return 0;
            
            sigma = 10 * sigma - 48 + numberText[i];
            
            if (numberText[i] < maxNegative[i])
                noOverflow = true;
        }
        
        if (sigma > 0)
            return (int)-sigma;
        else return (int)sigma;
    }
}

/** @brief Długość napisu odpowiadającego liczbie.
 * @param[in] x – długość odcinka
 * @return Liczba nieujemna określająca długość napisu.
 */
int numberLength(long long x) {
    int length = 0;
    
    //przypadek liczby ujemnej
    if (x < 0) {
        length++;
        x = -x;
    }
    
    while (x > 0) {
        length++;
        x /= 10;
    }
    
    return length;
}

/** @brief Transkrypcja liczby na napis.
 * @param[in] x  – liczba.
 * @return Wskaźnik na napis odpowiadający liczbie.
 */
char *numberToChar(long long x) {
    int l = numberLength(x) + 1;
    char *str;
    str = malloc(l);
    
    if (str == NULL)
        return NULL;
    
    int i = l - 1;
    str[i] = '\0';
    
    if (x < 0) {
        str[0] = '-';
        x = -x;
    }
    
    while (x > 0) {
        i--;
        str[i] = x % 10 + '0';
        x /= 10;
    }
    
    return str;
}

/** @brief Tworzy nową strukturę.
 * Tworzy nową drogę krajową, zawierającą tylko jeden element –
 * miasto wskazane przez napis.
 * @param[in] name  – wskaźnik na nazwę pierwszego miasta drogi.
 * @return Wskaźnik na utworzoną strukturę lub NULL, gdy nie udało się
 * zaalokować pamięci.
 */
Route startRoute(const char* name) {
    Route r = NULL;
    r = (Route)malloc(sizeof(struct Path));
    
    if (r == NULL)
        return r;
    
    r->first = newStop(name);
    
    if (r->first == NULL) {
        free(r);
        return r;
    }
    
    r->last = r->first;
    return r;
}

/** @brief Tworzy nową strukturę.
 * Tworzy nowy element drogi krajowe, zawierającą tylko jeden element –
 * miasto wskazane przez napis.
 * @param[in] name  – wskaźnik na nazwę miasta.
 * @return Wskaźnik na utworzoną strukturę lub NULL, gdy nie udało się
 * zaalokować pamięci.
 */
Stop newStop(const char* name) {
    Stop s = NULL;
    s = (Stop)malloc(sizeof(struct RouteCity));
    
    if (s == NULL)
        return s;
    
    s->name = strdup(name);
    s->left = NULL;
    s->right = NULL;
    
    if (s->name == NULL) {
        free(s);
        return NULL;
    }
    
    return s;
}

/** @brief Znajduje miasto.
 * Szuka elementu tożsamego miastu w drodze krajowej, od wskazanego
 * elementu począwszy w prawo (rekurencyjnie).
 * @param[in] s         – wskaźnik na element drogi
 * @param[in] cityName  – wskaźnik na nazwę miasta.
 * @return Wskaźnik na element drogi tożsamy miastu lub NULL, jeśli nie udało
 * się go znaleźć.
 */
Stop findStop(Stop s, const char *cityName) {
    if (s == NULL)
        return s;
    
    if (!strcmp(s->name, cityName))
        return s;
    
    return findStop(s->right, cityName);
}


/** @brief Usuwa miasto.
 * Usuwa element tożsamy miastu niezależnie od drogi, do której
 * należy bądź nie należy.
 * @param[in] s – wskaźnik na element drogi.
 */
void deleteLoneStop(Stop s) {
    free(s->name);
    free(s);
}

/** @brief Dodaje miasto.
 * Dodaje do drogi krajowej miasto z prawej strony.
 * @param[in,out] r     – wskaźnik na drogę
 * @param[in] cityName  – nazwa miasta.
 * @return Wartość @p true, jeśli operacja przebiegła pomyślnie, @p false,
 * jeśli nie udało się zaalokować pamięci.
 */
bool addRight(Route r, const char *cityName) {

    Stop s = newStop(cityName);
    
    if (s == NULL)
        return false;
    
    r->last->right = s;
    s->left = r->last;
    r->last = s;

    return true;
}

/** @brief Usuwa miasto.
 * Usuwa ostatni element drogi krajowej.
 * @param[in,out] r – wskaźnik na drogę.
 */
void deleteRight(Route r) {
    if (r != NULL)
        if (r->last->left != NULL) {
            Stop s = r->last, sl = s->left;
            sl->right = NULL;
            r->last = sl;
            deleteLoneStop(s);
        }
}

/** @brief Dodaje miasto.
 * Dodaje do drogi krajowej miasto z lewej strony.
 * @param[in,out] r     – wskaźnik na drogę
 * @param[in] cityName  – nazwa miasta.
 * @return Wartość @p true, jeśli operacja przebiegła pomyślnie, @p false,
 * jeśli nie udało się zaalokować pamięci.
 */
bool addLeft(Route r, const char *cityName) {
    Stop s = newStop(cityName);
    
    if (s == NULL)
        return false;
    
    r->first->left = s;
    s->right = r->first;
    r->first = s;
    return true;
}

/** @brief Usuwa miasto.
 * Usuwa pierwszy element drogi krajowej.
 * @param[in,out] r – wskaźnik na drogę.
 */
void deleteLeft(Route r) {
    if (r != NULL)
        if (r->first->right != NULL) {
            Stop s = r->first, sr = s->right;
            sr->left = NULL;
            r->first = sr;
            deleteLoneStop(s);
        }
}

/** @brief Usuwa elementy drogi krajowej.
 * Usuwa element drogi oraz rekurencjnie wszystkie po prawej.
 * @param[in] s – wskaźnik na element drogi.
 */
void deleteAllPath(Stop s) {
    if (s != NULL) {
        deleteAllPath(s->right);
        free(s->name);
        free(s);
    }
}

/** @brief Usuwa drogę krajową.
 * Usuwa całą drogę krajową ze wszystkimi powiązanymi elementami.
 * @param[in] r – wskaźnik na drogę krajową.
 */
void deleteRoute(Route r) {
    if (r != NULL) {
        deleteAllPath(r->first);
        free(r);
        r = NULL;
    }
}

/** @brief Sprawdza przynależność miasta.
 * Szuka miasta w elementach drogi krajowej od podanego w prawo i daje znać, czy
 * miasto jest wśród tych elementów, czy nie.
 * @param[in] s – wskaźnik na element miasta.
 * @param[in] city – wskaźnik na nazwę szukanego miasta.
 * @return Wartość @p true, jeśli miasto jest tożsame z któryms elementem,
 * wartość @p false w przeciwnym przypadku.
 */
bool searcherForIsPart(Stop s, const char *city) {
    if (s == NULL)
        return false;
    
    if (!strcmp(city, s->name))
        return true;
    
    return searcherForIsPart(s->right, city);
}

/** @brief Sprawdza przynależność miasta.
 * Szuka miasta w drodze krajowej i daje znać, czy
 * miasto należy do drogi, czy nie.
 * @param[in] r – wskaźnik na drogę krajową.
 * @param[in] city – wskaźnik na miasto.
 * @return Wartość @p true, jeśli miasto należy do drogi krajowej,
 * wartość @p false w przeciwnym przypadku.
 */
bool isPart(Route r, const char *city) {
    if (r == NULL)
        return false;
    
    return searcherForIsPart(r->first, city);
}

/** @brief Łączy dwie drogi krajowe.
 * Złącza dwie drogi krajowe ze sobą w podanej kolejności i czyści wskaźnik
 * drugiej. Wobec tego pierwsza staje się drogą połączoną z obu. Nie robi nic,
 * jeśli nazwy ostatniego miasta pierwszej drogi i pierwszego drugiej są różne.
 * @param[in,out] r1 – wskaźnik na pierwszą drogę krajową.
 * @param[in] r2     – wskaźnik na drugą drogę krajową
 * @return Wskaźnik na połączoną drogę lub NULL, jeśli drogi nie mogą zostać
 * połączone z powodu różnych nazw krańcowych miast.
 */
Route merge2Routes(Route r1, Route r2) {
    if (strcmp(r1->last->name, r2->first->name))
        return NULL;
    
    Stop p = r2->first->right;
    r1->last->right = p;
    p->left = r1->last;
    free(r2->first->name);
    free(r2->first);
    r1->last = r2->last;
    
    //r2 zostaje zwolnione. Nie wolno już się do niego odwoływać
    free(r2);
    return r1;
}

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
Route replaceRoute(Route r1, Route overwriting, const char *city) {
    //przypadek drogi jednoodcinkowej
    if (!strcmp(r1->first->name, overwriting->first->name)
                 && !strcmp(r1->last->name, overwriting->last->name)) {
        deleteRoute(r1);
        r1 = overwriting;
        return r1;
    }
    //przypadek, gdy zastępowany jest odcinek początkowy
    else if (!strcmp(r1->first->name, overwriting->first->name)) {
        r1->first->right->left = overwriting->last->left;
        overwriting->last->left->right = r1->first->right;
        deleteLoneStop(overwriting->last);
        deleteLoneStop(r1->first);
        r1->first = overwriting->first;
        free(overwriting);
        return r1;
    }
    //przypadek, gdy zastępowany jest odcinek końcowy
    else if (!strcmp(r1->last->name, overwriting->last->name)) {
        r1->last->left->right = overwriting->first->right;
        overwriting->first->right->left = r1->last->left;
        deleteLoneStop(overwriting->first);
        deleteLoneStop(r1->last);
        r1->last = overwriting->last;
        free(overwriting);
        return r1;
    }
    //przypadek, gdy zastępowany jest któryś z odcinków środkowych
    Stop s = findStop(r1->first, city), sn = s->right;
    s->right = overwriting->first->right;
    overwriting->first->right->left = s;
    sn->left = overwriting->last->left;
    overwriting->last->left->right = sn;
    deleteLoneStop(overwriting->first);
    deleteLoneStop(overwriting->last);
    free(overwriting);
    return r1;
}

/** @brief Sprawdza, czy podany odcinek należy do odcinka drogi krajowej.
 * Sprawdza, czy odcinek między dwoma miastami (w kolejności ich podania) należy
 * do odcinka drogi krajowej od wskazanego elementu w prawo.
 * @param[in] s         – wskaźnik na element drogi krajowej.
 * @param[in] first     – wskaźnik na nazwę pierwszego miasta.
 * @param[in] second    – wskaźnik na nazwę drugiego miasta.
 * @return Wartość @p true, gdy odcinek istnieje, wartość @p false
 * w przeciwnym przypadku.
 */
bool existsPath(Stop s, const char *first, const char *second) {
    if (s == NULL)
        return false;
    
    if (!strcmp(first, s->name)) {
        if (s->right == NULL)
            return false;
        
        if (!strcmp(second, s->right->name))
            return true;
            
        return false;
    }
    
    return existsPath(s->right, first, second);
}

/** @brief Sprawdza, czy miasto jest na skraju drogi krajowej.
 * @param[in] r         – wskaźnik na drogę krajową.
 * @param[in] city      – wskaźnik na nazwę  miasta.
 * @return Wartość @p true, gdy miasto jest początkiem lub końcem drogi
 * krajowej, wartość @p false w przeciwnym przypadku.
 */
bool isBeginningOrEnd(Route r, const char *city) {
    if (r == NULL)
        return false;
    
    else
        return (!strcmp(r->first->name, city) || !strcmp(r->last->name, city));

}

/** @brief Sprawdza, czy droga krajowa zawiera mniej niż 3 miasta.
 * @param[in] r         – wskaźnik na drogę krajową.
 * @return Wartość @p true, gdy droga krajowa zawiera mniej niż 3 miasta,
 * wartość @p false w przeciwnym przypadku.
 */
bool singleRoad(Route r) {
    if (r == NULL)
        return true;
    
    if (r->first == NULL)
        return true;
    
    if (r->first->right == NULL)
        return true;
    
    if (r->first->right->right == NULL)
        return true;
    
    return false;
}

/** @brief Kopiuje drogę krajową.
 * Tworzy identyczną drogę krajową.
 * @param[in] r         – wskaźnik na drogę krajową.
 * @return Wskaźnik na nową drogę krajową lub NULL, jeśli nie udało się
 * zaalokować pamięci.
 */
Route copyRoute(Route r) {
    Stop s = r->first;
    Route copy = startRoute(s->name);
    
    if (copy == NULL)
        return NULL;
    
    s = s->right;
    
    while (s != NULL) {
        if (!addRight(copy, s->name)) {
            deleteRoute(copy);
            return NULL;
        }
        
        s = s->right;
    }
    
    return copy;
}

/** @brief Kopiuje lustrzanie drogę krajową.
 * Tworzy lustrzane odbicie drogi krajowej.
 * @param[in] r         – wskaźnik na wskaźnik na drogę krajową.
 * @return Wskaźnik na lustrzane odbicie drogi krajoewj lub NULL, jeśli nie
 * udało się zaalokować pamięci.
 */
Route revRoute(Route r) {
    Stop s = r->first;
    Route rev = startRoute(s->name);
    
    if (rev == NULL)
        return NULL;
    
    s = s->right;
    
    while (s != NULL) {
        if (!addLeft(rev, s->name)) {
            deleteRoute(rev);
            return NULL;
        }
        s = s->right;
    }
    
    return rev;
}
