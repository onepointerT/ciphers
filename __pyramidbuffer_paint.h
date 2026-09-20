
#ifndef __PYRAMIDBUFFER_PAINT_H_
#define __PYRAMIDBUFFER_PAINT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "_pyramidbuffer.h"

#include <stdbool.h>


void _onepointer_init_pyramidwords_with_NULL( _pyramidword_t*** words, const size_t height_rows
                                            , const size_t words_floor_row
);

_pyramidword_t* _onepointer_init_pyramidword_spaced( const size_t wordlength, const size_t num_words_in_rings
                                                   , const size_t wordlength_rings, const size_t words_in_rings
);

_pyramidword_t* _onepointer_init_pyramidword_allocated( const size_t wordlength, const size_t num_words_in_rings
                                                      , const size_t wordlength_rings, const size_t words_in_rings
);

_pyramidword_t* _onepointer_init_pyramidword( const size_t wordlength, const size_t num_words_in_rings
                                            , const size_t wordlength_rings, const bool init_empty_nor_spaced
);

typedef void (*cb_row_f)( _pyramidword_t** w, const size_t num_words, const size_t wordlength, const bool allocate_words_nor_space_chars
                            , const size_t num_row_idx, const size_t num_rows, const ascii_t ascii_if_ascii );
typedef size_t (*cb_row_width_f)( const size_t num_words_previous_row, const size_t num_of_row, const size_t num_rows, const ascii_t ascii_if_ascii );

void _onepointer_pyramidpaint_pyramid( _pyramidword_t*** words, const size_t heigth_rows
                                     , const size_t words_floor_row, const cb_row_f paint_row_f
                                     , const cb_row_f paint_top_row_f, const size_t words_per_ring
                                     , const size_t wordlength, const size_t wordlength_rings
                                     , const bool init_words_empty_nor_spaced
                                     , const cb_row_width_f row_width_f
                                     , const ascii_t ascii_if_ascii
);


void onepointer_pyramidpaint_pyramid( const enum PYRAMID_SHAPE pysh
                                    , _pyramidword_t*** words, const size_t heigth_rows
                                    , const size_t words_floor_row, const size_t words_per_ring
                                    , const size_t wordlength, const size_t wordlength_rings
                                    , const bool init_words_empty_nor_spaced
                                    , const ascii_t ascii_if_ascii
);

#ifdef __cplusplus
}
#endif

#endif // __PYRAMIDBUFFER_PAINT_H_