/*
 * Copyright (c) 2006, Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * This file is part of the Contiki operating system.
 *
 */

/**
 * \file
 *         A very simple Contiki application showing how Contiki programs look
 * \author
 *         Adam Dunkels <adam@sics.se>
 */

#include "contiki.h"
#include "lcd_dogm128_6.h"
#include <stdio.h> /* For printf() */
#include "sys/etimer.h"
#include "dev/cc2538-sensors.h"
#include "dev/button-sensor.h"
#include "dev/leds.h"


static struct etimer et;
/*---------------------------------------------------------------------------*/
PROCESS(hello_world_process, "Hello world process");
AUTOSTART_PROCESSES(&hello_world_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(hello_world_process, ev, data)
{
  PROCESS_BEGIN();

   printf("Hello, world\n");
   etimer_set(&et,10*CLOCK_SECOND);

   while(1){
    
    PROCESS_YIELD();
     if(ev == PROCESS_EVENT_TIMER) {
	leds_toggle(LEDS_RED);
      
	   lcdInit();
	   printf("lcd init \n");
	   lcdBufferClear(0);
	   printf("lcd buffer clear \n");
	
	//    lcdBufferPrintString(0, "Default Buffer", 1, eLcdPage0);
	//    lcdBufferInvertPage(0, 0, 127, eLcdPage0);
	//    lcdBufferPrintStringAligned(0, "Left aligned", eLcdAlignLeft, eLcdPage2);
	//    lcdBufferPrintStringAligned(0, "Center aligned", eLcdAlignCenter,
	//                                eLcdPage3);
	//    lcdBufferPrintStringAligned(0, "Right aligned", eLcdAlignRight, eLcdPage4);
	//    lcdBufferPrintStringAligned(0, "Press LEFT", eLcdAlignCenter, eLcdPage7);
	
	//   lcdSendBuffer(0);
	    
            etimer_set(&et,10*CLOCK_SECOND);
	}else if(ev == sensors_event){
	   if(data == &button_select_sensor){
	       printf("button pressed \n");
		}
           }

	}

  
  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
