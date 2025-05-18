#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cjson/cJSON.h>
#include <ncurses.h>
#include "../include/place.h"
#include "../include/trajet.h"
#include "../include/voyageur.h"
#include "../include/gestion.h"
#include "../include/pile.h"

struct Gare{
  char* nom;
  float temps;
};

struct Trajet{
  Pile gares;
  int id;
  int places_dispo;
  int nb_voyageurs;
  Voyageur liste_voyageur[10];
};

//Fonctions permettant de gérer les gares

Gare init_gare(char* nom, float temps){
  Gare G = malloc(sizeof(struct Gare));
  G->nom = strdup(nom);
  G->temps = temps;
  return G;
}

Trajet init_trajet(Pile P, int id){
  Trajet T = malloc(sizeof(struct Trajet));
  T->gares = P;
  T->id = id;
  T->places_dispo = 10;
  T->nb_voyageurs = 0;
  return T;
}

int get_trajet_id(Trajet T){
  return T->id;
}

void ajouter_gare(int id_trajet, char* gare, char* new_gare, float duree) {
  Trajet T = charger_trajet(id_trajet, 0);
  Pile P = init_pile();
  while (!est_vide(T->gares)) {
    Gare G = depiler(T->gares);
    if (strcmp(G->nom, gare) == 0) {
      Gare new_G = init_gare(new_gare, duree);
      empiler(P, new_G);
    }
    empiler(P, G);
  }
  while (!est_vide(P)) {
    empiler(T->gares, depiler(P));
  }
  supprimer_trajet(id_trajet);
  ajouter_trajet(T);
}


void supprimer_gare(int id_trajet, char* gare) {
  Trajet T = charger_trajet(id_trajet, 0);
  Pile P = init_pile();
  while (!est_vide(T->gares)) {
    Gare G = depiler(T->gares);
    printf("G->nom = %s, gare = %s   ->   ", G->nom, gare);
    if (strcmp(G->nom, gare) == 0) {
      continue;
    } else {
      empiler(P, G);
    }
  }
  while (!est_vide(P)) {
    empiler(T->gares, depiler(P));
  }
  supprimer_trajet(id_trajet);
  ajouter_trajet(T);
}

void affiche_gare(Gare G){
  printf("%s (+%.1fh)\n", G->nom, G->temps);
}

