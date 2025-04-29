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
	menu_principal();

	// Pile P = init_pile();
	// Gare G1 = init_gare("Bordeaux", 0);
	// Gare G2 = init_gare("Bazus", 1);
	// Gare G3 = init_gare("Toulouse", 2.5);
	// Gare G4 = init_gare("Agen", 4);
	// empiler(P, G1);
	// empiler(P, G2);
	// empiler(P, G3);
	// empiler(P, G4);
	// Trajet T = init_trajet(P, 6);

	// exporter_trajet_json(T);
	return 0;
}