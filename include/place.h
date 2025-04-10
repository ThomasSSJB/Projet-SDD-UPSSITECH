#ifndef __PLACE_H__
#define __PLACE_H__

#include "../include/trajet.h"
#include "../include/voyageur.h"

typedef struct Place * Place;

Place init_place(Trajet, Voyageur, int);

//Permet de tester si une place est libre ou non | renvoie 1 si la place est prise 0 sinon
int estPrise(Place);
void reserver_place(Place, Voyageur);
void annuler_place(Place, Voyageur);

#endif