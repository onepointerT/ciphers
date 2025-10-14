

#ifndef __RSA_UTILS_H_
#define __RSA_UTILS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "ciphers.h"


const unsigned int genprim( const unsigned int from, const unsigned int to );
const unsigned int genprim_default();

typedef struct RSA_Values {
    unsigned int p;
    unsigned int q;
    unsigned int n;
} rsa_round_t;


const double _totient_euler( const int n_divisor );
const double _totient_carmichael( const struct RSA_Values* rsa_math );


struct RSA_Values* _init_rsa_values( const unsigned int pint, const unsigned int qint );

rsa_t* __rsa_generic( struct RSA_Values* rsav );
rsa_t* __rsa( const unsigned int from_seed, const unsigned int to_seed );

rsa_t* make_rsa( const char* key );

#ifdef __cplusplus
}
#endif

#endif // __RSA_UTILS_H_