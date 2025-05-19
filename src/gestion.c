#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ncurses.h>
#include <cjson/cJSON.h>
#include "../include/voyageur.h"
#include "../include/gestion.h"

cJSON* charger_json(char* chemin) {
  FILE* fichier = fopen(chemin, "r");
  if (!fichier) return NULL;

  fseek(fichier, 0, SEEK_END);
  long taille = ftell(fichier);
  rewind(fichier);

  char* contenu = malloc(taille + 1);
  fread(contenu, 1, taille, fichier);
  contenu[taille] = '\0';
  fclose(fichier);

  cJSON* racine = cJSON_Parse(contenu);
  free(contenu);
  return racine;
}

int sauvegarder_json(char* chemin, cJSON* racine) {
  char* contenu = cJSON_Print(racine);
  FILE* fichier = fopen(chemin, "w");
  if (!fichier) {
    free(contenu);
    return 0;
  }
  fputs(contenu, fichier);
  fclose(fichier);
  free(contenu);
  return 1;
}

int ajouter_utilisateur(char* id, char* mdp) {
  cJSON* root = charger_json("JSON/users.json");
  if (!root) return -1;

  cJSON* utilisateur = NULL;
  cJSON_ArrayForEach(utilisateur, root) {
    cJSON* id_json = cJSON_GetObjectItem(utilisateur, "id");
    if (id_json && strcmp(id_json->valuestring, id) == 0) {
      cJSON_Delete(root);
      mvprintw(15, 3, "Echec : l'identifiant %s est déjà enregistré.", id);
      return 1;
    }
  }

  char* mdp_chiffre = chiffrer(mdp, 10);
  cJSON* nouvel_utilisateur = cJSON_CreateObject();
  cJSON_AddStringToObject(nouvel_utilisateur, "id", id);
  cJSON_AddStringToObject(nouvel_utilisateur, "mdp", mdp_chiffre);
  cJSON_AddItemToArray(root, nouvel_utilisateur);

  if (!sauvegarder_json("JSON/users.json", root)) {
    cJSON_Delete(root);
    return -1;
  }

  cJSON_Delete(root);
  return 0;
}

int supprimer_utilisateur(char* id) {
  cJSON* racine = charger_json("JSON/users.json");
  if (!racine || !cJSON_IsArray(racine)) {
    cJSON_Delete(racine);
    return 0;
  }

  cJSON* utilisateur = NULL;
  int index = 0, trouve = 0;
  cJSON_ArrayForEach(utilisateur, racine) {
    cJSON* id_json = cJSON_GetObjectItem(utilisateur, "id");
    if (id_json && strcmp(id_json->valuestring, id) == 0) {
      cJSON_DeleteItemFromArray(racine, index);
      trouve = 1;
      break;
    }
    index++;
  }

  if (!trouve) {
    mvprintw(9, 3, "Echec : identifiant introuvable.");
    cJSON_Delete(racine);
    return 0;
  }

  char res[16];
  mvprintw(9, 3, "| Tapez 1 pour confirmer");
  move(10, 3); echo(); getnstr(res, 16); noecho();

  if (strcmp(res, "1") == 0) {
    sauvegarder_json("JSON/users.json", racine);
    mvprintw(12, 3, "L'utilisateur %s a été supprimé avec succès.", id);
  } else {
    mvprintw(12, 3, "L'utilisateur %s n'a pas été supprimé.", id);
  }

  cJSON_Delete(racine);
  return 1;
}

int verifier_connexion(char* id, char* mdp) {
  cJSON* racine = charger_json("JSON/users.json");
  if (!racine || !cJSON_IsArray(racine)) {
    cJSON_Delete(racine);
    return -1;
  }

  char* mdp_chiffre = chiffrer(mdp, 10);
  cJSON* utilisateur = NULL;
  cJSON_ArrayForEach(utilisateur, racine) {
    cJSON* id_json = cJSON_GetObjectItem(utilisateur, "id");
    cJSON* mdp_json = cJSON_GetObjectItem(utilisateur, "mdp");

    if (id_json && mdp_json && strcmp(id_json->valuestring, id) == 0) {
      if (strcmp(mdp_json->valuestring, mdp_chiffre) == 0) {
        mvprintw(12, 3, "Authentification réussie !");
        cJSON_Delete(racine);
        return 0;
      } else {
        mvprintw(12, 3, "Echec : mot de passe incorrect.");
        cJSON_Delete(racine);
        return 1;
      }
    }
  }

  mvprintw(12, 3, "Echec : identifiant introuvable.");
  cJSON_Delete(racine);
  return 1;
}

int modifier_mdp(char* id, char* new_mdp) {
  cJSON* racine = charger_json("JSON/users.json");
  if (!racine || !cJSON_IsArray(racine)) {
    cJSON_Delete(racine);
    return 0;
  }

  cJSON* utilisateur = NULL;
  int trouve = 0;
  cJSON_ArrayForEach(utilisateur, racine) {
    cJSON* id_json = cJSON_GetObjectItem(utilisateur, "id");
    if (id_json && strcmp(id_json->valuestring, id) == 0) {
      if (verifier_mdp(new_mdp)) {
        mvprintw(11, 3, "______________________");
        mvprintw(13, 3, "Echec :");
        cJSON_Delete(racine);
        return 1;
      }
      cJSON* mdp_json = cJSON_GetObjectItem(utilisateur, "mdp");
      char* mdp_chiffre = chiffrer(new_mdp, 10);
      if (mdp_json) {
        cJSON_SetValuestring(mdp_json, mdp_chiffre);
        trouve = 1;
        break;
      }
    }
  }

  if (!trouve) {
    mvprintw(12, 3, "Echec : identifiant introuvable.");
    cJSON_Delete(racine);
    return 0;
  }

  sauvegarder_json("JSON/users.json", racine);
  mvprintw(12, 3, "Mot de passe modifié avec succès !");
  cJSON_Delete(racine);
  return 0;
}

char* chiffrer(char* mdp, int cle) {
  int taille = strlen(mdp);
  char* new_mdp = malloc(taille + 1);
  for (int i=0 ; i<taille ; i++) {
    new_mdp[i] = mdp[i] + cle;
  }
  new_mdp[taille] = '\0';
  return new_mdp;
}