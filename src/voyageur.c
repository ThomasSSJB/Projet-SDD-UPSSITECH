#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ncurses.h>
#include "../include/place.h"
#include "../include/trajet.h"
#include "../include/voyageur.h"
#include "../include/pile.h"
#include "../include/node.h"

#define MAX_LINE 256

struct Voyageur{
  int id;
  char* mdp;
  char gare_depart[50];
  char gare_arrivee[50];
};


Voyageur init_Voyageur(int id){
  Voyageur V = malloc(sizeof(struct Voyageur));
  V->id = id;
  V->mdp = nouveau_mdp(V);
  return V;
}

char* getMdp(Voyageur V){
  return V->mdp;
}

int getId(Voyageur V){
  return V->id;
}

char* getGareDepart(Voyageur V){
  return V->gare_depart;
}

char* getGareArrivee(Voyageur V){
  return V->gare_arrivee;
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
    mvprintw(12, 3, "---------------------");
    mvprintw(14, 3, "Votre mot de passe doit contenir au moins : \n");
    mvprintw(15, 3, "  - 8 caractères\n");
    mvprintw(16, 3, "  - Un chiffre [0-9]\n");
    mvprintw(17, 3, "  - Une majuscule [A-Z]\n");
    mvprintw(18, 3, "  - Une minuscule [a-z]\n");
    mvprintw(19, 3, "  - 1 caractère spécial [, ; & * +]\n");
    return 1;
  } else {
    return 0;
  }
}


int confirmer_mdp(char* mdp, char* confirmation){
  return strcmp(confirmation, mdp);
}

void changer_mdp(Voyageur V) {
  char reponse[20];
  int choix;

  while (1) {
    printf("Entrez votre mot de passe actuel : \n");
    scanf(" %s", reponse);

    if (strcmp(reponse, V->mdp) == 0) {
      char* nouveau = nouveau_mdp(V);
      V->mdp = nouveau;
      printf("Votre mot de passe a bien été modifié\n");
      break;
    } else {
      printf("Mot de passe incorrect ! Tapez 1 pour réessayer, 0 si vous avez oublié votre mot de passe : \n");
      scanf("%d", &choix);
      if (choix == 0) {
        char* nouveau = nouveau_mdp(V);
        V->mdp = nouveau;
        break;
      }
    }
  }
}

int verifier_connexion(Voyageur V, char* mdp_connexion, int id_connexion){
  if (V->id == id_connexion && confirmer_mdp(V->mdp, mdp_connexion) == 0){
    return 0;
  } else {
    return 1;
  }
}

//Fonction qui proposera un trajet (ou plusieurs ?) au voyageur en entrant la gare de départ et la gare d'arrivée souhaitée

int recherche_trajet(char* depart, char* arrivee) {
  FILE* f = fopen("trajets.txt", "r");
  if (!f) {
    perror("Erreur d'ouverture du fichier trajets.txt");
    return 0;
  }

  typedef struct {
    int id;
    char nom[100];
    char gares[20][100];
    float temps[20];
    int nb_gares;
  } BufferTrajet;

  BufferTrajet trajets[10];
  int nb_trajets = 0;
  char line[MAX_LINE];
  fgets(line, MAX_LINE, f); // skip header

  int id;
  char nom_trajet[100], gare[100];
  float temps;

  while (fgets(line, MAX_LINE, f)) {
    if (sscanf(line, "%d;%99[^;];%99[^;];%f", &id, nom_trajet, gare, &temps) != 4)
      continue;

    if (nb_trajets == 0 || trajets[nb_trajets - 1].id != id) {
      trajets[nb_trajets].id = id;
      strcpy(trajets[nb_trajets].nom, nom_trajet);
      trajets[nb_trajets].nb_gares = 0;
      nb_trajets++;
    }

    BufferTrajet* t = &trajets[nb_trajets - 1];
    strcpy(t->gares[t->nb_gares], gare);
    t->temps[t->nb_gares] = temps;
    t->nb_gares++;
  }

  fclose(f);

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
      mvhline(y1++, x1, ACS_HLINE, 30);
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
    }
  }

  // Sinon : correspondances
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
    mvprintw(3, 3, "Aucun itinéraire trouvé de %s à %s.", depart, arrivee);
    return 0;
  }

  for (int i = 0; i < nbSolutions; i++) {
    Itineraire it = solutions[i];
    int x = (i == 0) ? x1 : x2;
    int y = (i == 0) ? y1 : y2;

    mvprintw(y++, x, "Itinéraire %d", i + 1);
    mvhline(y++, x, ACS_HLINE, 30);
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

    if (i==0) {
      y1 = y + 1;
    } else {
      y2 = y + 1;
    }
  }

  return (y1 > y2 ? y1 : y2);
}



Trajet selectionner_trajet(Trajet T, Voyageur V);

//Fonctions qui permettront de réserver/modifier un trajet (éventuellement l'annuler aussi ou alors une fonction dédiée à l'annulation ?)
Trajet reservation_trajet(Trajet T, Voyageur V);
Trajet modification_trajet(Trajet T, Voyageur V);