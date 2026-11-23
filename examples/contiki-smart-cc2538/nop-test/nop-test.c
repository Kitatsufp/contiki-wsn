/**
 * \addtogroup cc2538-platforms
 * @{
 *
 * \defgroup cc2538-examples cc2538 Example Projects
 * @{
 *
 * \defgroup cc2538-demo cc2538dk Demo Project
 *
 *   Example project demonstrating the cc2538dk functionality
 *
 *   This assumes that you are using a SmartRF06EB with a cc2538 EM
 *
 * - Boot sequence: LEDs flashing, LED2 followed by LED3 then LED4
 * - etimer/clock : Every LOOP_INTERVAL clock ticks the LED defined as
 *                  LEDS_PERIODIC will turn on
 * - rtimer       : Exactly LEDS_OFF_HYSTERISIS rtimer ticks later,
 *                  LEDS_PERIODIC will turn back off
 * - Buttons      :
 *                - BTN_DOWN turns on LEDS_REBOOT and causes a watchdog reboot
 *                - BTN_UP to soft reset (SYS_CTRL_PWRDBG::FORCE_WARM_RESET)
 *                - BTN_LEFT and BTN_RIGHT flash the LED defined as LEDS_BUTTON
 * - ADC sensors  : On-chip VDD / 3 and temperature, and ambient light sensor
 *                  values are printed over UART periodically.
 * - UART         : Every LOOP_INTERVAL the EM will print something over the
 *                  UART. Receiving an entire line of text over UART (ending
 *                  in \\r) will cause LEDS_SERIAL_IN to toggle
 * - Radio comms  : BTN_SELECT sends a rime broadcast. Reception of a rime
 *                  packet will toggle LEDs defined as LEDS_RF_RX
 *
 * @{
 *
 * \file
 *     Example demonstrating the cc2538dk platform
 */
#include "contiki.h"
#include "cpu.h"
#include "sys/etimer.h"
#include "sys/rtimer.h"
#include "dev/leds.h"
#include "dev/uart.h"
#include "dev/cc2538-sensors.h"
#include "dev/button-sensor.h"
#include "dev/als-sensor.h"
#include "dev/watchdog.h"
#include "dev/serial-line.h"
#include "dev/sys-ctrl.h"
#include "net/rime/broadcast.h"

#include <stdio.h>
#include <stdint.h>
/*---------------------------------------------------------------------------*/
#define LOOP_INTERVAL       CLOCK_SECOND
#define LEDS_OFF_HYSTERISIS (RTIMER_SECOND >> 1)
#define LEDS_PERIODIC       LEDS_YELLOW
#define LEDS_BUTTON         LEDS_RED
#define LEDS_SERIAL_IN      LEDS_ORANGE
#define LEDS_REBOOT         LEDS_ALL
#define LEDS_RF_RX          (LEDS_YELLOW | LEDS_ORANGE)
#define BROADCAST_CHANNEL   129
/*---------------------------------------------------------------------------*/
static struct etimer et;
static struct rtimer rt;
static uint16_t counter;
static uint16_t num;
static uint16_t i,j;
/*---------------------------------------------------------------------------*/
PROCESS(cc2538_demo_process, "cc2538 demo process");
AUTOSTART_PROCESSES(&cc2538_demo_process);
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(cc2538_demo_process, ev, data)
{

  PROCESS_BEGIN();

  
  counter = 0;
  

  GPIO_SET_OUTPUT(GPIO_D_BASE,1 << 2);
  

  etimer_set(&et, CLOCK_SECOND);

  while(1) {

    PROCESS_YIELD();

    if(ev == PROCESS_EVENT_TIMER) {
      leds_on(LEDS_PERIODIC);
      printf("-----------------------------------------\n"
             "Counter = 0x%08x\n", counter);

      etimer_set(&et, 3*CLOCK_SECOND);
      counter++;
    } else if(ev == sensors_event) {
      if(data == &button_select_sensor) {
         for(num=0;num<10;num++){
           GPIO_SET_PIN(GPIO_D_BASE,1 << 2);
	   for(i=0;i<10;i++){ 	
		asm("nop");
	    }
           GPIO_CLR_PIN(GPIO_D_BASE,1 << 2);
          }
	printf("10xduty cycle\n");


      } else if(data == &button_left_sensor || data == &button_right_sensor) {
        leds_toggle(LEDS_BUTTON);
        GPIO_CLR_PIN(GPIO_D_BASE,1 << 2);
	printf("L/R button pressed\n");
      } else if(data == &button_down_sensor) {  // PD5 SW2
        leds_toggle(LEDS_RED);
	printf("down button pressed\n");

         for(num=0;num<100;num++){
           GPIO_SET_PIN(GPIO_D_BASE,1 << 2);
	   for(i=0;i<2;i++){ 	
		asm("nop");
	    }
           GPIO_CLR_PIN(GPIO_D_BASE,1 << 2);
          }


      } else if(data == &button_up_sensor) {	// PD4 SW1
         for(num=0;num<10;num++){
           GPIO_SET_PIN(GPIO_D_BASE,1 << 2);
	   for(i=0;i<100;i++){ 	
		asm("nop");
	    }
           GPIO_CLR_PIN(GPIO_D_BASE,1 << 2);
          }
	printf("100xduty cycle\n");
        leds_toggle(LEDS_GREEN);
	printf("up button pressed\n");
      }
    } else if(ev == serial_line_event_message) {
//      leds_toggle(LEDS_SERIAL_IN);
    }
  }

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
/**
 * @}
 * @}
 * @}
 */
