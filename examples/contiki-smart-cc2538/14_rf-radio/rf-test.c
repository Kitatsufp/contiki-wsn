/*
 * Copyright (c) .
 */
#include "contiki.h"
#include "cpu.h"
#include "sys/etimer.h"
#include "sys/rtimer.h"
#include "dev/leds.h"
#include "dev/uart.h"
#include "dev/button-sensor.h"
#include "dev/watchdog.h"
#include "dev/serial-line.h"
#include "dev/sys-ctrl.h"
#include "dev/cc2538-rf.h"
//#include "dev/console.h"
#include "rf-packet.h"
#include "dev/i2c.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>


#define DEBUG 1
#if DEBUG
#define PRINTF(...) printf(__VA_ARGS__)
#else
#define PRINTF(...)
#endif
/*-------------------------------------------------------------*/
#define LOOP_INTERVAL       CLOCK_SECOND
#define LEDS_OFF_HYSTERISIS (RTIMER_SECOND >> 1)
#define LEDS_PERIODIC       LEDS_RED
#define LEDS_KEY1	    	LEDS_RED
#define LEDS_KEY2	    	LEDS_BLUE
#define LEDS_RF_RX          (LEDS_RED | LEDS_BLUE)
#define LED_INIT_ERROR      -4
#define BUTTON_INIT_ERROR   -8
#define RATE	1 
/*-------------------------------------------------------------*/
#define PAN_ID		0x2007  //  PAN_ID号
#define PAN_L		(PAN_ID & 0x00FF)       //  PAN_ID低8位
#define PAN_H		((PAN_ID & 0xFF00) >> 8)        //  PAN_ID高8位
#define ADD_SRC		0xAABB
#define ADD_SRC_L	(ADD_SRC & 0x00FF)
#define ADD_SRC_H	((ADD_SRC & 0xFF00) >> 8)
#define ADD_DEST	0xBEEF
#define ADD_DEST_L	(ADD_DEST & 0x00FF)
#define ADD_DEST_H  ((ADD_DEST & 0xFF00) >> 8)
/*------------------------------------------------------------ */

static struct rtimer rt;
static uint8_t num;
struct RF rf;

static uint8_t txbuf[23] = { 0x61, 0x88, 0, PAN_L, PAN_H,
  ADD_DEST_L, ADD_DEST_H, ADD_SRC_L, ADD_SRC_H,
  'h', 'e', 'l', 'l', 'o', ' ', '8', '0', '2', '.', '1', '5', '.', '4'
};                              // 定义一帧数据
/*------------------------------------------------------------*/
PROCESS(cc2538rf_demo_process, "cc2538rf demo process");
AUTOSTART_PROCESSES(&cc2538rf_demo_process);
void
rt_callback(struct rtimer *t, void *ptr)
{
  txbuf[2] = num++;
  cc2538_rf_driver.send(txbuf, sizeof(txbuf));
//  go_to_x_y(1, 6);
  printf("send:    %6d packets\n", ++rf.cur_num);
  leds_toggle(LEDS_KEY1);
  if(rtimer_set
     (&rt, RTIMER_NOW() + RTIMER_SECOND / RATE, 1, rt_callback,
      NULL) != RTIMER_OK) {
    printf("rtimer set error\n");
  }
}
/*------------------------------------------------------------*/
PROCESS_THREAD(cc2538rf_demo_process, ev, data)
{
  PROCESS_BEGIN();

  radio_value_t value;

  printf("Rf test start \n");
  if(cc2538_rf_driver.set_value(RADIO_PARAM_TXPOWER, 22) != RADIO_RESULT_OK)
    printf("set TXPOWER error\n");
  if(cc2538_rf_driver.get_value(RADIO_PARAM_TXPOWER, &value) ==
     RADIO_RESULT_OK)
    printf("rf power:  %d dbm     ", value);
  else
    printf("get power error\n");
  if(cc2538_rf_driver.get_value(RADIO_PARAM_CHANNEL, &value) ==
     RADIO_RESULT_OK)
    printf("channel:  %d\n", value);
  else
    printf("get channel error\n");
  if(cc2538_rf_driver.get_value(RADIO_PARAM_PAN_ID, &value) ==
     RADIO_RESULT_OK)
    printf("PAN ID:  %X     ", value);
  else
    printf("get PAN ID error\n");
  if(cc2538_rf_driver.get_value(RADIO_PARAM_16BIT_ADDR, &value) ==
     RADIO_RESULT_OK)
    printf("ADDRESS:  %X\n", value);
  else
    printf("get ADDRESS error\n");

  rf.mode = UNDEFINE;
  
  printf("choice mode. \nPress KEY1:\tsender\nPress KEY2:\treceiver\n");
  PROCESS_YIELD();
  
  if(ev == sensors_event) {
    if(data == &button_up_sensor) {
      printf("sending mode\n");
      rf.mode = SENDER;
    } else if(data == &button_down_sensor) {
      printf("receiving mode\n");
      rf.mode = RECEIVER;
    }
    if(data == &button_left_sensor) {
      printf("button left pressed\n");
     
     leds_toggle(LEDS_RED);
    } else if(data == &button_right_sensor) {
      printf("button right pressed\n");
     leds_toggle(LEDS_RED);
    
    } else if(data == &button_select_sensor) {
      printf("button select pressed\n");
     leds_toggle(LEDS_RED);
    }
  }
  if(rf.mode == SENDER) {
    if(rtimer_set
       (&rt, RTIMER_NOW() + RTIMER_SECOND / RATE, 1, rt_callback,
        NULL) != RTIMER_OK) {
      printf("rtimer set error\n");
    }
    num = 0;

    while(1) {
      PROCESS_YIELD();
      if(ev == PROCESS_EVENT_TIMER) {
      }
    }
  } else if(rf.mode == RECEIVER) {
	  value = ADD_DEST;
	  if(cc2538_rf_driver.set_value(RADIO_PARAM_16BIT_ADDR, value) ==
     RADIO_RESULT_OK){
     printf("Set ADDRESS:  %X", value);
	 }
          else {
    	  printf("Set ADDRESS error\n");
  	  }
  }
  PROCESS_END();
}

/*-------------------------------------------------------------*/
/**
 * @}
 * @}
 * @}
 */
