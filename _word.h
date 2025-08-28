
#ifndef __WORDS_H_
#define __WORDS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <string.h>

#include "_blockbuf.h"

typedef _charbuf_t word_t;

typedef struct _oneptr_ciphers_word_array {
    word_t** words;
    word_t* word;
    unsigned short words_size;
    unsigned short words_index;
} _word_array_t;

_word_array_t* _oneptr_ciphers_word_array_init( const unsigned short number_words, const unsigned short wordsize );
bool _oneptr_ciphers_word_array_switch( _word_array_t* word_array, const unsigned short wordnum );
char* _oneptr_ciphers_word_array( _word_array_t* word_array, const unsigned short wordnum );
char* _oneptr_ciphers_word_array_complete( _word_array_t* word_array );
char* _oneptr_ciphers_word_array_word( _word_array_t* word_array );
void _oneptr_ciphers_word_array_writeto( _word_array_t* word_array, const unsigned short wordnum, const char* _Source );


typedef struct _oneptr_ciphers_word_table {
    _word_array_t** word_lanes;
    _word_array_t* word_lane;
    unsigned short word_lanes_size;
    unsigned short word_lanes_index;
    unsigned short words_size;
} _word_table_t;

_word_table_t* _oneptr_ciphers_word_table_init( const unsigned short number_words, const unsigned short wordsize, const unsigned short number_word_lanes );
bool _oneptr_ciphers_word_table_switch( _word_table_t* word_table, const unsigned short wordlanenum );
bool _oneptr_ciphers_word_table_switch2( _word_table_t* word_table, const unsigned short wordlanenum, const unsigned short wordnum );
char* _oneptr_ciphers_word_table( _word_table_t* word_table, const unsigned short wordlanenum, const unsigned short wordnum );
char* _oneptr_ciphers_word_table_lane( _word_table_t* word_table, const unsigned short wordlanenum );
char* _oneptr_ciphers_word_table_complete( _word_table_t* word_table );
void _oneptr_ciphers_word_table_writeto( _word_table_t* word_table, const unsigned short wordlanenum, const unsigned short wordnum, const char* _Source );


unsigned short _getpos_laneshifting_bitblocks( const unsigned short bitblocks, const unsigned short shifting_modificator, const unsigned short start_pos, const bool shift_direction_left );
unsigned short _getpos_laneshifting( const unsigned short wordlength, const unsigned short shifting_modificator, const unsigned short start_pos, const bool shift_direction_left );
word_t* _rightshift_bitblock( _word_array_t* word_lane, const unsigned short w, const unsigned short origin_w, const unsigned short shifting_modificator, const bool rightshift );
bool _leftrotate_and_rightshift_bitblocks( _word_array_t* word_lane, const unsigned short w, const unsigned short shift_positions );

#ifdef __cplusplus
}
#endif

#endif // __WORDS_H_