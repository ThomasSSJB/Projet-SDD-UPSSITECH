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
 * @brief Get the Id object
 * 
 * @param V Un voyageur
 * @return int 
 */
int getId(Voyageur V);

/**
 * @brief Get the Gare Depart object
 * 
 * @param V Un voyageur
 * @return char* 
 */
char* getGareDepart(Voyageur V);

/**
 * @brief Get the Gare Arrivee object
 * 
 * @param V Un voyageur
 * @return char* 
 */
char* getGareArrivee(Voyageur V);

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
 * @brief Quand l'utilisateur créer son compte, il doit l'entrer un identifiant valide
 * 
 * @param id L'identifiant à vérifier
 * @return 0 si valide, 1 sinon
 */
int verifier_id(char* id);

//Fonction qui proposera un trajet (ou plusieurs ?) au voyageur en entrant la gare de départ et la gare d'arrivée souhaitée
int* recherche_trajet(char* depart, char* arrivee);
Trajet selectionner_trajet(Trajet, Voyageur);

//Fonctions qui permettront de réserver/modifier un trajet (éventuellement l'annuler aussi ou alors une fonction dédiée à l'annulation ?)
Trajet modification_trajet(Trajet, Voyageur);
Trajet annulation_trajet(Trajet, Voyageur);

void modifier_itineraire(Voyageur);

#endif