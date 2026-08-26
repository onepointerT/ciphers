
#include "_pyramidbuffer.h"

#include <stddef.h>
#include <stdlib.h>


_pyramidring_t* _onepointer_pyramidring_init( const size_t words, const size_t wordlength
                                            , const size_t vertical_index, const size_t horizontal_index
) {
    _pyramidring_t* pyr = (_pyramidring_t*) malloc(sizeof(_pyramidring_t));

    pyr->ringbuf = _onepointer_ringbuffer_init( words, wordlength );
    pyr->v_idx = vertical_index;
    pyr->h_idx = horizontal_index;
    pyr->leftof_nor_rightof = true;
    pyr->topof_nor_lowerof = false;
    pyr->put_where_if = NULL;
    pyr->put_original_where_if = NULL;

    return pyr;
}

bool _onepointer_pyramidring_put( const _pyramidring_t* pyr ) {
    if ( pyr->put_where_if == NULL ) return false;
    pyr->put_where_if->ring = pyr->ringbuf;
    return true;
}

bool _onepointer_pyramidring_put_original( const _pyramidring_t* pyr ) {
    if ( pyr->put_where_if == NULL ) return false;
    pyr->put_original_where_if->ring = pyr->ringbuf;
    return true;
}

_pyramidword_t* _onepointer_pyramidword_init( const size_t wordlength, const size_t words_in_ring
                                            , const size_t wordlength_in_ring, const size_t size_referencer_array
) {
    _pyramidword_t* pyw = (_pyramidword_t*) malloc(sizeof(_pyramidword_t));

    pyw->word = _onpointer_charbuf_init( wordlength );
    if ( wordlength_in_ring == 0 || words_in_ring == 0 ) pyw->ring = NULL;
    else pyw->ring = _onepointer_pyramidring_init( words_in_ring, wordlength_in_ring, 0, 0 );
    if ( size_referencer_array == 0 ) pyw->ref_out = NULL;
    else {
        pyw->ref_out = (struct _onepointer_pyramidreference**) malloc(sizeof(struct _onepointer_pyramidreference*)*size_referencer_array);
        for ( size_t r = 0; r < size_referencer_array; r++ ) { pyw->ref_out[r] = NULL; }
    }

    return pyw;
}

void _onepointer_pyramidword_resize_array( _pyramidword_t** ref_arr, const int additional_size ) {
    size_t size_ref_arr = sizeof(ref_arr) / sizeof(_pyramidword_t*);

    _pyramidword_t** ref_arr_new = (_pyramidword_t**) malloc(sizeof(_pyramidword_t*)*(size_ref_arr+additional_size));
    for ( size_t ra_idx = 0; ra_idx < size_ref_arr+additional_size; ra_idx++ ) {
        if ( ra_idx >= size_ref_arr ) ref_arr_new[ra_idx] = NULL;
        else ref_arr_new[ra_idx] = ref_arr[ra_idx];
    }

    ref_arr = ref_arr_new;
}

void _onepointer_pyramidword_addword( _pyramidword_t** ref_arr, _pyramidword_t* pyr ) {
    size_t size_ref_arr = sizeof(ref_arr) / sizeof(_pyramidword_t*);
    bool added = false;
    for( size_t ra_idx = 0; ra_idx < size_ref_arr; ra_idx++ ) {
        if ( ref_arr[ra_idx] == NULL ) { ref_arr[ra_idx] = pyr; added = true; }
    }
    if ( ! added ) {
        _onepointer_pyramidword_resize_array( ref_arr, 1 );
        ref_arr[size_ref_arr] = pyr;
    }
}


