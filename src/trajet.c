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
  int id;
};

//Fonctions permettant de gérer les gares

Gare init_gare(char* nom, float temps){
  Gare G = malloc(sizeof(struct Gare));
  G->nom = nom;
  G->temps = temps;
  return G;
}

void affiche_gare(Gare G){
  printf("%s (+%.1fh)\n", G->nom, G->temps);
}

void ajouter_gare(Trajet T, Gare G) {
  empiler(T->gares, G);
}

void supprimer_gare(Trajet T) {
  depiler(T->gares);
}

//Fonctions permettant de gérer les trajets

Trajet init_trajet(Pile P, int x){
  Trajet T = malloc(sizeof(struct Trajet));
  T->gares = P;
  T->id = x;
  return T;
}

void affiche_trajet(Trajet T){
  Pile P = init_pile();
  if (! est_vide(T->gares)) {
    printf("Trajet %d : ", T->id);
  } else {
    printf("Le trajet %d est vide.\n", T->id);
  }

  while (! est_vide(T->gares)) {
    empiler(P, depiler(T->gares));
  }

  while (! est_vide(P)) {
    Gare G = depiler(P);
    if (G->temps == 0) {
      printf("%s, ", G->nom);
    } else if (! est_vide(P)) {
      printf("%s (+%.1fh), ", G->nom, G->temps);
    } else {
      printf("%s (+%.1fh)\n", G->nom, G->temps);
    }
    empiler(T->gares, G);
  }
}

void inverse_trajet(Trajet T){
  float temps_total;
  if (! est_vide(T->gares)) {
    Gare der = depiler(T->gares);
    temps_total = der->temps;
    empiler(T->gares, der);
  }
  Pile P = init_pile();
  while (! est_vide(T->gares)) {
    Gare G = depiler(T->gares);
    G->temps = temps_total - G->temps;
    empiler(P, G);
  }
  T->gares = P;
  printf("Trajet %d inversé.\n", T->id);
}