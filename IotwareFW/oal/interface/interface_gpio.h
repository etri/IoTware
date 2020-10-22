/*
Copyright (C) <2020> <ETRI>

This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 3.0 of the License, or (at your option) any later version. This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details. You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/

#ifndef INTERFACE_GPIO_H
#define INTERFACE_GPIO_H


#include "iw_common.h"
#include "iw_if.h"

void config_gpio(unsigned char bank, unsigned char pin_number, unsigned char dir, unsigned char pull, unsigned int speed, unsigned char input, unsigned char drive, unsigned char sense);
void get_gpio(unsigned char bank, unsigned char gpio, unsigned char *val );
void set_gpio(unsigned char bank,  unsigned char  gpio, unsigned char val);
void toggle_gpio(unsigned char bank,  unsigned char  gpio);


void init_digital(uint8_t defined_pin, uint8_t mode);
void put_digital(uint8_t defined_pin, uint8_t value);
uint8_t get_digital(uint8_t defined_pin);
void toggle_digital(uint8_t defined_pin);

#endif /* INTERFACE_GPIO_H */

