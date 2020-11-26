/*
Copyright (C) <2020> <ETRI>

This License and Service Agreement (LSA) applies to all works and their derivative works based on source form version and object form version of IoTware Project. Currently, the LSA for IoTware Project has two policies, 'Open Source License' and 'Commercial License'. Therefore, all works including the source code and executable code of IoTware Project and derivative works based thereon are subject to either 'Open Source License' or 'Commercial License' depending on the user's needs and purpose. Details related to the selection of the applicable license are specified in this LSA. If you use any or all of IoTware Project in any form, you are deemed to have consented to this LSA. If you breach any of the terms and conditions set forth in this LSA, you are solely responsible for any losses or damages incurred by Electronics and Communications Research Institute (ETRI), and ETRI assume no responsibility for you or any third party.
If you use the source form version or object form version of IoTware Project in whole or in part to develop a code or a derivative work, and you want to commercialize the result in some form, you will be covered under a commercial license. And if you are subject to a commercial license, the contract for the use of IoTware Project is subject to TECHNOLOGY LICENSE AGREEMENT of ETRI. You acknowledge that ETRI has all legal rights, title and interest, including intellectual property rights in the IoTware Project (regardless of whether such intellectual property rights are registered or where such rights exist) and agree with no objection thereto. Except as provided in a subsidiary agreement, nothing in this LSA grants you the right to use IoTware Project or the name, service mark, logo, domain name and other unique identification marks of ETRI.
If you use the source form version or object form version of IoTware Project in whole or in part to develop a code or a derivative work, and you do not commercialize the result in any form, you will be covered under an open source license. IoTware Project is in accordance with Free Software Foundation (FSF)'s open source policy, and is allowed to use it in the appropriate scope and manner, and you must comply with the applicable open source license policy applied to IoTware Project. IoTware Project is, in principle, subject to GNU Lesser General Public License version 2.1 (LGPLv2.1). If you have acquired all or a part of the IoTware Project in any way and it is subject to a license other than the open source license described above, please contact the following address for the technical support and other inquiries before use, and check the usage information.
*/

#include "iw_common.h"
#include "iw_sys.h"
#include "iw_dev_batt.h"

static power_mode_t g_pwr_mode = POW_RUN_MODE;
static auto_control_t g_autocontrol_mode = AUTOCONTROL_OFF;
//static lowpower_mode_t g_lowpower_mode = LOWPOWER_OFFMODE;

iw_error_t iw_get_mcu_info(mcu_type_t *id)
{
	if (id != NULL)
	{
#if (IW_MCU_STM32 == 1)
		*id = MCU_TYPE_STM32L476;
#elif (IW_MCU_NORDIC == 1)
		*id = MCU_TYPE_NRF52840;
#else
		*id = MCU_TYPE_UNKNOWN;
#endif
		return IW_SUCCESS;
	}
	else
		return IW_FAIL;
}

const char* iw_get_mcu_name(mcu_type_t type)
{
	switch (type) {
		case MCU_TYPE_STM32L476:	return "STM32L476 ";
		case MCU_TYPE_NRF52840:		return "NRF52840  ";
		case MCU_TYPE_CORTEX_M0:	return "CORTEX M0 ";
		case MCU_TYPE_TCC_EMUL:		return "TCC_EMUL  ";
		case MCU_TYPE_RESERVED:		return "RESERVED  ";
		default:					return "UNKNOWN   ";
	}

	return 0;
}

iw_error_t iw_get_autocontrol(auto_control_t *enable)
{
	*enable = g_autocontrol_mode;
	return IW_SUCCESS;
}

iw_error_t iw_set_autocontrol(auto_control_t enable)
{
	g_autocontrol_mode = enable;

	if(g_autocontrol_mode == AUTOCONTROL_OFF) {
		iw_set_power_mode(POW_RUN_MODE);
	}

	return IW_SUCCESS;
}

