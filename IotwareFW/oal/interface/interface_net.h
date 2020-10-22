/*
Copyright (C) <2020> <ETRI>

This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 3.0 of the License, or (at your option) any later version. This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details. You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/

#ifndef INTERFACE_NET_H
#define INTERFACE_NET_H

#include "iw_common.h"
#include "iw_dev_net.h"

uint8_t get_registered_net_num(void);
iw_error_t get_all_networks_info(registered_net_info_t *info);
iw_error_t init_network(unsigned char dev_id);
iw_error_t is_ready_network(unsigned char dev_id, dev_init_status_t *status); 
iw_error_t deinit_network(unsigned char dev_id);
iw_error_t available_network(unsigned char dev_id, unsigned int *len);
iw_error_t recv_data_network(unsigned char dev_id, char *buf, unsigned int *len);
iw_error_t send_data_network( unsigned char dev_id, char *buf, unsigned int len);
int connect_network(unsigned char dev_id, unsigned int timeout);
iw_error_t disconnect_network(unsigned char dev_id);
iw_error_t is_connected_network(unsigned char dev_id, int *connected);
int scan_network(unsigned char dev_id, int *rssi);	
iw_error_t get_capa_network(unsigned char dev_id, wake_src_type_t *wake_src);
iw_error_t set_config_network(unsigned char dev_id, wake_src_type_t wake_src, void *config);
iw_error_t get_config_network(unsigned char dev_id, wake_src_type_t *wake_src, void *config);
iw_error_t set_pwrmode_network(unsigned char dev_id, net_pwr_mode_t pwr_mode);
iw_error_t get_pwrmode_network(unsigned char dev_id, net_pwr_mode_t *pwr_mode); 
iw_error_t get_rssi_network(unsigned char dev_id, int *rssi);
int connect_server(unsigned char dev_id, char *address, int port, unsigned int timeout, int proto);
int disconnect_server(unsigned char dev_id);
iw_error_t get_network_info(net_type_t type,  char *name, registered_net_info_t *info);
iw_error_t get_network_devid(net_type_t type, char *name, unsigned char *devid);
iw_error_t get_network_dev_id_by_type(net_type_t type, uint8_t *dev_id);
iw_error_t get_network_dev_id_by_name(char *name, uint8_t *dev_id);
int get_connected_network(unsigned char dev_id);

#endif /* INTERFACE_NET_H */
