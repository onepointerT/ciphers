
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
    size_t wordlen;
    size_t num_rings;
} _word_wingside_t;

typedef struct _onepointer_wing {
    _word_wingside_t* side[2];

     // Cipher information
    _pyramidring_t*** outter_rings;
    size_t num_outter_rings;

    // Buffers
    _word_ringbuf_t** ringbuf;
    size_t ringbuf_size;
    word_t** wordbuf;
    size_t wordbuf_size;
} _word_wing_t;

typedef struct _onepointer_wingbuffer {
    _word_wing_t* wings[2];
    _word_ringbuf_t** rings[3];
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


typedef struct _onepointer_wingpage {
    _word_wing_t* page[2];
} _word_wingpage_t;

typedef struct _onepointer_wingbook {
    _word_wingpage_t** pages;
    size_t num_pages;
} _word_wingbook_t;

#ifdef __cplusplus
}
#endif

#endif // __WINGBUFFER_H_