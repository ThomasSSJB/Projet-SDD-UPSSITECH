#ifndef __PILE_H__
#define __PILE_H__

#include "../include/node.h"
#include "../include/trajet.h"

typedef struct Pile * Pile;

/**
 * @brief Permet d'initialiser une pile
 * 
 * @return Pile 
 */
Pile init_pile();

/**
 * @brief Permet d'afficher une pile
 * 
 * @param P Une pile
 */
void affiche_pile(Pile P);

/**
 * @brief Permet de tester si une pile est vide
 * 
 * @param P Une pile
 * @return 1 si la pile est vide, 0 sinon
 */
int est_vide(Pile P);

/**
 * @brief Permet d'ajouter un élément au sommet de la pile
 * 
 * @param P Une pile
 * @param G Une gare qu'on souhaite empiler
 */
void empiler(Pile P, Gare G);

/**
 * @brief Permet de dépiler l'éméent en haut de la pile
 * 
 * @param P Une pile
 * @return Gare 
 */
Gare depiler(Pile P);

/**
 * @brief Permet d'obtenir l'élément au sommet de la pile
 * 
 * @param P Une pile
 * @return Node 
 */
Node get_pile_top(Pile P);

#endif