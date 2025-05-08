#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include "../include/place.h"
#include "../include/trajet.h"
#include "../include/voyageur.h"
#include "../include/pile.h"
#include "../include/node.h"
#include "../include/menu.h"

typedef void (*FonctionMenu)();

struct OptionMenu {
  char* texte;
  FonctionMenu action;
};

struct Menu {
  char* titre;
  int nb_options;
  OptionMenu* options;
};

OptionMenu init_optionmenu(char* texte, FonctionMenu action) {
  OptionMenu Opt = malloc(sizeof(struct OptionMenu));
  Opt->texte = texte;
  Opt->action = action;
  return Opt;
}

Menu init_menu(char* titre, int nb_options, OptionMenu* options) {
  Menu M = malloc(sizeof(struct Menu));
  M->titre = titre;
  M->nb_options = nb_options;
  M->options = options;
  return M;
}


// Menus Principaux

void init_screen() {
  initscr();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
}

void menu_principal() {
  init_screen();

  OptionMenu Opt1 = init_optionmenu("| Profil Administrateur", menu_administrateur);
  OptionMenu Opt2 = init_optionmenu("| Profil Contrôleur", menu_controleur);
  OptionMenu Opt3 = init_optionmenu("| Profil Utilisateur", menu_utilisateur);
  OptionMenu Opt4 = init_optionmenu("Quitter", NULL);

  OptionMenu liste_options[4] = {Opt1, Opt2, Opt3, Opt4};

  Menu M = init_menu("Menu Principal", 4, liste_options);
  affiche_menu(M);
}

void affiche_menu(Menu menu) {
  int choix = 0, ch;
  int test = 1;

  while (test) {
    clear();
    mvprintw(0, 1, "============================");
    mvprintw(1, 4, "> %s", menu->titre);
    mvprintw(2, 1, "============================");
    for (int i = 0; i < menu->nb_options; i++) {
      if (i == choix)
        attron(A_REVERSE);
      mvprintw(i + 4, 4, "%s", menu->options[i]->texte);
      if (i == choix)
        attroff(A_REVERSE);
    }
    refresh();

    ch = getch();
    switch (ch) {
      case KEY_UP:
      choix = (choix - 1 + menu->nb_options) % menu->nb_options;
        break;
      case KEY_DOWN:
        choix = (choix + 1) % menu->nb_options;
        break;
      case 10: // Entrée
        if (strcmp(menu->options[choix]->texte, "Quitter") == 0 || strcmp(menu->options[choix]->texte, "Retour") == 0) {
          endwin();
          test = 0;
          break;
        } else {
          menu->options[choix]->action();
        }
        break;
    }
  }
}

int menu_connexion() {
  init_screen();
  clear();

  char id[16] = "";
  char mdp[16] = "";

  mvprintw(0, 1, "============================");
  mvprintw(1, 4, "> Connexion");
  mvprintw(2, 1, "============================");
  mvprintw(4, 3, "| Entrez votre identifiant");
  mvprintw(7, 3, "| Entrez votre mot de passe");
  mvprintw(10, 3, "Valider");

  echo();
  move(5, 3);
  getnstr(id, 16);

  // noecho();
  move(8, 3);
  getnstr(mdp, 16);
  verifier_mdp(mdp);

  attron(A_REVERSE);
  refresh();
  noecho();
  move(10, 3);
  attron(A_REVERSE);
  mvprintw(10, 3, "Valider");
  attroff(A_REVERSE);

  refresh();

  while (1){
    int ch = getch();
    if (ch == 10) {
      return 1;
    }
  }
}

// Menus pour l'Administateur

void menu_administrateur() {
  init_screen();

  OptionMenu Opt1 = init_optionmenu("| Gestion des Trajets", menu_gestion_trajets);
  OptionMenu Opt2 = init_optionmenu("| Administration", menu_changer_mdp);
  OptionMenu Opt3 = init_optionmenu("| Exportation", menu_exportation);
  OptionMenu Opt4 = init_optionmenu("Retour", NULL);

  OptionMenu liste_options[4] = {Opt1, Opt2, Opt3, Opt4};

  Menu M = init_menu("Menu Administrateur", 4, liste_options);
  if (menu_connexion()) {
    affiche_menu(M);
  }
}

void menu_gestion_trajets() {
  init_screen();

  OptionMenu Opt1 = init_optionmenu("| Ajouter un Trajet", menu_ajouter_trajet);
  OptionMenu Opt2 = init_optionmenu("| Modifier un Trajet", menu_modifier_trajet);
  OptionMenu Opt3 = init_optionmenu("| Supprimer un Trajet", menu_supprimer_trajet);  
  OptionMenu Opt4 = init_optionmenu("| Ajouter Gare", menu_ajouter_gare);  
  OptionMenu Opt5 = init_optionmenu("| Supprimer Gare", menu_supprimer_gare);
  OptionMenu Opt6 = init_optionmenu("Retour", NULL);

  OptionMenu liste_options[6] = {Opt1, Opt2, Opt3, Opt4, Opt5, Opt6};

  Menu M = init_menu("Gestion des Trajets", 6, liste_options);
  affiche_menu(M);
}

