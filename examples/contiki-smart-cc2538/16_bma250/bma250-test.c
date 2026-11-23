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
#include "cfs/cfs.h"

#include "spi-arch.h"
#include "spi.h"
#include "dev/ssi.h"
#include "spi.h"
#include "sys/etimer.h"

#include <stdio.h> /* For printf() */


#define SPI_SEL_PORT_BASE 	GPIO_PORT_TO_BASE(SPI_SEL_PORT)  //CS RF2.10 PD5
#define SPI_SEL_PIN_MASK 	GPIO_PIN_MASK(SPI_SEL_PIN)
#define ACC_PWR_PORT_BASE 	GPIO_PORT_TO_BASE(ACC_PWR_PORT)  //CS RF2.8  PD4
#define ACC_PWR_PIN_MASK 	GPIO_PIN_MASK(ACC_PWR_PIN)  

static struct etimer et;

/*---------------------------------------------------------------------------*/
PROCESS(hello_world_process, "Hello world process");
AUTOSTART_PROCESSES(&hello_world_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(hello_world_process, ev, data)
{

  int8_t d[6]={0};
  int16_t x,y,z;

  	PROCESS_BEGIN();
	etimer_set(&et, CLOCK_SECOND);

	
   while(1){
	PROCESS_YIELD();
	if(ev == PROCESS_EVENT_TIMER){
	printf("spi init\n");
	spi_init();
	GPIO_SET_OUTPUT(SPI_SEL_PORT_BASE,SPI_SEL_PIN_MASK);
	GPIO_SET_OUTPUT(ACC_PWR_PORT_BASE,ACC_PWR_PIN_MASK);
	GPIO_CLR_PIN(ACC_PWR_PORT_BASE,ACC_PWR_PIN_MASK);
	GPIO_SET_PIN(SPI_SEL_PORT_BASE,SPI_SEL_PIN_MASK);
	clock_delay_usec(1000);
	GPIO_CLR_PIN(SPI_SEL_PORT_BASE,SPI_SEL_PIN_MASK);
	GPIO_SET_PIN(ACC_PWR_PORT_BASE,ACC_PWR_PIN_MASK);
	clock_delay_usec(1000);
	
//	spix_cs_init(SPI_SEL_PORT,SPI_SEL_PIN); // that one to activate SPI
//	spix_cs_init(ACC_PWR_PORT,ACC_PWR_PIN); // that one to power on the acc
	printf("spi cs enable\n");
	
	clock_delay_usec(100);

	// On doit mettre à la masse le SS pour indiquer le début de la
	//communication avec l'accéléromètre
	//GPIO_CLR_PIN(SPI_SEL_PORT_BASE,SPI_SEL_PIN_MASK);
	// 0x80 : lecture, 0x02 : adresse du registre
	SPI_WRITE(0x80|0x02);
	SPI_FLUSH();
	SPI_READ(d[0]);
	SPI_READ(d[1]);
	SPI_READ(d[2]);
	SPI_READ(d[3]);
	SPI_READ(d[4]);
	SPI_READ(d[5]);
	printf("read finished\n");
	// On indique la fin de la communication en mettant à 1 le SS
	GPIO_SET_PIN(SPI_SEL_PORT_BASE,SPI_SEL_PIN_MASK);
	
	x = (((int16_t)(d[1]) << 2) | ((d[0] >> 6) & 0x03));
	y = (((int16_t)(d[3]) << 2) | ((d[2] >> 6) & 0x03));
	z = (((int16_t)(d[5]) << 2) | ((d[4] >> 6) & 0x03));
	
	printf("x-axis : %d\ny-axis : %d\nz-axis : %d\n",x,y,z);
	clock_delay_usec(100);

	 etimer_set(&et, CLOCK_SECOND);
	}
}	


  
  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
