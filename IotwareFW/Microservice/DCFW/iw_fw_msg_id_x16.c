/*
Copyright (C) <2020> <ETRI>

This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 3.0 of the License, or (at your option) any later version. This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details. You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/

#include "iw_fw_msg_id_x16.h"

#pragma pack(push, 1)

static struct CMAttributeReadMsg cmATTRIBUTE_READ = {
	{
		.ack = CM_ACK_REQ_MSG,
		.message_id = CM_MSG_ID_ATTRIBUTE_READ,
		.dup = 0,
		.qos = CM_QOS_ACK_NEED
	},
	.reserved = 0,
	.length = 0
};

static struct CMAttributeReadAckMsg cmATTRIBUTE_READ_ACK = {
	{
		.ack = CM_ACK_MSG_ID,
		.message_id = CM_MSG_ID_ATTRIBUTE_READ_ACK,
		.dup = 0,
		.qos = CM_QOS_ACK_NONE,
		.reserved = 0,
		.return_code = 0
	},
	.reserved = 0,
	.length = 0
};

#pragma pack(pop)

static size_t sizeof_attribute_read()
{
	return sizeof(struct CMAttributeReadMsg) - sizeof(struct CMAttributeReadPayload *);
}

static size_t sizeof_payload()
{
	return sizeof(struct CMAttributeReadPayload) - sizeof(struct CMAttributePayload *);
}

uint8_t *cm_msg_x16_serialize(const struct CMAttributeReadMsg *msg, size_t *size)
{
	*size = sizeof_attribute_read() + sizeof_payload() + cm_attributes_serialize_size(msg->payload->attr_count + 1);
	uint8_t *buf = calloc(1, *size);
	size_t sz = sizeof_attribute_read();
	if(buf!=NULL)
		memcpy(buf, msg, sz);
	if (msg->payload) {
		memcpy(buf+sz, msg->payload, sizeof_payload());
		sz += sizeof_payload();
		cm_attributes_serialize(msg->payload->attributes, msg->payload->attr_count + 1, buf+sz);
	}
	return buf;
}

static struct CMAttributeReadPayload *payload_parse(const uint8_t *msg, const size_t size)
{
	struct CMAttributeReadPayload *payload = NULL;
	size_t sz = sizeof_payload();
	if (size >= sz) {
		payload = calloc(1, sizeof(struct CMAttributeReadPayload));
		const uint8_t *pt = msg;
		if(payload!=NULL){
			memcpy(payload, pt, sz);
			pt += sz;
			payload->attributes = cm_attributes_parse(pt, size - sz, payload->attr_count);
		}
	}
	return payload;
}

struct CMAttributeReadMsg cm_msg_x16_parse(const uint8_t *msg, const size_t size)
{
	struct CMAttributeReadMsg buf;
	size_t sz = sizeof_attribute_read();
	if (size >= sz) {
		const uint8_t *pt = msg;
		memcpy(&buf, pt, sz);
		pt += sz;
		buf.payload = payload_parse(pt, size - sz);
	}
	return buf;
}

struct CMAttributeReadMsg cm_msg_x16()
{
	return cmATTRIBUTE_READ;
}

static size_t sizeof_attribute_read_ack()
{
	return sizeof(struct CMAttributeReadAckMsg) - sizeof(struct CMAttributeReadAckPayload *);
}

static size_t sizeof_ack_payload()
{
	return sizeof(struct CMAttributeReadAckPayload) - sizeof(struct CMAttributeValuePayload *);
}

uint8_t *cm_msg_x16_ack_serialize(const struct CMAttributeReadAckMsg *msg, size_t *size)
{
	*size = sizeof_attribute_read_ack() + sizeof_ack_payload() + cm_attribute_values_serialize_size(msg->payload->attribute_values, msg->payload->attr_count + 1);
	uint8_t *buf = calloc(1, *size);
	size_t sz = sizeof_attribute_read_ack();
	if(buf!=NULL){
		memcpy(buf, msg, sz);
		if (msg->payload) {
			memcpy(buf+sz, msg->payload, sizeof_ack_payload());
			sz += sizeof_ack_payload();
			cm_attribute_values_serialize(msg->payload->attribute_values, msg->payload->attr_count + 1, buf+sz);
		}
	}
	return buf;
}

static struct CMAttributeReadAckPayload *ack_payload_parse(const uint8_t *msg, const size_t size)
{
	struct CMAttributeReadAckPayload *payload = NULL;
	size_t sz = sizeof_ack_payload();
	if (size >= sz) {
		payload = calloc(1, sizeof(struct CMAttributeReadAckPayload));
		const uint8_t *pt = msg;
		if(payload!=NULL){
			memcpy(payload, pt, sz);
			pt += sz;
			payload->attribute_values = cm_attribute_values_parse(pt, size - sz, payload->attr_count);
		}
	}
	return payload;
}

struct CMAttributeReadAckMsg cm_msg_x16_ack_parse(const uint8_t *msg, const size_t size)
{
	struct CMAttributeReadAckMsg buf;
	size_t sz = sizeof_attribute_read_ack();
	if (size >= sz) {
		const uint8_t *pt = msg;
		memcpy(&buf, pt, sz);
		pt += sz;
		buf.payload = ack_payload_parse(pt, size - sz);
	}
	return buf;
}

struct CMAttributeReadAckMsg cm_msg_x16_ack()
{
	return cmATTRIBUTE_READ_ACK;
}
