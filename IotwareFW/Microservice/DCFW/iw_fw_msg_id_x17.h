/*
Copyright (C) <2020> <ETRI>

This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 3.0 of the License, or (at your option) any later version. This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details. You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/

#pragma once

#include "iw_fw_collabomsg_internal.h"

#pragma pack(push, 1)

struct CMTaskInputDataPayload {
	uint8_t task_id;
	uint8_t object_id : 6;
	uint8_t attr_count : 2;
	struct CMAttributeValuePayload *attribute_values;
};

struct CMTaskInputDataMsg {
	struct CMFixedHeader header;
	uint8_t reserved : 2;
	uint8_t length : 6;
	struct CMTaskInputDataPayload *payload;
};

struct CMTaskInputDataAckMsg {
	struct CMDefaultAckMsg header;
};

#pragma pack(pop)

uint8_t *cm_msg_x17_serialize(const struct CMTaskInputDataMsg *msg, size_t *size);
struct CMTaskInputDataMsg cm_msg_x17_parse(const uint8_t *msg, const size_t size);
struct CMTaskInputDataMsg cm_msg_x17();
uint8_t *cm_msg_x17_ack_serialize(const struct CMTaskInputDataAckMsg *msg, size_t *size);
struct CMTaskInputDataAckMsg cm_msg_x17_ack_parse(const uint8_t *msg, const size_t size);
struct CMTaskInputDataAckMsg cm_msg_x17_ack();
