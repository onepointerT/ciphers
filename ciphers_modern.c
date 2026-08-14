// Copyright (C) 2026 The OnePointer Authors.
//

#include "ciphers_modern.h"

#include "__aes_utils.h"
#include "__rsa_utils.h"


hash_t* onepointer_ciphers_modern_twoone( const char* msg, const char* pin, const enum AES_CHIFFRE aes_x ) {
    rsa_t* rsa = make_rsa( msg );

    
}



hash_t* onepointer_ciphers_modern_twopad_pinversion( const char* owner_id, const char* pin, const enum AES_CHIFFRE aes_x ) {

}

hash_t* onepointer_ciphers_modern_twopad_password( const char* owner_id, const char* password, const enum AES_CHIFFRE aes_x ) {

}

bool onepointer_ciphers_modern_twopad_fits( const char* owner_id, const char* pin_input, const enum AES_CHIFFRE aes_x ) {

}
