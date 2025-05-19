#include <stdio.h>
#include <stdlib.h>
#include "../include/trajet.h"
#include "../include/node.h"

struct Node {
    Gare G;
    struct Node * next;
};

Node init_node(Gare G) {
    Node n = malloc(sizeof(struct Node));
    if (n == NULL) {
        printf("Erreur : impossible d'allouer de la mémoire\n");
        exit(1);
    }
    n->G = G;
    n->next = NULL;
    return n;
}

Node get_next_node(Node n) {
    return n->next;
}

Gare get_node_gare(Node n){
    return n->G;
}

void set_next_node(Node n, Node next){
    n->next = next;
}

void set_node_gare(Node n, Gare G){
    n->G = G;
}

void liberer_node(Node n) {
    liberer_gare(n->G);
    free(n);
}
