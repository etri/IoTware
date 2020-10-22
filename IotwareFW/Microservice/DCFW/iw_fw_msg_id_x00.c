/*
Copyright (C) <2020> <ETRI>

This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 3.0 of the License, or (at your option) any later version. This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details. You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/

#include "iw_fw_msg_id_x00.h"

#pragma pack(push, 1)

static struct CMServerAdvertiseMsg cmSERVER_ADVERTIZE = {
	{
		.ack = CM_ACK_REQ_MSG,
		.message_id = CM_MSG_ID_SERVER_ADVERTISE,
		.dup = 0,
		.qos = CM_QOS_ACK_NONE
	}
};

#pragma pack(pop)

uint8_t *cm_msg_x00_serialize(const struct CMServerAdvertiseMsg *msg, size_t *size)
{
	return cm_header_serialize(&msg->header, size);
}

struct CMServerAdvertiseMsg cm_msg_x00_parse(const uint8_t *msg, const size_t size)
{
	struct CMServerAdvertiseMsg buf;
	buf.header = cm_header_parse(msg, size);
	return buf;
}

struct CMServerAdvertiseMsg cm_msg_x00()
{
	return cmSERVER_ADVERTIZE;
}
