/*
Copyright (C) <2020> <ETRI>

This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 3.0 of the License, or (at your option) any later version. This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details. You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/


#include "iw_common.h"
#include "iw_dev.h"
#include "iw_dev_net.h"
#include "iw_oal.h"

static net_info_t neti; /* network information */
extern unsigned int g_uart_using_info[IF_ID_MAX]; 
extern unsigned char g_i2c_using_info[IF_ID_MAX]; 
static wifi_ap_info_t gApInfo;
static lora_join_info_t gLoraJoinInfo;
static char gLoraBand[LORA_BAND_NAME_LEN+1]={0,};
static ble_info_t gBleInfo;
static iw_task_t connection_task_handle;


void netConnectionTask(void *args);

void iw_register_network(net_type_t type, if_type_t iface, if_id_t iface_id, int info, char *name)
{
	unsigned char dev_id=neti.dev_num;
	net_dev_info_t *dev = &neti.dev[dev_id];

	if(iface == IF_TYPE_UART){
		if(g_uart_using_info[iface_id]==0)
			g_uart_using_info[iface_id] = info;
		else{
			iw_printf("Check (%d uart%d %s)\n",type, iface_id, name);
			return;
		}
	}
	else if(iface == IF_TYPE_I2C){
		if(g_i2c_using_info[iface_id]==0)
			g_i2c_using_info[iface_id] = 1;
		else{
			iw_printf("Check (%d i2c%d %s)\n",type, iface_id, name);
			return;	
		}

	}

	dev->type = type; /* sensor type */
	dev->iface = iface;
	dev->iface_id = iface_id;
	memcpy(dev->name, name, MAX_DRIVER_NAME_LEN); 
	neti.dev[dev_id].dev_id = dev_id+1;
	neti.dev_num++;
}

void iw_register_network_driver(net_driver_t *net_driver)
{
	unsigned char driver_id = neti.driver_num;
	net_driver_t *driver = &neti.driver[driver_id];

	memcpy(driver->name , net_driver->name, MAX_DRIVER_NAME_LEN);
	driver->func = net_driver->func;
	neti.driver_num++;
}

iw_error_t iw_register_network_(net_type_t dev_type, const char *name, if_type_t if_type, uint32_t if_num, void *if_arg, net_driver_t *dev_driver)
{
	if(MAX_NET_DEV <= neti.dev_num) return IW_FAIL;

	uint8_t dev_id = neti.dev_num;
	net_dev_info_t *dev = &neti.dev[dev_id];

	dev->type = dev_type;							// device type
	strncpy(dev->name, name, MAX_DRIVER_NAME_LEN); 
	dev->iface = if_type;							// device interface type
	dev->iface_id = if_num;							// device interface id
	//neti.dev[dev_id].dev_id = dev_id+1;
	dev->func = dev_driver->func;
	neti.dev_num++;
	
	//iw_printf("iw_register_network_(%d, %s, %d, %d)\n", dev->type, dev->name, dev->iface, dev->iface_id);

	return IW_SUCCESS;
}

uint8_t iw_get_registered_net_num(void)
{
	return neti.dev_num;
}

iw_error_t iw_get_network_devid(net_type_t type, char *name, unsigned char *devid)
{
	net_dev_info_t *dev;
	int i = 0, found = 0;

	for(i=0; i<neti.dev_num; i++)	
	{
		dev = &neti.dev[i];

		if(type>0 && (dev->type & type)){
			if(name != NULL){
				if(!strcmp(name, (char *)dev->name)){
					*devid = i;
					found = 1;
					break;
				}				
			}else{				
				*devid = i;
				found = 1;
			}
		}		
	}

	if(found)
		return IW_SUCCESS;	

	return IW_FAIL;
}

iw_error_t iw_get_network_dev_id_by_type(net_type_t type, uint8_t *dev_id)
{
	int32_t i;
	for(i = 0; i < neti.dev_num; i++) {
		net_dev_info_t *dev = &neti.dev[i];

		if((dev->type & type) == type) {
			*dev_id = i;
			return IW_SUCCESS;
		}
	}

	return IW_FAIL;
}

iw_error_t iw_get_network_dev_id_by_name(char *name, uint8_t *dev_id)
{
	int32_t i; 
	for(i = 0; i < neti.dev_num; i++) {
		net_dev_info_t *dev = &neti.dev[i];

		if(dev->type != 0 && strcmp(name, dev->name) == 0) {
			*dev_id = i;
			return IW_SUCCESS;
		}		
	}

	return IW_FAIL;
}

