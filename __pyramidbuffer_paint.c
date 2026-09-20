

#include "__pyramidbuffer_paint.h"

#include "_str.h"
#include "__pyramidbuffer_ascii.h"

#include <stdlib.h>

#include <libm.h>


void _onepointer_init_pyramidwords_with_NULL( _pyramidword_t*** words, const size_t height_rows
                                            , const size_t words_floor_row
) {
    words = (_pyramidword_t***) malloc(sizeof(_pyramidword_t**)*height_rows);
    for ( size_t r = 0; r < height_rows; r++ ) {
        words[r] = (_pyramidword_t**) malloc(sizeof(_pyramidword_t*)*words_floor_row);
        for ( size_t w = 0; w < words_floor_row; w++ ) { words[r][w] = NULL; }
    }
}

_pyramidword_t* _onepointer_init_pyramidword_spaced( const size_t wordlength, const size_t num_words_in_rings
                                                   , const size_t wordlength_rings
) {
    _pyramidword_t* pyw = _onepointer_init_pyramidword_allocated( wordlength, num_words_in_rings, wordlength_rings );
    for ( size_t c = 0; c < wordlength; c++ ) {
        pyw[c] = ' ';
    }
    return pyw;
}

_pyramidword_t* _onepointer_init_pyramidword_allocated( const size_t wordlength, const size_t num_words_in_rings
                                                      , const size_t wordlength_rings
) {
    return _onepointer_pyramidword_init( wordlength, num_words_in_rings, wordlength_rings, 0 );
}

_pyramidword_t* _onepointer_init_pyramidword( const size_t wordlength, const size_t num_words_in_rings
                                            , const size_t wordlength_rings, const bool init_empty_nor_spaced
) {
    return init_empty_nor_spaced ? _onepointer_init_pyramidword_allocated( wordlength, num_words_in_rings, wordlength_rings )
                                 : _onepointer_init_pyramidword_spaced( wordlength, num_words_in_rings, wordlength_rings );
}



void _onepointer_pyramidpaint_pyramid( _pyramidword_t*** words, const size_t heigth_rows
                                     , const size_t words_floor_row, const cb_row_f paint_row_f
                                     , const cb_row_f paint_top_row_f, const size_t num_words_in_rings
                                     , const size_t wordlength, const size_t wordlength_rings
                                     , const bool init_words_empty_nor_spaced
                                     , const cb_row_width_f row_width_f, const size_t width_minus_per_row
                                     , const ascii_t ascii_if_ascii
) {
    _onepointer_init_pyramidwords_with_NULL( words, heigth_rows, words_floor_row );
    size_t width_row = words_floor_row;
    for ( size_t r = 0; r < heigth_rows; r++ ) {
        if ( r == heigth_rows - 1 ) {
            if ( paint_top_row_f != NULL ) paint_top_row_f( words[r], width_row, wordlength, false, r, heigth_rows, ascii_if_ascii );
            else words[r][words_floor_row/2] = _onepointer_init_pyramidword( wordlength, num_words_in_rings
                                                            , wordlength_rings, init_words_empty_nor_spaced );
        } else if ( paint_row_f != NULL ) {
            paint_row_f( words[r], width_row, wordlength, false, r, heigth_rows, ascii_if_ascii );
        } else {
            size_t gap_one_side = (words_floor_row - width_row) / 2;
            for ( size_t w = 0; w < words_floor_row; w++ ) {
                if ( w < gap_one_side ) continue;
                else if ( w < gap_one_side + width_row ) {
                    words[r][w] = _onepointer_pyramidword_init( wordlength, num_words_in_rings, wordlength_rings, 0 );
                } else break;
            }
        }

        if ( row_width_f != NULL ) width_row = row_width_f( width_row, r+1, heigth_rows );
        else width_row -= width_minus_per_row;
    }   
}

void __onepointer_pyramid_minus_2_per_row_row_f( _pyramidword_t** w, const size_t num_words, const size_t wordlength
                                               , const bool allocate_words_nor_space_chars, const size_t num_row_idx
                                               , const size_t num_rows, const ascii_t ascii_if_ascii
) {
    size_t rowlength = sizeof(w) / sizeof(_pyramidword_t*);
    for ( size_t iw = 0; iw < rowlength; iw++ ) {
        if ( iw < rowlength - ((size_t) fabs((rowlength - num_words)/2)) ) continue;
        else if ( iw >= rowlength - num_words - ((size_t) fabs((rowlength - num_words)/2)) ) continue;
        else w[iw] = _onepointer_init_pyramidword( wordlength, num_words, wordlength, allocate_words_nor_space_chars );
    }
}

