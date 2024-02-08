#include"code.h"


int random_generate(){
    // srand(time(NULL));
    return rand()%96 +1; 
}

/* 
 * cette fonction génère aléatoirement un mot de passe de longueur N
 *dans un tableau ici asci qui est généré dans la boucle for
 */
char* password_generate(){
    int i,c=0;
    char  asci[96];
    for(i=0; i<96; i++)
        asci[i]=i+32;
    char  *pwd = malloc(sizeof(char)*PASSWORD_LENGTH);
    if( pwd!=NULL){
        srand(time(NULL));
        for(i=0; i<PASSWORD_LENGTH ; i++){
            c=random_generate();
            pwd[i] = asci[c];
        }
    }else{
        fprintf(stdout, "erreur lors de l'alocation");
        return NULL;
    }
    return pwd;
}


// fonction pour initialiser la type Info

Info* initialise(char *username, char *description, char *pwd){
    Info *element = malloc(sizeof(Info));
    if(element==NULL){
        fprintf(stdout, "erreur d'allocation fonction init");
        exit(1);
   }
    strcpy(element->nom, username);
    strcpy(element->description, description);
    strcpy(element->passwd, pwd);
    return element;
}


// fonction pour enregistrer le type info dans un fichier

int add_pass(const char *file, Info *data_write){
    FILE *myfile = NULL;  
    myfile = fopen(file, "ab");
    if(myfile ==NULL){
        fprintf(stdout," erreur d'ouverture de fichier");
        return 1;
    }
    if(fwrite(data_write, sizeof(Info), 1, myfile)!=1){;

        perror("erreur d'ecriture de donnees");
        fclose(myfile);
        return 1;
    }
    // fermer le fichier 
    fclose(myfile);
    return 0;
}

// fonction pour générer automatiquement le champ id du type Info

int id_generator(const char *file) {
    FILE *fic = NULL;
    Info data_read;
    int max_id = 0;

    // Ouvrir le fichier en mode lecture
    fic = fopen(file, "rb");
    if (fic == NULL) {
        perror("Erreur d'ouverture du fichier");
        return -1;
    }

    // Parcourir le fichier pour trouver le plus grand ID
    while (fread(&data_read, sizeof(Info), 1, fic) == 1) {
        if (data_read.id > max_id) {
            max_id = data_read.id;
        }
    }

  
    fclose(fic);

    // Si le fichier est vide, retourner l'ID 1
    if (max_id == 0) {
        return 1;
    }

    // Sinon, retourner le plus grand ID trouvé plus un
    return max_id + 1;
}


// fonction de lecture du type Info
void read_all_file(const char *file){
    FILE *fic = NULL;
    Info tmp ;
   
    
    if((fic = fopen(file, "rb"))==NULL){
        perror("errreur d'ouverture du fichier");
        return;
    }
    
    while(fread(&tmp, sizeof(Info), 1, fic)==1){
    
 		// afficher sur la sortie standard les champs du type 
        fprintf(stdout, " \nDESCRIPTION :  %s ; \nUSERNAME :  %s  ;\nPASSWORD : %s", tmp.description, tmp.nom, tmp.passwd);
        printf("\n_____________________________________________________________________________________________________________________________________________________\n");

    }
    
    fclose(fic);
   
}

/*
la fonction remove_pass verifie la correspondance entre un id et le champ id
si ils sont identiques on copie le fichier actuel dans un autre fichier sans 
la ligne ou id = info-id
*/
int remove_pass(const char *file, int id) {
    FILE *fic = NULL, *new_fic = NULL;
    Info *element = malloc(sizeof(Info));
    char new_file[] = "tmp.bin";

    if (element == NULL) {
        perror("Erreur d'allocation de mémoire");
        return -1;
    }

    if ((fic = fopen(file, "rb")) == NULL) {
        perror("Erreur d'ouverture en lecture");
        free(element); 
        return -1;
    }

    if ((new_fic = fopen(new_file, "wb")) == NULL) {
        perror("Erreur d'ouverture en écriture");
        fclose(fic);   
        free(element); 
        return -1;
    }

    
    while (fread(element, sizeof(Info), 1, fic) == 1) {
        if (element->id != id) {
            // Écrire l'élément dans le nouveau fichier si l'ID ne correspond pas
            fwrite(element, sizeof(Info), 1, new_fic);
        }
        
    }

    // Fermer les fichiers
    fclose(fic);
    fclose(new_fic);
    free(element);
    
    // verifier la suppression
 	if(remove(file)!=0){
 		perror("Erreur de suppression");
 		return -1;
 	}

    // Renommer le nouveau fichier en remplaçant l'ancien
    if (rename(new_file, file) != 0) {
        perror("Erreur de renommage du fichier");
        return -1;
    }

    return 0;
}

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
