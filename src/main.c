#include "../include/code.h"
#include "../include/user.h"


int main(int argc, char *argv[])
{
    char myfile[] = "password.bin";
    if(argc<2){
        help_();

    }else{
        
        if(strcmp(argv[1], "-h"))
            help_();

        else if (strcmp(argv[1], "-s"))
        {
            if(argc!=4)
                help_();
            else{
                if(save_user(argv[2], argv[3], myfile)){
                    printf("inscription valide \nconnectez vous\n");
                }
                printf("inscription echouer\n");
                printf("\t\t-s  name password\n");
            }
        }
        
        else if(strcmp(argv[1], "-l")){

            if(argc!=4)
                help_();
            else{
                if (access(myfile, F_OK) != -1) {

                    Info_con *info_user = search_log(myfile, argv[2], argv[3]);
                    if(info_user==NULL){
                        printf("oups information ou format incorecte\n");
                        printf("\t\t-l  name password\n");
                    }
                    printf("\nsucces authentification\n");
                    char *file = info_user->file;
                    int c = 0;
                    printf("\n\n\t\t\tHello, %s    (--) \n\n", info_user->name);
                    free(info_user);
                    printf("Que voulez-vous faire ? : ");
                    help_();
                    scanf("%d", &c);
                    switch (c)
                    {
                        case 0:
                            char *passe = password_generate();
                            printf("\t\t {password_generate}: %s", passe);
                            free(passe);
                            break;
                        case 1:
                            char nom[MAX_USERNAME_LENGTH], description[MAX_DESCRIPTION_LENGTH];

                            printf("\nEntrer une description pour le mot de passe :... ");
                            fgets(description, sizeof(description), stdin);
                            description[strcspn(description,"\n")]='\0';
                            
                            printf("\nLe nom d'utilisateur pour le mot de passe :... ");
                            fgets(nom, sizeof(nom), stdin);
                            nom[strcspn(nom,"\n")]='\0';
                            
                            char* pass  = password_generate();
                            Info *data = initialise(nom, description, pass);
                            if(data==NULL){
                                printf("erreur d'enregistrement des donnees");
                                exit(EXIT_FAILURE);
                            }
                            if(add_pass(file, data)==-1)
                                printf("erreur de sauvegarde");
                            printf("sauvegarde OK");

                            break;
                        case 2:
                            int num = 0;
                            printf("veiller saisir l'id du password à supprimer : ");
                            scanf("%d", &num);
                            if(!(isdigit(num))){
                                printf("entre incorrecte\n");
                                exit(EXIT_FAILURE);
                            }
                            if(remove_pass(file, num)==1)
                                printf("suppression reussie\n");
                            break;
                        case 3:
                            char pwd[N], descript[MAX_DESCRIPTION_LENGTH];
                            int id_ = 0;
                            printf("saisir l'id du password à changer : ");
                            scanf("%d", &id_);
                            getchar();
                            if(!(isdigit(id_))){
                                printf("entre incorrecte\n");
                                exit(EXIT_FAILURE);
                            }
                            printf("saisir le mot de passe :");
                            fgets(descript, sizeof(descript), stdin);
                            description[strcspn(descript,"\n")]='\0';
                            if(change_password(file, id_, pwd)!=1)
                                printf("\t\terreur\n");
                            printf("\t\t changement reussi");

                            break;
                        case 4:
                            read_all_file(file);
                            break;
                        case 5:
                            char name[MAX_USERNAME_LENGTH];
                            printf("\nEntrer le nom pour le nom :... ");
                            fgets(name, sizeof(nom), stdin);
                            description[strcspn(nom,"\n")]='\0';
                            search_(file, nom);
                            break;
                        case 6:
                            char path_[30];
                            printf("donner le nom du fichier à exporter");
                            fgets(path_, sizeof(path_), stdin);
                            path_[strcspn(path_,"\n")]='\0';
                            export_file(file,path_);
                            break;
                        case 7:
                            printf("donner le nom du fichier à importer");
                            fgets(path_, sizeof(path_), stdin);
                            path_[strcspn(path_,"\n")]='\0';
                            import_file(path_, file);
                            break;
                        
                        default:
                            printf("pffffff");
                            break;
                    }

                }
            }
        }
    }
}
  