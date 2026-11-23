/*
 cc2538 uart example 

*/

#include "contiki.h"
#include "sys/etimer.h"
#include "sys/rtimer.h"
#include "dev/leds.h"
#include "dev/uart.h"
#include "dev/uart1.h"
#include "lib/ringbuf.h"

#include<stdio.h>
#include<stdint.h>


#define LOOP_INTERVAL		CLOCK_SECOND
#define LEDS_OFF_HYSTERISIS	(RTIMER_SECOND >> 1)
#define LEDS_PERIODIC		LEDS_YELLOW

//#undef UART1_CONF_UART
#define UART1_CONF_UART	1	/* contiki-conf.h definition */

/*------------------------------------------------------------
 * UART1_RX:	PA4		
 * UART1_TX:	PA5		
 *------------------------------------------------------------*/


#define IGNORE_CHAR(c) (c == 0x0d)
#define END 0x0a

static	struct	etimer	et;
static	struct	rtimer	rt;
static	uint16_t	counter;
static  struct  ringbuf rxbuf;

int my_uart1_input_byte(unsigned char c);

PROCESS(cc2538_uart_process, "cc2538 uart process");
AUTOSTART_PROCESSES(&cc2538_uart_process);

unsigned int
uart0_send_bytes(const	unsigned  char *s, unsigned int len)
{

  unsigned int i = 0;
  while(s && *s != 0){
	if(i >= len){
	  break;
	}
	uart_write_byte(0, *s++);
	i++;
   }
//   printf("uart0 send \n");
   return i;

}

unsigned int
uart1_send_bytes(const	unsigned  char *s, unsigned int len)
{

  unsigned int i = 0;
  while(s && *s != 0){
	if(i >= len){
	  break;
	}
	uart_write_byte(1, *s++);
	i++;
   }
//   printf("uart1 send \n");
   return i;
}

/*---------------------------------------------------------------------------*/
int
my_uart1_input_byte(unsigned char c)
{
  static uint8_t overflow = 0 ;

  printf("uart1 routine start..\n");
  printf("input char %c\n",c);

  if(IGNORE_CHAR(c)) {
      return 0;
    }
  
//    if(!overflow) {
//      /* Add character */
//      if(ringbuf_put(&rxbuf, c) == 0) {
//        /* Buffer overflow: ignore the rest of the line */
//        overflow = 1;
//      }
//      printf("input char %c\n",c);
//    } else {
//      /* Buffer overflowed:
//       * Only (try to) add terminator characters, otherwise skip */
//      if(c == END && ringbuf_put(&rxbuf, c) != 0) {
//        overflow = 0;
//      }
//    }

    /* Wake up consumer process */
//  process_poll(&serial_line_process);
  return 1;



}


void rt_callback(struct rtimer *t, void *ptr)
{
  leds_off(LEDS_PERIODIC);
}

PROCESS_THREAD(cc2538_uart_process, ev, data)
{
  char	string0[25];
  char  string1[25];

  PROCESS_BEGIN();
  counter = 0;

  uart_init(1); 
  uart_set_input(1,my_uart1_input_byte);
//  uart1_set_input(my_uart1_input_byte);
  
  etimer_set(&et, 2*CLOCK_SECOND);
 	//printf('========= PROCESS =========');        

  while(1){
    PROCESS_YIELD();


    if(ev == PROCESS_EVENT_TIMER) {
	leds_on(LEDS_PERIODIC);
	sprintf(string0, "Test 0x%02x to UART0 \n", counter);
	sprintf(string1, "Test 0x%02x to UART1 \n", counter);

	uart0_send_bytes((uint8_t *)string0, sizeof(string0)-1);
	uart1_send_bytes((uint8_t *)string1, sizeof(string1)-1);

	etimer_set(&et, CLOCK_SECOND);
	rtimer_set(&rt, RTIMER_NOW() + LEDS_OFF_HYSTERISIS, 1, rt_callback, NULL);

	counter ++;
	}
      }
   PROCESS_END();
}


