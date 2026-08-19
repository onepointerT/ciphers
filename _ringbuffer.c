
#include "_ringbuffer.h"

#include <libm.h>

#include <stdlib.h>


_ringbuffer_shiftalgo_t* _onepointer_ringbuffer_shiftalgorithms_array_init( const size_t algorithm_count ) {
    return (_ringbuffer_shiftalgo_t*) malloc(sizeof(_ringbuffer_shiftalgo_t));
}

_word_ringbuf_t* _onepointer_ringbuffer_init( const size_t ringsize, const size_t wordlen ) {
    _word_ringbuf_t* wrb = (_word_ringbuf_t*) malloc(sizeof(_word_ringbuf_t));
    
    wrb->words = (word_t**) malloc( sizeof(_onpointer_charbuf_init(wordlen))*(ringsize/wordlen) );
    wrb->ringsize = ringsize;
    wrb->wordlen = wordlen;
    wrb->shifted = false;
    wrb->shift_count = 0;

    return wrb;
}

void _onepointer_ringbuffer_copy_word( _word_ringbuf_t* wrb, word_t* word, const size_t into_wordof_ring ) {
    if ( into_wordof_ring >= fmodl(wrb->ringsize, wrb->wordlen) ) return;
    wrb->words[into_wordof_ring] = _onepointer_merge_word( wrb->words[into_wordof_ring], word, NULL );
}

void _onepointer_ringbuffer_copy_words_to_wordarray( const _word_ringbuf_t* wrb, _word_array_t* wa, const size_t startpos, const size_t _offset ) {
    if ( wa == NULL || wrb == NULL ) return; else if ( sizeof(wa->words) / sizeof(word_t*) < _offset ) return;
    const size_t word_idx_offset = _onepointer_ringbuffer_wordindex_contiguous( wrb, startpos, _offset );
    size_t offset_still = word_idx_offset;
    while ( offset_still > 0 ) {
        word_t* word_now = NULL;
        _onepointer_ringbuffer_get_contiguous( wrb, word_now, startpos, offset_still );
        _onepointer_word_array_writeto( wa, offset_still, wrb->words[startpos+offset_still]->buf );

        offset_still = _onepointer_ringbuffer_wordindex_contiguous( wrb, offset_still, -1 );
    }
}

void _onepointer_ringbuffer_copy_words_from_wordarray( _word_ringbuf_t* wrb, const _word_array_t* wa, const size_t startpos_wa_Src
                                            , const size_t startpos_dest, const size_t _N_words, const int _offset_ringbuf
) {
    if ( wa == NULL || wrb == NULL ) return;

    for ( size_t wa_pos = startpos_wa_Src; wa_pos < sizeof(wa->words)/sizeof(word_t) && wa_pos < startpos_wa_Src + _N_words; wa_pos++ ) {
        word_t* w = wa->words[wa_pos];
        size_t wrb_idx = _onepointer_ringbuffer_wordindex_contiguous( wrb, startpos_dest, _offset_ringbuf );
        size_t num_words = 0;
        while ( num_words < _N_words ) {
            _onepointer_charbuf_cpy( wrb->words[wrb_idx], w->buf, 0, 0 );
            wrb_idx = _onepointer_ringbuffer_wordindex_contiguous( wrb, wrb_idx, 1 );

            ++num_words;
        }
    }
}

void _onepointer_ringbuffer_insert_word( _word_ringbuf_t* wrb, word_t* word, const size_t into_wordof_ring
                                       , word_t* (*cb_insertion_algorithm)(word_t*, word_t*)
) {
    if ( into_wordof_ring >= fmodl(wrb->ringsize, wrb->wordlen) ) return;
    wrb->words[into_wordof_ring] = cb_insertion_algorithm( wrb->words[into_wordof_ring], word );
}

