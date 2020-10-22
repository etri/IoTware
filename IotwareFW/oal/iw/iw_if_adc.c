/*
Copyright (C) <2020> <ETRI>

This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 3.0 of the License, or (at your option) any later version. This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details. You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/

#include "iw_common.h"
#include "bsp_main.h"
#include "iw_if.h"


extern iw_adc_info_t g_adc_using_info[USE_ADC_MAX];

iw_error_t iw_init_adc(uint8_t id, uint8_t defined_pin)
{
	//iw_printf("iw_init_adc(%d, %d)\n", id, defined_pin);
	//return bsp_init_adc(id, 0, 0);
	return bsp_init_analog(id, defined_pin);
}

iw_error_t iw_deinit_adc(uint8_t id)
{
    return bsp_deinit_adc(id);
}


iw_error_t iw_get_voltage(uint8_t id , uint16_t *val)
{
    return bsp_get_voltage(id, val);
}

iw_error_t iw_get_adc(uint8_t id , uint16_t *val)
{
	*val = bsp_get_analog(id);
    return IW_SUCCESS;
}
