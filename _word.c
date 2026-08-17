
#include "_word.h"

#include <stdlib.h>
#include <ctype.h>

#include <libm.h>


word_t* _onepointer_merge_word( word_t* w1, word_t* w2, const merge_characters_f cb_merge_positions_f ) {
    word_t* w = NULL;
    if ( w1 == NULL && w2 != NULL ) {
        w = _onpointer_charbuf_init( w2->bufsize );
        _onepointer_charbuf_cpy_n( w, w2->buf, 0, 0, w->bufsize );
    } else if ( w2 == NULL && w1 != NULL ) {
        w = _onpointer_charbuf_init( w1->bufsize );
        _onepointer_charbuf_cpy_n( w, w1->buf, 0, 0, w->bufsize );
    } else if ( w1 != NULL && w2 != NULL ) {
        const size_t w_bufsize = w1->bufsize > w2->bufsize ? w1->bufsize : w2->bufsize;
        w = _onpointer_charbuf_init( w_bufsize );
        char c1, c2 = '';
        for ( size_t widx = 0; widx < w_bufsize; widx++ ) {
            if ( widx >= w1->bufsize ) c1 = ' ';
            else c1 = w1->buf[widx];
            if ( widx >= w2->bufsize ) c2 = ' ';
            else c2 = w2->buf[widx];
            if ( cb_merge_positions_f == NULL ) w->buf[widx] = cb_merge_positions_f(c1, c2);
            else return NULL;
        }
    } 
    return w;
}

void _onepointer_copy_word( const word_t* _Source, word_t* _Dest ) {
    if ( _Source == NULL ) return;
    if ( _Dest == NULL ) _Dest = _onpointer_charbuf_init( _Source->bufsize );
    _onepointer_charbuf_cpy( _Dest, _Source->buf, 0, 0 );
}

size_t _onepointer_word_index_contiguous( word_t* w, const size_t startpos, const int _offset, const bool startpos_successive ) {
    const int _offset_position = startpos + _offset;
    if ( _offset_position < 0 || _offset_position >= w->bufsize ) {
        int _offset_now = _offset_position;
        while ( _offset_now < 0 || _offset_now >= w->bufsize ) {
            // While we have a modulo != 0, the contingent outside of the buffer is w->buffsize
            unsigned long cont_compl_pos = (unsigned long) fmodl(_offset_position, w->bufsize);
            while ( cont_compl_pos != 0 ) {
                if ( _offset_now < - w->bufsize ) _offset_now += w->bufsize;
                else if ( _offset_now > w->bufsize ) _offset_now -= w->bufsize;
                --cont_compl_pos;
            }
        }

        // For every at this position we already have everything awkwardly calculated the buffer
        // inside the buffer's limits and donot need to retest.
        if ( startpos + _offset_now < 0 || startpos + _offset_now >= w->bufsize )
            return _onepointer_word_index_contiguous( w, startpos, _offset_now, startpos_successive );
        else if ( _offset_now == 0 && startpos_successive ) return _onepointer_word_index_contiguous( w, startpos, _offset < 0 ? -1 : 1, true );
        else return startpos + _offset_now;
    } else return _offset_position;
}

size_t _onepointer_word_index_count_contiguous( word_t* w, const size_t startpos, const size_t endpos ) {
    const size_t w_idx_startpos = _onepointer_word_index_contiguous( w, startpos, 0, false );
    const size_t w_idx_endpos = _onepointer_word_index_contiguous( w, endpos, 0, false );
    const size_t w_idx_between = _onepointer_word_index_contiguous( w, w_idx_startpos, w_idx_endpos - w_idx_startpos, false );
    return _onepointer_word_index_contiguous( w, w_idx_between, startpos + w_idx_between, false );
}

char _onepointer_word_getpos_contiguous( word_t* w, const size_t startpos, const int _offset ) {
    const size_t cidx = _onepointer_word_index_contiguous( w, startpos, _offset, false );
    return w->buf[cidx];
}

