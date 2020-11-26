/*
Copyright (C) <2020> <ETRI>

This License and Service Agreement (LSA) applies to all works and their derivative works based on source form version and object form version of IoTware Project. Currently, the LSA for IoTware Project has two policies, 'Open Source License' and 'Commercial License'. Therefore, all works including the source code and executable code of IoTware Project and derivative works based thereon are subject to either 'Open Source License' or 'Commercial License' depending on the user's needs and purpose. Details related to the selection of the applicable license are specified in this LSA. If you use any or all of IoTware Project in any form, you are deemed to have consented to this LSA. If you breach any of the terms and conditions set forth in this LSA, you are solely responsible for any losses or damages incurred by Electronics and Communications Research Institute (ETRI), and ETRI assume no responsibility for you or any third party.
If you use the source form version or object form version of IoTware Project in whole or in part to develop a code or a derivative work, and you want to commercialize the result in some form, you will be covered under a commercial license. And if you are subject to a commercial license, the contract for the use of IoTware Project is subject to TECHNOLOGY LICENSE AGREEMENT of ETRI. You acknowledge that ETRI has all legal rights, title and interest, including intellectual property rights in the IoTware Project (regardless of whether such intellectual property rights are registered or where such rights exist) and agree with no objection thereto. Except as provided in a subsidiary agreement, nothing in this LSA grants you the right to use IoTware Project or the name, service mark, logo, domain name and other unique identification marks of ETRI.
If you use the source form version or object form version of IoTware Project in whole or in part to develop a code or a derivative work, and you do not commercialize the result in any form, you will be covered under an open source license. IoTware Project is in accordance with Free Software Foundation (FSF)'s open source policy, and is allowed to use it in the appropriate scope and manner, and you must comply with the applicable open source license policy applied to IoTware Project. IoTware Project is, in principle, subject to GNU Lesser General Public License version 2.1 (LGPLv2.1). If you have acquired all or a part of the IoTware Project in any way and it is subject to a license other than the open source license described above, please contact the following address for the technical support and other inquiries before use, and check the usage information.
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

