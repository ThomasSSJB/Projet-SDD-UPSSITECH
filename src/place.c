#include <stdio.h>
#include <stdlib.h>
#include "../include/place.h"
#include "../include/trajet.h"
#include "../include/voyageur.h"
#include "../include/pile.h"
#include "../include/node.h"

struct Place{
  Trajet T;
  Voyageur V;
  int etat;
};

Place init_place(Trajet T, Voyageur V, int etat){
  Place P = malloc(sizeof(struct Place));
  P->T = T;
  P->V = V;
  P->etat = 0;
  return P;
}

int estPrise(Place P){
  return P->etat;
}