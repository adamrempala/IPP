/** @file
 * Implementacja klasy przechowującej mapę dróg krajowych
 *
 * @author Adam Rempała <ar406309@students.mimuw.edu.pl>
 * @copyright Adam Rempała
 * @date 17.08.2019
 */

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>

#include "map.h"

/***FUNKCJE POMOCNICZE***/

/** @brief Usuwa elementy ze wskaźników.
 * Zwalnia pamięć wskazywaną przez wskaźniki.
 * @param[in] a – wskaźnik na napis
 * @param[in] b – wskaźnik na napis
 * @param[in] r – wskaźnik na drogę krajową
 * @param[in] q – wskaźnik na zlecenie drogi
 * @return Wartość @p false
 */
bool makingRouteFreer(char *a, char *b, Route r, Quest q) {
    free(a);
    free(b);
    deleteRoute(r);
    deleteQuest(q);
    return false;
}

/** @brief Kontrola poprawności nazwy miasta.
 * Sprawdza, czy nazwa miasta nie zawiera niedozwolonych znaków
 * oraz czy jest niepusta.
 * @param[in] name  – wskaźnik na nazwę miasta.
 * @return Wartość @p true, jeśli nazwa prawidłowa, @p false
 * w przeciwnym przypadku.
 */
bool properName(const char* name) {
    if (strlen(name) == 0)
        return false;

    int i = 0;

    while (name[i] != '\0') {
        if ((name[i] < 32 && name[i] > 0) || name[i] == ';')
            return false;

        i++;
    }
    return true;
}

/** @brief Sprawdza, czy podany rok istnieje.
 * @param[in] year        – numer roku.
 * @return Wartość @p true, jeśli numer roku jest różny od zera,
 * wartość @p false w przeciwnym wypadku.
 */
bool properYear(int year) {
    if (year != 0)
        return true;

    return false;
}

/** @brief Dopisuje do tekstu kolejne elementy opisu drogi.
 * Dopisuje do konstruowanego opisu drogi rekurencyjnie na podstawie jej
 * kolejnego elementu oraz danych o połączeniu z hashmapy. W każdym wywołaniu
 * dopisywany jest blok: średnik, długość, średnik, rok, średnik, nazwa
 * kolejnego miasta.
 * @param[in] map           – wskaźnik na strukturę przechowującą mapę dróg;
 * @param[in,out] text      – wskaźnik na wsk. na opis drogi;
 * @param[in] stop          – wskaźnik na strukturę elementu drogi krajowej
 * @param[in] charSize      – długość napisu w text
 * @param[in] maxSize       – rozmiar pamięci na napis
 * @return Wartość @p true, jeśli operacja przebiegła pomyślnie, wartość
 * @p false w przeciwnym przypadku.
 */
bool constructDescription(Map *map, char **text, Stop stop,
                          size_t charSize, size_t maxSize) {
    if (stop->right == NULL)
        return true;

    char *c1 = stop->name, *c2 = stop->right->name;
    Cities cities = searchHash(map->bindings, c2);
    CityNext way = findElt(cities->first, c1);
    // znalezione połączenie dostarczy informacje o długości i roku
    char *yearChar, *distanceChar;
    yearChar = numberToChar(way->year);
    distanceChar = numberToChar(way->length);

    if (yearChar == NULL || distanceChar == NULL) {
        free(yearChar);
        free(distanceChar);
        return false;
    }

    size_t yearSize = strlen(yearChar);
    size_t distSize = strlen(distanceChar);
    size_t citySize = strlen(c2);
    size_t charsToAdd = 3 + yearSize + distSize + citySize;

    // jeśli jest za mało miejsca, trzeba powiększyć
    while (charSize + charsToAdd + 1 >= maxSize) {
        maxSize *= 2;
        *text = realloc(*text, 2 * maxSize);
    }

    if (*text == NULL) {
        free(yearChar);
        free(distanceChar);
        return false;
    }

    // dopisanie bloku
    strcpy(&(*text)[charSize], ";");
    strcpy(&(*text)[charSize + 1], distanceChar);
    strcpy(&(*text)[charSize + distSize + 1], ";");
    strcpy(&(*text)[charSize + distSize + 2], yearChar);
    strcpy(&(*text)[charSize + distSize + yearSize + 2], ";");
    strcpy(&(*text)[charSize + distSize + yearSize + 3], c2);
    free(distanceChar);
    free(yearChar);
    charSize += 3 + yearSize + distSize + citySize;
    // rekurencja
    return constructDescription(map, text, stop->right, charSize, maxSize);
}

