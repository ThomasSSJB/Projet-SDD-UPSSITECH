#ifndef __VOYAGEUR_H__
#define __VOYAGEUR_H__

#include "../include/trajet.h"

typedef struct Voyageur * Voyageur;

Voyageur init_Voyageur(int);

//Fonctions pour gérer les mdp
char* nouveau_mdp(Voyageur);
char* getMdp(Voyageur);
int verifier_mdp();
int confirmer_mdp(char*, char*);
void changer_mdp(Voyageur);

//Fonction qui proposera un trajet (ou plusieurs ?) au voyageur en entrant la gare de départ et la gare d'arrivée souhaitée
Trajet recherche_trajet(Gare, Gare);
Trajet selectionner_trajet(Trajet, Voyageur);

//Fonctions qui permettront de réserver/modifier un trajet (éventuellement l'annuler aussi ou alors une fonction dédiée à l'annulation ?)
Trajet modification_trajet(Trajet, Voyageur);
Trajet annulation_trajet(Trajet, Voyageur);

void modifier_itineraire(Voyageur);

#endif