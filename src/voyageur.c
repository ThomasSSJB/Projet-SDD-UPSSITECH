#include <stdio.h>
#include <stdlib.h>
#include "../include/place.h"
#include "../include/trajet.h"
#include "../include/voyageur.h"
#include "../include/pile.h"
#include "../include/node.h"

struct Voyageur{
  int id;
  char* mdp;
  Trajet T;
};

Voyageur init_Voyageur(int id, char* mdp, Trajet T){
  Voyageur V = malloc(sizeof(struct Voyageur));
  V->id = id;
  V->mdp = mdp;
  V->T = T;
  return V;
}

void changer_mdp(Voyageur V, char* new_mdp){
  V->mdp = new_mdp;
}