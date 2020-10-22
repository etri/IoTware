

### Get development environment
ifeq ($(findstring MINGW, $(shell uname)), MINGW)
    export OS = Mingw
else
    export OS = Linux
endif



ifeq ($(IW_OS),nanoqplus)
export NOS_HOME := $(IW_HOME)/os/nanoqplus

ifeq ($(IW_BSP),stm32)
export IW_MCU      := stm32l4xx
export IW_PLATFORM := stm32l4_nucleo
else ifeq ($(IW_BSP),nordic)
export IW_MCU      := nRF52840
export IW_PLATFORM := nRF52840
endif

endif

### for windows ###
ifeq ($(OS),Mingw)
PWD = $(shell npwd)
export PWD
endif

include $(IW_MK_PATH)/toolchain.mk

include ${IW_HOME}/oal/Makefile

include $(IW_HOME)/include/Makefile
include $(IW_HOME)/LWMSG/Makefile
include $(IW_HOME)/COLLABOMSG/Makefile
include $(IW_HOME)/Microservice/Makefile

#include $(IW_HOME)/app/Makefile
include $(IW_APP_HOME)/Makefile

include $(IW_HOME)/../Task/Makefile
include $(IW_MK_PATH)/compile.mk

include $(IW_MK_PATH)/burn.mk

