/*
Copyright (C) <2020> <ETRI>

This License and Service Agreement (LSA) applies to all works and their derivative works based on source form version and object form version of IoTware Project. Currently, the LSA for IoTware Project has two policies, 'Open Source License' and 'Commercial License'. Therefore, all works including the source code and executable code of IoTware Project and derivative works based thereon are subject to either 'Open Source License' or 'Commercial License' depending on the user's needs and purpose. Details related to the selection of the applicable license are specified in this LSA. If you use any or all of IoTware Project in any form, you are deemed to have consented to this LSA. If you breach any of the terms and conditions set forth in this LSA, you are solely responsible for any losses or damages incurred by Electronics and Communications Research Institute (ETRI), and ETRI assume no responsibility for you or any third party.
If you use the source form version or object form version of IoTware Project in whole or in part to develop a code or a derivative work, and you want to commercialize the result in some form, you will be covered under a commercial license. And if you are subject to a commercial license, the contract for the use of IoTware Project is subject to TECHNOLOGY LICENSE AGREEMENT of ETRI. You acknowledge that ETRI has all legal rights, title and interest, including intellectual property rights in the IoTware Project (regardless of whether such intellectual property rights are registered or where such rights exist) and agree with no objection thereto. Except as provided in a subsidiary agreement, nothing in this LSA grants you the right to use IoTware Project or the name, service mark, logo, domain name and other unique identification marks of ETRI.
If you use the source form version or object form version of IoTware Project in whole or in part to develop a code or a derivative work, and you do not commercialize the result in any form, you will be covered under an open source license. IoTware Project is in accordance with Free Software Foundation (FSF)'s open source policy, and is allowed to use it in the appropriate scope and manner, and you must comply with the applicable open source license policy applied to IoTware Project. IoTware Project is, in principle, subject to GNU Lesser General Public License version 2.1 (LGPLv2.1). If you have acquired all or a part of the IoTware Project in any way and it is subject to a license other than the open source license described above, please contact the following address for the technical support and other inquiries before use, and check the usage information.
*/

/**
 * @file iw_fw_collabomsg_internal.c
 * @brief 내부 용 분산 협업 관련 정의 헤더 파일
 * @details 내부 시스템에서만 사용하기 위해 분리된 분산 협업 관련 정의 헤더 파일
 */
#pragma once

#include <stdlib.h>
#include <string.h>

#include "iw_fw_stdmacros.h"

#pragma pack(push, 1)

/**
 * @brief 기본 헤더 정보 구조체 (1byte)
 */
struct CMFixedHeader {
	uint8_t ack : 1;						/**< ACK 여부  */
	uint8_t message_id : 5;					/**< 메시지 ID  */
	uint8_t dup : 1;						/**< DUP  */
	uint8_t qos : 1;						/**< QOS  */
};

/**
 * @brief 기본 ACK 정보 구조체 (2bytes)
 */
struct CMDefaultAckMsg {
	uint8_t ack : 1;						/**< ACK 여부  */
	uint8_t message_id : 5;					/**< 메시지 ID  */
	uint8_t dup : 1;						/**< DUP  */
	uint8_t qos : 1;						/**< QOS  */
	uint8_t reserved : 2;					/**< 예약  */
	uint8_t return_code : 6;				/**< return code  */
};

/**
 * @brief ACK 여부
 */
enum CM_ACK {
	CM_ACK_REQ_MSG = 0,						/**< 일반 메시지  */
	CM_ACK_MSG_ID,							/**< ACK  */
};

/**
 * @brief QOS
 */
enum CM_QOS {
	CM_QOS_ACK_NONE = 0,					/**< ACK 불필요  */
	CM_QOS_ACK_NEED,						/**< ACK 필요  */
};

/**
 * @brief return code
 */
