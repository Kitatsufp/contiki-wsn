/*
|-------------------------------------------------------------------|
| HCMC University of Technology                                     |
| Telecommunications Departments                                    |
| Wireless Embedded Firmware for Smart Lighting System (SLS)        |
| Version: 2.0                                                      |
| Author: sonvq@hcmut.edu.vn                                        |
| Date: 01/2019                                                     |
| - HW support in ISM band: TelosB, CC2538, CC2530, CC1310, z1      |
| - Support Sensor shield: TSL256x,BMPX8X, Si7021	                |
|-------------------------------------------------------------------|

Topology description:

		|----------|     IPv6     |-----------|	  IPv4/IPv6		|----------|
		| 6LoWPAN  |<------------>|  Gateway  |<--------------->| Client   |   
		| network  |   wireless	  | + BR + DB |  wired/wireless | software |
		|----------|              |-----------|					|----------|

*/


#include "contiki.h"
#include "contiki-lib.h"
#include "contiki-net.h"

#include "net/ipv6/uip-ds6.h"
#include "net/ip/uip-udp-packet.h"
#include "net/ipv6/uip-ds6-route.h"

//#include <stdlib.h>
//#include <string.h>
//#include "lib/ringbuf.h"

#include "net/ip/uip-debug.h"
#include "dev/leds.h"
#include "net/rpl/rpl.h"
#include "dev/watchdog.h"

#include "random.h"

#include "sys/etimer.h"
#include "sys/ctimer.h"


//#include "sls.h"	


#ifdef SLS_USING_CC2538DK
#include "dev/button-sensor.h"
#include "dev/uart.h"
#include "dev/gpio.h"

#ifdef CC2538DK_HAS_SHIELD
#include "dev/i2c.h"
#include "dev/tsl256x.h"
#include "dev/bmpx8x.h"
#include "dev/si7021.h"
#endif

#endif


/*---------------------------------------------------------------------------*/
#define UIP_IP_BUF   ((struct uip_ip_hdr *)&uip_buf[UIP_LLH_LEN])
#define UIP_UDP_BUF  ((struct uip_udp_hdr *)&uip_buf[uip_l2_l3_hdr_len])


#define MAX_PAYLOAD_LEN 			120
#define READ_SENSOR_PERIOD			5			// seconds


// CC2538DK has shield with sensors
#ifdef CC2538DK_HAS_SHIELD
static uint16_t TSL256X_light;
static uint16_t BMPx8x_pressure;
static int16_t 	BMPx8x_temperature;
static uint16_t Si7021_humidity;
static uint16_t Si7021_temperature;
#endif



/*---------------------------------------------------------------------------*/
//static char buf[MAX_PAYLOAD_LEN];


/* SLS define */
static 	net_struct_t net_db;
static 	env_struct_t env_db;
static 	radio_value_t aux;



/*define timers */
static	struct	etimer	et;
static 	uint32_t	timer_cnt;



/* define prototype of fucntion call */
static 	void get_radio_parameter(void);
static 	void init_default_parameters(void);
static	void show_configuration();



/* if using CC2538DK-SHIELD */
static 	void init_sensor();
static 	void process_sensor(uint8_t verbose);
static 	void set_led_cc2538_shield(int value);


/*---------------------------------------------------------------------------*/
PROCESS(udp_echo_server_process, "SLS server process");
AUTOSTART_PROCESSES(&udp_echo_server_process);

/*---------------------------------------------------------------------------*/
static void init_default_parameters(void) {
	net_db.panid 	= IEEE802154_CONF_PANID;
	memset(&env_db, 0,sizeof(env_db));
	timer_cnt =0;
}


