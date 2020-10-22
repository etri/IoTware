/*
Copyright (C) <2020> <ETRI>

This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 3.0 of the License, or (at your option) any later version. This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details. You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/
#include "iw_common.h"
#include "iw_oal.h"

iw_tick_t iw_get_tick_count(void)
{
    return kernel_get_tick_count();
}

unsigned int iw_get_time_ms(void)
{
    return (kernel_get_time_ms() * IW_TICK_PERIOD_MS);
}


void iw_sleep(unsigned int ms)
{
    kernel_sleep(ms);
}
