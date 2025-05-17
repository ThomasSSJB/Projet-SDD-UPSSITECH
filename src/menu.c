#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include "../include/place.h"
#include "../include/trajet.h"
#include "../include/voyageur.h"
#include "../include/gestion.h"
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

Voyageur VOYAGEUR_ACTUEL = NULL;

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

  OptionMenu Opt1 = init_optionmenu("| Se Connecter", menu_connexion);
  OptionMenu Opt2 = init_optionmenu("| Créer un Compte", menu_creation_compte);
  OptionMenu Opt3 = init_optionmenu("Quitter", NULL);

  OptionMenu liste_options[3] = {Opt1, Opt2, Opt3};

  Menu M = init_menu("Menu Principal", 3, liste_options);
  affiche_menu(M);
}

void affiche_menu(Menu menu) {
  int choix = 0, ch;
  int test = 1;

  while (test) {
    clear();
    mvprintw(0, 1, "=============================");
    mvprintw(1, 4, "> %s", menu->titre);
    mvprintw(2, 1, "=============================");
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
        } else  if (strcmp(menu->options[choix]->texte, "Se déconnecter") == 0) {
          free(VOYAGEUR_ACTUEL);
          endwin();
          exit(0);
          break;
        } else {
          menu->options[choix]->action();
        }
        break;
    }
  }
}

void menu_connexion() {
  init_screen();
  clear();

  char id[4] = "";
  char mdp[16] = "";

  mvprintw(0, 1, "=============================");
  mvprintw(1, 4, "> Se Connecter");
  mvprintw(2, 1, "=============================");
  mvprintw(4, 3, "| Entrez un identifiant");
  mvprintw(7, 3, "| Entrez un mot de passe");
  mvprintw(10, 3, "Valider");

  echo();
  move(5, 3);
  getnstr(id, 16);
  id[strcspn(id, "\n")] = '\0';
  move(8, 3);
  getnstr(mdp, 16);
  noecho();

  attron(A_REVERSE);
  mvprintw(10, 3, "Valider");
  attroff(A_REVERSE);

  while (1){
    int ch = getch();
    if (ch == 10) {
      attron(A_BOLD);
      mvprintw(10, 3, "Valider");
      attroff(A_BOLD);
      if (verifier_connexion(id, mdp)) {
        attron(A_REVERSE);
        mvprintw(14, 3, "Retour");
        attroff(A_REVERSE);
      } else {
        VOYAGEUR_ACTUEL = init_voyageur(id, mdp);
        if (id[0] == '0') {
          attron(A_REVERSE);
          mvprintw(14, 3, "Accéder au profil Administrateur");
          while (1) {
            ch = getch();
            if (ch == 10) {
              attroff(A_REVERSE);
              menu_administrateur();
              break;
            }
          }
        } else if (id[0] == '1') {
          attron(A_REVERSE);
          mvprintw(14, 3, "Accéder au profil Contrôleur");
          while (1) {
            ch = getch();
            if (ch == 10) {
              attroff(A_REVERSE);
              menu_controleur();
              break;
            }
          }
        } else {
          attron(A_REVERSE);
          mvprintw(14, 3, "Accéder au profil Voyageur");
          while (1) {
            ch = getch();
            if (ch == 10) {
              attroff(A_REVERSE);
              menu_voyageur();
              break;
            }
          }
        }
      } 
      while (1) {
        ch = getch();
        if (ch == 10) {
          break;
        }
      } break;
    }
  }
}