iw_error_t iw_get_all_networks_info(registered_net_info_t *info)
{
	net_dev_info_t *dev;
	unsigned char dev_id;

	for(dev_id=0; dev_id<neti.dev_num; dev_id++)	
	{
		dev = &neti.dev[dev_id];
		info[dev_id].type=dev->type;
		info[dev_id].dev_id = dev_id;
		memcpy(info[dev_id].name, dev->name, strlen((char *)dev->name));
	}
	return IW_SUCCESS;
}

iw_error_t iw_get_network_info(net_type_t type, char *name, registered_net_info_t *info)
{
	net_dev_info_t *dev;
	char found = 0;
	unsigned char dev_id;
	
	if(info == NULL)
		return IW_FAIL;

	for(dev_id=0; dev_id<neti.dev_num; dev_id++)	
	{
		dev = &neti.dev[dev_id];
		if(dev->type == type){
			if(found == 0 || (found > 0 && name != NULL && !strcmp(dev->name, name))){
				info[dev_id].type=dev->type;
				info[dev_id].dev_id = dev_id;
				memcpy(info[dev_id].name, dev->name, strlen((char *)dev->name));
				if(found > 0)
					break;
				found++;
			}
		}
	}

	if(found) 
		return IW_SUCCESS;

	return IW_FAIL;
}

iw_error_t iw_init_network(unsigned char dev_id)
{
	//uint8_t driver_num = neti.driver_num;
	//uint8_t i;
	net_dev_info_t *dev = &neti.dev[dev_id];
	//uint8_t driver_found = 0;

	iw_printf("iw_init_network(%d)\n", dev_id);
	iw_sleep(100);
	if(neti.dev_num <= dev_id) return IW_FAIL;
	if(dev->type == 0) return IW_FAIL;
	if(dev->func == 0) return IW_FAIL;

	iw_error_t retval = dev->func->init((void *)dev);
	if(retval == IW_FAIL) return IW_FAIL;

#ifndef __OS_TEST__ 
	if(dev->type == NET_DEV_TYPE_WIFI || 
		dev->type == NET_DEV_TYPE_LORA/* ||
		dev->type == NET_DEV_TYPE_BLE*/) {
		connection_task_handle = iw_create_task(netConnectionTask, "netConnectionTask", 0x200, &dev->type, IW_TSK_PRIO_IDLE+5, NULL);
		if(connection_task_handle == NULL) {
			//iw_printf("iw_init_network() -> Can not create task\n");
			return IW_FAIL;
		}
	}
#endif
	
	return IW_SUCCESS;
}

iw_error_t iw_deinit_network(unsigned char dev_id)
{
	//iw_put_digital(PIN_GREEN_LED, DIGITAL_LOW);

	if(connection_task_handle) {
		//iw_suspend_task(connection_task_handle);
		iw_delete_task(connection_task_handle);
		connection_task_handle = 0;
	}

	if(neti.dev[dev_id].func && neti.dev[dev_id].func->deinit) {
		neti.dev[dev_id].func->deinit(&neti.dev[dev_id]);
	}

	//iw_put_digital(PIN_GREEN_LED, DIGITAL_HIGH);
	iw_printf("iw_deinit_network()\n");
	return IW_SUCCESS;
}

iw_error_t iw_is_ready_network(unsigned char dev_id, dev_init_status_t *status) 
{
	if(neti.dev[dev_id].func && neti.dev[dev_id].func->is_ready)
		return neti.dev[dev_id].func->is_ready(&neti.dev[dev_id], status);
	return IW_FAIL;
}

iw_error_t iw_available_network(unsigned char dev_id, unsigned int *len)
{
	if(neti.dev[dev_id].func && neti.dev[dev_id].func->available)
		return neti.dev[dev_id].func->available(&neti.dev[dev_id], len);
	else
		return IW_FAIL;	
}

iw_error_t iw_recv_data_network (unsigned char dev_id, char *buff, unsigned int *len)
{
	if(neti.dev[dev_id].func && neti.dev[dev_id].func->recv_data)
		return neti.dev[dev_id].func->recv_data(&neti.dev[dev_id],buff, len);
	else
		return IW_FAIL; 
}

iw_error_t iw_send_data_network (unsigned char dev_id, char *buff, unsigned int len)
{
	if(neti.dev[dev_id].func && neti.dev[dev_id].func->send_data)
		return neti.dev[dev_id].func->send_data(&neti.dev[dev_id],buff, len);
	else
		return IW_FAIL; 
}

