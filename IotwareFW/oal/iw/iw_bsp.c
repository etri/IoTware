/*
Copyright (C) <2020> <ETRI>

This License and Service Agreement (LSA) applies to all works and their derivative works based on source form version and object form version of IoTware Project. Currently, the LSA for IoTware Project has two policies, 'Open Source License' and 'Commercial License'. Therefore, all works including the source code and executable code of IoTware Project and derivative works based thereon are subject to either 'Open Source License' or 'Commercial License' depending on the user's needs and purpose. Details related to the selection of the applicable license are specified in this LSA. If you use any or all of IoTware Project in any form, you are deemed to have consented to this LSA. If you breach any of the terms and conditions set forth in this LSA, you are solely responsible for any losses or damages incurred by Electronics and Communications Research Institute (ETRI), and ETRI assume no responsibility for you or any third party.
If you use the source form version or object form version of IoTware Project in whole or in part to develop a code or a derivative work, and you want to commercialize the result in some form, you will be covered under a commercial license. And if you are subject to a commercial license, the contract for the use of IoTware Project is subject to TECHNOLOGY LICENSE AGREEMENT of ETRI. You acknowledge that ETRI has all legal rights, title and interest, including intellectual property rights in the IoTware Project (regardless of whether such intellectual property rights are registered or where such rights exist) and agree with no objection thereto. Except as provided in a subsidiary agreement, nothing in this LSA grants you the right to use IoTware Project or the name, service mark, logo, domain name and other unique identification marks of ETRI.
If you use the source form version or object form version of IoTware Project in whole or in part to develop a code or a derivative work, and you do not commercialize the result in any form, you will be covered under an open source license. IoTware Project is in accordance with Free Software Foundation (FSF)'s open source policy, and is allowed to use it in the appropriate scope and manner, and you must comply with the applicable open source license policy applied to IoTware Project. IoTware Project is, in principle, subject to GNU Lesser General Public License version 2.1 (LGPLv2.1). If you have acquired all or a part of the IoTware Project in any way and it is subject to a license other than the open source license described above, please contact the following address for the technical support and other inquiries before use, and check the usage information.
*/
/******************************************************************************
 * Pre-Processors
 ******************************************************************************/
#include "iw_common.h"
#include "iw_dev.h"
#include "iw_if.h"
#include "iw_dev_net.h"
#include "iw_dev_sensor.h"
#include "iw_bsp.h"
#include "iw_sys.h"

/******************************************************************************
 * Public Variables 
 ******************************************************************************/

unsigned int g_uart_using_info[IF_ID_MAX] = {0, 0, 0, 0, 0};
unsigned char g_i2c_using_info[IF_ID_MAX] = {0, 0, 0, 0, 0};
iw_adc_info_t g_adc_using_info[USE_ADC_MAX];

#if defined(IOTWARE_BOARD)
extern sensor_driver_t si7020_driver;
extern sensor_driver_t ccs811_driver;
extern net_driver_t esp8266_driver;
extern net_driver_t rak811_driver;
extern net_driver_t nrf51822_driver;
extern net_driver_t hc06_driver;
#elif defined(DISPOSAL_IOT_BOARD)
extern sensor_driver_t sht20_driver;
extern sensor_driver_t tg400_driver;
extern net_driver_t atsamr35_driver;
#else
#endif

/******************************************************************************
 * Public Functions
 *****************************************************************************/
 
void iw_delay_us(unsigned int us)
{
    bsp_delay_us(us);
}

void iw_delay_ms(unsigned int ms)
{
    bsp_delay_us( ms * 1000 );
}

/* added by ys */
void iw_init_mcu(void)
{
    bsp_init_mcu();
}

void iw_init_board(void)
{
    bsp_init_board();
#if 0
	uint8_t sensor_num = iw_get_registered_sensor_num();
	iw_printf("iw_get_registered_sensor_num() %d\n", sensor_num);
	//iw_delay_ms(1000);
	for(uint8_t i = 0; i < sensor_num; i++) {
		sensor_dev_info_t *info = iw_get_sensor_dev_info(i);

		if(info) {
			iw_printf("sensor device %d %s\n", info->dev_id, info->name);

			if(info->iface == IF_TYPE_UART) {
				//iw_init_uart(i, g_uart_using_info[i]);
			}
			else if(info->iface == IF_TYPE_I2C) {
				iw_init_i2c(info->iface_id);
			}
			else if(info->iface == IF_TYPE_ADC) {
				iw_init_adc(info->iface_id);
			}
			else if(info->iface == IF_TYPE_SPI) {
			//	iw_init_spi(info->iface_id);
			}
			else if(info->iface == IF_TYPE_GPIO) {
			}
		}
	}
#endif
	//iw_delay_ms(1000);	// delay를 사용하면 부팅이 안됨
	//iw_put_digital(PIN_RED_LED, DIGITAL_LOW);
}

