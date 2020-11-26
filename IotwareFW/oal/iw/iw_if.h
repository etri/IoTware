/*
Copyright (C) <2020> <ETRI>

This License and Service Agreement (LSA) applies to all works and their derivative works based on source form version and object form version of IoTware Project. Currently, the LSA for IoTware Project has two policies, 'Open Source License' and 'Commercial License'. Therefore, all works including the source code and executable code of IoTware Project and derivative works based thereon are subject to either 'Open Source License' or 'Commercial License' depending on the user's needs and purpose. Details related to the selection of the applicable license are specified in this LSA. If you use any or all of IoTware Project in any form, you are deemed to have consented to this LSA. If you breach any of the terms and conditions set forth in this LSA, you are solely responsible for any losses or damages incurred by Electronics and Communications Research Institute (ETRI), and ETRI assume no responsibility for you or any third party.
If you use the source form version or object form version of IoTware Project in whole or in part to develop a code or a derivative work, and you want to commercialize the result in some form, you will be covered under a commercial license. And if you are subject to a commercial license, the contract for the use of IoTware Project is subject to TECHNOLOGY LICENSE AGREEMENT of ETRI. You acknowledge that ETRI has all legal rights, title and interest, including intellectual property rights in the IoTware Project (regardless of whether such intellectual property rights are registered or where such rights exist) and agree with no objection thereto. Except as provided in a subsidiary agreement, nothing in this LSA grants you the right to use IoTware Project or the name, service mark, logo, domain name and other unique identification marks of ETRI.
If you use the source form version or object form version of IoTware Project in whole or in part to develop a code or a derivative work, and you do not commercialize the result in any form, you will be covered under an open source license. IoTware Project is in accordance with Free Software Foundation (FSF)'s open source policy, and is allowed to use it in the appropriate scope and manner, and you must comply with the applicable open source license policy applied to IoTware Project. IoTware Project is, in principle, subject to GNU Lesser General Public License version 2.1 (LGPLv2.1). If you have acquired all or a part of the IoTware Project in any way and it is subject to a license other than the open source license described above, please contact the following address for the technical support and other inquiries before use, and check the usage information.
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

