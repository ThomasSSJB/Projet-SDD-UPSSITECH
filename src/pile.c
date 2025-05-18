#include <stdio.h>
#include <stdlib.h>
#include "../include/trajet.h"
#include "../include/voyageur.h"
#include "../include/pile.h"
#include "../include/node.h"

struct Pile {
    Node top;
};

Pile init_pile() {
    Pile P = malloc(sizeof(struct Pile));
    P->top = NULL;
    return P;
}

void affiche_pile(Pile P) {
    if (est_vide(P)) {
        printf("Pile vide\n");
    } else {
        Node n = P->top;
        while (n != NULL) {
            Gare G = get_node_gare(n);
            affiche_gare(G);
            n = get_next_node(n);
        }
        printf("------------------ Pile\n\n");
    }
}

int est_vide(Pile P) {
    return P->top == NULL;
}

void empiler(Pile P, Gare G) {
    Node N = init_node(G);
    set_next_node(N, P->top);
    P->top = N;
}

Gare depiler(Pile P) {
    if (est_vide(P)) {
        printf("Erreur : la pile est vide\n");
        exit(1);
    }
    Node n = P->top;
    Gare G = get_node_gare(n);
    P->top = get_next_node(n);
    free(n);
    return G;
}

Node get_pile_top(Pile P) {
    return P->top;
}