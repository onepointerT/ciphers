
#include "_blockbuf.h"

#include <stdlib.h>

short _oneptr_ciphers_charbuf_cpy( _charbuf_t* cbuf_dest, const char* _Source, const unsigned long start, const unsigned short insert_pos ) {

    unsigned int _srclength = sizeof(_Source) / sizeof(const char);

    unsigned int ibuf = insert_pos < cbuf_dest->bufsize ? insert_pos : cbuf_dest->bufsize;
    unsigned long isrc = start < _srclength ? start : _srclength; 
    if ( isrc == _srclength || ibuf == cbuf_dest->bufsize ) return -1;

    unsigned short cc = 0;
    for ( ; ibuf < cbuf_dest->bufsize && isrc < _srclength; ibuf++, isrc++, ++cc ) {
        cbuf_dest->buf[ibuf] = _Source[isrc];
    }

    return cc;
}

short _oneptr_ciphers_charbuf_cpy_n( _charbuf_t* cbuf_dest, const char* _Source, const unsigned long start, const unsigned short insert_pos, const unsigned long _N ) {
    
    unsigned int _srclength = sizeof(_Source)/sizeof(const char);

    unsigned int ibuf = insert_pos < cbuf_dest->bufsize ? insert_pos : cbuf_dest->bufsize;
    unsigned long isrc = start < _srclength ? start : _srclength; 
    if ( isrc == _srclength || ibuf == cbuf_dest->bufsize ) return -1;

    unsigned short cc = 0;
    for ( ; ibuf < cbuf_dest->bufsize && isrc < _srclength && isrc < start + _N; ibuf++, isrc++, ++cc ) {
        cbuf_dest->buf[ibuf] = _Source[isrc];
    }

    return cc;

}


bool _oneptr_ciphers_blockbuf_switchbuf( _blockbuf_t* _blkbuf, const unsigned int bufindex ) {
    if ( bufindex >= _blkbuf->buffercount ) return false;

    _blkbuf->buf = _blkbuf->buffers[bufindex];
    _blkbuf->bufferindex = bufindex;

    return true;
}


bool _oneptr_ciphers_blockbuf_addbuf( _blockbuf_t* _blkbuf ) {

    _blkbuf->buffers[_blkbuf->buffercount] = (_charbuf_t*) malloc(sizeof(_charbuf_t*));
    _blkbuf->buffercount = _blkbuf->buffercount + 1;

    unsigned int bufidx = _blkbuf->bufferindex;
    if ( ! _oneptr_ciphers_blockbuf_switchbuf(_blkbuf, _blkbuf->buffercount-1) ) return false;
    else _oneptr_ciphers_blockbuf_switchbuf(_blkbuf, bufidx);

    return true;
}


bool _oneptr_ciphers_blockbuf_addbufs( _blockbuf_t* _blkbuf, const unsigned int addnum_buffers ) {
    for ( unsigned int anb = 0; anb < addnum_buffers; anb++ ) {
        if ( ! _oneptr_ciphers_blockbuf_addbuf( _blkbuf ) ) return false;
    }

    return true;
}


short _oneptr_ciphers_blockbuf_strcpy( _blockbuf_t* _blkbuf, const char* _Source, const unsigned long start, const unsigned short insert_pos ) {
    return _oneptr_ciphers_charbuf_cpy( _blkbuf->buf, _Source, start, insert_pos );
}


short _oneptr_ciphers_blockbuf_strcpy_n( _blockbuf_t* _blkbuf, const char* _Source, const unsigned long start, const unsigned short insert_pos, const unsigned long _N ) {
    if ( _blkbuf->buf == 0 ) return -1;
    return _oneptr_ciphers_charbuf_cpy_n( _blkbuf->buf, _Source, start, insert_pos, _N );
}


char* _word( _blockbuf_t* _blockbuf, const unsigned int index ) {
    if ( _blockbuf->buffercount <= index ) return "";
    return _blockbuf->buffers[index]->buf;
}