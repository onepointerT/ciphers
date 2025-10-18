
#include "logic.h"

#include <stdlib.h>
#include <libm.h>

char* __rightrotate( const char* word, const unsigned short bitlength, const unsigned short wordparts ) {

    unsigned int wordlength = sizeof(word)/sizeof(const char);

    unsigned short wp_size_addition = 0;
    if ( wordlength % bitlength != 0 ) ++wp_size_addition;

    const unsigned short wp_size = (wordlength/wordparts) + wp_size_addition;
    const unsigned short bit_size = bitlength;

    char** wp = (char**) malloc(sizeof(char*)*wp_size);
    unsigned short wi = 0;
    for ( unsigned short iwp = wp_size - 1; iwp >= 0 && wi < wordlength; iwp-- ) {
        wp[iwp] = (char*) malloc(sizeof(char)*bit_size);
        
        char* wpw = wp[iwp];
        for ( unsigned short wwi = 0; wi < wordlength && wwi < bit_size; wi++, wwi++ ) {
            wpw[wwi] = word[wi];
        }
    }

    char* new_word = (char*) malloc(sizeof(char)*wordlength);
    unsigned short iwp = wp_size - 1;
    unsigned short iw = 0;
    for ( unsigned short inw = 0; inw < wordlength && iwp >= 0; inw++, ++iw ) {
        if ( iw >= bitlength ) { iw = 0; --iwp; }

        if ( inw >= 0 && inw < bitlength ) {
            new_word[inw] = wp[wp_size-1][iw];
        } else {
            new_word[inw] = wp[iwp][iw];
        }
    }

    return new_word;
}