Trajet charger_trajet(int id_recherche, int est_inverse) {
  cJSON* racine = charger_json("JSON/trajets.json");
  if (!racine) {
    printf("Erreur lors du chargement du fichier JSON.\n");
    return NULL;
  }

  cJSON* trajets = cJSON_GetObjectItem(racine, "trajets");
  if (!trajets || !cJSON_IsArray(trajets)) {
    cJSON_Delete(racine);
    printf("Erreur : l'objet 'trajets' est manquant ou invalide.\n");
    return NULL;
  }

  Pile pile = init_pile();
  float max_temps = 0.0;
  cJSON* trajet = NULL;

  // Trouver le trajet avec l'ID recherché
  cJSON_ArrayForEach(trajet, trajets) {
    cJSON* id_obj = cJSON_GetObjectItem(trajet, "id");
    if (cJSON_IsNumber(id_obj) && id_obj->valueint == id_recherche) {
      cJSON* gares = cJSON_GetObjectItem(trajet, "gares");
      if (gares && cJSON_IsArray(gares)) {
        cJSON* gare = NULL;

        cJSON_ArrayForEach(gare, gares) {
          cJSON* nom = cJSON_GetObjectItem(gare, "nom");
          cJSON* temps = cJSON_GetObjectItem(gare, "temps");

          if (cJSON_IsString(nom) && cJSON_IsNumber(temps)) {
            if (temps->valuedouble > max_temps)
              max_temps = temps->valuedouble;

            Gare g = init_gare(nom->valuestring, temps->valuedouble);
            empiler(pile, g);
          }
        }
      }
      break; // On sort après avoir trouvé le bon trajet
    }
  }

  cJSON_Delete(racine);

  if (est_vide(pile)) {
      printf("Aucun trajet trouvé avec l'ID %d.\n", id_recherche);
      return NULL;
  }

  Trajet trajet_final = init_trajet(pile, id_recherche);

  if (est_inverse) {
      Pile pile_inverse = init_pile();
      while (!est_vide(pile)) {
          Gare g = depiler(pile);
          g->temps = max_temps - g->temps;
          empiler(pile_inverse, g);
      }
      trajet_final->gares = pile_inverse;
  }
  
  return trajet_final;
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

int affiche_trajet_ncurses(int id, int inverse, int y, int x) {
  Trajet t = charger_trajet(id, inverse);
  if (!t) return 0;

  mvprintw(y, x, "Trajet %d%s : ", t->id, inverse ? " (retour)" : "");
  Pile temp = init_pile();
  y++;
  x += 3;

  while (!est_vide(t->gares)) {
    empiler(temp, depiler(t->gares));
  }

  while (!est_vide(temp)) {
      Gare g = depiler(temp);
      if (g->temps == 0) {
          mvprintw(y, x, "| %s", g->nom);
      } else {
          mvprintw(y, x, "| %s (+%.1fh)", g->nom, g->temps);
      }
      empiler(t->gares, g);
      y++;
  }
  return y;
}

int verifier_trajet(int id) {
  cJSON* racine = charger_json("JSON/trajets.json");
  if (!racine) return 0; // Impossible de charger le fichier

  cJSON* trajets = cJSON_GetObjectItem(racine, "trajets");
  if (!trajets || !cJSON_IsArray(trajets)) {
    cJSON_Delete(racine);
    return 0;
  }

  for (int i = 0; i < cJSON_GetArraySize(trajets); ++i) {
    cJSON* trajet = cJSON_GetArrayItem(trajets, i);
    cJSON* trajet_id = cJSON_GetObjectItem(trajet, "id");
    if (trajet_id && cJSON_IsNumber(trajet_id)) {
      if (trajet_id->valueint == id) {
        cJSON_Delete(racine);
        return 1; // ID déjà présent
      }
    }
  }

  cJSON_Delete(racine);
  return 0; // ID non trouvé
}

int verifier_gare(int id_trajet, char* gare) {
    if (!verifier_trajet(id_trajet)) {
      // Trajet non présent
      return 0;
    }

    cJSON* racine = charger_json("JSON/trajets.json");
    if (!racine) return 0;

    cJSON* trajets = cJSON_GetObjectItem(racine, "trajets");

    cJSON* trajet = NULL;
    cJSON_ArrayForEach(trajet, trajets) {
      cJSON* id_obj = cJSON_GetObjectItem(trajet, "id");
      if (cJSON_IsNumber(id_obj) && id_obj->valueint == id_trajet) {
        cJSON* gares = cJSON_GetObjectItem(trajet, "gares");
        if (!gares || !cJSON_IsArray(gares)) {
          cJSON_Delete(racine);
          return 0;
        }
        cJSON* gare_item = NULL;
        cJSON_ArrayForEach(gare_item, gares) {
          cJSON* nom = cJSON_GetObjectItem(gare_item, "nom");
          if (cJSON_IsString(nom) && strcmp(nom->valuestring, gare) == 0) {
            cJSON_Delete(racine);
            return 1; // Gare trouvée
          }
        }
        break; // On a trouvé le trajet, pas besoin de continuer
      }
    }
    cJSON_Delete(racine);
    return 0; // Gare non trouvée
}



int verifier_duree(float duree) {
  return duree == 0.0;
}

int ajouter_trajet(Trajet trajet) {
    cJSON* racine = charger_json("JSON/trajets.json");
    if (!racine) return 0;

    cJSON* trajets = cJSON_GetObjectItem(racine, "trajets");
    if (!trajets || !cJSON_IsArray(trajets)) {
        cJSON_Delete(racine);
        return 0;
    }

    cJSON* nouveau_trajet = cJSON_CreateObject();
    cJSON_AddNumberToObject(nouveau_trajet, "id", trajet->id);

    cJSON* liste_gares = cJSON_CreateArray();

    // On utilise une pile temporaire pour inverser sans détruire
    Pile pile_temp = init_pile();
    Node n = get_pile_top(trajet->gares);
    while (n != NULL) {
        empiler(pile_temp, get_node_gare(n));
        n = get_next_node(n);
    }

    // On reparcourt dans l’ordre d’origine
    Node courant = get_pile_top(pile_temp);
    while (courant != NULL) {
        Gare G = get_node_gare(courant);
        cJSON* gare = cJSON_CreateObject();
        cJSON_AddStringToObject(gare, "nom", G->nom);
        cJSON_AddNumberToObject(gare, "temps", G->temps);
        cJSON_AddItemToArray(liste_gares, gare);
        courant = get_next_node(courant);
    }

    cJSON_AddItemToObject(nouveau_trajet, "gares", liste_gares);
    cJSON_AddItemToArray(trajets, nouveau_trajet);

    int res = sauvegarder_json("JSON/trajets.json", racine);
    cJSON_Delete(racine);
    return res;
}


int supprimer_trajet(int id_trajet) {
    cJSON* racine = charger_json("JSON/trajets.json");
    if (!racine) return 0;

    cJSON* trajets = cJSON_GetObjectItem(racine, "trajets");
    if (!trajets || !cJSON_IsArray(trajets)) {
        cJSON_Delete(racine);
        return 0;
    }

    int index = 0;
    cJSON* trajet = NULL;
    cJSON_ArrayForEach(trajet, trajets) {
        cJSON* id = cJSON_GetObjectItem(trajet, "id");
        if (id && id->valueint == id_trajet) {
            cJSON_DeleteItemFromArray(trajets, index);
            int res = sauvegarder_json("JSON/trajets.json", racine);
            cJSON_Delete(racine);
            return res;
        }
        index++;
    }

    cJSON_Delete(racine);
    return 0;
}
