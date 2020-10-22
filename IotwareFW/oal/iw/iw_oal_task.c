/*
Copyright (C) <2020> <ETRI>

This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 3.0 of the License, or (at your option) any later version. This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details. You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/

#include "iw_common.h"
#include "iw_oal.h"

iw_task_t iw_create_task(iw_task_fn_t fn,const char *name, unsigned int stack_size, void *arg,   
                         unsigned int priority, void *ext)
{
    iw_task_t task = 0;

    kernel_create_task(fn, name, stack_size, arg, priority, &task);
    //iw_printf("%s:x%x\n",name, task);

    return task;
}

void iw_delete_task(iw_task_t task)
{
    kernel_delete_task(task);
}

void iw_exit_task(void)
{
#if (IW_FREERTOS==1)
    while(1){
       iw_sleep(1000000);
    }
#endif
}

void iw_suspend_task(iw_task_t task)
{
    kernel_suspend_task(task);
}

void iw_resume_task(iw_task_t task)
{
    kernel_resume_task(task);
}

iw_error_t iw_get_task_handle(iw_task_t *handle)
{
    return kernel_get_task_handle(handle);
}

iw_error_t iw_get_task_free_stack(iw_task_t handle, unsigned int *free_stack) 
{
    return kernel_get_task_free_stack(handle, free_stack);
}

iw_error_t iw_signal_task(iw_task_t task, uint32_t bits, int32_t is_isr) 
{
    return kernel_signal_task(task, bits, is_isr); 
}

uint32_t iw_wait_signal(uint32_t bits_to_wait, uint32_t tick) 
{
    return kernel_wait_signal(bits_to_wait, tick); 
}
