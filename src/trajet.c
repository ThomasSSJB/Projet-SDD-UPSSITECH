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

/* void affiche_trajet(Trajet T){
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
} */


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


void affiche_trajet(int id_recherche) {
  FILE* f = fopen("trajets.txt", "r");
  if (!f) {
      perror("Erreur lors de l'ouverture du fichier trajets.txt");
      return;
  }

  char ligne[256];
  int id;
  char nom_trajet[50];
  char nom_gare[50];
  float temps;

  Pile pile = init_pile();

  // Lire les gares du trajet demandé
  while (fgets(ligne, sizeof(ligne), f)) {
      if (ligne[0] == '#' || ligne[0] == '\n') continue;

      if (sscanf(ligne, "%d;%49[^;];%49[^;];%f", &id, nom_trajet, nom_gare, &temps) == 4) {
          if (id == id_recherche) {
              char* nom_gare_dyn = malloc(strlen(nom_gare) + 1);
              strcpy(nom_gare_dyn, nom_gare);
              Gare g = init_gare(nom_gare_dyn, temps);
              empiler(pile, g);
          }
      }
  }

  fclose(f);

  // Vérification si aucun arrêt trouvé
  if (est_vide(pile)) {
      printf("Aucun trajet avec l'ID %d trouvé.\n", id_recherche);
      return;
  }

  // Affichage dans le bon ordre
  printf("Trajet %d : ", id_recherche);
  Pile temp = init_pile();

  while (!est_vide(pile)) {
      Gare g = depiler(pile);
      empiler(temp, g); // Inverser l'ordre
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
  }
}
