#ifndef CRYPT_H
#define CRYPT_H

#include <sodium.h>
#include<string.h>

// prototype des fonctions
//unsigned char key_generate();
unsigned char hash_pass(unsigned char MESSAGE);
static int encrypt(const char *target_file, const char *source_file,
        const unsigned char key[crypto_secretstream_xchacha20poly1305_KEYBYTES]);
static int decrypt(const char *target_file, const char *source_file,
        const unsigned char key[crypto_secretstream_xchacha20poly1305_KEYBYTES]);

char* key_genere(char *name, char *pass);
char *salt_generate(char *name, int len_name);
#endif

