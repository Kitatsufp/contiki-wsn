#CFLAGS=-g -Wall -W -I../../..
CFLAGS=-g -Wall -W 

aes_cli: aes_cli.o net.o noerr.o aes.o util.o

#aes.o:
aes.o: aes.c
	$(COMPILE.c) $(OUTPUT_OPTION) $<

#util.o:
util.o: util.c
	$(COMPILE.c) $(OUTPUT_OPTION) $<

#net.o: ../net.c
net.o: net.c
	$(COMPILE.c) $(OUTPUT_OPTION) $<

#noerr.o: ../../noerr/noerr.c
noerr.o: noerr.c
	$(COMPILE.c) $(OUTPUT_OPTION) $<