void menu_creation_compte() {
  init_screen();
  clear();

  char id[4] = "";
  char mdp1[16] = "";
  char mdp2[16] = "";

  mvprintw(0, 1, "=============================");
  mvprintw(1, 4, "> Créer un Compte");
  mvprintw(2, 1, "=============================");
  mvprintw(4, 3, "| Entrez un identifiant");
  mvprintw(7, 3, "| Entrez un mot de passe");
  mvprintw(10, 3, "| Confirmer le mot de passe");
  mvprintw(13, 3, "Valider");

  echo();
  move(5, 3);
  getnstr(id, 16);
  move(8, 3);
  getnstr(mdp1, 16);
  move(11, 3);
  getnstr(mdp2, 16);
  noecho();

  attron(A_REVERSE);
  mvprintw(13, 3, "Valider");
  attroff(A_REVERSE);

  while (1){
    int ch = getch();
    if (ch == 10) {
      attron(A_BOLD);
      mvprintw(13, 3, "Valider");
      attroff(A_BOLD);
      if (verifier_id(id)) {
        attron(A_REVERSE);
        mvprintw(20, 3, "Retour");
      } else if (strcmp(mdp1, mdp2) != 0) {
        mvprintw(15, 3, "Les mots de passe doivent être identiques.");
        attron(A_REVERSE);
        mvprintw(17, 3, "Retour");
      } else if (verifier_mdp(mdp1)) {
        attron(A_REVERSE);
        mvprintw(22, 3, "Retour");
      } else if (ajouter_utilisateur(id, mdp1)) {
        attron(A_REVERSE);
        mvprintw(17, 3, "Retour");
      } else {
        mvprintw(15, 3, "L'utilisateur %s a été enregistré avec succès !", id);
        attron(A_REVERSE);
        mvprintw(17, 3, "Retour");
      }
      while (1) {
        ch = getch();
        if (ch == 10) {
          attroff(A_REVERSE);
          break;
        }
      }
    }
    break;
  }
}

// Menus pour l'Administateur

void menu_administrateur() {
  init_screen();

  OptionMenu Opt1 = init_optionmenu("| Gestion des Trajets", menu_gestion_trajets);
  OptionMenu Opt2 = init_optionmenu("| Modifier un mot de passe", menu_modifier_mdp);
  OptionMenu Opt3 = init_optionmenu("| Supprimer un Utilisateur", menu_supprimer_user);
  OptionMenu Opt4 = init_optionmenu("| Exportation", menu_exportation);
  OptionMenu Opt5 = init_optionmenu("Se déconnecter", NULL);

  OptionMenu liste_options[5] = {Opt1, Opt2, Opt3, Opt4, Opt5};

  Menu M = init_menu("Menu Administrateur", 5, liste_options);
  affiche_menu(M);
}

void menu_gestion_trajets() {
  init_screen();

  OptionMenu Opt1 = init_optionmenu("| Ajouter un Trajet", menu_ajouter_trajet);
  OptionMenu Opt2 = init_optionmenu("| Modifier un Trajet", menu_modifier_trajet);
  OptionMenu Opt3 = init_optionmenu("| Supprimer un Trajet", menu_supprimer_trajet);
  OptionMenu Opt4 = init_optionmenu("Retour", NULL);

  OptionMenu liste_options[4] = {Opt1, Opt2, Opt3, Opt4};

  Menu M = init_menu("Gestion des Trajets", 4, liste_options);
  affiche_menu(M);
}

void menu_modifier_mdp() {
  init_screen();
  clear();

  char id[4] = "";
  char mdp[16] = "";

  mvprintw(0, 1, "=============================");
  mvprintw(1, 4, "> Modifier un mot de passe");
  mvprintw(2, 1, "=============================");
  mvprintw(4, 3, "| Entrez un identifiant");
  mvprintw(7, 3, "| Entrez le nouveau mot de passe");
  mvprintw(10, 3, "Valider");

  echo();
  move(5, 3);
  getnstr(id, 16);
  move(8, 3);
  getnstr(mdp, 16);
  noecho();

  attron(A_REVERSE);
  mvprintw(10, 3, "Valider");
  attroff(A_REVERSE);

  while (1) {
    int ch = getch();
    if (ch == 10) {
      attron(A_BOLD);
      mvprintw(10, 3, "Valider");
      attroff(A_BOLD);
      if (modifier_mdp(id, mdp)) {
        attron(A_REVERSE);
        mvprintw(22, 3, "Retour");
      } else {
        attron(A_REVERSE);
        mvprintw(14, 3, "Retour");
      }
      while (1) {
        ch = getch();
        if (ch == 10) {
          attroff(A_REVERSE);
          break;
        }
      }
      break;
    }
  }
}

