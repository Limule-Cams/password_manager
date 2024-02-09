#include "user.h"

// fonction de verification de connexion 

   int logger(const char *name, const char *password, const char *myfile) {
    FILE *fic = NULL;
    Info_con user;

    // Ouvrir le fichier en mode lecture
    fic = fopen(myfile, "rb");
    if(fic == NULL) {
        perror("Erreur d'ouverture du fichier");
        return -1;
    }

    // Parcourir le fichier pour trouver l'utilisateur
    while(fread(&user, sizeof(Info_con), 1, fic) == 1) {
        // Vérifier si les informations de connexion correspondent
        if(strcmp(user.name, name) == 0 && strcmp(user.password, password) == 0) {
            fclose(fic);
            return 0; // Connexion réussie
        }
    }
    
    // Fermer le fichier
    fclose(fic);

    // Si les informations de connexion ne correspondent à aucun utilisateur, retourner 1
    return 1;
}


void save_user(const char *name, const char *password, const char *myfile) {

    FILE *fic = NULL;
    Info_con user1;
    Info_con existing_user;

    // Ouvrir le fichier en mode lecture pour vérifier les doublons
    fic = fopen(myfile, "rb");
    if (fic == NULL) {
        perror("erreur lors de l'ouverture du fichier");
        exit(EXIT_FAILURE);
    }

    // Vérifier s'il existe déjà un utilisateur avec le même nom
    while (fread(&existing_user, sizeof(Info_con), 1, fic) == 1) {
        if (strcmp(existing_user.name, name) == 0) {
            fclose(fic);
            fprintf(stderr, "L'utilisateur %s existe déjà.\n", name);
            exit(EXIT_FAILURE);
        }
    }

    // Fermer le fichier pour le rouvrir en mode ajout
    fclose(fic);

    // Ouvrir le fichier en mode ajout
    fic = fopen(myfile, "ab");
    if (fic == NULL) {
        perror("erreur lors de l'ouverture du fichier");
        exit(EXIT_FAILURE);
    }

    // Vérification de la longueur du nom d'utilisateur
    if (strlen(name) < 4) {
        perror("nom d'utilisateur < 4");
        fclose(fic);
        exit(EXIT_FAILURE);
    }

    // Copie du nom d'utilisateur et du mot de passe
    strncpy(user1.name, name, sizeof(user1.name) - 1);
    user1.name[sizeof(user1.name) - 1] = '\0'; // Assurer une terminaison nulle

    /*
     * ici vous pouvez implémenter votre mécanisme de hachage
     * ou directement appelé votre fonction pour hacher le mot de passe avant de le sauvegarder
    */
    strncpy(user1.password, password, sizeof(user1.password) - 1);
    user1.password[sizeof(user1.password) - 1] = '\0'; // Assurer une terminaison nulle

    // Copie des premiers 4 caractères de name dans user1->file
    strncpy(user1.file, name, 4);
    user1.file[4] = '\0'; // Assurer une terminaison nulle

    // Concaténation de ".pass" à user1->file
    strcat(user1.file, ".pass");

    // Écriture de user1 dans le fichier
    if (fwrite(&user1, sizeof(Info_con), 1, fic) != 1) {
        perror("erreur lors de l'écriture dans le fichier");
        fclose(fic);
        exit(EXIT_FAILURE);
    }

    // Ajout d'un saut de ligne dans le fichier
    fputc('\n', fic);

    // Fermeture du fichier
    fclose(fic);
}

//____________________________________________________________________________________________________________

int search_(char *file, char *name){
    Info_con user;
    FILE *fic = NULL;
    if((fic = fopen(file, "rb")==NULL)){
            perror("erreur d'ouverture");
            return -1;
    }
    while(fread(&user, sizeof(Info_con), 1, file)==1){
        if(strcmp(user.name, name)){
            fclose(fic);
            return 1;
        }
    }
    fclose(fic);
    return 0;
}


int change_password(char *file, char *name, int id, Info_con new_info){
    
    FILE *fic = NULL;
    int size_data = search_(file, name);

    if((file = fopen(file, "rb+")==NULL)){
            perror("erreur d'ouverture");
            return -1;
    }
    Info_con user;
    long position;
    while(fread(&user, sizeof(Info_con), 1, file)==1){
        if(strcmp(user.name, name)){
            position = ftell(fic) - sizeof(Info_con);
            fseek(fic, position, SEEK_SET);
            fwrite(&new_info, sizeof(Info_con), 1, fic);
            fclose(fic);
            return 0; 
        }
    }

    return 0;
}
