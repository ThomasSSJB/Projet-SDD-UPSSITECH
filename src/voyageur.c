#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
      return NULL;
  }

  Trajet* resultats = malloc(2 * sizeof(Trajet));

  char line[MAX_LINE];
  int id;
  char nom_trajet[100], gare[100];
  float temps;

  typedef struct {
      int id;
      char nom[100];
      char gares[20][100];
      float temps[20];
      int nb_gares;
  } BufferTrajet;

  BufferTrajet trajets[10];
  int nb_trajets = 0;

  fgets(line, MAX_LINE, f); // skip header

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

  int trouve = 0;
  int y = 12;
  int x = 3;

  // Essai en un seul trajet
  for (int i = 0; i < nb_trajets; i++) {
      BufferTrajet* t = &trajets[i];
      int idx_dep = -1, idx_arr = -1;

      for (int j = 0; j < t->nb_gares; j++) {
          if (strcmp(t->gares[j], depart) == 0)
              idx_dep = j;
          if (strcmp(t->gares[j], arrivee) == 0)
              idx_arr = j;
      }

      if (idx_dep != -1 && idx_arr != -1) {
          trouve = 1;
          mvprintw(y++, x, "Trajet %d%s :", t->id, idx_dep < idx_arr ? "" : " (retour)");

          if (idx_dep < idx_arr) {
              for (int j = 0; j < t->nb_gares; j++) {
                  if (j == idx_dep)
                      mvprintw(y++, x, "-> | %s (+%.1fh)", t->gares[j], t->temps[j] - t->temps[idx_dep]);
                  else if (j == idx_arr)
                      mvprintw(y++, x, "<- | %s (+%.1fh)", t->gares[j], t->temps[j] - t->temps[idx_dep]);
                  else if (j > idx_dep && j < idx_arr)
                      mvprintw(y++, x, "   | %s (+%.1fh)", t->gares[j], t->temps[j] - t->temps[idx_dep]);
                  else
                      mvprintw(y++, x, "   | %s", t->gares[j]);
              }
          } else {
              for (int j = t->nb_gares - 1; j >= 0; j--) {
                  if (j == idx_dep)
                      mvprintw(y++, x, "-> | %s (+%.1fh)", t->gares[j], t->temps[idx_dep] - t->temps[j]);
                  else if (j == idx_arr)
                      mvprintw(y++, x, "<- | %s (+%.1fh)", t->gares[j], t->temps[idx_dep] - t->temps[j]);
                  else if (j < idx_dep && j > idx_arr)
                      mvprintw(y++, x, "   | %s (+%.1fh)", t->gares[j], t->temps[idx_dep] - t->temps[j]);
                  else
                      mvprintw(y++, x, "   | %s", t->gares[j]);
              }
          }
          y++;
      }
  }

  // Correspondance entre deux trajets
  if (!trouve) {
      for (int i = 0; i < nb_trajets; i++) {
          BufferTrajet* t1 = &trajets[i];
          for (int j = 0; j < t1->nb_gares; j++) {
              if (strcmp(t1->gares[j], depart) == 0) {
                  for (int k = 0; k < t1->nb_gares; k++) {
                      if (j == k) continue;
                      char* inter = t1->gares[k];
                      for (int m = 0; m < nb_trajets; m++) {
                          if (i == m) continue;
                          BufferTrajet* t2 = &trajets[m];
                          int idx_inter = -1, idx_arr = -1;
                          for (int a = 0; a < t2->nb_gares; a++) {
                              if (strcmp(t2->gares[a], inter) == 0)
                                  idx_inter = a;
                              if (strcmp(t2->gares[a], arrivee) == 0)
                                  idx_arr = a;
                          }

                          if (idx_inter != -1 && idx_arr != -1) {
                              trouve = 1;

                              mvprintw(y++, x, "Trajet %d%s :", t1->id, j < k ? "" : " (retour)");
                              if (j < k) {
                                  for (int x1 = 0; x1 < t1->nb_gares; x1++) {
                                      if (x1 == j)
                                          mvprintw(y++, x, "-> | %s (+0.0h)", t1->gares[x1]);
                                      else if (x1 == k)
                                          mvprintw(y++, x, "<- | %s (+%.1fh)", t1->gares[x1], t1->temps[x1] - t1->temps[j]);
                                      else if (x1 > j && x1 < k)
                                          mvprintw(y++, x, "   | %s (+%.1fh)", t1->gares[x1], t1->temps[x1] - t1->temps[j]);
                                      else
                                          mvprintw(y++, x, "   | %s", t1->gares[x1]);
                                  }
                              } else {
                                  for (int x1 = t1->nb_gares - 1; x1 >= 0; x1--) {
                                      if (x1 == j)
                                          mvprintw(y++, x, "-> | %s (+0.0h)", t1->gares[x1]);
                                      else if (x1 == k)
                                          mvprintw(y++, x, "<- | %s (+%.1fh)", t1->gares[x1], t1->temps[j] - t1->temps[x1]);
                                      else if (x1 < j && x1 > k)
                                          mvprintw(y++, x, "   | %s (+%.1fh)", t1->gares[x1], t1->temps[j] - t1->temps[x1]);
                                      else
                                          mvprintw(y++, x, "   | %s", t1->gares[x1]);
                                  }
                              }

                              y++;
                              mvprintw(y++, x, "Trajet %d%s :", t2->id, idx_inter < idx_arr ? "" : " (retour)");

                              if (idx_inter < idx_arr) {
                                  for (int x2 = 0; x2 < t2->nb_gares; x2++) {
                                      if (x2 == idx_inter)
                                          mvprintw(y++, x, "-> | %s (+0.0h)", t2->gares[x2]);
                                      else if (x2 == idx_arr)
                                          mvprintw(y++, x, "<- | %s (+%.1fh)", t2->gares[x2], t2->temps[x2] - t2->temps[idx_inter]);
                                      else if (x2 > idx_inter && x2 < idx_arr)
                                          mvprintw(y++, x, "   | %s (+%.1fh)", t2->gares[x2], t2->temps[x2] - t2->temps[idx_inter]);
                                      else
                                          mvprintw(y++, x, "   | %s", t2->gares[x2]);
                                  }
                              } else {
                                  for (int x2 = t2->nb_gares - 1; x2 >= 0; x2--) {
                                      if (x2 == idx_inter)
                                          mvprintw(y++, x, "-> | %s (+0.0h)", t2->gares[x2]);
                                      else if (x2 == idx_arr)
                                          mvprintw(y++, x, "<- | %s (+%.1fh)", t2->gares[x2], t2->temps[idx_inter] - t2->temps[x2]);
                                      else if (x2 < idx_inter && x2 > idx_arr)
                                          mvprintw(y++, x, "   | %s (+%.1fh)", t2->gares[x2], t2->temps[idx_inter] - t2->temps[x2]);
                                      else
                                          mvprintw(y++, x, "   | %s", t2->gares[x2]);
                                  }
                              }
                              y++;
                          }
                      }
                  }
              }
          }
      }
  }

  if (!trouve) {
      mvprintw(y++, x, "Aucun itinéraire trouvé de %s à %s.", depart, arrivee);
  }

  return y;
}






Trajet selectionner_trajet(Trajet T, Voyageur V);

//Fonctions qui permettront de réserver/modifier un trajet (éventuellement l'annuler aussi ou alors une fonction dédiée à l'annulation ?)
Trajet reservation_trajet(Trajet T, Voyageur V);
Trajet modification_trajet(Trajet T, Voyageur V);