int iw_connect_network(unsigned char dev_id, unsigned int timeout)
{
	if(neti.dev[dev_id].func && neti.dev[dev_id].func->connect)
		return neti.dev[dev_id].func->connect(&neti.dev[dev_id], timeout);
	else
		return NET_FAIL;
}

iw_error_t iw_disconnect_network(unsigned char dev_id)
{
	if(neti.dev[dev_id].func && neti.dev[dev_id].func->disconnect)
		return neti.dev[dev_id].func->disconnect(&neti.dev[dev_id]);
	else
		return IW_FAIL;
}

iw_error_t iw_is_connected_network(unsigned char dev_id, int *connected)
{
	if(neti.dev[dev_id].func && neti.dev[dev_id].func->is_connected)
		return neti.dev[dev_id].func->is_connected(&neti.dev[dev_id], connected);
	else
		return IW_FAIL;
}

int iw_scan_network(unsigned char dev_id, int *rssi)
{
	if(neti.dev[dev_id].func && neti.dev[dev_id].func->scan)
		return neti.dev[dev_id].func->scan(&neti.dev[dev_id], rssi);
	else
		return NET_FAIL;
}

iw_error_t iw_get_capa_network(unsigned char dev_id, wake_src_type_t *wake_src)
{
	if(neti.dev[dev_id].func && neti.dev[dev_id].func->get_capa)
		return neti.dev[dev_id].func->get_capa(&neti.dev[dev_id], wake_src);
	else
		return IW_FAIL;
}

iw_error_t iw_set_config_network(unsigned char dev_id, wake_src_type_t wake_src, void *config)
{
	if(neti.dev[dev_id].func && neti.dev[dev_id].func->set_config)
		return neti.dev[dev_id].func->set_config(&neti.dev[dev_id], wake_src, config);
	else
		return IW_FAIL;
}

iw_error_t iw_get_config_network(unsigned char dev_id, wake_src_type_t *wake_src, void *config)
{
	if(neti.dev[dev_id].func && neti.dev[dev_id].func->get_config)
		return neti.dev[dev_id].func->get_config(&neti.dev[dev_id], wake_src, config);
	else
		return IW_FAIL;
}

iw_error_t iw_set_pwrmode_network(unsigned char dev_id, net_pwr_mode_t pwr_mode)
{
	if(neti.dev[dev_id].func && neti.dev[dev_id].func->set_power)
		return neti.dev[dev_id].func->set_power(&neti.dev[dev_id],pwr_mode);
	else
		return IW_FAIL;
}

iw_error_t iw_get_pwrmode_network(unsigned char dev_id, net_pwr_mode_t *pwr_mode)
{
	if(neti.dev[dev_id].func && neti.dev[dev_id].func->get_power)
		return neti.dev[dev_id].func->get_power(&neti.dev[dev_id],pwr_mode);
	else
		return IW_FAIL;
}

iw_error_t iw_get_rssi_network(unsigned char dev_id, int *rssi)
{
	//iw_printf("iw_get_rssi_network()\n");
	
	if(neti.dev[dev_id].func && neti.dev[dev_id].func->get_rssi)
		return neti.dev[dev_id].func->get_rssi(&neti.dev[dev_id], rssi);
	else
		return IW_FAIL; 
}


int iw_connect_server(unsigned char dev_id, char *address, int port, unsigned int timeout, int proto)
{
	if(neti.dev[dev_id].func && neti.dev[dev_id].func->connect_server)
		return neti.dev[dev_id].func->connect_server(&neti.dev[dev_id], address, port, timeout, proto);
	else
		return NET_FAIL;
}

int iw_disconnect_server(unsigned char dev_id)
{
	if(neti.dev[dev_id].func && neti.dev[dev_id].func->disconnect_server)
		return neti.dev[dev_id].func->disconnect_server(&neti.dev[dev_id]);
	else
		return NET_FAIL;
}

void iw_register_wifi_info(char *ssid, char *pw)
{
	if(ssid != NULL && ssid[0] != 0)
		memcpy(gApInfo.ssid, ssid, strlen(ssid));
	
	if(pw != NULL && pw[0] != 0)
		memcpy(gApInfo.pw, pw, strlen(pw));

	//iw_printf("iw_register_wifi_info(%s, %s)\n", gApInfo.ssid, gApInfo.pw);	
}

