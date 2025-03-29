#ifndef __VOYAGEUR_H__
#define __VOYAGEUR_H__

#include "../include/trajet.h"

typedef struct Voyageur * Voyageur;

Voyageur init_Voyageur(int, char*, Trajet);
void changer_mdp(Voyageur, char*);

//Fonction qui proposera un trajet (ou plusieurs ?) au voyageur en entrant la gare de départ et la gare d'arrivée souhaitée
Trajet recherche_trajet(Gare, Gare);
Trajet selectionner_trajet(Trajet, Voyageur);

//Fonctions qui permettront de réserver/modifier un trajet (éventuellement l'annuler aussi ou alors une fonction dédiée à l'annulation ?)
Trajet reservation_trajet(Trajet, Voyageur);
Trajet modification_trajet(Trajet, Voyageur);

#endif