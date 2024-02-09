#ifndef CODE_H
#define CODE_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<unistd.h>

#define PASSWORD_LENGTH 20
typedef struct Info{
    int id;
    char description[30];
    char nom[20];
    char passwd[20]; 
 }Info;


 
int choice();
char* password_generate();
Info* initialise(char *username, char *description, char *pwd);
int add_pass(const char *file, Info *data_write);
int id_generator(const char *file);
void read_all_file(const char *file);
int remove_pass(const char *file, int id);
int change_password(char *file, char *name, int id, Info new_info);
long search_(char *file, char *name, int id);
int export_file(char *myfile, char *file);

#endif