iw_error_t iw_get_wifi_info(wifi_ap_info_t *ap)//char *ssid, char *pw)
{
	if(gApInfo.ssid[0] != 0){
		memcpy(ap->ssid, gApInfo.ssid, strlen(gApInfo.ssid));

		if(gApInfo.pw[0] != 0)
			memcpy(ap->pw, gApInfo.pw, strlen(gApInfo.pw));

		return IW_SUCCESS;
	}

	return IW_FAIL;
}

iw_error_t iw_register_lora_abp_info(char *dev_addr, char *nwks_key, char *apps_key)
{
	if(dev_addr == NULL || nwks_key == NULL || apps_key == NULL)
		return IW_FAIL;
	gLoraJoinInfo.join_mode = JOIN_ABP;
	memset(gLoraJoinInfo.dev_addr, 0x0, LORA_JOIN_DEV_ADDR+1);
	memcpy(gLoraJoinInfo.dev_addr, dev_addr, strlen(dev_addr));

	memset(gLoraJoinInfo.nwks_key, 0x0, LORA_JOIN_NWKS_KEY+1);
	memcpy(gLoraJoinInfo.nwks_key, nwks_key, strlen(nwks_key));
	
	memset(gLoraJoinInfo.apps_key, 0x0, LORA_JOIN_APPS_KEY+1);
	memcpy(gLoraJoinInfo.apps_key, apps_key, strlen(apps_key));

	return IW_SUCCESS;
}

iw_error_t iw_register_lora_otaa_info(char *dev_eui, char *app_eui, char *app_key)
{
	if(dev_eui == NULL || app_eui == NULL || app_key == NULL)
		return IW_FAIL;
	
	gLoraJoinInfo.join_mode = JOIN_OTAA;
	memset(gLoraJoinInfo.dev_eui, 0x0, LORA_JOIN_DEV_EUI+1);
	memcpy(gLoraJoinInfo.dev_eui, dev_eui, strlen(dev_eui));

	memset(gLoraJoinInfo.app_eui, 0x0, LORA_JOIN_APP_EUI+1);
	memcpy(gLoraJoinInfo.app_eui, app_eui, strlen(app_eui));
	
	memset(gLoraJoinInfo.app_key, 0x0, LORA_JOIN_APP_KEY+1);
	memcpy(gLoraJoinInfo.app_key, app_key, strlen(app_key));

	return IW_SUCCESS;
}

iw_error_t iw_get_lora_info(lora_join_info_t *join)
{
	if(join == NULL)
		return IW_FAIL;

	if(gLoraJoinInfo.join_mode == JOIN_ABP){
		join->join_mode = JOIN_ABP;

		if(strlen(gLoraJoinInfo.dev_addr) != LORA_JOIN_DEV_ADDR ||
			strlen(gLoraJoinInfo.nwks_key) != LORA_JOIN_NWKS_KEY ||
			strlen(gLoraJoinInfo.apps_key) != LORA_JOIN_APPS_KEY){
			iw_printf("Check Lora info\n");
			return IW_FAIL;	
		}

		if(gLoraJoinInfo.dev_addr != NULL){
			memcpy(join->dev_addr, gLoraJoinInfo.dev_addr, strlen(gLoraJoinInfo.dev_addr));
		}else
			return IW_FAIL;

		if(gLoraJoinInfo.nwks_key != NULL){
			memcpy(join->nwks_key, gLoraJoinInfo.nwks_key, strlen(gLoraJoinInfo.nwks_key));
		}else
			return IW_FAIL;

		if(gLoraJoinInfo.apps_key != NULL){
			memcpy(join->apps_key, gLoraJoinInfo.apps_key, strlen(gLoraJoinInfo.apps_key));
		}else
			return IW_FAIL;			 
	}else if(gLoraJoinInfo.join_mode == JOIN_OTAA){
		join->join_mode = JOIN_OTAA;

		if(strlen(gLoraJoinInfo.dev_eui) != LORA_JOIN_DEV_EUI ||
			strlen(gLoraJoinInfo.app_eui) != LORA_JOIN_APP_EUI ||
			strlen(gLoraJoinInfo.app_key) != LORA_JOIN_APP_KEY){
			iw_printf("Check Lora info\n");
			return IW_FAIL;	
		}

		if(gLoraJoinInfo.dev_eui != NULL){
			memcpy(join->dev_eui, gLoraJoinInfo.dev_eui, strlen(gLoraJoinInfo.dev_eui));
		}else
			return IW_FAIL;

		if(gLoraJoinInfo.app_eui != NULL){
			memcpy(join->app_eui, gLoraJoinInfo.app_eui, strlen(gLoraJoinInfo.app_eui));
		}else
			return IW_FAIL;

		if(gLoraJoinInfo.app_key != NULL){
			memcpy(join->app_key, gLoraJoinInfo.app_key, strlen(gLoraJoinInfo.app_key));
		}else
			return IW_FAIL;
	}
	else
		return IW_FAIL;
	//iw_printf("join:%d\n",gLoraJoinInfo.join_mode );
	return IW_SUCCESS;
}