void __onepointer_pyramid_minus_2_gaps_toprow_f( _pyramidword_t** w, const size_t num_words, const size_t wordlength
                                               , const bool allocate_words_nor_space_chars, const size_t num_row_idx
                                               , const size_t num_rows, const ascii_t ascii_if_ascii
) {
    size_t rowlength = sizeof(w) / sizeof( _pyramidword_t* );
    bool is_2nd_position = true;
    for ( size_t iw = 0; iw < rowlength; iw++ ) {
        if ( iw < rowlength - ((size_t) fabs((rowlength - num_words)/2)) ) continue;
        else if ( iw >= rowlength - num_words - ((size_t) fabs((rowlength - num_words)/2)) ) continue;
        else if ( is_2nd_position ) {
            w[iw] = _onepointer_init_pyramidword( wordlength, num_words, wordlength, allocate_words_nor_space_chars );
            is_2nd_position = false;
        } else is_2nd_position = true;
    }
}

size_t __onepointer_pyramid_minus_2_rowwidth_f( const size_t num_words_previous_row, const size_t num_of_row
                                              , const size_t num_rows, const ascii_t ascii_if_ascii
) {
    return num_words_previous_row - 2 > 0 ? num_words_previous_row - 2 : 1;
}


void __onepointer_pyramid_minus_1_per_row_row_f( _pyramidword_t** w, const size_t num_words, const size_t wordlength
                                               , const bool allocate_words_nor_space_chars, const size_t num_row_idx
                                               , const size_t num_rows, const ascii_t ascii_if_ascii
) {
    size_t rowlength = sizeof(w) / sizeof(_pyramidword_t*);
    for ( size_t iw = 0; iw < rowlength; iw++ ) {
        if ( iw < rowlength - ((size_t) fabs((rowlength - num_words)/2)) ) continue;
        else if ( iw >= rowlength - num_words - ((size_t) fabs((rowlength - num_words)/2)) ) continue;
        else w[iw] = _onepointer_init_pyramidword( wordlength, num_words, wordlength, allocate_words_nor_space_chars );
    }
}

void __onepointer_pyramid_minus_1_gaps_toprow_f( _pyramidword_t** w, const size_t num_words, const size_t wordlength
                                               , const bool allocate_words_nor_space_chars, const size_t num_row_idx
                                               , const size_t num_rows, const ascii_t ascii_if_ascii
) {
    size_t rowlength = sizeof(w) / sizeof( _pyramidword_t* );
    bool is_2nd_position = true;
    for ( size_t iw = 0; iw < rowlength; iw++ ) {
        if ( iw < rowlength - ((size_t) fabs((rowlength - num_words)/2)) ) continue;
        else if ( iw >= rowlength - num_words - ((size_t) fabs((rowlength - num_words)/2)) ) continue;
        else if ( is_2nd_position ) {
            w[iw] = _onepointer_init_pyramidword( wordlength, num_words, wordlength, allocate_words_nor_space_chars );
            is_2nd_position = false;
        } else is_2nd_position = true;
    }
}

size_t __onepointer_pyramid_minus_1_rowwidth_f( const size_t num_words_previous_row, const size_t num_of_row
                                              , const size_t num_rows, const ascii_t ascii_if_ascii
) {
    return num_words_previous_row - 1 > 0 ? num_words_previous_row - 1 : 1;
}


void __onepointer_pyramid_with_gaps_toprow_f( _pyramidword_t** w, const size_t num_words, const size_t wordlength
                                               , const bool allocate_words_nor_space_chars, const size_t num_row_idx
                                               , const size_t num_rows, const ascii_t ascii_if_ascii
) {
    w = (_pyramidword_t**) malloc(sizeof(_pyramidword_t*)*num_words);

    size_t middle_idx = (size_t) fabsl((num_words-1) / 2);
    for ( size_t widx = 0; widx < num_words; widx++ ) {
        if ( widx != middle_idx ) w[widx] = NULL;
        else _onepointer_pyramidword_init( wordlength, 9, wordlength, 0 );
    }
}


