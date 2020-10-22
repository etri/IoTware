/*
Copyright (C) <2020> <ETRI>

This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 3.0 of the License, or (at your option) any later version. This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details. You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/

#include "iw_fw_msg_id_x15.h"

#pragma pack(push, 1)

static struct CMAttributeWriteMsg cmATTRIBUTE_WRITE = {
	{
		.ack = CM_ACK_REQ_MSG,
		.message_id = CM_MSG_ID_ATTRIBUTE_WRITE,
		.dup = 0,
		.qos = CM_QOS_ACK_NEED
	},
	.reserved = 0,
	.length = 0
};

static struct CMAttributeWriteAckMsg cmATTRIBUTE_WRITE_ACK = {
	{
		.ack = CM_ACK_MSG_ID,
		.message_id = CM_MSG_ID_ATTRIBUTE_WRITE,
		.dup = 0,
		.qos = CM_QOS_ACK_NONE,
		.reserved = 0,
		.return_code = 0
	}
};

#pragma pack(pop)

static size_t sizeof_attribute_write()
{
	return sizeof(struct CMAttributeWriteMsg) - sizeof(struct CMAttributeWritePayload *);
}

static size_t sizeof_payload()
{
	return sizeof(struct CMAttributeWritePayload) - sizeof(struct CMAttributeValuePayload *);
}

uint8_t *cm_msg_x15_serialize(const struct CMAttributeWriteMsg *msg, size_t *size)
{
	*size = sizeof_attribute_write() + sizeof_payload() + cm_attribute_values_serialize_size(msg->payload->attribute_values, msg->payload->attr_count + 1);
	uint8_t *buf = calloc(1, *size);
	size_t sz = sizeof_attribute_write();
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

static struct CMAttributeWritePayload *payload_parse(const uint8_t *msg, const size_t size)
{
	struct CMAttributeWritePayload *payload = NULL;
	size_t sz = sizeof_payload();
	if (size >= sz) {
		payload = calloc(1, sizeof(struct CMAttributeWritePayload));
		if(payload!=NULL)
		{
			const uint8_t *pt = msg;
			memcpy(payload, pt, sz);
			pt += sz;
			payload->attribute_values = cm_attribute_values_parse(pt, size - sz, payload->attr_count);
		}
		
		return payload;
	}
}

struct CMAttributeWriteMsg cm_msg_x15_parse(const uint8_t *msg, const size_t size)
{
	struct CMAttributeWriteMsg buf;
	size_t sz = sizeof_attribute_write();
	if (size >= sz) {
		const uint8_t *pt = msg;
		memcpy(&buf, pt, sz);
		pt += sz;
		buf.payload = payload_parse(pt, size - sz);
	}
	return buf;
}

struct CMAttributeWriteMsg cm_msg_x15()
{
	return cmATTRIBUTE_WRITE;
}

uint8_t *cm_msg_x15_ack_serialize(const struct CMAttributeWriteAckMsg *msg, size_t *size)
{
	return cm_default_ack_serialize(&msg->header, size);
}

struct CMAttributeWriteAckMsg cm_msg_x15_ack_parse(const uint8_t *msg, const size_t size)
{
	struct CMAttributeWriteAckMsg buf;
	buf.header = cm_default_ack_parse(msg, size);
	return buf;
}

struct CMAttributeWriteAckMsg cm_msg_x15_ack()
{
	return cmATTRIBUTE_WRITE_ACK;
}
