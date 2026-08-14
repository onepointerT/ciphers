
#include "__rsa_utils.h"

#include <stdlib.h>

#include <libm.h>
#include <libprim.h>
#include <_random_interval.h>


const unsigned int genprim( const unsigned int from, const unsigned int to ) {
    return prim_chooseone( from, to );
}

const unsigned int genprim_default() {
    return prim_chooseone( pow(10, 1023), pow(10, 1024) );
}


const double _totient_euler( const int n_divisor  ) {
    struct Interval* primint = prim_number_interval( 1, n_divisor );
    struct IntArray* primdivn = _filter_interval_by_divisor( primint, n_divisor );
    const size_t size = primdivn->size;

    double result_product = 1.0;
    for ( unsigned int pn = 0; pn < size; pn++ ) {
        result_product = result_product * ( 1 - (1 / primdivn->numbers[pn]) );
    }

    return n_divisor * result_product;
}

const double _totient_carmichael( const struct RSA_Values* rsa_math ) {
    bool is_odd = ( rsa_math->n % 2 ) == 0;
    double phi_n = 0;
    if ( is_odd && is_prim_number( rsa_math->n ) ) {
        phi_n = _totient_euler( rsa_math->n );
    } else if ( is_odd ) {
        phi_n = 0.5 * _totient_euler( rsa_math->n );
    } else {
        struct Interval* primint = prim_number_interval( 1, rsa_math->n );
        const size_t size = primint->interval->size;

        phi_n = 0;
        unsigned int pi = 1;
        for ( ; pi < size; pi = pi + 2 ) {
            phi_n = phi_n + lcm( primint->interval->numbers[pi], primint->interval->numbers[pi - 1] );
        }
        if ( pi <= size ) {
            phi_n = phi_n + lcm( primint->interval->numbers[primint->interval->size-1], 1.0);
        }
    }

    return phi_n;
}


struct RSA_Values* _init_rsa_values( const unsigned int pint, const unsigned int qint ) {
    struct RSA_Values* rsav = (struct RSA_Values*) malloc(sizeof(struct RSA_Values));

    rsav->p = pint;
    rsav->q = qint;
    rsav->n = rsav->p * rsav->q;

    return rsav;
}


rsa_t* __rsa_generic( struct RSA_Values* rsav ) {

    const double delta_n = _totient_carmichael( rsav );
    struct IntArray* e_interval = __init_int_array( delta_n - 1 );
    __init_int_array_interval( 1, delta_n, e_interval );
    
    unsigned int e = 0;
    while ( true ) {
        unsigned int e_idx = int_array_choseone( e_interval );
        e = e_interval->numbers[e_idx];

        if ( gcd( e, delta_n ) == 1 ) break;
    }

    unsigned int d = 0;
    while ( true ) {
        d = gcd( e, delta_n );

        if ( d * e == 1 % (int) delta_n ) break;
    }

    rsa_t* rsa = _onepointer_ciphers_init_rsa( sizeof((char*) d)/sizeof(const char) );
    rsa->pub->hash = (char*) (0x0 + (unsigned int) e % rsav->n);
    rsa->priv->hash = (char*) (0x0 + (unsigned int) d);

    return rsa;
}


rsa_t* __rsa( const unsigned int from_seed, const unsigned int to_seed ) {
    struct RSA_Values* rsav = _init_rsa_values( genprim(from_seed, to_seed), genprim(from_seed, to_seed) );

    return __rsa_generic( rsav );
}


rsa_t* make_rsa( const char* key ) {
    
    unsigned int seed = 1;
    const unsigned int keyint = 0x0 + (unsigned int) key;
    while ( keyint > seed ) {
        seed = genprim( keyint, 3*keyint );
    }

    const unsigned int from_prim = genprim( seed, 2*keyint );
    const unsigned int to_prim = genprim( keyint, 2*seed );

    return __rsa( from_prim, to_prim );
}