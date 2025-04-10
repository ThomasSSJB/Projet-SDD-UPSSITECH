#ifndef __TRAJET_H__
#define __TRAJET_H__

typedef struct Gare * Gare;
typedef struct Trajet * Trajet;

//include après la déclaration des structures pour éviter les dépendances circulaires
#include "../include/pile.h"

//Fonctions permettant de gérer les gares

/**
 * @brief Initialise une gare
 * 
 * @param nom Le nom de la gare
 * @param temps Le temps depuis la gare de départ
 * @return Gare 
 */
Gare init_gare(char* nom, float temps);

/**
 * @brief Affiche une gare
 * 
 * @param G La gare a afficher
 */
void affiche_gare(Gare G);

/**
 * @brief Ajoute une gare dans un trajet
 * 
 * @param T Le trajet auquel on souahite ajouter une gare
 * @param G La gare a ajouter
 */
void ajouter_gare(Trajet T, Gare G);

/**
 * @brief Supprime une gare dans un trajet
 * 
 * @param T Le trajet auquel on souhaite supprimer une gare
 * @param G La gare a supprimer
 */
void supprimer_gare(Trajet T, Gare G);

/**
 * @brief Initialise un trajet
 * 
 * @param P La pile de gares
 * @param id L'identifiant du trajet
 * @return Trajet 
 */
Trajet init_trajet(Pile P, int x);

/* void affiche_trajet(Trajet);*/
Trajet ajouter_trajet(Trajet);
Trajet modifier_trajet(Trajet);
Trajet supprimer_trajet(Trajet);

//Fonction qui permettra de rechercher le trajet d'un voyageur avec son numéro d'identification
Trajet trajet_voyageur(int);


Trajet charger_trajet(int, int);
void affiche_trajet(int, int);

#endif