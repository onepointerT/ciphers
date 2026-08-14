
#include "logic.h"

#include <stdint.h>
#include <stdlib.h>

#include "_blockbuf.h"

char* __leftrotate_short( const char* word, const unsigned short n_positions ) {
    
    size_t wordlength = sizeof(word) / sizeof(const char);
    
    size_t bits = wordlength / n_positions;
    size_t bitblocks = wordlength / bits;

    _blockbuf_t* blokbuf = _onepointer_blockbuf_init( bitblocks, bitblocks / bits );
    unsigned int bufi = 0;
    unsigned int bufpos = 0;
    for ( unsigned int wordi = 0; wordi < wordlength && bufi < bits; wordi++ && bufi++ ) {
        if ( bufpos == blokbuf->buf->bufsize - 1 ) { _onepointer_blockbuf_switchbuf( blokbuf, bufi ); bufpos = 0; }
        else bufpos++;

        _onepointer_blockbuf_strcpy_n( blokbuf, word, wordi, bufpos, 1 );
    }

    _onepointer_blockbuf_switchbuf( blokbuf, 0 );
    unsigned int wpos = ( bitblocks * bits ) - 1;
    char* new_word = (char*) malloc(sizeof(char)*bits);
    for ( bufi = 0, bufpos = 0, wpos = wordlength - 1 - blokbuf->buf->bufsize
        ; bufi < blokbuf->buffercount && bufpos < blokbuf->buf->bufsize && wpos < wordlength
        ; wpos++
    ) {        
        new_word[wpos] = blokbuf->buf->buf[bufpos];
        if ( bufpos == blokbuf->buf->bufsize - 1 ) {
            bufpos = 0;
            _onepointer_blockbuf_switchbuf( blokbuf, bufi++ );
        } else bufpos++;

        if ( wpos == wordlength - 1 ) wpos = 0;
    }

    return new_word;
}