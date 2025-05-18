#ifndef __MENU_H__
#define __MENU_H__

#include "../include/voyageur.h"

typedef struct OptionMenu * OptionMenu;
typedef struct Menu * Menu;

// Initialisation des structures

/**
 * @brief Initialise la structure OptionMenu
 * 
 * @return OptionMenu
 */
OptionMenu init_optionmenu();

/**
 * @brief Initialise la structure Menu
 * 
 * @return Menu
 */
Menu init_menu();

// Menus Principaux

/**
 * @brief Initialise une fenêtre ncurses
 */
void init_screen();

/**
 * @brief Lance le menu menu principal
 */
void menu_principal();

/**
 * @brief Fonction générique permettant d'afficher un menu passé en paramètre
 *
 * @param Menu Menu à afficher
 */
void affiche_menu(Menu);

/**
 * @brief Lance le menu de connexion
 */
void menu_connexion();

/**
 * @brief Lance le menu de création de compte
 */
void menu_creation_compte();

// Menus pour l'Administateur

/**
 * @brief Lance le menu administrateur
 */
void menu_administrateur();

/**
 * @brief Lance le menu de gestion des trajets
 */
void menu_gestion_trajets();

/**
 * @brief Lance le menu pour modifier un mot de passe
 */
void menu_modifier_mdp();

/**
 * @brief Lance le menu pour supprimer un utilisateur
 */
void menu_supprimer_user();

/**
 * @brief Lance le menu pour ajouter un trajet
 */
void menu_ajouter_trajet();

/**
 * @brief Lance le menu pour modifier un trajet
 */
void menu_modifier_trajet();

/**
 * @brief Lance le menu pour supprimer un trajet
 */
void menu_supprimer_trajet();

/**
 * @brief Lance le menu pour ajouter une gare à un trajet
 *
 * @param id Identifiant du trajet
 * @param y Coordonnée y sur le menu ncurses
 */
void menu_ajouter_gare(int id, int y);

/**
 * @brief Lance le menu pour supprimer une gare à un trajet
 *
 * @param id Identifiant du trajet
 * @param y Coordonnée y sur le menu ncurses
 */
void menu_supprimer_gare(int id, int y);

// Menus pour le Contrôleur

/**
 * @brief Lance le menu contrôleur
 */
void menu_controleur();

/**
 * @brief Lance le menu pour vérifier un voyageur
 */
void menu_verifier_voyageur();

/**
 * @brief Lance le menu pour qui permet de modifier le mot de passe du contrôleur
 */
void menu_modifier_coord();

// Menus pour le Voyageur

/**
 * @brief Lance le menu voyageur
 */
void menu_voyageur();

/**
 * @brief Lance le menu pour rechercher des itinéraires et les réserver
 */
void menu_rechercher_trajet();

/**
 * @brief Lance le menu pour voir tous les trajets disponibles
 */
void menu_voir_trajets();

/**
 * @brief Lance le menu pour gérer les réservations
 */
void menu_reservation();

/**
 * @brief Lance le menu annuler (= supprimer) une réservation
 *
 * @param y Coordonnée y sur le menu ncurses
 */
void menu_annuler_reservation(int y);

#endif