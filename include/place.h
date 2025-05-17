#ifndef __PLACE_H__
#define __PLACE_H__

#include "../include/trajet.h"
#include "../include/voyageur.h"

typedef struct Place * Place;

/**
 * @brief Initialise une place
 * 
 * @param T Trajet associé à la place créée 
 * @param V Voyageur associé à la place créée
 * @param etat Indique si la place est disponible (0) ou pas (1)
 * @return Place 
 */
Place init_place(Trajet T, Voyageur V, int etat);

/**
 * @brief Test si une place est prise
 * 
 * @param P Place en question
 * @return 1 si la place est prise, 0 sinon
 */
int estPrise(Place P);


void reserver_place(Place, Voyageur);
void annuler_place(Place, Voyageur);

#endif