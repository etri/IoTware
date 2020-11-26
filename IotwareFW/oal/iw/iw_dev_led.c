/*
Copyright (C) <2020> <ETRI>

This License and Service Agreement (LSA) applies to all works and their derivative works based on source form version and object form version of IoTware Project. Currently, the LSA for IoTware Project has two policies, 'Open Source License' and 'Commercial License'. Therefore, all works including the source code and executable code of IoTware Project and derivative works based thereon are subject to either 'Open Source License' or 'Commercial License' depending on the user's needs and purpose. Details related to the selection of the applicable license are specified in this LSA. If you use any or all of IoTware Project in any form, you are deemed to have consented to this LSA. If you breach any of the terms and conditions set forth in this LSA, you are solely responsible for any losses or damages incurred by Electronics and Communications Research Institute (ETRI), and ETRI assume no responsibility for you or any third party.
If you use the source form version or object form version of IoTware Project in whole or in part to develop a code or a derivative work, and you want to commercialize the result in some form, you will be covered under a commercial license. And if you are subject to a commercial license, the contract for the use of IoTware Project is subject to TECHNOLOGY LICENSE AGREEMENT of ETRI. You acknowledge that ETRI has all legal rights, title and interest, including intellectual property rights in the IoTware Project (regardless of whether such intellectual property rights are registered or where such rights exist) and agree with no objection thereto. Except as provided in a subsidiary agreement, nothing in this LSA grants you the right to use IoTware Project or the name, service mark, logo, domain name and other unique identification marks of ETRI.
If you use the source form version or object form version of IoTware Project in whole or in part to develop a code or a derivative work, and you do not commercialize the result in any form, you will be covered under an open source license. IoTware Project is in accordance with Free Software Foundation (FSF)'s open source policy, and is allowed to use it in the appropriate scope and manner, and you must comply with the applicable open source license policy applied to IoTware Project. IoTware Project is, in principle, subject to GNU Lesser General Public License version 2.1 (LGPLv2.1). If you have acquired all or a part of the IoTware Project in any way and it is subject to a license other than the open source license described above, please contact the following address for the technical support and other inquiries before use, and check the usage information.
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
