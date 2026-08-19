
#ifndef __WINGBUFFER_H_
#define __WINGBUFFER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "_word.h"


typedef enum WINGBUF_ALGORITHM {
    TKIP
};


typedef struct _onepointer_wingbuffer {
    word_t*** words;
    size_t wordlen;
    size_t num_words;
    bool symmetric;
    bool shifted;
    enum WINGBUF_ALGORITHM shifting_algo;
} _word_wingbuf_t;

#ifdef __cplusplus
}
#endif

#endif // __WINGBUFFER_H_