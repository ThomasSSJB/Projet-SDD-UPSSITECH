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
Voyageur init_voyageur(char* id, char* mdp);

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
 * @return char* 
 */
char* getId(Voyageur V);

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
 * @param V  Le voyageur
 * @param new_mdp Le nouveau mdp
 */
void changer_mdp(Voyageur V, char* new_mdp);

/**
 * @brief Quand l'utilisateur créer son compte, il doit l'entrer un identifiant valide
 * 
 * @param id L'identifiant à vérifier
 * @return 0 si valide, 1 sinon
 */
int verifier_id(char* id);

/**
 * @brief Permet d'afficher un ou deux itinéraires (1 itinéraire = 1 ou 2 trajets) à partir d'une gare de départ, et d'une gare d'arrivée
 * 
 * @param depart La gare de depart
 * @param arrivee La gare d'arrivée
 * @return {y_max, nbSolutions} : y_max est la coordonnée y maximale sur laquelle quelque chose a été écrit, et nbSolution est le nombre d'itinéraires trouvés
 */
int* recherche_trajet(char* depart, char* arrivee);

//Fonctions qui permettront de réserver/modifier un trajet (éventuellement l'annuler aussi ou alors une fonction dédiée à l'annulation ?)
Trajet modification_trajet(Trajet, Voyageur);
Trajet annulation_trajet(Trajet, Voyageur);

void modifier_itineraire(Voyageur);

#endif