bool _onepointer_word_shift_word( word_t* w, const bool leftwise_not_rightwise, const size_t startpos, const int _offset, const bool startpos_untouched ) {

    const size_t offset_idx = _onepointer_word_index_contiguous( w, startpos, _offset, startpos_untouched );
    char tmp = ' ';
    size_t pos_idx = offset_idx;
    while ( pos_idx != offset_idx ) {
        tmp = w->buf[pos_idx];
        w->buf[pos_idx] = w->buf[_onepointer_word_index_contiguous( w, pos_idx, leftwise_not_rightwise ? 1 : -1 )];
        pos_idx = _onepointer_word_index_contiguous( w, pos_idx, leftwise_not_rightwise ? -1 : 1, startpos_untouched );
        w->buf[pos_idx] = tmp;
    }
    return true;
}

void _onepointer_word_swap_positions_at( word_t* w, const size_t pos1, const size_t pos2 ) {
    const size_t w_idx = _onepointer_word_index_contiguous(w, pos1, 0, false);
    const size_t w_idx_other_side = _onepointer_word_index_contiguous(w, pos2, 0, false);
    char tmp = w->buf[w_idx];
    w->buf[w_idx] = w->buf[w_idx_other_side];
    w->buf[w_idx_other_side] = tmp;
}

void _onepointer_word_swap_position_at( word_t* w, const size_t pos1, const bool startpos_successive, const bool start_left_not_right ) {
    const size_t w_idx_minus1 = _onepointer_word_index_contiguous(w, pos1, -1, false);
    const size_t w_idx_plus1 = _onepointer_word_index_contiguous(w, pos1, 1, false);
    
    _onepointer_word_swap_positions_at( w, ! startpos_successive ? pos1 : (start_left_not_right ? w_idx_minus1 : w_idx_plus1)
                                      , start_left_not_right ? w_idx_plus1 : w_idx_minus1 );
    if ( ! startpos_successive ) _onepointer_word_swap_positions_at( w, pos1, start_left_not_right ? w_idx_minus1 : w_idx_plus1 );
    _onepointer_word_swap_positions_at( w, ! startpos_successive ? pos1 : (start_left_not_right ? w_idx_plus1 : w_idx_minus1)
                                      , start_left_not_right ? w_idx_minus1 : w_idx_plus1 );
}

void _onepointer_word_swap_positions_asymetrically_at( word_t* w, const size_t pos1, const size_t pos2
                                                     , const bool leftwise_not_rightwise_shift, const bool swap_current_positions_symetrically_too
) {
    // Get the indices for asymetric swapping positions and swap 
    const size_t w_idx = _onepointer_word_index_contiguous( w, pos1, 0, false );
    const size_t w_idx_beneath = _onepointer_word_index_contiguous( w, pos1, 1, false );
    const size_t w_idx_other = _onepointer_word_index_contiguous( w, pos2, 0, false );
    const size_t w_idx_other_beneath = _onepointer_word_index_contiguous( w, pos2, -1, false );

    _onepointer_word_swap_positions_at( w, w_idx, w_idx_beneath );
    _onepointer_word_swap_positions_at( w, w_idx_other, w_idx_other_beneath );

    if ( swap_current_positions_symetrically_too ) _onepointer_word_swap_positions_at( w, w_idx, w_idx_other );
}

