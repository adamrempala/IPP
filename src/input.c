/** @file
 * Implementacja operacji wejścia
 *
 * @author Adam Rempała <ar406309@students.mimuw.edu.pl>
 * @copyright Adam Rempała
 * @date 17.08.2019
 */

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <stdbool.h>

#include "input.h"

/** @brief Wyjście awaryjne programu.
 * Zwalnia całą zalokowaną pamięć i zwraca wartość 0.
 * @param[in] map   – wskaźnik na mapę.
 * @param[in] param – wskaźnik na ewentualny napis.
 */
void alertInputExit(Map *map, char *param) {
    free(param);
    deleteMap(map);
    exit(0);
}

/** @brief Sprawdza, czy napis reprezentuje liczbę.
 * @param[in] str     – wskaźnik na napis
 * @return Wartość @p true, jeśli napis zawiera same cyfry, wartość @p false
 * w przeciwnym przypadku.
 */
bool isNumber(char *str) {
    if (strlen(str) == 0)
        return false;

    while (*str != '\0') {
        if (*str < 48 || *str > 57)
            return false;
        str++;
    }

    return true;
}
/** @brief Znajduje najbliższy średnik w linijce.
 * Szuka najbliższego średnika w linijce od pozycji indeksu i przypisuje
 * pozycję zmiennej. Informuje, gdy spodziewanego średnika nie ma. Zmienia też
 * wartość indeksu na pozycję najbliższego średnika.
 * @param[in] text          – wskaźnik na napis
 * @param[in,out] index     – wskaźnik na wartość pozycji indeksu
 * @param[in,out] searched  – wskaźnik na miejsce, gdzie ma być poz. średnika
 * @return Wartość @p true, jeśli udało się przypisać wartość @p false
 * w przeciwnym przypadku.
 */
bool findSemicolons(size_t *index, char *text, size_t *searched) {
    do
        *index = *index + 1;
    while (text[*index] != ';' && text[*index] != '\0');

    if (text[*index] != ';')
        return false;

    *searched = *index;
    return true;
}


/** @brief Kopiuje ze źródła do wskazanego miejsca.
 * Funkcja kopiuje fragment źródła od startu do końca włącznie do wskazanego
 * miejsca. W przypadku błędu alokacji pamięci zwalnia wszystko i zwraca 0.
 * @param[in,out] dir   – wskaźnik na wsk. na miejsce, gdzie będzie kopia napisu.
 * @param[in] src       – wskaźnik na źródło.
 * @param[in] map       – wskaźnik na mapę.
 * @param[in] start     – wartość startu.
 * @param[in] end       – wartość końca.
 * @param[in] p1        – ewentualny parametr do wyczyszczenia.
 * @param[in] p2        – ewentualny parametr do wyczyszczenia.
 * @param[in] p3        – ewentualny parametr do wyczyszczenia.
*/
void iCopy(char **dir, char *src, Map *map, size_t start, size_t end,
             char *p1, char *p2, char *p3) {
    *dir = malloc(end - start + 2);

    if (*dir == NULL) {
        free(p1);
        free(p2);
        free(p3);
        alertInputExit(map, src);
    }

    memcpy(*dir, &src[start], end - start + 1);
    char *place;
    place = *dir;
    place[end - start + 1] = '\0';
}

/** @brief Realokuje pamięć chara.
 * Jeśli pamięć we wskaźniku jest za mała na kolejny znak, realokuje ją.
 * @param[in,out] strSpace  – wskaźnik na wskaźnik na napis
 * @param[in] max           – wskaźnik na wartość maksymalnego rozmiaru napisu
 * @param[in] len           – wskaźnik na długość napisu
 * @return Wartość @p false, jeśli nie udało się zaalokować pamięci, wartość
 * @p true w przeciwnym wypadku.
 */
bool prepareForSign(char **strSpace, size_t *max, size_t *len) {
    if (*len + 1 >= *max) {
        *max = 2 * (*max) + 1;
        *strSpace = realloc(*strSpace, sizeof(char) * (*max));

        if (*strSpace == NULL)
            return false;
    }
    return true;
}

/** @brief Wypisuje ERROR z numerem linii
 * @param[in] lineID      – numer linii
 */
void printError(unsigned long long lineID) {
    fprintf(stderr, "ERROR %llu\n", lineID);
}

