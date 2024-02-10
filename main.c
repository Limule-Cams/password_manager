#include "code.h"


int main(){
    int ch;
    const char myfile[] = "password.bin", fils [] = "export.db";
    printf("\n Hello welcome to password manager (- -) !");
    printf("\n                                      Selectionner l'action a effectuer\n 1- Generer un mot de passe\n 2- Consulter vos mot de pass\n");
    scanf("%d",&ch);
    getchar();
    switch(ch){
        case 1:
            char nom[15], description[30], car;
            do{
                printf("\nEntrer une description pour le mot de passe :... ");
                fgets(description, sizeof(description), stdin);
                description[strcspn(description,"\n")]='\0';
                
                printf("\nLe nom d'utilisateur pour le mot de passe :... ");
                fgets(nom, sizeof(nom), stdin);
                nom[strcspn(nom,"\n")]='\0';
                
                printf("la description: %s et le nom: %s\n", description, nom);
                printf("voulez-vous continuer y/n : ");
                scanf("%c",&car);
                getchar();
            }while(car!='y');
            char* pass  = password_generate();
            printf("%s\n", pass);
            Info *data = initialise(nom, description, pass);
            printf("%s\n", data->passwd);
            int a = add_pass(myfile, data);
            if(a==0)
                printf("succes save");


            free(data);

            break;

        case 2:
            read_all_file(myfile);
            break;

        case 3:
            export_file(myfile, fils);
            break;
        
        case 4:
            import_file(fils, myfile);
            break;

        default:
            printf("choix invalide reessayer");
    }

  
    return 0;
}
