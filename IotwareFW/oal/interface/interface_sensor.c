/*
Copyright (C) <2020> <ETRI>

This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 3.0 of the License, or (at your option) any later version. This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details. You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/

#include "interface_sensor.h"
#include "service.h"

uint8_t get_registered_sensor_num(void)
{
	return iw_get_registered_sensor_num();
}

iw_error_t get_all_sensors_info(registered_sensor_info_t *info)
{
	return iw_get_all_sensors_info(info);
}

iw_error_t get_sensor_info(sensor_type_t type, char *name, registered_sensor_info_t *info)
{
	return iw_get_sensor_info(type, name, info);
}

iw_error_t get_sensor_devid(sensor_type_t type, char *name, unsigned char *devid)
{
	return iw_get_sensor_devid(type, name, devid);
}

iw_error_t init_sensor(unsigned char dev_id)
{
	return iw_init_sensor(dev_id);
}

iw_error_t deinit_sensor(unsigned char dev_id)		
{
	return iw_deinit_sensor(dev_id);
}

iw_error_t is_ready_sensor(unsigned char dev_id, dev_init_status_t *status) 	
{
	return iw_is_ready_sensor(dev_id, status);
}

iw_error_t read_sensor(unsigned char dev_id, sensor_type_t type, void *buff, unsigned int len)
{
    if(get_lowbattery_control()  == 1)
    {
        iw_printf("autocontrol_lowbattery sensor blocking\n");
		return IW_SUCCESS;
    }
	else
	{
	    return iw_read_sensor( dev_id,  type, buff,  len);
	}
}

iw_error_t get_capa_sensor(unsigned char dev_id, wake_src_type_t *wake_src)
{
	return iw_get_capa_sensor( dev_id, wake_src);
}

iw_error_t set_config_sensor(unsigned char dev_id, wake_src_type_t wake_src, void *config)
{
	return iw_set_config_sensor(  dev_id,  wake_src, config);
}

iw_error_t get_config_sensor(unsigned char dev_id, wake_src_type_t *wake_src, void *config)
{
	return iw_get_config_sensor(  dev_id,  wake_src, config);
}

iw_error_t set_pwrmode_sensor(unsigned char dev_id, sensor_pwr_mode_t pwr_mode)
{
	return iw_set_pwrmode_sensor( dev_id,  pwr_mode);
}

iw_error_t get_pwrmode_sensor(unsigned char dev_id, sensor_pwr_mode_t *pwr_mode) 
{
	return iw_get_pwrmode_sensor(  dev_id, pwr_mode); 
}

