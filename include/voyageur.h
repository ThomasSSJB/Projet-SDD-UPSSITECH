#ifndef __VOYAGEUR_H__
#define __VOYAGEUR_H__

#include "../include/trajet.h"

typedef struct Voyageur * Voyageur;

/**
 * @brief Initialise un voyageur
 * 
 * @param id Identifiant du voyageur
 * @return Voyageur 
 */
Voyageur init_Voyageur(int id);

/**
 * @brief Récupère le mdp d'un voyageur
 * 
 * @param V Un voyageur
 * @return char* 
 */
char* getMdp(Voyageur V);

/**
 * @brief Permet de créer un nouveau mdp pour un voyageur
 * 
 * @param V Un voyageur
 * @return char* 
 */
char* nouveau_mdp(Voyageur V);

/**
 * @brief Permet de vérifier que le mdp est conforme (8 caractères, un chiffres, ...)
 * 
 * @param mdp Un mot de passe
 * @return int 
 */
int verifier_mdp(char* mdp);

/**
 * @brief Quand l'utilisateur créer ou change de mdp, il doit l'entrer deux fois pour le confirmer
 * 
 * @param mdp Le mot de passe souhaité
 * @param confirmation La confirmation
 * @return int 
 */
int confirmer_mdp(char* mdp, char* confirmation);

/**
 * @brief Permet à un voyageur de changer de mdp
 * 
 * @param V 
 */
void changer_mdp(Voyageur V);

/**
 * @brief Permet de vérifier l'identifiant et le mot de passe d'un voyageur lors de sa connexion
 * 
 * @param V Un voyageur
 * @param mdp_connexion Le mdp entré à la connexion
 * @param id_connexion L'identifiant entré à la connexion
 * @return 0 si les infotmations de connections sont bonnes, 1 sinon
 */
int verifier_connexion(Voyageur V, char* mdp_connexion, int id_connexion);

//Fonction qui proposera un trajet (ou plusieurs ?) au voyageur en entrant la gare de départ et la gare d'arrivée souhaitée
Trajet recherche_trajet(Gare, Gare);
Trajet selectionner_trajet(Trajet, Voyageur);

//Fonctions qui permettront de réserver/modifier un trajet (éventuellement l'annuler aussi ou alors une fonction dédiée à l'annulation ?)
Trajet modification_trajet(Trajet, Voyageur);
Trajet annulation_trajet(Trajet, Voyageur);

void modifier_itineraire(Voyageur);

#endif