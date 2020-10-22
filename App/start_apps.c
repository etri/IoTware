
/*
Copyright (C) <2020> <ETRI>

This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 3.0 of the License, or (at your option) any later version. This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details. You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/
#define IOTWARE_MODE 0	// 0: default, 1: to21 test, 2: to21 test (low power)
#if (IOTWARE_MODE == 0)
/**
 * @file start_apps.c
 * @brief 사용자 응용 프로그램 시작 지점
 * @details OS가 부팅 중에 로드하는 사용자 응용 프로그램의 시작 지점
 * 
 */
#include <stdio.h>

#include "iw_fw_microservices.h"
#include "iw_fw_lwMsgUtils.h"
#include "ms_generator.h"
#include "iw_fw_collabomsg.h"

#include "interface_all.h"


extern void iw_fw_lwMsgTx(void* pvParameters);
extern void iw_fw_lwMsgRx(void* pvParameters);
extern void lwmsg_dist_task(void *pvParameters);
extern void wifi_task(void *args);
extern void lora_task(void *args);

iw_queue_t xNetworkRx;
iw_queue_t xNetworkTx;
iw_task_t xNetworkRxTask;
iw_task_t xNetworkTxTask;



/**
 * @brief OS에서 로드하는 사용자 응용 프로그램의 시작 지점
 * @details OS에서 로드하는 사용자 응용 프로그램의 시작 지점으로 시동 시 
 * 필요로 하는 사용자 응용 프로그램을 여기서 구동
 */
void start_apps(void)
{
	// temp
	xNetworkRx = create_queue(10, 128 * sizeof(uint8_t));
	xNetworkTx = create_queue(10, 256 * sizeof(uint8_t));

#if defined(IOTWARE_BOARD) || defined(DISPOSAL_IOT_BOARD)
	xNetworkTxTask = create_task(wifi_task, "wifi_task", IW_MINIMAL_STACK_SIZE, NULL, MS_TASK_PRIORITY, NULL);
	// xNetworkTxTask = create_task(lora_task, "lora_task", IW_MINIMAL_STACK_SIZE, NULL, MS_TASK_PRIORITY, NULL);
#endif

	init_mservices();
	NEW_MS_GENERATORS();

	printf("\nEnter to start!\n");
	// getchar();

	set_lwmsg_receive_queue(create_queue(mainQUEUE_LENGTH, sizeof(PLWMessage)));
	set_lwmsg_send_queue(create_queue(mainQUEUE_LENGTH, sizeof(PLWMessage)));

	iw_task_t temp;
	// LWMSG Distribution Task
	temp = create_task(lwmsg_dist_task, "lwmsg_dist_task", IW_MINIMAL_STACK_SIZE * 2, NULL, MS_TASK_PRIORITY+1, NULL);
	set_lwmsg_receive_task(temp);

	temp = create_task(iw_fw_lwMsgTx, "iw_fw_lwMsgTx", IW_MINIMAL_STACK_SIZE * 2, NULL, MS_TASK_PRIORITY+1, NULL);
	set_lwmsg_send_task(temp);
	xNetworkRxTask = create_task(iw_fw_lwMsgRx, "iw_fw_lwMsgRx", IW_MINIMAL_STACK_SIZE * 2, NULL, MS_TASK_PRIORITY+1, NULL);
	

	PLWMessage lwm_msg = make_device_registration_lwmsg(1, GET_LWM_TX_DRI());
	if (lwm_msg)
		REQUEST_LWMSG(lwm_msg);
}
#elif (IOTWARE_MODE == 1)

#include "interface_all.h"


float g_fTemperature;
float g_fHumidity;
uint32_t g_uiCo2;
uint32_t g_uiCo2Last;

#define SEND_TEST3_STR "GET http://maluka.tmep.cz/?teplota=26.2\r\n"
#define SEND_LORA_STR "{if=2;dri=2;di=4dasc44321}{tif=[{ti=101;sp={miop=0;maop=3600"

enum{
	STATUS_SCANNING = 0,
};
#define MAX_R_LEN 512u
#define MAX_W_LEN 512u
char rbuf[MAX_R_LEN]={0,};
char wbuf[MAX_W_LEN]={0,};


void Co2ReadSensorTaskOnOff(uint8_t on);
uint8_t Co2ReadSensorTaskStatus(void);