bool _onepointer_pyramidreference_type_has( const _pyramidreference_t* pref, const enum PYRAMID_REFERENCE_TYPE prt ) {
    switch ( pref->type )
    {
    case CIPHER_STREAMING:
        switch ( prt )
        {
        case MIXED_NO_CIPHER_STREAMS:
            return false;

        case CIPHER_STREAMING:
            return true;

        case MIXED:
            return true;
        
        default:
            return false;
        }
        break;
    
    case REFERENCE_WAVER:
        switch ( prt )
        {
        case MIXED_NO_REFERENCES:
            return false;

        case REFERENCE_WAVER:
            return true;

        case MIXED:
            return true;
        
        default:
            return false;
        }
        break;

    case INHERITOR:
        switch ( prt )
        {
        case MIXED_NO_INHERITOR:
            return false;

        case INHERITOR:
            return true;

        case MIXED:
            return true;
        
        default:
            return false;
        }
        break;

    case MIXED:
        switch( prt )
        {
        default:
            return true;
        }

    case MIXED_NO_CIPHER_STREAMS:
        switch ( prt )
        {
        case MIXED_NO_CIPHER_STREAMS:
            return true;

        case CIPHER_STREAMING:
            return false;

        case MIXED:
            return true;
        
        default:
            return true;
        }
        break;

    case MIXED_NO_INHERITOR:
        switch ( prt )
        {
        case MIXED_NO_INHERITOR:
            return true;

        case INHERITOR:
            return false;

        case MIXED:
            return true;
        
        default:
            return true;
        }
        break;

    case MIXED_NO_REFERENCES:
        switch ( prt )
        {
        case MIXED_NO_REFERENCES:
            return true;

        case REFERENCE_WAVER:
            return false;

        case MIXED:
            return true;
        
        default:
            return true;
        }
        break;

    default:
        break;
    }
}



size_t _onepointer_pyramidword_referencerarray_resize( _pyramidword_t* pyw, const size_t new_size_ref_out ) {
    _pyramidreference_t** ref_tmp = pyw->ref_out;
    size_t ref_tmp_size = sizeof(ref_tmp)/sizeof(_pyramidreference_t*);

    pyw->ref_out = (_pyramidreference_t**) malloc(sizeof(_pyramidreference_t*)*new_size_ref_out);
    for ( size_t r = 0; r < new_size_ref_out; r++ ) {
        if ( r < ref_tmp_size ) pyw->ref_out[r] = ref_tmp[r];
        else pyw->ref_out[r] = NULL;
    }
}

_pyramidreference_t* _onepointer_pyramidreference_init( const size_t wordlength, const enum PYRAMID_REFERENCE_TYPE ref_type ) {
    _pyramidreference_t* pyrf = (_pyramidreference_t*) malloc(sizeof(_pyramidreference_t));

    if ( wordlength == 0 ) {
        pyrf->origin = NULL;
        pyrf->dest = NULL;
    } else {
        pyrf->origin = _onepointer_pyramidword_init( wordlength, 0, 0, 0 );
        pyrf->dest = (_pyramidreference_t**) malloc(sizeof(_pyramidreference_t*));
    }
    pyrf->pass = NULL;
    pyrf->type = ref_type;

    return pyrf;
}

void _onepointer_pyramidreference_resize_array( _pyramidreference_t** ref_arr, const int additional_size ) {
    size_t size_ref_arr = sizeof(ref_arr) / sizeof(_pyramidreference_t*);
    
    _pyramidreference_t** ref_arr_new = (_pyramidreference_t**) malloc(sizeof(_pyramidreference_t*)*(size_ref_arr+additional_size));
    for ( size_t pr_idx = 0; pr_idx < size_ref_arr+additional_size; pr_idx++ ) {
        if ( pr_idx >= size_ref_arr ) ref_arr_new[pr_idx] = NULL;
        else ref_arr_new[pr_idx] = ref_arr[pr_idx];
    }

    ref_arr = ref_arr_new;
}
void _onepointer_pyramidreference_addref( _pyramidreference_t** ref_arr, _pyramidreference_t* pyr ) {
    size_t size_ref_arr = sizeof(ref_arr) / sizeof(_pyramidreference_t*);
    for ( size_t ra = 0; ra < size_ref_arr; ra++ ) {
        if ( ref_arr[ra] == NULL ) { ref_arr[ra] = pyr; return; }
    }
    _onepointer_pyramidreference_resize_array( ref_arr, 1 );
    ref_arr[size_ref_arr] = pyr;
}


void _onepointer_pyramidroute_resize_array( _pyramidreference_t*** ref_arr, const size_t additional_size ) {
    size_t size_ref_arr = sizeof(ref_arr) / sizeof(_pyramidreference_t**);

    _pyramidreference_t*** ref_arr_new = (_pyramidreference_t***) malloc(sizeof(_pyramidreference_t**)*(size_ref_arr+additional_size));
    for ( size_t pr_idx = 0; pr_idx < size_ref_arr+additional_size; pr_idx++ ) {
        if ( pr_idx >= size_ref_arr ) ref_arr_new[pr_idx] = NULL;
        else ref_arr_new[pr_idx] = ref_arr[pr_idx];
    }

    ref_arr = ref_arr_new;
}