/** @brief Uruchamia @ref removeRoute.
 * Sprawdza, czy dane są poprawne; jeżeli tak, to uruchamia @ref removeRoute.
 * Jeśli nie, to wypisuje ERROR. Wypisuje ERROR również wtedy, gdy funkcja
 * kończy się niepowodzeniem.
 * @param[in] map               – wskaźnik na mapę
 * @param[in] text              – wskaźnik na linijkę
 * @param[in] textLength        – rozmiar linijki
 * @param[in] lineID            – numer linijki
 */
void performRemoveRoute(Map *map, char *text, size_t textLength,
                        unsigned long long lineID) {
    if (text[11] != ';')
        return printError(lineID);

    char *numb = NULL;
    iCopy(&numb, text, map, 12, textLength - 1, NULL, NULL, NULL);
    unsigned r = correctUnsigned(numb);
    free(numb);

    if (!removeRoute(map, r))
        return printError(lineID);
}

/***
 * Poniższe funkcje sprawdzają, czy linijka jest poprawna pod względem
 * składniowym, odpalają prawidłową funkcję i wypisują wyniki.
 * W funkcjach pojawiają się zmienne smcolx. Oznaczają one pozycję x-tego
 * średnika w linijce text (z wyłączeniem pierwszego).
 */

/** @brief Uruchamia @ref getRouteDescription.
 * Sprawdza, czy dane są poprawne; jeżeli tak, to uruchamia
 * @ref getRouteDescription. Jeśli nie, to wypisuje ERROR. Wypisuje ERROR
 * również wtedy, gdy funkcja kończy się niepowodzeniem. W przeciwnym razie
 * wypisuje wynik funkcji i zwalnia zaalokowaną pamięć.
 * @param[in] map           – wskaźnik na mapę
 * @param[in] text          – wskaźnik na linijkę
 * @param[in] textLength    – rozmiar linijki
 * @param[in] lineID        – numer linijki
 */
void performGetRouteDescription(Map *map, char *text,
                                size_t textLength, unsigned long long lineID) {
    if (text[19] != ';')
        return printError(lineID);

    char *numb = NULL;
    iCopy(&numb, text, map, 20, textLength - 1, NULL, NULL, NULL);

    if (!isNumber(numb)) {
        free(numb);
        return printError(lineID);
    }

    unsigned roadNumber = correctUnsigned(numb);

    // Jeżeli wynikiem correctUnsigned jest zero, trzeba jeszcze sprawdzić,
    // czy to zero wynika z podanego numeru „0”, czy też błędnego napisu.
    // W przypadku tego pierwszego wykonujemy funkcję, w przypadku drugiego
    // wypisujemy błąd.
    if (textLength > 20)
        if (roadNumber == 0 && numb[textLength - 21] != '0') {
            free(numb);
            return printError(lineID);
        }

    free(numb);
    const char *descr;
    descr = getRouteDescription(map, roadNumber);

    if (descr == NULL)
        return printError(lineID);

    printf("%s\n", descr);
    free((void*)descr);
}

/** @brief Uruchamia @ref addRoad.
 * Sprawdza, czy dane są poprawne; jeżeli tak, to uruchamia @ref addRoad.
 * Jeśli nie, to wypisuje ERROR. Wypisuje ERROR również wtedy, gdy funkcja
 * kończy się niepowodzeniem.
 * @param[in] map           – wskaźnik na mapę
 * @param[in] text          – wskaźnik na linijkę
 * @param[in] textLength    – rozmiar linijki
 * @param[in] lineID        – numer linijki
 */
void performAddRoad(Map *map, char *text,
                    size_t textLength, unsigned long long lineID) {
    if (text[7] != ';')
        return printError(lineID);

    char *city1 = NULL, *city2 = NULL, *length = NULL, *numb = NULL;
    size_t smcol1, smcol2, smcol3, index = 7;

    if (!findSemicolons(&index, text, &smcol1))
        return printError(lineID);

    if (!findSemicolons(&index, text, &smcol2))
        return printError(lineID);

    if (!findSemicolons(&index, text, &smcol3))
        return printError(lineID);


    iCopy(&city1, text, map, 8, smcol1 - 1, NULL, NULL, NULL);
    iCopy(&city2, text, map, smcol1 + 1, smcol2 - 1, city1, NULL, NULL);
    iCopy(&length, text, map, smcol2 + 1, smcol3 - 1, city1, city2, NULL);
    iCopy(&numb, text, map, smcol3 + 1, textLength - 1, city1, city2, length);

    unsigned dist = correctUnsigned(length);
    free(length);
    int yr = iYear(numb);
    free(numb);
    bool isOK = addRoad(map, city1, city2, dist, yr);
    free(city1);
    free(city2);

    if (!isOK)
        return printError(lineID);
}

