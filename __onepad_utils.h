

#ifndef __TWOONE_UTILS_H_
#define __TWOONE_UTILS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "_hash.h"

// Onepad uses Twopad for password/PIN checking and encrypts/decrypts the message with TKIP.

hash_t* __onepad_generic( const char* msg, const char* key );
hash_t* __onepad_legitimate( const char* key );
const char* __onepad_get_msg_on_legitimation( const hash_t* hsh, const char* key );

hash_t* make_onepad( const char* msg, const char* key );

#ifdef __cplusplus
}
#endif

#endif // __TWOONE_UTILS_H_