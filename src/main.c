#include <stdio.h>
#include <stdlib.h>
#include <cjson/cJSON.h>
#include <ncurses.h>
#include "../include/place.h"
#include "../include/trajet.h"
#include "../include/voyageur.h"
#include "../include/pile.h"
#include "../include/node.h"
#include "../include/menu.h"

int main(){

	printf("\n");

	printf("affichage des 4 trajets aller\n");
	affiche_trajet(1, 0);
	affiche_trajet(2, 0);
	affiche_trajet(3, 0);
	affiche_trajet(4, 0);

	printf("\n");

	printf("affichage des 4 trajets retour\n");
	affiche_trajet(1, 1);
	affiche_trajet(2, 1);
	affiche_trajet(3, 1);
	affiche_trajet(4, 1);

	printf("\n");
	return 0;
}