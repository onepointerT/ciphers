
#include "ciphers.h"
#include "__rsa_utils.h"

rsa_t* onepointer_ciphers_ciphers_rsa( const char* key ) {
    return make_rsa( key );
}