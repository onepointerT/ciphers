
#include "ciphers.h"

hash_t* onepointer_ciphers_ciphers_sha512( const char* msg ) {
    return onepointer_ciphers_ciphers_sha( msg, 512, 128 );
}
