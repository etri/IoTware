/*
Copyright (C) <2020> <ETRI>

This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 3.0 of the License, or (at your option) any later version. This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details. You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/

#pragma once

#include "iw_fw_collabomsg_internal.h"

#pragma pack(push, 1)

enum CM_SET_DEVICE_MODE {
	CM_SET_DEVICE_MODE_SLEEP = 0 << 0,
	CM_SET_DEVICE_MODE_SLEEP_TASK = 0 << 1,
	CM_SET_DEVICE_MODE_CLUSTER_HEADER = 0 << 2,
	CM_SET_DEVICE_MODE_UNDEFINED = 0 << 3,
};

struct CMSetDeviceModeMsg {
	struct CMFixedHeader header;
	uint8_t reserved : 2;
	uint8_t length : 6;
	uint8_t mode : 2;
	uint8_t wake_up_interval : 6;
};

struct CMSetDeviceModeAckMsg {
	struct CMDefaultAckMsg header;
};

#pragma pack(pop)

uint8_t *cm_msg_x04_serialize(const struct CMSetDeviceModeMsg *msg, size_t *size);
struct CMSetDeviceModeMsg cm_msg_x04_parse(const uint8_t *msg, const size_t size);
struct CMSetDeviceModeMsg cm_msg_x04();
uint8_t *cm_msg_x04_ack_serialize(const struct CMSetDeviceModeAckMsg *msg, size_t *size);
struct CMSetDeviceModeAckMsg cm_msg_x04_ack_parse(const uint8_t *msg, const size_t size);
struct CMSetDeviceModeAckMsg cm_msg_x04_ack();
