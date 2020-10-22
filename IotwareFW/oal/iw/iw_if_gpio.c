/*
Copyright (C) <2020> <ETRI>

This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 3.0 of the License, or (at your option) any later version. This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details. You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/

#include "iw_common.h"
#include "bsp_main.h"

void iw_config_gpio(unsigned char bank, unsigned char gpio, unsigned char dir, unsigned char pull, unsigned int speed, unsigned char input, unsigned char drive, unsigned char sense)
{
	bsp_config_gpio( bank, gpio, dir, pull, speed, input, drive, sense);
}

void iw_get_gpio(unsigned char bank, unsigned char gpio, unsigned char *val )
{
	bsp_get_gpio(bank, gpio, val );
}

void iw_set_gpio(unsigned char bank, unsigned char gpio, unsigned char val)
{
	bsp_set_gpio(bank, gpio, val);
}

void iw_toggle_gpio(unsigned char bank, unsigned char gpio)
{
	bsp_toggle_gpio(bank, gpio); 
}

void iw_init_gpio(void)
{
	bsp_init_gpio();
}



/*
	new interface
*/
void iw_init_digital(uint8_t defined_pin, uint8_t mode)
{
	bsp_init_digital(defined_pin, mode);
}

void iw_put_digital(uint8_t defined_pin, uint8_t value)
{
	bsp_put_digital(defined_pin, value);
}

uint8_t iw_get_digital(uint8_t defined_pin)
{
	return bsp_get_digital(defined_pin);
}

void iw_toggle_digital(uint8_t defined_pin)
{
	bsp_toggle_digital(defined_pin);
}
