/** @file
 * Interfejs operacji wejścia
 *
 * @author Adam Rempała <ar406309@students.mimuw.edu.pl>
 * @copyright Adam Rempała
 * @date 17.08.2019
 */

#include "map.h"

#ifndef INPUT_H
#define INPUT_H


/** @brief Input.
 * Wczytuje linijki i wypisuje wyniki i ewentualne błędy. W przypadku
 * błędu alokacji pamięci na tym etapie zwalnia wszystko i zwraca 0.
 * @param[in,out] map   – wskaźnik na mapę.
*/
void input(Map *map);

#endif // INPUT_H
