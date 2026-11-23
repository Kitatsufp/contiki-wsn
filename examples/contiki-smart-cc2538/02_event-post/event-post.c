/**
 * \file
 *         Event posting between processes example application.
 * \author
 *         Clément Burin des Roziers <clement.burin-des-roziers@inria.fr>
 */

#include "contiki.h"
#include "dev/leds.h"

#include <stdio.h> /* For printf() */

/* Driver Include */
#include "dev/ds18b20.h"

static uint8_t ch[8];

/* Variables: the application specific event value */
static process_event_t event_data_ready;

/*---------------------------------------------------------------------------*/
/* We declare the two processes */
PROCESS(temp_process, "Temperature process");
PROCESS(print_process, "Print process");

/* We require the processes to be started automatically */
AUTOSTART_PROCESSES(&temp_process, &print_process);
/*---------------------------------------------------------------------------*/
/* Implementation of the first process */
PROCESS_THREAD(temp_process, ev, data)
{
    // variables are declared static to ensure their values are kept
    // between kernel calls.
    static struct etimer timer;
//    static int count = 0;
   // static int average, valid_measure;
    ch[8]='\0';
    
    // those 3 variables are recomputed at every run, therefore it is not
    // necessary to declare them static.
    
    // any process mustt start with this.
    PROCESS_BEGIN();
    
    /* allocate the required event */
    event_data_ready = process_alloc_event();
    
    /* Initialize the temperature sensor */
    
    
    // set the etimer module to generate an event in one second.
    etimer_set(&timer, CLOCK_CONF_SECOND);
    
    while (1)
    {
        // wait here for the timer to expire
        PROCESS_WAIT_EVENT_UNTIL(ev == PROCESS_EVENT_TIMER);
        
        leds_toggle(LEDS_BLUE);
        
        // do the process work
        
	ds18b20_read(ch);	

    	printf("Read value: %s\n",ch);    
            // and pass a pointer to the last measure as data
            process_post(&print_process, event_data_ready, ch);
        
        // reset the timer so it will generate another event
        etimer_reset(&timer);
    }
    // any process must end with this, even if it is never reached.
    PROCESS_END();
}
/*---------------------------------------------------------------------------*/
/* Implementation of the second process */
PROCESS_THREAD(print_process, ev, data)
{
    PROCESS_BEGIN();
    
    while (1)
    {
        // wait until we get a data_ready event
        PROCESS_WAIT_EVENT_UNTIL(ev == event_data_ready);
        
        // display it
        printf("temperature = %s\n", data);
        
    }
    PROCESS_END();
}
/*---------------------------------------------------------------------------*/
