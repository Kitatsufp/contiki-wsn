
PROJECT_SOURCEFILES += util.c aes_lib.c

CONTIKI_PROJECT = udp-echo-server

CFLAGS += -DPROJECT_CONF_H=\"project-conf.h\"

MODULES +=  core/net/mac core/net core/net/mac/sicslowmac core/net/mac/contikimac core/net/llsec/noncoresec core/net/mac/tsch


#ifdef CC2538DK_HAS_SHIELD
#CONTIKI_TARGET_SOURCEFILES += tsl256x.c bmpx8x.c si7021.c
#MODULES += platform/cc2538dk/dev
#endif


ifdef WITH_COMPOWER
APPS+=powertrace
CFLAGS+= -DCONTIKIMAC_CONF_COMPOWER=1 -DWITH_COMPOWER=1 -DQUEUEBUF_CONF_NUM=4
endif

CONTIKI_WITH_IPV6 = 1
MAKE_WITH_ORCHESTRA ?= 0 # force Orchestra from command line
MAKE_WITH_SECURITY ?= 0 # force Security from command line

APPS += orchestra
#MODULES += core/net/mac/tsch

ifeq ($(MAKE_WITH_ORCHESTRA),1)
CFLAGS += -DWITH_ORCHESTRA=1
endif

ifeq ($(MAKE_WITH_SECURITY),1)
CFLAGS += -DWITH_SECURITY=1
endif

all: $(CONTIKI_PROJECT)

CONTIKI = ../../..
CONTIKI_WITH_IPV6 = 1
#CFLAGS += -DUIP_CONF_ND6_SEND_NA=1
include $(CONTIKI)/Makefile.include
