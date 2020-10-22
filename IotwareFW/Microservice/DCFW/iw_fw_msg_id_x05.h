/*
Copyright (C) <2020> <ETRI>

This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 3.0 of the License, or (at your option) any later version. This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details. You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/

#pragma once

#include "iw_fw_collabomsg_internal.h"

#pragma pack(push, 1)

enum CM_CHECK_STATUS {
	CM_CHECK_STATUS_SLEEP = 0 << 0,
	CM_CHECK_STATUS_WAKE_UP = 0 << 1,
	CM_CHECK_STATUS_UNDEFINED1 = 0 << 2,
	CM_CHECK_STATUS_UNDEFINED2 = 0 << 3,
};

struct CMCheckStatusMsg {
	struct CMFixedHeader header;
};

struct CMCheckStatusAckMsg {
	struct CMDefaultAckMsg header;
	uint8_t reserved : 2;
	uint8_t length : 6;
	uint8_t status : 2;
	uint8_t wake_up_interval : 6;
};

#pragma pack(pop)

uint8_t *cm_msg_x05_serialize(const struct CMCheckStatusMsg *msg, size_t *size);
struct CMCheckStatusMsg cm_msg_x05_parse(const uint8_t *msg, const size_t size);
struct CMCheckStatusMsg cm_msg_x05();
uint8_t *cm_msg_x05_ack_serialize(const struct CMCheckStatusAckMsg *msg, size_t *size);
struct CMCheckStatusAckMsg cm_msg_x05_ack_parse(const uint8_t *msg, const size_t size);
struct CMCheckStatusAckMsg cm_msg_x05_ack();
