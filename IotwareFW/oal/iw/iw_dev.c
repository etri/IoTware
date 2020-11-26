/*
Copyright (C) <2020> <ETRI>

This License and Service Agreement (LSA) applies to all works and their derivative works based on source form version and object form version of IoTware Project. Currently, the LSA for IoTware Project has two policies, 'Open Source License' and 'Commercial License'. Therefore, all works including the source code and executable code of IoTware Project and derivative works based thereon are subject to either 'Open Source License' or 'Commercial License' depending on the user's needs and purpose. Details related to the selection of the applicable license are specified in this LSA. If you use any or all of IoTware Project in any form, you are deemed to have consented to this LSA. If you breach any of the terms and conditions set forth in this LSA, you are solely responsible for any losses or damages incurred by Electronics and Communications Research Institute (ETRI), and ETRI assume no responsibility for you or any third party.
If you use the source form version or object form version of IoTware Project in whole or in part to develop a code or a derivative work, and you want to commercialize the result in some form, you will be covered under a commercial license. And if you are subject to a commercial license, the contract for the use of IoTware Project is subject to TECHNOLOGY LICENSE AGREEMENT of ETRI. You acknowledge that ETRI has all legal rights, title and interest, including intellectual property rights in the IoTware Project (regardless of whether such intellectual property rights are registered or where such rights exist) and agree with no objection thereto. Except as provided in a subsidiary agreement, nothing in this LSA grants you the right to use IoTware Project or the name, service mark, logo, domain name and other unique identification marks of ETRI.
If you use the source form version or object form version of IoTware Project in whole or in part to develop a code or a derivative work, and you do not commercialize the result in any form, you will be covered under an open source license. IoTware Project is in accordance with Free Software Foundation (FSF)'s open source policy, and is allowed to use it in the appropriate scope and manner, and you must comply with the applicable open source license policy applied to IoTware Project. IoTware Project is, in principle, subject to GNU Lesser General Public License version 2.1 (LGPLv2.1). If you have acquired all or a part of the IoTware Project in any way and it is subject to a license other than the open source license described above, please contact the following address for the technical support and other inquiries before use, and check the usage information.
*/
#include "iw_common.h"
#include "iw_dev.h"
#include "iw_dev_sensor.h"
#include "iw_dev_net.h"
#include "iw_dev_led.h"
#include "iw_dev_batt.h"
#include "iw_bsp.h"
#include "bot_cle110.h"


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


void iw_register_device(void)
{
#if defined(IOTWARE_BOARD)
#if defined(LOWPOWER_ENABLE)
	iw_register_sensor_((sensor_type_t)(SENSOR_TYPE_CO2), "ccs811", IF_TYPE_I2C, BSP_I2C2, 0, &ccs811_driver);
#else
#if (IW_MCU_STM32==1)		// STM
	// SENSOR
    iw_register_sensor_((sensor_type_t)(SENSOR_TYPE_TMP|SENSOR_TYPE_HUM), "si7020", IF_TYPE_I2C, BSP_I2C2, 0, &si7020_driver);
	iw_register_sensor_((sensor_type_t)(SENSOR_TYPE_CO2), "ccs811", IF_TYPE_I2C, BSP_I2C3, 0, &ccs811_driver);

	// WIFI
	iw_register_network_(NET_DEV_TYPE_WIFI, "esp8266", IF_TYPE_UART, BSP_UART2, 0, &esp8266_driver);
    //iw_register_wifi_info("TO21COMMS_ASUS", "to63420042$"); //TO21COMMS_ASUS
	//iw_register_wifi_info("TO21COMMS_ipTIME", "to63420042"); //TO21COMMS_ipTIME
  	iw_register_wifi_info("IoTWare_Test", "1234567890"); //TO21COMMS_ipTIME

	// LED
	iw_register_led(LED_RED, PIN_RED_LED);
    iw_register_led(LED_GREEN, PIN_GREEN_LED);
    iw_register_led(LED_BLUE, PIN_BLUE_LED);

	// Battery
    iw_register_battery(IF_TYPE_ADC, BSP_ADC1, PIN_ADC_BATT);
#elif (IW_MCU_NORDIC==1)	// NORDIC
	// SENSRO
    iw_register_sensor_((sensor_type_t)(SENSOR_TYPE_TMP|SENSOR_TYPE_HUM), "si7020", IF_TYPE_I2C, BSP_I2C1, 0, &si7020_driver);
	iw_register_sensor_((sensor_type_t)(SENSOR_TYPE_CO2), "ccs811", IF_TYPE_I2C, BSP_I2C2, 0, &ccs811_driver);

	// WIFI
	//iw_register_network_(NET_DEV_TYPE_WIFI, "esp8266", IF_TYPE_UART, BSP_EXP_UART1, 0, &esp8266_driver);
	iw_register_network_(NET_DEV_TYPE_WIFI, "esp8266", IF_TYPE_UART, BSP_UART2, 0, &esp8266_driver);
    //iw_register_wifi_info("TO21COMMS_ASUS", "to63420042$"); //TO21COMMS_ASUS
	//iw_register_wifi_info("TO21COMMS_ipTIME", "to63420042"); //TO21COMMS_ipTIME
  	// iw_register_wifi_info("IoTWare_Test", "1234567890"); //TO21COMMS_ipTIME
	iw_register_wifi_info("iotware-ap", "11111111");

	// LORA
    //iw_register_network_(NET_DEV_TYPE_LORA, "rak811", IF_TYPE_UART, BSP_UART2, 0, &rak811_driver);
	//iw_register_lora_abp_info("26041B13","0FA2B64B6E6DC9EA457A67F9BB072CEB","FF0F529EB343466DA1544586261DA331");    
	//iw_register_lora_otaa_info("9090988909988789", "70B3D57ED00209F5", "0F87F0459ABE8EFD51B2632F36700EEB");    
    //iw_register_lora_band_info("KR920"); // KR920 or US915

	// BLE
    //iw_register_network(NET_DEV_TYPE_BLE, IF_TYPE_UART, IF_ID_2, 115200, "nrf51822");
    iw_register_network_(NET_DEV_TYPE_BLE, "botcle110", IF_TYPE_UART, BSP_EXP_UART2, 0, &botcle_driver);
	iw_register_ble_info("TO21_MASTER", "0000", BLE_MASTER_MODE);
	//iw_register_ble_info("TO21_SLAVE", "0000", BLE_SLAVE_MODE);

	// LED
	iw_register_led(LED_RED, PIN_RED_LED);
	iw_register_led(LED_GREEN, PIN_GREEN_LED);

	// Battery
	 iw_register_battery(IF_TYPE_ADC, BSP_ADC1, PIN_ADC_BATT);
#else						// TELECHIPS

#endif
#endif // defined(LOWPOWER_ENABLE)
#elif defined(DISPOSAL_IOT_BOARD)
	iw_register_sensor(SENSOR_TYPE_TMP|SENSOR_TYPE_HUM, IF_TYPE_I2C, IF_ID_2, 0, "sht20");
	iw_register_sensor(SENSOR_TYPE_CO2, IF_TYPE_UART, IF_ID_2, 38400, "tg400");
    iw_register_network(NET_DEV_TYPE_LORA, IF_TYPE_UART, IF_ID_1, 115200, "atsamr35");
    //iw_register_lora_abp_info("26041B13","0FA2B64B6E6DC9EA457A67F9BB072CEB","FF0F529EB343466DA1544586261DA331");
    iw_register_lora_otaa_info("9090988909988789", "70B3D57ED00209F5", "0F87F0459ABE8EFD51B2632F36700EEB");   
#else
#endif
}	
