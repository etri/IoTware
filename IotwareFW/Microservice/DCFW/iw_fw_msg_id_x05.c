/*
Copyright (C) <2020> <ETRI>

This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 3.0 of the License, or (at your option) any later version. This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details. You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/

#include "iw_fw_msg_id_x05.h"

#pragma pack(push, 1)

static struct CMCheckStatusMsg cmCHECK_STATUS = {
	{
		.ack = CM_ACK_REQ_MSG,
		.message_id = CM_MSG_ID_CHECK_STATUS,
		.dup = 0,
		.qos = CM_QOS_ACK_NEED
	}
};

static struct CMCheckStatusAckMsg cmCHECK_STATUS_ACK = {
	{
		.ack = CM_ACK_MSG_ID,
		.message_id = CM_MSG_ID_CHECK_STATUS_ACK,
		.dup = 0,
		.qos = CM_QOS_ACK_NONE,
		.reserved = 0,
		.return_code = 0
	},
	.reserved = 0,
	.length = 1,
	.status = 0,
	.wake_up_interval = 0
};

#pragma pack(pop)

uint8_t *cm_msg_x05_serialize(const struct CMCheckStatusMsg *msg, size_t *size)
{
	return cm_header_serialize(&msg->header, size);
}

struct CMCheckStatusMsg cm_msg_x05_parse(const uint8_t *msg, const size_t size)
{
	struct CMCheckStatusMsg buf;
	buf.header = cm_header_parse(msg, size);
	return buf;
}

struct CMCheckStatusMsg cm_msg_x05()
{
	return cmCHECK_STATUS;
}

uint8_t *cm_msg_x05_ack_serialize(const struct CMCheckStatusAckMsg *msg, size_t *size)
{
	*size = sizeof(struct CMCheckStatusAckMsg);
	uint8_t *buf = calloc(1, *size);
	if(buf!=NULL)
		memcpy(buf, msg, *size);
	return buf;
}

struct CMCheckStatusAckMsg cm_msg_x05_ack_parse(const uint8_t *msg, const size_t size)
{
	struct CMCheckStatusAckMsg buf;

	if (sizeof(buf) <= size) {
		memcpy(&buf, msg, sizeof(buf));
	}

	return buf;
}

struct CMCheckStatusAckMsg cm_msg_x05_ack()
{
	return cmCHECK_STATUS_ACK;
}
