
#include "ciphers.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "logic.h"
#include "_blockbuf.h"


hash_t* onepointer_ciphers_ciphers_sha( const char* msg, const unsigned short bitlength, const unsigned int rounds ) {

    static int H0 = 0x67452301;
    static int H1 = 0xEFCDBA89;
    static int H2 = 0x98BADCFE;
    static int H3 = 0x10325476;
    static int H4 = 0xC3D2E1F0;

    size_t msg_length = sizeof(msg)/sizeof(const char);

    const char* msg_length_bit = (const char*) sizeof((int) msg);
    const size_t msg_length_bit_length = sizeof(msg_length_bit)/sizeof(const char*);
    size_t tmp_msg_length = (448 % bitlength) + 1;
    char* tmp_msg = (char*) malloc(sizeof(char)*tmp_msg_length);
    strcpy( tmp_msg, msg );
    
    tmp_msg[tmp_msg_length-1] = '1';
    size_t pos = 0;
    if ( msg_length < tmp_msg_length ) {
        for ( pos = msg_length + 1; pos < 448 % bitlength; pos++ ) {
            tmp_msg[pos] = '0';
        }
    }
    for ( unsigned long msgpos = 0; msgpos < msg_length_bit_length && pos < tmp_msg_length; pos++, ++msgpos ) {
        tmp_msg[pos] = msg_length_bit[msgpos];
    }

    _blockbuf_t* blkbuf_msg512 = _onepointer_blockbuf_init( msg_length / 512, 512 );
    unsigned long startpos_msg = 0;
    while ( blkbuf_msg512->bufferindex < blkbuf_msg512->buffercount ) {
        _onepointer_blockbuf_strcpy_n( blkbuf_msg512, msg, startpos_msg, 0, 512 );
        _onepointer_blockbuf_switchbuf( blkbuf_msg512, blkbuf_msg512->bufferindex + 1 );
        startpos_msg += 512;
    }


    _blockbuf_t* blkbuf_word = _onepointer_blockbuf_init( 80, 32 );

    hash_t* sha = _onepointer_hash_init( bitlength );

    for ( unsigned long i = 0; i < blkbuf_msg512->buffercount; i++ ) {

        // Compress message
        char** chunks = (char**) malloc(sizeof(char*)*16);
        unsigned int isha = 0;
        for ( unsigned int ic = 0; ic < 16 && isha < bitlength; ic++ ) { 
            unsigned int bufpos = 0;
            unsigned short wbuf = 0;
            
            chunks[ic] = (char*) malloc(sizeof(char)*32);
            char* word = chunks[ic];
            
            // Copy chunk of word
            for ( unsigned int wpos = 0; wpos < 32 && bufpos < 512; wpos++, ++bufpos ) {
                word[wpos] = blkbuf_msg512->buffers[i]->buf[bufpos];
            }
            _onepointer_blockbuf_switchbuf( blkbuf_word, wbuf );
            _onepointer_blockbuf_strcpy( blkbuf_word, word, 0, 0 );
            ++wbuf;

            for ( unsigned int iw = 16; iw < 80 && wbuf < blkbuf_word->buffercount; iw++, ++wbuf ) {
                _onepointer_blockbuf_switchbuf( blkbuf_word, wbuf );
                char* tmpval = __xor_str(_word(blkbuf_word, iw-1), __xor_str(_word(blkbuf_word, iw-8), __xor_str(_word(blkbuf_word, iw-14), _word(blkbuf_word, iw-16))));
                blkbuf_word->buf->buf = __leftrotate(tmpval, 32, 32 / iw);
            }
        }



        int a = H0;
        int b = H1;
        int c = H2;
        int d = H3;
        int e = H4;
        int f = 0;
        int k = 0;

        for ( unsigned int i = 0; i < 80; i++ ) {
            if ( i >= 0 && i <= 19 ) {
                f = (b && c) || ((! b) && d);
                k = 0x5A827999;
            } else if ( i >= 20 && i <= 39 ) {
                f = b | c | d;
                k = 0x6ED9EBA1;
            } else if ( i >= 40 && i <= 59 ) {
                f = (b && c) || (b && d) || (c && d);
                k = 0x8F1BBCDC;
            } else if ( i >= 60 && i <= 79 ) {
                f = b | c | d;
                k = 0xCA62C1D6;
            }

            int tmp = ((int) __leftrotate( (char*) a, 5, 5 / sizeof((char*)a)/sizeof(char) ))
                        + f + e + k + (int) _word(blkbuf_word, i);
            e = d;
            d = c;
            c = (int) __leftrotate( (char*) b , 30, (sizeof((char*) b) / sizeof(char) / 30));
            b = a;
            a = tmp;

        }

        H0 = H0 + a;
        H1 = H1 + b;
        H2 = H2 + c;
        H3 = H3 + d;
        H4 = H4 + e;

        sha->hash = (char*) H0 + H1 + H2 + H3 + H4;
    }

    return sha;
}