void control_pwrmode_network(unsigned char dev_id)
{
	static int cnt=0;
    static int cnt1=0; 

	cnt++;
	if( (cnt1==0) && (cnt==2) )
	{
        set_pwrmode_network(dev_id, NET_PWR_OFF_MODE);
		cnt=0;
		cnt1=1;
	}
	if( (cnt1==1) && (cnt==2) )
	{
		set_pwrmode_network(dev_id, NET_PWR_RUN_MODE );
		cnt=0;
		cnt1=0;
	}
    iw_printf("test %d %d\n", cnt1, cnt);
}

void loraTask(void *args)
{
	int ret = IW_FAIL;
	unsigned char dev_id = 0;
	int status, rssi;

	iw_printf("loraTask start\n");

	ret = get_network_devid(NET_DEV_TYPE_LORA, NULL, &dev_id);
	iw_printf("get_network_devid(%d, %d) %d\n", NET_DEV_TYPE_LORA, dev_id, ret);
	
	iw_init_network(dev_id);

	if(ret == IW_SUCCESS){
		while(1){		
			if( (is_connected_network(dev_id, &status) == IW_SUCCESS)  &&
					(status == NET_CONNECTED)){
				
				connect_server(dev_id, NULL, 2, 5000, PROTO_TCP);
				memcpy(wbuf, SEND_LORA_STR, strlen(SEND_LORA_STR));

				while(1)
				{
					#if 0
					control_pwrmode_network(dev_id);
					#endif
					
					if( send_data_network(dev_id, wbuf, strlen(wbuf)) == IW_SUCCESS){
						unsigned int len ;
						do{
							ret = available_network(dev_id, &len);
							
							//printf("%d available\n", len);
							if(ret == IW_SUCCESS && len>0)
							{
								unsigned int readLen = 0;	
								memset(rbuf, 0x0, MAX_R_LEN);
								readLen = MAX_R_LEN;
								ret = recv_data_network(dev_id, rbuf, &readLen);
								//iw_printf("ret:%d readLen:%d\n",ret, readLen);
								sleep(1000);
								if(ret == IW_SUCCESS && readLen>0)
									iw_printf("%s\n", rbuf);
								sleep(1000);
								iw_get_rssi_network(dev_id, &rssi);
								iw_printf("rssi:%d\n", rssi);
								sleep(1000);	
								break;
							}else{
								sleep(1000);
								
							}
						}while(1);
						break;
					}
					sleep(2000);
				}
				
			}else
				sleep(2000);
		}
	}
	iw_printf("LoRa Task exit\n");
	exit_task();
}


uint8_t wifitask_on = true;
uint8_t wifitask_status = false;
uint8_t wifitask_server_connect = false;
void WifiTaskOnOff(uint8_t on)
{
	wifitask_on = on;
}
uint8_t WifiTaskStatus(void)
{
	return wifitask_status;
}
uint8_t WifiTaskServerConnectStatus(void)
{
	return wifitask_server_connect;
}

