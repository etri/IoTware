/*
Copyright (C) <2020> <ETRI>

This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 3.0 of the License, or (at your option) any later version. This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details. You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/

#pragma once

#include "iw_fw_collabomsg_internal.h"

#pragma pack(push, 1)

struct CMConnectMsg {
	struct CMFixedHeader header;
	uint8_t reserved : 2;
	uint8_t length : 6;
	uint8_t protocol_version : 4;
	uint8_t id_flag : 1;
	uint8_t pwd_flag : 1;
	uint8_t undefined : 2;
	uint8_t keep_alive;
	// uint8_t *optional_payload;
};

struct CMConnectAckMsg {
	struct CMDefaultAckMsg header;
};

#pragma pack(pop)

uint8_t *cm_msg_x02_serialize(const struct CMConnectMsg *msg, size_t *size);
struct CMConnectMsg cm_msg_x02_parse(const uint8_t *msg, const size_t size);
struct CMConnectMsg cm_msg_x02();
uint8_t *cm_msg_x02_ack_serialize(const struct CMConnectAckMsg *msg, size_t *size);
struct CMConnectAckMsg cm_msg_x02_ack_parse(const uint8_t *msg, const size_t size);
struct CMConnectAckMsg cm_msg_x02_ack();
