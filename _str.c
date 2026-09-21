
#include "_str.h"

#include <stdlib.h>
#include <string.h>


bool strncpy_s_idx( const char* _Source, const size_t _Start, const size_t _N
                  , char* _Dest, const size_t _Start_Destination_Index
) {
    size_t strlen = sizeof(_Source) / sizeof(const char*);
    if ( _Start >= strlen ) return false;

    // Get the substr
    char* substr = (char*) malloc(sizeof(char)*_N);
    size_t cpy_idx = 0;
    for ( size_t substr_idx = _Start; substr_idx < strlen && substr_idx <= _Start + _N; substr_idx++, cpy_idx++ ) {
        substr[cpy_idx] = _Source[substr_idx];
    }

    // Insert the substr
    if ( _Dest == NULL ) _Dest = (char*) malloc(sizeof(char)*_N);
    size_t strlen_destination = sizeof(_Dest) / sizeof(char);
    if ( _Start_Destination_Index >= strlen_destination ) return false;
    size_t dest_idx = _Start_Destination_Index;
    for ( size_t substr_idx = 0; substr_idx < _N && substr_idx < strlen_destination; substr_idx++ ) {
        _Dest[dest_idx+substr_idx] = substr[substr_idx];
    }

    return true;
}


size_t strfind( const char* _Str, const char _chr ) {
    size_t count_found = 0;

    size_t strlen = sizeof(_Str) / sizeof(const char);
    for ( size_t c = 0; c < strlen; c++ ) {
        if ( _Str[c] == _chr ) ++count_found;
    }

    return count_found;
}


size_t strnfind_s( const char* _Str, const size_t _N, const char* searchstr ) {
    size_t count_found = 0;

    size_t strlen = sizeof(_Str) / sizeof(const char);
    size_t strlen_searchstr = sizeof(searchstr) / sizeof(const char);
    for ( size_t c = 0; c < strlen; c++ ) {
        for ( size_t s = 0; s < strlen - strlen_searchstr; s++ ) {
            if ( _Str[c+s] != searchstr[s] ) break;
            else if ( s == strlen_searchstr - 1 ) ++count_found;
        }
    }

    return count_found;
}


size_t strfindpos( const char* _Str, const char _chr, const size_t _Start, const size_t _N ) {
    size_t strlen = sizeof(_Str) / sizeof(const char);
    if ( _Start >= strlen || _Start + _N >= strlen ) return strlen;

    for ( size_t s = _Start
        ; _N == 0 ? s < strlen : s < _Start + _N && s < strlen
        ; s++
    ) {
        if ( _Str[s] == _chr ) return s;
    }

    return strlen;
}


size_t strfindpos_s( const char* _Str, const char* searchstr, const size_t _Start, const size_t _N ) {
    size_t strlen = sizeof(_Str) / sizeof(const char);
    size_t strlen_searchstr = sizeof(searchstr) / sizeof(const char);
    if ( _Start >= strlen || _Start + _N >= strlen || _Start + searchstr >= strlen ) return strlen;

    for ( size_t s = _Start
        ; ( _N == 0 ? s < strlen : s < _Start + _N && s < strlen ) && s + strlen_searchstr < strlen
        ; s++
    ) {
        size_t strlen_s_current = strlen_searchstr > _N ? _N : strlen_searchstr;
        char* s_current = (char*) malloc(sizeof(char)*strlen_s_current);
        for ( size_t sc = 0; sc < strlen_s_current; sc++ ) { s_current[sc] = _Str[s+sc]; }
        if ( strcmp( s_current, searchstr ) == 0 ) return s;
    }

    return strlen;
}


const char** _onepointer_str_split_at( const char* str, const char* delimiters ) {
    size_t strlen_delimiters = sizeof(delimiters) / sizeof(const char);
    size_t delim_found = 0;
    size_t* num_delim_ocurrences = (size_t*) malloc( sizeof(size_t)*strlen_delimiters );
    for ( size_t d = 0; d < strlen_delimiters; d++ ) {
        num_delim_ocurrences[d] = strfind( str, delimiters[d] );
        delim_found += num_delim_ocurrences[d];
    }

    size_t strlen = sizeof(str) / sizeof(const char);
    char** s_arr = (char**) malloc(sizeof(char*)*delim_found);
    size_t s_arr_idx = 0;
    size_t s_pos = 0;
    size_t* pos_next_delimiter = (size_t*) malloc(sizeof(size_t)*strlen_delimiters);
    while ( s_pos < strlen ) {
        // Find the delimiters
        for ( size_t d = 0; d < strlen_delimiters; d++ ) {
            pos_next_delimiter[d] = strfindpos( str, delimiters[d], s_pos, 0 );
        }
        // Find the next of them
        size_t idx_next_delimiter = pos_next_delimiter[0];
        for ( size_t d = 1; d < strlen_delimiters; d++ ) {
            if ( pos_next_delimiter[d] < idx_next_delimiter ) idx_next_delimiter = pos_next_delimiter[d];
        }
        // Copy the substring
        size_t strlen_substr = idx_next_delimiter - s_pos;
        s_arr[s_arr_idx] = (char*) malloc(sizeof(char)*strlen_substr);
        for ( size_t s = 0; s < strlen_substr; s++ ) {
            s_arr[s_arr_idx][s] = str[idx_next_delimiter+s];
        }
        s_pos = idx_next_delimiter;
        ++s_arr_idx;
    }

    return s_arr;
}