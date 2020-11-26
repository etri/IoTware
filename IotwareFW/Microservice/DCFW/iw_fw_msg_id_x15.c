/*
Copyright (C) <2020> <ETRI>

This License and Service Agreement (LSA) applies to all works and their derivative works based on source form version and object form version of IoTware Project. Currently, the LSA for IoTware Project has two policies, 'Open Source License' and 'Commercial License'. Therefore, all works including the source code and executable code of IoTware Project and derivative works based thereon are subject to either 'Open Source License' or 'Commercial License' depending on the user's needs and purpose. Details related to the selection of the applicable license are specified in this LSA. If you use any or all of IoTware Project in any form, you are deemed to have consented to this LSA. If you breach any of the terms and conditions set forth in this LSA, you are solely responsible for any losses or damages incurred by Electronics and Communications Research Institute (ETRI), and ETRI assume no responsibility for you or any third party.
If you use the source form version or object form version of IoTware Project in whole or in part to develop a code or a derivative work, and you want to commercialize the result in some form, you will be covered under a commercial license. And if you are subject to a commercial license, the contract for the use of IoTware Project is subject to TECHNOLOGY LICENSE AGREEMENT of ETRI. You acknowledge that ETRI has all legal rights, title and interest, including intellectual property rights in the IoTware Project (regardless of whether such intellectual property rights are registered or where such rights exist) and agree with no objection thereto. Except as provided in a subsidiary agreement, nothing in this LSA grants you the right to use IoTware Project or the name, service mark, logo, domain name and other unique identification marks of ETRI.
If you use the source form version or object form version of IoTware Project in whole or in part to develop a code or a derivative work, and you do not commercialize the result in any form, you will be covered under an open source license. IoTware Project is in accordance with Free Software Foundation (FSF)'s open source policy, and is allowed to use it in the appropriate scope and manner, and you must comply with the applicable open source license policy applied to IoTware Project. IoTware Project is, in principle, subject to GNU Lesser General Public License version 2.1 (LGPLv2.1). If you have acquired all or a part of the IoTware Project in any way and it is subject to a license other than the open source license described above, please contact the following address for the technical support and other inquiries before use, and check the usage information.
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
