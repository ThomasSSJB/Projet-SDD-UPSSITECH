#ifndef __TRAJET_H__
#define __TRAJET_H__

typedef struct Gare * Gare;
typedef struct Trajet * Trajet;

//include après la déclaration des structures pour éviter les dépendances circulaires
#include "../include/pile.h"

/**
 * @brief Initialise une gare
 * 
 * @param nom Le nom de la gare
 * @param temps Le temps depuis la gare de départ
 * @return Gare 
 */
Gare init_gare(char* nom, float temps);

/**
 * @brief Initialise un trajet
 * 
 * @param P La pile de gares
 * @param id L'identifiant du trajet
 * @return Trajet 
 */
Trajet init_trajet(Pile P, int x);

/**
 * @brief Récupère l'id du trajet T
 * 
 * @param T Un trajet
 * @return int 
 */
int get_trajet_id(Trajet T);

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
 * @brief Affiche une gare
 * 
 * @param G La gare a afficher
 */
void affiche_gare(Gare G);

/**
 * @brief Permet de récupérer un trajet depuis le fichier "trajets.txt"
 * 
 * @param id_recherche Identifiant du trajet voulu
 * @param est_inverse Permet de récupérer soit le trajet aller(0) soit le trajet retour(1)
 * @return Trajet 
 */
Trajet charger_trajet(int id_recherche, int est_inverse);

/**
 * @brief Permet d'afficher un trajet en utilisant la fonction "charger_trajet" déclarée juste avant
 * 
 * @param id Identifiant du trajet voulu
 * @param inverse Permet d'afficher soit le trajet aller(0) soit le trajet retour(1)
 */
void affiche_trajet(int id, int inverse);



Trajet ajouter_trajet(Trajet);
Trajet modifier_trajet(Trajet);
Trajet supprimer_trajet(Trajet);

//Fonction qui permettra de rechercher le trajet d'un voyageur avec son numéro d'identification
Trajet trajet_voyageur(int);


Trajet charger_trajet(int, int);
void affiche_trajet(int, int);

//Fonctions liées aux menus
int affiche_trajet_ncurses(int, int, int, int);

#endif