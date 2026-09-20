
#include "__pyramidbuffer_ascii.h"

#include "_str.h"

#include <stdlib.h>


bool _onepointer_pyramid_ascii_row_has_words( const ascii_t ascii_if_ascii ) {
    return strfind( ascii_if_ascii, '=' ) > 0;
}


void __onepointer_pyramid_ascii_row_f( _pyramidword_t** w, const size_t num_words, const size_t wordlength
                                     , const bool allocate_words_nor_space_chars, const size_t num_row_idx
                                     , const size_t num_rows, const ascii_t ascii_if_ascii
) {
    size_t num_words = strfind(ascii_if_ascii, '=');
    size_t num_gaps = strfind(ascii_if_ascii, ' ');
    size_t num_rings = strfind(ascii_if_ascii, 'O');
    size_t num_elements = num_words + num_gaps + num_rings;

    w = (_pyramidword_t**) malloc(sizeof(_pyramidword_t*)*(num_words+num_gaps));
    for ( size_t wi = 0; wi < num_words+num_gaps; wi++ ) {
        w[wi] = ascii_if_ascii[wi] == ' ' ? NULL
                    : ( ascii_if_ascii[wi] == 'O' ? _onepointer_pyramidword_init( 0, 9, 8, 0 )
                                                  : _onepointer_pyramidword_init( wordlength, 0, 8, 0 )
                );
        if ( ! allocate_words_nor_space_chars ) {
            for ( size_t widx = 0; widx < wordlength; widx++ ) { w[wi]->word->buf[widx] = ' '; }
        }
    }
}


size_t __onepointer_pyramid_ascii_rowwidth_f( const size_t num_words_previous_row, const size_t num_of_row
                                            , const size_t num_rows, const ascii_t ascii_if_ascii
) {
    size_t num_words = strfind(ascii_if_ascii, '=');
    size_t num_gaps = strfind(ascii_if_ascii, ' ');
    size_t num_rings = strfind(ascii_if_ascii, 'O');
    return num_words + num_gaps + num_rings;
}


void __onepointer_pyramid_ascii_row( _pyramidword_t** w, const size_t num_words, const size_t wordlength
                                   , const bool allocate_words_nor_space_chars, const size_t num_row_idx
                                   , const size_t num_rows, const ascii_t ascii_if_ascii
) {
    __onepointer_pyramid_ascii_row_f( w, num_words, wordlength, allocate_words_nor_space_chars, num_row_idx, num_rows, ascii_if_ascii );
}


size_t __onepointer_pyramid_ascii_rowwidth( const size_t num_words_previous_row, const size_t num_of_row
                                          , const size_t num_rows, const ascii_t ascii_if_ascii
) {
    return __onepointer_pyramid_ascii_rowwidth_f( num_words_previous_row, num_of_row, num_rows, ascii_if_ascii );
}


void _onepointer_pyramidpaint_ascii( _pyramidword_t*** words, const size_t heigth_rows
                                   , const size_t words_floor_row, const cb_row_f paint_row_f
                                   , const cb_row_f paint_top_row_f, const size_t words_per_ring
                                   , const size_t wordlength, const size_t wordlength_rings
                                   , const bool init_words_empty_nor_spaced
                                   , const cb_row_width_f row_width_f
                                   , const ascii_t ascii_if_ascii
) {
    if ( ascii_if_ascii == NULL ) return;

    const char** rows = _onepointer_str_split_at( ascii_if_ascii, '\n' );
    const size_t num_ascii_rows = sizeof(rows) / sizeof(char*);
    words = (_pyramidword_t***) malloc(sizeof(_pyramidword_t**)*heigth_rows);
    
    // Paint the rows
    size_t row_idx = 0;
    for ( size_t wr_idx = 0; wr_idx < heigth_rows; wr_idx++ ) {
        words[wr_idx] = NULL;

        if ( row_idx >= num_ascii_rows ) continue;

        size_t num_words = strfind( rows[wr_idx], '=' );
        if ( wr_idx == heigth_rows - 1 && paint_top_row_f != NULL )
            paint_top_row_f( words[wr_idx], num_words, wordlength, init_words_empty_nor_spaced
                           , wr_idx, heigth_rows, ascii_if_ascii
            
            );
        else if ( paint_row_f != NULL )
            paint_row_f( words[wr_idx], num_words, wordlength, init_words_empty_nor_spaced
                       , wr_idx, heigth_rows, ascii_if_ascii
            );
        else __onepointer_pyramid_ascii_row( words[wr_idx], num_words, wordlength, init_words_empty_nor_spaced
                                           , wr_idx, heigth_rows, ascii_if_ascii
        );
    }
}




void onepointer_pyramidascii_pyramid( const enum PYRAMID_SHAPE pysh
                                    , _pyramidword_t*** words, const size_t heigth_rows
                                    , const size_t words_floor_row, const size_t words_per_ring
                                    , const size_t wordlength, const size_t wordlength_rings
                                    , const bool init_words_empty_nor_spaced
                                    , const ascii_t ascii_if_ascii
) {
    _onepointer_pyramidpaint_ascii( words, heigth_rows, words_floor_row
                                  , &__onepointer_pyramid_ascii_row
                                  , NULL
                                  , words_per_ring, wordlength, wordlength_rings
                                  , init_words_empty_nor_spaced
                                  , &__onepointer_pyramid_ascii_rowwidth
                                  , ascii_if_ascii
    );
}


