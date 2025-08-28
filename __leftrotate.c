
#include "logic.h"

#include <stdlib.h>
#include <libm.h>

char* __leftrotate( const char* word, const unsigned short bitlength, const unsigned short wordparts ) {

    unsigned int wordlength = sizeof(word)/sizeof(const char);
    if ( wordlength % bitlength != 0 ) return 0;

    char* wp[wordparts];
    unsigned short wi = 0;
    for ( unsigned short iwp = 0; iwp <= wordparts && wi < wordlength; iwp++ ) {
        wp[iwp] = (char*) malloc(sizeof(char)*bitlength);
        char* wpw = wp[iwp];
        for ( unsigned short wwi = 0; wi < wordlength && wwi < bitlength; wi++, wwi++ ) {
            wpw[wwi] = word[wi];
        }
    }

    char* new_word = (char*) malloc(sizeof(char)*wordlength);
    wi = 0;
    unsigned short wpi = 1;
    for ( unsigned short inw = 0; inw < wordlength && wpi < wordparts-1; inw++ ) {
        if ( inw >= 0 && inw <= bitlength ) {
            new_word[inw] = wp[wordparts-1][wi];
            ++wi;
        } else {
            new_word[inw] = wp[wpi][wi];
            ++wi;
        }
        if ( wi == wordlength ) { wi = 0; ++wpi; }
    }

    return new_word;
}