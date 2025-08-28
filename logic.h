
#ifndef __LOGIC_H_
#define __LOGIC_H_

#ifdef __cplusplus
extern "C" {
#endif


char __xor_char( const char c1, const char c2 );
char* __xor_str( const char* s1, const char* s2 );

char* __leftrotate( const char* word, const unsigned short bitlength, const unsigned short wordparts );
char* __leftrotate_short( const char* word, const unsigned short n_positions );
//const char* __rightrotate( const char* word, const unsigned short bitlength, const unsigned short wordparts );

double __multiplicative_inverse( const double n, const double base, const double exponent );

char* leftrotate( const char* word, const unsigned short wordbit, const unsigned short bitlength, const unsigned int bits_selected );
char* leftrotatemultiple( const char* word, const unsigned int rotate_mulitplicator, const unsigned short wordbit, const unsigned short bitlength, const unsigned int bits_selected );

#ifdef __cplusplus
}
#endif

#endif // __LOGIC_H_