#ifdef WIFI_IP
const char server_ip[] = WIFI_IP;
#else
const char server_ip[] = "192.168.100.9";
#endif
#ifdef WIFI_PORT
const int server_port = WIFI_PORT;
#else
const int server_port = 18800;
#endif
char msg[256];
void WifiTask(void *args)
{
	uint8_t dev_id = 0;
	int status;
	int ret_val = IW_FAIL;
	DATE_TIME_T dt;
	power_mode_t power;
	power_mode_t power_prev = 0xff;

	uint32_t send_period = 3;	// sec
	uint32_t send_tick = 0;

	uint8_t msg_send = false;
	uint8_t mode_seq = 0;
	uint32_t mode_tick = 0;

	uint32_t trans_count = 10;

	iw_printf("%s -> start\n", __func__);

	ret_val = get_network_devid(NET_DEV_TYPE_WIFI, NULL, &dev_id);
	iw_printf("get_network_devid(%d, %d) %d\n", NET_DEV_TYPE_WIFI, dev_id, ret_val);
	iw_sleep(3000);

	//iw_get_power_mode(&power);
	// if(power != POW_SLEEP_MODE) {
	// 	if(iw_init_network(dev_id) == IW_SUCCESS) {
	// 		wifitask_status = true;
	// 	}
	// }

	if(ret_val == IW_SUCCESS) {
		while(1){
			//iw_put_digital(PIN_GREEN_LED, DIGITAL_LOW);
			
			iw_get_power_mode(&power);
		
			if(power == POW_RUN_MODE) {				// 50 < power
				if(!wifitask_status) {
					iw_printf("Wifi On\n");
					if(iw_init_network(dev_id) == IW_SUCCESS) {
						wifitask_status = true;
					}
				}
				send_period = 3;
			}
			else if(power == POW_DYNAMIC_MODE) {	// 30 < power < 50
	
				if(!wifitask_status) {
					iw_printf("Wifi On\n");
					if(iw_init_network(dev_id) == IW_SUCCESS) {
						wifitask_status = true;
					}
				}
				int batt_p, batt_t;
				if(iw_get_battery_status(&batt_p, &batt_t) == IW_SUCCESS) {

				}
			
			}
			else if(power == POW_LOW_MODE) {		// 10 < power < 30
				
				if(power_prev != power) {
					power_prev = power;
					
					mode_seq = 0;
				}

				iw_printf("%s -> low power seq : %d\n", __func__, mode_seq);
	
				if(mode_seq == 0) {
					if(!wifitask_status) {
						iw_printf("%s -> Wifi On\n", __func__);
						if(iw_init_network(dev_id) == IW_SUCCESS) {
							wifitask_status = true;
							mode_seq = 1;
							msg_send = false;
						}
					}
					else {
						mode_seq = 1;
						msg_send = false;
					}
				}
				else if(mode_seq == 1) {
					if(msg_send) {
						msg_send = false;
						iw_printf("%s -> Wifi Off\n",  __func__);
						iw_disconnect_server(dev_id);
						iw_sleep(1000);
						iw_disconnect_network(dev_id);
						iw_sleep(1000);
						iw_deinit_network(dev_id);
						wifitask_server_connect = false;
						wifitask_status = false;
						mode_seq = 2;
						mode_tick = 0;
					}
				}
				else {
					mode_tick++;
					if(30 <= mode_tick) {
						mode_tick = 0;
						mode_seq = 0;
					}
				}

			}
			else {									// power < 10
				if(power_prev != power) {
					power_prev = power;
					
					mode_seq = 0;
				}

				//iw_printf("%s -> sleep power seq : %d\n", __func__, mode_seq);


				if(mode_seq == 0) {
					if(!wifitask_status) {
						iw_printf("%s -> Wifi On\n", __func__);
						if(iw_init_network(dev_id) == IW_SUCCESS) {
							wifitask_status = true;
							mode_seq = 1;
							msg_send = false;
						}
					}
					else {
						mode_seq = 1;
						msg_send = false;
					}
				}
				else if(mode_seq == 1) {
					if(msg_send) {
						msg_send = false;
						iw_printf("%s -> Wifi Off\n",  __func__);
						iw_disconnect_server(dev_id);
						iw_sleep(1000);
						iw_disconnect_network(dev_id);
						iw_sleep(1000);
						iw_deinit_network(dev_id);
						wifitask_server_connect = false;
						wifitask_status = false;
						mode_seq = 2;
						mode_tick = 0;
					}
				}
				else {

				}				

				// if(wifitask_status) {
				// 	iw_printf("%s -> Wifi Off\n",  __func__);
				// 	iw_disconnect_server(dev_id);
				// 	iw_deinit_network(dev_id);
				// 	wifitask_server_connect = false;
				// 	wifitask_status = false;
				// }
			}

			/* waiting module ready */
			if( iw_is_connected_network(dev_id, &status) == IW_SUCCESS && status == NET_GOT_IP){
				if(!wifitask_server_connect) {
					if(iw_connect_server(dev_id, (char *)server_ip, server_port, 2000*10, PROTO_TCP) == NET_SUCCESS) {
						wifitask_server_connect = true;
						iw_printf("%s -> connect server(%s)\n", __func__, server_ip);
					}
					else {
						iw_printf("%s -> not connect server(%s)\n", __func__, server_ip);
					}
				}
			}
			else {
				//iw_printf("%s -> network disconnected\n", __func__);
				wifitask_server_connect = false;
			}

			send_tick++;
			if(send_period <= send_tick) {
				send_tick = 0;

				if(wifitask_server_connect) {
					//if(Co2ReadSensorTaskStatus()) {
						iw_get_rtc_time(&dt);

						sprintf(msg, "[%02d:%02d:%02d] IotWare CO2 : %ld", dt.hour, dt.min, dt.sec, g_uiCo2Last);
						iw_send_data_network(dev_id, msg, strlen(msg));
						iw_printf("%s\n", msg);

						trans_count--;
						if(trans_count == 0) {
							iw_disconnect_server(dev_id);
							iw_printf("%s -> disconnect server(%s)\n", __func__, server_ip);
							Co2ReadSensorTaskOnOff(0);
							break;
						}

						msg_send = true;
					//}
				}
			}


			// if(wifitask_status != wifitask_on) {
			// 	if(wifitask_on) {
			// 		iw_printf("Wifi Task On\n");
			// 		iw_init_network(dev_id);
			// 	}
			// 	else {
			// 		iw_printf("Wifi Task Off\n");
			// 		iw_disconnect_server(dev_id);
			// 		iw_deinit_network(dev_id);
			// 		wifitask_server_connect = false;
			// 	}
			// 	wifitask_status = wifitask_on;
			// }

			//iw_toggle_digital(PIN_GREEN_LED);
			//iw_printf("%s\n", __func__);
			iw_sleep(1000);
		} // while(1)
	}

	iw_printf("%s -> exit\n", __func__);
	exit_task();
}

