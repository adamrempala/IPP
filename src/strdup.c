/** @file
 * Implementacja kopiowania napisów
 *
 * @author Adam Rempała <ar406309@students.mimuw.edu.pl>
 * @copyright Adam Rempała
 * @date 17.08.2019
 */

#include <stdlib.h>
#include <string.h>

#include "strdup.h"

/** @brief Kopiuje napis do wskaźnika.
 * @param[in] src   – wskaźnik na napis.
 * @return Wskaźnik na kopię napisu.
 */
char *strdup(const char *src) {
    char *dst = malloc(strlen (src) + 1);
    if (dst == NULL) return NULL;
    strcpy(dst, src);
    return dst;
}
