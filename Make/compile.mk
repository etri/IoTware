#
#
#

MD	:= mkdir -p
CP	:= cp

ifeq ($(IW_DEBUG),y)
STRIP   := ls
IW_CFLAGS += -DLOG_LEVEL=3
else
STRIP   := $(IW_CROSS)strip
endif

ifeq ($(IW_VERBOSE),y)
Q_ :=
else
Q_ := @
endif

ifeq ($(OS),Mingw)
PWD := $(shell npwd)
endif

OUTNME	:= $(basename $(IW_MODULE))
OUTEXT  := $(subst $(OUTNME),,$(IW_MODULE))

#MAKELEVEL=0
ifeq ($(IW_PWD),)
ifeq ($(MAKELEVEL),0)
IW_PWD := $(PWD)
export IW_PWD
endif
ifeq ($(MAKELEVEL),1)
IW_PWD := $(IW_HOME)
export IW_PWD
endif
endif

OBJDIR  := $(IW_APP_HOME)/$(IW_OUT)/obj
BINDIR  := $(IW_APP_HOME)/$(IW_OUT)/bin
LIBDIR  := $(IW_APP_HOME)/$(IW_OUT)/lib
TASKDIR := $(IW_APP_HOME)/$(IW_OUT)/Task

ifeq ($(OUTEXT),.so)
OUTTRG  := $(LIBDIR)/$(IW_MODULE)
else
ifeq ($(OUTEXT),.a)
OUTTRG  := $(LIBDIR)/$(IW_MODULE)
else
ifeq ($(OUTEXT),.elf)
OUTTRG  := $(BINDIR)/$(IW_MODULE)
OUTBIN  := $(OUTTRG:%.elf=%.bin)
OUTLST  := $(OUTTRG:%.elf=%.lst)
OUTHEX  := $(OUTTRG:%.elf=%.hex)
OUTMAP  := $(OUTTRG:%.elf=%.map)
else
OUTTRG  := $(BINDIR)/$(IW_MODULE)
endif
endif
endif

ifeq ($(OUTEXT),.so)
IW_CFLAGS += -fPIC
endif
IW_CFLAGS += $(addprefix -D,$(IW_DEFS))
IW_CFLAGS += $(addprefix -I,$(IW_INCS))
IW_CFLAGS := $(strip $(IW_CFLAGS))

ifeq ($(OUTEXT),.so)
IW_LDFLAGS += -shared 
endif
IW_LDFLAGS += -Wl,-Map=$(OUTMAP)
IW_LDFLAGS := $(strip $(IW_LDFLAGS))
IW_LDLIBS  := $(strip $(IW_LDLIBS))

ifeq ($(IW_VERBOSE),y)
$(info IW_CFLAGS  = $(IW_CFLAGS) )
$(info IW_LDFLAGS = $(IW_LDFLAGS))
$(info IW_LDLIBS  = $(IW_LDLIBS) )
endif

OBJS := $(addprefix $(OBJDIR)/,$(IW_SRCS))
OBJS := $(addsuffix .o,$(basename $(OBJS)))
DEPS := $(addsuffix .d,$(basename $(OBJS)))

ifeq ($(IW_MK_PATH),)
HEADER := $(IW_PWD)/
else
HEADER :=
endif

ifeq ($(IW_MAKEFILE),)
export IW_MAKEFILE := $(HEADER)$(firstword $(MAKEFILE_LIST))
endif

all:
	$(Q_)make -f $(IW_MAKEFILE) -C $(IW_HOME) build-all

ifeq ($(OUTEXT),.elf)
build-all: $(OUTLST) $(OUTHEX) $(OUTBIN)	
# ifeq ($(IW_EMULATOR),1)
# 	$(Q_)$(CP) $(OUTBIN) $(IW_HOME)/../tools/emulator/emu.bin
# 	$(Q_)$(CP) $(OUTTRG) $(IW_HOME)/../tools/emulator/emu.elf
# endif
else
build-all: $(OUTTRG)
endif
	@echo ""

$(OBJDIR)/%.d $(OBJDIR)/%.o: %.s
ifeq ($(IW_VERBOSE),y)
	@echo ""
endif
	@echo "Compile(ASM)....($(IW_MODULE)) <-- $<"
	@if [ ! -d "$(patsubst %/,%,$(dir $@))" ]; then \
		$(MD) "$(patsubst %/,%,$(dir $@))"; \
	fi
	$(Q_)$(AS) $(IW_CFLAGS) -MMD -MF"$(@:%.o=%.d)" -o "$(@:%.d=%.o)" -c $<

$(OBJDIR)/%.d $(OBJDIR)/%.o: %.S
ifeq ($(IW_VERBOSE),y)
	@echo ""
endif
	@echo "Compile(ASM)....($(IW_MODULE)) <-- $<"
	@if [ ! -d "$(patsubst %/,%,$(dir $@))" ]; then \
		$(MD) "$(patsubst %/,%,$(dir $@))"; \
	fi
	$(Q_)$(AS) $(IW_CFLAGS) -MMD -MF"$(@:%.o=%.d)" -o "$(@:%.d=%.o)" -c $<

$(OBJDIR)/%.d $(OBJDIR)/%.o: %.c
ifeq ($(IW_VERBOSE),y)
	@echo ""
