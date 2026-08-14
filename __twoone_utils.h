

#ifndef __TWOONE_UTILS_H_
#define __TWOONE_UTILS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "_hash.h"


hash_t* __twoone_generic( const char* msg, const char* key );
hash_t* __twoone( const char* msg, const char* key );

hash_t* make_twoone( const char* key );

#ifdef __cplusplus
}
#endif

#endif // __TWOONE_UTILS_H_