/** @brief Kopiuje ze źródła do wskazanego miejsca.
 * Funkcja kopiuje fragment źródła od startu do końca włącznie do wskazanego
 * miejsca. W przypadku błędu alokacji pamięci zwalnia wszystko, co utworzyła
 * funkcja, która ją wywołała, czyli makeANewRoute i zwraca false.
 * @param[in,out] dir   – wskaźnik na wskaźnik na miejsce kopii napisu.
 * @param[in] src       – wskaźnik na źródło.
 * @param[in] start     – pozycja, z której zaczynamy czytać napis.
 * @param[in] end       – pozycja, z której kończymy czytać napis.
 * @return Wartość @p true, jeśli operacja przebiegła pomyślnie,
 * wartość @p false, jeśli nie udało się zaalokować pamięci.
 */
bool mCopy(char **dir, char *src, size_t start, size_t end) {
    *dir = malloc(end - start + 2);

    if (*dir == NULL)
        return false;

    memcpy(*dir, &src[start], end - start + 1);
    char *place;
    place = *dir;
    place[end - start + 1] = '\0';
    return true;
}

/** @brief Wykonuje zlecenie.
 * Wykonuje operacje budowy lub naprawy dróg pod drogę krajową, zgodnie
 * ze strukturą.
 * @param[in] map          – wskaźnik na strukturę przechowującą mapę dróg;
 * @param[in,out] quest    – wskaźnik na zlecenie
 * @return Wartość @p true, gdy wszystkie operacje się udały, wartość @p false
 * w przeciwnym wypadku.
 */
bool performQuest(Map *map, Quest quest) {
    while (quest->first != NULL) {
        Data data = popData(quest);

        // przypadek budowy
        if (data->toBuild) {
            if (!addRoad(map, data->start, data->end, data->length, data->year))
                return false;
        }
        // przypadek naprawy
        else {
            if (!repairRoad(map, data->start, data->end, data->year))
                return false;
        }

        deleteData(data);
    }
    return true;
}

/** @brief Sprawdza możliwość budowy odcinka i dodaje go.
 * Pobiera z napisu potrzebne dane na temat odcinka (w tym przypisuje wskaźnikowi
 * end napis, który przekazuje do funkcji @ref makeANewRoute), po czym sprawdza,
 * czy odcinek istnieje. Jeśli tak, czy wymaga remontu oraz czy długość jest
 * prawidłowa, a rok ostatniego remontu wcześniejszy niż planowany. Jeśli nie,
 * przeznacza ją do budowy. Jeśli wszystkie dane są poprawne, dodaje miasto do
 * drogi krajowej, a jeżeli trzeba, dodaje odcinek do zlecenia.
 * @param[in,out] map   – wskaźnik na strukturę przechowującą mapę dróg;
 * @param[in] descr     – wskaźnik na opis drogi krajowej
 * @param[in,out] quest – wskaźnik na zlecenie
 * @param[in,out] r     – wskaźnik na drogę krajową
 * @param[in] start     – wskaźnik na nazwę miasta startowego
 * @param[in,out] end   – wskaźnik na wskaźnik na miejsce miasta docelowego
 * @param[in] lenPos    – pozycja pierwszego znaku długości w descr
 * @param[in] yrPos     – pozycja pierwszego znaku roku w descr
 * @param[in] c2Pos     – pozycja pierwszego znaku drugiego miasta w descr
 * @param[in] scanner   – aktualna pozycja przeszukiwania w @ref makeANewRoute
 * @return Wartość @p true, gdy wszystkie dane były poprawne, a operacje się
 * udały, wartość @p false w przypadku gdy któreś dane nie były poprawne, braku
 * zgodności co do długości odcinka w hashmapie i w opisie, gdy rok w opisie
 * jest wcześniejszy niż w hashmapie lub gdy nie udało się zaalokować pamięci.
 */
