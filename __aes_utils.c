
#include "__aes_utils.h"

#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "logic.h"
#include "_blockbuf.h"
#include "_word.h"



char* __aes_sbox_rijndael( const char* x ) {
    double minv_X = __multiplicative_inverse(*((double*) x), 2, 8);
    const char* minv_char_X = (char*) &minv_X;
    return __xor_str((char*) minv_char_X, 
            __xor_str(__leftrotate_short(minv_char_X, 1),
            __xor_str(__leftrotate_short(minv_char_X, 2),
            __xor_str(__leftrotate_short(minv_char_X, 3),
            __xor_str(__leftrotate_short(minv_char_X, 4), (char*) 0x63))))
        );
}


const char* __aes_calculate_Cj( const unsigned int j ) {
    unsigned int bin_j = exp( pow( 2, j ) ) * ln(pow(2,8));
    const char* bin_char_j = (char*) bin_j;
    const unsigned int j_length = sizeof(bin_char_j) / sizeof(char);

    char* C = (char*) malloc(sizeof(char)*j);
    unsigned int i2 = j_length-1;
    for ( unsigned int i = j; i >= 0; i-- ) {
        if ( i2 >= 0 ) { C[i] = bin_char_j[i2]; i2--; }
        else C[i] = '0';
    }

    return C;
}


const char* __aes_keyexpansion( const char* word, const unsigned short R, size_t wordlength ) {

    
    const unsigned int N = wordlength;
    const unsigned int b = 1;

    char* new_word = (char*) malloc(sizeof(char*)/sizeof(char)*wordlength);
    strcpy( new_word, word );
    for ( unsigned int i = N; i >= b/32 * (R+1) - 1; i-- ) {
        if ( i == 0 % N ) {
            new_word[i] = (char) __xor_str( (char*) word[_getpos_laneshifting(wordlength, N, i, true)],
                            __xor_str( __aes_sbox_rijndael(__leftrotate_short((char*) word[_getpos_laneshifting(wordlength, 1, i, true)], 8)), __aes_calculate_Cj(i/(N-1)) ) );
        } else if ( N > 6 && i == 4 % N ) {
            new_word[i] = (char) __xor_str( (char*) word[_getpos_laneshifting(wordlength, N, i, true)], (char*) __aes_sbox_rijndael((char*) word[_getpos_laneshifting(wordlength, 1, i, true)]) );
        } else {
            new_word[i] = (char) __xor_str( (char*) word[_getpos_laneshifting(wordlength, N, i, true)], (char*) word[_getpos_laneshifting(wordlength, 1, i, true)] );
        }
    }
}


_word_table_t* __aes_add_round_key( const _word_table_t* word_table, const _word_table_t* key_table ) {

    _word_table_t* word_table2 = _oneptr_ciphers_word_table_init( 4, 8, 4 );

    unsigned short wl = 0;
    for ( unsigned short w = 0; w < 4 && wl < 4; ) {
        const char* new_keyed_word = __xor_str( _oneptr_ciphers_word_table(word_table, wl, w)
                                              , _oneptr_ciphers_word_table(key_table, wl, w) );
        _oneptr_ciphers_word_table_writeto( word_table, wl, w, new_keyed_word );
        if ( w == 3 ) { wl++; w = 0; }
        else w++;
    }

    return word_table2;
}


void __aes_sub_bytes( _word_table_t* word_table ) {

    for ( unsigned int wl = 0; wl < word_table->word_lanes_size; wl++ ) {
        for ( unsigned int w = 0; w < word_table->words_size; w++ ) {
            _oneptr_ciphers_word_table_writeto( word_table, wl, w, __aes_sbox_rijndael(_oneptr_ciphers_word_table(word_table, wl, w)));
        }
    }

}


unsigned short __aes_shift_row_shiftmodificator( const unsigned short row, const unsigned short bitblock_length, const unsigned short wordlength ) {
    return row == 1 ?  1
                : row == 2 && wordlength < 256 ? 2
                : row == 2 && wordlength >= 256 ? 3
                : row == 3 && wordlength < 224 ? 3
                : row == 3 && wordlength >= 224 ? 4 : 0;
}

