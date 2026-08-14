
#ifndef __BLOCKBUF_H_
#define __BLOCKBUF_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>


typedef struct _onepointer_charbuf {
    char* buf;
    size_t bufsize;
} _charbuf_t;


_charbuf_t* _onpointer_charbuf_init( const unsigned size_t bufsize );
short _onepointer_charbuf_cpy( _charbuf_t* cbuf_dest, const char* _Source, const unsigned long start, const size_t insert_pos );
short _onepointer_charbuf_cpy_n( _charbuf_t* cbuf_dest, const char* _Source, const unsigned long start, const size_t insert_pos, const unsigned long _N );



typedef struct _onepointer_blockbuf {
    _charbuf_t** buffers;
    _charbuf_t* buf;
    unsigned int buffercount;
    unsigned int bufferindex;
} _blockbuf_t;


_blockbuf_t* _onepointer_blockbuf_init( const unsigned int buffercount, const size_t buffersize_each );
bool _onepointer_blockbuf_switchbuf( _blockbuf_t* _blkbuf, const unsigned int bufindex );
bool _onepointer_blockbuf_addbuf( _blockbuf_t* _blkbuf );
bool _onepointer_blockbuf_addbufs( _blockbuf_t* _blkbuf, const unsigned int addnum_buffers );
short _onepointer_blockbuf_strcpy( _blockbuf_t* _blkbuf, const char* _Source, const unsigned long start, const size_t insert_pos );
short _onepointer_blockbuf_strcpy_n( _blockbuf_t* _blkbuf, const char* _Source, const unsigned long start, const size_t insert_pos, const unsigned long _N );

char* _word( _blockbuf_t* _blockbuf, const unsigned int index );


#ifdef __cplusplus
}
#endif

#endif // __BLOCKBUF_H_