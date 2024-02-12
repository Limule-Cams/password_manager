#include "../include/code.h"
   

int random_generate(){
    // srand(time(NULL));
    return rand()%96 +1;
}                                                       

char* password_generate(){
    int i,c=0;
    char  asci[96];
    for(i=0; i<96; i++)
        asci[i]=i+32;
    char  *pwd = malloc(sizeof(char)*N);
    if( pwd!=NULL){
        srand(time(NULL));
        for(i=0; i<N; i++){
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
        return NULL;
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
        free(data_write);
        return -1;
    }
    if(fwrite(data_write, sizeof(Info), 1, myfile)!=1){;

        perror("erreur d'ecriture de donnees");
        free(data_write);
        fclose(myfile);
        return -1;
    }
    // fermer le fichier 
    fclose(myfile);
    free(data_write);
    return 1;
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

    return 1;
}

int search_(char *file, char *name){
    Info user;
    FILE *fic = NULL;
    fic = fopen(file, "rb");
    if((fic==NULL)){
            perror("erreur d'ouverture");
            return -1;
    }

    while(fread(&user, sizeof(Info), 1, fic)==1){
        if(strcmp(user.nom, name)){
            fclose(fic);
            printf("nom d'utilisateur trouver");
            printf("%s  %s  %s", user.nom, user.description, user.passwd);
            return 1;
        }
    }
    fclose(fic);
    return 0;
}

int change_password(char *file, int id, char *pwd) {

    FILE *fic = fopen(file, "rb+");
    if (fic == NULL) {
        perror("Erreur d'ouverture");
        return -1;
    }

    Info user;
    while (fread(&user, sizeof(Info), 1, fic) == 1) {
        if (user.id == id) {
            char nom[MAX_USERNAME_LENGTH];
            char description[MAX_DESCRIPTION_LENGTH];
            strncpy(nom, user.nom, sizeof(user.nom));
            strncpy(description, user.description, sizeof(user.description));

            // initialiser un nouvel élément avec le nouveau mot de passe
            Info *new_info = initialise(nom, description, pwd);

            // aller à la position dans le fichier et écrire le nouvel élément en remplaçant l'ancien
            long position = ftell(fic) - sizeof(Info);
            fseek(fic, position, SEEK_SET);
            fwrite(new_info, sizeof(Info), 1, fic);

            fclose(fic);
            free(new_info);
            return 1;
        }
    }

    fclose(fic);
    return -1;
}



int export_file(const char *myfile, const char *file){
    FILE *fic_r = NULL, *fic_w = NULL;
    Info user;

    fic_r = fopen(myfile,"rb");

    if(fic_r==NULL){
        perror("Fail open");
        return -1;
    }

    fic_w = fopen(file, "w") ;

    if(fic_w==NULL){
        perror("Fail open");
        fclose(fic_r);
        return -1;
    }
    fputs("id  description  nom  password\n", fic_w);
    while(fread(&user, sizeof(Info), 1, fic_r)==1){
        fprintf(fic_w, "%s %s %s\n", user.description, user.nom, user.passwd);
    }
    fclose(fic_r);
    fclose(fic_w);
    return 0;
}

void import_file(const char *file_path, const char *master_password){
    FILE *fic_r = NULL;
    fic_r = fopen(file_path, "r");
    if (fic_r == NULL) {
        perror("fail open file");
        return;
    }

    char username[MAX_USERNAME_LENGTH];
    char description[MAX_DESCRIPTION_LENGTH];
    char password[PASSWORD_LENGTH];

    size_t taille_premiere_ligne = strlen("id description nom password\n");
    fseek(fic_r, taille_premiere_ligne, SEEK_SET);

    while (fscanf(fic_r, "%s %s %s", username, description, password)==3)
    {
        Info *user = initialise(username, description, password);
        if(user!=NULL){
            add_pass(master_password, user);
        }
    }

    fclose(fic_r);
    
}
