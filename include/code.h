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
#define N 35

typedef struct Info{
    int id;
    char description[MAX_DESCRIPTION_LENGTH];
    char nom[MAX_USERNAME_LENGTH];
    char passwd[PASSWORD_LENGTH]; 
 }Info;


 
int random_generate();
int id_generator(const char *file);
Info* initialise(char *username, char *description, char *pwd);
char* password_generate(); //0
int add_pass(const char *file, Info *data_write); //1
int remove_pass(const char *file, int id); //2
int change_password(char *file, int id, char *pwd); //3
void read_all_file(const char *file); //4
int search_(char *file, char *name);//5
int export_file(const char *myfile, const char *file);//6
void import_file(const char *file_path, const char *master_password);//7

#endif

