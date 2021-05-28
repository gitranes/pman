#include "encrypt/algorithm.h"

const EVP_CIPHER* enc_algo_cipher_type(enum EncryptAlgo algo)
{
    switch (algo)
    {
    case ENC_ALGO_NONE:
        return EVP_enc_null();
    case ENC_ALGO_AES_256:
        return EVP_aes_256_cbc();
    default:
        return NULL;
    }
}
