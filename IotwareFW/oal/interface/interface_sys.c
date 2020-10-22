/*
Copyright (C) <2020> <ETRI>

This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 3.0 of the License, or (at your option) any later version. This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details. You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
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