bool _onepointer_word_swap_positions( word_t* w, const size_t startpos, const int _offset, const bool startpos_successive, const bool asymetrically ) {
    
    const size_t c_idx = _onepointer_word_index_contiguous( w, startpos, _offset, startpos_successive );
    const size_t c_idx_other_side = _onepointer_word_index_contiguous( w, startpos, - _offset, startpos_successive );

    if ( _offset == 0 && startpos_successive ) {
        _onepointer_word_swap_positions_at( w, _onepointer_word_index_contiguous(w, startpos-1, 0, false)
                                             , _onepointer_word_index_contiguous(w, startpos+1, 0, false) );
    } else if ( c_idx != c_idx_other_side) {
        size_t w_len_between = _onepointer_word_index_count_contiguous( w, c_idx, c_idx_other_side );

        if ( w_len_between == 0 )
            return _onepointer_word_swap_positions( w, startpos, -1, true, asymetrically );
        while ( w_len_between != 0 ) {
            if ( asymetrically ) {
                size_t w_idx_0 = c_idx;
                while ( w_idx_0 != startpos ) {
                    _onepointer_word_swap_positions_asymetrically_at( w, w_idx_0
                                                                    , startpos_successive 
                                                                            ? _onepointer_word_index_contiguous( w, startpos, w_idx_0, false )
                                                                            : _onepointer_word_index_contiguous( w, startpos, -1 + w_idx_0, false )
                                                                    , false, true
                    );

                    w_idx_0 = _onepointer_word_index_contiguous( w, w_idx_0, 1, false );
                }
            } else {
                size_t w_idx_offset = 0;

                size_t cidx = _onepointer_word_index_contiguous(w, c_idx, w_idx_offset, false);
                while ( cidx != startpos ) {
                    size_t cidx_other_side = _onepointer_word_index_contiguous(w, c_idx_other_side, - w_idx_offset, false);
                    _onepointer_word_swap_positions_at( w, cidx, cidx_other_side );
                    ++w_idx_offset;
                    cidx = _onepointer_word_index_contiguous(w, c_idx, w_idx_offset, false);
                }
            }
            
            if ( asymetrically ) {
                _onepointer_word_swap_position_at( w, startpos, startpos_successive, true );
            }
        }

    } else if ( c_idx == c_idx_other_side ) {
        return _onepointer_word_swap_positions( w, startpos, c_idx, false, asymetrically );
    } else {
        return _onepointer_word_swap_n( w, startpos, (unsigned long) fmodl(c_idx + c_idx_other_side, 2), _offset, false, false );
    }
    return true;
}

bool _onepointer_word_swap_3shift( word_t* w, const size_t startpos, const int _offset, const int _shifting_offset, const bool startpos_successive ) {

}

bool _onepointer_word_swap_3( word_t* w, const size_t startpos, const int _offset, const bool clap_sides, const bool shift_sides_if_clap_true ) {

}

bool _onepointer_word_swap_n( word_t* w, const size_t startpos, const size_t _shift_width, const int _offset, const bool clap_sides, const bool shift_sides_if_clap_true ) {

}

bool _onepointer_word_swap_zig( word_t* w, const size_t startpos, const int _offset, const bool clap_sides, const bool shift_sides_if_clap_true ) {

}

bool _onepointer_word_swap_swipping( word_t* w, const size_t startpos, const size_t swipping_width, const bool startpos_successive ) {

}

bool _onepointer_word_invert_positions( word_t* w, const size_t startpos_middle, const int _offset_width_one_side, const bool startpos_successive ) {

}



_word_array_t* _bankest_word_array_init( const unsigned short number_words, const unsigned short wordsize ) {
    _word_array_t* wa = (_word_array_t*) malloc(sizeof(_word_array_t));

    wa->words_size = 0;

    wa->words = (word_t*) malloc(sizeof(word_t*)*number_words);
    for ( unsigned short nw = 0; nw < number_words; nw++ ) {
        wa->words[nw] = _onpointer_charbuf_init( wordsize );
        ++wa->words_size;
    }

    _onepointer_ciphers_word_array_switch( wa, 0 );

    return wa;
}

bool _onepointer_ciphers_word_array_switch( _word_array_t* word_array, const unsigned short wordnum ) {
    if ( wordnum >= word_array->words_size ) return false;

    word_array->word = word_array->words[wordnum];
    word_array->words_index = wordnum;

    return true;
}

char* _onepointer_ciphers_word_array( _word_array_t* word_array, const unsigned short wordnum ) {
    if ( wordnum >= word_array->words_size ) return "";
    return word_array->words[wordnum]->buf;
}

