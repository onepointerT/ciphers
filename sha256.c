
#include "ciphers.h"

hash_t* onepointer_ciphers_ciphers_sha256( const char* msg ) {
    return onepointer_ciphers_ciphers_sha( msg, 256, 64 );
}
