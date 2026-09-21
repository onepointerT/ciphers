
#include "__twopad_utils.h"


hash_t* __twopad_generic( const char* passphrase, const char* key_pin ) {
    // Intiialize the pyramid with PYRAMID_MINUS_2_PER_ROW, have one ringbuffer on each side per row.

    // RSA the passphrase, and put the public key in the pyramid row with the index 1

    // AES the pin and put the resulting hash in the pyramid row with the index 3

    // Like in all this algorithm, every character that is more long than the row goes to the rings at the sides.

    // SHA3 the rows 1 and 3 and write it to the floor row

    // Once per row, cipher stream to the topper row, beginning with row 3 and then reference wave back to the row,
    // shifting to the rings at the sides. The rings now use a chosable ring algorithm with shifting and swapping and
    // shift themselves in the SHA3'ed row below, until the top - 1 of the pyramid is reached.

    // Lastly, but not least, the inheritor algorithm is used symetrically until top, to get a generic
    // seed-/checksum-PIN at the toppest row.

    // The floor row is putted into the hash and the most toppest word is appended and returned.
}

bool __twopad_legitimate_pin( const hash_t* twopad_stored, const char* key_pin ) {

}

hash_t* make_twopad( const char* passphrase, const char* key_pin ) {

}

