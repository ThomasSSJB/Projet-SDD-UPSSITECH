#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include "../include/place.h"
#include "../include/trajet.h"
#include "../include/voyageur.h"
#include "../include/pile.h"
#include "../include/node.h"

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

Trajet recherche_trajet(Gare G1, Gare G2);
Trajet selectionner_trajet(Trajet T, Voyageur V);

//Fonctions qui permettront de réserver/modifier un trajet (éventuellement l'annuler aussi ou alors une fonction dédiée à l'annulation ?)

Trajet reservation_trajet(Trajet T, Voyageur V);
Trajet modification_trajet(Trajet T, Voyageur V);