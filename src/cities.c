/** @file
 * Implementacja klasy przechowującej listę połączeń z miastami
 *
 * @author Adam Rempała <ar406309@students.mimuw.edu.pl>
 * @copyright Adam Rempała
 * @date 17.08.2019
 */

#include "cities.h"

/** @brief Tworzy nową strukturę.
 * Tworzy nową strukturę zawierającą wskaźnik na NULL-a (pusta lista).
 * @return Wskaźnik na pustą listę lub NULL, jeśli nie udało się zaalokować
 * pamięci.
 */
Cities newList() {
    Cities cityList = NULL;
    cityList = (Cities)malloc(sizeof(struct CityList));
    
    if (cityList == NULL)
        return NULL;
    
    cityList->first = NULL;
    return cityList;
}

/** @brief Usuwa elementy listy
 * Usuwa element listy i, rekurencyjnie, wszystkie następne.
 * @param[in] cityElt    – wskaźnik na element listy
 */
void deleteListCities(CityNext cityElt) {
    if (cityElt != NULL) {
        deleteListCities(cityElt->next);
        free(cityElt->cityName);
        free(cityElt);
    }
}

/** @brief Usuwa listę
 * Usuwa wszystko, co jest związane ze wskazaną listą.
 * @param[in] cityList    – wskaźnik na listę
 */
void deleteList(Cities cityList) {
    if (cityList != NULL) {
        if (cityList->first != NULL)
            deleteListCities(cityList->first);
        
        free(cityList);
    }
}

/** @brief Szukanie połączenia
 * Na podstawie nazwy miasta szuka połączenia we wszystkich elementach listy,
 * począwszy od wskazanego.
 * @param[in] cityElt   – wskaźnik na element listy
 * @param[in] city      – wskaźnik na nazwę szukanego miasta
 * @return Wskaźnik na szukane połączenie lub NULL, jeśli nie udało się go
 * znaleźć.
 */
CityNext findElt(CityNext cityElt, const char *city) {
    if (cityElt == NULL)
        return cityElt;
    
    if (!strcmp(cityElt->cityName, city))
        return cityElt;
    
    return findElt(cityElt->next, city);
}

/** @brief Sprawdzanie, czy połączenie istnieje
 * Na podstawie nazwy miasta sprawdza, czy na liście, począwszy od podanego
 * elementu, istnieje połączenie do danego miasta.
 * @param[in] cityElt     – wskaźnik na element listy
 * @param[in] city  – wskaźnik na nazwę szukanego miasta
 * @return wartość @p true, jeśli istnieje, w przeciwnym wypadku @p false.
 */
bool contains(CityNext cityElt, const char *city) {
    if (cityElt == NULL)
        return false;
    
    if (!strcmp(cityElt->cityName, city))
        return true;
    
    return contains(cityElt->next, city);
}

/** @brief Dodanie połączenia do listy
 * Dodaje na początek listy połączenie.
 * @param[in,out] cityList  – wskaźnik na listę
 * @param[in] cityName      – wskaźnik na nazwę miasta, do którego prowadzi nowy odcinek
 * @param[in] length        – długość odicnka
 * @param[in] year          – rok ostatniego remontu lub budowy
 * @return Wartość @p true, jeśli udało się dodać miasto lub @p false, jeśli
 * nie udało się zaalokować pamięci.
 */
bool addCity(Cities cityList, const char *cityName, unsigned length, int year) {
    if (contains(cityList->first, cityName))
        return false;
    else {
        CityNext newCityElt = NULL;
        newCityElt = (CityNext)malloc(sizeof(struct CityElts));
        
        if (newCityElt == NULL)
            return false;
        
        newCityElt->cityName = strdup(cityName);
        
        if (newCityElt->cityName == NULL) {
            free(newCityElt);
            return false;
        }
        
        newCityElt->length = length;
        newCityElt->year = year;
        newCityElt->next = cityList->first;
        cityList->first = newCityElt;
        return true;
    }
}

/** @brief Usuwa połączenie z listy
 * Usuwa z listy wskaźnik na połączenie, chyba że dany nie istnieje.
 * @param[in,out] cityList  – wskaźnik na listę.
 * @param[in] cityName      – wskaźnik na nazwę miasta, do którego prowadzi odcinek.
 * @return Wartość @p true, jeśli udało się usunąć wskaźnik na połączenie
 * lub @p false, jeśli wskaźnik nie istnieje.
 */
bool deleteCity(Cities cityList, const char *cityName) {
    // Usuwany będzie element current, ale musimy podpiąć kolejny element listy
    // do poprzedniego.
    CityNext previous, current;
    current = cityList->first;
    previous = cityList->first;
    
    if (current == NULL)
        return false;
    
    // przypadek, gdy usuwany element jest na początku listy
    if (!strcmp((current)->cityName, cityName)) {
        cityList->first = current->next;
        free(current->cityName);
        free(current);
        return true;
    }
    // przeciwny przypadek
    else {
        current = current->next;
        
        // pętla szukająca usuwanego elementu
        while (current != NULL) {	
            if (!strcmp((current)->cityName, cityName)) {
                // usunięcie znalezionego połączenia
                previous->next = current->next;
                free(current->cityName);
                free(current);
                return true;
            }
            previous = current;
            current = current->next;
        }
        
        return false;
    }
}

/** @brief Długość odcinka
 * Podaje informację o długości odcinka lub 0, jeśli odcinek nie istnieje.
 * @param[in,out] cityList     – wskaźnik na listę
 * @param[in] city             – wskaźnik na nazwę miasta, do którego prowadzi odcinek
 * @return Liczba nieujemna opisująca długość odcinka lub 0, gdy odcinek
 * nie istnieje.
 */
unsigned getLength(Cities cityList, const char *city) {
    CityNext cityElt = findElt(cityList->first, city);
    
    if (cityElt == NULL)
        return 0;
    
    return cityElt->length;
}

/** @brief Rok ostatniego remontu odcinka
 * Podaje informację o roku ostatniego remontu lub budowy odcinka lub
 * zwraca 0, jeśli odcinek nie istnieje.
 * @param[in,out] cityList     – wskaźnik na listę
 * @param[in] city             – wskaźnik na nazwę miasta, do którego prowadzi odcinek
 * @return Liczba niezerowa opisująca rok ostatniego remontu lub budowy odcinka
 * lub 0, gdy odcinek nie istnieje.
 */
int getYear(Cities cityList, const char *city) {
    CityNext cityElt = findElt(cityList->first, city);
    
    if (cityElt == NULL)
        return 0;
    
    return cityElt->year;
}

