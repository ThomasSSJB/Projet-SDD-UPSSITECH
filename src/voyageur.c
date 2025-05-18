#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ncurses.h>
#include <cjson/cJSON.h>
#include "../include/place.h"
#include "../include/trajet.h"
#include "../include/voyageur.h"
#include "../include/gestion.h"
#include "../include/pile.h"
#include "../include/node.h"

#define MAX_LINE 256

struct Voyageur{
  char* id;
  char* mdp;
  char gare_depart[50]; 
  char gare_arrivee[50]; 
};

struct ResRecherche {
  int y_max;
  int nb_itineraires;
  char depart[16];
  char arrivee[16];
  char correspondance[16];
  int itineraire1[2][2];  // [trajet][id et inversion]
  int itineraire2[2][2];
};

Voyageur init_voyageur(char* id, char* mdp){
  Voyageur V = malloc(sizeof(struct Voyageur));
  V->id = id;
  V->mdp = mdp;
  return V;
}

ResRecherche init_resrecherche() {
  ResRecherche R = malloc(sizeof(ResRecherche));
  return R;
}

char* getMdp(Voyageur V){
  return V->mdp;
}

char* getId(Voyageur V){
  return V->id;
}

char* getGareDepart(Voyageur V){
  return V->gare_depart;
}

char* getGareArrivee(Voyageur V){
  return V->gare_arrivee;
}

int getYmax(ResRecherche R) {
  return R->y_max;
}

int getNbItineraire(ResRecherche R) {
  return R->nb_itineraires;
}

char* getDepart(ResRecherche R) {
  return R->depart;
}

char* getArrivee(ResRecherche R) {
  return R->arrivee;
}

char* getCorrespondance(ResRecherche R) {
  return R->correspondance;
}

int (*getItineraire1(ResRecherche R))[2] {
  return R->itineraire1;
}

int (*getItineraire2(ResRecherche R))[2] {
  return R->itineraire2;
}

char* nouveau_mdp(Voyageur V) {

  char temp_mdp[20];
  char confirmation[20];

  do {
      printf("Entrez un nouveau mot de passe : \n");
      scanf(" %s", temp_mdp);
  } while (verifier_mdp(temp_mdp) != 0);

  do {
      printf("Veuillez confirmer le nouveau mot de passe : \n");
      scanf(" %s", confirmation);
      if (confirmer_mdp(temp_mdp, confirmation) != 0) {
          printf("Les deux mots de passe ne sont pas identiques, veuillez réessayer.\n");
      }
  } while (confirmer_mdp(temp_mdp, confirmation) != 0);

  char* nouveau = malloc(strlen(temp_mdp) + 1);
  strcpy(nouveau, temp_mdp);

  V->mdp = nouveau ;
  return nouveau;
}

