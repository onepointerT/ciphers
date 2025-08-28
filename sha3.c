
#include "ciphers.h"

hash_t* oneptr_ciphers_ciphers_sha3( const char* msg ) {
    return oneptr_ciphers_ciphers_sha( msg, 180, 3 );
}