iw_error_t iw_get_power_mode(power_mode_t *pwr_mode)
{
	*pwr_mode = g_pwr_mode;
	return IW_SUCCESS;
}

iw_error_t iw_set_power_mode(power_mode_t pwr_mode)
{
	g_pwr_mode = pwr_mode;
	return IW_SUCCESS;
#if 0
	if (pwr_mode == POW_RUN_MODE || pwr_mode == POW_LOW_MODE)
	{
		g_pwr_mode = pwr_mode;
		if (pwr_mode == POW_LOW_MODE)
		{
			bsp_enter_sleepmode(32000);
			//           sleep_mode_enter();
		}
		return IW_SUCCESS;
	}
	else
	{
		return IW_FAIL;
	}
#endif	
}

// iw_error_t iw_set_lowpower_status(lowpower_mode_t enable)
// {
// 	g_lowpower_mode = enable;
// 	return IW_SUCCESS;
// }

// iw_error_t iw_get_lowpower_status(lowpower_mode_t *enable)
// {
// 	if (enable != NULL)
// 	{
// 		*enable = g_lowpower_mode;
// 		return IW_SUCCESS;
// 	}
// 	return IW_FAIL;
// }

/*
	Li-po battery discharge
	min : 3.0V
	max : 4.2V
*/
const uint8_t batt_table[151] =
{
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3,
	3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5,
	5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21,
	22, 23, 24, 25, 28, 30, 33, 35, 38, 40, 43, 45, 48, 50, 53, 55,
	58, 60, 63, 65, 68, 70, 73, 75, 76, 77, 78, 78, 79, 80, 81, 82,
	83, 83, 84, 85, 86, 87, 88, 88, 89, 90, 91, 92, 93, 93, 94, 95,
	96, 97, 98, 98, 99, 100
};


iw_error_t iw_get_battery_status(int *percent, int *time)
{
	uint32_t idx = 0;
	uint16_t volt;

	if(iw_get_battery_voltage(&volt) == IW_SUCCESS) {
		if(volt < 2700) volt = 2700;
		else if(4200 < volt) volt = 4200;

		idx = (volt - 2700) / 10;
		*percent = batt_table[idx];
		*time = 0;

		//iw_printf("iw_get_battery_status() %d %d\n", volt, idx);

		return IW_SUCCESS;
	}

	return IW_FAIL;
}

iw_error_t iw_get_memory_status(uint32_t *global_data, uint32_t *total_heap, uint32_t *free_heap)
{
	if (global_data != NULL)
	{
		*global_data = GLOBAL_DATA_SIZE;
	}

	if (total_heap != NULL)
	{
		*total_heap = TOTAL_HEAP_SIZE;
	}

	if (free_heap != NULL)
	{
		*free_heap = FREE_HEAP_SIZE;
	}
	return IW_SUCCESS;
}

iw_error_t iw_get_network_status(uint8_t dev_id, int *connected, int *strength, wake_src_type_t *wake_src, net_pwr_mode_t *mode)
{
	iw_error_t ret = IW_FAIL;

	ret = iw_is_connected_network(dev_id, connected);
	if (ret == IW_FAIL)
		return ret;

	ret = iw_get_rssi_network(dev_id, strength);
	if (ret == IW_FAIL)
		return ret;

	ret = iw_get_config_network(dev_id, wake_src, NULL);
	if (ret == IW_FAIL)
		return ret;

	ret = iw_get_pwrmode_network(dev_id, mode);
	return ret;
}

iw_error_t iw_get_sensor_status(uint8_t dev_id, wake_src_type_t *wake_src, sensor_pwr_mode_t *mode)
{
	iw_error_t ret = IW_FAIL;
	ret = iw_get_config_sensor(dev_id, wake_src, NULL);
	if (ret == IW_FAIL)
		return ret;

	ret = iw_get_pwrmode_sensor(dev_id, mode);
	return ret;
}