enum CM_RET_CODE {
	CM_RET_CODE_ACCEPTED = 0,				/**<   */
	CM_RET_CODE_NOT_SUPPORTED,				/**<   */
	CM_RET_CODE_INVALID_TASK_ID,			/**<   */
	CM_RET_CODE_INVALID_OBJECT_ID,			/**<   */
	CM_RET_CODE_INVALID_ATTRIBUTE_ID,		/**<   */
};

/**
 * @brief 메시지 ID
 */
enum CM_MSG_ID {
	CM_MSG_ID_SERVER_ADVERTISE = 0x00,		/**< Server Advertise  */
	CM_MSG_ID_DISCOVER_SERVER = 0x01,		/**< 미구현  */
	CM_MSG_ID_SERVER_INFO = 0x01,			/**< 미구현  */
	CM_MSG_ID_CONNECT = 0x02,				/**< Connect  */
	CM_MSG_ID_CONNECT_ACK = 0x02,			/**< Connect ACK  */
	CM_MSG_ID_MEMBER_INFO = 0x03,			/**< Member Info  */
	CM_MSG_ID_SET_DEVICE_MODE = 0x04,		/**< Set Device Mode  */
	CM_MSG_ID_CHECK_STATUS = 0x05,			/**< Check Status  */
	CM_MSG_ID_CHECK_STATUS_ACK = 0x05,		/**< Check Status ACK  */
	CM_MSG_ID_POWER_INFO = 0x06,			/**< 미구현  */
	CM_MSG_ID_PING_REQUEST = 0x07,			/**< 미구현  */
	CM_MSG_ID_PING_REQUEST_ACK = 0x07,		/**< 미구현  */
	// 0x08 reserved
	// 0x09 reserved
	// 0x0a reserved
	// 0x0b reserved
	// 0x0c reserved
	// 0x0d reserved
	// 0x0e reserved
	// 0x0f reserved
	CM_MSG_ID_TASK_CREATION = 0x10,			/**< Task Creation  */
	CM_MSG_ID_TASK_RUN_ALL = 0x11,			/**< Task Run All  */
	CM_MSG_ID_TASK_STOP_ALL = 0x12,			/**< Task Stop All  */
	CM_MSG_ID_TASK_RUN = 0x13,				/**< Task Run  */
	CM_MSG_ID_TASK_STOP = 0x14,				/**< Task Stop  */
	CM_MSG_ID_ATTRIBUTE_WRITE = 0x15,		/**< Attribute Write  */
	CM_MSG_ID_ATTRIBUTE_READ = 0x16,		/**< Attribute Read  */
	CM_MSG_ID_ATTRIBUTE_READ_ACK = 0x16,	/**< Attribute Read ACK  */
	CM_MSG_ID_TASK_INPUT_DATA = 0x17,		/**< Task Input Data  */
	CM_MSG_ID_TASK_OUTPUT_DATA = 0x18,		/**< Task Output Data  */
	CM_MSG_ID_TASK_INFO = 0x19,				/**< 미구현  */
	// 0x1a reserved
	// 0x1b reserved
	// 0x1c reserved
	// 0x1d reserved
	// 0x1e reserved
	// 0x1f reserved
};

/**
 * @brief Task ID
 */
enum CM_TASK_ID {
	CM_TASK_ID_GENERIC_TASK = 0,			/**< Generic  */
	CM_TASK_ID_SENSING_TASK = 1,			/**< Sensing  */
	CM_TASK_ID_NUMERIC_TASK = 2,			/**< Numeric  */
	CM_TASK_ID_EVENT_TASK = 3,				/**< Event  */
};

/**
 * @brief Object ID
 */
