/*
Copyright (C) <2020> <ETRI>

This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 3.0 of the License, or (at your option) any later version. This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details. You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/
#ifndef IW_DEV_H
#define IW_DEV_H

#define MAX_DRIVER_NAME_LEN 20

typedef enum{
	DEVICE_TYPE_SENSOR =0,
	DEVICE_TYPE_NETWORK,
}device_type_t;


typedef enum{
	WAKEUP_SRC_NONE			= 0,
	WAKEUP_SRC_INTERRUPT 	= ( 1 << 0 ), 
	WAKEUP_SRC_TIMER 		= ( 1 << 1 ),
}wake_src_type_t;


typedef enum{
	DEV_NOT_INITIALIZED = 0,
	DEV_INITIALIZED,
	DEV_INIT_ERROR,
	DEV_DEINITIALIZED,
}dev_init_status_t;


void iw_register_device(void);

void iw_register_wifi_info(char *ssid, char *pw);
void iw_register_server_info(char *server_address, unsigned int port);


#endif /* IW_DEV_H */

