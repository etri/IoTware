


ifeq ($(BOARD),iotware)
IW_CFLAGS += -DIOTWARE_BOARD=1
else ifeq ($(BOARD),disposal_iot)
IW_CFLAGS += -DDISPOSAL_IOT_BOARD=1
endif

ifdef LOWPOWER_ENABLE
IW_CFLAGS += -DLOWPOWER_ENABLE=$(LOWPOWER_ENABLE)
endif # LOWPOWER_ENABLE

#IW_CFLAGS += -D__OS_TEST__
IW_CFLAGS += -D__TO21COMMS__

include ${IW_HOME}/oal/drivers/Makefile
include ${IW_HOME}/oal/iw/Makefile
include ${IW_HOME}/oal/interface/Makefile
include ${IW_HOME}/oal/bsp/Makefile
include ${IW_HOME}/oal/kernel/Makefile


