#include "../include/user.h"

Info_con* search_log(const char *file, const char *name, const char *password){
    
    Info_con *user;
    if((user = malloc(sizeof(Info_con)))==NULL){
        perror("erreu d'alocation");
        return NULL;
    }
    FILE *fic = fopen(file, "rb");
    if(fic == NULL) {
        free(user);
        perror("erreur fichier");
        return NULL;
    }

    while(fread(user, sizeof(Info_con), 1, fic) == 1) {
        if(strcmp(user->name, name) == 0) {
            // si aucun mot de passe n'est fourni, retourner directement l'utilisateur trouvé
            if (password == NULL || strcmp(user->password, password) == 0) {
                fclose(fic);
                return user; 
            }
        }
    }

    fclose(fic);
    free(user);
    return NULL; 
}



bool save_user(const char *name, const char *password, const char *myfile) {

    FILE *fic = NULL;
    Info_con user1;


    fic = fopen(myfile, "ab");
    if (fic == NULL) {
        exit(EXIT_FAILURE);
    }

    fseek(fic, 0, SEEK_END);
    long pos = ftell(fic);
    if(pos==0){
        ;//ne fait rien sinon on verifie les doublons
    }else{

        if((search_log(myfile, name, NULL))!=NULL){
            fprintf(stdout, "\nnom d'utilisateur nom valide\n");
            exit(EXIT_FAILURE);
        }
    }

    
    // Vérification de la longueur du nom d'utilisateur
    if (strlen(name) < 4) {
        fprintf(stdout,"nom d'utilisateur < 4\n");
        fclose(fic);
        exit(EXIT_FAILURE);
    }

    /*
     * ici vous pouvez implémenter votre mécanisme de hachage
     * ou directement appelé votre fonction pour hacher le mot de passe avant de le sauvegarder
    */

    // copie du nom d'utilisateur et du mot de passe
    // et  ajouter le caractere de fin de chaine
    strncpy(user1.name, name, sizeof(user1.name) - 1);
    user1.name[sizeof(user1.name) - 1] = '\0'; 
    strncpy(user1.password, password, sizeof(user1.password) - 1);
    user1.password[sizeof(user1.password) - 1] = '\0'; 
    strncpy(user1.file, name, 4);
    user1.file[4] = '\0'; 

    // Concaténation de ".pass" à user1->file 
    strcat(user1.file, ".pass");

    if (fwrite(&user1, sizeof(Info_con), 1, fic) != 1) {
        perror("erreur lors de l'écriture dans le fichier");
        fclose(fic);
        return false;
    }

    //fputc('\n', fic);
    fclose(fic);
    return true;
}



int change_password_u(char *file, char *name, char *pwd) {

    FILE *fic = fopen(file, "rb+");
    if (fic == NULL) {
        return -1;
    }

    Info_con user;

     while (fread(&user, sizeof(Info_con), 1, fic) == 1) {
        if (strcmp(user.name, name) == 0) {
            strcpy(user.password, pwd);

            // aller à la position dans le fichier et écrire le nouvel élément en remplaçant l'ancien
            long position = ftell(fic) - sizeof(Info_con);
            fseek(fic, position, SEEK_SET);
            fwrite(&user, sizeof(Info_con), 1, fic);
            fclose(fic);
            return 1;
        }
    }
    
    fclose(fic);
    return -1;
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
            char no_valide[] = "éèêàâûîôùïö";
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

void help_(){
    printf(
        "\n\n__________________________________________________________________"
        "\n\t!-h pour l'aide { ./main -h }                                 |"
        "\n\t!-s enregistre un utilisateur { ./main -s username password } |"
        "\n\t!-s changer de password { ./main -c username password         |"
        "\n\t!-l se connecter { ./main -s username password }              |"
        "\n--------------------------------------------------------------------\n\n"

    );
}