#if 0
void control_pwrmode_sensor(unsigned char dev_id)
{
	static int cnt=0;
    static int cnt1=0;

	cnt++;
	if( (cnt1==0) && (cnt==10) )
	{
	    set_pwrmode_sensor(dev_id, SENSOR_PWR_OFF_MODE);
        cnt=0;
		cnt1=1;
	}
	if( (cnt1==1) && (cnt==10) )
	{
	    set_pwrmode_sensor(dev_id, SENSOR_PWR_RUN_MODE);
        cnt=0;
		cnt1=0;
	}
}
#endif

//iw_mutex_t temp_hum_sens;


void tempReadSensorTask(void *args)
{
	unsigned char temp_id = 0;
	unsigned char hum_id = 0;
	float temp;
	float hum;
	int ret = IW_FAIL;
	
	iw_printf("tempReadSensorTask(%d) start\n", args);

	//ret = get_sensor_devid(SENSOR_TYPE_TMP, NULL, &dev_id);
	ret = iw_get_sensor_dev_id_by_type(SENSOR_TYPE_TMP, &temp_id);
	ret = iw_get_sensor_dev_id_by_type(SENSOR_TYPE_TMP, &hum_id);
	
	iw_init_sensor(temp_id);
	//iw_init_sensor(hum_id);
	
	if(ret == IW_SUCCESS) {
		while(1) {
#if 1
			//iw_printf("read_sensor(%d, %d)\n", temp_id, SENSOR_TYPE_TMP);
			if(read_sensor(temp_id, SENSOR_TYPE_TMP, &temp, 1) == IW_SUCCESS) {
				//iw_printf("TEMPERATURE: %d\n", (int)(temp * 100));
				iw_printf("TEMPERATURE: %f\n", temp);
				g_fTemperature = temp;
			}
			else {
				iw_printf("TEMPERATURE: err\n");
				g_fTemperature = 0;
			}

			//iw_printf("read_sensor(%d, %d)\n", temp_id, SENSOR_TYPE_HUM);
			// if(read_sensor(hum_id, SENSOR_TYPE_HUM, &hum, 1)==IW_SUCCESS){
			// 	iw_printf("HUMIDITY: %d\n", (int)(hum * 100));
			// 	g_fHumidity = hum;
			// }
			// else {
			// 	iw_printf("HUMIDITY: err\n");
			// 	g_fHumidity = 0;
			// }
			
#endif
			//led_toggle(LED_RED);
			sleep(1000);
		}
	}
	else {
		iw_printf("tempReadSensorTask() ERR : get_sensor_devid() -> %d\n", ret);
	}

	exit_task();
}

void humReadSensorTask(void *args)
{
	unsigned char dev_id = 0;
	float hum;
	int ret = IW_FAIL;
	
	ret = get_sensor_devid(SENSOR_TYPE_HUM, NULL, &dev_id);
	if(ret == IW_SUCCESS){
		while(1)
		{	
			sleep(5000);
			if(read_sensor(dev_id, SENSOR_TYPE_HUM, &hum, 1)==IW_SUCCESS){
				iw_printf("HUMIDITY: %d\n", (unsigned int)hum);
			}
			else {
				iw_printf("HUMIDITY: err\n");
			}
			//led_toggle(LED_GREEN);
		}
	}
	else {
		iw_printf("humReadSensorTask() ERR : get_sensor_devid() -> %d\n", ret);
	}

	exit_task();
	/* command line test sample code */ 
}

