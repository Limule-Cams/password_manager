#ifndef USER_H
#define USER_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<unistd.h>
#include<ctype.h>
#include<sodium.h>

typedef struct{
    unsigned char name[crypto_generichash_BYTES];
    unsigned char password[crypto_generichash_BYTES];   
    char file[10];
}Info_con;

typedef enum{
    false,
    true
}bool;

/**
 * @brief sert de fonction de connexion et de recherche à la fois
 * 
 * @param file Le chemin vers le fichier de données des utilisateurs.
 * @param name Le nom de l'utilisateur à modifier.
 * @param password utiliser seulement pour les tentatives de connexions.
 * @return user si succès, NULL en cas d'erreur
 */
Info_con* search_log(const char *file, const char *name, const char *password);

/**
 * @brief Change le mot de passe d'un utilisateur dans le fichier de données des utilisateurs.
 * 
 * @param file Le chemin vers le fichier de données des utilisateurs.
 * @param name Le nom de l'utilisateur dont le mot de passe doit être modifié.
 * @param pwd Le nouveau mot de passe à assigner à l'utilisateur.
 * @return 1 si la modification a été effectuée avec succès, -1 en cas d'erreur ou si l'utilisateur n'a pas été trouvé.
 */
int change_password_u(char *file, char *name, char *pwd);

/**
 * @brief sauvegarde les infos de connexion des utilisateurs dans le fichier de logging.
 * 
 * @param myfile Le chemin vers le fichier de données des utilisateurs.
 * @param name Le nom de l'utilisateur à enregistrer
 * @param pwd Le mot de passe à assigner à l'utilisateur.
 * @return true si  succès, false en cas d'erreur .
 */
bool save_user(const char *name, const char *password, const char *myfile);

/**
 * @brief verificateur de la force du mot de passe
 * @param password le mot de passe à verifier.
 * @return l'indicateur de force du mot de passe
 */
int checkPasswordStrength(const char *password);

/**
 * @brief affiche de l'aide 
 */
void help_();

#endif