/*---------------------------------------------------------------------------*/
static void get_radio_parameter(void) {
#ifndef SLS_USING_CC2530DK
	NETSTACK_RADIO.get_value(RADIO_PARAM_CHANNEL, &aux);
	net_db.channel = (unsigned int) aux;
	PRINTF(" - CH = %u, ", (unsigned int) aux);	

 	aux = packetbuf_attr(PACKETBUF_ATTR_RSSI);
	net_db.rssi = (int8_t)aux;
 	PRINTF("RSSI = %d dBm, ", net_db.rssi);

	aux = packetbuf_attr(PACKETBUF_ATTR_LINK_QUALITY);
	net_db.lqi = aux;
 	PRINTF("LQI = %u, ", aux);

	NETSTACK_RADIO.get_value(RADIO_PARAM_TXPOWER, &aux);
	net_db.tx_power = aux;
 	PRINTF("Tx Power = %d dBm \n", aux);
#endif 	
}




/*---------------------------------------------------------------------------*/
static void et_timeout_hanler(){
	PRINTF("\nTimer %ds expired... reading sensors, counter = %ld \n", READ_SENSOR_PERIOD, timer_cnt);
    process_sensor(PRINT_SENSOR);
	timer_cnt++;
}	



/*---------------------------------------------------------------------------*/
static void init_sensor() {
#ifdef CC2538DK_HAS_SHIELD
	GPIO_SET_OUTPUT(GPIO_B_BASE, (0x01 | 0x01<<1 | 0x01<<2 | 0x01<<3 | 0x01<<4 | 0x01<<5));	
	GPIO_CLR_PIN(GPIO_B_BASE, (0x01 | 0x01<<1 | 0x01<<2 | 0x01<<3 | 0x01<<4 | 0x01<<5));
	SENSORS_ACTIVATE(bmpx8x);
	if(TSL256X_REF == TSL2561_SENSOR_REF) {
    	PRINTF("Light sensor test --> TSL2561\n");
  	} else if(TSL256X_REF == TSL2563_SENSOR_REF) {
    	PRINTF("Light sensor test --> TSL2563\n");
  	} else {
    	PRINTF("Unknown light sensor reference, aborting\n");
  	}

	SENSORS_ACTIVATE(tsl256x);
  	//TSL256X_REGISTER_INT(light_interrupt_callback);
	tsl256x.configure(TSL256X_INT_OVER, 0x15B8);
#endif	
}

/*---------------------------------------------------------------------------*/
static void set_led_cc2538_shield(int value){
// turn on/off 3 LEDs
#ifdef CC2538DK_HAS_SHIELD
	if (value>0) 	{	GPIO_SET_PIN(GPIO_B_BASE, (0x01 | 0x01<<1 | 0x01<<2 ));} 
	else 			{	GPIO_CLR_PIN(GPIO_B_BASE, (0x01 | 0x01<<1 | 0x01<<2 ));}
#endif	
}