char* _onepointer_ciphers_word_array_complete( _word_array_t* word_array ) {
    size_t bufsize_each = word_array->words[0]->bufsize;
    size_t size_wa = word_array->words_size * bufsize_each;

    _charbuf_t* cb = _onpointer_charbuf_init( size_wa );
    
    size_t cbi = 0;
    for ( unsigned int wa = 0; wa < word_array->words_size && cbi < size_wa; wa++, cbi = cbi + bufsize_each ) {
        _onepointer_charbuf_cpy_n( cb, word_array->words[wa]->buf, 0, cbi, bufsize_each );
    }
    
    return cb->buf;
}

char* _onepointer_ciphers_word_array_word( _word_array_t* word_array ) {

    const unsigned short wordssize = word_array->words_size;
    const unsigned short wordlength = word_array->words[0]->bufsize;


    char* word = (char*) malloc(sizeof(char)*wordssize*wordlength);
    unsigned int wordidx = 0;
    for ( unsigned short w = 0; w < wordssize; w++ ) {
        for ( unsigned short wi = 0; w < wordlength; wi++, wordidx++ ) {
            char c = word_array->words[w]->buf[wi];
            word[wordidx] = isalnum((int) c) == 0 ? c : ' ';
        }
    }

    return word;
}

void _onepointer_ciphers_word_array_writeto( _word_array_t* word_array, const unsigned short wordnum, const char* _Source ) {
    if ( wordnum >= word_array->words_size ) return;
    _onepointer_charbuf_cpy_n( word_array->words[wordnum], _Source, 0, 0, word_array->words[wordnum]->bufsize );
}

_word_table_t* _onepointer_ciphers_word_table_init( const unsigned short number_words, const unsigned short wordsize, const unsigned short number_word_lanes ) {
    _word_table_t* wt = (_word_table_t*) malloc(sizeof(_word_table_t));

    wt->word_lanes_size = 0;
    wt->words_size = wordsize;

    wt->word_lanes = (_word_array_t*) malloc(sizeof(_word_array_t*)*number_word_lanes);
    for ( unsigned short nwl = 0; nwl < number_word_lanes; nwl++ ) {
        wt->word_lanes[nwl] = _onepointer_ciphers_word_array_init( number_words, wordsize );
        ++wt->word_lanes_size;
    }

    _onepointer_ciphers_word_table_switch2( wt, 0, 0 );

    return wt;
}

bool _onepointer_ciphers_word_table_switch( _word_table_t* word_table, const unsigned short wordlanenum ) {
    if ( wordlanenum >= word_table->word_lanes_size ) return false;

    word_table->word_lane = word_table->word_lanes[wordlanenum];
    word_table->word_lanes_index = wordlanenum;

    return true;
}

bool _onepointer_ciphers_word_table_switch2( _word_table_t* word_table, const unsigned short wordlanenum, const unsigned short wordnum ) {
    if ( ! _onepointer_ciphers_word_table_switch( word_table, wordlanenum ) ) return false;
    return _onepointer_ciphers_word_array_switch( word_table->word_lane, wordnum );
}

char* _onepointer_ciphers_word_table( _word_table_t* word_table, const unsigned short wordlanenum, const unsigned short wordnum ) {
    if ( wordlanenum >= word_table->word_lanes_size || wordnum >= word_table->words_size ) return "";
    return word_table->word_lanes[wordlanenum]->words[wordnum]->buf;
}

char* _onepointer_ciphers_word_table_lane( _word_table_t* word_table, const unsigned short wordlanenum ) {
    if ( wordlanenum >= word_table->word_lanes_size ) return "";
    return _onepointer_ciphers_word_array_word( word_table->word_lanes[wordlanenum] );
}

char* _onepointer_ciphers_word_table_complete( _word_table_t* word_table ) {
    size_t bufsize_each = word_table->word_lanes[0]->words[0]->bufsize;
    size_t wordsize = word_table->word_lanes_size * word_table->words_size * bufsize_each;

    _charbuf_t* cb = _onpointer_charbuf_init( wordsize );

    unsigned int cbi = 0;
    for ( unsigned int wtl = 0; wtl < word_table->word_lanes && cbi < wordsize; wtl++, cbi = cbi + bufsize_each ) {
        char* word_lane = _onepointer_ciphers_word_table_lane( word_table, wtl );
        _onepointer_charbuf_cpy_n( cb, word_lane, 0, cbi, bufsize_each );
        
    }

    return cb->buf;
}

