/**
 * @mainpage Documentation du projet Gestion de Trajets
 *
 * Ce projet permet de gérer une compagnie de chemin de fer
en permettant de créer des parcours (ensemble de trajets de gares) et la réservation de places sur des
tronçons de parcours par des voyageurs. Le programme propose 3 menus :
 * 
 * - Menu administrateur (gestion des trajets, gestion des utilisateurs)
 * - Menu contrôleur (vérification d'un voyageur)
 * - Menu voyageur (recherche, choix, réservation d'un trajet)
 *
 * Consultez les modules et fichiers pour plus de détails.
 */

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
Trajet init_trajet(Pile P, int id);

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
 * @param id_trajet L'identifiant du trajet auquel on souhaite ajouter une gare
 * @param gare La gare qui précède new_gare
 * @param new_gare La gare à ajouter
 * @param duree La duree du début du trajet jusqu'à new_gare
 */
void ajouter_gare(int id_trajet, char* gare, char* new_gare, float duree);

/**
 * @brief Supprime une gare dans un trajet
 * 
 * @param id_trajet L'identifiant du trajet auquel on souhaite supprimer une gare
 * @param gare La gare à supprimer
 */
void supprimer_gare(int id_trajet, char* gare);

/**
 * @brief Affiche une gare
 * 
 * @param G La gare à afficher
 */
void affiche_gare(Gare G);

/**
 * @brief Affiche une gare dans un menu ncurses
 * 
 * @param G La gare à afficher
 * @param y Coordonnéee y (lignes) sur le menu ncurses
 * @param x Coordonnéee x (colonnes) sur le menu ncurses
 * @return La prochaine coordonnée y libre après affichage
 */
int affiche_gare_ncurses(Gare G, int y, int x);

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
 * @param inverse Permet d'afficher soit le trajet aller (0) soit le trajet retour (1)
 */
void affiche_trajet(int id, int inverse);

/**
 * @brief Permet d'afficher un trajet dans un menu ncurses
 * 
 * @param id Identifiant du trajet voulu
 * @param inverse Permet d'afficher soit le trajet aller (0) soit le trajet retour (1)
 * @param y Coordonnéee y (lignes) sur le menu ncurses
 * @param x Coordonnéee x (colonnes) sur le menu ncurses
 * @return La prochaine coordonnée y libre après affichage
 */
int affiche_trajet_ncurses(int id, int inverse, int y, int x);

/**
 * @brief Permet de vérifier si un trajet est déjà présent dans trajets.json
 * 
 * @param id Identifiant du trajet à vérifier
 * @return 0 si ok, 1 sinon
 */
int verifier_trajet(int id);

/**
 * @brief Permet de vérifier si une gare  d'un trajet est déjà présente dans trajets.json
 * 
 * @param id Identifiant du trajet à vérifier
 * @param gare Gare à vérifier
 * @return 0 si ok, 1 sinon
 */
int verifier_gare(int id, char* gare);

/**
 * @brief Permet de vérifier si la durée entre deux gares est bien un float
 * 
 * @param duree Durée entre deux gares à vérifier
 * @return 0 si ok, 1 sinon
 */
int verifier_duree(float duree);

/**
 * @brief Permet d'ajouter un trajet au fichier trajets.json
 * 
 * @param Trajet Trajet à ajouter
 * @return 0 si ok, 1 sinon
 */
int ajouter_trajet(Trajet trajet);

/**
 * @brief Permet de modifier un trajet : gares et durées
 * 
 * @param Trajet Trajet à modifier
 * @return 0 si ok, 1 sinon
 */
int modifier_trajet(Trajet trajet);

/**
 * @brief Permet de supprimer un trajet du fichier trajets.json
 * 
 * @param id_trajet Identifiant du trajet à supprimer
 * @return 0 si ok, 1 sinon
 */
int supprimer_trajet(int id_trajet);

/**
 * @brief Permet de rechercher le trajet d'un voyageur avec son numéro d'identification
 * 
 * @param id Identifiant du voyageur à vérifier
 * @return Trajet
 */
Trajet trajet_voyageur(int id);

#endif