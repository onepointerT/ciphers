
#include "ciphers.h"

hash_t* onepointer_ciphers_ciphers_sha3( const char* msg ) {
    return onepointer_ciphers_ciphers_sha( msg, 180, 3 );
}
