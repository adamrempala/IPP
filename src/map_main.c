/** @file
 * Plik main
 *
 * @author Adam Rempała <ar406309@students.mimuw.edu.pl>
 * @copyright Adam Rempała
 * @date 17.08.2019
 */

#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "input.h"

/** @brief Main.
 * Wykonuje program i zwraca 0.
 @return 0.
*/
int main() {
    Map *m = NULL;
    m = newMap();
    
    if (m == NULL)
        exit(0);

    input(m);
    deleteMap(m);
    return 0;
}
