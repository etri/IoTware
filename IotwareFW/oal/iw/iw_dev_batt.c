/*
Copyright (C) <2020> <ETRI>

This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 3.0 of the License, or (at your option) any later version. This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details. You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/


#include "iw_dev_batt.h"

static unsigned char gBattAdcPort = INVALID_BATT_PORT;

extern iw_adc_info_t g_adc_using_info[USE_ADC_MAX];

void iw_register_battery(if_type_t type, if_id_t port, uint8_t defined_pin)
{
	if(type == IF_TYPE_ADC){
		if(g_adc_using_info[0].use == true){
			iw_printf("Check ADC port\n");
			return;
		}
		g_adc_using_info[0].use = true; 
		g_adc_using_info[0].bank = 0;
		g_adc_using_info[0].gpio = defined_pin;
		gBattAdcPort = port;
	}
}

void iw_init_battery(void)
{
	//iw_printf("iw_init_battery()\n");
	if(g_adc_using_info[0].use == true) {
		iw_init_adc(gBattAdcPort, g_adc_using_info[0].gpio);
	}
}

void iw_deinit_battery(void)
{
	//iw_printf("iw_deinit_battery()");
	if(g_adc_using_info[0].use == true) {
		iw_deinit_adc(gBattAdcPort);
	}
}

iw_error_t iw_get_battery_voltage(uint16_t *val)
{
	iw_error_t ret_val = IW_FAIL;
	if(gBattAdcPort != INVALID_BATT_PORT) {
		ret_val = iw_get_voltage((if_id_t)gBattAdcPort, val);
	}
	return ret_val;
}

iw_error_t iw_get_battery_adc(uint16_t *val)
{
	iw_error_t ret_val = IW_FAIL;
	if(gBattAdcPort != INVALID_BATT_PORT) {
		ret_val = iw_get_adc((if_id_t)gBattAdcPort, val);
	}
	return ret_val;
}



