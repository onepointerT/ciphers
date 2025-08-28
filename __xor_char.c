
#include "logic.h"

char __xor_char( const char c1, const char c2 ) {
    return c1 | c2;
}

char* __xor_str( const char* s1, const char* s2 ) {
    return (char*) (((int) s1) | ((int) s2));
}