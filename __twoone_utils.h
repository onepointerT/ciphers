

#ifndef __TWOONE_UTILS_H_
#define __TWOONE_UTILS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "_hash.h"
#include "_pyramidbuffer.h"


hash_t* __twoone_generic( const char* passphrase, const char* key );
hash_t* __twoone_authentificate( const char* passphrase, const char* key, const _pyramidring_t* pyr );

hash_t* make_twoone( const char* passphrase, const char* key );

#ifdef __cplusplus
}
#endif

#endif // __TWOONE_UTILS_H_