bool iw_set_rtc_time(DATE_TIME_T *dt)
{
	return bsp_set_rtc_time(dt);
}

bool iw_get_rtc_time(DATE_TIME_T *dt)
{
	return bsp_get_rtc_time(dt);
}

bool iw_init_rtc(void)
{
	return bsp_init_rtc();
}
void iw_init_bsp(void)
{
	mcu_type_t mcu_type;

	iw_init_mcu();
	iw_init_gpio();
	iw_init_uart(DEBUG_UART_PORT, DEBUG_UART_BAUDRATE);

	iw_get_mcu_info(&mcu_type);

	iw_printf("\n\n\n\n");
	iw_printf("**************************************\n");
	iw_printf("*         IoTware Board V2.0         *\n");
	iw_printf("*  Build Time : %s %s *\n", __DATE__, __TIME__);
	iw_printf("*         BSP : %10s           *\n", iw_get_mcu_name(mcu_type));
	iw_printf("*          OS : %-16s     *\n", iw_os_name());
	iw_printf("**************************************\n");

	iw_register_device();

	iw_init_board();
	//iw_put_digital(PIN_GREEN_LED, DIGITAL_LOW);    
	//while(1);

	iw_init_rtc();

#if defined(IOTWARE_BOARD)
	//iw_register_sensor_driver(&si7020_driver);		// Humidity and Temperature Sensor 
	//iw_register_sensor_driver(&ccs811_driver);		// co2 Sensor
	//iw_register_network_driver(&rak811_driver);		// LORA module
	//iw_register_network_driver(&esp8266_driver);	// Wi-Fi module
	//iw_register_network_driver(&nrf51822_driver);	// BLE
	//iw_register_network_driver(&hc06_driver);		// BLE
#elif defined(DISPOSAL_IOT_BOARD)
	iw_register_sensor_driver(&sht20_driver);
	iw_register_sensor_driver(&tg400_driver);
	iw_register_network_driver(&atsamr35_driver);
#else
#endif
	//iw_delay_ms(100);
}

void init_device_task(void *args)
{
	uint8_t dev_num, i;

	iw_printf("init_device_task() start\n");

	dev_num = iw_get_registered_sensor_num();
	iw_printf("sensor num %d\n", dev_num);
	for(i = 0; i < dev_num; i++) {
		iw_init_sensor(i);
	}

#if 0 // sukaa
	iw_get_registered_net_num(&dev_num);
	iw_printf("net num:%d\n", dev_num);
	if(dev_num > 0) {
		for(i = 0; i < dev_num; i++) {
			iw_init_network(i);
		}
	}
#endif

#if 0 /* for memory usage test */
	while(1){
		unsigned int a, b, c;
		iw_get_memory_status(&a, &b, &c);
		iw_printf("g:%d t:%d u:%d\n", a, b, b-c);
		
		iw_sleep(5000);
	}
#endif
	//iw_printf("init device task exit\n");
	iw_exit_task();
}

void iw_init_device(void)
{
#if 0	
	iw_create_task(init_device_task, "init_device_task", 0x100, NULL, IW_TSK_PRIO_IDLE+7, NULL);
#else
	uint8_t dev_num, i;

	iw_printf("iw_init_device()\n");
	iw_delay_ms(100);
	
	dev_num = iw_get_registered_sensor_num();
	iw_printf("sensor num %d\n", dev_num);
	for(i = 0; i < dev_num; i++) {
		iw_init_sensor(i);
	}

#if 0 // sukaa
	iw_get_registered_net_num(&dev_num);
	iw_printf("net num:%d\n", dev_num);
	if(dev_num > 0) {
		for(i = 0; i < dev_num; i++) {
			iw_init_network(i);
		}
	}
#endif	
#endif
}