void show_list_sample()
{

	iw_list_t list;
	iw_list_node_t node_1, node_2, node_3;
	unsigned int node_data_1 = 1, node_data_2 = 2, node_data_3 = 3;

	init_list(&list);
	node_1.pData = &node_data_1;
	node_2.pData = &node_data_2;
	node_3.pData = &node_data_3;

	add_list_node(&list, &node_1);
	add_list_node(&list, &node_2);
	add_list_node(&list, &node_3);

	iw_printf("data:%d->%d->%d\n",
		*(unsigned int *)list.head->pData, 
		*(unsigned int *)list.head->next->pData, 
		*(unsigned int *)list.head->next->next->pData);
	iw_printf("head:x%x->x%x (%d)\n", 
		list.head , &node_3, list.nodeNum);
}

uint8_t co2task_on = false;
uint8_t co2task_status = false;
void Co2ReadSensorTaskOnOff(uint8_t on)
{
	co2task_on = on;
}
uint8_t Co2ReadSensorTaskStatus(void)
{
	return co2task_status;
}
void Co2ReadSensorTask(void *args)
{
	unsigned char dev_id = 0;
	unsigned int co2;
	int ret = IW_FAIL;
	power_mode_t power;
	uint32_t period = 3;
	uint32_t tick = 0;

	uint8_t mode_seq = 0;
	uint32_t mode_tick = 0;
	uint8_t capture = 0;

	iw_printf("%s -> start\n", __func__);

	ret = get_sensor_devid(SENSOR_TYPE_CO2, NULL, &dev_id);
	// if(iw_init_sensor(dev_id) == IW_FAIL) {
	// 	iw_printf("%s -> iw_init_sensor() err\n", __func__);
	// 	co2task_status = false;
	// }

	if(ret == IW_SUCCESS) {
		while(1) {
			//iw_put_digital(PIN_GREEN_LED, DIGITAL_LOW);
#if 1
			iw_get_power_mode(&power);
#else
			power = POW_RUN_MODE;
#endif
			if(power == POW_RUN_MODE) {
				if(!co2task_status) {
					iw_printf("CO2 On\n");
					if(iw_init_sensor(dev_id) == IW_SUCCESS) {
						co2task_status = true;
						period = 3;
					}
				}
			}
			else if(power == POW_DYNAMIC_MODE) {
				if(!co2task_status) {
					iw_printf("CO2 On\n");
					if(iw_init_sensor(dev_id) == IW_SUCCESS) {
						co2task_status = true;
						period = 3;
					}
				}

				//iw_toggle_digital(PIN_RED_LED);

			}
			else if(power == POW_LOW_MODE) {

				//iw_toggle_digital(PIN_GREEN_LED);

				if(mode_seq == 0) {
					if(!co2task_status) {
						if(iw_init_sensor(dev_id) == IW_SUCCESS) {
							iw_printf("CO2 On\n");
							co2task_status = true;
							period = 1;
							mode_seq = 1;
							capture = 0;
						}
					}
				}
				else if(mode_seq == 1) {
					if(capture) {
						capture = 0;

						iw_printf("CO2 Off\n");
						iw_deinit_sensor(dev_id);
						co2task_status = false;

						mode_seq = 2;
						mode_tick = 0;
					}
				}
				else {
					mode_tick++;
					if(10 < mode_tick) {
						mode_seq = 0;
					}
				}

			}
			else {
				//iw_toggle_digital(PIN_GREEN_LED);

				if(co2task_status) {
					iw_printf("CO2 Off\n");
					iw_deinit_sensor(dev_id);
					co2task_status = false;
				}
			}

			dev_init_status_t ready;
			iw_is_ready_sensor(dev_id, &ready);

			if(ready == DEV_INITIALIZED) {
				tick++;
				if(period <= tick) {
					tick = 0;

					if(iw_read_sensor(dev_id, SENSOR_TYPE_CO2, &co2, 1) == IW_SUCCESS){
						//if(!WifiTaskServerConnectStatus()) {
							iw_printf("%s -> CO2: %d\n", __func__, (unsigned int)co2);
						//}
						g_uiCo2 = co2;
						g_uiCo2Last = co2;
						capture = 1;
						//led_toggle(LED_BLUE);
					}
					else {
						//iw_printf("%s -> CO2: err\n", __func__);
						g_uiCo2 = 0;
					}
				}
			}
			else {
				tick = 0;
			}

			// if(co2task_status != co2task_on) {
			// 	if(co2task_on) {
			// 		iw_printf("CO2 Sensor Task On\n");
			// 		iw_init_sensor(dev_id);
			// 	}
			// 	else {
			// 		iw_printf("CO2 Sensor Task Off\n");
			// 		iw_deinit_sensor(dev_id);
			// 	}
			// 	co2task_status = co2task_on;
			// }

			//iw_put_digital(PIN_GREEN_LED, DIGITAL_HIGH);
			sleep(1000);				
		} // while(1)
	}

	iw_printf("%s -> exit\n", __func__);
	exit_task();
}

