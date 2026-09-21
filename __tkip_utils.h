

#ifndef __TWOONE_UTILS_H_
#define __TWOONE_UTILS_H_

#ifdef __cplusplus
extern "C" {
#endif



#include "_hash.h"

// TKIP uses twoone for storing and encrypting passwords, checks and optionally legitimates PINs
// with Twopad and follow ups encrypts message with the RSA-keys and the seed of the resulting ring
// of TwoOne. For legitimation before decryption Twopad is used and the decryption password then
// calculated with twoone.
// The tkip algorithm uses ringbuffers for encrypting with the public key and the result ring of TwoPad,
// Concluding that AES/SHA3 is to be used for deterministical encryption without message lost inside the
// cipher.

hash_t* __tkip_generic( const char* passphrase, const char* key_pin );
bool __tkip_legitimate_pin( const hash_t* twopad_stored, const char* key_pin );

hash_t* make_tkip( const char* passphrase, const char* key_pin );

#ifdef __cplusplus
}
#endif

#endif // __TWOONE_UTILS_H_