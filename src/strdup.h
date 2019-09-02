/** @file
 * Interfejs kopiowania napisów
 *
 * @author Adam Rempała <ar406309@students.mimuw.edu.pl>
 * @copyright Adam Rempała
 * @date 17.08.2019
 */

#ifndef STRDUP_H
#define STRDUP_H

/** @brief Kopiuje napis do wskaźnika.
 * @param[in] src   – wskaźnik na napis.
 * @return Wskaźnik na kopię napisu.
 */
char *strdup(const char *src);

#endif // STRDUP_H
