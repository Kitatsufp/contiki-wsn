
/**
 * \file
 * test serial input process and display on lcd screen
 *            --smartbeacon
 * \author
 */

#include "contiki.h"
#include "dev/serial-line.h"
#include "dev/leds.h"
#include "dev/lcd5110.h"

#include <stdio.h> /* For printf() */
/*---------------------------------------------------------------------------*/
PROCESS(test_serial_process, "serial line test process");
AUTOSTART_PROCESSES(&test_serial_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(test_serial_process, ev, data)
{
  PROCESS_BEGIN();
  unsigned int row = 0;

  initLcd();
  
    for(;;) {
	PROCESS_YIELD();
	if(ev == serial_line_event_message) {
	   leds_toggle(LEDS_RED);
  	   printf("received line: %s\n", (char *)data);
	   LCD_write_english_string(0,row,data);
	   row++;
	   if(row >= 6) row =0;

	}
      }
  PROCESS_END();
}
  
/*---------------------------------------------------------------------------*/
