#ifndef __TRAJET_H__
#define __TRAJET_H__

typedef struct Gare * Gare;
typedef struct Trajet * Trajet;

//include après la déclaration des structures pour éviter les dépendances circulaires
#include "../include/pile.h"

//Fonctions permettant de gérer les gares
Gare init_gare(char*, float);
void affiche_gare(Gare);
void ajouter_gare(Trajet, Gare);
void supprimer_gare(Trajet, Gare);

//Fonctions permettant de gérer les trajets
Trajet init_trajet(Pile, int);
void affiche_trajet(Trajet);
void inverse_trajet(Trajet);
Trajet ajouter_trajet(Trajet);
Trajet modifier_trajet(Trajet);
Trajet supprimer_trajet(Trajet);

//Fonction qui permettra de rechercher le trajet d'un voyageur avec son numéro d'identification
Trajet trajet_voyageur(int);

#endif