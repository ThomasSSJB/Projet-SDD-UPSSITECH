#ifndef __VOYAGEUR_H__
#define __VOYAGEUR_H__

#include "../include/trajet.h"

typedef struct Voyageur * Voyageur;
typedef struct ResRecherche * ResRecherche;

/**
 * @brief Initialise un voyageur
 * 
 * @param id Identifiant du voyageur
 * @return Voyageur 
 */
Voyageur init_voyageur(char* id, char* mdp);

/**
 * @brief Initialise un résultat de recherche de trajets
 * 
 * @return ResRecherche
 */
ResRecherche init_resrecherche();

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
 * @brief Permet de retourner le y_max de la structure ResRecherche
 * 
 * @param R Un résultat de recherche de trajets ResRecherche
 * @return int y_max
 */
int getYmax(ResRecherche R);

/**
 * @brief Permet de retourner le nbItineraire de la structure ResRecherche
 * 
 * @param R Un résultat de recherche de trajets ResRecherche
 * @return int nbItineraire
 */
int getNbItineraire(ResRecherche R);

/**
 * @brief Permet de retourner la gare de départ de la structure ResRecherche
 * 
 * @param R Un résultat de recherche de trajets ResRecherche
 * @return char* depart
 */
char* getDepart(ResRecherche R);

/**
 * @brief Permet de retourner la gare d'arrivee de la structure ResRecherche
 * 
 * @param R Un résultat de recherche de trajets ResRecherche
 * @return char* arrivee
 */
char* getArrivee(ResRecherche R);

/**
 * @brief Permet de retourner la gare de correspondance de la structure ResRecherche
 * 
 * @param R Un résultat de recherche de trajets ResRecherche
 * @return char* correspondance
 */
char* getCorrespondance(ResRecherche R);

/**
 * @brief Permet de retourner le tableau itineraire1 de la structure ResRecherche
 * 
 * @param R Un résultat de recherche de trajets ResRecherche
 * @return (int*)[2] itineraire1
 */
int (*getItineraire1(ResRecherche R))[2];

/**
 * @brief Permet de retourner le tableau itineraire2 de la structure ResRecherche
 * 
 * @param R Un résultat de recherche de trajets ResRecherche
 * @return (int*)[2] itineraire2
 */
int (*getItineraire2(ResRecherche R))[2];

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
 * @brief Permet de vérifier si un utilisateur est déjà présent dans users.json
 * 
 * @param id Identifiant de l'utilisateur à vérifier
 * @return 0 si ok, 1 sinon
 */
int verifier_user(char* id);

/**
 * @brief Permet d'afficher un ou deux itinéraires (1 itinéraire = 1 ou 2 trajets) à partir d'une gare de départ, et d'une gare d'arrivée
 * 
 * @param depart La gare de depart
 * @param arrivee La gare d'arrivée
 * @return {y_max, nbSolutions} : y_max est la coordonnée y maximale sur laquelle quelque chose a été écrit, et nbSolution est le nombre d'itinéraires trouvés
 */
ResRecherche recherche_trajet(char* depart, char* arrivee);

/**
 * @brief Permet d'ajouter une réservation à un utilisateur dans le fichier users.json
 * 
 * @param user_id Identifiant de l'utilisateur
 * @param itineraire Tableau de 2 trajets, qui eux-mêmes contiennent [id_trajet, inversion], inversion = 1 si le trajet est inversé, 0 sinon
 * @param nb_trajets Nombre de trajets trouvés (peut valoir 0, 1 ou 2)
 * @param gare_depart Gare de depart trouvée
 * @param gare_arrivee Gare d'arrivée trouvée
 * @param gare_correspondance Gare de correspondance trouvée (peut valoir "")
 * @return 0 si ok, 1 sinon
 */
int ajouter_reservation(char* user_id, int itineraire[2][2], int nb_trajets, char* gare_depart, char* gare_arrivee, char* gare_correspondance);

/**
 * @brief Permet de supprimer une réservation à un utilisateur
 * 
 * @param id_user Identifiant de l'utilisateur à vérifier
 * @param index Numéro de la réservation à supprimer
 */
void supprimer_reservation(char* id_user, int index);

/**
 * @brief Permet de vérifier si un itinéraire est déjà réservé par un utilisateur précis
 * 
 * @param id_user Identifiant de l'utilisateur à vérifier
 * @param itineraire Itinéraire à vérifier
 * @param nb_trajets Nombre de trajets
 * @return 0 si ok, 1 sinon
 */
int verifierReservation(char* id_user, int itineraire[2][2], int nb_trajets);

/**
 * @brief Permet de retourner le nombre de réservation que possède un utilisateur
 * 
 * @param id_user Identifiant de l'utilisateur
 * @return Nombre de réservations
 */
int getNombreReservation(char* id_user);

/**
 * @brief Permet de retourner une réservation parmi celles faites par un utilisateur
 * 
 * @param id_user Identifiant de l'utilisateur
 * @param index Numéro de la réservation
 * @return ResRecherche compléter
 */
ResRecherche charger_resrecherche(char* id_user, int index);

#endif