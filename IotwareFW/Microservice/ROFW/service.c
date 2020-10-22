/*
Copyright (C) <2020> <ETRI>

This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 3.0 of the License, or (at your option) any later version. This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details. You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
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

