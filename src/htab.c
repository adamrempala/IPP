/** @file
 * Implementacja klasy przechowującej hashmapę
 *
 * @author Adam Rempała <ar406309@students.mimuw.edu.pl>
 * @copyright Adam Rempała
 * @date 17.08.2019
 */


#include "strdup.h"
#include "htab.h"
#include "prime.h"

/**
 * Początkowy rozmiar tablicy haszującej
 */
#define BASE 67

/**
 * Liczba, za pomocą której haszujemy
 */
#define HASHPRIME 241

/**
 * Deklaracja struktury usuniętego elementu.
 */
static struct HashItem DELETED = {NULL, NULL};

/***FUNKCJE POMOCNICZE***/

/** @brief Tworzy nową strukturę (hasło).
 * Nowo powstała struktura zawiera wskaźnik na napis będący kluczem
 * oraz na listę miast.
 * @param[in] key   – wskaźnik na napis (klucz);
 * @param[in] value – wskaźnik na listę połączeń.
 * @return Wskaźnik na utworzoną strukturę lub NULL, gdy nie udało się
 * zaalokować pamięci.
 */
static Term newItem(const char *key, Cities value) {
    Term term = NULL;
    term = (Term)malloc(sizeof(struct HashItem));
    
    if (term == NULL)
        return NULL;
    
    term->key = strdup(key);
    
    if (term->key == NULL) {
        free(term);
        return NULL;
    }
    
    term->value = value;
    return term;
}

/** @brief Tworzy nową hashmapę na podstawie liczby bazowej.
 * Dostając liczbę bazową, szuka kolejnej pierwszej i na tej podstawie
 * tworzy hashmapę o jej rozmiarze.
 * @param[in] base   – liczba bazowa
 * @return Wskaźnik na nowo powstałą strukturę lub NULL, jeśli nie udało się
 * zaalokować pamięci.
 */
static Hash newHashSized(const int base) {
    Hash table = NULL;
    table = (Hash)malloc(sizeof(struct Hashtable));
    
    if (table == NULL)
        return table;
    
    table->base = base;
    table->size = nextPrime(table->base);
    table->numberOfElements = 0;
    table->items = (Term*)calloc((size_t)table->size, sizeof(struct HashItem*));
    
    if (table->items == NULL) {
        free(table);
        return NULL;
    }
    
    for (size_t i = 0; i < table->size; i++)
        table->items[i] = NULL;
    
    return table;
}

/** @brief Usuwa strukturę.
 * Usuwa hasło oraz wszystko, co z nim związane. Nic nie robi,
 * jeżeli hasło ma wartość NULL.
 * @param[in] term – wskaźnik na hasło.
 */
static void deleteItem(Term term) {
    if (term->key != NULL)
        free(term->key);
    
    if (term->value != NULL)
        deleteList(term->value);
    
    free(term);
}

/** @brief Usuwa strukturę.
 * Usuwa hasło, ale nie usuwa listy miast. Funkcja używana w przypadku
 * przenoszenia elementów do większej tablicy, czyli @ref resizeHash.
 * Nic nie robi, jeżeli hasło ma wartość NULL.
 * @param[in] term – wskaźnik na hasło.
 */
static void deleteItemButNotCities(Term term) {
    if (term->key != NULL)
        free(term->key);
    
    free(term);
}

/** @brief Usuwa strukturę.
 * Usuwa hashmapę, ale nie usuwa list miast. Funkcja używana w przypadku
 * przenoszenia elementów do większej tablicy, czyli @ref resizeHash.
 * Nic nie robi,
 * jeżeli hasło ma wartość NULL.
 * @param[in] table – wskaźnik na hashmapę.
 */
void deleteTableButNotCities(Hash table) {
    for (unsigned i = 0; i < table->size; i++) {
        Term item = table->items[i];
        
        if (item != NULL) {
            deleteItemButNotCities(item);
            table->items[i] = NULL;
        }
    }
    free(table->items);
    free(table);
}

/** @brief Rozszerza tablicę.
 * Sensu stricto tworzy nową, większą tablicę, przenosi wszystkie
 * elementy ze starej i usuwa tę starą.
 * @param[in,out] table – wskaźnik na hashmapę.
 * @param[in] base  – nowa liczba podstawowa.
 * @return Wartość @p true w przypadku powodzenia lub braku konieczności
 * zwiększenia, wartość @p false w przypadku gdy nie udało się
 * zaalokować pamięci.
 */
