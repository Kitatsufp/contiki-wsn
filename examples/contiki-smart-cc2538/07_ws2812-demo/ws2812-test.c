
/**
 * \file
 *         A very simple Contiki application showing how Contiki programs look
 * \author
 *         Adam Dunkels <adam@sics.se>
 */

#include "contiki.h"
#include "cfs/cfs.h"
#include "cpu.h"

#include "sys/etimer.h"
#include "ws2812.h"
#include "dev/cc2538-sensors.h"
#include "dev/button-sensor.h"
#include "dev/sys-ctrl.h"

#include <stdio.h> /* For printf() */



static struct etimer et;
static uint32_t	sys_clk_freq;
static uint32_t	sys_io_clk_freq;


/*---------------------------------------------------------------------------*/
PROCESS(hello_world_process, "Hello world process");
AUTOSTART_PROCESSES(&hello_world_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(hello_world_process, ev, data)
{


  	PROCESS_BEGIN();
	etimer_set(&et, CLOCK_SECOND);

	sys_clk_freq = sys_ctrl_get_sys_clock();
	sys_io_clk_freq = sys_ctrl_get_io_clock();
	
	printf("Freq: sys %lu Hz, io %lu Hz\n",sys_clk_freq, sys_io_clk_freq);

	ws2812_init();
	
   while(1){
	
//	PROCESS_YIELD();
//	if(ev == PROCESS_EVENT_TIMER){
//	printf("ws2812 demo routing\n");
//	printf("");
	
	PROCESS_WAIT_EVENT();
	if(ev == sensors_event){
	  printf("ws2812 init\n");
	  if(data == &button_up_sensor){
		printf("up button pressed. \n");
		ws2812_reset();	 
		ws2812_write(0xFF);
		ws2812_write(0x00);
		ws2812_write(0x00);
		ws2812_write(0xFF);
		ws2812_write(0x00);
		ws2812_write(0x00);
	  }else if(data == &button_down_sensor){
		printf("down button pressed. \n");
		ws2812_reset();	 
		ws2812_write(0x00);
                ws2812_write(0xFF);
                ws2812_write(0x00);
		ws2812_write(0x00);
                ws2812_write(0xFF);
                ws2812_write(0x00);
	  }else if(data == &button_left_sensor){
		printf("down button pressed. \n");
		ws2812_reset();	 
		ws2812_write(0x00);
                ws2812_write(0x00);
                ws2812_write(0xFF);
		ws2812_write(0x00);
                ws2812_write(0x00);
                ws2812_write(0xFF);
	  }else if(data == &button_right_sensor){
		printf("down button pressed. \n");
		ws2812_reset();	 
		ws2812_write(0x00);
                ws2812_write(0x00);
                ws2812_write(0x00);
		ws2812_write(0xFF);
                ws2812_write(0xFF);
                ws2812_write(0xFF);
	   
	  }else if(data == &button_select_sensor){
		printf("left button pressed. \n");
		ws2812_reset();	 
	   }

	}else if(ev == PROCESS_EVENT_TIMER){
//	 printf("etimer expired\n");
	 etimer_set(&et, CLOCK_SECOND);
	}
      
    }

  
  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