void menu_supprimer_user() {
  init_screen();
  clear();

  char id[4] = "";

  mvprintw(0, 1, "=============================");
  mvprintw(1, 4, "> Supprimer un Utilisateur");
  mvprintw(2, 1, "=============================");
  mvprintw(4, 3, "| Entrez un identifiant");
  mvprintw(7, 3, "Valider");

  echo();
  move(5, 3);
  getnstr(id, 16);
  noecho();

  attron(A_REVERSE);
  mvprintw(7, 3, "Valider");
  attroff(A_REVERSE);

  while (1) {
    int ch = getch();
    if (ch == 10) {
      attron(A_BOLD);
      mvprintw(7, 3, "Valider");
      attroff(A_BOLD);
      int res = supprimer_utilisateur(id);
      attron(A_REVERSE);
      if (res == 0) {
        mvprintw(11, 3, "Retour");
      } else {
        mvprintw(14, 3, "Retour");
      }
      while (1) {
        ch = getch();
        if (ch == 10) {
          attroff(A_REVERSE);
          break;
        }
      } break;
    }
  }
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
  init_screen();
  clear();

  char id[4] = "";
  char depart[16] = "";
  char arrivee[16] = "";
  char duree[16] = "";

  mvprintw(0, 1, "=============================");
  mvprintw(1, 4, "> Ajouter un Trajet");
  mvprintw(2, 1, "=============================");
  mvprintw(4, 3, "| Entrez l'identifiant du Trajet");
  mvprintw(7, 3, "| Entrez une Gare de départ");
  mvprintw(10, 3, "| Entrez une Gare d'arrivée");
  mvprintw(13, 3, "| Entrez la durée entre ces deux gares (en h)");
  mvprintw(16, 3, "Valider");

  echo();
  move(5, 3);
  getnstr(id, 16);
  move(8, 3);
  getnstr(depart, 16);
  move(11, 3);
  getnstr(arrivee, 16);
  move(14, 3);
  getnstr(duree, 16);
  noecho();

  attron(A_REVERSE);
  mvprintw(16, 3, "Valider");
  attroff(A_REVERSE);

  while (1){
    int ch = getch();
    if (ch == 10) {
      attron(A_BOLD);
      mvprintw(16, 3, "Valider");
      attroff(A_BOLD);
      if (atoi(id) == 0) {
        mvprintw(18, 3, "Echec : l'identifiant doit être entier et différent de 0.");
      } else if (verifier_trajet(atoi(id))) {
        mvprintw(18, 3, "Echec : l'identifiant existe déjà.");
      } else if (verifier_duree(atof(duree))) {
        mvprintw(18, 3, "Echec : la durée doit être un float.");
      } else {
        Pile P = init_pile();
        Gare G_dep = init_gare(depart, 0);
        Gare G_arr = init_gare(arrivee, atof(duree));
        empiler(P, G_dep);
        empiler(P, G_arr);
        Trajet T = init_trajet(P, atoi(id));
        ajouter_trajet(T);
        mvprintw(18, 3, "Trajet enregistré avec succès !");
      }
      attron(A_REVERSE);
      mvprintw(20, 3, "Retour");
      while (1) {
        ch = getch();
        if (ch == 10) {
          attroff(A_REVERSE);
          break;
        }
      } break;
    }
  }
}

