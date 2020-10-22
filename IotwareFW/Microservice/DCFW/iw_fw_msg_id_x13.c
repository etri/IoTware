/*
Copyright (C) <2020> <ETRI>

This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 3.0 of the License, or (at your option) any later version. This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details. You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/

#include "iw_fw_msg_id_x13.h"

#pragma pack(push, 1)

static struct CMTaskRunMsg cmTASK_RUN = {
	{
		.ack = CM_ACK_REQ_MSG,
		.message_id = CM_MSG_ID_TASK_RUN,
		.dup = 0,
		.qos = CM_QOS_ACK_NEED
	},
	.reserved = 0,
	.length = 0
};

static struct CMTaskRunAckMsg cmTASK_RUN_ACK = {
	{
		.ack = CM_ACK_MSG_ID,
		.message_id = CM_MSG_ID_TASK_RUN,
		.dup = 0,
		.qos = CM_QOS_ACK_NONE,
		.reserved = 0,
		.return_code = 0
	}
};

#pragma pack(pop)

uint8_t *cm_msg_x13_serialize(const struct CMTaskRunMsg *msg, size_t *size)
{
	*size = sizeof(struct CMTaskRunMsg);
	uint8_t *buf = calloc(1, *size);
	if(buf!=NULL)
		memcpy(buf, msg, *size);
	return buf;
}

struct CMTaskRunMsg cm_msg_x13_parse(const uint8_t *msg, const size_t size)
{
	struct CMTaskRunMsg buf;

	if (sizeof(buf) <= size) {
		memcpy(&buf, msg, sizeof(buf));
	}

	return buf;
}

struct CMTaskRunMsg cm_msg_x13()
{
	return cmTASK_RUN;
}

uint8_t *cm_msg_x13_ack_serialize(const struct CMTaskRunAckMsg *msg, size_t *size)
{
	return cm_default_ack_serialize(&msg->header, size);
}

struct CMTaskRunAckMsg cm_msg_x13_ack_parse(const uint8_t *msg, const size_t size)
{
	struct CMTaskRunAckMsg buf;
	buf.header = cm_default_ack_parse(msg, size);
	return buf;
}

struct CMTaskRunAckMsg cm_msg_x13_ack()
{
	return cmTASK_RUN_ACK;
}
