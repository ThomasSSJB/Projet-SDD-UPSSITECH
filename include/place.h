#ifndef __PLACE_H__
#define __PLACE_H__

#include "../include/trajet.h"
#include "../include/voyageur.h"

typedef struct Place * Place;

Place init_place(Trajet, Voyageur);

#endif