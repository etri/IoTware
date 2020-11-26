/*
Copyright (C) <2020> <ETRI>

This License and Service Agreement (LSA) applies to all works and their derivative works based on source form version and object form version of IoTware Project. Currently, the LSA for IoTware Project has two policies, 'Open Source License' and 'Commercial License'. Therefore, all works including the source code and executable code of IoTware Project and derivative works based thereon are subject to either 'Open Source License' or 'Commercial License' depending on the user's needs and purpose. Details related to the selection of the applicable license are specified in this LSA. If you use any or all of IoTware Project in any form, you are deemed to have consented to this LSA. If you breach any of the terms and conditions set forth in this LSA, you are solely responsible for any losses or damages incurred by Electronics and Communications Research Institute (ETRI), and ETRI assume no responsibility for you or any third party.
If you use the source form version or object form version of IoTware Project in whole or in part to develop a code or a derivative work, and you want to commercialize the result in some form, you will be covered under a commercial license. And if you are subject to a commercial license, the contract for the use of IoTware Project is subject to TECHNOLOGY LICENSE AGREEMENT of ETRI. You acknowledge that ETRI has all legal rights, title and interest, including intellectual property rights in the IoTware Project (regardless of whether such intellectual property rights are registered or where such rights exist) and agree with no objection thereto. Except as provided in a subsidiary agreement, nothing in this LSA grants you the right to use IoTware Project or the name, service mark, logo, domain name and other unique identification marks of ETRI.
If you use the source form version or object form version of IoTware Project in whole or in part to develop a code or a derivative work, and you do not commercialize the result in any form, you will be covered under an open source license. IoTware Project is in accordance with Free Software Foundation (FSF)'s open source policy, and is allowed to use it in the appropriate scope and manner, and you must comply with the applicable open source license policy applied to IoTware Project. IoTware Project is, in principle, subject to GNU Lesser General Public License version 2.1 (LGPLv2.1). If you have acquired all or a part of the IoTware Project in any way and it is subject to a license other than the open source license described above, please contact the following address for the technical support and other inquiries before use, and check the usage information.
*/

#include "interface_sys.h"


iw_error_t get_mcu_info(mcu_type_t *id)
{
	return iw_get_mcu_info(id);
}

iw_error_t get_autocontrol(auto_control_t *enable)
{
	return iw_get_autocontrol(enable);
}

iw_error_t set_autocontrol(auto_control_t enable)
{
	return iw_set_autocontrol(enable);
}

iw_error_t get_power_mode(power_mode_t *pwr_mode)
{
	return iw_get_power_mode(pwr_mode);
}

iw_error_t set_power_mode(power_mode_t pwr_mode)
{
	return iw_set_power_mode(pwr_mode);
}

// iw_error_t get_lowpower_status(lowpower_mode_t *enable)
// {
// 	return iw_get_lowpower_status(enable);
// }

// iw_error_t set_lowpower_status(lowpower_mode_t enable)
// {
// 	return iw_set_lowpower_status(enable);
// }

iw_error_t get_battery_voltage(uint16_t *voltage)
{
	return iw_get_battery_voltage(voltage);
}

iw_error_t get_battery_status(int *percent, int *time)
{
	return iw_get_battery_status(percent, time);
}

iw_error_t get_memory_status(uint32_t *global_data, uint32_t *total_heap, uint32_t *free_heap)
{
	return iw_get_memory_status(global_data, total_heap, free_heap);
}

iw_error_t get_network_status(uint8_t dev_id, int *connected, int *strength, wake_src_type_t *wake_src, net_pwr_mode_t *mode)
{
	return iw_get_network_status(dev_id, connected, strength, wake_src, mode);
}

iw_error_t get_sensor_status(uint8_t dev_id, wake_src_type_t *wake_src, sensor_pwr_mode_t *mode)
{
	return iw_get_sensor_status(dev_id, wake_src, mode);
}

