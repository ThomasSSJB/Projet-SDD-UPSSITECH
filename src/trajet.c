#include <stdio.h>
#include <stdlib.h>
#include "../include/place.h"
#include "../include/trajet.h"
#include "../include/voyageur.h"
#include "../include/pile.h"
#include "../include/node.h"

struct Gare{
  char* nom;
  float temps;
};

struct Trajet{
  Pile gares;
};

Gare init_gare(char* nom, float temps){
  Gare G = malloc(sizeof(struct Gare));
  G->nom = nom;
  G->temps = temps;
  return G;
}

Trajet init_trajet(){
  Trajet T = malloc(sizeof(struct Trajet));
  return T;
}

void affiche_gare(Gare G){
  printf("%s (+%.1f)\n", G->nom, G->temps);
}

void affiche_trajet(Trajet T){

}