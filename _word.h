
#ifndef __WORDS_H_
#define __WORDS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "_blockbuf.h"

typedef _charbuf_t word_t;
typedef char (*merge_characters_f)(char, char);

word_t* _onepointer_merge_word( word_t* w1, word_t* w2, const merge_characters_f cb_merge_positions_f );
void _onepointer_copy_word( const word_t* _Source, word_t* _Dest );
size_t _onepointer_word_index_contiguous( word_t* w, const size_t startpos, const int _offset, const bool startpos_successive );
size_t _onepointer_word_index_count_contiguous( word_t* w, const size_t startpos, const size_t endpos );
char _onepointer_word_getpos_contiguous( word_t* w, const size_t startpos, const int _offset );
bool _onepointer_word_shift_word( word_t* w, const bool leftwise_not_rightwise, const size_t startpos, const int _offset, const bool startpos_untouched, const bool asymetrically );
void _onepointer_word_swap_positions_at( word_t* w, const size_t pos1, const size_t pos2 );
void _onepointer_word_swap_position_at( word_t* w, const size_t pos1, const bool startpos_successive, const bool start_left_not_right );
void _onepointer_word_swap_positions_asymetrically_at( word_t* w, const size_t pos1, const size_t pos2, const bool leftwise_not_rightwise_shift, const bool swap_current_positions_symetrically_too );
bool _onepointer_word_swap_positions( word_t* w, const size_t startpos, const int _offset, const bool startpos_successive );
bool _onepointer_word_swap_3shift( word_t* w, const size_t startpos, const int _offset, const int _shifting_offset, const bool startpos_successive );
bool _onepointer_word_swap_3( word_t* w, const size_t startpos, const int _offset, const bool clap_sides, const bool shift_sides_if_clap_true );
bool _onepointer_word_swap_n( word_t* w, const size_t startpos, const size_t _shift_width, const int _offset, const bool swap_only
                        , const bool clap_sides, const bool shift_sides_if_clap_true );
bool _onepointer_word_swap_sides( word_t* w, const size_t startpos, const int _offset, const bool clap_sides, const bool shift_sides_if_clap_true );
bool _onepointer_word_shift_zig( word_t* w, const size_t startpos, const int _offset, const bool both_sides );
bool _onepointer_word_swap_swipping( word_t* w, const size_t startpos, const size_t swipping_width, const int _offset
                            , const bool startpos_successive, const bool shift_then_swap_nor_swap_then_shift
);
bool _onepointer_word_invert_positions( word_t* w, const size_t startpos_middle, const int _offset_width_one_side, const bool startpos_successive );

typedef struct _onepointer_word_array {
    word_t** words;
    word_t* word;
    unsigned short words_size;
    unsigned short words_index;
} _word_array_t;

_word_array_t* _onepointer_word_array_init( const unsigned short number_words, const unsigned short wordsize );
bool _onepointer_word_array_switch( _word_array_t* word_array, const unsigned short wordnum );
char* _onepointer_word_array( _word_array_t* word_array, const unsigned short wordnum );
char* _onepointer_word_array_complete( _word_array_t* word_array );
char* _onepointer_word_array_word( _word_array_t* word_array );
void _onepointer_word_array_writeto( _word_array_t* word_array, const unsigned short wordnum, const char* _Source );


typedef struct _onepointer_word_table {
    _word_array_t** word_lanes;
    _word_array_t* word_lane;
    unsigned short word_lanes_size;
    unsigned short word_lanes_index;
    unsigned short words_size;
} _word_table_t;

_word_table_t* _onepointer_word_table_init( const unsigned short number_words, const unsigned short wordsize, const unsigned short number_word_lanes );
bool _onepointer_word_table_switch( _word_table_t* word_table, const unsigned short wordlanenum );
bool _onepointer_word_table_switch2( _word_table_t* word_table, const unsigned short wordlanenum, const unsigned short wordnum );
char* _onepointer_word_table( _word_table_t* word_table, const unsigned short wordlanenum, const unsigned short wordnum );
char* _onepointer_word_table_lane( _word_table_t* word_table, const unsigned short wordlanenum );
char* _onepointer_word_table_complete( _word_table_t* word_table );
void _onepointer_word_table_writeto( _word_table_t* word_table, const unsigned short wordlanenum, const unsigned short wordnum, const char* _Source );


unsigned short _getpos_laneshifting_bitblocks( const unsigned short bitblocks, const unsigned short shifting_modificator, const unsigned short start_pos, const bool shift_direction_left );
unsigned short _getpos_laneshifting( const unsigned short wordlength, const unsigned short shifting_modificator, const unsigned short start_pos, const bool shift_direction_left );
word_t* _rightshift_bitblock( _word_array_t* word_lane, const unsigned short w, const unsigned short origin_w, const unsigned short shifting_modificator, const bool rightshift );
bool _leftrotate_and_rightshift_bitblocks( _word_array_t* word_lane, const unsigned short w, const unsigned short shift_positions );

#ifdef __cplusplus
}
#endif

#endif // __WORDS_H_