void rxTask(void *args)
{
	char buf[10]={0,};
    
    while(1)
	{
    	get_cmd_str(buf);
    	//iw_printf("%s\n",buf);
	}
}

#define BLE_TEST_BUF_LEN 32
void bleTask(void *args)
{
	unsigned char dev_id = 0;
	dev_init_status_t status;
	bool found_id = false;
	int ret_val = IW_FAIL;
	char msg[32] = {0};
	int connected = NET_DISCONNECTED;
	DATE_TIME_T dt;

	iw_printf("[%s] : Start\n", __func__);

	ret_val = get_network_devid(NET_DEV_TYPE_BLE, NULL, &dev_id);
	iw_printf("[%s] : get_network_devid(%d)\n", __func__, dev_id);
	if(ret_val == IW_SUCCESS) {
		found_id = true;
	}
	
	if(found_id == true) {
		iw_init_network(dev_id);

		while(1) {
			iw_is_connected_network(dev_id, &connected);

			if(connected == NET_CONNECTED) {
				//sprintf(msg, "ble");
				// sprintf(msg, "ble %d", iw_get_tick_count());
				// iw_send_data_network(dev_id, msg, strlen(msg));
				// iw_printf("[%s] send %s\n", __func__, msg);

				iw_get_rtc_time(&dt);

				//sprintf(msg, "[%02d:%02d] IotWare CO2 : %ld", dt.min, dt.sec, g_uiCo2Last);
				sprintf(msg, "[%02d:%02d] CO2 : %ld", dt.min, dt.sec, g_uiCo2Last);
				iw_send_data_network(dev_id, msg, strlen(msg));

				iw_printf("%s -> send msg : %s\n", __func__, msg);

			}

#if 0
			/* waiting module ready */
			is_ready_network(dev_id, &status);
			if(status == DEV_INITIALIZED) {		
				rbuf = alloc_mem(BLE_TEST_BUF_LEN);
				wbuf = alloc_mem(BLE_TEST_BUF_LEN);

				if(rbuf==NULL && wbuf==NULL){
					iw_printf("alloc failed.(%s)\n",__func__);
					break;	
				}
				memset(rbuf, 0x0, BLE_TEST_BUF_LEN);
				memset(wbuf, 0x0, BLE_TEST_BUF_LEN);

				while(1){
					//iw_printf("BUF:%s\n", wbuf);
					strcpy(wbuf, "Master sended to Slave."); /* BLE MASTER MODE */
					//strcpy(wbuf, "Slave sended to Master"); /* BLE SLAVE MODE */
					if( send_data_network(dev_id, wbuf, strlen(wbuf)) == IW_SUCCESS)
					{
						unsigned int len ;
						//sleep(3000);
						ret_val = available_network(dev_id, &len);
						//printf("%d available\n", len);
						if(ret_val == IW_SUCCESS && len>0){
							unsigned int readLen = 0;
							memset(rbuf, 0x0, BLE_TEST_BUF_LEN);
							readLen = BLE_TEST_BUF_LEN-1;
							ret_val = recv_data_network(dev_id, rbuf, &readLen);
							iw_printf("received->%s\n", rbuf);
						}
					}
					iw_sleep(2000);
				}				
			}
#endif			
			iw_sleep(1000);
		}
	}
	
	if(rbuf != NULL)
		free_mem(rbuf);
	if(wbuf != NULL)
		free_mem(wbuf);
	iw_printf("ble task exit\n");
	exit_task();
}

void TestTask(void *args)
{
	//iw_init_uart(DEBUG_UART_PORT, DEBUG_UART_BAUDRATE);
	
	iw_printf("[%s] start\n", __func__);
	//bsp_put_digital(PIN_GREEN_LED, DIGITAL_LOW);

	//iw_create_mutex(&temp_hum_sens);

	//iw_printf("system clock %d %d\n", HAL_RCC_GetSysClockFreq(), HSE_VALUE);

	while (1) {
		//iw_printf("[%s] %d\n", __func__, iw_get_tick_count());
		iw_led_toggle(LED_RED);
		iw_sleep(1000);
	}
}

