/*
Copyright (C) <2020> <ETRI>

This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 3.0 of the License, or (at your option) any later version. This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details. You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/

#include "iw_fw_msg_id_x04.h"

#pragma pack(push, 1)

static struct CMSetDeviceModeMsg cmSET_DEVICE_MODE = {
	{
		.ack = CM_ACK_REQ_MSG,
		.message_id = CM_MSG_ID_SET_DEVICE_MODE,
		.dup = 0,
		.qos = CM_QOS_ACK_NEED
	},
	.reserved = 0,
	.length = 1
};

static struct CMSetDeviceModeAckMsg cmSET_DEVICE_MODE_ACK = {
	{
		.ack = CM_ACK_MSG_ID,
		.message_id = CM_MSG_ID_SET_DEVICE_MODE,
		.dup = 0,
		.qos = CM_QOS_ACK_NONE,
		.reserved = 0,
		.return_code = 0
	}
};

#pragma pack(pop)

uint8_t *cm_msg_x04_serialize(const struct CMSetDeviceModeMsg *msg, size_t *size)
{
	*size = sizeof(struct CMSetDeviceModeMsg);
	uint8_t *buf = calloc(1, *size);
	if(buf!=NULL)
		memcpy(buf, msg, *size);
	return buf;
}

struct CMSetDeviceModeMsg cm_msg_x04_parse(const uint8_t *msg, const size_t size)
{
	struct CMSetDeviceModeMsg buf;

	if (sizeof(buf) <= size) {
		memcpy(&buf, msg, sizeof(buf));
	}

	return buf;
}

struct CMSetDeviceModeMsg cm_msg_x04()
{
	return cmSET_DEVICE_MODE;
}

uint8_t *cm_msg_x04_ack_serialize(const struct CMSetDeviceModeAckMsg *msg, size_t *size)
{
	return cm_default_ack_serialize(&msg->header, size);
}

struct CMSetDeviceModeAckMsg cm_msg_x04_ack_parse(const uint8_t *msg, const size_t size)
{
	struct CMSetDeviceModeAckMsg buf;
	buf.header = cm_default_ack_parse(msg, size);
	return buf;
}

struct CMSetDeviceModeAckMsg cm_msg_x04_ack()
{
	return cmSET_DEVICE_MODE_ACK;
}
