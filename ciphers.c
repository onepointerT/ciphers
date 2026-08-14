
#include "ciphers.h"

#include "__aes_utils.h"


aes_t* onepointer_ciphers_ciphers_aes( const char* msg, const char* key ) {
    return __aes( msg, key );
}
