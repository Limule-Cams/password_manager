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
            return 1;
        }
    }
    fclose(fic);
    return 0;
}


int change_password(char *file, char *name, int id, Info new_info){
    
    FILE *fic = NULL;
    int size_data = search_(file, name);

    if((fic = fopen(file, "rb+"))==NULL){
            perror("erreur d'ouverture");
            return -1;
    }
    Info user;
    long position;
    while(fread(&user, sizeof(Info), 1, fic)==1){
        if(strcmp(user.nom, name)==0 && user.id==id){
            position = ftell(fic) - sizeof(Info);
            fseek(fic, position, SEEK_SET);
            fwrite(&new_info, sizeof(Info), 1, fic);
            fclose(fic);
            return 0; 
        }
    }

    return 0;
}

int export_file(char *myfile, char *file){
    FILE *fic_r = NULL, *fic_w = NULL;
    Info user;

    fic_r = fopen(myfile,"rb");
    fic_w = fopen(file, "w") ;

    if(fic_r==NULL){
        perror("Fail open");
        return -1;
    }
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