/*---------------------------------------------------------------------------*/
static void process_sensor(uint8_t verbose) {
#ifdef CC2538DK_HAS_SHIELD
	int32_t 	tData;
	uint32_t 	rhData;
	uint8_t 	H, L;

	BMPx8x_pressure = bmpx8x.value(BMPx8x_READ_PRESSURE);
    BMPx8x_temperature = bmpx8x.value(BMPx8x_READ_TEMP);
    TSL256X_light = tsl256x.value(TSL256X_VAL_READ);
    	

   	if(TSL256X_light != TSL256X_ERROR) {
      	env_db.light = TSL256X_light;
	     //context-aware control here: if light is too low, turn on LEDs
		set_led_cc2538_shield(env_db.light < 20);		

    } else {
    	PRINTF("Error, enable the DEBUG flag in the tsl256x driver for info, or check if the sensor is properly connected \n");
    }	

	if((BMPx8x_pressure != BMPx8x_ERROR) && (BMPx8x_temperature != BMPx8x_ERROR)) {
     	//PRINTF("BMPx8x : Pressure = %u.%u(hPa), \n", pressure / 10, pressure % 10);
    	//PRINTF("Temperature = %d.%u(ºC) \n", temperature / 10, temperature % 10);
    	env_db.pressure = BMPx8x_pressure;
    	env_db.temp = BMPx8x_temperature;
    } else {
    	PRINTF("Error, enable the DEBUG flag in the BMPx8x driver for info, or check if the sensor is properly connected \n");
    }
		
		
	//convert Si7021 temperature
	Si7021_temperature = si7021_readTemp(TEMP_NOHOLD);	
	H = (uint8_t)(Si7021_temperature >> 8);
	L = (uint8_t)(Si7021_temperature & 0xFF);
  	tData = ((uint32_t)H  << 8) + (L & 0xFC);
	tData = (((tData) * 21965L) >> 13) - 46850;

	//convert Si7021 humidity 
	Si7021_humidity = si7021_readHumd(RH_NOHOLD);
	H = (uint8_t)(Si7021_humidity >> 8);
	L = (uint8_t)(Si7021_humidity & 0xFF);
	rhData = ((uint32_t)H << 8) + (L & 0xFC);
	rhData = (((rhData) * 15625L) >> 13) - 6000;
	env_db.humidity = Si7021_humidity;
		
	if (verbose) {
		PRINTF("\n--------------------- READING SENSORS ----------------------\n");
	    PRINTF(" - Temperature (Si7021) = %d.%d (ºC) \n", (uint16_t)(tData /1000), (uint16_t)(tData % 1000));
    	PRINTF(" - Humidity (Si7021)    = %d.%d (RH) \n", (uint16_t)(rhData/1000), (uint16_t)(rhData % 1000));
    	PRINTF(" - Temperature (BMPx8x) = %d.%d (ºC) \n", (uint16_t)(env_db.temp/10), (uint16_t)(env_db.temp % 10));
	    PRINTF(" - Pressure (BMPx8x)    = %d.%d (hPa)\n", (uint16_t)(env_db.pressure / 10), (uint16_t)(env_db.pressure % 10));
    	PRINTF(" - Light (TSL256X)      = %d (lux)\n", (uint16_t)(env_db.light));
		PRINTF("------------------------------------------------------------\n");
	}
#endif	

}


/*---------------------------------------------------------------------------*/
void show_configuration() {
	PRINTF("--------------------- NODE INFORMATION-------------\n");
	PRINTF("- PAN ID = 0x%04x; chanel = %d; with PA = %d \n", net_db.panid, RF_CHANNEL, 0);
	PRINTF("- Routing: WITH_NON_STORING = %d\n", WITH_NON_STORING);	
	PRINTF("- Channel check rate = %d\n", NETSTACK_CONF_RDC_CHANNEL_CHECK_RATE);	
#ifdef CC2538DK_HAS_SHIELD		
	PRINTF("- Init parameters, timers, sensor_shield = ENABLED, reading_sensor_interval = %d \n", READ_SENSOR_PERIOD);
#else
	PRINTF("- Init parameters, timers, sensor_shield = DISABLED, reading_sensor_interval = %d  \n", READ_SENSOR_PERIOD);
#endif	
	PRINTF("---------------------------------------------------\n");
}

/*---------------------------------------------------------------------------*/
PROCESS_THREAD(udp_echo_server_process, ev, data) {
  	/* Variables inside a thread should be declared as static */

	PROCESS_BEGIN();
 
  	//NETSTACK_MAC.off(1); 		/* disable RDC */
	show_configuration();	

	init_default_parameters();
	
	/* timer for events */
	etimer_set(&et, CLOCK_SECOND*READ_SENSOR_PERIOD);

	/*if having sensor shield */
	init_sensor();
	
	/* read sensor for the first time */
	process_sensor(PRINT_SENSOR);


	PRINTF("Enter forever loop...\n");	
 	while(1) {
    	PROCESS_YIELD();

    	/* ev timeout */
    	if (ev==PROCESS_EVENT_TIMER) {
    		et_timeout_hanler();
    		etimer_restart(&et);
   		}
  	}
	PROCESS_END();
}
