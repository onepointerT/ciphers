
#ifndef __CIPHERS_H_
#define __CIPHERS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "_word.h"



typedef struct Hash {
    char* hash;
    void* parts;
} hash_t;

hash_t* oneptr_ciphers_ciphers_init_hash( const unsigned int lenght_of_hash_bits );
hash_t* oneptr_ciphers_ciphers_init_hash_parts( const unsigned int lenght_of_hash_bits, void* parts_struct );

typedef struct SHAsum {
    hash_t* hash;
    void* checksum_cipher;
} sha_t;

hash_t* oneptr_ciphers_ciphers_sha( const char* msg, const unsigned short bitlength, const unsigned int rounds );
hash_t* oneptr_ciphers_ciphers_sha1( const char* msg );
hash_t* oneptr_ciphers_ciphers_sha3( const char* msg );
hash_t* oneptr_ciphers_ciphers_sha256( const char* msg );
hash_t* oneptr_ciphers_ciphers_sha512( const char* msg );

enum PKI_CIPHERS {
    DIFFIE,
    AES,
    MSHAPv1,
    PGP
};

typedef struct PKI {
    const char* priv;
    const char* pub;
    const hash_t* checksum;
    const enum PKI_CIPHERS encrypt_cypher;
} pki_t;


enum TKIP_CIPHERS {
    SHA1,
    SHA3,
    SHA256, 
    SHA316, // (SHA512 - SHA256) *& SHA1 * (SHA3/SHA512)
    SHA418, // (SHA316 - SHA256) /| SHA1 *| (SHA1 & SHA3)
    SHA512
};


typedef struct TKIP {
    pki_t* pki;
    sha_t* sha;
    sha_t* reversive_sha;
    const enum TKIP_CIPHERS tkip_cipher;
} tkip_t;


enum AES_CHIFFRE {
    AES128,
    AES160,
    AES192,
    AES224,
    AES256
};

typedef struct AES {
    hash_t* msg;
    hash_t* key;
    unsigned short keystrength;
    unsigned short msgstrength;
    void* cipher;
} aes_t;

aes_t* _oneptr_ciphers_init_aes( const unsigned short wordlength, const unsigned short word_columns );
aes_t* oneptr_ciphers_ciphers_aes( const char* msg, const char* key );

typedef struct AES_TKIP {
    aes_t* aes;
    tkip_t* tkip;
    const char* checksum;
    const char* checksum_reverse;
} aes_tkip_t;

#ifdef __cplusplus
}
#endif

#endif // __CIPHERS_H_