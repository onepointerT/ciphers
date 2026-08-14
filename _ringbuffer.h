
#ifndef __RINGBUFFER_H_
#define __RINGBUFFER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "_word.h"


typedef enum RINGBUFFER_SHIFTING_ALGORITHMUS {
    SHIFT_WORDS,
    SHIFT_NEIGHBOURS,
    SWAP_NEIGHBOURS,
    SWAP_SYMETRIC,
    SWAP_ASYMETRIC,
    SWAP_ASYMETRIC_AND_SHIFT,
    ROTATE_SYMETRICALLY,
    ROTATE_ASYMETRICALLY
} _ringbuffer_shiftalgo_t;

_ringbuffer_shiftalgo_t* _onepointer_ringbuffer_shiftalgorithms_array_init( const size_t algorithm_count );

typedef struct _onepointer_ciphers_ringbuffer {
    word_t** words;
    size_t ringsize;
    size_t wordlen;
    bool shifted;
    unsigned short shift_count;
} _word_ringbuf_t;


_word_ringbuf_t* _onepointer_ringbuffer_init( const size_t ringsize, const size_t wordlen );
void _onepointer_ringbuffer_copy_word( _word_ringbuf_t* wrb, word_t* word, const size_t into_wordof_ring );
void _onepointer_ringbuffer_copy_words_to_wordarray( const _word_ringbuf_t* wrb, _word_array_t* wa, const size_t startpos, const size_t _offset );
void _onepointer_ringbuffer_copy_words_from_wordarray( _word_ringbuf_t* wrb, const _word_array_t* wa, const size_t startpos_dest, const int _offset );
void _onepointer_ringbuffer_insert_word( _word_ringbuf_t* wrb, word_t* word, const size_t into_wordof_ring
                                       , word_t* (*cb_insertion_algorithm)(word_t*, word_t*)
);

const size_t _onepointer_ringbuffer_wordindex_contiguous( const _word_ringbuf_t* wrb, const size_t startpos, const int _offset );
void _onepointer_ringbuffer_get_contiguous( _word_ringbuf_t* wrb, word_t* wptr, const size_t startpos, const int _offset );
bool _onepointer_ringbuffer_shift_at_ring_contiguous( _word_ringbuf_t* wrb, const size_t startpos, const size_t _offset_one_site
                                                    , const int wordpositions, const bool swap_left_nots_right
);
bool _onepointer_ringbuffer_shift_once_contiguous( _word_ringbuf_t* wrb, const size_t startpos, const int wordpositions, bool swap_left_not_right );
bool _onepointer_ringbuffer_shift_by_one_contiguous( _word_ringbuf_t* wrb, const size_t startpos, const size_t _offset_one_side, const bool swap_left_not_right );
bool _onepointer_ringbuffer_shift_contiguous( _word_ringbuf_t* wrb, const size_t startpos, const size_t _offset_one_side, const int wordpositions );
bool _onepointer_ringbuffer_swap_contiguous( _word_ringbuf_t* wrb, const size_t startpos, const int _offset );

bool onepointer_ringbuffer_shift_words( _word_ringbuf_t* wrb, const size_t startpos, const int shift_width );
bool onepointer_ringbuffer_shift_neighbours( _word_ringbuf_t* wrb, const size_t startpos, const size_t neighbour_count, const int shift_width );
bool onepointer_ringbuffer_swap_neighbours( _word_ringbuf_t* wrb, const size_t startpos, const size_t neighbour_count, const bool inverting
                                                    , const bool symmetric, const bool invert_on_each_side
);
bool onepointer_ringbuffer_swap_sides( _word_ringbuf_t* wrb, const size_t startpos, const size_t _offset_one_side, const bool inverting );
bool onepointer_ringbuffer_swap_symetric( _word_ringbuf_t* wrb, const size_t startpos, const bool neighbour_count, const bool inverting );
bool onepointer_ringbuffer_swap_asymetric( _word_ringbuf_t* wrb, const size_t startpos, const bool neighbour_count, const bool starting_outside );
bool onepointer_ringbuffer_swap_symetric( _word_ringbuf_t* wrb, const size_t startpos, const bool neighbour_count, const size_t shift_width
                                        , const bool starting_outside
);
bool onepointer_ringbuffer_rotate_symetrically( _word_ringbuf_t* wrb, const size_t startpos, const bool neighbour_count );
bool onepointer_ringbuffer_rotate_symetrically( _word_ringbuf_t* wrb, const size_t startpos, const bool neighbour_count, const bool starting_outside );
bool onepointer_ringbuffer_shift_once( _word_ringbuf_t* wrb, const size_t startpos, const bool symetrical
                                        , const enum RINGBUFFER_SHIFTING_ALGORITHMUS algo
);
bool onepointer_ringbuffer_shift( _word_ringbuf_t* wrb, const size_t startpos, const enum RINGBUFFER_SHIFTING_ALGORITHMUS* algorithms );

#ifdef __cplusplus
}
#endif

#endif // __RINGBUFFER_H_