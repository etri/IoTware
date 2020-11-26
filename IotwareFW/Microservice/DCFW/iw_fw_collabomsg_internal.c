/*
Copyright (C) <2020> <ETRI>

This License and Service Agreement (LSA) applies to all works and their derivative works based on source form version and object form version of IoTware Project. Currently, the LSA for IoTware Project has two policies, 'Open Source License' and 'Commercial License'. Therefore, all works including the source code and executable code of IoTware Project and derivative works based thereon are subject to either 'Open Source License' or 'Commercial License' depending on the user's needs and purpose. Details related to the selection of the applicable license are specified in this LSA. If you use any or all of IoTware Project in any form, you are deemed to have consented to this LSA. If you breach any of the terms and conditions set forth in this LSA, you are solely responsible for any losses or damages incurred by Electronics and Communications Research Institute (ETRI), and ETRI assume no responsibility for you or any third party.
If you use the source form version or object form version of IoTware Project in whole or in part to develop a code or a derivative work, and you want to commercialize the result in some form, you will be covered under a commercial license. And if you are subject to a commercial license, the contract for the use of IoTware Project is subject to TECHNOLOGY LICENSE AGREEMENT of ETRI. You acknowledge that ETRI has all legal rights, title and interest, including intellectual property rights in the IoTware Project (regardless of whether such intellectual property rights are registered or where such rights exist) and agree with no objection thereto. Except as provided in a subsidiary agreement, nothing in this LSA grants you the right to use IoTware Project or the name, service mark, logo, domain name and other unique identification marks of ETRI.
If you use the source form version or object form version of IoTware Project in whole or in part to develop a code or a derivative work, and you do not commercialize the result in any form, you will be covered under an open source license. IoTware Project is in accordance with Free Software Foundation (FSF)'s open source policy, and is allowed to use it in the appropriate scope and manner, and you must comply with the applicable open source license policy applied to IoTware Project. IoTware Project is, in principle, subject to GNU Lesser General Public License version 2.1 (LGPLv2.1). If you have acquired all or a part of the IoTware Project in any way and it is subject to a license other than the open source license described above, please contact the following address for the technical support and other inquiries before use, and check the usage information.
*/

/**
 * @file iw_fw_collabomsg_internal.c
 * @brief 내부 용 분산 협업 관련 정의 구현 파일
 * @details 내부 시스템에서만 사용하기 위해 분리된 분산 협업 관련 정의 구현 파일
 */
#include "iw_fw_collabomsg_internal.h"

/**
 * @brief 기본 헤더 구조체 직렬화
 * @details 기본 헤더 구조체 직렬화
 * @param msg 기본 헤더 구조체 포인터
 * @param size 직렬화 한 바이트 배열 길이 출력
 * @return 직렬화 한 바이트 배열 포인터
 */
uint8_t *cm_header_serialize(const struct CMFixedHeader *msg, size_t *size)
{
	*size = sizeof(struct CMFixedHeader);
	uint8_t *buf = calloc(1, *size);
	if(buf!=NULL)
		memcpy(buf, msg, *size);
	
	return buf;
}

/**
 * @brief 기본 ACK 구조체 직렬화
 * @details 기본 ACK 구조체 직렬화
 * @param msg 기본 ACK 구조체 포인터
 * @param size 직렬화 한 바이트 배열 길이 출력
 * @return 직렬화 한 바이트 배열 포인터
 */
uint8_t *cm_default_ack_serialize(const struct CMDefaultAckMsg *msg, size_t *size)
{
	*size = sizeof(struct CMDefaultAckMsg);
	uint8_t *buf = calloc(1, *size);
	if(buf!=NULL)
		memcpy(buf, msg, *size);
	return buf;
}

/**
 * @brief 기본 헤더 구조체 파싱
 * @details 기본 헤더 구조체 파싱
 * @param msg 바이트 배열 포인터
 * @param size 바이트 배열 길이
 * @return 기본 헤더 구조체
 */
struct CMFixedHeader cm_header_parse(const uint8_t *msg, const size_t size)
{
	struct CMFixedHeader buf;