const size_t _onepointer_ringbuffer_wordindex_contiguous( const _word_ringbuf_t* wrb, const size_t startpos, const int _offset ) {
    const size_t count_words = fmodl(wrb->ringsize, wrb->wordlen);
    size_t pos_offset = startpos;
    int _offset_still = _offset;
    while ( _offset_still >= count_words || _offset_still < (-1)*count_words ) {
        if ( _offset_still < (-1)*count_words ) {
            pos_offset += count_words;
            --_offset_still;
        } else if ( _offset_still >= count_words ) {
            pos_offset -= count_words;
            ++_offset_still;
        } else break;
    }
    return startpos + (_offset_still);
}

void _onepointer_ringbuffer_get_contiguous( _word_ringbuf_t* wrb, word_t* wptr, const size_t startpos, const int _offset ) {
    const size_t pos_offset = _onepointer_ringbuffer_wordindex_contiguous( wrb, startpos, _offset );
    if ( pos_offset >= fmodl(wrb->ringsize, wrb->wordlen) || startpos >= fmodl(wrb->ringsize, wrb->wordlen) ) { wptr = NULL; return; }
    else wptr = wrb->words[pos_offset];
}

bool _onepointer_ringbuffer_shift_at_ring_contiguous( _word_ringbuf_t* wrb, const size_t startpos, const size_t _offset_one_site, const int wordpositions, const bool swap_left_not_right ) {
    const size_t word_count_total = fmodl(wrb->ringsize, wrb->wordlen);
    if ( startpos >= word_count_total ) return false;
    const size_t word_idx_left = _onepointer_ringbuffer_wordindex_contiguous( wrb, startpos, - _offset_one_site );
    const size_t word_idx_right = _onepointer_ringbuffer_wordindex_contiguous( wrb, startpos, _offset_one_site );

    word_t* tmp = NULL;
    int wordpositions_still = wordpositions;
    while ( wordpositions_still > 0 ) {
        _onepointer_ringbuffer_get_contiguous( wrb, tmp, swap_left_not_right ? word_idx_left : word_idx_right, 0 );
        for ( unsigned int w = swap_left_not_right ? word_idx_left + 1 : word_idx_right - 1
            ; w != word_idx_left && w != word_idx_right
            ; swap_left_not_right ? ++w : --w
        ) {
            if ( w == startpos ) continue;
            
            word_t* tmp_shift = NULL;
            _onepointer_ringbuffer_get_contiguous( wrb, tmp_shift, w, 0 );
            const size_t word_idx_insert = _onepointer_ringbuffer_wordindex_contiguous( wrb, w, swap_left_not_right ? -1 : 1 );
            wrb->words[word_idx_insert] = tmp_shift;
        }
        wrb->words[swap_left_not_right ? word_idx_right : word_idx_left] = tmp;
        --wordpositions_still;
    }

    return true;
}

bool _onepointer_ringbuffer_shift_once_contiguous( _word_ringbuf_t* wrb, const size_t startpos, const int wordpositions, bool swap_left_not_right ) {
    const size_t word_idx_left = _onepointer_ringbuffer_wordindex_contiguous( wrb, startpos, - wordpositions );
    const size_t word_idx_right = _onepointer_ringbuffer_wordindex_contiguous( wrb, startpos, wordpositions );

    word_t* tmp = NULL;
    const size_t tmp_idx = swap_left_not_right ? word_idx_left : word_idx_right;
    _onepointer_ringbuffer_get_contiguous( wrb, tmp, startpos, swap_left_not_right ? - wordpositions : wordpositions );
    _onepointer_ringbuffer_shift_one_contiguous( wrb, startpos, (wordpositions/2) - 1 );
    wrb->words[ swap_left_not_right ? word_idx_right : word_idx_left ] = tmp;

    return true;
}

bool _onepointer_ringbuffer_shift_by_one_contiguous( _word_ringbuf_t* wrb, const size_t startpos, const size_t _offset_one_side, const bool swap_left_not_right ) {
    return _onepointer_ringbuffer_shift_at_ring_contiguous( wrb, startpos, _offset_one_side, 1, swap_left_not_right );
}

