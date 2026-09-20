
#ifndef __STR_H_
#define __STR_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

size_t strfind( const char* _Str, const char _chr );
size_t strnfind_s( const char* _Str, const size_t _N, const char* searchstr );

size_t strfindpos( const char* _Str, const char _chr, const size_t _Start, const size_t _N );
size_t strfindpos_s( const char* _Str, const char* searchstr, const size_t _Start, const size_t _N );

const char** _onepointer_str_split_at( const char* str, const char* delimiters );


#ifdef __cplusplus
}
#endif

#endif // __STR_H_