void menu_modifier_trajet() {
  init_screen();
  clear();

  char id[4] = "";

  mvprintw(0, 1, "=============================");
  mvprintw(1, 4, "> Modifier un Trajet");
  mvprintw(2, 1, "=============================");
  mvprintw(4, 3, "| Entrez l'identifiant d'un trajet");
  mvprintw(7, 3, "Valider");

  echo();
  move(5, 3);
  getnstr(id, 4);
  noecho();
  attron(A_REVERSE);
  mvprintw(7, 3, "Valider");
  attroff(A_REVERSE);
  
  while (1) {
    int ch = getch();
    if (ch == 10) {
      attron(A_BOLD);
      mvprintw(7, 3, "Valider");
      attroff(A_BOLD);
      if (verifier_trajet(atoi(id))) {
        int y = affiche_trajet_ncurses(atoi(id), 0, 9, 3);
        int selected = 0;
        y++;
        while (1) {
          // Affichage des options
          for (int i = 0; i < 3; i++) {
            if (i == selected) attron(A_REVERSE);
            if (i == 0) mvprintw(y + i, 3, "| Ajouter une Gare");
            else if (i == 1) mvprintw(y + i, 3, "| Supprimer une Gare");
            else mvprintw(y + i, 3, "Retour");
            if (i == selected) attroff(A_REVERSE);
          }
          refresh();

          ch = getch();
          switch (ch) {
            case KEY_UP:
              selected = (selected + 2) % 3;
              break;
            case KEY_DOWN:
              selected = (selected + 1) % 3;
              break;
            case 10: // Entrée
              switch (selected) {
                case 0:
                  menu_ajouter_gare(atoi(id), y);
                  return;
                case 1:
                  menu_supprimer_gare(atoi(id), y);
                  return;
                case 2:
                  return;
              }
          }
        }
      } else {
        mvprintw(9, 3, "Echec : identifiant introuvable.");
        attron(A_REVERSE);
        mvprintw(11, 3, "Retour");
        while (1) {
          ch = getch();
          if (ch == 10) {
            attroff(A_REVERSE);
            break;
          }
        }
      }
      break;
    }
  }
}

void menu_supprimer_trajet() {
  init_screen();
  clear();

  char id[4] = "";

  mvprintw(0, 1, "=============================");
  mvprintw(1, 4, "> Supprimer un Trajet");
  mvprintw(2, 1, "=============================");
  mvprintw(4, 3, "| Entrez l'identifiant d'un Trajet");
  mvprintw(7, 3, "Valider");

  echo();
  move(5, 3);
  getnstr(id, 16);
  noecho();

  attron(A_REVERSE);
  mvprintw(7, 3, "Valider");
  attroff(A_REVERSE);

  while (1) {
    int ch = getch();
    if (ch == 10) {
      attron(A_BOLD);
      mvprintw(7, 3, "Valider");
      attroff(A_BOLD);
      char res[16];
      mvprintw(9, 3, "| Tapez 1 pour confirmer");
      echo();
      move(10, 3);
      getnstr(res, 16);
      noecho();
      if (!verifier_trajet(atoi(id))) {
        mvprintw(12, 3, "Echec : identifiant introuvable.");
      } else if (strcmp(res, "1") != 0) {
        mvprintw(12, 3, "Le trajet n'a pas été supprimé.");
      } else if (supprimer_trajet(atoi(id))) {
        mvprintw(12, 3, "Le trajet a été supprimé.");
      }
      attron(A_REVERSE);
      mvprintw(14, 3, "Retour");
      while (1) {
        ch = getch();
        if (ch == 10) {
          attroff(A_REVERSE);
          break;
        }
      } break;
    }
  }
}