int verifier_mdp(char* mdp){
  char caracteres_speciaux[] = ",;&*+" ;
  char chiffres[] = "0123456789" ;
  char majuscules[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ" ;
  char minuscules[] = "abcdefghijklmnopqrstuvwxyz" ;
  if (strlen(mdp) < 8 || strpbrk(mdp, caracteres_speciaux) == NULL || strpbrk(mdp, chiffres) == NULL || strpbrk(mdp, majuscules) == NULL || strpbrk(mdp, minuscules) == NULL){
    mvprintw(15, 3, "Votre mot de passe doit contenir au moins : \n");
    mvprintw(16, 3, "  - 8 caractères\n");
    mvprintw(17, 3, "  - Un chiffre [0-9]\n");
    mvprintw(18, 3, "  - Une majuscule [A-Z]\n");
    mvprintw(19, 3, "  - Une minuscule [a-z]\n");
    mvprintw(20, 3, "  - 1 caractère spécial [, ; & * +]\n");
    return 1;
  } else {
    return 0;
  }
}


int confirmer_mdp(char* mdp, char* confirmation){
  return strcmp(confirmation, mdp);
}

void changer_mdp(Voyageur V, char* new_mdp) {
  V->mdp = new_mdp;
}

int verifier_id(char* id) {
  int res = 0;
  if (strlen(id) != 3) res++;

  for (int i = 0; i < 3; i++) {
    if (id[i] < '0' || id[i] > '9') res++;
  }

  if (res != 0) {
    mvprintw(15, 3, "L'identifiant doit comporter exactement 3 chiffres :");
    mvprintw(16, 3, "  - si l'id est entre 000 et 099 : administrateur");
    mvprintw(17, 3, "  - si l'id est entre 100 et 199 : contrôleur");
    mvprintw(18, 3, "  - si l'id est entre 200 et 999 : voyageur");
    return 1;
  } else {
    return 0;
  }
}

int verifier_user(char* id) {
  cJSON* racine = charger_json("JSON/users.json");
  cJSON* utilisateur = NULL;
  cJSON_ArrayForEach(utilisateur, racine) {
    cJSON* id_obj = cJSON_GetObjectItem(utilisateur, "id");
    if (id_obj && cJSON_IsString(id_obj)) {
      if (strcmp(id_obj->valuestring, id) == 0) {
        cJSON_Delete(racine);
        return 0;
      }
    }
  }
  cJSON_Delete(racine);
  return 1;
}

// Fonction qui propose un ou plusieurs trajets au voyageur en entrant la gare de départ et la gare d'arrivée souhaitée

ResRecherche recherche_trajet(char* depart, char* arrivee) {
  ResRecherche res = malloc(sizeof(*res));
  strncpy(res->depart, depart, 15);
  strncpy(res->arrivee, arrivee, 15);
  res->depart[15] = '\0';
  res->arrivee[15] = '\0';
  res->correspondance[0] = '\0';

  cJSON* racine = charger_json("JSON/trajets.json");
  cJSON* trajets_json = cJSON_GetObjectItem(racine, "trajets");

  typedef struct {
    int id;
    char gares[20][100];
    float temps[20];
    int nb_gares;
  } BufferTrajet;

  BufferTrajet trajets[10];
  int nb_trajets = 0;

  cJSON* trajet_json;
  cJSON_ArrayForEach(trajet_json, trajets_json) {
    if (nb_trajets >= 10) break;

    BufferTrajet* t = &trajets[nb_trajets];
    cJSON* id = cJSON_GetObjectItem(trajet_json, "id");
    cJSON* gares = cJSON_GetObjectItem(trajet_json, "gares");

    if (!cJSON_IsNumber(id) || !cJSON_IsArray(gares)) continue;

    t->id = id->valueint;
    t->nb_gares = 0;

    cJSON* gare;
    cJSON_ArrayForEach(gare, gares) {
      cJSON* nom = cJSON_GetObjectItem(gare, "nom");
      cJSON* temps = cJSON_GetObjectItem(gare, "temps");

      if (!cJSON_IsString(nom) || !cJSON_IsNumber(temps)) continue;

      strcpy(t->gares[t->nb_gares], nom->valuestring);
      t->temps[t->nb_gares] = temps->valuedouble;
      t->nb_gares++;
  }
    nb_trajets++;
  }

  cJSON_Delete(racine);

  int x1 = 3, y1 = 12;
  int x2 = 40, y2 = 12;

  // Recherche d'un trajet direct
  for (int i = 0; i < nb_trajets; i++) {
    BufferTrajet* t = &trajets[i];
    int idx_dep = -1, idx_arr = -1;

    for (int j = 0; j < t->nb_gares; j++) {
      if (strcmp(t->gares[j], depart) == 0) idx_dep = j;
      if (strcmp(t->gares[j], arrivee) == 0) idx_arr = j;
    }

    if (idx_dep != -1 && idx_arr != -1) {
      mvprintw(y1++, x1, "Itinéraire direct");
      mvprintw(y1++, x1, "-----------------------------");
      attron(A_BOLD);
      mvprintw(y1++, x1, "%s -> %s", depart, arrivee);
      attroff(A_BOLD);
      y1++;

      int step = (idx_dep < idx_arr) ? 1 : -1;
      mvprintw(y1++, x1, "Trajet %d%s :", t->id, step == -1 ? " (retour)" : "");

      for (int j = idx_dep; j != idx_arr + step; j += step) {
        float delta = fabs(t->temps[j] - t->temps[idx_dep]);
        if (j == idx_dep)
          mvprintw(y1++, x1, "-> | %s (+0.0h)", t->gares[j]);
        else if (j == idx_arr)
            mvprintw(y1++, x1, "<- | %s (+%.1fh)", t->gares[j], delta);
        else
          mvprintw(y1++, x1, "   | %s (+%.1fh)", t->gares[j], delta);
      }

      res->y_max = y1+1;
      res->nb_itineraires = 1;
      res->itineraire1[0][0] = t->id;
      res->itineraire1[0][1] = (idx_dep > idx_arr);
      res->itineraire1[1][0] = 0;
      res->itineraire1[1][1] = 0;

      return res;
    }
  }

  // Recherche d'itinéraires avec correspondances
  typedef struct {
    BufferTrajet* t1;
    int idx_dep, idx_inter;
    int retour1;
    BufferTrajet* t2;
    int idx_inter2, idx_arr;
    int retour2;
  } Itineraire;

  Itineraire solutions[2];
  int nbSolutions = 0;

  for (int i = 0; i < nb_trajets; i++) {
    BufferTrajet* t1 = &trajets[i];
    for (int j = 0; j < t1->nb_gares; j++) {
      if (strcmp(t1->gares[j], depart) != 0) continue;
      for (int k = 0; k < t1->nb_gares; k++) {
        if (j == k) continue;
        char* inter = t1->gares[k];

        for (int m = 0; m < nb_trajets; m++) {
          if (i == m) continue;
          BufferTrajet* t2 = &trajets[m];
          int idx_inter2 = -1, idx_arr = -1;

          for (int n = 0; n < t2->nb_gares; n++) {
            if (strcmp(t2->gares[n], inter) == 0) idx_inter2 = n;
            if (strcmp(t2->gares[n], arrivee) == 0) idx_arr = n;
          }

          if (idx_inter2 != -1 && idx_arr != -1 && nbSolutions < 2) {
            solutions[nbSolutions++] = (Itineraire){
              .t1 = t1, .idx_dep = j, .idx_inter = k, .retour1 = (j > k),
              .t2 = t2, .idx_inter2 = idx_inter2, .idx_arr = idx_arr, .retour2 = (idx_inter2 > idx_arr)
            };
          }
        }
      }
    }
  }

  if (nbSolutions == 0) {
    mvprintw(y1++, 3, "Aucun itinéraire trouvé de %s à %s.", depart, arrivee);
    res->nb_itineraires = 0;
    res->y_max = y1+1;
    return res;
  }

  for (int i = 0; i < nbSolutions; i++) {
    Itineraire it = solutions[i];
    int x = (i == 0) ? x1 : x2;
    int y = (i == 0) ? y1 : y2;

    mvprintw(y++, x, "Itinéraire %d", i + 1);
    mvprintw(y++, x, "-----------------------------");
    attron(A_BOLD);
    mvprintw(y++, x, "%s -> %s -> %s", it.t1->gares[it.idx_dep], it.t2->gares[it.idx_inter2], it.t2->gares[it.idx_arr]);
    attroff(A_BOLD);
    y++;

    mvprintw(y++, x, "Trajet %d%s :", it.t1->id, it.retour1 ? " (retour)" : "");
    for (int j = it.idx_dep; it.retour1 ? j >= it.idx_inter : j <= it.idx_inter; j += it.retour1 ? -1 : 1) {
      float delta = fabs(it.t1->temps[j] - it.t1->temps[it.idx_dep]);
      if (j == it.idx_dep)
        mvprintw(y++, x, "-> | %s (+0.0h)", it.t1->gares[j]);
      else if (j == it.idx_inter)
        mvprintw(y++, x, "<- | %s (+%.1fh)", it.t1->gares[j], delta);
      else
        mvprintw(y++, x, "   | %s (+%.1fh)", it.t1->gares[j], delta);
    }
    y++;

    mvprintw(y++, x, "Trajet %d%s :", it.t2->id, it.retour2 ? " (retour)" : "");
    for (int j = it.idx_inter2; it.retour2 ? j >= it.idx_arr : j <= it.idx_arr; j += it.retour2 ? -1 : 1) {
      float delta = fabs(it.t2->temps[j] - it.t2->temps[it.idx_inter2]);
      if (j == it.idx_inter2)
        mvprintw(y++, x, "-> | %s (+0.0h)", it.t2->gares[j]);
      else if (j == it.idx_arr)
        mvprintw(y++, x, "<- | %s (+%.1fh)", it.t2->gares[j], delta);
      else
        mvprintw(y++, x, "   | %s (+%.1fh)", it.t2->gares[j], delta);
    }

    if (i == 0) {
      y1 = y + 1;
      res->itineraire1[0][0] = it.t1->id;
      res->itineraire1[0][1] = it.retour1;
      res->itineraire1[1][0] = it.t2->id;
      res->itineraire1[1][1] = it.retour2;
      strncpy(res->correspondance, it.t1->gares[it.idx_inter], 15);
      res->correspondance[15] = '\0';
    } else {
        y2 = y + 1;
      res->itineraire2[0][0] = it.t1->id;
      res->itineraire2[0][1] = it.retour1;
      res->itineraire2[1][0] = it.t2->id;
      res->itineraire2[1][1] = it.retour2;
    }
  }

  res->nb_itineraires = nbSolutions;
  res->y_max = (y1 > y2 ? y1 : y2) + 1;
  return res;
}


// Fonctions qui permettront de réserver/modifier un trajet (éventuellement l'annuler aussi ou alors une fonction dédiée à l'annulation ?)

int ajouter_reservation(char* user_id, int itineraire[2][2], int nb_trajets, char* gare_depart, char* gare_arrivee, char* gare_correspondance) {
  cJSON* racine = charger_json("JSON/users.json");
  if (!racine) return 0;

  // Chercher l'utilisateur
  cJSON* utilisateur = NULL;
  cJSON_ArrayForEach(utilisateur, racine) {
    cJSON* id = cJSON_GetObjectItem(utilisateur, "id");
    if (id && cJSON_IsString(id) && strcmp(id->valuestring, user_id) == 0) {
      
      // Créer la réservation
      cJSON* reservation = cJSON_CreateObject();

      // Ajouter les trajets depuis l'itinéraire
      cJSON* trajets_array = cJSON_CreateArray();
      for (int i = 0; i < nb_trajets; i++) {
        cJSON* trajet = cJSON_CreateObject();
        cJSON_AddNumberToObject(trajet, "id", itineraire[i][0]);
        cJSON_AddNumberToObject(trajet, "inversion", itineraire[i][1]);
        cJSON_AddItemToArray(trajets_array, trajet);
      }
      cJSON_AddItemToObject(reservation, "trajets", trajets_array);

      cJSON_AddStringToObject(reservation, "gare_depart", gare_depart);
      cJSON_AddStringToObject(reservation, "gare_arrivee", gare_arrivee);

      if (nb_trajets == 2 && gare_correspondance != NULL) {
        cJSON_AddStringToObject(reservation, "gare_correspondance", gare_correspondance);
      }

      // Ajouter la réservation à la liste
      cJSON* reservations = cJSON_GetObjectItem(utilisateur, "reservations");
      if (!reservations || !cJSON_IsArray(reservations)) {
        reservations = cJSON_CreateArray();
        cJSON_AddItemToObject(utilisateur, "reservations", reservations);
      }
      cJSON_AddItemToArray(reservations, reservation);

      // Sauvegarder
      int res = sauvegarder_json("JSON/users.json", racine);
      cJSON_Delete(racine);
      return res;
    }
  }

  cJSON_Delete(racine);
  return 0; // Utilisateur non trouvé
}

void supprimer_reservation(char* id_user, int index) {
  cJSON* racine = charger_json("JSON/users.json");
  if (!racine) return;

  cJSON* utilisateur = NULL;
  cJSON_ArrayForEach(utilisateur, racine) {
    cJSON* id = cJSON_GetObjectItem(utilisateur, "id");
    if (id && cJSON_IsString(id) && strcmp(id->valuestring, id_user) == 0) {
      cJSON* reservations = cJSON_GetObjectItem(utilisateur, "reservations");
      if (!reservations || !cJSON_IsArray(reservations)) break;

      int taille = cJSON_GetArraySize(reservations);
      if (index < 0 || index >= taille) break;

      // Supprimer la réservation à l'index donné
      cJSON_DeleteItemFromArray(reservations, index);

      // Si plus aucune réservation, supprimer le champ "reservations"
      if (cJSON_GetArraySize(reservations) == 0) {
        cJSON_DeleteItemFromObject(utilisateur, "reservations");
      }

      sauvegarder_json("JSON/users.json", racine);
      break;
    }
  }

  cJSON_Delete(racine);
}

int verifierReservation(char* id_user, int itineraire[2][2], int nb_trajets) {
  cJSON* racine = charger_json("JSON/users.json");
  if (!racine) return 0;

  cJSON* utilisateur = NULL;
  cJSON_ArrayForEach(utilisateur, racine) {
    cJSON* id = cJSON_GetObjectItem(utilisateur, "id");
    if (id && cJSON_IsString(id) && strcmp(id->valuestring, id_user) == 0) {
      cJSON* reservations = cJSON_GetObjectItem(utilisateur, "reservations");

      if (!reservations || !cJSON_IsArray(reservations)) {
        cJSON_Delete(racine);
        return 0; // Pas de réservation existante
      }

      cJSON* reservation = NULL;
      cJSON_ArrayForEach(reservation, reservations) {
        cJSON* trajets = cJSON_GetObjectItem(reservation, "trajets");

        if (trajets && cJSON_IsArray(trajets) && cJSON_GetArraySize(trajets) == nb_trajets) {
          int match = 1;
          for (int i = 0; i < nb_trajets; i++) {
            cJSON* trajet = cJSON_GetArrayItem(trajets, i);
            cJSON* id = cJSON_GetObjectItem(trajet, "id");
            cJSON* inversion = cJSON_GetObjectItem(trajet, "inversion");

            if (!id || !inversion || !cJSON_IsNumber(id) || !cJSON_IsNumber(inversion)) {
              match = 0;
              break;
            }

            if (id->valueint != itineraire[i][0] || inversion->valueint != itineraire[i][1]) {
              match = 0;
              break;
            }
          }

          if (match) {
            cJSON_Delete(racine);
            return 1; // Réservation déjà existante
          }
        }
      }
      break;
    }
  }

  cJSON_Delete(racine);
  return 0; // Réservation inexistante
}

int getNombreReservation(char* id_user) {
  cJSON* racine = charger_json("JSON/users.json");
  if (!racine) return 0;

  cJSON* utilisateur = NULL;
  cJSON_ArrayForEach(utilisateur, racine) {
      cJSON* id = cJSON_GetObjectItem(utilisateur, "id");
      if (id && cJSON_IsString(id) && strcmp(id->valuestring, id_user) == 0) {
        cJSON* reservations = cJSON_GetObjectItem(utilisateur, "reservations");
        if (reservations && cJSON_IsArray(reservations)) {
          int nb = cJSON_GetArraySize(reservations);
          cJSON_Delete(racine);
          return nb;
        }
      }
  }

  cJSON_Delete(racine);
  return 0;
}

ResRecherche charger_resrecherche(char* id_user, int index) {
  cJSON* racine = charger_json("JSON/users.json");
  if (!racine) return NULL;

  cJSON* utilisateur = NULL;
  cJSON_ArrayForEach(utilisateur, racine) {
    cJSON* id = cJSON_GetObjectItem(utilisateur, "id");
    if (id && cJSON_IsString(id) && strcmp(id->valuestring, id_user) == 0) {
      cJSON* reservations = cJSON_GetObjectItem(utilisateur, "reservations");
      if (!reservations || !cJSON_IsArray(reservations)) break;

      cJSON* res_obj = cJSON_GetArrayItem(reservations, index);
      if (!res_obj) break;

      // Correction ici : c'était "reservations" au lieu de "trajets"
      cJSON* trajets = cJSON_GetObjectItem(res_obj, "trajets");
      if (!trajets || !cJSON_IsArray(trajets)) break;

      ResRecherche res = malloc(sizeof(struct ResRecherche));
      if (!res) break;

      res->nb_itineraires = cJSON_GetArraySize(trajets);
      res->y_max = 0;
      strcpy(res->depart, "");
      strcpy(res->arrivee, "");
      strcpy(res->correspondance, "");

      cJSON* dep = cJSON_GetObjectItem(res_obj, "gare_depart");
      cJSON* arr = cJSON_GetObjectItem(res_obj, "gare_arrivee");
      cJSON* cor = cJSON_GetObjectItem(res_obj, "gare_correspondance");

      if (dep && cJSON_IsString(dep)) strncpy(res->depart, dep->valuestring, 15);
      if (arr && cJSON_IsString(arr)) strncpy(res->arrivee, arr->valuestring, 15);
      if (cor && cJSON_IsString(cor)) strncpy(res->correspondance, cor->valuestring, 15);

      // Remplir itinéraires
      for (int i = 0; i < res->nb_itineraires && i < 2; i++) {
        cJSON* trajet_info = cJSON_GetArrayItem(trajets, i);
        if (!trajet_info) continue;

        cJSON* id_trajet = cJSON_GetObjectItem(trajet_info, "id");
        cJSON* inversion = cJSON_GetObjectItem(trajet_info, "inversion");

        if (!cJSON_IsNumber(id_trajet) || !cJSON_IsNumber(inversion)) continue;

        if (i == 0) {
          res->itineraire1[0][0] = id_trajet->valueint;
          res->itineraire1[0][1] = inversion->valueint;
        } else if (i == 1) {
          res->itineraire2[0][0] = id_trajet->valueint;
          res->itineraire2[0][1] = inversion->valueint;
        }
      }
      cJSON_Delete(racine);
      return res;
    }
  }
  cJSON_Delete(racine);
  return NULL;
}
