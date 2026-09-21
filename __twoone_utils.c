
#include "__twoone_utils.h"

#include "_pyramidbuffer.h"

#include <stdbool.h>
#include <stddef.h>

#include <libm.h>


_word_pyramidbuf_t* __make_pyramidbuf_twoone( const size_t wordlength, const size_t msg_or_passphrase_length, const size_t keylength ) {
    _word_pyramidbuf_t* wpyb = _onepointer_pyramidbuffer_init( PYRAMID_MINUS_2_PER_ROW, wordlength
                                                             , (size_t) fabsl( msg_or_passphrase_length/4 ) 
                                                                        + (fabsl( msg_or_passphrase_length / 3 ) * 2)
                                                                        + 2 + ((size_t) 2*fabsl(keylength / wordlength))
                                                             , ((size_t) fabsl( msg_or_passphrase_length / wordlength )) + 4
                                                             , 0, NULL
    );
}

// The msg and the key are written beneath to each other into a complete row and shifted to the topper word stones.
bool __fill_pyramidbuf_twoone( _word_pyramidbuf_t* wpyb, const char* msg, const char* key ) {
    size_t msglength = sizeof(msg) / sizeof(const char);
    size_t keylength = sizeof(key) / sizeof(const char);
    
    // Starting from the topside, leaving the toppest emtpy, the msg and the key are filled into the toppest
    // few rows of the pyramid, leaving the latest floor rows 0-~4 empty for calculational algorithm.
    int idx_fitting_row = wpyb->num_rows - 1;
    for ( ; idx_fitting_row >= 0; idx_fitting_row-- ) {
        size_t words_in_row = sizeof(wpyb->words[idx_fitting_row]) / sizeof(_pyramidword_t*);
        if ( words_in_row*wpyb->wordlength >= msglength+keylength ) break;
    }
    if ( idx_fitting_row < 0 ) return false;

    _pyramidword_t** prow = wpyb->words[idx_fitting_row];
    size_t wpos = 0;
    char* wbuf = msg;
    _pyramidword_t* pword = NULL;
    size_t widx = 0;
    for ( size_t spos = 0; spos < msglength+keylength; spos++, widx++ ) {
        if ( wpos >= msglength ) { wpos = 0; wbuf = key; }

        if ( ((size_t) fabsl((wpos+1)/wpyb->wordlength)) >= wpyb->wordlength
          || pword == NULL
        ) {
            size_t num_rowwords = wpos == 0 ? 0 : (size_t) fmodl( wpos+1, wpyb->wordlength );
            pword = prow[num_rowwords];
            widx = 0;
        }

        pword->word->buf[widx] = wbuf[spos];
    }

    // Now shift the full row to the topper word stones
    size_t rw = 0;
    size_t prow_length = sizeof(prow) / sizeof(_pyramidword_t*);
    pword = prow[rw];
    widx = 0;
    size_t ri_widx = 0;
    for ( size_t ri = idx_fitting_row + 1; ri < wpyb->num_rows; ri++, widx++, ri_widx = 0 ) {

        _pyramidword_t** current_row_filling = wpyb->words[ri];
        size_t current_row_length = sizeof(current_row_filling) / sizeof(_pyramidword_t*);

        for ( size_t cw = 0; cw < current_row_length; cw++, widx++, ri_widx++ ) {
            _pyramidword_t* current_word_filling = current_row_filling[cw];

            if ( widx >= wpyb->wordlength ) {
                widx = 0;
                ++rw;
                pword = prow[rw];
            }
            if ( ri_widx >= wpyb->wordlength ) { ri_widx = 0; continue; }

            current_word_filling->word->buf[ri_widx] = pword->word->buf[widx];
        }

    }

    return true;
}


size_t __make_pyramidbuf_inheritors_twoone( _word_pyramidbuf_t* wpyb, const size_t idx_fitting_row ) {

}


hash_t* __twoone_generic( const char* passphrase, const char* key ) {
    size_t msglength = sizeof(msg) / sizeof(const char);
    size_t keylength = sizeof(key) / sizeof(const char);
    _word_pyramidbuf_t* wpyb = __make_pyramidbuf_twoone( 8, msglength, keylength );
    if ( ! __fill_pyramidbuf_twoone( wpyb, msg, key ) ) return NULL;
    
    // AES the PIN and the passphrase

    // Shorten the PIN with RSA, until it is not longer than the wordlength for the toppest word stone

    // SHA1 the PIN and the passphrase words with cipher streaming to the follow-up row
    
    // RSA the result with the seed of the toppest stone (as automatic passphrase appended to SHA1) to the next row

    // Now RSA pub/priv (passphrase is private key user-input authentificator) and shift the public key into the next row,
    // the privat key in the most bottom row.

    // The floor row now creates a resulting ringbuffer of wordlength with words of the count of private_key_length/wordlength + 1
    // NOTE: This is a password/PIN checking and input algorithm, that fulfills, since the public key can be recalculated with the filled
    //      pyramidbuffer.

    // Put the toppest word of the pyramid into the empty word of the result ring.

    // Now we can legitimly, legal and as long as the correct PIN and passphrase is provided and authentificated with `__twoone_authentificate`
    //    automatically sign or encrypt and decrypt a password. Even sending the resulting ringbuffer per cloud/email is legitim secure with pgp
    //    as long as the passphrase is matched by the PIN and the passphrase is unknown to everyone except legal co-employers and recipients.
    // NOTE: A combination of AES message/passphrase algorithm should be preferred to store the PIN-matching password in an encrypted, PIN-testable
    //      wallet-like string.
}


hash_t* __twoone_authentificate( const char* passphrase, const char* key, const _pyramidring_t* pyr ) {

}

hash_t* make_twoone( const char* passphrase, const char* key ) {

}
