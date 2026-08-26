
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

_pyramidring_t* _onepointer_pyramidring_init( const size_t words, const size_t wordlength
                                            , const size_t vertical_index, const size_t horizontal_index
);
size_t _onepointer_pyramidword_referencerarray_resize( _pyramidword_t* pyw, const int new_size_ref_out );
bool _onepointer_pyramidring_put( const _pyramidring_t* pyr );
bool _onepointer_pyramidring_put_original( const _pyramidring_t* pyr );

typedef struct _onepointer_pyramidword {
    word_t* word;
    _pyramidring_t* ring;
    struct _onepointer_pyramidreference** ref_out;
    struct _onepointer_pyramidreference** ref_in;
} _pyramidword_t;

_pyramidword_t* _onepointer_pyramidword_init( const size_t wordlength, const size_t words_in_ring
                                            , const size_t wordlength_in_ring, const size_t size_referencer_array
);
void _onepointer_pyramidword_resize_array( _pyramidword_t** ref_arr, const int additional_size );
void _onepointer_pyramidword_addword( _pyramidword_t** ref_arr, _pyramidword_t* pyr );




typedef enum PYRAMID_REFERENCE_TYPE {
    CIPHER_STREAMING,
    REFERENCE_WAVER,
    INHERITOR,
    MIXED,
    MIXED_NO_INHERITOR,
    MIXED_NO_CIPHER_STREAMS,
    MIXED_NO_REFERENCES
} _pyramid_reference_type;

bool _onepointer_pyramidreference_type_has( const _pyramidreference_t* pref, const enum PYRAMID_REFERENCE_TYPE prt );

typedef struct _onepointer_pyramidreference {
    _pyramidword_t* origin;
    _pyramidword_t** dest;
    _word_ringbuf_t* pass;
    enum PYRAMID_REFERENCE_TYPE type;
} _pyramidreference_t;

_pyramidreference_t* _onepointer_pyramidreference_init( const size_t wordlength, const enum PYRAMID_REFERENCE_TYPE ref_type );
void _onepointer_pyramidreference_resize_array( _pyramidreference_t** ref_arr, const size_t additional_size );
void _onepointer_pyramidreference_addref( _pyramidreference_t** ref_arr, _pyramidreference_t* pyr );

typedef _pyramidreference_t*** _pyramid_route_array_t;
typedef _pyramidreference_t** _pyramid_route_t;

void _onepointer_pyramidroute_resize_array( _pyramidreference_t*** ref_arr, const size_t additional_size );
void _onepointer_pyramidroute_addroute_ref( _pyramidreference_t*** ref_arr, _pyramidreference_t** pref_route );

typedef enum PYRAMID_SHAPE {
    PYRAMID_MINUS_2_PER_ROW,
    PYRAMID_MINUS_1_PER_ROW,
    PYRAMID_MINUS_2_PER_ROW_GAPS_ON_TOP,
    PYRAMID_MINUS_1_PER_ROW_GAPS_ON_TOP,
    PYRAMID_WITH_GAPS,
    PYRAMID_WITH_STRAIGHT_ROOF,
    ASCII_ONLY
} _onepointer_pyramid_shape;


typedef char* ascii_t;

#define ONEPOINTER_PYRAMIDBUFFER_SIZE_TMPBUFFERS 3


typedef enum PYRAMID_ALGORITHM {
    NONE
} _onepointer_pyramid_algorithm;


typedef struct _onepointer_pyramidbuffer {
    // Pyramid-shaped wordbuffers
    _pyramidword_t*** words;
    size_t wordlength;
    size_t num_words_floor;
    size_t num_words_uppside;
    size_t num_rows;
    size_t gap_size_uppside;

    // If ascii
    ascii_t if_ascii;

    // Flags
    bool symetric;
    bool inheriting;
    bool shifted;
    enum PYRAMID_SHAPE pshape;

    // Buffers
    word_t** wordbuf;
    size_t wordbuf_size;
    _word_ringbuf_t** ringbuf;
    size_t ringbuf_size;

    // Cipher streaming, reference wavers and similar
    _pyramidreference_t** inheritors;
    _pyramidreference_t** cipher_streams;
    _pyramidreference_t** reference_wavers;
    _pyramidreference_t* graph_start;
    _pyramidreference_t** route;

    // Algorithms
    enum PYRAMID_ALGORITHM shifting_algo;
    enum RINGBUFFER_SHIFTING_ALGORITHMUS ring_algo;
} _word_pyramidbuf_t;

