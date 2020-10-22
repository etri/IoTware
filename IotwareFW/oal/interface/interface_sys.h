/*
Copyright (C) <2020> <ETRI>

This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 3.0 of the License, or (at your option) any later version. This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details. You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/
#ifndef INTERFACE_SYS_H
#define INTERFACE_SYS_H

#include "iw_common.h"
#include "iw_oal.h"
#include "iw_sys.h"
#include "iw_dev.h"
#include "iw_dev_batt.h"

iw_error_t get_mcu_info(mcu_type_t *id);

iw_error_t set_autocontrol(auto_control_t enable);
iw_error_t get_autocontrol(auto_control_t *enable);
iw_error_t set_power_mode(power_mode_t pwr_mode);
iw_error_t get_power_mode(power_mode_t *pwr_mode);
// iw_error_t set_pwrmode_sys(system_power_t pwr_mode);
// iw_error_t get_pwrmode_sys(system_power_t *pwr_mode);

iw_error_t get_battery_voltage(uint16_t *voltage);
iw_error_t get_battery_status(int *percent, int *time);

iw_error_t get_memory_status(uint32_t *global_data, uint32_t *total_heap, uint32_t *free_heap);

iw_error_t get_network_status(uint8_t dev_id, int *connected, int *strength, wake_src_type_t *wake_src, net_pwr_mode_t *mode);

iw_error_t get_sensor_status(uint8_t dev_id, wake_src_type_t *wake_src, sensor_pwr_mode_t *mode);

#endif /* INTERFACE_SYS_H */