/** @brief Uruchamia @ref removeRoad.
 * Sprawdza, czy dane są poprawne; jeżeli tak, to uruchamia @ref removeRoad.
 * Jeśli nie, to wypisuje ERROR. Wypisuje ERROR również wtedy, gdy funkcja
 * kończy się niepowodzeniem.
 * @param[in] map           – wskaźnik na mapę
 * @param[in] text          – wskaźnik na linijkę
 * @param[in] textLength    – rozmiar linijki
 * @param[in] lineID        – numer linijki
 */
void performRemoveRoad(Map *map, char *text, size_t textLength,
                       unsigned long long lineID) {
    if (text[10] != ';')
        return printError(lineID);

    char *city1 = NULL, *city2 = NULL;
    size_t smcol1, index = 10;

    if (!findSemicolons(&index, text, &smcol1))
        return printError(lineID);

    iCopy(&city1, text, map, 11, smcol1 - 1, NULL, NULL, NULL);
    iCopy(&city2, text, map, smcol1 + 1, textLength - 1, city1, NULL, NULL);

    bool isOK = removeRoad(map, city1, city2);
    free(city1);
    free(city2);

    if (!isOK)
        return printError(lineID);
}

/** @brief Uruchamia @ref repairRoad.
 * Sprawdza, czy dane są poprawne; jeżeli tak, to uruchamia @ref repairRoad.
 * Jeśli nie, to wypisuje ERROR. Wypisuje ERROR również wtedy, gdy funkcja
 * kończy się niepowodzeniem.
 * @param[in] map             – wskaźnik na mapę
 * @param[in] text            – wskaźnik na linijkę
 * @param[in] textLength      – rozmiar linijki
 * @param[in] lineID          – numer linijki
 */
void performRepairRoad(Map *map, char *text, size_t textLength,
                       unsigned long long lineID) {
    if (text[10] != ';')
        return printError(lineID);

    char *city1 = NULL, *city2 = NULL, *numb = NULL;
    size_t smcol1, smcol2, index = 10;

    if (!findSemicolons(&index, text, &smcol1))
        return printError(lineID);

    if (!findSemicolons(&index, text, &smcol2))
        return printError(lineID);

    iCopy(&city1, text, map, 11, smcol1 - 1, NULL, NULL, NULL);
    iCopy(&city2, text, map, smcol1 + 1, smcol2 - 1, city1, NULL, NULL);
    iCopy(&numb, text, map, smcol2 + 1, textLength - 1, city1, city2, NULL);

    int yr = iYear(numb);
    free(numb);
    bool isOK = repairRoad(map, city1, city2, yr);
    free(city1);
    free(city2);

    if (!isOK)
        return printError(lineID);
}

/** @brief Uruchamia @ref newRoute.
 * Sprawdza, czy dane są poprawne; jeżeli tak, to uruchamia @ref newRoute.
 * Jeśli nie, to wypisuje ERROR. Wypisuje ERROR również wtedy, gdy funkcja
 * kończy się niepowodzeniem.
 * @param[in] map           – wskaźnik na mapę
 * @param[in] text          – wskaźnik na linijkę
 * @param[in] textLength    – rozmiar linijki
 * @param[in] lineID        – numer linijki
 */
void performNewRoute(Map *map, char *text, size_t textLength,
                     unsigned long long lineID) {
    if (text[8] != ';')
        return printError(lineID);

    char *numb = NULL, *city1 = NULL, *city2 = NULL;
    size_t smcol1, smcol2, index = 8;

    if (!findSemicolons(&index, text, &smcol1))
        return printError(lineID);

    if (!findSemicolons(&index, text, &smcol2))
        return printError(lineID);

    iCopy(&numb, text, map, 9, smcol1 - 1, NULL, NULL, NULL);
    iCopy(&city1, text, map, smcol1 + 1, smcol2 - 1, numb, NULL, NULL);
    iCopy(&city2, text, map, smcol2 + 1, textLength - 1, numb, city1, NULL);

    unsigned road = correctUnsigned(numb);
    free(numb);
    bool isOK = newRoute(map, road, city1, city2);
    free(city1);
    free(city2);

    if (!isOK)
        return printError(lineID);
}

/** @brief Uruchamia @ref extendRoute.
 * Sprawdza, czy dane są poprawne; jeżeli tak, to uruchamia @ref extendRoute.
 * Jeśli nie, to wypisuje ERROR. Wypisuje ERROR również wtedy, gdy funkcja
 * kończy się niepowodzeniem.
 * @param[in] map           – wskaźnik na mapę
 * @param[in] text          – wskaźnik na linijkę
 * @param[in] textLength    – rozmiar linijki
 * @param[in] lineID        – numer linijki
 */
