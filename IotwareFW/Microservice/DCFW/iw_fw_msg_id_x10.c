/*
Copyright (C) <2020> <ETRI>

This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 3.0 of the License, or (at your option) any later version. This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details. You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/

#include "iw_fw_msg_id_x10.h"

#pragma pack(push, 1)

static struct CMTaskCreationMsg cmTASK_CREATION = {
	{
		.ack = CM_ACK_REQ_MSG,
		.message_id = CM_MSG_ID_TASK_CREATION,
		.dup = 0,
		.qos = CM_QOS_ACK_NEED
	},
	.reserved = 0,
	.length = 0
};

static struct CMTaskCreationAckMsg cmTASK_CREATION_ACK = {
	{
		.ack = CM_ACK_MSG_ID,
		.message_id = CM_MSG_ID_TASK_CREATION,
		.dup = 0,
		.qos = CM_QOS_ACK_NONE,
		.reserved = 0,
		.return_code = 0
	}
};

#pragma pack(pop)

uint8_t *cm_msg_x10_serialize(const struct CMTaskCreationMsg *msg, size_t *size)
{
	*size = sizeof(struct CMTaskCreationMsg);
	uint8_t *buf = calloc(1, *size);
	if(buf!=NULL)
		memcpy(buf, msg, *size);
	return buf;
}

struct CMTaskCreationMsg cm_msg_x10_parse(const uint8_t *msg, const size_t size)
{
	struct CMTaskCreationMsg buf;

	if (sizeof(buf) <= size) {
		memcpy(&buf, msg, sizeof(buf));
	}

	return buf;
}

struct CMTaskCreationMsg cm_msg_x10()
{
	return cmTASK_CREATION;
}

uint8_t *cm_msg_x10_ack_serialize(const struct CMTaskCreationAckMsg *msg, size_t *size)
{
	return cm_default_ack_serialize(&msg->header, size);
}

struct CMTaskCreationAckMsg cm_msg_x10_ack_parse(const uint8_t *msg, const size_t size)
{
	struct CMTaskCreationAckMsg buf;
	buf.header = cm_default_ack_parse(msg, size);
	return buf;
}

struct CMTaskCreationAckMsg cm_msg_x10_ack()
{
	return cmTASK_CREATION_ACK;
}
