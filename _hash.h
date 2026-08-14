
#ifndef __HASH_H_
#define __HASH_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "_word.h"



typedef struct Hash {
    char* hash;
    void* parts;
} hash_t;

hash_t* _onepointer_hash_init( const unsigned int lenght_of_hash_bits );
hash_t* _onepointer_hash_init_parts( const unsigned int lenght_of_hash_bits, void* parts_struct );

#ifdef __cplusplus
}
#endif

#endif // __HASH_H_