enum CM_OBJECT_ID {
	CM_OBJECT_ID_GENERIC = 0,				/**< Generic  */
	CM_OBJECT_ID_GENERIC_SENSOR = 1,		/**< Generic Sensor  */
	CM_OBJECT_ID_ILLUMINANCE = 2,			/**< Illuminance  */
	CM_OBJECT_ID_PRESENCE = 3,				/**< Presence  */
	CM_OBJECT_ID_TEMPERATURE = 4,			/**< Temperature  */
	CM_OBJECT_ID_HUMIDITY = 5,				/**< Humidity  */
	CM_OBJECT_ID_POWER_MEASUREMENT = 6,		/**< Power Measurement  */
	CM_OBJECT_ID_ACTUATION = 7,				/**< Actuation  */
	CM_OBJECT_ID_CO2 = 8,					/**< CO2  */
	CM_OBJECT_ID_TIMER = 9,					/**< Timer  */
	CM_OBJECT_ID_AVERAGE = 10,				/**< Average  */
	CM_OBJECT_ID_SUM = 11,					/**< Sum  */
	CM_OBJECT_ID_COUNTER = 12,				/**< Counter  */
};

/**
 * @brief Attribute ID
 */
enum CM_ATTRIBUTE_ID {
	CM_ATTRIBUTE_ID_GENERIC = 0,			/**< Generic  */
	CM_ATTRIBUTE_ID_SENSOR_VALUE = 1,		/**< Sensor Value  */
	CM_ATTRIBUTE_ID_SENSOR_UNITS = 2,		/**< Sensor Units  */
	CM_ATTRIBUTE_ID_MIN_MEASURED_VALUE = 3,	/**< Min Measured Value  */
	CM_ATTRIBUTE_ID_MAX_MEASURED_VALUE = 4,	/**< Max Measured Value  */
	CM_ATTRIBUTE_ID_MIN_RANGE_VALUE = 5,	/**< Min Range Value  */
	CM_ATTRIBUTE_ID_MAX_RANGE_VALUE = 6,	/**< Max Range Value  */
	CM_ATTRIBUTE_ID_AVERAGE_VALUE = 7,		/**< Average Value  */
	CM_ATTRIBUTE_ID_ARRAY_VALUE = 8,		/**< Array Value  */
	CM_ATTRIBUTE_ID_PERIOD = 9,				/**< Period  */
	CM_ATTRIBUTE_ID_NUMBER = 10,			/**< Number  */
};

/**
 * @brief Attributes 메시지의 추가 Payload 구조체
 */
struct CMAttributePayload {
	uint8_t attribute_id : 6;				/**< Attribute ID  */
	uint8_t value_length : 2;				/**< Value Length  */
};

/**
 * @brief Attribute Values 메시지의 추가 Payload 구조체
 */
struct CMAttributeValuePayload {
	uint8_t attribute_id : 6;				/**< Attribute ID  */
	uint8_t value_length : 2;				/**< Value Length  */
	uint8_t *values;						/**< 추가 Payload 포인터  */
};

#pragma pack(pop)

uint8_t *cm_header_serialize(const struct CMFixedHeader *msg, size_t *size);
uint8_t *cm_default_ack_serialize(const struct CMDefaultAckMsg *msg, size_t *size);

struct CMFixedHeader cm_header_parse(const uint8_t *msg, const size_t size);
struct CMDefaultAckMsg cm_default_ack_parse(const uint8_t*msg, const size_t size);

size_t cm_attributes_serialize_size(const size_t count);
size_t cm_attribute_values_serialize_size(const struct CMAttributeValuePayload *msg, const size_t count);
void cm_attributes_serialize(const struct CMAttributePayload *msg, const size_t count, uint8_t *dest);
void cm_attribute_values_serialize(const struct CMAttributeValuePayload *msg, const size_t count, uint8_t *dest);

struct CMAttributePayload *cm_attributes_parse(const uint8_t *msg, const size_t size, const size_t attr_count);
size_t sizeof_attribute_value_payload();
struct CMAttributeValuePayload *cm_attribute_values_parse(const uint8_t *msg, const size_t size, const size_t attr_count);

size_t cm_attribute_real_attr_count(const uint8_t packed_count);
uint8_t cm_attribute_packed_attr_count(const size_t real_count);

size_t cm_attribute_real_value_length(const uint8_t packed_length);
uint8_t cm_attribute_packed_value_length(const size_t real_length);