bool addToRouteAndQuest(Map *map, char *descr, Quest quest, Route r,
                        char *start, char **end, size_t lenPos, size_t yrPos,
                        size_t c2Pos, size_t scanner) {
    int year;
    unsigned length;
    char *lengthChar = NULL, *yearChar = NULL;

    // skopiowanie oraz konwersja potrzebnych danych
    if (!mCopy(&lengthChar, descr, lenPos, yrPos - 2))
        return false;

    length = correctUnsigned(lengthChar);
    free(lengthChar);

    if (!mCopy(&yearChar, descr, yrPos, c2Pos - 2))
        return false;

    year = iYear(yearChar);
    free(yearChar);

    if (!mCopy(end, descr, c2Pos, scanner - 1))
        return false;

    // jakakolwiek sprzeczność kończy funkcję falsem
    if (!properName(start) || !properName(*end) || !properYear(year)
                || !length || sameCity(start, *end) || isPart(r, *end))
        return false;

    // sprawdza, czy trzeba utworzyć lub naprawić drogę
    // jeśli tak, to odnotowuje to w zleceniu
    // jakakolwiek sprzeczność zamyka funkcję
    Cities linkList1 = searchHash(map->bindings, start);

    if (linkList1 != NULL) {
        if (contains(linkList1->first, *end)) {
            CityNext way = findElt(linkList1->first, *end);
            if (way->length != length || way->year > year)
                return false;

            else if (way->year < year)
                if (!pushData(quest, start, *end, length, year, false))
                    return false;
        }
        else if (!pushData(quest, start, *end, length, year, true))
            return false;
    }
    else if (!pushData(quest, start, *end, length, year, true))
        return false;

    // dodanie miasta do drogi
    if(!addRight(r, *end))
        return false;

    return true;
}

/***FUNKCJE GLOBALNE***/


/** @brief Tworzy nową strukturę.
 * Tworzy nową, pustą strukturę niezawierającą żadnych miast, odcinków dróg ani
 * dróg krajowych.
 * @return Wskaźnik na utworzoną strukturę lub NULL, gdy nie udało się
 * zaalokować pamięci.
 */
Map* newMap(void) {
    Map *m;
    m = NULL;
    m = (Map*) malloc(sizeof(Map));

    if (m == NULL)
        return m;

    m->bindings = NULL;
    m->bindings = newTable();

    if (m->bindings == NULL) {
        free(m);
        return m;
    }

    for (int i = 0; i < 1000; i++)
        m->routes[i] = NULL;

    return m;
}

/** @brief Usuwa strukturę.
 * Usuwa strukturę wskazywaną przez @p map.
 * Nic nie robi, jeśli wskaźnik ten ma wartość NULL.
 * @param[in] map        – wskaźnik na usuwaną strukturę.
 */
void deleteMap(Map *map) {
    deleteTable(map->bindings);

    for (int i = 0; i < 1000; i++)
        deleteRoute(map->routes[i]);

    free(map);
}

