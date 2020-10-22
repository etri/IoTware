/*
Copyright (C) <2020> <ETRI>

This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 3.0 of the License, or (at your option) any later version. This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details. You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/

#include "interface_net.h"
#include "service.h"

uint8_t get_registered_net_num(void)
{
	return iw_get_registered_net_num();
}

iw_error_t get_all_networks_info(registered_net_info_t *info)
{
	return iw_get_all_networks_info(info);
}

iw_error_t get_network_devid(net_type_t type, char *name, unsigned char *devid)
{
	return iw_get_network_devid(type, name, devid);
}

iw_error_t get_network_dev_id_by_type(net_type_t type, uint8_t *dev_id)
{
	return iw_get_network_dev_id_by_type(type, dev_id);
}

iw_error_t get_network_dev_id_by_name(char *name, uint8_t *dev_id)
{
	return iw_get_network_dev_id_by_name(name, dev_id);
}

iw_error_t get_network_info(net_type_t type, char *name, registered_net_info_t *info)
{
	return iw_get_network_info(type, name, info); 
}

iw_error_t init_network(unsigned char dev_id)
{
	return iw_init_network(dev_id);
}

iw_error_t deinit_network(unsigned char dev_id)		
{
	return iw_deinit_network(dev_id);
}

iw_error_t is_ready_network(unsigned char dev_id, dev_init_status_t *status) 
{
	return iw_is_ready_network(dev_id, status);
}

iw_error_t available_network(unsigned char dev_id, unsigned int *len)
{
	return iw_available_network(dev_id, len);
}

iw_error_t recv_data_network(unsigned char dev_id, char *buf, unsigned int *len)
{
	return iw_recv_data_network(dev_id, buf, len);
}

iw_error_t send_data_network( unsigned char dev_id, char *buf, unsigned int len)
{
    if(get_lowbattery_control()  == 1)
    {
        printf("autocontrol_lowbattery network blocking\n");
		return IW_SUCCESS;
    }
	else
	{
	    return iw_send_data_network(dev_id, buf, len);
	}
}

int connect_network(unsigned char dev_id, unsigned int timeout)
{
	return iw_connect_network(dev_id, timeout);
}

iw_error_t disconnect_network(unsigned char dev_id)
{
	return iw_disconnect_network(dev_id);
}

iw_error_t is_connected_network(unsigned char dev_id, int *connected)
{
	return iw_is_connected_network(dev_id, connected);
}

int scan_network(unsigned char dev_id, int *rssi)
{
	return iw_scan_network(dev_id, rssi);
}

iw_error_t get_capa_network(unsigned char dev_id, wake_src_type_t *supported)
{
	return iw_get_capa_network(dev_id, supported);
}

iw_error_t set_config_network(unsigned char dev_id, wake_src_type_t wake_src, void *config)
{
	return iw_set_config_network(dev_id,  wake_src, config);
}

iw_error_t get_config_network(unsigned char dev_id, wake_src_type_t *wake_src, void *config)
{
	return iw_get_config_network(dev_id,  wake_src, config);
}

iw_error_t set_pwrmode_network(unsigned char dev_id, net_pwr_mode_t pwr_mode)
{
	return iw_set_pwrmode_network(dev_id,  pwr_mode);
}

iw_error_t get_pwrmode_network(unsigned char dev_id, net_pwr_mode_t *pwr_mode) 
{
	return iw_get_pwrmode_network(dev_id, pwr_mode); 
}

iw_error_t get_rssi_network(unsigned char dev_id, int *rssi)
{
	return iw_get_rssi_network(dev_id, rssi);
}

int connect_server(unsigned char dev_id, char *address, int port, unsigned int timeout, int proto)
{
	return iw_connect_server(dev_id, address, port, timeout, proto);
}

int disconnect_server(unsigned char dev_id)
{
	return iw_disconnect_server(dev_id);
}