bool _onepointer_ringbuffer_shift_contiguous( _word_ringbuf_t* wrb, const size_t startpos, const size_t _offset_one_side, const int wordpositions ) {
    
    word_t* tmp = NULL;
    unsigned int wordpositions_still = wordpositions;
    while ( wordpositions_still > 0 ) {
        for ( unsigned int w = wordpositions < 0 
                            ? _onepointer_ringbuffer_wordindex_contiguous(wrb, startpos, -1)
                            : _onepointer_ringbuffer_wordindex_contiguous(wrb, startpos, 1)
            ; w > startpos && w < startpos
            ; w = _onepointer_ringbuffer_wordindex_contiguous(wrb, w, wordpositions < 0 ? - 1 : 1)
        ) {
            const size_t word_idx_left = _onepointer_ringbuffer_wordindex_contiguous( wrb, startpos, - _offset_one_side );
            const size_t word_idx_right = _onepointer_ringbuffer_wordindex_contiguous( wrb, startpos, _offset_one_side );

            tmp = wrb->words[w];
            for ( unsigned int wi = _onepointer_ringbuffer_wordindex_contiguous(wrb, wordpositions < 0 ? w - 1 : w + 1
                                                                                 , wordpositions < 0 ? -1 : 1 )
                ; 
                ; wi = _onepointer_ringbuffer_wordindex_contiguous(wrb, wi, wordpositions < 0 ? -1 : 1)
            ) {
                if ( wi == startpos ) { if ( wordpositions < 0 ) --wi; else ++wi; }
                wrb->words[wi] = tmp;
            }
            size_t wi_tmp = _onepointer_ringbuffer_wordindex_contiguous(wrb, w, wordpositions < 0 ? 1 : -1);
            if ( wi_tmp == startpos ) { if ( wordpositions < 0 ) ++wi_tmp; else --wi_tmp; }
            wrb->words[w] = wrb->words[wi_tmp];
        }
        --wordpositions_still;
    }
    wrb->words[_onepointer_ringbuffer_wordindex_contiguous(wrb, startpos, wordpositions < 0 ? 1 : -1)] = tmp;

    return true;
}


bool _onepointer_ringbuffer_swap_contiguous( _word_ringbuf_t* wrb, const size_t startpos, const int _offset ) {
    word_t* tmp = NULL;
    _onepointer_ringbuffer_get_contiguous( wrb, tmp, startpos, 0 );

    const size_t idx_startpos = _onepointer_ringbuffer_wordindex_contiguous( wrb, startpos, 0 );
    const size_t idx_offset = _onepointer_ringbuffer_wordindex_contiguous( wrb, startpos, _offset );

    wrb->words[idx_startpos] = wrb->words[idx_offset];
    wrb->words[idx_offset] = tmp;

    return true;
}


bool onepointer_ringbuffer_shift_words( _word_ringbuf_t* wrb, const size_t startpos, const int shift_width, const bool shift_left_not_right ) {
    return _onepointer_ringbuffer_shift_by_one_contiguous(wrb, startpos, shift_width/2, shift_left_not_right);
}

bool onepointer_ringbuffer_shift_neighbours( _word_ringbuf_t* wrb, const size_t startpos, const size_t neighbour_count, const int shift_width ) {
    return _onepointer_ringbuffer_shift_once_contiguous(wrb, startpos, neighbour_count, shift_width);
}