void _onepointer_ciphers_word_table_writeto( _word_table_t* word_table, const unsigned short wordlanenum, const unsigned short wordnum, const char* _Source ) {
    if ( wordlanenum >= word_table->word_lanes_size || wordnum >= word_table->word_lanes[wordlanenum]->words_size ) return;
   _onepointer_ciphers_word_array_writeto( word_table->word_lanes[wordlanenum], wordnum, _Source );
}


unsigned short _getpos_laneshifting_bitblocks( const unsigned short bitblocks, const unsigned short shifting_modificator, const unsigned short start_pos, const bool shift_direction_left ) {
    unsigned short sms_mod = shift_direction_left ? (start_pos - shifting_modificator) % bitblocks : (start_pos + shifting_modificator) % bitblocks;
    if ( sms_mod > 0 ) {
        short pos = start_pos;
        unsigned short overcounting_rest = 0;
        for ( unsigned short smi = 0; smi < sms_mod; smi++ ) {
            if ( smi == sms_mod - 1 ) return shift_direction_left ? pos - overcounting_rest : pos + overcounting_rest;
            else overcounting_rest = shift_direction_left ? bitblocks - pos - overcounting_rest : pos + overcounting_rest - bitblocks;
        }
    } else return shift_direction_left ? start_pos - shifting_modificator : start_pos + shifting_modificator;
}


unsigned short _getpos_laneshifting( const unsigned short wordlength, const unsigned short shifting_modificator, const unsigned short start_pos, const bool shift_direction_left ) {
    unsigned short sms_mod = shift_direction_left ? (start_pos - shifting_modificator) % wordlength : (start_pos + shifting_modificator) % wordlength;
    if ( sms_mod > 0 ) {
        short pos = start_pos;
        unsigned short overcounting_rest = 0;
        for ( unsigned short smi = 0; smi < sms_mod; smi++ ) {
            if ( smi == sms_mod - 1 ) return shift_direction_left ? pos - overcounting_rest : pos + overcounting_rest;
            else overcounting_rest = shift_direction_left ? wordlength - pos - overcounting_rest : pos + overcounting_rest - wordlength;
        }
    } else return shift_direction_left ? start_pos - shifting_modificator : start_pos + shifting_modificator;
}


word_t* _rightshift_bitblocks( _word_array_t* word_lane, const unsigned short w, const unsigned short origin_w, const unsigned short shifting_modificator, const bool rightshift ) {
    if ( w == origin_w ) return 0;

    // Save the final overwritten word as tmp
    unsigned short goal_pos = _getpos_laneshifting_bitblocks( word_lane->words_size, shifting_modificator, w, ! rightshift );
    
    word_t* goal_word = word_lane->words[goal_pos];
    word_t* origin_word = word_lane->words[w];

    word_lane->words[goal_pos] = origin_word;

    word_t* overwritten_word = _rightshift_bitblocks( word_lane
                                    , goal_pos
                                    , origin_w, shifting_modificator, rightshift
                                );

    word_lane->words[w] = overwritten_word;

    return goal_word;
}



bool _leftrotate_and_rightshift_bitblocks( _word_array_t* word_lane, const unsigned short w, const unsigned short shift_positions ) {
    word_t* origin_word = word_lane->words[w];
    
    const unsigned short goal_pos = _getpos_laneshifting_bitblocks( word_lane->words_size, shift_positions, w, true );
    word_t* word_at_final_pos = word_lane->words[goal_pos];
    word_t* compare_word = _rightshift_bitblocks( word_lane, goal_pos, w, shift_positions, true );

    if ( strcmp( compare_word->buf, word_at_final_pos->buf ) != 0 ) return false;

    word_lane->words[goal_pos] = origin_word;

    return true;
}