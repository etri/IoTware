/*
Copyright (C) <2020> <ETRI>

This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 3.0 of the License, or (at your option) any later version. This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details. You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/

#include "iw_common.h"
#include "iw_oal.h"
#include "iw_bsp.h"

#define IW_PUT_FIFO_NEXT(pfifo)        ((pfifo->put_pos + 1) % pfifo->block_count)
#define IW_GET_FIFO_NEXT(pfifo)        ((pfifo->get_pos + 1) % pfifo->block_count)
#define IW_PUT_FIFO_PTR(pfifo)         (pfifo->block_pdata + (pfifo->put_pos * pfifo->block_size))
#define IW_GET_FIFO_PTR(pfifo)         (pfifo->block_pdata + (pfifo->get_pos * pfifo->block_size))


iw_error_t iw_init_fifo( iw_fifo_t *pfifo, void *pdata, unsigned int size, unsigned int count )
{
    pfifo->put_pos = 0;
    pfifo->get_pos = 0;
    pfifo->block_size  = size;
    pfifo->block_count = count;
    pfifo->block_pdata = (uint8_t *) pdata;
    return IW_SUCCESS;
}

static inline void inline_memcpy(void *d, void *s, size_t len)
{
    #define BASE_INTTYPE    int32_t
    register BASE_INTTYPE *dx = (BASE_INTTYPE *)d;
    register BASE_INTTYPE *sx = (BASE_INTTYPE *)s;
    while (len >= sizeof(BASE_INTTYPE))
    {
        *dx++ = *sx++;
        len -= sizeof(BASE_INTTYPE);
    }
    if (len > 0)
    {
        register int8_t *d8 = (int8_t *)dx;
        register int8_t *s8 = (int8_t *)sx;
        do
        {
            *d8++ = *s8++;
            len -= sizeof(uint8_t);
        } while (len > 0);
    }
}

iw_error_t iw_put_fifo( iw_fifo_t *pfifo, void *pdata )
{
    int put_next_pos = IW_PUT_FIFO_NEXT( pfifo );
    if( pfifo->get_pos != put_next_pos )
    {
        inline_memcpy( IW_PUT_FIFO_PTR( pfifo ), pdata, pfifo->block_size );
        pfifo->put_pos = put_next_pos;
        return IW_SUCCESS; // ok
    }
    return IW_FAIL; // full
}

iw_error_t iw_get_fifo( iw_fifo_t *pfifo, void *pdata )
{
    if( pfifo->put_pos != pfifo->get_pos )
    {
        inline_memcpy( pdata, IW_GET_FIFO_PTR( pfifo ), pfifo->block_size );
        pfifo->get_pos = IW_GET_FIFO_NEXT( pfifo );
        return IW_SUCCESS; // ok
    }
    return IW_FAIL; // empty
}

unsigned int iw_count_fifo( iw_fifo_t *pfifo )
{
    if( pfifo->put_pos > pfifo->get_pos )
    {
        return pfifo->put_pos - pfifo->get_pos;
    }
    if( pfifo->get_pos != pfifo->put_pos )
    {
        return (pfifo->put_pos + pfifo->block_count) - pfifo->get_pos;
    }
    return 0; // empty
}

iw_error_t iw_flush_fifo( iw_fifo_t *pfifo )
{
    pfifo->get_pos = pfifo->put_pos;
    return IW_SUCCESS; // ok
}

