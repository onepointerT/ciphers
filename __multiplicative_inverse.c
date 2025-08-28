
#include "logic.h"

#include "libm.h"

double __multiplicative_inverse( const double n, const double base, const double exponent ) {
    return multiplinv( exp( pow( base, exponent ) * fabs(ln( n )) ) );
}