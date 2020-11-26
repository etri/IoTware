/*
Copyright (C) <2020> <ETRI>

This License and Service Agreement (LSA) applies to all works and their derivative works based on source form version and object form version of IoTware Project. Currently, the LSA for IoTware Project has two policies, 'Open Source License' and 'Commercial License'. Therefore, all works including the source code and executable code of IoTware Project and derivative works based thereon are subject to either 'Open Source License' or 'Commercial License' depending on the user's needs and purpose. Details related to the selection of the applicable license are specified in this LSA. If you use any or all of IoTware Project in any form, you are deemed to have consented to this LSA. If you breach any of the terms and conditions set forth in this LSA, you are solely responsible for any losses or damages incurred by Electronics and Communications Research Institute (ETRI), and ETRI assume no responsibility for you or any third party.
If you use the source form version or object form version of IoTware Project in whole or in part to develop a code or a derivative work, and you want to commercialize the result in some form, you will be covered under a commercial license. And if you are subject to a commercial license, the contract for the use of IoTware Project is subject to TECHNOLOGY LICENSE AGREEMENT of ETRI. You acknowledge that ETRI has all legal rights, title and interest, including intellectual property rights in the IoTware Project (regardless of whether such intellectual property rights are registered or where such rights exist) and agree with no objection thereto. Except as provided in a subsidiary agreement, nothing in this LSA grants you the right to use IoTware Project or the name, service mark, logo, domain name and other unique identification marks of ETRI.
If you use the source form version or object form version of IoTware Project in whole or in part to develop a code or a derivative work, and you do not commercialize the result in any form, you will be covered under an open source license. IoTware Project is in accordance with Free Software Foundation (FSF)'s open source policy, and is allowed to use it in the appropriate scope and manner, and you must comply with the applicable open source license policy applied to IoTware Project. IoTware Project is, in principle, subject to GNU Lesser General Public License version 2.1 (LGPLv2.1). If you have acquired all or a part of the IoTware Project in any way and it is subject to a license other than the open source license described above, please contact the following address for the technical support and other inquiries before use, and check the usage information.
*/

#include "iw_common.h"
#include "iw_sys.h"
#include "iw_oal.h"
#include "iw_dev_batt.h"
#include "service.h"

static unsigned char lowbattery_control = 0;
#if 0
static unsigned char lowsignal_control = 0;
#endif
static unsigned char lowpower_scene_control = 0;

unsigned char get_lowbattery_control(void)
{
    return lowbattery_control;
}
#if 0
unsigned char get_lowsignal_control(void)
{
    return lowsignal_control;
}
#endif
unsigned char get_lowpower_scene_control(void)
{
    return lowpower_scene_control;
}

void autocontrol_lowbattery(void)
{
    unsigned short voltage=0;
	unsigned int   sum=0;
	unsigned char  cnt=0;
	
    for(cnt=5;cnt > 0; cnt--)
    {
        iw_get_battery_voltage(&voltage);
        sum += voltage;
		//iw_printf("vol:%dmV\n", voltage);
		iw_sleep(2000);	
    }
	sum /= 5;
	//iw_printf("sum vol:%dmV\n", sum);

	if( sum < 3500 )  // 3.5 v
	{
		if( sum < 3400 )  // 3.4 v
		{
			lowbattery_control=2;
		}
		else
		{
            lowbattery_control=1;
		}
	}
	else
	{
        lowbattery_control=0;
	}
}

#if 0
void autocontrol_lowsignal(void)
{
    //unsigned char ret_val = 0;
	unsigned char dev_id = 0;
	int rssi=0;

	iw_get_network_devid(NET_DEV_TYPE_LORA, NULL, &dev_id);
	iw_get_rssi_network(dev_id, &rssi);

	if(rssi < -80)
	{
		if( rssi < -85 )
		{
			lowsignal_control=2;
		}
		else
		{
            lowsignal_control=1;
		}
	}
}
#endif

void autocontrol_lowpower_scene(void)
{
    //iw_printf("autocontrol_lowpower_scene\n");
}
				
void autocontrol_task(void *args)
{
    auto_control_t automode=AUTOCONTROL_OFF;

	//iw_printf("autocontrol_task\n");
    int loop_condition=TRUE;
	while(loop_condition == FALSE)
    {
	    if(iw_get_autocontrol(&automode)==IW_SUCCESS)
	    {
			if (automode == AUTOCONTROL_ON)
		    {
                autocontrol_lowbattery();
				//autocontrol_lowsignal();
				#if 1
                autocontrol_lowpower_scene();
				#endif
			}
			else
			{
			    ;
			}
	    }
        iw_sleep(5000);
		iw_sleep(5000);
		iw_sleep(5000);
	}
}

void lowpower_task(void *args)
{
	int loop_condition=TRUE;
	while(loop_condition == FALSE)
	{
        iw_sleep(5000);
	}
}

void iw_init_autocontrol_service(void)
{
    //iw_set_autocontrol(AUTOCONTROL_ON);
    iw_create_task(autocontrol_task, "autocontrol_task", 0x50, NULL, IW_TSK_PRIO_IDLE+5, NULL);
}

void iw_init_lowpower_service(void)
{
    iw_create_task(lowpower_task, "lowpower_task", 0x50, NULL, IW_TSK_PRIO_IDLE+5, NULL);
}

