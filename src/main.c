#include <stdio.h>
#include <stdlib.h>
#include "../include/place.h"
#include "../include/trajet.h"
#include "../include/voyageur.h"
#include "../include/pile.h"
#include "../include/node.h"

int main(){
	printf("\n");

	Gare G1 = init_gare("Paris", 0);
	Gare G2 = init_gare("Lyon", 1.5);
	Gare G3 = init_gare("Marseille", 3);

	Pile P = init_pile();
	empiler(P, G1);
	empiler(P, G2);
	empiler(P, G3);
	affiche_pile(P);

	depiler(P);
	affiche_pile(P);

	printf("\n");
	return 0;
}