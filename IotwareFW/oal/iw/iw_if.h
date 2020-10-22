/*
Copyright (C) <2020> <ETRI>

This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 3.0 of the License, or (at your option) any later version. This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details. You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/
#ifndef IW_IF_H
#define IW_IF_H


#include "iw_bsp.h"
//#include "bsp_main.h"


#define USE_ADC_MAX	3

typedef enum{
	IF_TYPE_I2C		= 1,
 	IF_TYPE_UART,
 	IF_TYPE_SPI,
 	IF_TYPE_GPIO, 
 	IF_TYPE_ADC
}if_type_t;

typedef enum
{
	IF_ID_0		= 0,
 	IF_ID_1,
 	IF_ID_2,
 	IF_ID_3,
 	IF_ID_4,
 	IF_ID_MAX, 	
}if_id_t;

#if 0
typedef struct
{
	unsigned char   port;
	unsigned char   addr;
	unsigned int  	tx_len;
	unsigned char	*tx_buf;
	unsigned int   	rx_len;
	unsigned char	*rx_buf;
}iw_i2c_msg;
#else
#define iw_i2c_msg		bsp_i2c_msg_t
#endif

typedef struct
{
	if_id_t port;
	unsigned char addr;
}iw_i2c_info_t;

typedef struct
{
	bool use;
	if_id_t port;
	unsigned char bank; 
	unsigned char gpio;	
}iw_adc_info_t;

/* iw_i2c */
iw_error_t iw_init_i2c( unsigned char port );
iw_error_t iw_deinit_i2c( unsigned char port );
iw_error_t iw_transfer_i2c(iw_i2c_msg *msg, int num);

/* iw_gpio */
void iw_config_gpio(unsigned char bank, unsigned char gpio, unsigned char dir, unsigned char pull, unsigned int speed, unsigned char input, unsigned char drive, unsigned char sense);
void iw_get_gpio(unsigned char bank, unsigned char gpio, unsigned char *val );
void iw_set_gpio(unsigned char bank, unsigned char gpio, unsigned char val);
void iw_toggle_gpio(unsigned char bank, unsigned char gpio);
void iw_init_gpio(void);

void iw_init_digital(uint8_t defined_pin, uint8_t mode);
void iw_put_digital(uint8_t defined_pin, uint8_t value);
uint8_t iw_get_digital(uint8_t defined_pin);
void iw_toggle_digital(uint8_t defined_pin);


/* iw_uart */
iw_error_t iw_init_uart( int port , unsigned int baudrate);
iw_error_t iw_deinit_uart( int port);
iw_error_t iw_putc_uart( int port, char ch );
iw_error_t iw_getc_uart( int port, char *ch );

/* iw_adc */
iw_error_t iw_init_adc(uint8_t id, uint8_t defined_pin);
iw_error_t iw_deinit_adc(uint8_t id);
iw_error_t iw_get_voltage(uint8_t id, uint16_t *val);
iw_error_t iw_get_adc(uint8_t id, uint16_t *val);

#endif /* IW_IF_H */