void TestLedTask(void *args)
{
	uint8_t tg = 0;
	//iw_init_uart(DEBUG_UART_PORT, DEBUG_UART_BAUDRATE);
	
	iw_printf("test task start\n");
	//bsp_put_digital(PIN_GREEN_LED, DIGITAL_LOW);

	//iw_create_mutex(&temp_hum_sens);

	//iw_printf("system clock %d %d\n", HAL_RCC_GetSysClockFreq(), HSE_VALUE);

	while (1) {
		iw_printf("test task %d\n", iw_get_tick_count());
		if(tg == 0) {
			tg = 1;
			iw_put_digital(PIN_RED_LED, DIGITAL_LOW);
		}
		else {
			tg = 0;
			iw_put_digital(PIN_RED_LED, DIGITAL_HIGH);
		}
		//iw_led_toggle(LED_RED);
		iw_sleep(1000);
	}
}

void start_apps(void *args)
{	
#if (IW_MCU_STM32==1)
//	create_task(tempReadSensorTask, "Temperature", 0x100, NULL, IW_TSK_PRIO_IDLE+2, NULL);
//	create_task(humReadSensorTask, "Humidity", 0x100, NULL, IW_TSK_PRIO_IDLE+2, NULL);
	create_task(Co2ReadSensorTask, "CO2", 0x200, NULL, IW_TSK_PRIO_IDLE+2, NULL);
	create_task(WifiTask, "WifiTask", 0x200, NULL, IW_TSK_PRIO_IDLE+2, NULL);
//	create_task(loraTask, "loraTask", 0x100, NULL, IW_TSK_PRIO_IDLE+2, NULL);
	//create_task(rxTask, "rxTask", 0x100, NULL, IW_TSK_PRIO_IDLE+2, NULL);
//	create_task(bleTask, "bleTask", 0x100, NULL, IW_TSK_PRIO_IDLE+2, NULL);
//	create_task(TestTask, "TestTask", 0x100, NULL, IW_TSK_PRIO_IDLE+2, NULL);
#elif (IW_MCU_NORDIC==1)
	create_task(Co2ReadSensorTask, "CO2", 0x200, NULL, IW_TSK_PRIO_IDLE+3, NULL);
// #if (IW_FREERTOS == 1)
// //	create_task(tempReadSensorTask, "Temperature", 0x100, NULL, IW_TSK_PRIO_IDLE+2, NULL);
// //	create_task(humReadSensorTask, "Humidity", 0x100, NULL, IW_TSK_PRIO_IDLE+2, NULL);
// //	create_task(WifiTask, "WifiTask", 0x200, NULL, IW_TSK_PRIO_IDLE+2, NULL);
// //	create_task(loraTask, "loraTask", 0x100, NULL, IW_TSK_PRIO_IDLE+2, NULL);
// 	//create_task(rxTask, "rxTask", 0x100, NULL, IW_TSK_PRIO_IDLE+2, NULL);
// //	create_task(bleTask, "bleTask", 0x200, NULL, IW_TSK_PRIO_IDLE+2, NULL);
// //	create_task(TestTask, "TestTask", 0x200, NULL, IW_TSK_PRIO_IDLE+1, NULL);
// #elif (IW_NANOQPLUS == 1)
// //	create_task(tempReadSensorTask, "Temperature", 0x100, NULL, IW_TSK_PRIO_IDLE+2, NULL);
// //	create_task(humReadSensorTask, "Humidity", 0x100, NULL, IW_TSK_PRIO_IDLE+2, NULL);
// 	create_task(Co2ReadSensorTask, "CO2", 0x200, NULL, IW_TSK_PRIO_IDLE+1, NULL);
// 	create_task(WifiTask, "WifiTask", 0x200, NULL, IW_TSK_PRIO_IDLE+2, NULL);
// //	create_task(loraTask, "loraTask", 0x100, NULL, IW_TSK_PRIO_IDLE+2, NULL);
// 	//create_task(rxTask, "rxTask", 0x100, NULL, IW_TSK_PRIO_IDLE+2, NULL);
// //	create_task(bleTask, "bleTask", 0x200, NULL, IW_TSK_PRIO_IDLE+2, NULL);
// 	create_task(TestTask, "TestTask", 0x100, NULL, IW_TSK_PRIO_IDLE+2, NULL);
// #elif (IW_RIOT == 1)
// //	create_task(tempReadSensorTask, "Temperature", 0x100, NULL, IW_TSK_PRIO_IDLE+2, NULL);
// //	create_task(humReadSensorTask, "Humidity", 0x100, NULL, IW_TSK_PRIO_IDLE+2, NULL);
// 	create_task(Co2ReadSensorTask, "CO2", 0x200, NULL, IW_TSK_PRIO_IDLE+1, NULL);
// //	create_task(WifiTask, "WifiTask", 0x200, NULL, IW_TSK_PRIO_IDLE+2, NULL);
// //	create_task(loraTask, "loraTask", 0x100, NULL, IW_TSK_PRIO_IDLE+2, NULL);
// 	//create_task(rxTask, "rxTask", 0x100, NULL, IW_TSK_PRIO_IDLE+2, NULL);
// 	create_task(bleTask, "bleTask", 0x100, NULL, IW_TSK_PRIO_IDLE+2, NULL);
// 	create_task(TestTask, "TestTask", 0x100, NULL, IW_TSK_PRIO_IDLE+3, NULL);
// #else
// #endif
#else
	create_task(TestLedTask, "TestLedTask", 0x100, NULL, IW_TSK_PRIO_IDLE+3, NULL);
#endif // (IW_MCU_STM32==1)
}
#elif (IOTWARE_MODE == 2)

