#ifndef USER_H
#define USER_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<unistd.h>

typedef struct{
    char name[20];
    char password[30];
    char file[10];
}Info_con;

void save_user(const char *name, const char *password, const  char *myfile);
int logger(const char *name, const char *password, const char *myfile);
int search_(char *file, char *name);
int change_password(char *file, char *name, int id, Info_con new_info);

#endif
