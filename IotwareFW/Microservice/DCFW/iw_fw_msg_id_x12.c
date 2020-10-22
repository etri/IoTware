/*
Copyright (C) <2020> <ETRI>

This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 3.0 of the License, or (at your option) any later version. This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details. You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/

#include "iw_fw_msg_id_x12.h"

#pragma pack(push, 1)

static struct CMTaskStopAllMsg cmTASK_STOP_ALL = {
	{
		.ack = CM_ACK_REQ_MSG,
		.message_id = CM_MSG_ID_TASK_STOP_ALL,
		.dup = 0,
		.qos = CM_QOS_ACK_NEED
	}
};

static struct CMTaskStopAllAckMsg cmTASK_STOP_ALL_ACK = {
	{
		.ack = CM_ACK_MSG_ID,
		.message_id = CM_MSG_ID_TASK_STOP_ALL,
		.dup = 0,
		.qos = CM_QOS_ACK_NONE,
		.reserved = 0,
		.return_code = 0
	}
};

#pragma pack(pop)

uint8_t *cm_msg_x12_serialize(const struct CMTaskStopAllMsg *msg, size_t *size)
{
	return cm_header_serialize(&msg->header, size);
}

struct CMTaskStopAllMsg cm_msg_x12_parse(const uint8_t *msg, const size_t size)
{
	struct CMTaskStopAllMsg buf;
	buf.header = cm_header_parse(msg, size);
	return buf;
}

struct CMTaskStopAllMsg cm_msg_x12()
{
	return cmTASK_STOP_ALL;
}

uint8_t *cm_msg_x12_ack_serialize(const struct CMTaskStopAllAckMsg *msg, size_t *size)
{
	return cm_default_ack_serialize(&msg->header, size);
}

struct CMTaskStopAllAckMsg cm_msg_x12_ack_parse(const uint8_t *msg, const size_t size)
{
	struct CMTaskStopAllAckMsg buf;
	buf.header = cm_default_ack_parse(msg, size);
	return buf;
}

struct CMTaskStopAllAckMsg cm_msg_x12_ack()
{
	return cmTASK_STOP_ALL_ACK;
}
