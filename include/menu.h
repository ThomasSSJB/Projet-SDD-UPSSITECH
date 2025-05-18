#ifndef __MENU_H__
#define __MENU_H__

#include "../include/voyageur.h"

typedef struct OptionMenu * OptionMenu;
typedef struct Menu * Menu;

// Initialisation des structures
OptionMenu init_optionmenu();
Menu init_menu();

// Menus Principaux
void init_screen();
void menu_principal();
void affiche_menu(Menu);
void menu_connexion();
void menu_creation_compte();

// Menus pour l'Administateur
void menu_administrateur();
void menu_gestion_trajets();
void menu_modifier_mdp();
void menu_supprimer_user();
void menu_exportation();

void menu_ajouter_trajet();
void menu_modifier_trajet();
void menu_supprimer_trajet();
void menu_ajouter_gare(int, int);
void menu_supprimer_gare(int, int);

void menu_exporter_trajet();
void menu_exporter_voyageur();

// Menus pour le Contrôleur
void menu_controleur();
void menu_verifier_voyageur();
void menu_modifier_coord();

// Menus pour le Voyageur
void menu_voyageur();
void menu_rechercher_trajet();
void menu_voir_trajets();
void menu_reservation();
void menu_annuler_reservation(int);

#endif