/** @brief Dodaje do mapy odcinek drogi między dwoma różnymi miastami.
 * Jeśli któreś z podanych miast nie istnieje, to dodaje go do mapy, a następnie
 * dodaje do mapy odcinek drogi między tymi miastami.
 * @param[in,out] map    – wskaźnik na strukturę przechowującą mapę dróg;
 * @param[in] city1      – wskaźnik na napis reprezentujący nazwę miasta;
 * @param[in] city2      – wskaźnik na napis reprezentujący nazwę miasta;
 * @param[in] length     – długość w km odcinka drogi;
 * @param[in] builtYear  – rok budowy odcinka drogi.
 * @return Wartość @p true, jeśli odcinek drogi został dodany.
 * Wartość @p false, jeśli wystąpił błąd: któryś z parametrów ma niepoprawną
 * wartość, obie podane nazwy miast są identyczne, odcinek drogi między tymi
 * miastami już istnieje lub nie udało się zaalokować pamięci.
 */
bool addRoad(Map *map, const char *city1, const char *city2,
             unsigned length, int builtYear) {
    if (map == NULL)
        return false;

    Hash mb = map->bindings;

    // odrzucenie błędnych danych
    if (!properName(city1) || !properName(city2) || sameCity(city1, city2))
        return false;

    if (!properYear(builtYear) || length == 0)
        return false;

    // jeśli miasta nie mają swoich struktur w tablicy hasz., należy je utworzyć
    if (searchHash(mb, city1) == NULL) {
        Cities list = newList();

        if (list == NULL)
            return false;

        if (!insertHash(mb, city1, list)) {
            deleteList(list);
            return false;
        }
    }

    if (searchHash(mb, city2) == NULL) {
        Cities list = newList();

        if (list == NULL)
            return false;

        if (!insertHash(mb, city2, list)) {
            deleteList(list);
            return false;
        }
    }

    // program szuka struktur i dopisuje do nich miasta
    Cities linkList1 = searchHash(mb, city1), linkList2 = searchHash(mb, city2);

    if (contains(linkList1->first, city2))
        return false;

    if (addCity(linkList1, city2, length, builtYear)) {
        if (!addCity(linkList2, city1, length, builtYear)) {
            deleteCity(linkList1, city2);
            return false;
        }
        else {
            return true;
        }
    }
    return false;
}


/** @brief Modyfikuje rok ostatniego remontu odcinka drogi.
 * Dla odcinka drogi między dwoma miastami zmienia rok jego ostatniego remontu
 * lub ustawia ten rok, jeśli odcinek nie był jeszcze remontowany.
 * @param[in,out] map    – wskaźnik na strukturę przechowującą mapę dróg;
 * @param[in] city1      – wskaźnik na napis reprezentujący nazwę miasta;
 * @param[in] city2      – wskaźnik na napis reprezentujący nazwę miasta;
 * @param[in] repairYear – rok ostatniego remontu odcinka drogi.
 * @return Wartość @p true, jeśli modyfikacja się powiodła.
 * Wartość @p false, jeśli wystąpił błąd: któryś z parametrów ma niepoprawną
 * wartość, któreś z podanych miast nie istnieje, nie ma odcinka drogi między
 * podanymi miastami, podany rok jest wcześniejszy niż zapisany dla tego
 * odcinka drogi rok budowy lub ostatniego remontu.
 */
bool repairRoad(Map *map, const char *city1,
                const char *city2, int repairYear) {
    Hash mb;
    if (map == NULL)
        return false;

    mb = map->bindings;

    // odrzucenie błędnych danych
    if (!properName(city1) || !properName(city2) || sameCity(city1, city2))
        return false;

    if (properYear(repairYear) == false)
        return false;

    if (searchHash(mb, city1) == NULL || searchHash(mb, city2) == NULL)
        return false;

    // wyszukanie struktur i przeprowadzenie operacji
    // odrzucenie przypadku nieistnienia drogi oraz
    // wcześniejszego remontu
    Cities linkList1 = searchHash(mb, city1), linkList2 = searchHash(mb, city2);

    if (!contains(linkList1->first, city2))
        return false;

    CityNext connection1 = findElt(linkList1->first, city2);
    CityNext connection2 = findElt(linkList2->first, city1);

    if (repairYear < connection1->year || repairYear < connection2->year)
        return false;
    else {
        connection1->year = repairYear;
        connection2->year = repairYear;
    }

    return true;
}

