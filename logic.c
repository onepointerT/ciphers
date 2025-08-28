
#include "logic.h"

#include <stdlib.h>
#include <string.h>


char* leftrotate( const char* word, const unsigned short wordbit, const unsigned short bitlength, const unsigned int bits_selected ) {
    
    unsigned int wordlength = sizeof(word)/sizeof(const char);

    char* shortword = (char*) malloc(sizeof(char)*bitlength*bits_selected);
    unsigned int iw = wordbit;
    while ( wordbit < sizeof(word)/sizeof(const char) && iw < (bitlength*bits_selected)) {
        for ( unsigned int isw = 0; isw < bitlength*bits_selected; isw++, ++iw ) {
            shortword[isw] = word[iw];
            if ( iw == bitlength*bits_selected) break;
        }
        break;
    }

    const char* shortword_rotated = __leftrotate( shortword, bitlength, bits_selected/bitlength );
    char* new_word = (char*) malloc(sizeof(char)*wordlength);
    strncpy( new_word, word, wordlength );
    unsigned int isw = 0;
    for ( unsigned int iw = wordbit; iw < wordlength && iw <= wordbit+(bitlength*bits_selected) && isw < bitlength*bits_selected; iw++, ++isw ) {
        new_word[iw] = shortword_rotated[isw];
    }

    return new_word;
}


char* leftrotatemultiple( const char* word, const unsigned int rotate_mulitplicator, const unsigned short wordbit, const unsigned short bitlength, const unsigned int bits_selected ) {

    for ( unsigned int m = 0; m < rotate_mulitplicator; m++ ) {
        char* word = leftrotate( word, wordbit, bitlength, bits_selected );
        if ( m == rotate_mulitplicator-1 ) return word;
    }

}