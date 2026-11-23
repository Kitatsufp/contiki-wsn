
#include "contiki.h"
#include "lib/random.h"
#include "clock.h"
#include "sys/ctimer.h"
#include "net/ip/uip.h"
#include "net/ipv6/uip-ds6.h"
#include "mqtt_sn.h"
#include "dev/leds.h"
#include "net/rime/rime.h"
#include "net/ip/uip.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//#ifdef SLS_USING_CC2538DK
#include "dev/button-sensor.h"
#include "dev/uart.h"
#include "dev/gpio.h"

//#ifdef CC2538DK_HAS_SHIELD
#include "dev/i2c.h"
#include "dev/tsl256x.h"
#include "dev/bmpx8x.h"
#include "dev/si7021.h"
//#endif

//#endif

#define LED_RED        LEDS_BLUE
#define LED_GREEN      LEDS_RED
#define LED_BLUE       LEDS_GREEN

uint8_t sensor_value;

static uint16_t udp_port = 1883;
static uint16_t keep_alive = 60;
//static uint16_t broker_address[] = {120, 72, 103, 71};
static uint16_t broker_address[] = {0xaaaa, 0, 0, 0, 0, 0, 0, 0x1};
static struct   etimer time_poll;
// static uint16_t tick_process = 0;
static char     pub_msg[30];

static char     device_id[17];
static char     topic_hw[25];
static char     *topics_mqtt[] = {"/led_control",
                                  "/temp",
                                  "/humid",
                                  "/light",
                                  "/pressure"};
// static char     *will_topic = "/6lowpan_node/offline";
// static char     *will_message = "O dispositivo esta offline";
// This topics will run so much faster than others


/*---------------------------------------------------------------------------*/
#define UIP_IP_BUF   ((struct uip_ip_hdr *)&uip_buf[UIP_LLH_LEN])
#define UIP_UDP_BUF  ((struct uip_udp_hdr *)&uip_buf[uip_l2_l3_hdr_len])


#define MAX_PAYLOAD_LEN       120
#define READ_SENSOR_PERIOD      10     // seconds


// CC2538DK has shield with sensors
//#ifdef CC2538DK_HAS_SHIELD
static uint16_t TSL256X_light;
static uint16_t BMPx8x_pressure;
static int16_t  BMPx8x_temperature;
static uint16_t Si7021_humidity;
static uint16_t Si7021_temperature;
//#endif

/*---------------------------------------------------------------------------*/
//static char buf[MAX_PAYLOAD_LEN];

/*define timers */
//static  struct  etimer  et;
//static  uint32_t  timer_cnt;



/* define prototype of fucntion call 
static  void get_radio_parameter(void);
static  void init_default_parameters(void);
static  void show_configuration();
*/


/* if using CC2538DK-SHIELD */
static  void init_sensor();
static  void process_sensor();
static  void set_led_cc2538_shield(int value);

/*---------------------------------------------------------------------------*/

static  uint8_t led_on_off;                                  

mqtt_sn_con_t mqtt_sn_connection;

void mqtt_sn_callback(char *topic, char *message){
  printf("\nMessage received:");
  printf("\nTopic:%s Message:%s",topic,message);
}

void init_broker(void){
  char *all_topics[ss(topics_mqtt)+1];
  sprintf(device_id,"%02X%02X%02X%02X%02X%02X%02X%02X",
          linkaddr_node_addr.u8[0],linkaddr_node_addr.u8[1],
          linkaddr_node_addr.u8[2],linkaddr_node_addr.u8[3],
          linkaddr_node_addr.u8[4],linkaddr_node_addr.u8[5],
          linkaddr_node_addr.u8[6],linkaddr_node_addr.u8[7]);
  // sprintf(topic_hw,"Hello addr:%02X%02X",linkaddr_node_addr.u8[6],linkaddr_node_addr.u8[7]);

  mqtt_sn_connection.client_id     = device_id;
  mqtt_sn_connection.udp_port      = udp_port;
  mqtt_sn_connection.ipv6_broker   = broker_address;
  mqtt_sn_connection.keep_alive    = keep_alive;
  //mqtt_sn_connection.will_topic    = will_topic;   // Configure as 0x00 if you don't want to use
  //mqtt_sn_connection.will_message  = will_message; // Configure as 0x00 if you don't want to use
  mqtt_sn_connection.will_topic    = 0x00;
  mqtt_sn_connection.will_message  = 0x00;

  mqtt_sn_init();   // Inicializa alocação de eventos e a principal PROCESS_THREAD do MQTT-SN

  size_t i;
  for(i=0;i<ss(topics_mqtt);i++)
    all_topics[i] = topics_mqtt[i];
  all_topics[i] = topic_hw;

  mqtt_sn_create_sck(mqtt_sn_connection,
                     all_topics,
                     ss(all_topics),
                     mqtt_sn_callback);
  // mqtt_sn_sub((char*)"#",0);
}

/*---------------------------------------------------------------------------*/

static void init_sensor() {
//#ifdef CC2538DK_HAS_SHIELD
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
//#endif  
}

/*---------------------------------------------------------------------------*/
static void set_led_cc2538_shield(int value){
// turn on/off 3 LEDs
//#ifdef CC2538DK_HAS_SHIELD
  if (value>0)  { GPIO_SET_PIN(GPIO_B_BASE, (0x01 | 0x01<<1 | 0x01<<2 ));} 
  else      { GPIO_CLR_PIN(GPIO_B_BASE, (0x01 | 0x01<<1 | 0x01<<2 ));}
//#endif  
}


