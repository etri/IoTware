/*
Copyright (C) <2020> <ETRI>

This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 3.0 of the License, or (at your option) any later version. This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details. You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/

#include "iw_common.h"
#include "iw_if.h"
#include "bsp_main.h"


extern unsigned int g_uart_using_info[IF_ID_MAX];

iw_error_t iw_init_uart(int port , unsigned int baudrate)
{
	if(bsp_init_uart(port , baudrate)==0)
		return IW_SUCCESS;
	else
		return IW_FAIL;
}

iw_error_t iw_deinit_uart(int port)
{
	if(bsp_deinit_uart(port) == 0)
		return IW_SUCCESS;
	else
		return IW_FAIL;
}

iw_error_t iw_putc_uart(int port, char data)
{
	return (iw_error_t)bsp_putc_uart(port, data);
}

iw_error_t iw_getc_uart(int port, char *data)
{
	return (iw_error_t)bsp_getc_uart(port, data);
}