iw_error_t iw_register_lora_band_info(char *band)
{
	if(band == NULL)
		return IW_FAIL;
	if(strlen(band) != LORA_BAND_NAME_LEN)
		return IW_FAIL;
	strncpy(gLoraBand, band, LORA_BAND_NAME_LEN);
	return IW_SUCCESS;
}

iw_error_t iw_get_lora_band_info(char *band)
{
	if(band == NULL || gLoraBand[0]==0)
		return IW_FAIL;
	
	strncpy(band, gLoraBand, LORA_BAND_NAME_LEN);
	return IW_SUCCESS;	
}

iw_error_t iw_register_ble_info(char *name, char *pincode, ble_mode_t mode)
{
	if(name!=NULL && name[0]!=0)
		strncpy(gBleInfo.name, name, strlen(name));
	if(pincode!=NULL && pincode[0]!=0)
		strncpy(gBleInfo.pincode, pincode, PINCODE_LEN);
	if(mode==BLE_MASTER_MODE || mode==BLE_SLAVE_MODE)
		gBleInfo.mode = mode;
	return IW_SUCCESS;
}

iw_error_t iw_get_ble_info(ble_info_t *info)
{
	if(info == NULL)
		return IW_FAIL;

	*info = gBleInfo;
	return IW_SUCCESS;
}


void netConnectionTask(void *args)
{
	net_type_t type = *(net_type_t *)args;
	int rssi;
	iw_error_t retval = IW_FAIL;
	int net_retval = 0;
	dev_init_status_t dev_status = DEV_NOT_INITIALIZED;
	unsigned char dev_id = 0;
	//bool found_id = false;
	int connected_status = 0;

	iw_printf("netConnectionTask start\n");

	retval = iw_get_network_devid(type, NULL, &dev_id);

	if(retval == IW_SUCCESS) {
		do {
			retval = iw_is_connected_network(dev_id, &connected_status);
			//iw_printf("iw_is_connected_network(%d, %d) %d\n", dev_id, connected_status, retval);
			if(retval == IW_SUCCESS) {
				if((type == NET_DEV_TYPE_WIFI && connected_status == NET_GOT_IP) || (type == NET_DEV_TYPE_LORA && connected_status == NET_CONNECTED)) {
					/* do nothing */
					iw_sleep(1000);
				}
				else {			
					/* try connecting ap */
					retval = iw_is_ready_network(dev_id, &dev_status);
					//iw_printf("iw_is_ready_network(%d, %d) %d\n", dev_id, dev_status, retval);
					if(retval == IW_SUCCESS) {
						if(dev_status == DEV_INITIALIZED) {
							//iw_printf("net dev ready\n");

							net_retval = iw_scan_network(dev_id, &rssi);
							if(net_retval == NET_SUCCESS) {
								if(type == NET_DEV_TYPE_WIFI) {
									iw_printf("rssi:%d\n", rssi);
								}

								net_retval = iw_connect_network(dev_id, 20*1000);
								if(net_retval == NET_SUCCESS) {
									iw_printf("success connect!\n");
								}
								else if(net_retval == NET_FAIL) {
									iw_printf("fail to connect!\n");
								}
								else {
									iw_printf("connection timeout!\n");	
								}
								iw_sleep(1000);
							}
							else {
								if(net_retval == NET_AP_NOT_FOUND) {
									iw_printf("err: ap not found.\n");
									iw_sleep(1000);
								}
								else if(net_retval == NET_TIMEOUT) {
									iw_printf("err: timeout\n");	
								}
								else {
									iw_printf("err:%d\n", net_retval);
								}
								iw_sleep(1000);	
							}			
						}
						else if(dev_status == DEV_INIT_ERROR) {
							break;
						}
						else {
							iw_sleep(1000);
						}
					}
					else {
						iw_sleep(1000);
					}
				}
			} // if(retval == IW_SUCCESS)
			else {
				iw_sleep(1000);
			}
			//iw_toggle_digital(PIN_GREEN_LED);
		} while(1);
	}

	iw_printf("%s exit\n", __func__);
	iw_exit_task();
}
