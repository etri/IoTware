/*
Copyright (C) <2020> <ETRI>

This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 3.0 of the License, or (at your option) any later version. This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details. You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/
#ifndef IW_DEV_BATT_H
#define IW_DEV_BATT_H

#include "iw_common.h"
#include "iw_if.h"


#define INVALID_BATT_PORT (0xff)

void iw_register_battery(if_type_t type, if_id_t port, uint8_t defined_pin);
void iw_init_battery(void);
void iw_deinit_battery(void);
iw_error_t iw_get_battery_voltage(uint16_t *val);
iw_error_t iw_get_battery_adc(uint16_t *val);
#endif /* IW_DEV_BATT_H */

