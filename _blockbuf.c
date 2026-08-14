
#include "_blockbuf.h"

#include <stdlib.h>

short _onepointer_charbuf_cpy( _charbuf_t* cbuf_dest, const char* _Source, const unsigned long start, const size_t insert_pos ) {

    if ( cbuf_dest == NULL || _Source == NULL ) return -1;

    size_t _srclength = sizeof(_Source) / sizeof(const char);
    
    size_t ibuf = insert_pos + _srclength < cbuf_dest->bufsize ? insert_pos : cbuf_dest->bufsize - insert_pos - 1;
    size_t isrc = start < _srclength ? start : 0; 

    if ( _srclength >= cbuf_dest->bufsize ) cbuf_dest = _onpointer_charbuf_init( _srclength );

    size_t cc = 0;
    for ( ; ibuf < cbuf_dest->bufsize && isrc < _srclength; ibuf++, isrc++, ++cc ) {
        cbuf_dest->buf[ibuf] = _Source[isrc];
    }

    return cc;
}

short _onepointer_charbuf_cpy_n( _charbuf_t* cbuf_dest, const char* _Source, const unsigned long start, const size_t insert_pos, const unsigned long _N ) {
    
    size_t _srclength = sizeof(_Source)/sizeof(const char);

    size_t ibuf = insert_pos + _srclength < cbuf_dest->bufsize ? insert_pos : cbuf_dest->bufsize - insert_pos - 1;
    size_t isrc = start < _srclength ? start : 0; 
    if ( isrc == _srclength || ibuf == cbuf_dest->bufsize ) return -1;

    if ( insert_pos + (_N - start) >= cbuf_dest->bufsize ) cbuf_dest = _onpointer_charbuf_init( insert_pos + (_N - start) 
                                                    + (cbuf_dest->bufsize - insert_pos)
                                                    + (size_t) (cbuf_dest->bufsize - (cbuf_dest->bufsize - insert_pos) - (insert_pos + (_N - start)) )
                                                );

    size_t cc = 0;
    for ( ; ibuf < cbuf_dest->bufsize && isrc < _srclength && cc < _N; ibuf++, isrc++, ++cc ) {
        cbuf_dest->buf[ibuf] = _Source[isrc];
    }

    return cc;

}


bool _onepointer_blockbuf_switchbuf( _blockbuf_t* _blkbuf, const unsigned int bufindex ) {
    if ( bufindex >= _blkbuf->buffercount ) return false;

    _blkbuf->buf = _blkbuf->buffers[bufindex];
    _blkbuf->bufferindex = bufindex;

    return true;
}


bool _onepointer_blockbuf_addbuf( _blockbuf_t* _blkbuf ) {

    _blkbuf->buffers[_blkbuf->buffercount] = (_charbuf_t*) malloc(sizeof(_charbuf_t*));
    _blkbuf->buffercount = _blkbuf->buffercount + 1;

    unsigned int bufidx = _blkbuf->bufferindex;
    if ( ! _onepointer_blockbuf_switchbuf(_blkbuf, _blkbuf->buffercount-1) ) return false;
    else _onepointer_blockbuf_switchbuf(_blkbuf, bufidx);

    return true;
}


bool _onepointer_blockbuf_addbufs( _blockbuf_t* _blkbuf, const unsigned int addnum_buffers ) {
    for ( unsigned int anb = 0; anb < addnum_buffers; anb++ ) {
        if ( ! _onepointer_blockbuf_addbuf( _blkbuf ) ) return false;
    }

    return true;
}


short _onepointer_blockbuf_strcpy( _blockbuf_t* _blkbuf, const char* _Source, const unsigned long start, const size_t insert_pos ) {
    return _onepointer_charbuf_cpy( _blkbuf->buf, _Source, start, insert_pos );
}


short _onepointer_blockbuf_strcpy_n( _blockbuf_t* _blkbuf, const char* _Source, const unsigned long start, const size_t insert_pos, const unsigned long _N ) {
    if ( _blkbuf->buf == 0 ) return -1;
    return _onepointer_charbuf_cpy_n( _blkbuf->buf, _Source, start, insert_pos, _N );
}


char* _word( _blockbuf_t* _blockbuf, const unsigned int index ) {
    if ( _blockbuf->buffercount <= index ) return "";
    return _blockbuf->buffers[index]->buf;
}