static bool resizeHash(Hash table, const int base) {
    if (base < BASE) {
        return true;
    }
    Hash nova = newHashSized(base);
    
    if (nova == NULL)
        return false;
    
    for (unsigned i = 0; i < table->size; i++) {
        Term item = table->items[i];
        if (item != NULL && item != &DELETED) {
            if (!insertHash(nova, item->key, item->value)) {
                deleteTable(nova);
                return false;
            }
        }
    }
    table->base = nova->base;
    table->numberOfElements = nova->numberOfElements;

    const int tmpInt = table->size;
    table->size = nova->size;
    nova->size = tmpInt;
    
    Term* tmpItem = table->items;
    table->items = nova->items;
    nova->items = tmpItem;
    
    deleteTableButNotCities(nova);
    return true;
}

/** @brief Funkcja haszująca
 * @param[in] key – wskaźnik na klucz.
 * @param[in] p – liczba pierwsza.
 * @param[in] maxElem – maksymalna liczba elementów haszmapy.
 * @return Wynik funkcji haszującej w postaci liczby całkowitej.
 * mniejszej od m, a większej lub równej 0.
 */
static int hasher(const char *key, const int p, const int maxElem) {
    long long hash = 0;
    size_t keyLength = strlen(key);
    for (size_t i = 0; i < keyLength; i++) {
        int uChar = (int)key[i];
        
        if (uChar < 0)
            uChar += 256;
        
        hash += (long long)pow(p, keyLength - (i + 1)) * uChar;
        hash = hash % maxElem;
    }
    return (int)hash;
}

/** @brief Funkcja ustalająca indeks w haszmapie
 * @param[in] key       – wskaźnik na klucz.
 * @param[in] hashers   – maksymalna liczba elementów hashmapy.
 * @return Potencjalny indeks w hashmapie.
 */
static int getHash(const char *key, const int hashers) {
    return hasher(key, HASHPRIME, hashers);
}

/***FUNKCJE GLOBALNE***/

/** @brief Tworzy nową strukturę.
 * Tworzy nową strukturę niezawierającą żadnych haseł, ale mającą swój rozmiar.
 * @return Wskaźnik na utworzoną strukturę lub NULL, gdy nie udało się
 * zaalokować pamięci.
 */
Hash newTable() {
    return newHashSized(BASE);
}

/** @brief Usuwa strukturę.
 * Usuwa hashmapę,ze wszystkimi odniesieniami, które zawiera.
 * @param[in] table – wskaźnik na hashmapę.
 */
void deleteTable(Hash table) {
    for (unsigned i = 0; i < table->size; i++) {
        Term item = table->items[i];
        
        if (item != NULL) {
            deleteItem(item);
        }
    }
    free(table->items);
    free(table);
}

/** @brief Wprowadzenie hasła do hashmapy.
 * Wprowadza do hashmapy hasło z listą połączeń.
 * @param[in,out] table – wskaźnik na hashmapę.
 * @param[in] key       – wskaźnik na klucz.
 * @param[in] value     – wskaźnik na liste połączeń.
 * @return Wartość @p true w przypadku pomyślnego przebiegu,
 * wartość @p false, jeśli nie udało się zalokować pamięci.
 */
bool insertHash(Hash table, const char *key, Cities value) {
    if (table->numberOfElements * 10 / table->size > 7) {
        if (!resizeHash(table, (table->base) * 2))
            return false;
    }
    Term item = newItem(key, value);
    
    if (item == NULL)
        return false;
    
    int index = getHash(item->key, table->size);
    Term actual = table->items[index];
    
    while (actual != NULL && actual != &DELETED) {
        // gdy miejsce jest zajęte, przechodzimy do następnego
        index = (index + 1) % table->size;
        actual = table->items[index];
    }
    
    table->items[index] = item;
    table->numberOfElements++;
    return true;
}


/** @brief Wyszukiwanie listy połączeń
 * Na podstawie klucza szuka w hashmapie listy połączeń z danym miastem.
 * @param[in] table – wskaźnik na hashmapę.
 * @param[in] key       – wskaźnik na klucz (miasto).
 * @return Wskaźnik na listę połączeń lub NULL, jeśli nie udało się
 * znaleźć hasła.
 */
Cities searchHash(Hash table, const char *key) {
    int index = getHash(key, table->size);
    Term item = table->items[index];
    while (item != NULL) {
        if (item != &DELETED)
            if (strcmp(item->key, key) == 0)
                return item->value;
        index = (index + 1) % table->size;
        item = table->items[index];
    }
    return NULL;
}

void deleteHash(Hash table, const char *key) {
    int index = getHash(key, table->size);
    Term item;
    item = table->items[index];
    while (item == NULL) {
        if (item != &DELETED)
            if (strcmp(item->key, key) == 0) {
                deleteItem(item);
                table->items[index] = &DELETED;
            }
        index = (index + 1) % table->size;
        item = table->items[index];
    }
    table->numberOfElements--;
}
