
#include "_blockbuf.h"
#include "_hash.h"
#include "_pyramidbuffer.h"
#include "_ringbuffer.h"
#include "_wingbuffer.h"

#include <stdlib.h>


hash_t* _onepointer_hash_init( const unsigned int lenght_of_hash_bits ) {
    hash_t* hash = (hash_t*) malloc(sizeof(hash_t));

    hash->hash = (char*) malloc(sizeof(char)*lenght_of_hash_bits);
    hash->parts = 0;

    return hash;
}


hash_t* _onepointer_hash_init_parts( const unsigned int lenght_of_hash_bits, void* parts_struct ) {
    hash_t* hash = _onepointer_hash_init( lenght_of_hash_bits );

    hash->parts = parts_struct;

    return hash;
}


_charbuf_t* _onpointer_charbuf_init( const size_t bufsize ) {

    struct _onepointer_charbuf* cbuf = (struct _onepointer_charbuf*) malloc(sizeof(struct _onepointer_charbuf));

    cbuf->buf = (char*) malloc(sizeof(char)*bufsize);
    cbuf->bufsize = bufsize;

    return cbuf;
}


_blockbuf_t* _onepointer_blockbuf_init( const unsigned int buffercount, const size_t buffersize_each ) {

    struct _onepointer_blockbuf* blkbuf = (struct _onepointer_blockbuf*) malloc(sizeof(struct _onepointer_blockbuf));

    blkbuf->buffercount = buffercount;
    blkbuf->bufferindex = 0;

    *blkbuf->buffers = (struct _onepointer_charbuf*) malloc(sizeof(struct _onepointer_charbuf)*buffercount);
    for ( unsigned int bci = 0; bci < buffercount; bci++ ) {
        blkbuf->buffers[bci] = _onpointer_charbuf_init( buffersize_each );
    }

    blkbuf->buf = blkbuf->buffers[0];

    return blkbuf;
}

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


_pyramidring_t* _onepointer_pyramidring_init( const size_t words, const size_t wordlength
                                            , const size_t vertical_index, const size_t horizontal_index
) {
    _pyramidring_t* pyr = (_pyramidring_t*) malloc(sizeof(_pyramidring_t));

    pyr->ringbuf = _onepointer_ringbuffer_init( words, wordlength );
    pyr->v_idx = vertical_index;
    pyr->h_idx = horizontal_index;
    pyr->leftof_nor_rightof = true;
    pyr->topof_nor_lowerof = false;
    pyr->put_where_if = NULL;
    pyr->put_original_where_if = NULL;

    return pyr;
}


_pyramidword_t* _onepointer_pyramidword_init( const size_t wordlength, const size_t words_in_ring
                                            , const size_t wordlength_in_ring, const size_t size_referencer_array
) {
    _pyramidword_t* pyw = (_pyramidword_t*) malloc(sizeof(_pyramidword_t));

    pyw->word = _onpointer_charbuf_init( wordlength );
    if ( wordlength_in_ring == 0 || words_in_ring == 0 ) pyw->ring = NULL;
    else pyw->ring = _onepointer_pyramidring_init( words_in_ring, wordlength_in_ring, 0, 0 );
    if ( size_referencer_array == 0 ) pyw->ref_out = NULL;
    else {
        pyw->ref_out = (struct _onepointer_pyramidreference**) malloc(sizeof(struct _onepointer_pyramidreference*)*size_referencer_array);
        for ( size_t r = 0; r < size_referencer_array; r++ ) { pyw->ref_out[r] = NULL; }
    }

    return pyw;
}



_pyramidreference_t* _onepointer_pyramidreference_init( const size_t wordlength, const enum PYRAMID_REFERENCE_TYPE ref_type ) {
    _pyramidreference_t* pyrf = (_pyramidreference_t*) malloc(sizeof(_pyramidreference_t));

    if ( wordlength == 0 ) {
        pyrf->origin = NULL;
        pyrf->dest = NULL;
    } else {
        pyrf->origin = _onepointer_pyramidword_init( wordlength, 0, 0, 0 );
        pyrf->dest = (_pyramidreference_t**) malloc(sizeof(_pyramidreference_t*));
    }
    pyrf->pass = NULL;
    pyrf->type = ref_type;

    return pyrf;
}



