/*
Copyright (C) <2020> <ETRI>

This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 3.0 of the License, or (at your option) any later version. This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details. You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/

#include "interface_gpio.h"

void config_gpio(unsigned char bank, unsigned char gpio, unsigned char dir, unsigned char pull, unsigned int speed, unsigned char input, unsigned char drive, unsigned char sense)
{
	iw_config_gpio( bank, gpio, dir, pull, speed, input, drive, sense);
}

void get_gpio(unsigned char bank, unsigned char gpio, unsigned char *val )
{
	iw_get_gpio( bank, gpio, val );
}

void set_gpio(unsigned char bank,  unsigned char  gpio, unsigned char val)
{
	iw_set_gpio( bank,  gpio, val);
}

void toggle_gpio(unsigned char bank,  unsigned char  gpio)
{
	iw_toggle_gpio( bank,  gpio);
}


void init_digital(uint8_t defined_pin, uint8_t mode)
{
	iw_init_digital(defined_pin, mode);
}

void put_digital(uint8_t defined_pin, uint8_t value)
{
	iw_put_digital(defined_pin, value);
}

uint8_t get_digital(uint8_t defined_pin)
{
	return iw_get_digital(defined_pin);
}

void toggle_digital(uint8_t defined_pin)
{
	iw_toggle_digital(defined_pin);
}
