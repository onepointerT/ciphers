
#ifndef __PYRAMIDBUFFER_H_
#define __PYRAMIDBUFFER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "_word.h"
#include "_ringbuffer.h"


typedef enum PYRAMIDBUF_ALGORITHM {
    TKIP
};

// Forward-declaration
struct _onepointer_pyramidreference;
struct _onepointer_pyramidword;


typedef struct _onepointer_pyramidring {
    _word_ringbuf_t* ringbuf;
    size_t v_idx;
    size_t h_idx;
    bool leftof_nor_rightof;
    bool topof_nor_lowerof;
    struct _onepointer_pyramidword* put_where_if;
    struct _onepointer_pyramidword* put_original_where_if;
} _pyramidring_t;


typedef struct _onepointer_pyramidword {
    word_t* word;
    _pyramidring_t* ring;
    struct _onepointer_pyramidreference** ref_out;
} _pyramidword_t;


typedef struct _onepointer_pyramidreference {
    _pyramidword_t* origin;
    _pyramidword_t* dest;
    _word_ringbuf_t* pass;
} _pyramidreference_t;

typedef struct _onepointer_pyramidbuffers {
    // Pyramid-shaped wordbuffers
    _pyramidword_t*** words;
    size_t num_words_floor;
    size_t num_words_uppside;
    size_t gap_size_uppside;

    // Flags
    bool symmetric;
    bool inheriting;
    bool shifted;

    // Buffers
    word_t** wordbuf;
    size_t wordbuf_size;
    _word_ringbuf_t** ringbuf;
    size_t ringbuf_size;

    // Cipher streaming, reference wavers and similar
    _pyramidreference_t** inheritors;
    _pyramidreference_t** cipher_streams;
    _pyramidreference_t** reference_wavers;

    // Algorithms
    enum PYRAMID_ALGORITHM shifting_algo;
    enum RINGBUFFER_SHIFTING_ALGORITHMUS ring_algo;
} _word_pyramidbuf_t;

#ifdef __cplusplus
}
#endif

#endif // __PYRAMIDBUFFER_H_