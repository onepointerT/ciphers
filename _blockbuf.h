
#ifndef __BLOCKBUF_H_
#define __BLOCKBUF_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>


typedef struct _oneptr_ciphers_charbuf {
    char* buf;
    unsigned short bufsize;
} _charbuf_t;


_charbuf_t* _oneptr_ciphers_charbuf_init( const unsigned short bufsize );
short _oneptr_ciphers_charbuf_cpy( _charbuf_t* cbuf_dest, const char* _Source, const unsigned long start, const unsigned short insert_pos );
short _oneptr_ciphers_charbuf_cpy_n( _charbuf_t* cbuf_dest, const char* _Source, const unsigned long start, const unsigned short insert_pos, const unsigned long _N );



typedef struct _oneptr_ciphers_blockbuf {
    _charbuf_t** buffers;
    _charbuf_t* buf;
    unsigned int buffercount;
    unsigned int bufferindex;
} _blockbuf_t;


_blockbuf_t* _oneptr_ciphers_blockbuf_init( const unsigned int buffercount, const unsigned short buffersize_each );
bool _oneptr_ciphers_blockbuf_switchbuf( _blockbuf_t* _blkbuf, const unsigned int bufindex );
bool _oneptr_ciphers_blockbuf_addbuf( _blockbuf_t* _blkbuf );
bool _oneptr_ciphers_blockbuf_addbufs( _blockbuf_t* _blkbuf, const unsigned int addnum_buffers );
short _oneptr_ciphers_blockbuf_strcpy( _blockbuf_t* _blkbuf, const char* _Source, const unsigned long start, const unsigned short insert_pos );
short _oneptr_ciphers_blockbuf_strcpy_n( _blockbuf_t* _blkbuf, const char* _Source, const unsigned long start, const unsigned short insert_pos, const unsigned long _N );

char* _word( _blockbuf_t* _blockbuf, const unsigned int index );


#ifdef __cplusplus
}
#endif

#endif // __BLOCKBUF_H_