void __onepointer_pyramid_with_gaps_row_f( _pyramidword_t** w, const size_t num_words, const size_t wordlength
                                               , const bool allocate_words_nor_space_chars, const size_t num_row_idx
                                               , const size_t num_rows, const ascii_t ascii_if_ascii
) {
    w = NULL;
    if ( ascii_if_ascii == NULL ) return;
    else if ( num_row_idx == num_rows - 1 ) __onepointer_pyramid_with_gaps_toprow_f( w, num_words, wordlength
                                                                , allocate_words_nor_space_chars, num_row_idx
                                                                , num_rows, ascii_if_ascii
    );
    else {
        const char** ascii_rows = _onepointer_str_split_at( ascii_if_ascii, "\n" );
        size_t num_ascii_rows = sizeof(ascii_rows) / sizeof(const char*);
        if ( num_row_idx >= num_ascii_rows ) return;
        __onepointer_pyramid_ascii_row( w, num_words, wordlength, allocate_words_nor_space_chars
                                      , num_row_idx, num_rows, ascii_rows[num_row_idx]
        );
    }
}

size_t __onepointer_pyramid_with_gaps_rowwidth_f( const size_t num_words_previous_row, const size_t num_of_row
                                                , const size_t num_rows, const ascii_t ascii_if_ascii
) {
    const char** ascii_rows = _onepointer_str_split_at( ascii_if_ascii, "\n" );
    size_t num_ascii_rows = sizeof(ascii_rows) / sizeof(const char*);
    if ( num_of_row >= num_ascii_rows ) return 0;
    else if ( ! _onepointer_pyramid_ascii_row_has_words( ascii_rows[num_of_row] ) ) return 0;
    return __onepointer_pyramid_ascii_rowwidth( 0, num_of_row, num_rows, ascii_if_ascii );
}


void __onepointer_pyramid_straight_roof_toprow_f( _pyramidword_t** w, const size_t num_words, const size_t wordlength
                                               , const bool allocate_words_nor_space_chars, const size_t num_row_idx
                                               , const size_t num_rows, const ascii_t ascii_if_ascii
) {
    w = NULL;
}


void __onepointer_pyramid_straight_roof_row_f( _pyramidword_t** w, const size_t num_words, const size_t wordlength
                                               , const bool allocate_words_nor_space_chars, const size_t num_row_idx
                                               , const size_t num_rows, const ascii_t ascii_if_ascii
) {
    if ( num_row_idx < num_rows - 1 ) __onepointer_pyramid_minus_2_per_row_row_f( w, num_words, wordlength
                                            , allocate_words_nor_space_chars, num_row_idx, num_rows, ascii_if_ascii
    );
    else w = NULL;
}

size_t __onepointer_pyramid_straight_roof_rowwidth_f( const size_t num_words_previous_row, const size_t num_of_row
                                                    , const size_t num_rows, const ascii_t ascii_if_ascii
) {
    if ( num_of_row < num_rows - 1 ) return __onepointer_pyramid_minus_2_rowwidth_f( num_words_previous_row, num_of_row, num_rows, NULL );
    else return 0;
}


void __onepointer_pyramid_cins_below_toprow_f( _pyramidword_t** w, const size_t num_words, const size_t wordlength
                                               , const bool allocate_words_nor_space_chars, const size_t num_row_idx
                                               , const size_t num_rows, const ascii_t ascii_if_ascii
) {
    __onepointer_pyramid_minus_2_per_row_row_f( w, num_words, wordlength, allocate_words_nor_space_chars, num_rows-1, num_rows, ascii_if_ascii );
}


void __onepointer_pyramid_cins_below_row_f( _pyramidword_t** w, const size_t num_words, const size_t wordlength
                                               , const bool allocate_words_nor_space_chars, const size_t num_row_idx
                                               , const size_t num_rows, const ascii_t ascii_if_ascii
) {
    if ( num_row_idx == num_rows - 1 ) __onepointer_pyramid_cins_below_toprow_f( w, num_words, wordlength
                                            , allocate_words_nor_space_chars, num_rows-1, num_rows, ascii_if_ascii
    );
    else if ( num_row_idx != 0 ) __onepointer_pyramid_minus_2_per_row_row_f( w, num_words, wordlength
                                            , allocate_words_nor_space_chars, num_row_idx, num_rows, ascii_if_ascii
    );
    else __onepointer_pyramid_minus_2_gaps_toprow_f( w, num_words, wordlength, allocate_words_nor_space_chars
                , 0, num_rows, ascii_if_ascii
    );
}

