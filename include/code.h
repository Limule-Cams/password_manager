#ifndef CODE_H
#define CODE_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<unistd.h>

#define PASSWORD_LENGTH 20
#define MAX_USERNAME_LENGTH 20
#define MAX_DESCRIPTION_LENGTH 30

typedef struct Info{
    int id;
    char description[MAX_DESCRIPTION_LENGTH];
    char nom[MAX_USERNAME_LENGTH];
    char passwd[PASSWORD_LENGTH]; 
 }Info;


 
int choice();
char* password_generate();
Info* initialise(char *username, char *description, char *pwd);
int add_pass(const char *file, Info *data_write);
int id_generator(const char *file);
void read_all_file(const char *file);
int remove_pass(const char *file, int id);
int change_password(char *file, char *name, int id, Info new_info);
int search_(char *file, char *name);
int export_file(const char *myfile, const char *file);
void import_file(const char *file_path, const char *master_password);

#endif

