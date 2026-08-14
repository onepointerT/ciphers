
#include "ciphers.h"
#include "_blockbuf.h"

#include <stdlib.h>


hash_t* _onepointer_hash_init( const unsigned int lenght_of_hash_bits ) {
    hash_t* hash = (hash_t*) malloc(sizeof(hash_t));

    hash->hash = (char*) malloc(sizeof(char)*lenght_of_hash_bits);
    hash->parts = 0;

    return hash;
}


hash_t* _onepointer_hash_init_parts( const unsigned int lenght_of_hash_bits, void* parts_struct ) {
    hash_t* hash = _onepointer_hash_init( lenght_of_hash_bits );

    hash->parts = parts_struct;

    return hash;
}


_charbuf_t* _onpointer_charbuf_init( const unsigned short bufsize ) {

    struct _onepointer_charbuf* cbuf = (struct _onepointer_charbuf*) malloc(sizeof(struct _onepointer_charbuf));

    cbuf->buf = (char*) malloc(sizeof(char)*bufsize);
    cbuf->bufsize = bufsize;

    return cbuf;
}


_blockbuf_t* _onepointer_blockbuf_init( const unsigned int buffercount, const unsigned short buffersize_each ) {

    struct _onepointer_blockbuf* blkbuf = (struct _onepointer_blockbuf*) malloc(sizeof(struct _onepointer_blockbuf));

    blkbuf->buffercount = buffercount;
    blkbuf->bufferindex = 0;

    *blkbuf->buffers = (struct _onepointer_charbuf*) malloc(sizeof(struct _onepointer_charbuf)*buffercount);
    for ( unsigned int bci = 0; bci < buffercount; bci++ ) {
        blkbuf->buffers[bci] = _onpointer_charbuf_init( buffersize_each );
    }

    blkbuf->buf = blkbuf->buffers[0];

    return blkbuf;
}

aes_t* _onepointer_ciphers_init_aes( const unsigned short wordlength, const unsigned short word_columns ) {
    aes_t* aes = (aes_t*) malloc(sizeof(aes_t));

    aes->keystrength = wordlength * word_columns * word_columns;
    aes->msgstrength = wordlength * word_columns * word_columns;
    aes->key = "";
    aes->msg = "";
    aes->cipher = 0;

    return aes;
}