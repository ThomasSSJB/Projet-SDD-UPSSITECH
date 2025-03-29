#ifndef __PILE_H__
#define __PILE_H__

#include "../include/node.h"
#include "../include/trajet.h"

typedef struct Pile * Pile;

Pile init_pile();
void affiche_pile(Pile);
int est_vide(Pile);
void empiler(Pile, Gare);
Gare depiler(Pile);
Node get_pile_top(Pile);

#endif