_word_pyramidbuf_t* _onepointer_pyramidbuffer_init( const enum PYRAMID_SHAPE pys, const size_t wordlength
                                        , const size_t height_rows, const size_t words_floor_row
                                        , const size_t words_in_ring
                                        , const ascii_t shape_or_symetric_with_NULL

);

_pyramidword_t** _onepointer_pyramidbuffer_row( _word_pyramidbuf_t* wpyb, const size_t h_idx );
_pyramidword_t* _onepointer_pyramidbuffer_word( _word_pyramidbuf_t* wpyb, const size_t h_idx, const size_t v_idx );
_pyramidword_t* _onepointer_pyramidbuffer_word_of_row( _pyramidword_t** pyw_row, const size_t v_idx );
bool _onepointer_pyramidbuffer_reference_from_to( _word_pyramidbuf_t* wpyb, _pyramidword_t* wfrom, _pyramidword_t* wto
                                                , const enum PYRAMID_REFERENCE_TYPE prt
);

const _pyramid_route_array_t _onepointer_pyramidreference_find_incoming_pword( const _pyramidword_t* pword, const _word_pyramidbuf_t* wpyb
                                                                             , const enum PYRAMID_REFERENCE_TYPE prt
);

const _pyramid_route_array_t _onepointer_pyramidreference_find_incoming_pword_from( const _pyramidword_t* pword, const _pyramidword_t* wfrom
                                                                            , const _word_pyramidbuf_t* wpyb, const enum PYRAMID_REFERENCE_TYPE prt
);
const _pyramidreference_t*** _onepointer_pyramidreference_find_incoming( const _pyramidword_t* pword, const _word_pyramidbuf_t* wpyb
                                                                      , const _pyramidreference_t* pref_current, const _pyramidword_t* wfrom
                                                                      , const enum PYRAMID_REFERENCE_TYPE* prt
);
_pyramid_route_t _onepointer_pyramidbuffer_get_route( const _word_pyramidbuf_t* wpyb, const _pyramidword_t* wto
                                                    , const _pyramidword_t* tfrom, const enum PYRAMID_REFERENCE_TYPE prt
);

const _pyramid_route_array_t _onepointer_pyramidreference_get_routes_to( const _word_pyramidbuf_t* wpyb, const _pyramidword_t* wto
                                                                        , const _pyramidword_t* tfrom, const enum PYRAMID_REFERENCE_TYPE prt
);

const _pyramid_route_array_t _onepointer_pyramidreference_get_routes_from( const _word_pyramidbuf_t* wpyb, const _pyramidword_t* wto
                                                                        , const _pyramidword_t* tfrom, const enum PYRAMID_REFERENCE_TYPE prt
);



typedef _blockbuf_t* (*pyramid_decipher_f)( const _word_pyramidbuf_t** wps, const _word_ringbuf_t** wrs );
typedef _word_pyramidbuf_t** (*pyramid_cipher_f)( const _word_pyramidbuf_t** wps, const _word_ringbuf_t** wrs );

/*
extern void onepointer_pyramidbuffer_cipherstream( _word_pyramidbuf_t* wpyb, const pyramid_cipher_f cph_func );
extern void onepointer_pyramidbuffer_cipherstream_descrypt( _word_pyramidbuf_t* wpyb, const pyramid_decipher_f dcph_func );

extern void onepointer_pyramidbuffer_inherit_buffer_words( _word_pyramidbuf_t* wpyb, const bool desinherit );

extern _quadbuf_t* toQuadbuf( const _word_pyramidbuf_t* p1, const _word_pyramidbuf_t* p2, const _word_pyramidbuf_t* p3, const _word_pyramidbuf_t* p4 );
extern _quadbuf_t* toQuadbuf2( const _word_pyramidbuf_t* p1, const _word_pyramidbuf_t* p2 );

extern _blockbuf_t* toBlockbufDecipher( const _word_pyramidbuf_t** wps, const _word_ringbuf_t** wrs, const pyramid_decipher_f dcph_func );
extern _blockbuf_t* toBlockbufCipher( const _word_pyramidbuf_t** wps, const _word_ringbuf_t** wrs, const pyramid_cipher_f cph_func );
*/


#ifdef __cplusplus
}
#endif

#endif // __PYRAMIDBUFFER_H_