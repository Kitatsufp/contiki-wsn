DEFINES+=PROJECT_CONF_H=\"project-conf.h\"

CONTIKI_PROJECT = test

CONTIKI_TARGET_SOURCEFILES += tsl256x.c bmpx8x.c si7021.c

MODULES += platform/cc2538dk/dev

all: $(CONTIKI_PROJECT)

CONTIKI = ../../..
CONTIKI_WITH_RIME = 1
include $(CONTIKI)/Makefile.include
