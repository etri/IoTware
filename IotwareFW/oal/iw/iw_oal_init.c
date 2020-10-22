/*
Copyright (C) <2020> <ETRI>

This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 3.0 of the License, or (at your option) any later version. This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details. You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/
#include "iw_common.h"
#include "iw_oal.h"
#include "iw_bsp.h"

void iw_init_os(void)
{
    kernel_init_os();
	//iw_delay_ms(100);
}

char *iw_os_name(void)
{
    return kernel_name();
}

void iw_start_scheduler (void)
{
    kernel_start_scheduler();
}

void iw_yield (void)
{
    kernel_yield();
}

void iw_yield_isr (int arg)
{
    kernel_yield_isr(arg);
}

void iw_enter_critical(void)
{
    kernel_enter_critical();
}

void iw_exit_critical(void)
{
    kernel_exit_critical();
}