endif
	@echo "Compile(C99)....($(IW_MODULE)) <-- $<"
	@if [ ! -d "$(patsubst %/,%,$(dir $@))" ]; then \
		$(MD) "$(patsubst %/,%,$(dir $@))"; \
	fi
	$(Q_)$(CC) $(IW_CFLAGS) -MMD -MF"$(@:%.o=%.d)" -o "$(@:%.d=%.o)" -c $<

$(OBJDIR)/%.d $(OBJDIR)/%.o: %.C
ifeq ($(IW_VERBOSE),y)
	@echo ""
endif
	@echo "Compile(C++)....($(IW_MODULE)) <-- $<"
	@if [ ! -d "$(patsubst %/,%,$(dir $@))" ]; then \
		$(MD) "$(patsubst %/,%,$(dir $@))"; \
	fi
	$(Q_)$(CXX) $(IW_CFLAGS) -MMD -MF"$(@:%.o=%.d)" -o "$(@:%.d=%.o)" -c $<

$(OBJDIR)/%.d $(OBJDIR)/%.o: %.cpp
ifeq ($(IW_VERBOSE),y)
	@echo ""
endif
	@echo "Compile(C++)....($(IW_MODULE)) <-- $<"
	@if [ ! -d "$(patsubst %/,%,$(dir $@))" ]; then \
		$(MD) "$(patsubst %/,%,$(dir $@))"; \
	fi
	$(Q_)$(CXX) $(IW_CFLAGS) -MMD -MF"$(@:%.o=%.d)" -o "$(@:%.d=%.o)" -c $<

$(OBJDIR)/%.d $(OBJDIR)/%.o: %.cxx
ifeq ($(IW_VERBOSE),y)
	@echo ""
endif
	@echo "Compile(C++)....($(IW_MODULE)) <-- $<"
	@if [ ! -d "$(patsubst %/,%,$(dir $@))" ]; then \
		$(MD) "$(patsubst %/,%,$(dir $@))"; \
	fi
	$(Q_)$(CXX) $(IW_CFLAGS) -MMD -MF"$(@:%.o=%.d)" -o "$(@:%.d=%.o)" -c $<

$(OBJDIR)/%.d $(OBJDIR)/%.o: %.cc
ifeq ($(IW_VERBOSE),y)
	@echo ""
endif
	@echo "Compile(C++)....($(IW_MODULE)) <-- $<"
	@if [ ! -d "$(patsubst %/,%,$(dir $@))" ]; then \
		$(MD) "$(patsubst %/,%,$(dir $@))"; \
	fi
	$(Q_)$(CXX) $(IW_CFLAGS) -MMD -MF"$(@:%.o=%.d)" -o "$(@:%.d=%.o)" -c $<

$(OBJDIR): FORCE
	@if [ ! -d "$(OBJDIR)" ]; then \
		$(MD) $(OBJDIR); \
	fi

$(OUTTRG): $(OBJDIR) $(OBJS)
ifeq ($(IW_VERBOSE),y)
	@echo ""
endif
	@echo "Link...($(IW_MODULE))"
	@echo " --> $@"
ifeq ($(OUTEXT),.so)
	$(Q_)$(MD) $(LIBDIR)
	$(Q_)$(CXX) $(IW_LDFLAGS) $(OBJS) $(IW_LDLIBS) -o $@
else
ifeq ($(OUTEXT),.a)
	$(Q_)$(MD) $(LIBDIR)
	$(Q_)$(AR) rc $@ $(OBJS)
else
ifeq ($(OUTEXT),.elf)
	$(Q_)$(MD) $(BINDIR)
	$(Q_)$(LD) $(IW_LDFLAGS) $(OBJS) $(IW_LDLIBS) -o $@
else
	$(Q_)$(MD) $(BINDIR)
	$(Q_)$(CXX) $(IW_LDFLAGS) $(OBJS) $(IW_LDLIBS) -o $@
endif
endif
endif

$(OUTLST): $(OUTTRG)
	@echo "gen-lst...($@)"
	$(Q_)$(OBJDUMP) -h -S $< > $@
 
$(OUTHEX): $(OUTTRG)
	@echo "gen-hex...($@)"
	$(Q_)$(OBJCOPY) -O ihex $< $@

$(OUTBIN): $(OUTTRG)
	@echo "gen-bin...($@)"
	$(Q_)$(OBJCOPY) -O binary $< $@
 
.PHONY: clean
clean: FORCE
	$(Q_)rm -rf $(OBJDIR);
	$(Q_)rm -rf $(BINDIR);
	$(Q_)rm -rf $(TASKDIR);
	$(Q_)rm -rf $(IW_APP_HOME)/$(IW_OUT);
	
.PHONY: FORCE
FORCE:

check_env:
	@echo "IW_SRCS = $(IW_SRCS)"
	@echo "IW_INCS = $(IW_INCS)"
	@echo "OBJS = $(OBJS)"
	@echo "OBJDIR = $(OBJDIR)"
	@echo "IW_PWD = $(IW_PWD)"
	@echo "IW_MAKEFILE = $(IW_MAKEFILE)"
	@echo "OS = $(OS)"
	@echo "PWD = $(PWD)"

ifeq ($(filter %clean,$(MAKECMDGOALS)),)
ifeq ($(MAKELEVEL),2)
-include $(DEPS)
endif
endif

