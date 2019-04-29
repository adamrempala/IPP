/** @file
 * Interfejs klasy przechowuj¹cej mapê dróg krajowych
 *
 * @author £ukasz Kamiñski <kamis@mimuw.edu.pl>, Marcin Peczarski <marpe@mimuw.edu.pl>
 * @copyright Uniwersytet Warszawski
 * @date 20.03.2019
 */

#ifndef __MAP_H__
#define __MAP_H__

#include <stdbool.h>

#include "road.h"
#include "hashtable.h"
/**
 * Struktura przechowuj¹ca mapê dróg krajowych. Zawiera wskazanie na tablic� haszuj�c� z 
 * powi�zaniami i tablic� dr�g krajowych (tablica routes[0] jest zawsze pusta).
 */
typedef struct Map {
    HashTable *bindings;
    Road* routes[1000];
} Map;

/** @brief Tworzy now¹ strukturê.
 * Tworzy now¹, pust¹ strukturê niezawieraj¹c¹ ¿adnych miast, odcinków dróg ani
 * dróg krajowych.
 * @return WskaŸnik na utworzon¹ strukturê lub NULL, gdy nie uda³o siê
 * zaalokowaæ pamiêci.
 */
Map* newMap(void);

/** @brief Usuwa strukturê.
 * Usuwa strukturê wskazywan¹ przez @p map.
 * Nic nie robi, jeœli wskaŸnik ten ma wartoœæ NULL.
 * @param[in] map        – wskaŸnik na usuwan¹ strukturê.
 */
void deleteMap(Map *map);

/** @brief Dodaje do mapy odcinek drogi miêdzy dwoma ró¿nymi miastami.
 * Jeœli któreœ z podanych miast nie istnieje, to dodaje go do mapy, a nastêpnie
 * dodaje do mapy odcinek drogi miêdzy tymi miastami.
 * @param[in,out] map    – wskaŸnik na strukturê przechowuj¹c¹ mapê dróg;
 * @param[in] city1      – wskaŸnik na napis reprezentuj¹cy nazwê miasta;
 * @param[in] city2      – wskaŸnik na napis reprezentuj¹cy nazwê miasta;
 * @param[in] length     – d³ugoœæ w km odcinka drogi;
 * @param[in] builtYear  – rok budowy odcinka drogi.
 * @return Wartoœæ @p true, jeœli odcinek drogi zosta³ dodany.
 * Wartoœæ @p false, jeœli wyst¹pi³ b³¹d: któryœ z parametrów ma niepoprawn¹
 * wartoœæ, obie podane nazwy miast s¹ identyczne, odcinek drogi miêdzy tymi
 * miastami ju¿ istnieje lub nie uda³o siê zaalokowaæ pamiêci.
 */
bool addRoad(Map *map, const char *city1, const char *city2,
             unsigned length, int builtYear);

/** @brief Modyfikuje rok ostatniego remontu odcinka drogi.
 * Dla odcinka drogi miêdzy dwoma miastami zmienia rok jego ostatniego remontu
 * lub ustawia ten rok, jeœli odcinek nie by³ jeszcze remontowany.
 * @param[in,out] map    – wskaŸnik na strukturê przechowuj¹c¹ mapê dróg;
 * @param[in] city1      – wskaŸnik na napis reprezentuj¹cy nazwê miasta;
 * @param[in] city2      – wskaŸnik na napis reprezentuj¹cy nazwê miasta;
 * @param[in] repairYear – rok ostatniego remontu odcinka drogi.
 * @return Wartoœæ @p true, jeœli modyfikacja siê powiod³a.
 * Wartoœæ @p false, jeœli wyst¹pi³ b³¹d: któryœ z parametrów ma niepoprawn¹
 * wartoœæ, któreœ z podanych miast nie istnieje, nie ma odcinka drogi miêdzy
 * podanymi miastami, podany rok jest wczeœniejszy ni¿ zapisany dla tego odcinka
 * drogi rok budowy lub ostatniego remontu.
 */
bool repairRoad(Map *map, const char *city1, const char *city2, int repairYear);

/** @brief £¹czy dwa ró¿ne miasta drog¹ krajow¹.
 * Tworzy drogê krajow¹ pomiêdzy dwoma miastami i nadaje jej podany numer.
 * Wœród istniej¹cych odcinków dróg wyszukuje najkrótsz¹ drogê. Jeœli jest
 * wiêcej ni¿ jeden sposób takiego wyboru, to dla ka¿dego wariantu wyznacza
 * wœród wybranych w nim odcinków dróg ten, który by³ najdawniej wybudowany lub
 * remontowany i wybiera wariant z odcinkiem, który jest najm³odszy.
 * @param[in,out] map    – wskaŸnik na strukturê przechowuj¹c¹ mapê dróg;
 * @param[in] routeId    – numer drogi krajowej;
 * @param[in] city1      – wskaŸnik na napis reprezentuj¹cy nazwê miasta;
 * @param[in] city2      – wskaŸnik na napis reprezentuj¹cy nazwê miasta.
 * @return Wartoœæ @p true, jeœli droga krajowa zosta³a utworzona.
 * Wartoœæ @p false, jeœli wyst¹pi³ b³¹d: któryœ z parametrów ma niepoprawn¹
 * wartoœæ, istnieje ju¿ droga krajowa o podanym numerze, któreœ z podanych
 * miast nie istnieje, obie podane nazwy miast s¹ identyczne, nie mo¿na
 * jednoznacznie wyznaczyæ drogi krajowej miêdzy podanymi miastami lub nie uda³o
 * siê zaalokowaæ pamiêci.
 */