void _onepointer_pyramidroute_addroute_ref( _pyramidreference_t*** ref_arr, _pyramidreference_t** pref_route ) {
    size_t size_ref_arr = sizeof(ref_arr) / sizeof(_pyramidreference_t**);
    for ( size_t ra = 0; ra < size_ref_arr; ra++ ) {
        if ( ref_arr[ra] == NULL ) { ref_arr[ra] = pref_route; return; }
    }
    _onepointer_pyramidroute_resize_array( ref_arr, 1 );
    ref_arr[size_ref_arr] = pref_route;
}


_word_pyramidbuf_t* _onepointer_pyramidbuffer_init( const enum PYRAMID_SHAPE pys, const size_t wordlength
                                        , const size_t height_rows, const size_t words_floor_row
                                        , const size_t words_in_ring
                                        , const ascii_t shape_or_symetric_with_NULL

) {
    _word_pyramidbuf_t* pyb = (_word_pyramidbuf_t*) malloc(sizeof(_word_pyramidbuf_t));

    bool from_ascii = shape_or_symetric_with_NULL != NULL;
    pyb->words = from_ascii ? NULL : _onepointer_pyramidword_init( wordlength, words_in_ring, wordlength, 0 );
    pyb->wordlength = wordlength;
    pyb->num_words_floor = words_floor_row;
    pyb->num_words_uppside = pys == PYRAMID_WITH_STRAIGHT_ROOF ? 0 : 1;
    pyb->num_rows = height_rows;
    pyb->gap_size_uppside = pys == PYRAMID_WITH_STRAIGHT_ROOF
                        || pys != PYRAMID_WITH_GAPS ? 0 : 1;
    pyb->if_ascii = shape_or_symetric_with_NULL;
    pyb->symetric = pyb->if_ascii == NULL;
    pyb->inheriting = false;
    pyb->shifted = false;
    pyb->pshape = pys;
    pyb->wordbuf = (word_t**) malloc(sizeof(word_t*)*ONEPOINTER_PYRAMIDBUFFER_SIZE_TMPBUFFERS);
    pyb->wordbuf_size = ONEPOINTER_PYRAMIDBUFFER_SIZE_TMPBUFFERS;
    pyb->ringbuf = (_word_ringbuf_t**) malloc(sizeof(_word_ringbuf_t*)*ONEPOINTER_PYRAMIDBUFFER_SIZE_TMPBUFFERS);
    pyb->ringbuf_size = ONEPOINTER_PYRAMIDBUFFER_SIZE_TMPBUFFERS;
    pyb->inheritors = NULL;
    pyb->cipher_streams = NULL;
    pyb->reference_wavers = NULL;
    pyb->graph_start = NULL;
    pyb->route = NULL;
    pyb->shifting_algo = NONE;
    pyb->ring_algo = NONE;

    return pyb;
}

_pyramidword_t** _onepointer_pyramidbuffer_row( _word_pyramidbuf_t* wpyb, const size_t h_idx ) {
    if ( h_idx >= wpyb->num_rows ) return NULL;
    return wpyb->words[h_idx];
}
_pyramidword_t* _onepointer_pyramidbuffer_word( _word_pyramidbuf_t* wpyb, const size_t h_idx, const size_t v_idx ) {
    _pyramidword_t** pyr = _onepointer_pyramidbuffer_row( wpyb, h_idx );
    if ( pyr == NULL ) return NULL;
    else if ( sizeof(pyr)/sizeof(_pyramidword_t*) < v_idx ) return NULL;
    return pyr[v_idx];
}
_pyramidword_t* _onepointer_pyramidbuffer_word_of_row( _pyramidword_t** pyw_row, const size_t v_idx ) {
    if ( sizeof(pyw_row)/sizeof(_pyramidword_t*) < v_idx ) return NULL;
    return pyw_row[v_idx];
}
bool _onepointer_pyramidbuffer_reference_from_to( _word_pyramidbuf_t* wpyb, _pyramidword_t* wfrom, _pyramidword_t* wto
                                                , const enum PYRAMID_REFERENCE_TYPE prt
) {
    _pyramidreference_t* pref = _onepointer_pyramidreference_init( wpyb->wordlength, prt );
    pref->origin = wfrom;
    pref->dest = wto;
    switch ( prt )
    {
    case CIPHER_STREAMING:
        _onepointer_pyramidreference_addref( wpyb->cipher_streams, pref );
        break;

    case REFERENCE_WAVER:
        _onepointer_pyramidreference_addref( wpyb->reference_wavers, pref );
        break;

    case INHERITOR:
        _onepointer_pyramidreference_addref( wpyb->inheritors, pref );
        break;
    
    default:
        return false;
    }

    _onepointer_pyramidreference_addref( wfrom->ref_out, pref );

    return true;
}

