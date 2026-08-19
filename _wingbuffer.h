
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


typedef struct _onepointer_wingbuffer {
    _word_pyramidbuf_t** wings;
    _word_ringbuf_t** rings;
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

#ifdef __cplusplus
}
#endif

#endif // __WINGBUFFER_H_