void menu_ajouter_gare(int id, int y) {
  char new_gare[16];
  char gare[16];
  char duree[16];
  y = y+5;
  mvprintw(y, 3, "| Entrez le nom de la nouvelle gare");
  mvprintw(y+3, 3, "| Entrez le nom de la gare après laquelle se sera ajouté");
  mvprintw(y+6, 3, "| Entrez la durée depuis la première gare (en h)");
  mvprintw(y+9, 3, "Valider");
  echo();
  move(y+1, 3);
  getnstr(new_gare, 16);
  move(y+4, 3);
  getnstr(gare, 16);
  move(y+7, 3);
  getnstr(duree, 16);
  noecho();

  attron(A_REVERSE);
  mvprintw(y+9, 3, "Valider");
  attroff(A_REVERSE);

  while (1) {
    int ch = getch();
    if (ch == 10) {
      attron(A_BOLD);
      mvprintw(y+9, 3, "Valider");
      attroff(A_BOLD);
      if (verifier_gare(id, gare)) {
        ajouter_gare(id, gare, new_gare, atof(duree));
        mvprintw(y+11, 3, "La gare a été ajoutée avec succès.");
      } else {
        mvprintw(y+11, 3, "Echec : gare introuvable.");
      }
      attron(A_REVERSE);
      mvprintw(y+13, 3, "Retour");
      while (1) {
        ch = getch();
        if (ch == 10) {
          attroff(A_REVERSE);
          break;
        }
      }
      break;
    }
  }
  for (int i=-2 ; i<100 ; i++) {
    move(y + i, 0);
    clrtoeol();
  }
  menu_modifier_trajet();
}

void menu_supprimer_gare(int id, int y) {
  char gare[16];
  y = y+5;
  mvprintw(y++, 3, "| Entrez le nom de la Gare à supprimer");
  mvprintw(y+2, 3, "Valider");
  echo();
  move(y++, 3);
  getnstr(gare, 16);
  noecho();

  attron(A_REVERSE);
  mvprintw(y+1, 3, "Valider");
  attroff(A_REVERSE);

  while (1) {
    int ch = getch();
    if (ch == 10) {
      attron(A_BOLD);
      mvprintw(y+1, 3, "Valider");
      attroff(A_BOLD);
      if (verifier_gare(id, gare)) {
        mvprintw(y+3, 3, "La gare a été supprimée avec succès.");
        supprimer_gare(id, gare);
      } else {
        mvprintw(y+3, 3, "Echec : gare introuvable.");
      }
      attron(A_REVERSE);
      mvprintw(y+5, 3, "Retour");
      while (1) {
        ch = getch();
        if (ch == 10) {
          attroff(A_REVERSE);
          break;
        }
      }
      break;
    }
  }
  for (int i=-2 ; i<100 ; i++) {
    move(y + i, 0);
    clrtoeol();
  }
  menu_modifier_trajet();
}

void menu_exporter_trajet() {
}

void menu_exporter_voyageur() {
}

// Menus pour le Contrôleur

void menu_controleur(){
  init_screen();

  OptionMenu Opt1 = init_optionmenu("| Vérifier un Voyageur", menu_verifier_voyageur);
  OptionMenu Opt2 = init_optionmenu("| Modifier son mot de passe", menu_modifier_coord);
  OptionMenu Opt3 = init_optionmenu("Se déconnecter", NULL);

  OptionMenu liste_options[3] = {Opt1, Opt2, Opt3};

  Menu M = init_menu("Menu Contôleur", 3, liste_options);
  affiche_menu(M);
}

void menu_verifier_voyageur() {

}

void menu_modifier_coord() {
  init_screen();
  clear();

  char id[4] = "";
  char new_mdp[16] = "";

  mvprintw(0, 1, "=============================");
  mvprintw(1, 4, "> Modifier votre mot de passe");
  mvprintw(2, 1, "=============================");
  mvprintw(4, 3, "| Entrez votre identifiant");
  mvprintw(7, 3, "| Entrez votre nouveau mot de passe");
  mvprintw(10, 3, "Valider");

  echo();
  move(5, 3);
  getnstr(id, 16);
  move(8, 3);
  getnstr(new_mdp, 16);
  noecho();

  attron(A_REVERSE);
  mvprintw(10, 3, "Valider");
  attroff(A_REVERSE);

  int y_retour = 14;
  while (1) {
    int ch = getch();
    if (ch == 10) {
      attron(A_BOLD);
      mvprintw(10, 3, "Valider");
      attroff(A_BOLD);
      if (strcmp(id, getId(VOYAGEUR_ACTUEL)) != 0) {
        mvprintw(12, 3, "Echec : identifiant incorrect.");
      } else if (modifier_mdp(id, new_mdp)) {
        y_retour = 22;
      } else {
        changer_mdp(VOYAGEUR_ACTUEL, new_mdp);
      }
      attron(A_REVERSE);
      mvprintw(y_retour, 3, "Retour");
      while (1) {
        ch = getch();
        if (ch == 10) {
          attroff(A_REVERSE);
          break;
        }
      }
      break;
    }
  }
}

