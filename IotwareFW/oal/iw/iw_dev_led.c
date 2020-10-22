/*
Copyright (C) <2020> <ETRI>

This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 3.0 of the License, or (at your option) any later version. This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details. You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/


#include "iw_dev_led.h"


typedef struct{
	unsigned char used;
	unsigned char pin;
	//unsigned char bank;
	//unsigned char gpio;
}led_info_t;

led_info_t gLedInfo[LED_MAX];

iw_error_t iw_register_led(led_color_t color, uint8_t defined_pin)
{
	if(color >= LED_MAX){
		printf("Check Led color\n");
		return IW_FAIL;
	}
	gLedInfo[color].used = 1;
	//gLedInfo[color].bank = bank;
	//gLedInfo[color].gpio = gpio;
	gLedInfo[color].pin = defined_pin;

	//iw_config_gpio(bank, gpio, 1, 0, 0, 0, 0, 0);
	iw_init_digital(defined_pin, DIGITAL_OUTPUT_PUSHPULL);
	return IW_SUCCESS;
}

iw_error_t iw_led_on(led_color_t color)
{
	if(color >= LED_MAX){
		printf("Check Led(%d)\n", color);
		return IW_FAIL;
	}
	if(gLedInfo[color].used == 0)
		return IW_FAIL;
	
	//iw_set_gpio(gLedInfo[color].bank, gLedInfo[color].gpio, 0);
	iw_put_digital(gLedInfo[color].pin, DIGITAL_LOW);
	return IW_SUCCESS;
}

iw_error_t iw_led_off(led_color_t color)
{
	if(color >= LED_MAX){
		printf("Check Led\n");
		return IW_FAIL;
	}
	if(gLedInfo[color].used == 0)
		return IW_FAIL;
	//iw_set_gpio(gLedInfo[color].bank, gLedInfo[color].gpio, 1);
	iw_put_digital(gLedInfo[color].pin, DIGITAL_HIGH);
	return IW_SUCCESS;
}

iw_error_t iw_led_toggle(led_color_t color)
{
	if(color >= LED_MAX){
		printf("Check Led\n");
		return IW_FAIL;
	}
	if(gLedInfo[color].used == 0)
		return IW_FAIL;

	//iw_toggle_gpio(gLedInfo[color].bank, gLedInfo[color].gpio);
	iw_toggle_digital(gLedInfo[color].pin);
	return IW_SUCCESS;
}