bool newRoute(Map *map, unsigned routeId,
              const char *city1, const char *city2);

/** @brief Wyd³u¿a drogê krajow¹ do podanego miasta.
 * Dodaje do drogi krajowej nowe odcinki dróg do podanego miasta w taki sposób,
 * aby nowy fragment drogi krajowej by³ najkrótszy. Jeœli jest wiêcej ni¿ jeden
 * sposób takiego wyd³u¿enia, to dla ka¿dego wariantu wyznacza wœród dodawanych
 * odcinków dróg ten, który by³ najdawniej wybudowany lub remontowany i wybiera
 * wariant z odcinkiem, który jest najm³odszy.
 * @param[in,out] map    – wskaŸnik na strukturê przechowuj¹c¹ mapê dróg;
 * @param[in] routeId    – numer drogi krajowej;
 * @param[in] city       – wskaŸnik na napis reprezentuj¹cy nazwê miasta.
 * @return Wartoœæ @p true, jeœli droga krajowa zosta³a wyd³u¿ona.
 * Wartoœæ @p false, jeœli wyst¹pi³ b³¹d: któryœ z parametrów ma niepoprawn¹
 * nazwê, nie istnieje droga krajowa o podanym numerze, nie ma miasta o podanej
 * nazwie, przez podane miasto ju¿ przechodzi droga krajowa o podanym numerze,
 * podana droga krajowa koñczy siê w podanym mieœcie, nie mo¿na jednoznacznie
 * wyznaczyæ nowego fragmentu drogi krajowej lub nie uda³o siê zaalokowaæ
 * pamiêci.
 */
bool extendRoute(Map *map, unsigned routeId, const char *city);

/** @brief Usuwa odcinek drogi miêdzy dwoma ró¿nymi miastami.
 * Usuwa odcinek drogi miêdzy dwoma miastami. Jeœli usuniêcie tego odcinka drogi
 * powoduje przerwanie ci¹gu jakiejœ drogi krajowej, to uzupe³nia j¹
 * istniej¹cymi odcinkami dróg w taki sposób, aby by³a najkrótsza. Jeœli jest
 * wiêcej ni¿ jeden sposób takiego uzupe³nienia, to dla ka¿dego wariantu
 * wyznacza wœród dodawanych odcinków drogi ten, który by³ najdawniej wybudowany
 * lub remontowany i wybiera wariant z odcinkiem, który jest najm³odszy.
 * @param[in,out] map    – wskaŸnik na strukturê przechowuj¹c¹ mapê dróg;
 * @param[in] city1      – wskaŸnik na napis reprezentuj¹cy nazwê miasta;
 * @param[in] city2      – wskaŸnik na napis reprezentuj¹cy nazwê miasta.
 * @return Wartoœæ @p true, jeœli odcinek drogi zosta³ usuniêty.
 * Wartoœæ @p false, jeœli z powodu b³êdu nie mo¿na usun¹æ tego odcinka drogi:
 * któryœ z parametrów ma niepoprawn¹ wartoœæ, nie ma któregoœ z podanych miast,
 * nie istnieje droga miêdzy podanymi miastami, nie da siê jednoznacznie
 * uzupe³niæ przerwanego ci¹gu drogi krajowej lub nie uda³o siê zaalokowaæ
 * pamiêci.
 */
bool removeRoad(Map *map, const char *city1, const char *city2);

/** @brief Udostêpnia informacje o drodze krajowej.
 * Zwraca wskaŸnik na napis, który zawiera informacje o drodze krajowej. Alokuje
 * pamiêæ na ten napis. Zwraca pusty napis, jeœli nie istnieje droga krajowa
 * o podanym numerze. Zaalokowan¹ pamiêæ trzeba zwolniæ za pomoc¹ funkcji free.
 * Informacje wypisywane s¹ w formacie:
 * numer drogi krajowej;nazwa miasta;d³ugoœæ odcinka drogi;rok budowy lub
 * ostatniego remontu;nazwa miasta;d³ugoœæ odcinka drogi;rok budowy lub
 * ostatniego remontu;nazwa miasta;…;nazwa miasta.
 * Kolejnoœæ miast na liœcie jest taka, aby miasta @p city1 i @p city2, podane
 * w wywo³aniu funkcji @ref newRoute, które utworzy³o tê drogê krajow¹, zosta³y
 * wypisane w tej kolejnoœci.
 * @param[in,out] map    – wskaŸnik na strukturê przechowuj¹c¹ mapê dróg;
 * @param[in] routeId    – numer drogi krajowej.
 * @return WskaŸnik na napis lub NULL, gdy nie uda³o siê zaalokowaæ pamiêci.
 */
char const* getRouteDescription(Map *map, unsigned routeId);

#endif /* __MAP_H__ */