unsigned short __aes_shift_row_shiftmodificator_bits( const unsigned short row, const unsigned short bitblock_length, const unsigned short wordlength ) {
    unsigned short shift_multiplicator = __aes_shift_row_shiftmodificator( row, bitblock_length, wordlength );
    return shift_multiplicator * bitblock_length;
}


char* __aes_shift_row_bitwise( const char* word, const unsigned short row, const unsigned short bitblock_start, const unsigned short bitblocks_count ) {

    const unsigned int wordlength = sizeof(word) / sizeof(const char);
    const unsigned short bitblock_length = wordlength / bitblocks_count;
    
    if ( row == 0 ) return word;

    char* bitblock_tmp = (char*) malloc(sizeof(char)*bitblock_length);
    char* bitblock_old = (char*) malloc(sizeof(char)*bitblock_length);
    unsigned short w_idx = bitblock_start;
    // Copy old bitblock
    for ( unsigned short wo_idx = 0
        ; wo_idx < bitblock_length && w_idx < wordlength && w_idx <= bitblock_start+bitblock_length
        ; wo_idx++, w_idx++
    ) {
        bitblock_old[wo_idx] = word[w_idx];
    }

    unsigned short shift_modificator = __aes_shift_row_shiftmodificator_bits( row, bitblock_length, wordlength );
    unsigned short winsert_block_idx = _getpos_laneshifting( wordlength, shift_modificator, bitblock_length - shift_modificator, true );

    w_idx = winsert_block_idx;
    for ( unsigned short wt_idx = 0
        ; wt_idx < bitblock_length && w_idx < winsert_block_idx + bitblock_length && w_idx < wordlength
        ; wt_idx++, w_idx++
    ) {
        bitblock_tmp[wt_idx] = word[w_idx];
    }

    char* new_bitblock = __xor_str( bitblock_tmp, bitblock_old );

    char* new_word = (char*) malloc(sizeof(char)*wordlength);
    strcpy( new_word, word );

    w_idx = winsert_block_idx;
    for ( unsigned short nbb_idx = 0
        ; nbb_idx < bitblock_length && w_idx < winsert_block_idx + bitblock_length && w_idx < wordlength
        ; nbb_idx++, w_idx++
    ) {
        new_word[w_idx] = new_bitblock[nbb_idx];
    }
}


void __aes_shift_row_bitblock( _word_array_t* word_lane, const unsigned short wl, const unsigned short w, const unsigned short shift_count ) {

    if ( w >= word_lane->words_size ) return;
    
    _leftrotate_and_rightshift_bitblocks( word_lane, w, shift_count );
}


void __aes_shift_row( _word_table_t* word_table, const unsigned short wl ) {

    if ( wl >= word_table->word_lanes_size ) return;

    _word_array_t* wa = word_table->word_lanes[wl];
    unsigned short bitblock_length = wa->words[0]->bufsize;
    unsigned int wordlength = bitblock_length * wa->words_size;

    unsigned short shift_modificator = __aes_shift_row_shiftmodificator( wl, bitblock_length, wordlength );
    __aes_shift_row_bitblock( wa, wl, 0, shift_modificator );
}



char* __aes_mix_columns_calc_product( const word_t* word, const unsigned short factor ) {
    if ( factor == 1 ) return word->buf;
    else if ( factor == 2 ) return (char*) ((int) word->buf < 128 ? 2 * (int) word->buf : __xor_str((char*) (2 * (int) word->buf), (char*) 0x11b));
    else if ( factor == 3 ) return __xor_str( __aes_mix_columns_calc_product( word, 2 ), word->buf );
    else if ( factor == 4 ) return __xor_str( __aes_mix_columns_calc_product( word, 3 ), word->buf );
    else return __xor_str( __xor_str( __aes_mix_columns_calc_product( word, factor - (0.5 * (factor % 3))  )
                                , __xor_str( __aes_mix_columns_calc_product( word, factor - (0.5 * (factor % 7))  )
                                            , __aes_mix_columns_calc_product( word, factor - (factor % 6))
                                        )
                                )
                    , word->buf
            );
}



