CFLAGS=-g -Wall -W 

test-net-client: test-net-client.o net.o noerr.o


net.o: net.c
	$(COMPILE.c) $(OUTPUT_OPTION) $<

noerr.o: noerr.c
	$(COMPILE.c) $(OUTPUT_OPTION) $<
