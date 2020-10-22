/*
Copyright (C) <2020> <ETRI>

This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 3.0 of the License, or (at your option) any later version. This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details. You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/

#include "iw_fw_msg_id_x11.h"

#pragma pack(push, 1)

static struct CMTaskRunAllMsg cmTASK_RUN_ALL = {
	{
		.ack = CM_ACK_REQ_MSG,
		.message_id = CM_MSG_ID_TASK_RUN_ALL,
		.dup = 0,
		.qos = CM_QOS_ACK_NEED
	}
};

static struct CMTaskRunAllAckMsg cmTASK_RUN_ALL_ACK = {
	{
		.ack = CM_ACK_MSG_ID,
		.message_id = CM_MSG_ID_TASK_RUN_ALL,
		.dup = 0,
		.qos = CM_QOS_ACK_NONE,
		.reserved = 0,
		.return_code = 0
	}
};

#pragma pack(pop)

uint8_t *cm_msg_x11_serialize(const struct CMTaskRunAllMsg *msg, size_t *size)
{
	return cm_header_serialize(&msg->header, size);
}

struct CMTaskRunAllMsg cm_msg_x11_parse(const uint8_t *msg, const size_t size)
{
	struct CMTaskRunAllMsg buf;
	buf.header = cm_header_parse(msg, size);
	return buf;
}

struct CMTaskRunAllMsg cm_msg_x11()
{
	return cmTASK_RUN_ALL;
}

uint8_t *cm_msg_x11_ack_serialize(const struct CMTaskRunAllAckMsg *msg, size_t *size)
{
	return cm_default_ack_serialize(&msg->header, size);
}

struct CMTaskRunAllAckMsg cm_msg_x11_ack_parse(const uint8_t *msg, const size_t size)
{
	struct CMTaskRunAllAckMsg buf;
	buf.header = cm_default_ack_parse(msg, size);
	return buf;
}

struct CMTaskRunAllAckMsg cm_msg_x11_ack()
{
	return cmTASK_RUN_ALL_ACK;
}
