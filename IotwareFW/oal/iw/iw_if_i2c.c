/*
Copyright (C) <2020> <ETRI>

This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 3.0 of the License, or (at your option) any later version. This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details. You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/

#include "iw_common.h"
#include "iw_dev.h"
#include "iw_if.h"
#include "bsp_main.h"

iw_error_t iw_init_i2c( unsigned char port )
{
	iw_error_t ret = IW_FAIL;

	if(bsp_init_i2c( port ) == 0) {
		//iw_printf("iw_init_i2c(%d) ok\n", port);
		ret = IW_SUCCESS;
	}
	else {
		iw_printf("iw_init_i2c(%d) err\n", port);
		ret = IW_FAIL;
	}

	return ret;
}

iw_error_t iw_deinit_i2c( unsigned char port )
{
    if(bsp_deinit_i2c( port )==0)
	    return IW_SUCCESS;
    else
	    return IW_FAIL;
}

iw_error_t iw_transfer_i2c(iw_i2c_msg *msg, int num)
{
	iw_error_t ret = IW_FAIL;

	if(bsp_xfer_i2c(msg, num) == 0) {
		ret = IW_SUCCESS;
		//iw_printf("iw_transfer_i2c(%d) ok\n", msg->port);
	}
	else {
		iw_printf("iw_transfer_i2c(%d) err\n", msg->port);
	}

	return ret;
}