const _pyramidreference_t** _onepointer_pyramidreference_find_incoming_pword( const _pyramidword_t* pword, const _word_pyramidbuf_t* wpyb
                                                                    , const enum PYRAMID_REFERENCE_TYPE prt
) {
    if ( pword->ref_in == NULL ) return NULL;
    size_t arr_size = sizeof(pword->ref_in) / sizeof(_pyramidreference_t*);
    if ( arr_size == 0 ) return NULL;
    _pyramidreference_t** pyr_arr = (_pyramidreference_t**) malloc(sizeof(_pyramidreference_t*)*arr_size);
    for ( unsigned int i = 0; i < arr_size; i++ ) { pyr_arr[i] = NULL; }

    for ( size_t i = 0; i < arr_size; i++ ) {
        _pyramidreference_t* pyr = pword->ref_in[i];
        if ( pyr == NULL ) break;
        else {
            switch ( prt )
            {
            case MIXED_NO_INHERITOR:
                if ( pyr->type != INHERITOR ) pyr_arr[i] = pyr;
                break;
            
            case MIXED_NO_REFERENCES:
                if ( pyr->type != REFERENCE_WAVER ) pyr_arr[i] = pyr;
                break;

            case MIXED_NO_CIPHER_STREAMS:
                if ( pyr->type != CIPHER_STREAMING ) pyr_arr[i] = pyr;
                break;

            case INHERITOR:
                if ( pyr->type == INHERITOR ) pyr_arr[i] = pyr;
                break;

            case CIPHER_STREAMING:
                if ( pyr->type == CIPHER_STREAMING ) pyr_arr[i] = pyr;
                break;

            case REFERENCE_WAVER:
                if ( pyr->type == REFERENCE_WAVER ) pyr_arr[i] = pyr;
                break;

            default:
                pyr_arr[i] = pyr;
                break;
            }
        }
    }

    return pyr_arr;
}



const _pyramid_route_array_t _onepointer_pyramidreference_find_incoming_pword_from( const _pyramidword_t* pword, const _pyramidword_t* wfrom
                                                                                  , const _word_pyramidbuf_t* wpyb
                                                                                  , const enum PYRAMID_REFERENCE_TYPE prt
) {
    if ( pword == NULL || wfrom == NULL || pword == wfrom ) return NULL;
    _pyramidreference_t** pyr_incoming = _onepointer_pyramidreference_find_incoming_pword( pword, wpyb, prt );
    if ( pyr_incoming == NULL ) return NULL;
    size_t c_incoming = sizeof(pyr_incoming) / sizeof(_pyramidreference_t*);
    
    _pyramid_route_array_t pyrou_arr = (_pyramid_route_array_t) malloc(sizeof(_pyramidreference_t**)*c_incoming);
    for ( size_t i = 0; i < c_incoming; i++ ) { pyrou_arr[i] = NULL; }
    size_t pyrou_idx = 0;

    // Use the incoming `_pyramidword_t*` nodes for searching a path to `wfrom`
    _pyramidword_t* w_dest = pword;
    for ( size_t i = 0; i < c_incoming; i++ ) {
        _pyramidword_t* w_in_from = pyr_incoming[i]->origin;
        if ( w_in_from == NULL ) continue;

        _pyramidreference_t**  pyr_tmp_arr = (_pyramidreference_t**) malloc(sizeof(_pyramidreference_t*)*wpyb->num_rows);
        for ( size_t i = 0; i < wpyb->num_rows; i++ ) { pyr_tmp_arr[i] = NULL; }

        if ( w_in_from == wfrom ) continue;
        // Try all paths from `pyr_incoming[i]->origin->ref_in`
        _pyramidreference_t** w_in_from_ref_in = _onepointer_pyramidreference_find_incoming_pword( w_in_from, wpyb, prt );
        if ( w_in_from_ref_in == NULL ) return NULL;
        size_t w_in_from_size = sizeof(w_in_from_ref_in) / sizeof(_pyramidreference_t*);
        for ( size_t w = 0; w < w_in_from_size; w++ ) {
            _pyramidword_t* w_in_from_before = w_in_from_ref_in[w];
            if ( w_in_from_before == NULL ) continue;

            _pyramidreference_t*** w_pyrou_arr = _onepointer_pyramidreference_find_incoming_pword_from( w_in_from_before, wfrom, wpyb, prt );
            if ( w_pyrou_arr == NULL ) continue;
            size_t w_pyrou_arr_size = sizeof(w_pyrou_arr) / sizeof(_pyramidreference_t**);

            // Control route path, if they are really to `wfrom`
            for ( size_t wifb = 0; wifb < w_pyrou_arr_size; wifb++ ) {
                _pyramidreference_t** wr_current = w_pyrou_arr[wifb];
                size_t wr_current_size = sizeof(wr_current) / sizeof(_pyramidreference_t*);

                // Control reference graph, if its really continuous to `wfrom` when using `wr_current[wr]->origin`
                _pyramidword_t* w_in_from_current = wr_current[0]->origin;
                if ( w_in_from_current == NULL ) continue;
                bool found_route = true;
                size_t wr = 1;
                while ( w_in_from_current != wfrom && wr < wr_current_size ) {
                    if ( w_in_from == NULL ) { found_route = false; break; }
                    else {
                        w_in_from_current = wr_current[wr]->origin;
                        ++wr;
                    }
                }

                if ( found_route ) {
                    pyrou_arr[pyrou_idx++] = wr_current;
                }
            }   
        }
    }

    return pyrou_arr;
}


