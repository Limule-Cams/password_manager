#ifndef CRYPT_H
#define CRYPT_H

#include <sodium.h>
#include<string.h>

// prototype des fonctions
//unsigned char key_generate();
unsigned char hash_pass(unsigned char MESSAGE);
int encrypt(const char *target_file, const char *source_file,
        const unsigned char key[crypto_secretstream_xchacha20poly1305_KEYBYTES]);
int decrypt(const char *target_file, const char *source_file,
        const unsigned char key[crypto_secretstream_xchacha20poly1305_KEYBYTES]);

unsigned char* key_genere(char *name, char *pass);
unsigned char* hash_passwd(unsigned char *key, unsigned char *password, size_t len_p);
#endif