bool onepointer_ringbuffer_swap_neighbours( _word_ringbuf_t* wrb, const size_t startpos, const size_t neighbour_count
                                          , const bool inverting, const bool symmetric, const bool invert_on_each_side
) {
    if ( neighbour_count < 2 ) return false;
    const unsigned int neighbours_one_side = neighbour_count / 2;

    if ( invert_on_each_side ) if ( ! onepointer_ringbuffer_swap_sides( wrb, startpos, neighbours_one_side, true ) ) return false;

    const size_t word_idx_left = _onepointer_ringbuffer_wordindex_contiguous( wrb, startpos, - neighbours_one_side );
    const size_t word_idx_right = _onepointer_ringbuffer_wordindex_contiguous( wrb, startpos, neighbours_one_side );

    word_t* tmp = NULL;
    unsigned short steps = 4;
    while ( steps > 2 ) {
        unsigned int unswapped_neighbours = neighbours_one_side;

        const size_t word_idx_middle = _onepointer_ringbuffer_wordindex_contiguous( wrb, startpos
                                            , steps == 3 ? - fabs(neighbours_one_side/2) : fabs(neighbours_one_side/2)
        );

        if ( symmetric ) onepointer_ringbuffer_swap_symetric( wrb, word_idx_middle, unswapped_neighbours - 1, invert_on_each_side );
        else onepointer_ringbuffer_swap_asymetric( wrb, word_idx_middle, unswapped_neighbours - 1, true );

        if ( ! onepointer_ringbuffer_swap_neighbours( wrb, word_idx_middle
                                , steps == 3 ? - unswapped_neighbours + 2 : unswapped_neighbours - 2
                                , inverting, symmetric, invert_on_each_side
                    ) ) return false;

        --steps;
    }

    if ( steps > 2 ) return false;
    if ( inverting ) {
        if ( onepointer_ringbuffer_swap_sides( wrb, startpos, neighbours_one_side, true ) ) {
            if ( invert_on_each_side ) return onepointer_ringbuffer_swap_sides( wrb, startpos, neighbours_one_side, true );
            return true;
        } else return false;
    }

    return true;
}

_word_ringbuf_t* onepointer_ringbuffer_swap_sides( _word_ringbuf_t* wrb, const size_t startpos, const size_t _offset_one_side, const bool symetrically_inverting_sides ) {
    _word_array_t* site_one = _onepointer_word_array_init( _offset_one_side, wrb->wordlen );
    _word_array_t* site_two = _onepointer_word_array_init( _offset_one_side, wrb->wordlen );
    word_t* w_start_tmp = wrb->words[_onepointer_ringbuffer_wordindex_contiguous(wrb, startpos, 0)];

    // Copy words to wordbuffer
    for ( size_t w = 1; w < _offset_one_side; w++ ) {
        site_one->words[w-1] = wrb->words[_onepointer_ringbuffer_wordindex_contiguous(wrb, startpos, w)];
        site_two->words[w-1] = wrb->words[_onepointer_ringbuffer_wordindex_contiguous(wrb, startpos, -w)];
    }

    _word_ringbuf_t* rb_result = wrb;
    if ( wrb->ringsize < 2*_offset_one_side ) rb_result = _onepointer_ringbuffer_init( (2*wrb->ringsize)+1, wrb->wordlen );
    rb_result->words[_offset_one_side-1] = w_start_tmp;

    for ( size_t w = 0; w < _offset_one_side; w++ ) {
        if ( symetrically_inverting_sides ) {
            rb_result->words[_onepointer_ringbuffer_wordindex_contiguous(rb_result, 0, w)] = site_two->words[w];
            rb_result->words[_onepointer_ringbuffer_wordindex_contiguous(rb_result, 0, -w)] = site_one->words[w];
        } else {
            rb_result->words[_onepointer_ringbuffer_wordindex_contiguous(rb_result, 0, w)] = site_two->words[w];
            rb_result->words[_onepointer_ringbuffer_wordindex_contiguous(rb_result, 0, -w)] = site_one->words[_offset_one_side-1-w];
        }
    }

    return rb_result;
}

_word_ringbuf_t* onepointer_ringbuffer_swap_symetric( _word_ringbuf_t* wrb, const size_t startpos, const size_t _offset_one_side ) {
    return onepointer_ringbuffer_swap_sides( wrb, startpos, _offset_one_side, true );
}

