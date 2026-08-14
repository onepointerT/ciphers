
#ifndef __CIPHERS_MODERN_H_
#define __CIPHERS_MODERN_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

#include "_hash.h"
#include "ciphers.h"


hash_t* onepointer_ciphers_modern_twoone( const char* msg, const char* pin, const enum AES_CHIFFRE aes_x );


hash_t* onepointer_ciphers_modern_twopad_pinversion( const char* owner_id, const char* pin, const enum AES_CHIFFRE aes_x );
hash_t* onepointer_ciphers_modern_twopad_password( const char* owner_id, const char* password, const enum AES_CHIFFRE aes_x );
bool onepointer_ciphers_modern_twopad_fits( const char* owner_id, const char* pin_input, const enum AES_CHIFFRE aes_x );



#ifdef __cplusplus
}
#endif

#endif // __CIPHERS_MODERN_H_