// Menus pour le Voyageur

void menu_voyageur() {
  init_screen();

  OptionMenu Opt1 = init_optionmenu("| Rechercher un Trajet", menu_rechercher_trajet);
  OptionMenu Opt2 = init_optionmenu("| Voir tous les Trajets", menu_voir_trajets);
  OptionMenu Opt3 = init_optionmenu("| Réservation", menu_reservation);
  OptionMenu Opt4 = init_optionmenu("Se déconnecter", NULL);

  OptionMenu liste_options[4] = {Opt1, Opt2, Opt3, Opt4};

  Menu M = init_menu("Menu Voyageur", 4, liste_options);
  affiche_menu(M);
}

void menu_rechercher_trajet() {
  init_screen();
  clear();

  char depart[16] = "";
  char arrivee[16] = "";

  mvprintw(0, 1, "=============================");
  mvprintw(1, 4, "> Rechercher un Trajet");
  mvprintw(2, 1, "=============================");
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
  refresh();

  while (1) {
    int ch = getch();
    if (ch == 10) {
      attroff(A_REVERSE);
      attron(A_BOLD);
      mvprintw(10, 3, "Valider");
      refresh();
      attroff(A_BOLD);

      int* res = recherche_trajet(depart, arrivee);
      int y_menu = res[0] + 1;
      int selected = 0;
      int nb_options = (res[1] == 0) ? 1 : (res[1] == 1 ? 2 : 3);

      while (1) {
        // Affichage des options
        for (int i = 0; i < nb_options; i++) {
          if (i == selected) attron(A_REVERSE);
          if (res[1] == 0) {
            mvprintw(y_menu + i, 3, "Retour");
          } else if (res[1] == 1) {
            mvprintw(y_menu + i, 3, i == 0 ? "| Réserver cet itinéraire" : "Retour");
          } else {
            if (i == 0) mvprintw(y_menu + i, 3, "| Réserver l'itinéraire 1");
            else if (i == 1) mvprintw(y_menu + i, 3, "| Réserver l'itinéraire 2");
            else mvprintw(y_menu + i, 3, "Retour");
          }
          if (i == selected) attroff(A_REVERSE);
        }
        refresh();

        int input = getch();
        switch (input) {
          case KEY_UP:
            selected = (selected + nb_options - 1) % nb_options;
            break;
          case KEY_DOWN:
            selected = (selected + 1) % nb_options;
            break;
          case 10: // Entrée
            switch (res[1]) {
              case 0: // Aucun itinéraire
                return;
              case 1:
                if (selected == 0) {
                  menu_reservation();
                  return;
                } else {
                  return;
                }
              case 2:
                if (selected == 0 || selected == 1) {
                  menu_reservation();
                  return;
                } else {
                  return;
                }
            }
          break;
        }
      }
    }
  }
}


void menu_voir_trajets() {
  init_screen();
  clear();

  mvprintw(0, 1, "=============================");
  mvprintw(1, 4, "> Voir tous les Trajets");
  mvprintw(2, 1, "=============================");

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
  init_screen();
  clear();

  mvprintw(0, 1, "=============================");
  mvprintw(1, 4, "> Réservation");
  mvprintw(2, 1, "=============================");
  mvprintw(4, 1, "Retour");

  attron(A_REVERSE);
  mvprintw(4, 1, "Retour");
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

void menu_modifier_reservation() {
}

void menu_annuler_reservation() {
}