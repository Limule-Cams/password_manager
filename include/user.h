#ifndef USER_H
#define USER_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<unistd.h>
#include<ctype.h>
#define SIZE_NAME 35
#define P_LEN 30

typedef struct{
    char name[SIZE_NAME];
    char password[P_LEN];   
    char file[10];
}Info_con;

typedef enum{
    false,
    true
}bool;

Info_con* search_log(const char *file, const char *name, const char *password);
bool save_user(const char *name, const char *password, const  char *myfile);
int change_password_u(char *file, char *name, char *pwd);
bool save_user(const char *name, const char *password, const char *myfile);
int checkPasswordStrength(const char *password);
void help_();

#endif
