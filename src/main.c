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

	Pile gares = init_pile();
	Trajet T3 = init_trajet(gares, 3);
	ajouter_gare(T3, G1);
	ajouter_gare(T3, G2);
	ajouter_gare(T3, G3);

	affiche_trajet(T3);

	printf("\n");
	return 0;
}