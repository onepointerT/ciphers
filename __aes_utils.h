

#ifndef __AES_UTILS_H_
#define __AES_UTILS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "ciphers.h"

#include "_word.h"



char* __aes_sbox_rijndael( const char* x );

const char* __aes_calculate_Cj( const unsigned int j );
const char* __aes_keyexpansion( const char* word, const unsigned short R, size_t wordlength );
_word_table_t* __aes_add_round_key( const _word_table_t* word_table, const _word_table_t* key_table );

void __aes_sub_bytes( _word_table_t* word_table );

unsigned short __aes_shift_row_shiftmodificator( const unsigned short row, const unsigned short bitblock_length, const unsigned short wordlength );
unsigned short __aes_shift_row_shiftmodificator_bits( const unsigned short row, const unsigned short bitblock_length, const unsigned short wordlength );
char* __aes_shift_row_bitwise( const char* word, const unsigned short row, const unsigned short bitblock_start, const unsigned short bitblocks_count );
void __aes_shift_row_bitblock( _word_array_t* word_lane, const unsigned short wl, const unsigned short w, const unsigned short shift_count );
void __aes_shift_row( _word_table_t* word_table, const unsigned short wl );

char* __aes_mix_columns_calc_product( const word_t* word, const unsigned short factor );
void __aes_mix_columns_calc_column_default4( const _word_table_t* origin, _word_table_t* destination, const unsigned short column );
_word_table_t* __aes_mix_columns( const _word_table_t* origin );

_word_table_t* __aes_make_word_table484( const char* word );
_word_table_t* __aes_make_keyexpansion( const char* word, const char* key );


void __aes_round_generic( _word_table_t* msg_table, _word_table_t* key_table, const unsigned short r, const unsigned short rounds );
aes_t* __aes_generic( const char* msg, const char* key, const short elements, const short wordsize );
aes_t* __aes( const char* msg, const char* key );

aes_t* make_aes( _word_table_t* msg_table, _word_table_t* key_table );

#ifdef __cplusplus
}
#endif

#endif // __AES_UTILS_H_