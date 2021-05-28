#pragma once

#include <openssl/evp.h>

enum EncryptAlgo
{
    ENC_ALGO_NONE = 0,
    ENC_ALGO_AES_256 = 1
};

const EVP_CIPHER* enc_algo_cipher_type(enum EncryptAlgo algo);