/*---------------------------------------------------------------------------*/
static void process_sensor() {
//#ifdef CC2538DK_HAS_SHIELD
  int32_t   tData;
  uint32_t  rhData;
  uint8_t   H, L;

  BMPx8x_pressure = bmpx8x.value(BMPx8x_READ_PRESSURE);
    BMPx8x_temperature = bmpx8x.value(BMPx8x_READ_TEMP);
    TSL256X_light = tsl256x.value(TSL256X_VAL_READ);
      

    if(TSL256X_light != TSL256X_ERROR) {
      //env_db.light = TSL256X_light;
      //context-aware control here: if light is too low, turn on LEDs
    set_led_cc2538_shield(TSL256X_light < 20);   

    } else {
      PRINTF("Error, enable the DEBUG flag in the tsl256x driver for info, or check if the sensor is properly connected \n");
    } 

  if((BMPx8x_pressure != BMPx8x_ERROR) && (BMPx8x_temperature != BMPx8x_ERROR)) {
      //PRINTF("BMPx8x : Pressure = %u.%u(hPa), \n", pressure / 10, pressure % 10);
      //PRINTF("Temperature = %d.%u(ºC) \n", temperature / 10, temperature % 10);
      //env_db.pressure = BMPx8x_pressure;
      //env_db.temp = BMPx8x_temperature;
    } else {
      PRINTF("Error, enable the DEBUG flag in the BMPx8x driver for info, or check if the sensor is properly connected \n");
    }
    
    
  //convert Si7021 temperature
  Si7021_temperature = si7021_readTemp(TEMP_NOHOLD);  
  H = (uint8_t)(Si7021_temperature >> 8);
  L = (uint8_t)(Si7021_temperature & 0xFF);
  tData = ((uint32_t)H  << 8) + (L & 0xFC);
  tData = (((tData) * 21965L) >> 13) - 46850;
  Si7021_temperature = tData;

  //convert Si7021 humidity 
  Si7021_humidity = si7021_readHumd(RH_NOHOLD);
  H = (uint8_t)(Si7021_humidity >> 8);
  L = (uint8_t)(Si7021_humidity & 0xFF);
  rhData = ((uint32_t)H << 8) + (L & 0xFC);
  rhData = (((rhData) * 15625L) >> 13) - 6000;
  Si7021_humidity = rhData;

  //env_db.humidity = Si7021_humidity;
  
    /*if (verbose) {
      PRINTF("\n--------------------- READING SENSORS ----------------------\n");
      PRINTF(" - Temperature (Si7021) = %d.%d (ºC) \n", (uint16_t)(tData /1000), (uint16_t)(tData % 1000));
      PRINTF(" - Humidity (Si7021)    = %d.%d (RH) \n", (uint16_t)(rhData/1000), (uint16_t)(rhData % 1000));
      PRINTF(" - Temperature (BMPx8x) = %d.%d (ºC) \n", (uint16_t)(BMPx8x_temperature/10), (uint16_t)(BMPx8x_temperature % 10));
      PRINTF(" - Pressure (BMPx8x)    = %d.%d (hPa)\n", (uint16_t)(BMPx8x_pressure / 10), (uint16_t)(BMPx8x_pressure % 10));
      PRINTF(" - Light (TSL256X)      = %d (lux)\n", (uint16_t)(TSL256X_light));
      PRINTF("------------------------------------------------------------\n");
  } */
//#endif  

}
/*---------------------------------------------------------------------------*/

PROCESS(init_system_process, "[Contiki-OS] Initializing OS");
AUTOSTART_PROCESSES(&init_system_process);
/*---------------------------------------------------------------------------*/

PROCESS_THREAD(init_system_process, ev, data) {
  PROCESS_BEGIN();

  debug_os("Initializing the MQTT_SN_DEMO..");

  init_broker();

  /* timer for events */
  etimer_set(&time_poll,READ_SENSOR_PERIOD*CLOCK_SECOND);

  /*if having sensor shield */
  init_sensor();
  
  led_on_off = 0;

  while(1) {
    PROCESS_WAIT_EVENT();
    /* read sensor for the first time */
     
    //debug_os("State MQTT:%s",mqtt_sn_check_status_string());
    if (etimer_expired(&time_poll)) {
      if (led_on_off ==0) {
        led_on_off = 1;
      }
      else {
        led_on_off = 0;
      }
      etimer_reset(&time_poll);
    }

    if (led_on_off ==1) { 
	    process_sensor();
		  sprintf(pub_msg,"%d.%d", (uint16_t)(BMPx8x_temperature/10),(uint16_t)(BMPx8x_temperature % 10));
		  mqtt_sn_pub("/temp", pub_msg, true, 0);
      PRINTF("Message sent: topic: /temp Message: (BMPx8x) = %d.%d (C) \n", (uint16_t)(BMPx8x_temperature/10), (uint16_t)(BMPx8x_temperature % 10));

      //mqtt_sn_pub("/temp", "37.5C", true, 0);
    }
    else {   
      mqtt_sn_pub("/led_control","red_off",true,0);
      printf("\nMessage sent: topic: /led_control Message: red_off");
    }
  }

  PROCESS_END();
}