void performExtendRoute(Map *map, char *text, size_t textLength,
                        unsigned long long lineID) {
    if (text[11] != ';')
        return printError(lineID);

    char *numb = NULL, *city1 = NULL;
    size_t smcol1, index = 11;

    if (!findSemicolons(&index, text, &smcol1))
        return printError(lineID);

    iCopy(&numb, text, map, 12, smcol1 - 1, NULL, NULL, NULL);
    iCopy(&city1, text, map, smcol1 + 1, textLength - 1, numb, NULL, NULL);

    unsigned r = correctUnsigned(numb);
    free(numb);
    bool isOK = extendRoute(map, r, city1);
    free(city1);

    if (!isOK)
        return printError(lineID);
}

/** @brief Uruchamia @ref makeANewRoute.
 * Sprawdza, czy dane są poprawne; jeżeli tak, to uruchamia @ref makeANewRoute.
 * Jeśli nie, to wypisuje ERROR. Wypisuje ERROR również wtedy, gdy funkcja
 * kończy się niepowodzeniem.
 * @param[in] map           – wskaźnik na mapę
 * @param[in] text          – wskaźnik na linijkę
 * @param[in] textLength    – rozmiar linijki
 * @param[in] lineID        – numer linijki
 */
void performMakeANewRoute(Map *map, char *text, size_t textLength,
                          unsigned long long lineID) {
    char *numb = NULL, *desc = NULL;
    size_t smcol1 = 0;

    while (text[smcol1] != ';' && text[smcol1] != '\0')
        smcol1++;

    if (text[smcol1] != ';')
        return printError(lineID);

    iCopy(&numb, text, map, 0, smcol1 - 1, NULL, NULL, NULL);
    iCopy(&desc, text, map, smcol1 + 1, textLength - 1, numb, NULL, NULL);

    unsigned road = correctUnsigned(numb);
    free(numb);
    bool isOK = makeANewRoute(map, road, desc);
    free(desc);

    if (!isOK)
        return printError(lineID);
}

/** @brief Input.
 * Wczytuje linijki i wypisuje wyniki i ewentualne błędy, w przypadku
 * niepowodzenia funkcji albo gdy dane sa niepoprawne. W przypadku
 * błędu alokacji pamięci na tym etapie zwalnia wszystko i zwraca 0.
 * @param[in,out] map   – wskaźnik na mapę.
 */
void input(Map *map) {
    if (map == NULL)
        exit(0);

    unsigned long long lineID = 0;

    while (true) {
        char *text = NULL;
        int reader;
        lineID++;
        reader = getchar();

        if (reader == EOF)
            break;

        // przypadek #
        if (reader == 35) {
            while (reader != 10 && reader != EOF) {
                reader = getchar();
            }

            if (reader == EOF)
                break;
        }
        else if (reader != 10) {
            size_t textLength = 0;
            size_t maxSize = 0; // wielkość zaalokowanej pamięci

            while (reader != 10 && reader != EOF) {
                // przygotowanie miejsca na znak, jeśli jest taka potrzeba
                if (!prepareForSign(&text, &maxSize, &textLength))
                    alertInputExit(map, text);

                text[textLength++] = (char)reader;
                reader = getchar();
            }

            if (!prepareForSign(&text, &maxSize, &textLength))
                    alertInputExit(map, text);

            text[textLength] = '\0';

            if (!strncmp(text, "removeRoute", 11))
                performRemoveRoute(map, text, textLength, lineID);
            else if (!strncmp(text, "getRouteDescription", 19))
                performGetRouteDescription(map, text, textLength, lineID);
            else if (!strncmp(text, "addRoad", 7))
                performAddRoad(map, text, textLength, lineID);
            else if (!strncmp(text, "removeRoad", 10))
                performRemoveRoad(map, text, textLength, lineID);
            else if (!strncmp(text, "repairRoad", 10))
                performRepairRoad(map, text, textLength, lineID);
            else if (!strncmp(text, "newRoute", 8))
                performNewRoute(map, text, textLength, lineID);
            else if (!strncmp(text, "extendRoute", 11))
                performExtendRoute(map, text, textLength, lineID);
            else if (*text >= 48 && *text <= 57)
                performMakeANewRoute(map, text, textLength, lineID);
            else
                printError(lineID);

            free(text);

            if (reader == EOF)
                return;
        }
    }
}
