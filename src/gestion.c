#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ncurses.h>
#include <cjson/cJSON.h>
#include "../include/gestion.h"

int ajouter_utilisateur(char* id, char* mdp) {
  FILE* fichier = fopen("JSON/users.json", "r+");
  if (!fichier) {
    perror("Erreur ouverture users.json");
    return -1;
  }

  // Lire tout le contenu du fichier
  fseek(fichier, 0, SEEK_END);
  long taille = ftell(fichier);
  rewind(fichier);
  char* contenu = malloc(taille + 1);
  fread(contenu, 1, taille, fichier);
  contenu[taille] = '\0';

  cJSON* root = cJSON_Parse(contenu);
  if (!root) {
    free(contenu);
    fclose(fichier);
    return -1;
  }

  // Vérifier si l'id existe
  cJSON* utilisateur = NULL;
  cJSON_ArrayForEach(utilisateur, root) {
    cJSON* id_json = cJSON_GetObjectItem(utilisateur, "id");
    if (id_json && strcmp(id_json->valuestring, id) == 0) {
      cJSON_Delete(root);
      free(contenu);
      fclose(fichier);
      mvprintw(15, 3, "Echec : l'identifiant %s est déjà enregistré.", id);
      return 1;  // ID existe déjà
    }
  }

  // Ajouter un nouvel utilisateur
  cJSON* nouvel_utilisateur = cJSON_CreateObject();
  cJSON_AddStringToObject(nouvel_utilisateur, "id", id);
  cJSON_AddStringToObject(nouvel_utilisateur, "mdp", mdp);
  cJSON_AddItemToArray(root, nouvel_utilisateur);

  // Réécrire le fichier
  char* nouvel_contenu = cJSON_Print(root);
  freopen("JSON/users.json", "w", fichier);  // réouvrir en écriture
  fputs(nouvel_contenu, fichier);

  // Nettoyage
  cJSON_Delete(root);
  free(contenu);
  free(nouvel_contenu);
  fclose(fichier);

  return 0;
}


int verifier_connexion(char* id, char* mdp) {
  FILE* fichier = fopen("JSON/users.json", "r");
  if (!fichier) {
    perror("Erreur ouverture users.json");
    return -1;
  }

  // Lecture complète du fichier
  fseek(fichier, 0, SEEK_END);
  long taille = ftell(fichier);
  rewind(fichier);

  char* contenu = malloc(taille + 1);
  if (!contenu) {
    fclose(fichier);
    return -1;
  }

  fread(contenu, 1, taille, fichier);
  contenu[taille] = '\0';
  fclose(fichier);

  cJSON* racine = cJSON_Parse(contenu);
  free(contenu);

  if (!racine || !cJSON_IsArray(racine)) {
      cJSON_Delete(racine);
    return -1;
  }

  cJSON* utilisateur = NULL;
  cJSON_ArrayForEach(utilisateur, racine) {
    cJSON* id_json = cJSON_GetObjectItem(utilisateur, "id");
    cJSON* mdp_json = cJSON_GetObjectItem(utilisateur, "mdp");

    if (id_json && mdp_json && strcmp(id_json->valuestring, id) == 0) {
      if (strcmp(mdp_json->valuestring, mdp) == 0) {
        cJSON_Delete(racine);
        mvprintw(12, 3, "Autentification réussie !");
        return 0;  // OK
      } else {
        cJSON_Delete(racine);
        mvprintw(12, 3, "Echec : mauvais mot de passe.");
        return 1;  // Mauvais mot de passe
      }
    }
  }

  cJSON_Delete(racine);
  mvprintw(12, 3, "Echec : mauvais identifiant.");
  return 1;  // ID non trouvé
}