const _pyramidreference_t*** _onepointer_pyramidreference_find_incoming( const _pyramidword_t* pword, const _word_pyramidbuf_t* wpyb
                                                                      , const _pyramidreference_t* pref_current, const _pyramidword_t* wfrom
                                                                      , const enum PYRAMID_REFERENCE_TYPE* prt
) {
    _pyramidreference_t*** pyr_arr = (_pyramidreference_t***) malloc(sizeof(_pyramidreference_t**)
                                                                    *(sizeof(pword->ref_in)/sizeof(_pyramidreference_t*))
                                                                );

    _pyramid_route_array_t pyrou_arr = _onepointer_pyramidreference_find_incoming_pword_from( pword, wfrom, wpyb, prt );
    size_t pyrou_arr_size = sizeof(pyrou_arr) / sizeof(_pyramidreference_t**);
    for ( size_t p = 0; p < pyrou_arr_size; p++ ) {
        _pyramid_route_t pyrou = pyrou_arr[p];
        size_t pyrou_size = sizeof(pyrou) / sizeof(_pyramidreference_t*);

        bool includes_pref_current = false;
        _pyramidreference_t* ref_out = NULL;
        _pyramidreference_t* ref_pword_in = NULL;
        for ( size_t r = 0; p < pyrou_size; p++ ) {
            _pyramidreference_t* pyr = pyrou[r];
            if ( pyr->origin == wfrom ) ref_out = pyr;
            if ( pyr == pref_current ) includes_pref_current = true;

            size_t dest_size = sizeof(pyr->dest) / sizeof(_pyramidword_t*);
            for ( size_t d = 0; d < dest_size; d++ ) {
                _pyramidword_t* pyw = pyr->dest[d];
                if ( pyw == pword ) ref_pword_in = pyr;
            }
        }

        if ( includes_pref_current && ref_out != NULL && ref_pword_in != NULL ) {
            _pyramidreference_t** pyref_new = NULL;
            if ( pref_current == ref_pword_in && pref_current == ref_out ) {
                pyref_new = (_pyramidreference_t**) malloc(sizeof(_pyramidreference_t*));
                _onepointer_pyramidreference_addref( pyref_new, pref_current );
            } else if ( pref_current == ref_pword_in || pref_current == ref_out ) {
                pyref_new = (_pyramidreference_t**) malloc(sizeof(_pyramidreference_t*)*2);
                if ( pref_current != ref_pword_in ) _onepointer_pyramidreference_addref( pyref_new, ref_pword_in );
                else if ( pref_current != ref_out ) _onepointer_pyramidreference_addref( pyref_new, ref_out );
                _onepointer_pyramidreference_addref( pyref_new, pref_current );
            } else {
                pyref_new = (_pyramidreference_t**) malloc(sizeof(_pyramidreference_t*)*3);
                _onepointer_pyramidreference_addref( pyref_new, ref_out );
                _onepointer_pyramidreference_addref( pyref_new, pref_current );
                _onepointer_pyramidreference_addref( pyref_new, ref_pword_in );
            }
            _onepointer_pyramidroute_addroute_ref( pyr_arr, pyref_new );
        }
    }

    return pyr_arr;
}


