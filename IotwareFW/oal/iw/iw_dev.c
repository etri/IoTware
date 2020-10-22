/*
Copyright (C) <2020> <ETRI>

This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 3.0 of the License, or (at your option) any later version. This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details. You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
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