	if (sizeof(struct CMFixedHeader) <= size) {
		memcpy(&buf, msg, sizeof(struct CMFixedHeader));
	}

	return buf;
}

/**
 * @brief 기본 ACK 구조체 파싱
 * @details 기본 ACK 구조체 파싱
 * @param msg 바이트 배열 포인터
 * @param size 바이트 배열 길이
 * @return 기본 ACK 구조체
 */
struct CMDefaultAckMsg cm_default_ack_parse(const uint8_t*msg, const size_t size)
{
	struct CMDefaultAckMsg buf;

	if (sizeof(struct CMDefaultAckMsg) <= size) {
		memcpy(&buf, msg, sizeof(struct CMDefaultAckMsg));
	}

	return buf;
}

/**
 * @brief 실제 Attribute Count 값 획득
 * @details 실제 값과 다른, 구조체에 저장된 Attribute Count 값을 실제 값으로 역산
 * @param packed_count 구조체에 저장된 Attribute Count 값
 * @return 실제 Attribute Count 값
 */
size_t cm_attribute_real_attr_count(const uint8_t packed_count)
{
	return packed_count + 1;
}

/**
 * @brief 구조체에 저장될 Attribute Count 값 획득
 * @details 실제 Attribute Count 값을 구조체에 저장할 값으로 계산
 * @param real_count 구조체에 저장된 Attribute Count 값
 * @return 구조체에 저장할 Attribute Count 값
 */
uint8_t cm_attribute_packed_attr_count(const size_t real_count)
{
	return real_count - 1;
}

/**
 * @brief 실제 Value Length 값 획득
 * @details 실제 값과 다른, 구조체에 저장된 Value Length 값을 실제 값으로 역산
 * @param packed_length 구조체에 저장된 Value Length 값
 * @return 실제 Value Length 값
 */
size_t cm_attribute_real_value_length(const uint8_t packed_length)
{
	size_t rtn = 1;
	for (int i=0;i<packed_length;i++)
		rtn *= 2;
	return rtn;
}


/**
 * @brief 구조체에 저장될 Value Length 값 획득
 * @details 실제 Value Length 값을 구조체에 저장할 값으로 계산
 * @param real_count 구조체에 저장된 Value Length 값
 * @return 구조체에 저장할 Value Length 값
 */
uint8_t cm_attribute_packed_value_length(const size_t real_length)
{
	switch (real_length) {
		case 8:
			return 3;
		case 4:
			return 2;
		case 2:
			return 1;
		case 1:
		default:
			break;
	}
	return 0;
}

/**
 * @brief Attribute Values 메시지의 추가 Payload 구조체의 직렬화된 크기 획득
 * @details Attribute Values 메시지의 추가 Payload 구조체의 직렬화된 크기 획득
 * @return Attribute Values 메시지의 추가 Payload 구조체의 직렬화된 크기
 */
size_t sizeof_attribute_value_payload()
{
	return sizeof(struct CMAttributeValuePayload) - sizeof(uint8_t *);
}

/**
 * @brief Attributes 메시지의 추가 Payload 구조체의 직렬화된 크기 획득
 * @details Attributes 메시지의 추가 Payload 구조체의 직렬화된 크기 획득
 * @param count 실제 Attribute Count
 * @return Attributes 메시지의 추가 Payload 구조체의 직렬화된 크기
 */
size_t cm_attributes_serialize_size(const size_t count)
{
	return sizeof(struct CMAttributePayload) * count;
}

/**
 * @brief Attributes 메시지의 추가 Payload 구조체 직렬화
 * @details Attributes 메시지의 추가 Payload 구조체 직렬화
 * @param msg Attributes 메시지의 추가 Payload 구조체 포인터
 * @param count 실제 Attribute Count
 * @param dest 직렬화 한 바이트 배열이 담길 버퍼
 */
void cm_attributes_serialize(const struct CMAttributePayload *msg, const size_t count, uint8_t *dest)
{
	if (count > 0) {
		memcpy(dest, msg, cm_attributes_serialize_size(count));
	}
}