void menu_changer_mdp() {
}

void menu_exportation() {
  init_screen();

  OptionMenu Opt1 = init_optionmenu("| Exporter les Trajets", menu_exporter_trajet);
  OptionMenu Opt2 = init_optionmenu("| Exporter les Voyageurs", menu_exporter_voyageur);
  OptionMenu Opt3 = init_optionmenu("Retour", NULL);

  OptionMenu liste_options[3] = {Opt1, Opt2, Opt3};

  Menu M = init_menu("Exportation", 3, liste_options);
  affiche_menu(M);
}

void menu_ajouter_trajet() {
}

void menu_modifier_trajet() {
}

void menu_supprimer_trajet() {
}

void menu_ajouter_gare() {
}

void menu_supprimer_gare() {
}

void menu_exporter_trajet() {
}

void menu_exporter_voyageur() {
}

// Menus pour le Contrôleur

void menu_controleur(){
  init_screen();

  OptionMenu Opt1 = init_optionmenu("| Vérifier un Voyageur", menu_verifier_voyageur);
  OptionMenu Opt2 = init_optionmenu("| Modifier des coordonnées", menu_changer_mdp);
  OptionMenu Opt3 = init_optionmenu("Retour", NULL);

  OptionMenu liste_options[3] = {Opt1, Opt2, Opt3};

  Menu M = init_menu("Menu Contôleur", 3, liste_options);
  affiche_menu(M);
}

void menu_verifier_voyageur(){
}

void menu_modifier_coord() {
}

// Menus pour l'Utilisateur

void menu_utilisateur() {
  init_screen();

  OptionMenu Opt1 = init_optionmenu("| Rechercher un Trajet", menu_rechercher_trajet);
  OptionMenu Opt2 = init_optionmenu("| Voir tous les Trajets", menu_voir_trajets);
  OptionMenu Opt3 = init_optionmenu("| Réservation", menu_reservation);
  OptionMenu Opt4 = init_optionmenu("Retour", NULL);

  OptionMenu liste_options[4] = {Opt1, Opt2, Opt3, Opt4};

  Menu M = init_menu("Menu Utilisateur", 4, liste_options);
  affiche_menu(M);
}

void menu_rechercher_trajet() {
  init_screen();
  clear();

  char depart[16] = "";
  char arrivee[16] = "";

  mvprintw(0, 1, "============================");
  mvprintw(1, 4, "> Rechercher un Trajet");
  mvprintw(2, 1, "============================");
  mvprintw(4, 3, "| Entrez une gare de départ");
  mvprintw(7, 3, "| Entrez une gare d'arrivée");
  mvprintw(10, 3, "Valider");

  echo();
  move(5, 3);
  getnstr(depart, 16);

  move(8, 3);
  getnstr(arrivee, 16);

  refresh();
  noecho();

  attron(A_REVERSE);
  mvprintw(10, 3, "Valider");

  while (1) {
    int ch = getch();
    if (ch == 10) {
      attroff(A_REVERSE);
      attron(A_BOLD);
      mvprintw(10, 3, "Valider");
      refresh();
      attroff(A_BOLD);
      int y = recherche_trajet(depart, arrivee);
      attron(A_REVERSE);
      mvprintw(y, 3, "Retour");

      while (1) {
        int ch = getch();
        if (ch == 10) {
          attroff(A_REVERSE);
          break;
        }
      }
      break;
    }
  }
}

void menu_voir_trajets() {
  init_screen();
  clear();

  mvprintw(0, 1, "============================");
  mvprintw(1, 4, "> Voir tous les Trajets");
  mvprintw(2, 1, "============================");

  int y;
  y = affiche_trajet_ncurses(1, 0, 4, 4);
  y = affiche_trajet_ncurses(2, 0, y+1, 4);
  y = affiche_trajet_ncurses(3, 0, y+1, 4);
  y = affiche_trajet_ncurses(4, 0, y+1, 4);

  y = affiche_trajet_ncurses(1, 1, 4, 38);
  y = affiche_trajet_ncurses(2, 1, y+1, 38);
  y = affiche_trajet_ncurses(3, 1, y+1, 38);
  y = affiche_trajet_ncurses(4, 1, y+1, 38);

  attron(A_REVERSE);
  mvprintw(y+1, 4, "Retour");
  refresh();
  while (1) {
    int ch = getch();
    if (ch == 10) {
      attroff(A_REVERSE);
      endwin();
      break;
    }
  }
}

void menu_reserver_trajet() { 
}

void menu_reservation() { 
}

void menu_modifier_reservation() {
}

void menu_annuler_reservation() {

}