/** @brief Łączy dwa różne miasta drogą krajową.
 * Tworzy drogę krajową pomiędzy dwoma miastami i nadaje jej podany numer.
 * Wśród istniejących odcinków dróg wyszukuje najkrótszą drogę. Jeśli jest
 * więcej niż jeden sposób takiego wyboru, to dla każdego wariantu wyznacza
 * wśród wybranych w nim odcinków dróg ten, który był najdawniej wybudowany lub
 * remontowany i wybiera wariant z odcinkiem, który jest najmłodszy.
 * @param[in,out] map    – wskaźnik na strukturę przechowującą mapę dróg;
 * @param[in] routeId    – numer drogi krajowej;
 * @param[in] city1      – wskaźnik na napis reprezentujący nazwę miasta;
 * @param[in] city2      – wskaźnik na napis reprezentujący nazwę miasta.
 * @return Wartość @p true, jeśli droga krajowa została utworzona.
 * Wartość @p false, jeśli wystąpił błąd: któryś z parametrów ma niepoprawną
 * wartość, istnieje już droga krajowa o podanym numerze, któreś z podanych
 * miast nie istnieje, obie podane nazwy miast są identyczne, nie można
 * jednoznacznie wyznaczyć drogi krajowej między podanymi miastami
 * lub nie udało się zaalokować pamięci.
 */
bool newRoute(Map *map, unsigned routeId,
             const char *city1, const char *city2) {
    // odrzucenie błędnych danych
    if (map == NULL) return false;

    Hash mb = map->bindings;

    if (!properName(city1) || !properName(city2) || sameCity(city1, city2))
        return false;

    if (searchHash(mb, city1) == NULL || searchHash(mb, city2) == NULL)
        return false;

    if (routeId < 1 || routeId > 999)
        return false;

    if (map->routes[routeId] != NULL)
        return false;

    // przeprowadzenie algorytmu Dijkstry
    HeapCard card = NULL;
    card = dijkstra(map->bindings, city1, city2, NULL, false);

    // jeśli nie otrzymaliśmy najlepszej konstrukcji, zwraca false
    if (card == NULL)
        return false;

    // na podstawie opisu tworzy drogę
    Route r = card->construction;
    card->construction = NULL;
    deleteCard(card);

    if(r == NULL)
        return false;

    map->routes[routeId] = r;
    return true;
}

/** @brief Wydłuża drogę krajową do podanego miasta.
 * Dodaje do drogi krajowej nowe odcinki dróg do podanego miasta w taki sposób,
 * aby nowy fragment drogi krajowej był najkrótszy. Jeśli jest więcej niż jeden
 * sposób takiego wydłużenia, to dla każdego wariantu wyznacza wśród dodawanych
 * odcinków dróg ten, który był najdawniej wybudowany lub remontowany i wybiera
 * wariant z odcinkiem, który jest najmłodszy.
 * @param[in,out] map    – wskaźnik na strukturę przechowującą mapę dróg;
 * @param[in] routeId    – numer drogi krajowej;
 * @param[in] city       – wskaźnik na napis reprezentujący nazwę miasta.
 * @return Wartość @p true, jeśli droga krajowa została wydłużona.
 * Wartość @p false, jeśli wystąpił błąd: któryś z parametrów ma niepoprawną
 * nazwę, nie istnieje droga krajowa o podanym numerze, nie ma miasta o podanej
 * nazwie, przez podane miasto już przechodzi droga krajowa o podanym numerze,
 * podana droga krajowa kończy się w podanym mieście, nie można jednoznacznie
 * wyznaczyć nowego fragmentu drogi krajowej lub nie udało się zaalokować
 * pamięci.
 */