_pyramid_route_t _onepointer_pyramidbuffer_get_route( const _word_pyramidbuf_t* wpyb, const _pyramidword_t* wto
                                                    , const _pyramidword_t* tfrom, const enum PYRAMID_REFERENCE_TYPE prt
) {
    _pyramid_route_array_t pyroute_arr = _onepointer_pyramidreference_find_incoming_pword_from( wto, tfrom, wpyb, prt );
    size_t pyroute_arr_size = sizeof(pyroute_arr) / sizeof(_pyramid_route_t);

    for ( size_t r = 0; r < pyroute_arr_size; r++ ) {
        _pyramid_route_t pyroute = pyroute_arr[r];
        size_t pyroute_len = sizeof(pyroute) / sizeof(_pyramidreference_t*);
        if ( pyroute_len == 0 ) continue;

        for ( size_t rr = 0; rr < pyroute; rr++ ) {
            _pyramidreference_t* pyr = pyroute[rr];
            if ( _onepointer_pyramidreference_type_has( pyr, prt ) ) {
                _onepointer_pyramidreference_addref( pyroute, pyr );
            }
        }

        _onepointer_pyramidroute_addroute_ref( pyroute_arr, pyroute );
    }

    return pyroute_arr;
}



const _pyramid_route_array_t _onepointer_pyramidreference_get_routes_to( const _word_pyramidbuf_t* wpyb, const _pyramidword_t* wto
                                                                        , const _pyramidword_t* tfrom, const enum PYRAMID_REFERENCE_TYPE prt
) {
    _pyramid_route_array_t pyrarr = (_pyramid_route_array_t) malloc(sizeof(_pyramid_route_t)*7);

    size_t tfrom_refout_size = sizeof(tfrom->ref_out) / sizeof(_pyramidreference_t*);
    for ( size_t to = 0; to < tfrom_refout_size; to++ ) {
        _pyramidreference_t* pyr = tfrom->ref_out[to];
        size_t r_out_dest_size = sizeof( pyr->dest ) / sizeof(_pyramidword_t*);
        for ( size_t rd = 0; rd < r_out_dest_size; rd++ ) {
            _pyramidword_t* pword = pyr->dest[rd];

            _pyramid_route_array_t pyrarr_continuing = _onepointer_pyramidreference_get_routes_to( wpyb, wto, pword );
            size_t pyrarr_continuing_size = sizeof(pyrarr_continuing) / sizeof(_pyramid_route_t);
            for ( size_t pr = 0; pr < pyrarr_continuing_size; pr++ ) {
                _pyramid_route_t proute = pyrarr_continuing[pr];
                _onepointer_pyramidreference_addref( proute, pword );
                _onepointer_pyramidroute_addroute_ref( pyrarr, proute );
            }
        }
    }
    
    return pyrarr;
}



const _pyramid_route_array_t _onepointer_pyramidreference_get_routes_from( const _word_pyramidbuf_t* wpyb, const _pyramidword_t* wto
                                                                        , const _pyramidword_t* tfrom, const enum PYRAMID_REFERENCE_TYPE prt
) {
    _pyramid_route_array_t pyroute_arr = (_pyramid_route_array_t) malloc(sizeof(_pyramid_route_t)*7);

    size_t wto_refin_size = sizeof(wto->ref_in) / sizeof(_pyramidreference_t*);
    for ( size_t from = 0; from < wto_refin_size; from++ ) {
        _pyramidreference_t* pyr = wto->ref_in[from];
        if ( pyr == NULL || ! _onepointer_pyramidreference_type_has( pyr, prt ) ) continue;
        _pyramidword_t* pword = pyr->origin;

        _pyramid_route_array_t pyroute_arr_pword = _onepointer_pyramidreference_get_routes_from( wpyb, pword, tfrom, prt );
        size_t pyroute_arr_pword_size = sizeof(pyroute_arr_pword) / sizeof(_pyramid_route_t);
        for ( size_t pr = 0; pr < pyroute_arr_pword_size; pr++ ) {
            _pyramid_route_t proute = pyroute_arr_pword[pr];
            _onepointer_pyramidreference_addref( proute, pyr );
            _onepointer_pyramidroute_addroute_ref( pyroute_arr, proute );
        }
    }

    return pyroute_arr;
}
