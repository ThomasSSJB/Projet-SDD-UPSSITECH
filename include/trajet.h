#ifndef __TRAJET_H__
#define __TRAJET_H__

typedef struct Gare * Gare;
typedef struct Trajet * Trajet;

Gare init_gare(char*, float);
Trajet init_trajet();
void affiche_gare(Gare);
void affiche_trajet(Trajet);
void ajouter_gare(Trajet);
void supprimer_gare(Trajet);

//Fonctions permettant de gérer les trajets
Trajet ajouter_trajet(Trajet);
Trajet modifier_trajet(Trajet);
Trajet supprimer_trajet(Trajet);

//Fonction qui permettra de rechercher le trajet d'un voyageur avec son numéro d'identification
Trajet trajet_voyageur(int);

#endif