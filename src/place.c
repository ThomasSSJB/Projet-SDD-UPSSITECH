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
};

Place init_place(Trajet T, Voyageur V){
  Place P = malloc(sizeof(struct Place));
  P->T = T;
  P->V = V;
  return P;
}