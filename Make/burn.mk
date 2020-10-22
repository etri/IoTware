#
# Makefile for make burn and debug
#

######### Commands for flash fusing and debugging ##########

ifeq ($(JTAG_LINK),)
export JTAG_LINK = ft2232
endif

ifneq ($(IW_BURN_BSP),)
IW_BSP := $(IW_BURN_BSP)
endif

ifeq ($(IW_BSP), stm32)
export OPENOCD_CFG = board/st_l476rg-$(JTAG_LINK).cfg
else ifeq ($(IW_BSP), nordic)
export OPENOCD_CFG = board/nordic_nrf51_dk-$(JTAG_LINK).cfg
else ifeq ($(IW_BSP), telechips)

else
$(error Error : IW_BSP = $(IW_BSP))
endif

ifeq ($(OS),Mingw)
export OPENOCD_CMD = $(IW_HOME)/../tools/openocd/bin/openocd
else
export OPENOCD_CMD = sudo $(IW_HOME)/../tools/openocd/bin/openocd
endif

ifeq ($(IW_BSP),nordic)
EXT := hex
IW_BURN_HEX := yes
else
EXT := bin
endif

ifneq ($(wildcard $(IW_HOME)/wdk.$(EXT)),)
TARGET := $(IW_HOME)/wdk
else
TARGET := $(IW_APP_HOME)/out/bin/$(IW_TARGET)
# TARGET := $(IW_PWD)/out/bin/$(IW_TARGET)
endif

burn :
ifneq ($(IW_BURN_HEX),)
	#$(OPENOCD_CMD) -f $(OPENOCD_CFG) -c 'wdk-program $(IW_HOME)/$(IW_BURN_HEX)'
	#$(OPENOCD_CMD) -f $(OPENOCD_CFG) -c 'wdk-program $(IW_PWD)/$(IW_BURN_HEX)'
	$(OPENOCD_CMD) -f $(OPENOCD_CFG) -c 'program $(TARGET).hex verify reset exit'
else
#ifeq ($(JTAG_LINK),ft2232)
#	$(OPENOCD_CMD) -f $(OPENOCD_CFG) -c 'program $(TARGET).bin 0x8000000 verify reset exit'
#else
#	$(OPENOCD_CMD) -f $(OPENOCD_CFG) -c 'wdk-program $(TARGET).$(EXT)'
#endif
	$(OPENOCD_CMD) -f $(OPENOCD_CFG) -c 'program $(TARGET).bin 0x8000000 verify reset exit'
endif

burn-softdevice :
#	$(OPENOCD_CMD) -f $(OPENOCD_CFG) -c 'wdk-program $(IW_HOME)/oal/bsp/firmware/wdk-nRF52840-softdevice.hex'
	$(OPENOCD_CMD) -f $(OPENOCD_CFG) -c 'program $(IW_HOME)/oal/bsp/firmware/wdk-nRF52840-softdevice.hex 0x0 verify reset exit'

debug :
	$(OPENOCD_CMD) -f $(OPENOCD_CFG) -c wdk-debug

gdb :
ifneq ("$(IW_EMULATOR)","1")
	$(GDB) -x=$(IW_HOME)/tools/openocd/wdk.gdb $(TARGET).elf
else
	$(GDB) -x=$(IW_HOME)/tools/openocd/wemu.gdb $(TARGET).elf
endif

burn-stflash :
	@echo "Usage: make burn"
ifeq ($(OS),Cygwin)
	$(IW_HOME)/tools/stlink/bin/st-flash.exe write $(TARGET).bin 0x8000000
else ifeq ($(OS),Linux)
	sudo $(IW_HOME)/tools/stlink/bin/st-flash write $(TARGET).bin 0x8000000
else ifeq ($(OS),Mingw)
	$(IW_HOME)/tools/stlink/bin/st-flash.exe write $(TARGET).bin 0x8000000
endif
