/*
Copyright (C) <2020> <ETRI>

This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 3.0 of the License, or (at your option) any later version. This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details. You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/

#include "iw_common.h"
#include "iw_oal.h"



iw_error_t iw_create_sem(iw_sem_t *sem, unsigned int max_count, unsigned int init_count)
{
	*sem = kernel_create_sem(max_count, init_count);

    return (*sem == NULL) ? IW_FAIL : IW_SUCCESS;
}

void iw_delete_sem(iw_sem_t sem)
{
    kernel_delete_sem(sem);
}

iw_error_t iw_lock_sem(iw_sem_t sem, unsigned int tick)
{
    return kernel_lock_sem(sem, tick);
}

iw_error_t iw_unlock_sem(iw_sem_t sem, iw_task_t receive_task)
{
    return kernel_unlock_sem(sem, receive_task);
}

iw_error_t iw_unlock_sem_isr(iw_sem_t sem, int32_t *pWoken)
{
    return kernel_unlock_sem_isr(sem, pWoken);
}