_word_ringbuf_t* onepointer_ringbuffer_swap_asymetric( _word_ringbuf_t* wrb, const size_t startpos, const size_t _offset_one_side ) {
    return onepointer_ringbuffer_swap_sides( wrb, startpos, _offset_one_side, false );
}

bool onepointer_ringbuffer_rotate_symetrically( _word_ringbuf_t* wrb, const size_t startpos ) {
    size_t wrb_idx = _onepointer_ringbuffer_wordindex_contiguous( wrb, startpos, 0 );
    size_t words_shifted = 0;
    while ( words_shifted != wrb->ringsize ) {
        _onepointer_ringbuffer_shift_by_one_contiguous( wrb, wrb_idx, wrb->ringsize, false );

        wrb_idx = _onepointer_ringbuffer_wordindex_contiguous( wrb, wrb_idx, 1 );
        ++words_shifted;
    }
    return true;
}

bool onepointer_ringbuffer_rotate_asymetrically( _word_ringbuf_t* wrb, const size_t startpos, const bool starting_outside ) {
    return _onepointer_ringbuffer_shift_at_ring_contiguous( wrb, startpos, wrb->ringsize / 2, wrb->ringsize, true );
}


bool onepointer_ringbuffer_shift_once( _word_ringbuf_t* wrb, const size_t startpos
                                        , const enum RINGBUFFER_SHIFTING_ALGORITHMUS algo
) {
    switch (algo)
    {
    case SHIFT_WORDS:
        return onepointer_ringbuffer_shift_words( wrb, startpos, (size_t) fabsl(wrb->ringsize / 2), true );

    case SHIFT_NEIGHBOURS:
        return onepointer_ringbuffer_shift_neighbours( wrb, startpos, (size_t) fabsl(wrb->ringsize / 3)
                                                                    , (size_t) fabsl(wrb->ringsize / 5)
                        );

    case SWAP_NEIGHBOURS:
        return onepointer_ringbuffer_swap_neighbours( wrb, startpos, (size_t) (wrb->ringsize / 4), true, true, false );

    case SWAP_SYMETRIC:
        return onepointer_ringbuffer_swap_symetric( wrb, startpos, (size_t) fabsl((wrb->ringsize-1)/2) );

    case SWAP_ASYMETRIC:
        return onepointer_ringbuffer_swap_asymetric( wrb, startpos, (size_t) fabsl((wrb->ringsize-1)/2) );

    case SWAP_ASYMETRIC_AND_SHIFT:
        return onepointer_ringbuffer_swap_asymetric( wrb, startpos, (size_t) fabsl((wrb->ringsize-1)/2) )
            && _onepointer_ringbuffer_shift_by_one_contiguous( wrb, startpos, (size_t) fabsl((wrb->ringsize-1)/4), true );

    case ROTATE_SYMETRICALLY:
        return onepointer_ringbuffer_rotate_symetrically( wrb, startpos )
            && onepointer_ringbuffer_rotate_symetrically( wrb, startpos )
            && onepointer_ringbuffer_rotate_symetrically( wrb, startpos );

    case ROTATE_ASYMETRICALLY:
        return onepointer_ringbuffer_rotate_asymetrically( wrb, startpos, false )
            && onepointer_ringbuffer_rotate_asymetrically( wrb, startpos, false );

    default:
        break;
    }
}


bool onepointer_ringbuffer_shift( _word_ringbuf_t* wrb, const size_t startpos, const enum RINGBUFFER_SHIFTING_ALGORITHMUS* algorithms ) {
    for ( size_t a = 0; a < sizeof(algorithms)/sizeof(enum RINGBUFFER_SHIFTING_ALGORITHMUS); a++ ) {
        const enum RINGBUFFER_SHIFTING_ALGORITHMUS rbsa = algorithms[a];
        if ( ! onepointer_ringbuffer_shift_once( wrb, startpos, rbsa ) ) return false;
    }
    return true;
}