_word_pyramidbuf_t* _onepointer_pyramidbuffer_init( const enum PYRAMID_SHAPE pys, const size_t wordlength
                                        , const size_t height_rows, const size_t words_floor_row
                                        , const size_t words_in_ring
                                        , const ascii_t shape_or_symetric_with_NULL

) {
    _word_pyramidbuf_t* pyb = (_word_pyramidbuf_t*) malloc(sizeof(_word_pyramidbuf_t));

    bool from_ascii = shape_or_symetric_with_NULL != NULL;
    pyb->words = from_ascii ? NULL : (_pyramidword_t***) malloc(sizeof(_pyramidword_t**)*height_rows);
    _onepointer_init_pyramidwords_or_paint( pyb->words, pys, height_rows, words_floor_row, shape_or_symetric_with_NULL );
    pyb->wordlength = wordlength;
    pyb->num_words_floor = words_floor_row;
    pyb->num_words_uppside = pys == PYRAMID_WITH_STRAIGHT_ROOF ? 0 : 1;
    pyb->num_rows = height_rows;
    pyb->gap_size_uppside = pys == PYRAMID_WITH_STRAIGHT_ROOF
                        || pys != PYRAMID_WITH_GAPS ? 0 : 1;
    pyb->if_ascii = shape_or_symetric_with_NULL;
    pyb->symetric = pyb->if_ascii == NULL;
    pyb->inheriting = false;
    pyb->shifted = false;
    pyb->pshape = pys;
    pyb->wordbuf = (word_t**) malloc(sizeof(word_t*)*ONEPOINTER_PYRAMIDBUFFER_SIZE_TMPBUFFERS);
    pyb->wordbuf_size = ONEPOINTER_PYRAMIDBUFFER_SIZE_TMPBUFFERS;
    pyb->ringbuf = (_word_ringbuf_t**) malloc(sizeof(_word_ringbuf_t*)*ONEPOINTER_PYRAMIDBUFFER_SIZE_TMPBUFFERS);
    pyb->ringbuf_size = ONEPOINTER_PYRAMIDBUFFER_SIZE_TMPBUFFERS;
    pyb->inheritors = NULL;
    pyb->cipher_streams = NULL;
    pyb->reference_wavers = NULL;
    pyb->graph_start = NULL;
    pyb->route = NULL;
    pyb->shifting_algo = NONE;
    pyb->ring_algo = NONE;

    return pyb;
}



_word_wingside_t* _onepointer_wingside_init( const size_t num_rows, const size_t num_width_floor
                                           , const size_t num_rings, const size_t wordlength, const size_t num_references
) {
    _word_wingside_t* wws = (_word_wingside_t*) malloc(sizeof(_word_wingside_t));

    wws->num_rings = num_rings;
    wws->wordlen = wordlength;
    wws->wing = _onepointer_pyramidbuffer_init( PYRAMID_WINGLY, wordlength, num_rows, num_width_floor, 8, NULL );
    wws->cipher_streams = (_pyramidreference_t**) malloc(sizeof(_pyramidreference_t*)*num_references);
    wws->references = (_pyramidreference_t**) malloc(sizeof(_pyramidreference_t*)*num_references);
    wws->num_references = num_references;
    wws->rings = (_word_ringbuf_t**) malloc(sizeof(_word_ringbuf_t*)*num_rings);
    for ( size_t r = 0; r < num_rings; r++ ) {

    }

    return wws;
}


_word_wing_t* _onepointer_wordwing_init( const size_t num_rows, const size_t num_width_floor
                                       , const size_t num_rings, const size_t wordlength, const size_t num_references 
                                       , const size_t num_outter_rings, const size_t num_tmp_bufs
) {
    _word_wing_t* ww = (_word_wing_t*) malloc(sizeof(_word_wing_t));

    ww->side = (_word_wingside_t**) malloc(sizeof(_word_wingside_t*)*ONEPOINTER_WING_NUM_WINGSIDES_DEFAULT);
    ww->num_wingsides = ONEPOINTER_WING_NUM_WINGSIDES_DEFAULT;
    ww->num_outter_rings = num_outter_rings;
    ww->outter_rings = (_pyramidring_t**) malloc(sizeof(_pyramidring_t*)*num_outter_rings);
    ww->ringbuf = (_word_ringbuf_t**) malloc(sizeof(_word_ringbuf_t*)*num_tmp_bufs);
    ww->ringbuf_idx = 0;
    ww->ringbuf_size = num_tmp_bufs;

}


_word_wingbuf_t* _onepointer_wingbuf_init( const size_t num_rows, const size_t num_width_floor
                                         , const size_t num_rings, const size_t wordlength, const size_t num_references 
                                         , const size_t num_outter_rings, const size_t num_inner_rings, const size_t num_tmp_bufs
) {

}


_word_wingpage_t* _onepointer_wingpage_init( const size_t num_rows, const size_t num_width_floor
                                           , const size_t num_rings, const size_t wordlength, const size_t num_references 
                                           , const size_t num_outter_rings, const size_t num_tmp_bufs
) {

}


_word_wingbook_t _onepointer_wingbook_init( const size_t num_pages
                                          , const size_t num_rows, const size_t num_width_floor
                                          , const size_t num_rings, const size_t wordlength, const size_t num_references 
                                          , const size_t num_outter_rings, const size_t num_tmp_bufs
) {

}
