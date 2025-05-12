#ifndef __GESTION_H__
#define __GESTION_H__

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

 int modifier_mdp(char* id, char* new_mdp);

#endif