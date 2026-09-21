
#ifndef __STR_H_
#define __STR_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stddef.h>

bool strncpy_s_idx( const char* _Source, const size_t _Start, const size_t _N
                  , char* _Dest, const size_t _Start_Destination_Index
);

size_t strfind( const char* _Str, const char _chr );
size_t strnfind_s( const char* _Str, const size_t _N, const char* searchstr );

size_t strfindpos( const char* _Str, const char _chr, const size_t _Start, const size_t _N );
size_t strfindpos_s( const char* _Str, const char* searchstr, const size_t _Start, const size_t _N );

const char** _onepointer_str_split_at( const char* str, const char* delimiters );


#ifdef __cplusplus
}
#endif

#endif // __STR_H_