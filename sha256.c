
#include "ciphers.h"

hash_t* oneptr_ciphers_ciphers_sha256( const char* msg ) {
    return oneptr_ciphers_ciphers_sha( msg, 256, 64 );
}
