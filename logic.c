
#include "logic.h"

#include <stdlib.h>
#include <string.h>



char* leftrotatemultiple( const char* word, const unsigned int rotate_mulitplicator, const unsigned short wordbit_length ) {

    const size_t wordlength = sizeof(word) / sizeof(const char);
    const unsigned short n_positions = ( wordlength / wordbit_length ) + ( wordlength % wordbit_length );

    char* word_new = __leftrotate_short( word, n_positions );
    for ( unsigned int m = 1; m < rotate_mulitplicator; m++ ) {
        word_new = __leftrotate_short( word_new, n_positions );
    }

    return word_new;
}



char* rightrotatemultiple( const char* word, const unsigned int rotate_mulitplicator, const unsigned short wordbit_length ) {

    const size_t wordlength = sizeof(word) / sizeof(const char);
    const unsigned short n_positions = ( wordlength / wordbit_length ) + ( wordlength % wordbit_length );

    char* word_new = __rightrotate_short( word, n_positions );
    for ( unsigned int m = 0; m < rotate_mulitplicator; m++ ) {
        word_new = __rightrotate_short( word_new, n_positions );
    }

    return word_new;
}