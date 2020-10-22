
/*
Copyright (C) <2020> <ETRI>

This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 3.0 of the License, or (at your option) any later version. This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details. You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/
#include "iw_common.h"
#include "iw_oal.h"

iw_error_t iw_create_mutex(iw_mutex_t *mutex)
{
    *mutex = kernel_create_mutex();

    return (*mutex == 0) ? IW_FAIL : IW_SUCCESS;
}

void iw_delete_mutex(iw_mutex_t mutex)
{
    kernel_delete_mutex(mutex);
}

iw_error_t iw_lock_mutex(iw_mutex_t mutex)
{
    return kernel_lock_mutex(mutex);
}

iw_error_t iw_lock_mutex_ms(iw_mutex_t mutex, unsigned int ms_to_wait)
{
    return kernel_lock_mutex_ms(mutex, ms_to_wait);
}

iw_error_t iw_unlock_mutex(iw_mutex_t mutex)
{
    return kernel_unlock_mutex(mutex);
}
