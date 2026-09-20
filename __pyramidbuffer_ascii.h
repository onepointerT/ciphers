
#ifndef __PYRAMIDBUFFER_ASCII_H_
#define __PYRAMIDBUFFER_ASCII_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "_pyramidbuffer.h"
#include "__pyramidbuffer_paint.h"

#include <stdbool.h>




bool _onepointer_pyramid_ascii_row_has_words( const ascii_t ascii_if_ascii );

void __onepointer_pyramid_ascii_row( _pyramidword_t** w, const size_t num_words, const size_t wordlength
                                   , const bool allocate_words_nor_space_chars, const size_t num_row_idx
                                   , const size_t num_rows, const ascii_t ascii_if_ascii
);
size_t __onepointer_pyramid_ascii_rowwidth( const size_t num_words_previous_row, const size_t num_of_row
                                          , const size_t num_rows, const ascii_t ascii_if_ascii
);


void _onepointer_pyramidpaint_ascii( _pyramidword_t*** words, const size_t heigth_rows
                                   , const size_t words_floor_row, const cb_row_f paint_row_f
                                   , const cb_row_f paint_top_row_f, const size_t words_per_ring
                                   , const size_t wordlength, const size_t wordlength_rings
                                   , const bool init_words_empty_nor_spaced
                                   , const cb_row_width_f row_width_f
                                   , const ascii_t ascii_if_ascii
);


void onepointer_pyramidascii_pyramid( const enum PYRAMID_SHAPE pysh
                                    , _pyramidword_t*** words, const size_t heigth_rows
                                    , const size_t words_floor_row, const size_t words_per_ring
                                    , const size_t wordlength, const size_t wordlength_rings
                                    , const bool init_words_empty_nor_spaced
                                    , const ascii_t ascii_if_ascii
);

#ifdef __cplusplus
}
#endif

#endif // __PYRAMIDBUFFER_ASCII_H_