bool extendRoute(Map *map, unsigned routeId, const char *city) {
    // odrzucenie błędnych danych
    if (map == NULL)
        return false;

    if (searchHash(map->bindings, city) == NULL)
        return false;

    if (routeId < 1 || routeId > 999)
        return false;

    Route route = map->routes[routeId];

    if (route == NULL)
        return false;

    if (!strcmp(city, route->first->name) || !strcmp(city, route->last->name))
        return false;

    // przeprowadzenie algorytmu Dijkstry w wersji „do któregokolwiek z dwóch”
    HeapCard card = doubleDijkstra(map->bindings, city, route);

    // w przypadku braku wyłonienia najlepszego kandydata
    if (card == NULL) {
        return false;
    }

    // rozstrzyga, do którego z końców doszło rozszerzenie i na podstawie tego
    // tworzy je, a następnie dobudowuje do drogi
    if (!strcmp(card->lastCity, route->first->name)) {
        Route r = card->construction;
        card->construction = NULL;

        if (r == NULL) {
            deleteCard(card);
            return false;
        }

        map->routes[routeId] = merge2Routes(r ,route);
    }
    else {
        Route r = revRoute(card->construction);

        if (r == NULL) {
            deleteCard(card);
            return false;
        }

        map->routes[routeId] = merge2Routes(route, r);
    }
    deleteCard(card);
    return true;
}

/** @brief Usuwa odcinek drogi między dwoma różnymi miastami.
 * Usuwa odcinek drogi między dwoma miastami. Jeśli usunięcie tego odcinka drogi
 * powoduje przerwanie ciągu jakiejś drogi krajowej, to uzupełnia ją
 * istniejącymi odcinkami dróg w taki sposób, aby była najkrótsza. Jeśli jest
 * więcej niż jeden sposób takiego uzupełnienia, to dla każdego wariantu
 * wyznacza wśród dodawanych odcinków drogi ten, który był najdawniej wybudowany
 * lub remontowany i wybiera wariant z odcinkiem, który jest najmłodszy.
 * @param[in,out] map    – wskaźnik na strukturę przechowującą mapę dróg;
 * @param[in] city1      – wskaźnik na napis reprezentujący nazwę miasta;
 * @param[in] city2      – wskaźnik na napis reprezentujący nazwę miasta.
 * @return Wartość @p true, jeśli odcinek drogi został usunięty.
 * Wartość @p false, jeśli z powodu błędu nie można usunąć tego odcinka drogi:
 * któryś z parametrów ma niepoprawną wartość, nie ma któregoś z podanych miast,
 * nie istnieje droga między podanymi miastami, nie da się jednoznacznie
 * uzupełnić przerwanego ciągu drogi krajowej lub nie udało się zaalokować
 * pamięci.
 */
bool removeRoad(Map* map, const char* city1, const char* city2) {
    // odrzucenie błędnych danych

    if (map == NULL)
        return false;

    if (!properName(city1) || !properName(city2))
        return false;

    Hash mb;
    mb = map->bindings;

    if (searchHash(mb, city1) == NULL || searchHash(mb, city2) == NULL)
        return false;

    Cities list1 = searchHash(mb, city1);
    Cities list2 = searchHash(mb, city2);
    HeapCard card;

    if (!contains(list1->first, city2))
        return false;

    // tu będą trzymane gotowe „tasiemce” do „wklejenia”
    Route corrections[1000];

    for (unsigned i = 1; i < 1000; i++)
        corrections[i] = NULL;

    // należy rozpatrzyć, czy nie należy gdzieś skonstruować objazdu
    for (unsigned i = 1; i < 1000; i++)
        if (map->routes[i] != NULL) {
            // program rozpatrza dwa przypadki przynależności usuwanego odcinka
            // do drogi krajowej – od prawej do lewej i od lewej do prawej
            if (existsPath(map->routes[i]->first, city1, city2)) {
                card = dijkstra(map->bindings, city1,
                                city2, map->routes[i], true);

                if (card == NULL) {
                    for (unsigned j = 1; j < i; j++)
                        deleteRoute(corrections[j]);

                    return false;
                }

                corrections[i] = card->construction;
                card->construction = NULL;
                deleteCard(card);
            }
            else if (existsPath(map->routes[i]->first, city2, city1)) {
                card = dijkstra(map->bindings, city2,
                                city1, map->routes[i], true);

                if (card == NULL) {
                    for (unsigned j = 1; j < i; j++)
                        deleteRoute(corrections[j]);

                    return false;
                }

                corrections[i] = card->construction;
                card->construction = NULL;
                deleteCard(card);
            }
        }

    // wkleja tasiemce do dróg w miejsce usuniętych oddinków
    for (unsigned i = 1; i < 1000; i++)
        if (corrections[i] != NULL) {
            map->routes[i] = replaceRoute(map->routes[i], corrections[i],
                        corrections[i]->first->name);
        }

    // na końcu usuwa odcinek z hashmapy
    deleteCity(list1, city2);
    deleteCity(list2, city1);
    return true;
}

