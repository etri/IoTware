/*
Copyright (C) <2020> <ETRI>

This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 3.0 of the License, or (at your option) any later version. This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details. You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/

#include "iw_common.h"
#include "iw_oal.h"

iw_queue_t iw_create_queue(unsigned int q_size, unsigned int item_size)
{
    return kernel_create_queue(q_size, item_size);
}

void iw_delete_queue(iw_queue_t q)
{
    kernel_delete_queue(q);
}

iw_error_t iw_send_queue(iw_queue_t q, void *ptr, unsigned int tick, iw_task_t receive_task)
{
    return  kernel_send_queue(q, ptr, tick, receive_task);
}

iw_error_t iw_send_queue_isr(iw_queue_t q, void *ptr, int32_t *pWoken)
{
    return  kernel_send_queue_isr(q, ptr, pWoken);
}

iw_error_t iw_recv_queue(iw_queue_t q, void *ptr, int block)
{
    return kernel_recv_queue(q, ptr, block);
}

iw_error_t iw_peek_queue(iw_queue_t q, void *ptr, uint32_t tick)
{
    return kernel_peek_queue(q, ptr, tick);
}

iw_error_t iw_reset_queue(iw_queue_t q)
{
    return kernel_reset_queue(q);
}

