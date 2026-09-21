

#ifndef __TWOONE_UTILS_H_
#define __TWOONE_UTILS_H_

#ifdef __cplusplus
extern "C" {
#endif



#include "_hash.h"


hash_t* __twopad_generic( const char* passphrase, const char* key_pin );
bool __twopad_legitimate_pin( const hash_t* twopad_stored, const char* key_pin );

hash_t* make_twopad( const char* passphrase, const char* key_pin );

#ifdef __cplusplus
}
#endif

#endif // __TWOONE_UTILS_H_