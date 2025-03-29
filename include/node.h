#ifndef __NODE_H__
#define __NODE_H__

#include "../include/trajet.h"

typedef struct Node * Node;

Node init_node(Gare);
Node get_next_node(Node);
Gare get_node_gare(Node);
void set_next_node(Node, Node);
void set_node_gare(Node, Gare);

#endif