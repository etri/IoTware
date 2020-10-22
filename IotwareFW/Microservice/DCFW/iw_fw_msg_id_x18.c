/*
Copyright (C) <2020> <ETRI>

This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 3.0 of the License, or (at your option) any later version. This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details. You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/

#include "iw_fw_msg_id_x18.h"

#pragma pack(push, 1)

static struct CMTaskOutputDataMsg cmTASK_OUTPUT_DATA = {
	{
		.ack = CM_ACK_REQ_MSG,
		.message_id = CM_MSG_ID_TASK_OUTPUT_DATA,
		.dup = 0,
		.qos = CM_QOS_ACK_NEED
	},
	.reserved = 0,
	.length = 0
};

static struct CMTaskOutputDataAckMsg cmTASK_OUTPUT_DATA_ACK = {
	{
		.ack = CM_ACK_MSG_ID,
		.message_id = CM_MSG_ID_TASK_OUTPUT_DATA,
		.dup = 0,
		.qos = CM_QOS_ACK_NONE,
		.reserved = 0,
		.return_code = 0
	}
};

#pragma pack(pop)

static size_t sizeof_task_output_data()
{
	return sizeof(struct CMTaskOutputDataMsg) - sizeof(struct CMTaskOutputDataPayload *);
}

static size_t sizeof_payload()
{
	return sizeof(struct CMTaskOutputDataPayload) - sizeof(struct CMAttributeValuePayload *);
}

uint8_t *cm_msg_x18_serialize(const struct CMTaskOutputDataMsg *msg, size_t *size)
{
	*size = sizeof_task_output_data() + sizeof_payload() + cm_attribute_values_serialize_size(msg->payload->attribute_values, msg->payload->attr_count + 1);
	uint8_t *buf = calloc(1, *size);
	size_t sz = sizeof_task_output_data();
	if(buf!=NULL){
		memcpy(buf, msg, sz);
		if (msg->payload) {
			memcpy(buf+sz, msg->payload, sizeof_payload());
			sz += sizeof_payload();
			cm_attribute_values_serialize(msg->payload->attribute_values, msg->payload->attr_count + 1, buf+sz);
		}
	}
	return buf;
}

static struct CMTaskOutputDataPayload *payload_parse(const uint8_t *msg, const size_t size)
{
	struct CMTaskOutputDataPayload *payload = NULL;
	size_t sz = sizeof_payload();
	if (size >= sz) {
		payload = calloc(1, sizeof(struct CMTaskOutputDataPayload));
		const uint8_t *pt = msg;
		if(payload!=NULL){
			memcpy(payload, pt, sz);
			pt += sz;
			payload->attribute_values = cm_attribute_values_parse(pt, size - sz, payload->attr_count);
		}
	}
	return payload;
}

struct CMTaskOutputDataMsg cm_msg_x18_parse(const uint8_t *msg, const size_t size)
{
	struct CMTaskOutputDataMsg buf;
	size_t sz = sizeof_task_output_data();
	if (size >= sz) {
		const uint8_t *pt = msg;
		memcpy(&buf, pt, sz);
		pt += sz;
		buf.payload = payload_parse(pt, size - sz);
	}
	return buf;
}

struct CMTaskOutputDataMsg cm_msg_x18()
{
	return cmTASK_OUTPUT_DATA;
}

uint8_t *cm_msg_x18_ack_serialize(const struct CMTaskOutputDataAckMsg *msg, size_t *size)
{
	return cm_default_ack_serialize(&msg->header, size);
}

struct CMTaskOutputDataAckMsg cm_msg_x18_ack_parse(const uint8_t *msg, const size_t size)
{
	struct CMTaskOutputDataAckMsg buf;
	buf.header = cm_default_ack_parse(msg, size);
	return buf;
}

struct CMTaskOutputDataAckMsg cm_msg_x18_ack()
{
	return cmTASK_OUTPUT_DATA_ACK;
}