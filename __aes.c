
#include <math.h>

#include "__aes_utils.h"


void __aes_round_generic( _word_table_t* msg_table, _word_table_t* key_table, const unsigned short r, const unsigned short rounds ) {
    __aes_sub_bytes( msg_table );
    for ( unsigned short row = 0; row < msg_table->word_lanes_size; row++ ) { __aes_shift_row( msg_table, row ); }
    msg_table = __aes_mix_columns( msg_table );
    __aes_add_round_key( msg_table, key_table );
}


aes_t* __aes_generic( const char* msg, const char* key, const short elements, const short wordsize ) {

    unsigned int msgsize = sizeof(msg) / sizeof(const char*);
    unsigned int keysize = sizeof(key) / sizeof(const char*);
    unsigned int max_strength = fmax( msgsize, keysize );
    unsigned short R = max_strength == 128 ? 10 : max_strength == 160 ? 11 : max_strength == 192 ? 12 
                            : max_strength == 224 ? 13 : max_strength == 256 ? 14 : 1;

    _word_table_t* key_table = __aes_make_keyexpansion( msg, key );
    _word_table_t* msg_table = __aes_make_word_table484( msg );
    __aes_add_round_key( msg_table, key_table );

    for ( unsigned int r = 1; r < R - 1; r++ ) {
        __aes_round_generic( msg_table, key_table, r, R );
    }

    __aes_sub_bytes( msg_table );
    for ( unsigned short row = 0; row < msg_table->word_lanes_size; row++ ) { __aes_shift_row( msg_table, row ); }
    __aes_add_round_key( msg_table, key_table );

    return make_aes( msg_table, key_table );
}


aes_t* __aes( const char* msg, const char* key ) {
    return __aes_generic( msg, key, 4, 128 );
}
