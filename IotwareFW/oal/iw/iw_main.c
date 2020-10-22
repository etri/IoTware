/*
Copyright (C) <2020> <ETRI>

This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 3.0 of the License, or (at your option) any later version. This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details. You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/

#include "iw_common.h"
#include "iw_bsp.h"
#include "iw_oal.h"
#include "iw_dev_led.h"
#include "iw_sys.h"


extern void iw_init_device(void);
extern void iw_init_autocontrol_service(void);
extern void iw_init_lowpower_service(void);
extern void iw_start();

void iw_init_service(void)
{
    iw_init_autocontrol_service();
    //iw_init_lowpower_service();
	iw_start();
}

#if 0
#include "priv_log.h"

int main(void)
{
	int *mem = NULL;
	iw_mutex_t mutex = NULL;
	iw_queue_t queue = NULL;
	iw_sem_t semaphore = NULL;

	iw_init_bsp();

test_memory:
	log_info("[MEMORY TEST]\n");

	log_info("try to allocate memory...\n");
	mem = (typeof(mem)) iw_alloc(sizeof(*mem));
	if (!mem) {
		log_error("failed to allocate memory\n");
		goto test_mutex;
	}

	*mem = 21;
	log_info("|mem:0x%X|*mem:%d\n", mem, *mem);

	log_info("try to deallocate memory...\n");
	iw_free(mem);

test_mutex:
	log_info("[MUTEX TEST]\n");
	
	log_info("creating mutex...\n");
	if (IW_FAIL == iw_create_mutex(&mutex)) {
		log_error("failed to create mutex\n");
		goto test_queue;
	}

	log_info("trying to lock mutex by time(100 ms)...\n");
	if (IW_FAIL == iw_lock_mutex_ms(mutex, 100)) {
		log_error("failed to lock mutex by time\n");
		goto test_queue;
	}
	//
	
	log_info("trying to unlock mutex...\n");
	if (IW_FAIL == iw_unlock_mutex(mutex)) {
		log_error("failed to unlock mutex\n");
		goto test_queue;
	}
	//

	log_info("trying to lock mutex...\n");
	if (IW_FAIL == iw_lock_mutex(mutex)) {
		log_error("failed to lock mutex\n");
		goto test_queue;
	}
	//
	
	log_info("trying to unlock mutex...\n");
	if (IW_FAIL == iw_unlock_mutex(mutex)) {
		log_error("failed to unlock mutex\n");
		goto test_queue;
	}
	//

	log_info("deleting mutex...\n");
	iw_delete_mutex(mutex);

test_queue:

test_sem:
	log_info("[TEST SEMAPHORE]\n");

	log_info("create semaphore...\n");
	if (IW_FAIL == iw_create_sem(&semaphore, 10, 0)) {
		log_error("failed to create semaphore...\n");
		goto test_task;
	}

	log_info("trying to lock semaphore...\n");
	if (IW_FAIL == iw_lock_sem(semaphore, 10)) {
		log_error("failed to lock semaphore...\n");
		goto test_task;
	}

	log_info("trying to unlock semaphore...\n");
	if (IW_FAIL == iw_unlock_sem(semaphore, (iw_task_t)NULL)) {
		log_error("failed to unlock semaphore...\n");
		goto test_task;
	}

	log_info("deleting semaphore...\n");
	iw_delete_sem(semaphore);

test_task:
	return 0;
}
#endif


int main(void)
{
#if 1
	iw_init_bsp();
	iw_init_os();			// os initialize
	iw_start_scheduler();	// os start
#endif
	
#if 0	// test without os
	iw_init_bsp();

#if 0	// ST board
	int tick = 0;

	iw_init_digital(PIN_I2C2_SCL, DIGITAL_INPUT_NOPULL);
	iw_init_digital(PIN_I2C2_SDA, DIGITAL_INPUT_PULLUP);
	iw_init_digital(PIN_I2C3_SCL, DIGITAL_INPUT_NOPULL);
	iw_init_digital(PIN_I2C3_SDA, DIGITAL_INPUT_PULLUP);
	iw_put_digital(PIN_I2C2_SCL, DIGITAL_LOW);
	iw_put_digital(PIN_I2C2_SDA, DIGITAL_LOW);
	iw_put_digital(PIN_I2C3_SCL, DIGITAL_LOW);
	iw_put_digital(PIN_I2C3_SDA, DIGITAL_LOW);

   	iw_init_digital(PIN_UART2_TX, DIGITAL_INPUT_NOPULL);
	iw_init_digital(PIN_UART2_RX, DIGITAL_INPUT_NOPULL);

	iw_put_digital(PIN_BLE_PWR_EN, DIGITAL_HIGH);
	iw_put_digital(PIN_LORA_PWR_EN, DIGITAL_LOW);
	iw_put_digital(PIN_TEMP_PWR_EN, DIGITAL_LOW);
	iw_put_digital(PIN_CO2_RESET, DIGITAL_LOW);
	iw_put_digital(PIN_CO2_PWR_EN, DIGITAL_LOW);
	
	iw_put_digital(PIN_RED_LED, DIGITAL_HIGH);
	iw_put_digital(PIN_GREEN_LED, DIGITAL_HIGH);
	iw_put_digital(PIN_BLUE_LED, DIGITAL_HIGH); 
	
    while(1) {
		//iw_init_uart(BSP_EXP_UART1, 115200);
		//iw_put_digital(PIN_BLE_PWR_EN, DIGITAL_HIGH);
		//iw_put_digital(PIN_LORA_PWR_EN, DIGITAL_HIGH);
		//iw_put_digital(PIN_WIFI_PWR_EN, DIGITAL_HIGH);
		//iw_put_digital(PIN_TEMP_PWR_EN, DIGITAL_HIGH);
		//iw_put_digital(PIN_CO2_PWR_EN, DIGITAL_HIGH);
		 iw_put_digital(PIN_RED_LED, DIGITAL_LOW);
		// iw_printf("Power On\n");
		 iw_delay_ms(1000);
		//iw_put_digital(PIN_BLE_PWR_EN, DIGITAL_LOW);
		//iw_put_digital(PIN_LORA_PWR_EN, DIGITAL_LOW);
		//iw_put_digital(PIN_WIFI_PWR_EN, DIGITAL_LOW);
		//iw_put_digital(PIN_TEMP_PWR_EN, DIGITAL_LOW);
		//iw_put_digital(PIN_CO2_PWR_EN, DIGITAL_LOW);
		 iw_put_digital(PIN_RED_LED, DIGITAL_HIGH);
		// iw_printf("Power Off\n");
		 iw_delay_ms(1000);

		iw_printf("main \n", iw_get_tick_count());
    }
#endif	

#if 1	// nordic board
	iw_put_digital(PIN_RED_LED, DIGITAL_HIGH);

	iw_init_digital(PIN_I2C1_SCL, DIGITAL_INPUT_NOPULL);
	iw_init_digital(PIN_I2C1_SDA, DIGITAL_INPUT_NOPULL);
	iw_put_digital(PIN_I2C2_SCL, DIGITAL_LOW);
	iw_put_digital(PIN_I2C2_SDA, DIGITAL_LOW);

	iw_put_digital(PIN_BLE_PWR_EN, DIGITAL_LOW);
	iw_put_digital(PIN_LORA_PWR_EN, DIGITAL_LOW);
	iw_put_digital(PIN_TEMP_PWR_EN, DIGITAL_LOW);
	iw_put_digital(PIN_WIFI_PWR_EN, DIGITAL_LOW);
	iw_put_digital(PIN_CO2_PWR_EN, DIGITAL_LOW);
	
	//iw_put_digital(PIN_RED_LED, DIGITAL_HIGH);
	//iw_put_digital(PIN_GREEN_LED, DIGITAL_LOW);
	//iw_put_digital(PIN_BLUE_LED, DIGITAL_HIGH);   

	bsp_init_adc(BSP_ADC1, 0, 0);

	iw_printf("%s \n", __func__);

	// uint8_t dev_id;
	// int connected = NET_DISCONNECTED;
	// iw_get_network_devid(NET_DEV_TYPE_BLE, NULL, &dev_id);
	// iw_init_network(dev_id);

	//bsp_init_uart(BSP_EXP_UART1, 9600);
	//bsp_init_uart(BSP_EXP_UART2, 9600);

	uint16_t value = 0;

    while(1) {
		//iw_init_uart(BSP_EXP_UART1, 115200);
		//iw_put_digital(PIN_BLE_PWR_EN, DIGITAL_HIGH);
		//iw_put_digital(PIN_LORA_PWR_EN, DIGITAL_HIGH);
		//iw_put_digital(PIN_WIFI_PWR_EN, DIGITAL_HIGH);
		//iw_put_digital(PIN_TEMP_PWR_EN, DIGITAL_HIGH);
		//iw_put_digital(PIN_CO2_PWR_EN, DIGITAL_HIGH);
		 //iw_put_digital(PIN_RED_LED, DIGITAL_LOW);
		 //bsp_putc_uart(BSP_EXP_UART1, 'A');
		 //bsp_putc_uart(BSP_EXP_UART2, 'A');
		 //iw_printf("main %d\n", iw_get_tick_count());
		 //iw_delay_ms(100);
		//iw_put_digital(PIN_BLE_PWR_EN, DIGITAL_LOW);
		//iw_put_digital(PIN_LORA_PWR_EN, DIGITAL_LOW);
		//iw_put_digital(PIN_WIFI_PWR_EN, DIGITAL_LOW);
		//iw_put_digital(PIN_TEMP_PWR_EN, DIGITAL_LOW);
		//iw_put_digital(PIN_CO2_PWR_EN, DIGITAL_LOW);
		 //iw_put_digital(PIN_RED_LED, DIGITAL_HIGH);
		 //bsp_putc_uart(BSP_EXP_UART1, 'B');
		 //bsp_putc_uart(BSP_EXP_UART2, 'B');
		// iw_printf("main %d\n", iw_get_tick_count());
		 //bsp_get_adc(BSP_ADC1, &value);
		 bsp_get_voltage(BSP_ADC1, &value);
		 iw_printf("main %d\n", value);
		 iw_delay_ms(100);

		//  iw_recv_data_network(dev_id, 0, 0);
		//  if(iw_is_connected_network(dev_id, &connected) == IW_SUCCESS) {
		// 	 if(connected == NET_CONNECTED) {
		// 	 	iw_send_data_network(dev_id, "BLE", strlen("BLE"));
		// 		iw_printf("[%s] send %s\n", __func__, "BLE");
		// 	 }
		//  }
    }	
#endif
#if 0	// tele
	iw_printf("%s \n", __func__);
	uint32_t cnt = 0;

    while(1) {
		bsp_set_gpio(0, 22, 0);
		iw_delay_ms(1000000);
		bsp_set_gpio(0, 22, 1);
		iw_delay_ms(1000000);
		iw_printf("main %d\n", cnt++);
    }
#endif

#endif	

	return 0;
}
