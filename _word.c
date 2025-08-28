
#include "_word.h"

#include <stdlib.h>
#include <ctype.h>

_word_array_t* _bankest_word_array_init( const unsigned short number_words, const unsigned short wordsize ) {
    _word_array_t* wa = (_word_array_t*) malloc(sizeof(_word_array_t));

    wa->words_size = 0;

    wa->words = (word_t*) malloc(sizeof(word_t*)*number_words);
    for ( unsigned short nw = 0; nw < number_words; nw++ ) {
        wa->words[nw] = _oneptr_ciphers_charbuf_init( wordsize );
        ++wa->words_size;
    }

    _oneptr_ciphers_word_array_switch( wa, 0 );

    return wa;
}

bool _oneptr_ciphers_word_array_switch( _word_array_t* word_array, const unsigned short wordnum ) {
    if ( wordnum >= word_array->words_size ) return false;

    word_array->word = word_array->words[wordnum];
    word_array->words_index = wordnum;

    return true;
}

char* _oneptr_ciphers_word_array( _word_array_t* word_array, const unsigned short wordnum ) {
    if ( wordnum >= word_array->words_size ) return "";
    return word_array->words[wordnum]->buf;
}

char* _oneptr_ciphers_word_array_complete( _word_array_t* word_array ) {
    size_t bufsize_each = word_array->words[0]->bufsize;
    size_t size_wa = word_array->words_size * bufsize_each;

    _charbuf_t* cb = _oneptr_ciphers_charbuf_init( size_wa );
    
    size_t cbi = 0;
    for ( unsigned int wa = 0; wa < word_array->words_size && cbi < size_wa; wa++, cbi = cbi + bufsize_each ) {
        _oneptr_ciphers_charbuf_cpy_n( cb, word_array->words[wa]->buf, 0, cbi, bufsize_each );
    }
    
    return cb->buf;
}

char* _oneptr_ciphers_word_array_word( _word_array_t* word_array ) {

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

void _oneptr_ciphers_word_array_writeto( _word_array_t* word_array, const unsigned short wordnum, const char* _Source ) {
    if ( wordnum >= word_array->words_size ) return;
    _oneptr_ciphers_charbuf_cpy_n( word_array->words[wordnum], _Source, 0, 0, word_array->words[wordnum]->bufsize );
}

_word_table_t* _oneptr_ciphers_word_table_init( const unsigned short number_words, const unsigned short wordsize, const unsigned short number_word_lanes ) {
    _word_table_t* wt = (_word_table_t*) malloc(sizeof(_word_table_t));

    wt->word_lanes_size = 0;
    wt->words_size = wordsize;

    wt->word_lanes = (_word_array_t*) malloc(sizeof(_word_array_t*)*number_word_lanes);
    for ( unsigned short nwl = 0; nwl < number_word_lanes; nwl++ ) {
        wt->word_lanes[nwl] = _oneptr_ciphers_word_array_init( number_words, wordsize );
        ++wt->word_lanes_size;
    }

    _oneptr_ciphers_word_table_switch2( wt, 0, 0 );

    return wt;
}

bool _oneptr_ciphers_word_table_switch( _word_table_t* word_table, const unsigned short wordlanenum ) {
    if ( wordlanenum >= word_table->word_lanes_size ) return false;

    word_table->word_lane = word_table->word_lanes[wordlanenum];
    word_table->word_lanes_index = wordlanenum;

    return true;
}

bool _oneptr_ciphers_word_table_switch2( _word_table_t* word_table, const unsigned short wordlanenum, const unsigned short wordnum ) {
    if ( ! _oneptr_ciphers_word_table_switch( word_table, wordlanenum ) ) return false;
    return _oneptr_ciphers_word_array_switch( word_table->word_lane, wordnum );
}

char* _oneptr_ciphers_word_table( _word_table_t* word_table, const unsigned short wordlanenum, const unsigned short wordnum ) {
    if ( wordlanenum >= word_table->word_lanes_size || wordnum >= word_table->words_size ) return "";
    return word_table->word_lanes[wordlanenum]->words[wordnum]->buf;
}

char* _oneptr_ciphers_word_table_lane( _word_table_t* word_table, const unsigned short wordlanenum ) {
    if ( wordlanenum >= word_table->word_lanes_size ) return "";
    return _oneptr_ciphers_word_array_word( word_table->word_lanes[wordlanenum] );
}

char* _oneptr_ciphers_word_table_complete( _word_table_t* word_table ) {
    size_t bufsize_each = word_table->word_lanes[0]->words[0]->bufsize;
    size_t wordsize = word_table->word_lanes_size * word_table->words_size * bufsize_each;

    _charbuf_t* cb = _oneptr_ciphers_charbuf_init( wordsize );

    unsigned int cbi = 0;
    for ( unsigned int wtl = 0; wtl < word_table->word_lanes && cbi < wordsize; wtl++, cbi = cbi + bufsize_each ) {
        char* word_lane = _oneptr_ciphers_word_table_lane( word_table, wtl );
        _oneptr_ciphers_charbuf_cpy_n( cb, word_lane, 0, cbi, bufsize_each );
        
    }

    return cb->buf;
}

void _oneptr_ciphers_word_table_writeto( _word_table_t* word_table, const unsigned short wordlanenum, const unsigned short wordnum, const char* _Source ) {
    if ( wordlanenum >= word_table->word_lanes_size || wordnum >= word_table->word_lanes[wordlanenum]->words_size ) return;
   _oneptr_ciphers_word_array_writeto( word_table->word_lanes[wordlanenum], wordnum, _Source );
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