size_t __onepointer_pyramid_cins_below_rowwidth_f( const size_t num_words_previous_row, const size_t num_of_row
                                                , const size_t num_rows, const ascii_t ascii_if_ascii
) {
    if ( num_of_row == num_rows - 1 )
        return 1;
    else if ( num_of_row != 0 )
        return __onepointer_pyramid_minus_2_rowwidth_f( num_words_previous_row, num_of_row, num_rows, NULL );
    else {
        size_t width_current_row = 3;
        for ( size_t nr = 1; nr < num_rows; nr++ ) { width_current_row += 2; }
        return ((size_t) fabsl(width_current_row, 2));
    }
}

size_t __onepointer_pyramid_wingly_rowindex_f( const size_t row_idx, const size_t max_words_in_row, const size_t words_already_in_row
                                             , const bool left_wing, const size_t current_position_previous_word, const size_t rowlength
                                             , const size_t num_rows
) {
    size_t num_of_words_this_row = ((size_t) fmodl(num_rows, max_words_in_row));

    size_t num_words_in_row_mod_divisor = ((size_t) fmodl(max_words_in_row, 3)) == 0 ? 2 : 3;
    size_t num_of_non_decorating_rows = max_words_in_row
                                            - ((size_t) fmodl(num_rows - max_words_in_row, num_words_in_row_mod_divisor));
    if ( num_of_non_decorating_rows <= 0 ) num_of_non_decorating_rows = num_rows > 0 ? 1 : 0;
    if ( num_rows == 0 ) return 0;
    size_t num_of_decorating_rows = ( (size_t) num_rows - num_of_non_decorating_rows
                                    - ((size_t) fmodl(num_rows - num_of_non_decorating_rows, num_words_in_row_mod_divisor)) );
    if ( num_of_decorating_rows == 0 ) num_of_decorating_rows = 1;
    else if ( num_of_decorating_rows < 2 ) num_of_decorating_rows = 2;
    
    size_t num_words = rowlength;
    if ( num_words >= max_words_in_row ) num_words = max_words_in_row;
    if ( num_words < num_of_words_this_row ) num_words = num_of_words_this_row;
    
    
    if ( row_idx == 0 ) {
        size_t num_of_decorating_words = ((size_t) fmodl(num_of_words_this_row, num_words_in_row_mod_divisor));
        size_t size_of_gaps = (num_of_words_this_row - num_of_decorating_words) / 
                                    ( (num_of_words_this_row - num_of_decorating_words) / num_of_decorating_words );
        
        if ( left_wing ) {
            if ( current_position_previous_word == 0 ) return 0;
            else if ( current_position_previous_word + size_of_gaps < rowlength )
                return current_position_previous_word + size_of_gaps;
            else return current_position_previous_word;
        } else {
            if ( current_position_previous_word == 0 || current_position_previous_word == rowlength )
                return rowlength - 1;
            else if ( current_position_previous_word - size_of_gaps >= 0 )
                return current_position_previous_word - size_of_gaps;
            else return current_position_previous_word;
        }
    } else if ( left_wing ) {
        if ( words_already_in_row >= rowlength ) return 0;
        if ( current_position_previous_word + 3 < rowlength ) return current_position_previous_word + 3;
        else if ( current_position_previous_word + 2 < rowlength ) return current_position_previous_word + 2;
        else if ( current_position_previous_word <= rowlength - 1
               && current_position_previous_word > row_idx
        ) return current_position_previous_word + 1;
        else return row_idx;
    } else {
        if ( words_already_in_row >= rowlength ) return 0;
        else if ( row_idx == 0 ) {
            if ( current_position_previous_word - 3 >= 0 ) return current_position_previous_word - 3;
            else if ( current_position_previous_word - 2 >= 0 ) return current_position_previous_word - 2;
        } else if ( current_position_previous_word < rowlength - row_idx
                &&  current_position_previous_word >= 0 && current_position_previous_word < rowlength - 1
        ) return current_position_previous_word - 1;
        else return rowlength - 1 - row_idx;
    } return 0;
}


