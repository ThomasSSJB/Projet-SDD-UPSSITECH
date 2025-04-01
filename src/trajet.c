#include <stdio.h>
#include <stdlib.h>
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


void afficher_trajet_json(const char *fichier, int idRecherche) {
  // Ouvrir le fichier JSON
  FILE *file = fopen(fichier, "r");
  if (!file) {
      printf("Erreur : Impossible d'ouvrir %s\n", fichier);
      return;
  }
  
  // Lire le fichier JSON
  fseek(file, 0, SEEK_END);
  long taille = ftell(file);
  rewind(file);
  char *jsonData = (char *)malloc(taille + 1);
  fread(jsonData, 1, taille, file);
  fclose(file);
  jsonData[taille] = '\0';
  
  // Parser le JSON
  cJSON *root = cJSON_Parse(jsonData);
  if (!root) {
      printf("Erreur : JSON invalide\n");
      free(jsonData);
      return;
  }
  
  // Récupérer le tableau des trajets
  cJSON *trajets = cJSON_GetObjectItem(root, "trajets");
  if (!trajets || !cJSON_IsArray(trajets)) {
      printf("Erreur : Structure JSON invalide\n");
      cJSON_Delete(root);
      free(jsonData);
      return;
  }
  
  // Parcourir les trajets
  int nbTrajets = cJSON_GetArraySize(trajets);
  for (int i = 0; i < nbTrajets; i++) {
      cJSON *trajet = cJSON_GetArrayItem(trajets, i);
      int id = cJSON_GetObjectItem(trajet, "id")->valueint;
      
      if (id == idRecherche) {
          printf("Trajet ID %d :\n", id);
          cJSON *gares = cJSON_GetObjectItem(trajet, "gares");
          int nbGares = cJSON_GetArraySize(gares);
          
          for (int j = 0; j < nbGares; j++) {
              cJSON *gare = cJSON_GetArrayItem(gares, j);
              printf("  - %s (%.1f h)\n", 
                  cJSON_GetObjectItem(gare, "nom")->valuestring,
                  cJSON_GetObjectItem(gare, "temps")->valuedouble);
          }
          break;
      }
  }
  
  // Libérer la mémoire
  cJSON_Delete(root);
  free(jsonData);
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