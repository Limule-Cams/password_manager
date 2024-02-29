#include "../include/crypt.h"

#define CHUNK_SIZE 4096

/*unsigned char key_generate(){
    unsigned char key[crypto_secretstream_xchacha20poly1305_KEYBYTES];
    crypto_secretstream_xchacha20poly1305_keygen(key);
    return key;
}
*/

static int encrypt(const char *target_file, const char *source_file,
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
    fwrite(header, 1, sizeof(header), fp_t);
    do {
        rlen = fread(buf_in, 1, sizeof(buf_in), fp_s);
        eof = feof(fp_s);
        tag = eof ? crypto_secretstream_xchacha20poly1305_TAG_FINAL : 0;
        crypto_secretstream_xchacha20poly1305_push(&st, buf_out, &out_len, buf_in, rlen,
                                                   NULL, 0, tag);
        fwrite(buf_out, 1, (size_t) out_len, fp_t);
    } while (! eof);
    fclose(fp_t);
    fclose(fp_s);
    return 0;
}

static int decrypt(const char *target_file, const char *source_file,
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
    fread(header, 1, sizeof(header), fp_s);
    if (crypto_secretstream_xchacha20poly1305_init_pull(&st, header, key) != 0) {
        /* incomplete header */
        fclose(fp_t);
        fclose(fp_s);
        return ret; 
    }
    do {
        rlen = fread(buf_in, 1, sizeof(buf_in), fp_s);
        eof = feof(fp_s);
        if (crypto_secretstream_xchacha20poly1305_pull(&st, buf_out, &out_len, &tag,
                                                       buf_in, rlen, NULL, 0) != 0) {
            fclose(fp_t);
            fclose(fp_s);
            return ret; 
        }
        if (tag == crypto_secretstream_xchacha20poly1305_TAG_FINAL) {
            if (! eof) {
                 /* end of stream reached before the end of the file */
                 fclose(fp_t);
                 fclose(fp_s);
                 return ret;
            }
        } else { /* not the final chunk yet */
            if (eof) {
                /* end of file reached before the end of the stream */
                fclose(fp_t);
                fclose(fp_s);
                return ret;
            }
        }
        fwrite(buf_out, 1, (size_t) out_len, fp_t);
    } while (! eof);

    return 0;
}

int check(void)
{
    unsigned char key[crypto_secretstream_xchacha20poly1305_KEYBYTES];

    if (sodium_init() != 0) {
        return 1;
    }
    crypto_secretstream_xchacha20poly1305_keygen(key);
    if (encrypt("/tmp/encrypted", "/tmp/original", key) != 0) {
        return 1;
    }
    if (decrypt("/tmp/decrypted", "/tmp/encrypted", key) != 0) {
        return 1;
    }
    return 0;
}

unsigned char hash_pass(unsigned char *pass, size_t len_pass){
    unsigned char hash[crypto_generichash_BYTES];

crypto_generichash(hash, sizeof hash,
                   pass, len_pass,
                   NULL, 0);

return hash;
}