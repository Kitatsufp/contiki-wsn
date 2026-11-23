DEFINES+=PROJECT_CONF_H=\"project-conf.h\"

ifndef TARGET
TARGET=sky
endif

PROJECT_SOURCEFILES += aes.c util.c

CONTIKI_PROJECT = server

all: $(CONTIKI_PROJECT) 

CONTIKI = ../../..
CONTIKI_WITH_IPV6 = 1
# needed for rimestats
CONTIKI_WITH_RIME = 1
include $(CONTIKI)/Makefile.include
