#include "user.h"

int search_(const char *file, const char *name, const char *password) {
    // fonction servant pour la recherche et la connexion suivant la valeur de password
    Info_con user;
    FILE *fic = fopen(file, "rb");
    if(fic == NULL) {
        perror("Erreur d'ouverture du fichier");
        return -1;
    }

    while(fread(&user, sizeof(Info_con), 1, fic) == 1) {
        if(strcmp(user.name, name) == 0) {
            // si aucun mot de passe n'est fourni, retourner directement l'utilisateur trouvé
            if (password == NULL || strcmp(user.password, password) == 0) {
                fclose(fic);
                return 1; // utilisateur et ou password trouvé 
            }
        }
    }

    fclose(fic);
    return -1; // Utilisateur non trouvé
}



void save_user(const char *name, const char *password, const char *myfile) {

    FILE *fic = NULL;
    Info_con user1;
    Info_con existing_user;

    if(search_(myfile, name, NULL)==1){
        fprintf(stdout, "erreur nom d'utilisateur existant\n ");
        exit(EXIT_FAILURE);
    }

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
    user1.password[sizeof(user1.password) - 1] = '\0'; 

    // Copie des premiers 4 caractères de name dans user1->file
    strncpy(user1.file, name, 4);
    user1.file[4] = '\0'; 

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



int change_password(char *file, char *name, char *password, Info_con new_info){
    
    FILE *fic = NULL;


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

int checkPasswordStrength(const char *password){
    int min_len = 8;
    bool is_uper;
    bool is_lower;
    bool is_digit;
    bool is_special_car;
    
    int len_pass = strlen(password), indicate=0;

    for(int i=0; i<len_pass; i++){

        char c = password[i];
        if(isdigit(c))
            is_digit = true;
        else if(isupper(c))
            is_uper = true;
        else if(islower(c))
            is_lower = true;
        else if(ispunct(c))
            is_special_car = true;
        else{
            char no_valide[] = "éèeêàâûîôùïö";
            if(strchr(no_valide, c))
                return -1;
        }
    }

    if(is_digit)
        indicate +=1;
    if(is_lower)
        indicate+=1;
    if(is_special_car)
        indicate+=2;
    if(is_uper)
        indicate+=1;
    if(len_pass>=min_len)
        indicate+=2;
        
    return indicate;
}