#include "interface_all.h"


void LowpowerTask(void *args)
{
	uint32_t co2_period = 30;
	uint32_t co2_tick = 0;
	uint32_t wifi_period = 100;
	uint32_t wifi_duty = 30;
	uint32_t wifi_tick = 0;
	iw_error_t ret;
	uint8_t dev_id;
	unsigned int co2;

	iw_put_digital(PIN_BLE_PWR_EN, DIGITAL_LOW);
	iw_put_digital(PIN_LORA_PWR_EN, DIGITAL_LOW);
	iw_put_digital(PIN_TEMP_PWR_EN, DIGITAL_LOW);
	iw_put_digital(PIN_WIFI_PWR_EN, DIGITAL_LOW);
	iw_put_digital(PIN_CO2_PWR_EN, DIGITAL_LOW);

	iw_put_digital(PIN_RED_LED, DIGITAL_HIGH);
	iw_put_digital(PIN_GREEN_LED, DIGITAL_HIGH);

	get_sensor_devid(SENSOR_TYPE_CO2, NULL, &dev_id);
	iw_init_sensor(dev_id);

#if LOWPOWER_ENABLE==1
	iw_printf("LOW POWER ENABLE\n");
	co2_period = 100;
	wifi_duty = 0;
#else
	co2_period = 30;
	wifi_duty = 24;
#endif

	while(1) {
		co2_tick++;
		if(co2_period < co2_tick) {
			co2_tick = 0;

			if(iw_read_sensor(dev_id, SENSOR_TYPE_CO2, &co2, 1) == IW_SUCCESS){
				iw_printf("CO2: %d\n", co2);
			}
			else {
				// iw_deinit_sensor(dev_id);
				// iw_sleep(100);
				// iw_init_sensor(dev_id);
			}

			iw_toggle_digital(PIN_RED_LED);
		}

		wifi_tick++;
		if(wifi_period < wifi_tick) wifi_tick = 0;
		if((0 < wifi_duty) && wifi_tick <= (wifi_period * wifi_duty / 100)) {
			//iw_put_digital(PIN_BLE_PWR_EN, DIGITAL_HIGH);
			iw_put_digital(PIN_WIFI_PWR_EN, DIGITAL_HIGH);
			//iw_put_digital(PIN_LORA_PWR_EN, DIGITAL_HIGH);
			//iw_put_digital(PIN_TEMP_PWR_EN, DIGITAL_HIGH);
			//iw_put_digital(PIN_RED_LED, DIGITAL_LOW);
		}
		else {
			//iw_put_digital(PIN_BLE_PWR_EN, DIGITAL_LOW);
			iw_put_digital(PIN_WIFI_PWR_EN, DIGITAL_LOW);
			//iw_put_digital(PIN_LORA_PWR_EN, DIGITAL_HIGH);
			//iw_put_digital(PIN_TEMP_PWR_EN, DIGITAL_HIGH);
			//iw_put_digital(PIN_RED_LED, DIGITAL_HIGH);
		}

		iw_sleep(100);
	}

}


void start_apps(void *args)
{	
	create_task(LowpowerTask, "LowPower", 0x200, NULL, IW_TSK_PRIO_IDLE+2, NULL);
}
#endif /* IOTWARE_MODE? */
