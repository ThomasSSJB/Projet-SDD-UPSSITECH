#ifndef __GESTION_H__
#define __GESTION_H__

#include <cjson/cJSON.h>

/**
 * @brief Permet de charger un fichier json
 * 
 * @param chemin Le chemin d'accès au fichier json à charger
 * @return Un pointeur vers la structure racine du JSON chargé, ou NULL en cas d'erreur
 */
cJSON* charger_json(char* chemin);

/**
 * @brief Permet de sauvegarder un fichier json
 * 
 * @param chemin Le chemin d'accès au fichier json à sauvegarder
 * @param racine Le pointeur vers la structure cJSON à sauvegarder
 * @return 1 en cas de succès, 0 en cas d'échec
 */
int sauvegarder_json(char* chemin, cJSON* racine);

/**
 * @brief Permet d'ajouter un utilisateur au fichier users.json
 * 
 * @param id L'identifiant entré à la connexion
 * @param mdp Le mdp entré à la connexion
 * @return 0 si les informations de connexions sont bonnes, 1 sinon
 */
int ajouter_utilisateur(char* id, char* mdp);

/**
 * @brief Permet de supprimer un utilisateur du fichier users.json
 * 
 * @param id L'identifiant à supprimer
 * @return 0 si id introuvable, 1 sinon
 */
int supprimer_utilisateur(char* id);

/**
 * @brief Permet de vérifier l'identifiant et le mot de passe d'un utilisateur lors de sa connexion
 * 
 * @param id L'identifiant entré à la connexion
 * @param mdp Le mdp entré à la connexion
 * @return 0 si les informations de connexions sont bonnes, 1 sinon
 */
 int verifier_connexion(char* id, char* mdp);

 /**
 * @brief Permet de modifier le mot de passe d'un utilisateur
 * 
 * @param id L'identifiant de l'utilisateur auquel on souhaite changer le mdp
 * @param new_mdp Le nouveau mdp
 * @return 0 , 1 sinon
 */
 int modifier_mdp(char* id, char* new_mdp);

#endif