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

#endif