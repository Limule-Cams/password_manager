#include "../include/crypt.h"

#define CHUNK_SIZE 4096

 int encrypt(const char *target_file, const char *source_file,
        const unsigned char key[crypto_secretstream_xchacha20poly1305_KEYBYTES])
{
    unsigned char  buf_in[CHUNK_SIZE];
    unsigned char  buf_out[CHUNK_SIZE + crypto_secretstream_xchacha20poly1305_ABYTES];
    unsigned char  header[crypto_secretstream_xchacha20poly1305_HEADERBYTES];
    crypto_secretstream_xchacha20poly1305_state st;
    FILE          *fp_t, *fp_s;
    unsigned long long out_len;
    size_t         rlen;
    int            eof;
    unsigned char  tag;

    fp_s = fopen(source_file, "rb");
    fp_t = fopen(target_file, "wb");
    crypto_secretstream_xchacha20poly1305_init_push(&st, header, key);
    fwrite(header, 1, sizeof header, fp_t);
    do {
        rlen = fread(buf_in, 1, sizeof buf_in, fp_s);
        eof = feof(fp_s);
        tag = eof ? crypto_secretstream_xchacha20poly1305_TAG_FINAL : 0;
        crypto_secretstream_xchacha20poly1305_push(&st, buf_out, &out_len, buf_in, rlen,
                                                   NULL, 0, tag);
        fwrite(buf_out, 1, (size_t) out_len, fp_t);
    } while (! eof);
    fclose(fp_t);
    fclose(fp_s);
    if(remove(source_file)==-1){
        return -1;
    }
    if (rename(target_file, source_file)==-1)
    {
        return -1;
    }
    
    return 0;
}

int decrypt(const char *target_file, const char *source_file,
        const unsigned char key[crypto_secretstream_xchacha20poly1305_KEYBYTES])
{
    unsigned char  buf_in[CHUNK_SIZE + crypto_secretstream_xchacha20poly1305_ABYTES];
    unsigned char  buf_out[CHUNK_SIZE];
    unsigned char  header[crypto_secretstream_xchacha20poly1305_HEADERBYTES];
    crypto_secretstream_xchacha20poly1305_state st;
    FILE          *fp_t, *fp_s;
    unsigned long long out_len;
    size_t         rlen;
    int            eof;
    int            ret = -1;
    unsigned char  tag;

    fp_s = fopen(source_file, "rb");
    fp_t = fopen(target_file, "wb");
    fread(header, 1, sizeof header, fp_s);
    if (crypto_secretstream_xchacha20poly1305_init_pull(&st, header, key) != 0) {
        goto ret; /* incomplete header */
    }
    do {
        rlen = fread(buf_in, 1, sizeof buf_in, fp_s);
        eof = feof(fp_s);
        if (crypto_secretstream_xchacha20poly1305_pull(&st, buf_out, &out_len, &tag,
                                                       buf_in, rlen, NULL, 0) != 0) {
            goto ret; /* corrupted chunk */
        }
        if (tag == crypto_secretstream_xchacha20poly1305_TAG_FINAL) {
            if (! eof) {
                goto ret; 
            }
        } else { /* not the final chunk yet */
            if (eof) {
                goto ret; 
            }
        }
        fwrite(buf_out, 1, (size_t) out_len, fp_t);
    } while (! eof);


    if(remove(source_file)==-1){
        return -1;
    }
    if (rename(target_file, source_file)==-1)
    {
        return -1;
    }
    ret = 0;
ret:
    fclose(fp_t);
    fclose(fp_s);
    return ret;
}


 unsigned char* key_genere(char *name, char *pass){
    int name_len = strlen(name);
    int message_len = name_len + 5;
    char message[message_len];
    char unsigned *key = malloc(sizeof(unsigned char)*crypto_generichash_BYTES);
    if(key==NULL){
        exit(EXIT_FAILURE);
    }
    strncpy(message, name, name_len);
    strncat(message, pass, 4);
    message[message_len] = '\0';
    unsigned char mess[message_len];
    memcpy(mess, message, message_len);
    crypto_generichash(key, crypto_generichash_BYTES,
                   mess, message_len,
                   NULL, 0);

    return key;
}


// sha-256 crypt
unsigned char* hash_passwd(unsigned char *key, unsigned char *password, int len_p){

    unsigned char *cypher_text = malloc(sizeof(unsigned char)*crypto_hash_sha256_BYTES);
    if(cypher_text==NULL){
        exit(EXIT_FAILURE);
    }

    crypto_hash_sha256_state state;

    crypto_hash_sha256_init(&state);

    crypto_hash_sha256_update(&state, key, crypto_generichash_BYTES);
    crypto_hash_sha256_update(&state, password, len_p);

    crypto_hash_sha256_final(&state, cypher_text);

    return cypher_text;

}
