#include <stdio.h>
#include <stdlib.h>
#include <cjson/cJSON.h>
#include <ncurses.h>
#include "../include/place.h"
#include "../include/trajet.h"
#include "../include/voyageur.h"
#include "../include/pile.h"
#include "../include/node.h"

int main(){
	printf("\n");

	affiche_trajet(1);
	affiche_trajet(2);
	affiche_trajet(3);
	affiche_trajet(4);
	
	printf("\n");
	return 0;
}