void __aes_mix_columns_calc_column_default4( const _word_table_t* origin, _word_table_t* destination, const unsigned short column ) {
    if ( column > origin->word_lanes_size || column > destination->word_lanes_size ) return;
    destination->word_lanes[column]->words[0]->buf
            = __xor_str( __aes_mix_columns_calc_product( origin->word_lanes[column]->words[0], 2 )
                , __xor_str( __aes_mix_columns_calc_product( origin->word_lanes[column]->words[1], 3 )
                , __xor_str( __aes_mix_columns_calc_product( origin->word_lanes[column]->words[2], 1 )
                        , __aes_mix_columns_calc_product( origin->word_lanes[column]->words[3], 1 ) ) )
    );
    destination->word_lanes[column]->words[0]->buf
            = __xor_str( __aes_mix_columns_calc_product( origin->word_lanes[column]->words[0], 1 )
                , __xor_str( __aes_mix_columns_calc_product( origin->word_lanes[column]->words[1], 2 )
                , __xor_str( __aes_mix_columns_calc_product( origin->word_lanes[column]->words[2], 3 )
                        , __aes_mix_columns_calc_product( origin->word_lanes[column]->words[3], 1 ) ) )
    );
    destination->word_lanes[column]->words[0]->buf
            = __xor_str( __aes_mix_columns_calc_product( origin->word_lanes[column]->words[0], 1 )
                , __xor_str( __aes_mix_columns_calc_product( origin->word_lanes[column]->words[1], 1 )
                , __xor_str( __aes_mix_columns_calc_product( origin->word_lanes[column]->words[2], 2 )
                        , __aes_mix_columns_calc_product( origin->word_lanes[column]->words[3], 3 ) ) )
    );
    destination->word_lanes[column]->words[0]->buf
            = __xor_str( __aes_mix_columns_calc_product( origin->word_lanes[column]->words[0], 3 )
                , __xor_str( __aes_mix_columns_calc_product( origin->word_lanes[column]->words[1], 1 )
                , __xor_str( __aes_mix_columns_calc_product( origin->word_lanes[column]->words[2], 1 )
                        , __aes_mix_columns_calc_product( origin->word_lanes[column]->words[3], 2 ) ) )
    );
}


_word_table_t* __aes_mix_columns( const _word_table_t* origin ) {
    _word_table_t* destination_b = _oneptr_ciphers_word_table_init( origin->words_size, origin->word_lanes[0]->words[0]->bufsize, origin->word_lanes_size );
    for ( unsigned short i = 0; i < origin->word_lanes_size; i++ ) {
        __aes_mix_columns_calc_column_default4( origin, destination_b, i );
    }
    return destination_b;
}


_word_table_t* __aes_make_word_table484( const char* word ) {

    _word_table_t* key_table = _oneptr_ciphers_word_table_init( 4, 8, 4 );

    size_t keysize = sizeof(word) / sizeof(const char);
    unsigned short wl = 0;
    unsigned int w = 0;
    unsigned int k_start = 0;
    for ( unsigned int k = 0
        ; k < keysize && wl < key_table->word_lanes && w < key_table->words_size
        ; k = k + 8
    ) {
        char* keypart = (char*) malloc(sizeof(char)*8);
        unsigned short kpi = 0;
        for ( unsigned int kp = k; kp < k + 9 && kpi < 8; kp++ ) {
            keypart[kpi] = word[kp];
        }
        _oneptr_ciphers_word_table_writeto( key_table, wl, w, keypart );
        if ( w == key_table->words_size - 1 ) w = 0;
        else w++;
    }

    return key_table;
}


_word_table_t* __aes_make_keyexpansion( const char* word, const char* key ) {

    size_t keylength = sizeof(key) / sizeof(const char);
    const char* key_expanded = __aes_keyexpansion( key, 0, keylength );
    _word_table_t* key_table = __aes_make_word_table484( key_expanded );

    _word_table_t* word_table = __aes_make_word_table484( word );

    return __aes_add_round_key( word_table, key_table );
}

aes_t* make_aes( _word_table_t* msg_table, _word_table_t* key_table ) {
    aes_t* aes = _oneptr_ciphers_init_aes( msg_table->word_lanes[0]->words[0]->bufsize, msg_table->word_lanes_size );

    strcpy( aes->msg->hash, _oneptr_ciphers_word_table_complete( msg_table ) );
    strcpy( aes->key->hash, _oneptr_ciphers_word_table_complete( key_table ) );

    return aes;
}