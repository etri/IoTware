/*
Copyright (C) <2020> <ETRI>

This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 3.0 of the License, or (at your option) any later version. This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details. You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/

#include "iw_fw_msg_id_x03.h"

#pragma pack(push, 1)

static struct CMMemberInfoMsg cmMEMBER_INFO = {
	{
		.ack = CM_ACK_REQ_MSG,
		.message_id = CM_MSG_ID_MEMBER_INFO,
		.dup = 0,
		.qos = CM_QOS_ACK_NEED
	},
	.reserved = 0,
	.length = 0
};

static struct CMMemberInfoAckMsg cmMEMBER_INFO_ACK = {
	{
		.ack = CM_ACK_MSG_ID,
		.message_id = CM_MSG_ID_MEMBER_INFO,
		.dup = 0,
		.qos = CM_QOS_ACK_NONE,
		.reserved = 0,
		.return_code = 0
	}
};

#pragma pack(pop)

static size_t sizeof_member_info()
{
	return sizeof(struct CMMemberInfoMsg) - sizeof(struct CMMemberInfoPayload *);
}

static size_t sizeof_payload()
{
	return sizeof(struct CMMemberInfoPayload) - sizeof(struct CMAttributePayload *);
}

uint8_t *cm_msg_x03_serialize(const struct CMMemberInfoMsg *msg, size_t *size)
{
	*size = sizeof_member_info() + sizeof_payload() + cm_attributes_serialize_size(msg->payload->attr_count + 1);
	uint8_t *buf = calloc(1, *size);
	size_t sz = sizeof_member_info();
	if(buf!=NULL){
		memcpy(buf, msg, sz);
		if (msg->payload) {
			memcpy(buf+sz, msg->payload, sizeof_payload());
			sz += sizeof_payload();
			cm_attributes_serialize(msg->payload->attributes, msg->payload->attr_count + 1, buf+sz);
		}
	}
	return buf;
}

static struct CMMemberInfoPayload *payload_parse(const uint8_t *msg, const size_t size)
{
	struct CMMemberInfoPayload *payload = NULL;
	size_t sz = sizeof_payload();
	if (size >= sz) {
		payload = calloc(1, sizeof(struct CMMemberInfoPayload));
		const uint8_t *pt = msg;
		if(payload!=NULL){
			memcpy(payload, pt, sz);
			pt += sz;
			payload->attributes = cm_attributes_parse(pt, size - sz, payload->attr_count);
		}
	}
	return payload;
}

struct CMMemberInfoMsg cm_msg_x03_parse(const uint8_t *msg, const size_t size)
{
	struct CMMemberInfoMsg buf;
	size_t sz = sizeof_member_info();
	if (size >= sz) {
		const uint8_t *pt = msg;
		memcpy(&buf, pt, sz);
		pt += sz;
		buf.payload = payload_parse(pt, size - sz);
	}
	return buf;
}

struct CMMemberInfoMsg cm_msg_x03()
{
	return cmMEMBER_INFO;
}

uint8_t *cm_msg_x03_ack_serialize(const struct CMMemberInfoAckMsg *msg, size_t *size)
{
	return cm_default_ack_serialize(&msg->header, size);
}

struct CMMemberInfoAckMsg cm_msg_x03_ack_parse(const uint8_t *msg, const size_t size)
{
	struct CMMemberInfoAckMsg buf;
	buf.header = cm_default_ack_parse(msg, size);
	return buf;
}

struct CMMemberInfoAckMsg cm_msg_x03_ack()
{
	return cmMEMBER_INFO_ACK;
}