/** @brief Udostępnia informacje o drodze krajowej.
 * Zwraca wskaźnik na napis, który zawiera informacje o drodze krajowej. Alokuje
 * pamięć na ten napis. Zwraca pusty napis, jeśli nie istnieje droga krajowa
 * o podanym numerze. Zaalokowaną pamięć trzeba zwolnić za pomocą funkcji free.
 * Informacje wypisywane są w formacie:
 * numer drogi krajowej;nazwa miasta;długość odcinka drogi;rok budowy lub
 * ostatniego remontu;nazwa miasta;długość odcinka drogi;rok budowy lub
 * ostatniego remontu;nazwa miasta;…;nazwa miasta.
 * Kolejność miast na liście jest taka, aby miasta @p city1 i @p city2, podane
 * w wywołaniu funkcji @ref newRoute, które utworzyło tę drogę krajową, zostały
 * wypisane w tej kolejności.
 * @param[in,out] map    – wskaźnik na strukturę przechowującą mapę dróg;
 * @param[in] routeId    – numer drogi krajowej.
 * @return Wskaźnik na napis lub NULL, gdy nie udało się zaalokować pamięci.
 */
char const* getRouteDescription(Map *map, unsigned routeId) {
    if (map == NULL)
        return NULL;

    // przypadki braki drogi
    if (routeId < 1 || routeId > 999) {
        char *str = NULL;
        str = malloc(1);
        if (str == NULL) return str;
        str[0] = '\0';
        return str;
    }

    if((map->routes[routeId]) == NULL) {
        char *str = NULL;
        str = malloc(1);
        if (str == NULL) return str;
        str[0] = '\0';
        return str;
    }

    // str – miejsce na opis, idInChar – miejsce na numer drogi
    char *str = NULL, *idInChar = NULL;
    idInChar = numberToChar(routeId);

    if (idInChar == NULL)
        return NULL;

    size_t idLength = strlen(idInChar);
    size_t charSize = 1 + strlen(map->routes[routeId]->first->name) + idLength;
    str = malloc(charSize + 1);

    if (str == NULL) {
        free(idInChar);
        return str;
    }

    // na początek napisu musimy wstawić numer drogi
    memcpy(str, idInChar, idLength);
    free(idInChar);
    strcpy(&str[idLength], ";");
    // wstawiamy też od razu nazwę pierwszego miasta
    strcpy(&str[idLength + 1], map->routes[routeId]->first->name);

    // kończymy napis
    if (!constructDescription(map, &str, map->routes[routeId]->first,
        charSize, charSize + 1))
        return NULL;

    return str;
}

/** @brief Tworzy drogę krajową.
 * Tworzy nową drogę krajową na podstawie opisu, dodając brakujące
 * odcinki. O ile numer drogi jest prawidłowy i taka droga jeszcze nie istnieje,
 * tworzy nową na podstawie opisu w formacie takim, jak w getRouteDescripion
 * (z wyłączeniem numeru drogi i średnika). Jeżeli jakiś odcinek nie istnieje,
 * jest on uzupełniany. Jeżeli jednak odcinek istnieje i ma w opisie inną
 * długość lub wcześniejszy rok, funkcja nie robi nic.
 * @param[in,out] map  – wskaźnik na strukturę przechowującą mapę dróg;
 * @param[in] no       – numer drogi krajowej.
 * @param[in] descr      – wskaźnik napisu z opisem drogi.
 * @return Wartość @p true, gdy udało się zrobić drogę. Wartość @p false, gdy
 * numer drogi jest nieprawidłowy, podana droga nie istnieje lub nie udało się
 * zaalokować pamięci.
 */
