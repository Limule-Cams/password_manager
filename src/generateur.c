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
    char  *pwd = malloc(sizeof(char)*PASS);
    if( pwd!=NULL){
        srand(time(NULL));
        for(i=0; i<PASS; i++){
            c=random_generate();
            pwd[i] = asci[c];
        }
    }else{
        fprintf(stdout, "erreur lors de l'alocation");
        return NULL;
    }
    return pwd;
}


Info* initialise(char *username, char *description, char *pwd, const char *file){

    Info *element = malloc(sizeof(Info));
    
    if(element==NULL){
        fprintf(stdout, "erreur d'allocation fonction init");
        return NULL;
   }
    
    int a = id_generator(file);
    if(a==-1){
        free(element);
        return NULL;
    }
    element->id = a;
    strcpy(element->nom, username);
    strcpy(element->description, description);
    strcpy(element->passwd, pwd);
    return element;
}


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


int id_generator(const char *file) {
    FILE *fic = NULL;
    Info data_read;
    int max_id = 0;

    if (access(file, F_OK) == -1) {  
        return 1;
    }

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

    // si le fichier est vide, retourner l'ID 1
    if (max_id == 0) {
        fprintf(stdout, "erreur de la fonction id_generator");
    }

    // sinon, retourner l'id suivant
    return max_id + 1;
}


void read_all_file(const char *file){
    FILE *fic = NULL;
    Info tmp ;
   
    
    if((fic = fopen(file, "rb"))==NULL){
        perror("errreur d'ouverture du fichier");
        return;
    }
    fprintf(stdout, "  id \t  DESCRIPTION  \t\t  USERNAME  \t\t PASSWORD\n"
                    "----------------------------------------------------\n\n\n"
    );
    while(fread(&tmp, sizeof(Info), 1, fic)==1){
    
 		// afficher sur la sortie standard les champs du type 
        fprintf(stdout, "  | %d |\t  | %s | \t\t\t | %s | \t\t | %s |\n", tmp.id, tmp.nom, tmp.description, tmp.passwd);
        printf("---------------------------------------------------------------------------------------------------\n");
    }
    
    fclose(fic);
   
}


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
    Info user, user1;
    FILE *fic = NULL;
    fic = fopen(file, "rb");
    if((fic==NULL)){
            perror("erreur d'ouverture");
            return -1;
    }

    while(fread(&user, sizeof(Info), 1, fic)==1){
        
        if(strcmp(user.nom, name)==0){
            long pos = ftell(fic) - sizeof(Info);
            fseek(fic, pos, SEEK_SET);
            fread(&user1, sizeof(Info), 1, fic);
            fclose(fic);
            fprintf(stdout,"[ %s ]     [ %s ]     [ %s ]\n", user1.nom, user1.description, user1.passwd);
            return 1;
        }
    }
    fclose(fic);
    return -1;
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
            Info *new_info = initialise(nom, description, pwd, file);

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
    fputs("id  description  nom  password", fic_w);
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
    char ch [] = "id  description  nom  password";

    size_t taille_premiere_ligne = strlen(ch);
    fseek(fic_r, taille_premiere_ligne, SEEK_SET);
    //fseek(fic_r,1,SEEK_CUR);

    while (fscanf(fic_r, "%s %s %s", description, username, password)==3)
    {
        Info *user = initialise(username, description, password, file_path);
        if(user!=NULL){
            add_pass(master_password, user);
        }
    }

    fclose(fic_r);
    
}

void help_I(){
    printf(
        "\n\n\t-0 genere un mot de passe"
        "\n\t-1 enregistre les donnees saisies"
        "\n\t-2 supprime le password le username et la description"
        "\n\t-3 changement de password"
        "\n\t-4 affiche tout les donnees"
        "\n\t-5 recherche un password par son nom"
        "\n\t-6 export le fichier password au format csv"
        "\n\t-7 import un fichier cvs dans notre sauvegarde personnelle"
        "\n\n:->$  "
    );
}

void quitter(){
    exit(EXIT_SUCCESS);
}
