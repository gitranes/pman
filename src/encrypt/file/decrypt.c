#include "encrypt/file/decrypt.h"
#include "encrypt/raw/cipher.h"

enum Sizes
{
    FENC_BUFFER_SIZE = 2048
};

int fenc_decrypt_to_file(
    struct CryptMeta* meta, FILE* in_fp, FILE* out_fp)
{
    int exit_code = 0;
    unsigned char file_buffer[FENC_BUFFER_SIZE] = "";
    struct Cipher* const cipher = enc_cipher_init();

    if (enc_cipher_prepare(
            cipher, meta, FENC_BUFFER_SIZE, ENC_CIPHER_DECRYPT))
    {
        exit_code = -1;
        goto cleanup;
    }

    while (!feof(in_fp))
    {
        const size_t read_bytes =
            fread(file_buffer, 1, FENC_BUFFER_SIZE, in_fp);
        if (enc_cipher_run(cipher, file_buffer, read_bytes))
        {
            exit_code = -2;
            goto cleanup;
        }
        fwrite(cipher->block.buf, 1, cipher->block.size, out_fp);
    }

cleanup:
    enc_cipher_clean(cipher);
    return exit_code;
}