bool makeANewRoute(Map *map, unsigned no, char *descr) {
    //odrzucenie błędnych danych
    if (no < 1 || no > 999)
        return false;

    if (map->routes[no] != NULL)
        return false;

    // informacja na temat pozycji pierwszych znaków interesujących nas danych
    // scanner przechodzi cały napis i zatrzymuje się na średnikach i na końcu
    size_t scanner = 0, lenPos = 0, yrPos = 0, c2Pos = 0;
    char *start = NULL, *end = NULL;
    bool oneMore = true; //jeśli prawdziwa, będziemy wykonywać pętlę
    // state: 0 – gdy oczekujemy pierwszego miasta, 1 – długosci, 2 – roku,
    // 3 – kolejnych miast
    int state = 0;
    Route r = NULL;
    Quest quest = newQuest(); //zlecenie budowy tudzież naprawy nowych odcinków

    if (quest == NULL)
        return false;

    // Najpierw program sprawdza, poprawność danych, jednocześnie tworząc drogę.
    while (oneMore) {
        // wywołanie przy dojściu do końca znaku jest ostatnim
        if (descr[scanner] == '\0') {
            if (state != 3)
                return makingRouteFreer(start, NULL, r, quest);

            oneMore = false;
        }

        if(descr[scanner] == ';' || !oneMore) {
            if (state == 0) {
                // pobranie informacji o pierwszym mieście i początek  drogi
                if (!mCopy(&start, descr, 0, scanner - 1))
                    return makingRouteFreer(start, NULL, r, quest);

                r = startRoute(start);

                if (r == NULL)
                    return makingRouteFreer(start, NULL, r, quest);

                // wiemy już, że o ile wszystko jest prawidłowe, długość odcinka
                // pojawia się w descr na poniższej pozycji, analog. w kolejnych
                lenPos = scanner + 1;
                // przejście do kolejnego stanu (analogicznie w kolejnych)
                state++;
            }
            else if (state == 1) {
                yrPos = scanner + 1;
                state++;
            }
            else if (state == 2) {
                c2Pos = scanner + 1;
                state++;
            }
            else if (state == 3) {
                if (!addToRouteAndQuest(map, descr, quest, r, start, &end, lenPos,
                    yrPos, c2Pos, scanner))
                    return makingRouteFreer(start, end, r, quest);

                free(start);
                start = end; //miasto końcowe staje się pocz. nowego odcinka
                end = NULL;
                lenPos = scanner + 1;
                state = 1;
            }
        }

        if (oneMore)
            scanner++;
    }
    free(start);

    // realizacja zlecenia
    if (!performQuest(map, quest))
        return makingRouteFreer(NULL, NULL, r, quest);

    deleteQuest(quest);
    map->routes[no] = r;
    return true;
}

/** @brief Usuwa drogę krajową.
 * Jeżeli droga krajowa istnieje, usuwa ją z mapy.
 * @param[in,out] map       – wskaźnik na strukturę przechowującą mapę dróg;
 * @param[in] routeId       – numer drogi krajowej.
 * @return Wartość @p true, gdy udało się usunąć drogę. Wartość @p false, gdy
 * numer drogi jest nieprawidłowy lub podana droga nie istnieje.
 */
bool removeRoute(Map *map, unsigned routeId) {
    if (routeId < 1 || routeId > 999)
        return false;

    if (map->routes[routeId] == NULL)
        return false;

    deleteRoute(map->routes[routeId]);
    map->routes[routeId] = NULL;
    return true;
}
