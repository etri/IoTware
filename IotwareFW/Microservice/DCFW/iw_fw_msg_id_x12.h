/*
Copyright (C) <2020> <ETRI>

This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 3.0 of the License, or (at your option) any later version. This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details. You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/

#pragma once

#include "iw_fw_collabomsg_internal.h"

#pragma pack(push, 1)

struct CMTaskStopAllMsg {
	struct CMFixedHeader header;
};

struct CMTaskStopAllAckMsg {
	struct CMDefaultAckMsg header;
};

#pragma pack(pop)

uint8_t *cm_msg_x12_serialize(const struct CMTaskStopAllMsg *msg, size_t *size);
struct CMTaskStopAllMsg cm_msg_x12_parse(const uint8_t *msg, const size_t size);
struct CMTaskStopAllMsg cm_msg_x12();
uint8_t *cm_msg_x12_ack_serialize(const struct CMTaskStopAllAckMsg *msg, size_t *size);
struct CMTaskStopAllAckMsg cm_msg_x12_ack_parse(const uint8_t *msg, const size_t size);
struct CMTaskStopAllAckMsg cm_msg_x12_ack();
