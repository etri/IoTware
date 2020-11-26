/*
Copyright (C) <2020> <ETRI>

This License and Service Agreement (LSA) applies to all works and their derivative works based on source form version and object form version of IoTware Project. Currently, the LSA for IoTware Project has two policies, 'Open Source License' and 'Commercial License'. Therefore, all works including the source code and executable code of IoTware Project and derivative works based thereon are subject to either 'Open Source License' or 'Commercial License' depending on the user's needs and purpose. Details related to the selection of the applicable license are specified in this LSA. If you use any or all of IoTware Project in any form, you are deemed to have consented to this LSA. If you breach any of the terms and conditions set forth in this LSA, you are solely responsible for any losses or damages incurred by Electronics and Communications Research Institute (ETRI), and ETRI assume no responsibility for you or any third party.
If you use the source form version or object form version of IoTware Project in whole or in part to develop a code or a derivative work, and you want to commercialize the result in some form, you will be covered under a commercial license. And if you are subject to a commercial license, the contract for the use of IoTware Project is subject to TECHNOLOGY LICENSE AGREEMENT of ETRI. You acknowledge that ETRI has all legal rights, title and interest, including intellectual property rights in the IoTware Project (regardless of whether such intellectual property rights are registered or where such rights exist) and agree with no objection thereto. Except as provided in a subsidiary agreement, nothing in this LSA grants you the right to use IoTware Project or the name, service mark, logo, domain name and other unique identification marks of ETRI.
If you use the source form version or object form version of IoTware Project in whole or in part to develop a code or a derivative work, and you do not commercialize the result in any form, you will be covered under an open source license. IoTware Project is in accordance with Free Software Foundation (FSF)'s open source policy, and is allowed to use it in the appropriate scope and manner, and you must comply with the applicable open source license policy applied to IoTware Project. IoTware Project is, in principle, subject to GNU Lesser General Public License version 2.1 (LGPLv2.1). If you have acquired all or a part of the IoTware Project in any way and it is subject to a license other than the open source license described above, please contact the following address for the technical support and other inquiries before use, and check the usage information.
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

