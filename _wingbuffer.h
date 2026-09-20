
#ifndef __WINGBUFFER_H_
#define __WINGBUFFER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "_word.h"
#include "_pyramidbuffer.h"
#include "_ringbuffer.h"


typedef enum WINGBUF_ALGORITHM {
    TKIP
};


typedef struct _onepointer_wingside {
    _word_pyramidbuf_t* wing;
    _word_ringbuf_t** rings;
    _pyramidreference_t** cipher_streams;
    _pyramidreference_t** references;
    size_t wordlen;
    size_t num_rings;
    size_t num_references;
} _word_wingside_t;

_word_wingside_t* _onepointer_wingside_init( const size_t num_rows, const size_t num_width_floor
                                           , const size_t num_rings, const size_t wordlength, const size_t num_references );



#define ONEPOINTER_WING_NUM_WINGSIDES_DEFAULT 2

typedef struct _onepointer_wing {
    _word_wingside_t** side;
    size_t num_wingsides;

     // Cipher information
    _pyramidring_t** outter_rings;
    size_t num_outter_rings;

    // Buffers
    _word_ringbuf_t** ringbuf;
    size_t ringbuf_idx;
    size_t ringbuf_size;
    word_t** wordbuf;
    size_t wordbuf_size;
    size_t wordbuf_idx;
} _word_wing_t;

_word_wing_t* _onepointer_wordwing_init( const size_t num_rows, const size_t num_width_floor
                                       , const size_t num_rings, const size_t wordlength, const size_t num_references 
                                       , const size_t num_outter_rings, const size_t num_tmp_bufs
);


#define ONEPOINTER_WINGBUF_NUM_WINGS_DEFAULT 2
#define ONEPOINTER_WINGBUF_NUM_RINGS_DEFAULT 3

typedef struct _onepointer_wingbuffer {
    _word_wing_t** wings;
    _word_ringbuf_t*** rings;
    size_t wordlen;
    size_t num_wings;
    size_t num_rings;

    // Cipher information
    _pyramidring_t** inner_rings;
    size_t num_inner_rings;
    _pyramidring_t** inner_rings_cipher_opened;
    size_t num_inner_cipher_rings;

    // Buffers
    _word_ringbuf_t** ringbuf;
    size_t ringbuf_size;
    word_t** wordbuf;
    size_t wordbuf_size;

    // Flags
    bool symmetric;
    bool shifted;

    enum WINGBUF_ALGORITHM shifting_algo;
} _word_wingbuf_t;

_word_wingbuf_t* _onepointer_wingbuf_init( const size_t num_rows, const size_t num_width_floor
                                         , const size_t num_rings, const size_t wordlength, const size_t num_references 
                                         , const size_t num_outter_rings, const size_t num_inner_rings, const size_t num_tmp_bufs
);


typedef struct _onepointer_wingpage {
    _word_wing_t* page[2];
} _word_wingpage_t;

_word_wingpage_t* _onepointer_wingpage_init( const size_t num_rows, const size_t num_width_floor
                                           , const size_t num_rings, const size_t wordlength, const size_t num_references 
                                           , const size_t num_outter_rings, const size_t num_tmp_bufs
);


typedef struct _onepointer_wingbook {
    _word_wingpage_t** pages;
    size_t num_pages;
} _word_wingbook_t;

_word_wingbook_t _onepointer_wingbook_init( const size_t num_pages
                                          , const size_t num_rows, const size_t num_width_floor
                                          , const size_t num_rings, const size_t wordlength, const size_t num_references 
                                          , const size_t num_outter_rings, const size_t num_tmp_bufs
);


#ifdef __cplusplus
}
#endif

#endif // __WINGBUFFER_H_