/**
 * @brief Attribute Values 메시지의 추가 Payload 구조체의 직렬화된 크기 획득
 * @details Attribute Values 메시지의 추가 Payload 구조체의 직렬화된 크기 획득
 * @param msg Attribute Values 메시지의 추가 Payload 구조체 포인터
 * @param count 실제 Attribute Count
 * @return Attribute Values 메시지의 추가 Payload 구조체의 직렬화된 크기
 */
size_t cm_attribute_values_serialize_size(const struct CMAttributeValuePayload *msg, const size_t count)
{
	size_t size = 0;
	for (int i=0;i<count;i++) {
		size += cm_attribute_real_value_length(msg[i].value_length);
	}
	size += sizeof_attribute_value_payload() * count;
	return size;
}

/**
 * @brief Attribute Values 메시지의 추가 Payload 구조체 직렬화
 * @details Attribute Values 메시지의 추가 Payload 구조체 직렬화
 * @param msg Attribute Values 메시지의 추가 Payload 구조체 포인터
 * @param count 실제 Attribute Count
 * @param dest 직렬화 한 바이트 배열이 담길 버퍼
 */
void cm_attribute_values_serialize(const struct CMAttributeValuePayload *msg, const size_t count, uint8_t *dest)
{
	if (count > 0) {
		size_t size = cm_attribute_values_serialize_size(msg, count);
		uint8_t *pt = dest;
		size_t sz = sizeof_attribute_value_payload();
		for (int i=0;i<count;i++) {
			memcpy(pt, msg + i, sz);
			pt += sz;
			size_t value_length = cm_attribute_real_value_length(msg[i].value_length);
			memcpy(pt, msg[i].values, value_length);
			pt += value_length;
		}
	}
}

/**
 * @brief Attributes 메시지의 추가 Payload 구조체 파싱
 * @details Attributes 메시지의 추가 Payload 구조체 파싱
 * @param msg 파싱 할 바이트 배열
 * @param size 바이트 배열의 길이
 * @param attr_count 구조체에 저장된 Attribute Count
 */
struct CMAttributePayload *cm_attributes_parse(const uint8_t *msg, const size_t size, const size_t attr_count)
{
	struct CMAttributePayload *attributes = NULL;
	size_t sz = sizeof(struct CMAttributePayload);
	if (sz <= size) {
		size_t real_attr_count = cm_attribute_real_attr_count(attr_count);
		attributes = calloc(real_attr_count, sz);
		const uint8_t *pt = msg;
		for (int i=0;i<real_attr_count;i++) {
			memcpy(attributes+i, pt, sz);
			pt += sz;
		}
	}
	return attributes;
}

/**
 * @brief Attribute Values 메시지의 추가 Payload 구조체 파싱
 * @details Attribute Values 메시지의 추가 Payload 구조체 파싱
 * @param msg 파싱 할 바이트 배열
 * @param size 바이트 배열의 길이
 * @param attr_count 구조체에 저장된 Attribute Count
 */
struct CMAttributeValuePayload *cm_attribute_values_parse(const uint8_t *msg, const size_t size, const size_t attr_count)
{
	struct CMAttributeValuePayload *attributes = NULL;
	size_t sz = sizeof_attribute_value_payload();
	if (sz <= size) {
		size_t real_attr_count = cm_attribute_real_attr_count(attr_count);
		attributes = calloc(real_attr_count, sizeof(struct CMAttributeValuePayload));
		const uint8_t *pt = msg;
		if(attributes != NULL){
			for (int i=0;i<real_attr_count;i++) {
				memcpy(attributes+i, pt, sz);
				pt += sz;
				size_t values_size = cm_attribute_real_value_length(attributes[i].value_length) * sizeof(uint8_t);
				uint8_t *values = calloc(1, values_size);
				if(values != NULL){
					memcpy(values, pt, values_size);
					attributes[i].values = values;
					pt += values_size;
				}
			}
		}
	}
	return attributes;
}
