
#include "ciphers.h"

hash_t* oneptr_ciphers_ciphers_sha512( const char* msg ) {
    return oneptr_ciphers_ciphers_sha( msg, 512, 128 );
}