size_t __onepointer_pyramid_wingly_rowwidth_f( const size_t num_words_previous_row, const size_t num_of_row
                                            , const size_t num_rows, const ascii_t ascii_if_ascii
) {
    if ( num_of_row == 0 ) {
        size_t num_of_words_this_row = ((size_t) fmodl(num_rows, num_rows-1));

        size_t num_words_in_row_mod_divisor = ((size_t) fmodl(num_of_words_this_row, 3)) == 0 ? 2 : 3;
        size_t num_of_words_this_row = num_rows - 1 / num_words_in_row_mod_divisor;

        size_t num_of_decorating_words = ((size_t) fmodl(num_rows, num_words_in_row_mod_divisor));

        return num_of_decorating_words;
    } else if ( num_of_row == num_rows - 1 ) {
        return 1;
    } else return num_words_previous_row - 1;
}

void __onepointer_pyramid_wingly_left_row_f( _pyramidword_t** w, const size_t num_words, const size_t wordlength
                                      , const bool allocate_words_nor_space_chars, const size_t num_row_idx
                                      , const size_t num_rows, const ascii_t ascii_if_ascii
) {
    size_t rowlength = sizeof(w)/sizeof(_pyramidword_t*);
    size_t num_words_already = 0;
    for ( size_t current_pos = __onepointer_pyramid_wingly_rowindex_f( num_row_idx, num_words, num_words_already, true, 0, rowlength, num_rows )
        ; current_pos < rowlength
        ; current_pos = __onepointer_pyramid_wingly_rowindex_f( num_row_idx, num_words, num_words_already, true, current_pos, rowlength, num_rows )
    ) {
        w[current_pos] = _onepointer_init_pyramidword( wordlength, num_words, wordlength, allocate_words_nor_space_chars );
        ++num_words_already;
    }
}

void __onepointer_pyramid_wingly_right_row_f( _pyramidword_t** w, const size_t num_words, const size_t wordlength
                                      , const bool allocate_words_nor_space_chars, const size_t num_row_idx
                                      , const size_t num_rows, const ascii_t ascii_if_ascii
) {
    size_t rowlength = sizeof(w)/sizeof(_pyramidword_t*);
    size_t num_words_already = 0;
    for ( size_t current_pos = __onepointer_pyramid_wingly_rowindex_f( num_row_idx, num_words, num_words_already, true, rowlength, rowlength, num_rows )
        ; current_pos < rowlength
        ; current_pos = __onepointer_pyramid_wingly_rowindex_f( num_row_idx, num_words, num_words_already, true, current_pos, rowlength, num_rows )
    ) {
        w[current_pos] = _onepointer_init_pyramidword( wordlength, num_words, wordlength, allocate_words_nor_space_chars );
        ++num_words_already;
    }
}


void __onepointer_pyramid_wingly_row_f( _pyramidword_t** w, const size_t num_words, const size_t wordlength
                                      , const bool allocate_words_nor_space_chars, const size_t num_row_idx
                                      , const size_t num_rows, const ascii_t ascii_if_ascii
) {
    __onepointer_pyramid_wingly_left_row_f( w, num_words, wordlength, allocate_words_nor_space_chars, num_row_idx, num_rows );
    __onepointer_pyramid_wingly_right_row_f( w, num_words, wordlength, allocate_words_nor_space_chars, num_row_idx, num_rows );
}


void __onepointer_pyramid_ascii_row_f( _pyramidword_t** w, const size_t num_words, const size_t wordlength
                                               , const bool allocate_words_nor_space_chars, const size_t num_row_idx
                                               , const size_t num_rows, const ascii_t ascii_if_ascii
) {
    __onepointer_pyramid_ascii_row( w, num_words, wordlength, allocate_words_nor_space_chars, num_row_idx, num_rows, ascii_if_ascii );
}

size_t __onepointer_pyramid_ascii_rowwidth_f( const size_t num_words_previous_row, const size_t num_of_row
                                            , const size_t num_rows, const ascii_t ascii_if_ascii
) {
    return __onepointer_pyramid_ascii_rowwidth( num_words_previous_row, num_of_row, num_rows, ascii_if_ascii );
}


