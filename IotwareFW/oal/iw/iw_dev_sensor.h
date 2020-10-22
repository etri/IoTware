/*
Copyright (C) <2020> <ETRI>

This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 3.0 of the License, or (at your option) any later version. This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details. You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/
#ifndef IW_DEV_SENSOR_H
#define IW_DEV_SENSOR_H

#include "iw_if.h"
#include "iw_dev.h"

#define MAX_SENSOR_DRIVER	5
#define MAX_SENSOR_DEV 		5

typedef enum{
	SENSOR_TYPE_NONE		= 0,
 	SENSOR_TYPE_TMP		    = (1 << 0),
 	SENSOR_TYPE_CO2		    = (1 << 1),
 	SENSOR_TYPE_HUM		    = (1 << 2),
	SENSOR_TYPE_ACC		    = (1 << 3), 
	SENSOR_TYPE_PPG		    = (1 << 4),
	SENSOR_TYPE_ECG		    = (1 << 5),
	SENSOR_TYPE_GYR		    = (1 << 6),	
}sensor_type_t;


typedef enum{	
 	SENSOR_PWR_RUN_MODE		= 0,
 	SENSOR_PWR_SLEEP_MODE	,
 	SENSOR_PWR_OFF_MODE		,
 	SENSOR_PWR_AUTO_MODE 	, 
 	SENSOR_PWR_MAX			,
}sensor_pwr_mode_t;


typedef struct{
	iw_error_t (*init)(void *param);
	iw_error_t (*deinit)(void *param);	
	iw_error_t (*is_ready)(void *param, dev_init_status_t *status); 	
	iw_error_t (*read)(void *param, sensor_type_t type, void *buff, unsigned int len);
	iw_error_t (*get_capa)(void *param, wake_src_type_t * wake_src);
	iw_error_t (*set_config)(void *param, wake_src_type_t wake_src, void * config);
	iw_error_t (*get_config)(void *param, wake_src_type_t *wake_src, void * config);
	iw_error_t (*set_power)(void *param, sensor_pwr_mode_t power_mode);
	iw_error_t (*get_power)(void *param, sensor_pwr_mode_t *power_mode);
}sensor_func_t;


typedef struct
{
	sensor_type_t 	type; 
	if_type_t 	iface;
	if_id_t 	iface_id;
	unsigned char   dev_id;
	char  	name[MAX_DRIVER_NAME_LEN];
	//sensor_pwr_mode_t pwr_mode;
	sensor_func_t	*func;
}sensor_dev_info_t;

typedef struct{
	unsigned char name[MAX_DRIVER_NAME_LEN];
	sensor_func_t *func;
}sensor_driver_t;

typedef struct
{
	unsigned char dev_num;
	sensor_dev_info_t dev[MAX_SENSOR_DEV];
	unsigned char driver_num;
	sensor_driver_t driver[MAX_SENSOR_DRIVER];
}sensor_info_t;



typedef struct
{
	unsigned char dev_id;
	sensor_type_t type;
	
	unsigned char name[MAX_DRIVER_NAME_LEN];
}registered_sensor_info_t;


void iw_register_sensor(sensor_type_t type, if_type_t iface, if_id_t iface_id, unsigned int info, char *name);
void iw_register_sensor_driver(sensor_driver_t *sensor_driver);
iw_error_t iw_register_sensor_(sensor_type_t dev_type, const char *name, if_type_t if_type, uint32_t if_num, void *if_arg, sensor_driver_t *dev_driver);
uint8_t iw_get_registered_sensor_num(void);

iw_error_t iw_get_sensor_devid(sensor_type_t type, char *name, unsigned char *devid);
iw_error_t iw_get_sensor_dev_id_by_type(sensor_type_t type, uint8_t *dev_id);
iw_error_t iw_get_sensor_dev_id_by_name(char *name, uint8_t *dev_id);

iw_error_t iw_get_all_sensors_info(registered_sensor_info_t *info);
iw_error_t iw_get_sensor_info(sensor_type_t type, char *name, registered_sensor_info_t *info);
sensor_dev_info_t *iw_get_sensor_dev_info(uint8_t dev_id);


iw_error_t iw_init_sensor(unsigned char dev_id);
iw_error_t iw_deinit_sensor(unsigned char dev_id);
iw_error_t iw_is_ready_sensor(unsigned char dev_id, dev_init_status_t *status);	
iw_error_t iw_read_sensor(unsigned char dev_id, sensor_type_t type, void *buff, unsigned int len);
iw_error_t iw_get_capa_sensor(unsigned char dev_id, wake_src_type_t *wake_src);
iw_error_t iw_set_config_sensor(unsigned char dev_id, wake_src_type_t wake_src, void *config);
iw_error_t iw_get_config_sensor(unsigned char dev_id, wake_src_type_t *wake_src, void *config);
iw_error_t iw_set_pwrmode_sensor(unsigned char dev_id, sensor_pwr_mode_t pwr_mode);
iw_error_t iw_get_pwrmode_sensor(unsigned char dev_id, sensor_pwr_mode_t *pwr_mode); 		


#endif /* IW_DEV_SENSOR_H */

