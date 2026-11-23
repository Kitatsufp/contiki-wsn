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
#include "dev/leds.h"
#include "sys/clock.h"
#include "dev/gpio.h"
//#include "cfs/cfs.h"

#include <stdio.h> /* For printf() */


static uint8_t port_num, pin_num;
/*---------------------------------------------------------------------------*/
PROCESS(hello_world_process, "Hello world process");
AUTOSTART_PROCESSES(&hello_world_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(hello_world_process, ev, data)
{
  PROCESS_BEGIN();

  port_num = 0; 
  printf("Toggle all pin status with dutycycle=2us\n");

  while(1){

//  for(port_num=0; port_num<=1; port_num ++)
 //  {
	for(pin_num=0; pin_num<=7; pin_num++){ 
	   GPIO_SET_OUTPUT(GPIO_PORT_TO_BASE(port_num),GPIO_PIN_MASK(pin_num));
	 }
	   clock_delay_usec(1);
	for(pin_num=0; pin_num<=7; pin_num++) {
	   GPIO_CLR_PIN(GPIO_PORT_TO_BASE(port_num),GPIO_PIN_MASK(pin_num));
	 }
	   clock_delay_usec(10);
	for(pin_num=0; pin_num<=7; pin_num++) {
	   GPIO_SET_PIN(GPIO_PORT_TO_BASE(port_num),GPIO_PIN_MASK(pin_num));
	 }
	   //leds_toggle(LEDS_RED); PC0 not defined
	   leds_toggle(LEDS_GREEN);
     }
	   leds_toggle(LEDS_YELLOW);
  //  } 
  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