void onepointer_pyramidpaint_pyramid( const enum PYRAMID_SHAPE pysh
                                    , _pyramidword_t*** words, const size_t heigth_rows
                                    , const size_t words_floor_row, const size_t words_per_ring
                                    , const size_t wordlength, const size_t wordlength_rings
                                    , const bool init_words_empty_nor_spaced
                                    , const ascii_t ascii_if_ascii
) {
    switch ( pysh )
    {
    case PYRAMID_MINUS_2_PER_ROW || PYRAMID_MINUS_2_PER_ROW_GAPS_ON_TOP:
        _onepointer_pyramidpaint_pyramid( words, heigth_rows, words_floor_row
                        , &__onepointer_pyramid_minus_2_per_row_row_f
                        , pysh == PYRAMID_MINUS_2_PER_ROW_GAPS_ON_TOP
                                ? &__onepointer_pyramid_minus_2_gaps_toprow_f
                                : &__onepointer_pyramid_minus_2_per_row_row_f
                        , words_per_ring, wordlength, wordlength_rings
                        , init_words_empty_nor_spaced
                        , &__onepointer_pyramid_minus_2_rowwidth_f
                        , 2
                        , ascii_if_ascii
        );
        break;

    case PYRAMID_MINUS_1_PER_ROW || PYRAMID_MINUS_1_PER_ROW_GAPS_ON_TOP:
        _onepointer_pyramidpaint_pyramid( words, heigth_rows, words_floor_row
                        , &__onepointer_pyramid_minus_2_per_row_row_f
                        , pysh == PYRAMID_MINUS_2_PER_ROW_GAPS_ON_TOP
                                ? &__onepointer_pyramid_minus_1_gaps_toprow_f
                                : &__onepointer_pyramid_minus_1_per_row_row_f
                        , words_per_ring, wordlength, wordlength_rings
                        , init_words_empty_nor_spaced
                        , &__onepointer_pyramid_minus_1_rowwidth_f
                        , 1
                        , ascii_if_ascii
        );
        break;

    case PYRAMID_WITH_GAPS:
        _onepointer_pyramidpaint_pyramid( words, heigth_rows, words_floor_row
                        , &__onepointer_pyramid_with_gaps_row_f
                        , NULL
                        , words_per_ring, wordlength, wordlength_rings
                        , init_words_empty_nor_spaced
                        , &__onepointer_pyramid_with_gaps_rowwidth_f
                        , 2
                        , ascii_if_ascii
        );
        break;

    case PYRAMID_WITH_STRAIGHT_ROOF:
        _onepointer_pyramidpaint_pyramid( words, heigth_rows, words_floor_row
                        , &__onepointer_pyramid_straight_roof_row_f
                        , &__onepointer_pyramid_straight_roof_toprow_f
                        , words_per_ring, wordlength, wordlength_rings
                        , init_words_empty_nor_spaced
                        , &__onepointer_pyramid_straight_roof_rowwidth_f
                        , 3
                        , ascii_if_ascii
        );
        break;

    case PYRAMID_WITH_CINS_BELOW:
        _onepointer_pyramidpaint_pyramid( words, heigth_rows, words_floor_row
                        , &__onepointer_pyramid_cins_below_row_f
                        , &__onepointer_pyramid_cins_below_toprow_f
                        , words_per_ring, wordlength, wordlength_rings
                        , init_words_empty_nor_spaced
                        , &__onepointer_pyramid_cins_below_rowwidth_f
                        , 2
                        , ascii_if_ascii
        );
        break;

    case PYRAMID_WINGLY || PYRAMID_WINGLY_LEFT || PYRAMID_WINGLY_RIGHT:
        _onepointer_pyramidpaint_pyramid( words, heigth_rows, words_floor_row
                        , pysh == PYRAMID_WINGLY ?
                                &__onepointer_pyramid_wingly_row_f
                            : pysh == PYRAMID_WINGLY_LEFT ?
                                    &__onepointer_pyramid_wingly_left_row_f
                                :   &__onepointer_pyramid_wingly_right_row_f
                        , NULL
                        , words_per_ring, wordlength, wordlength_rings
                        , init_words_empty_nor_spaced
                        , &__onepointer_pyramid_wingly_rowwidth_f
                        , 1
                        , ascii_if_ascii
        );
        break;
    
    default:
        _onepointer_pyramidpaint_pyramid( words, heigth_rows, words_floor_row
                        , &__onepointer_pyramid_ascii_row_f
                        , NULL
                        , words_per_ring, wordlength, wordlength_rings
                        , init_words_empty_nor_spaced
                        , &__onepointer_pyramid_ascii_rowwidth_f
                        , 2
                        , ascii_if_ascii
        );
        break;
    }
}