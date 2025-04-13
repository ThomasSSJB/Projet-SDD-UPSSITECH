#ifndef __NODE_H__
#define __NODE_H__

#include "../include/trajet.h"

typedef struct Node * Node;

/**
 * @brief Permet d'initialiser un nouveau node
 * 
 * @param G Une gare (les éléments des nodes sont des gares)
 * @return Node 
 */
Node init_node(Gare G);

/**
 * @brief Permet d'obtenir le node suivant
 * 
 * @param n Un node
 * @return Node 
 */
Node get_next_node(Node n);

/**
 * @brief Permet d'obtenir l'élément contenu dans un node
 * 
 * @param n Un node
 * @return Gare 
 */
Gare get_node_gare(Node n);

/**
 * @brief Permet de modifier le prochain node
 * 
 * @param n Un node
 * @param next Le node suivant
 */
void set_next_node(Node n, Node next);

/**
 * @brief Permet de modifier l'élément contenu dans le node
 * 
 * @param n Un node
 * @param G Une gare
 */
void set_node_gare(Node n, Gare G);

#endif