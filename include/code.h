#ifndef CODE_H
#define CODE_H



#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<unistd.h>

#define PASSWORD_LENGTH 64
#define MAX_USERNAME_LENGTH 40
#define MAX_DESCRIPTION_LENGTH 50

#define PASS 25
typedef struct{
    int id;
    char description[MAX_DESCRIPTION_LENGTH];
    char nom[MAX_USERNAME_LENGTH];
    char passwd[PASSWORD_LENGTH]; 
 }Info;


 /**
 * @brief generateur de nombre aleatoire
 */

int random_generate();
/**
 * @brief generateur id
 */

int id_generator(const char *file);
/**
 * @brief initialisation de la structure Info
 */
Info* initialise(char *username, char *description, char *pwd, const char *file);
/**
 * générateur de password 
 */

char* password_generate(); //0
/**
 * @brief enregistrement du type info crée dans initialise() 
 */

int add_pass(const char *file, Info *data_write); //1
/**
 * @brief copie du fichier password.bin dans un nouveau fichier
 * (sauf l'element ou l'id correspond)
 */

int remove_pass(const char *file, int id); //2
/**
 * @brief changement pour mot de passe générer aleatoirement par un personel
 */

int change_password(char *file, int id, char *pwd); //3
/**
 * @brief affiche le contenue du fichier
 */

void read_all_file(const char *file); //4
/**
 * @brief recherche par le nom
 */

int search_(char *file, char *name);//5
/**
 * @brief exportation de données dans un fichier csv de format (id description username password)
 */

int export_file(const char *myfile, const char *file);//6
/**
 * @brief importation du contenue d'un fichier csv dans notre fichier de sauvegarde personnelle
 */


void import_file(const char *file_path, const char *master_password);//7
/**
 * @brief aide
 */

void help_I();
void quitter();

#endif

