/** @file
 * Implementacja klasy operującej wokół liczb pierwszych
 *
 * @author Adam Rempała <ar406309@students.mimuw.edu.pl>
 * @copyright Adam Rempała
 * @date 17.08.2019
 */

#include <stdbool.h>

#include "prime.h"


/** @brief Sprawdza niefaktoryzowalność liczby.
 * @param[in] x   – liczba.
 * @return Wartość @p true, jeśli liczba jest pierwsza, wartość @p false
 * w przeciwnym przypadku.
 */
bool isPrime(const int x) {
    if (x < 2)
        return false;
    
    if (x < 4)
        return true;
    
    if (x % 2 == 0)
        return false;
    
    int i = 3;
    
    while (i * i <= x) {
        if (x % i == 0)
            return false;
        
        i = i + 2;
    }
    
    return true;
}

/** @brief Szuka kolejnej liczby pierwszej.
 * @param[in] x   – liczba.
 * @return Następna liczba pierwsza.
 */
int nextPrime(int x) {
    if (x % 2 == 0)
        x++;
    while (!isPrime(x))
        x += 2;
    
    return x;
}
