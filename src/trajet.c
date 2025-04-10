#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cjson/cJSON.h>
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
  int places_dispo;
  int id;
};

//Fonctions permettant de gérer les gares

Gare init_gare(char* nom, float temps){
  Gare G = malloc(sizeof(struct Gare));
  G->nom = nom;
  G->temps = temps;
  return G;
}

int get_trajet_id(Trajet T){
  return T->id;
}

void affiche_gare(Gare G){
  printf("%s (+%.1fh)\n", G->nom, G->temps);
}

void ajouter_gare(Trajet T, Gare G) {
  empiler(T->gares, G);
}

void supprimer_gare(Trajet T, Gare G) {
  Pile P = init_pile();
  while (! est_vide(T->gares)) {
    Gare G_temp = depiler(T->gares);
    if (G_temp != G) {
      empiler(P, G_temp);
    }
  }
  while (! est_vide(P)){
    empiler(T->gares, depiler(P));
  }
}

//Fonctions permettant de gérer les trajets

Trajet init_trajet(Pile P, int x){
  Trajet T = malloc(sizeof(struct Trajet));
  T->gares = P;
  T->id = x;
  T->places_dispo = 10;
  return T;
}


Trajet charger_trajet(int id_recherche, int est_inverse) {
  FILE* f = fopen("trajets.txt", "r");
  if (!f) {
      perror("Erreur d'ouverture du fichier trajets.txt");
      return NULL;
  }

  char ligne[256], nom_trajet[50], nom_gare[50];
  int id;
  float temps, max_temps = 0;

  Pile pile = init_pile();

  while (fgets(ligne, sizeof(ligne), f)) {
      if (ligne[0] == '#' || ligne[0] == '\n') continue;

      if (sscanf(ligne, "%d;%49[^;];%49[^;];%f", &id, nom_trajet, nom_gare, &temps) == 4 && id == id_recherche) {
          if (temps > max_temps) max_temps = temps;

          char* nom_dyn = malloc(strlen(nom_gare) + 1);
          strcpy(nom_dyn, nom_gare);
          Gare g = init_gare(nom_dyn, temps);
          empiler(pile, g);
      }
  }
  fclose(f);

  if (est_vide(pile)) {
      printf("Aucun trajet trouvé avec l’ID %d.\n", id_recherche);
      return NULL;
  }

  Trajet trajet = init_trajet(pile, id_recherche);

  if (est_inverse) {
      Pile pile_inverse = init_pile();
      while (!est_vide(pile)) {
          Gare g = depiler(pile);
          g->temps = max_temps - g->temps;
          empiler(pile_inverse, g);
      }
      trajet->gares = pile_inverse;
  }

  return trajet;
}


void affiche_trajet(int id, int inverse) {
  Trajet t = charger_trajet(id, inverse);
  if (!t) return;

  printf("Trajet %d%s : ", t->id, inverse ? " (retour)" : "");
  Pile temp = init_pile();

  while (!est_vide(t->gares)) {
      empiler(temp, depiler(t->gares));
  }

  while (!est_vide(temp)) {
      Gare g = depiler(temp);
      if (g->temps == 0) {
          printf("%s, ", g->nom);
      } else if (!est_vide(temp)) {
          printf("%s (+%.1fh), ", g->nom, g->temps);
      } else {
          printf("%s (+%.1fh)\n", g->nom, g->temps);
      }
      empiler(t->gares, g);
  }
}