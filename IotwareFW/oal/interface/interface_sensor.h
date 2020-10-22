/*
Copyright (C) <2020> <ETRI>

This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 3.0 of the License, or (at your option) any later version. This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details. You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/
#ifndef INTERFACE_SENSOR_H
#define INTERFACE_SENSOR_H

#include "iw_common.h"
#include "iw_dev_sensor.h"


uint8_t get_registered_sensor_num(void);
iw_error_t get_all_sensors_info(registered_sensor_info_t *info);
iw_error_t get_sensor_info(sensor_type_t type, char *name, registered_sensor_info_t *info);
iw_error_t get_sensor_devid(sensor_type_t type, char *name, unsigned char *devid);
iw_error_t init_sensor(unsigned char dev_id);
iw_error_t deinit_sensor(unsigned char dev_id);	
iw_error_t is_ready_sensor(unsigned char dev_id, dev_init_status_t *status); 	
iw_error_t read_sensor(unsigned char dev_id, sensor_type_t type, void *buff, unsigned int len);
iw_error_t get_capa_sensor(unsigned char dev_id, wake_src_type_t *wake_src);
iw_error_t set_config_sensor(unsigned char dev_id, wake_src_type_t wake_src, void *config);
iw_error_t get_config_sensor(unsigned char dev_id, wake_src_type_t *wake_src, void *config);
iw_error_t set_pwrmode_sensor(unsigned char dev_id, sensor_pwr_mode_t pwr_mode);
iw_error_t get_pwrmode_sensor(unsigned char dev_id, sensor_pwr_mode_t *pwr_mode); 

#endif /* INTERFACE_SENSOR_H */

