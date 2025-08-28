
#include "ciphers.h"

hash_t* oneptr_ciphers_ciphers_sha1( const char* msg ) {
    return oneptr_ciphers_ciphers_sha( msg, 180, 1 );
}
