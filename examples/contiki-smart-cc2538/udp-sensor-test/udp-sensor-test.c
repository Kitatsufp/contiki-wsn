/** \addtogroup cc2538-examples
 * @{
 *
 * \defgroup cc2538-echo-server cc2538dk UDP Echo Server Project
 *
 *  Tests that a node can correctly join an RPL network and also tests UDP
 *  functionality
 * @{
 *
 * \file
 *  An example of a simple UDP echo server for the cc2538dk platform
 */
#include "contiki.h"
#include "contiki-lib.h"
#include "contiki-net.h"

#include <string.h>

#define DEBUG DEBUG_PRINT
#include "net/ip/uip-debug.h"
#include "dev/watchdog.h"
#include "dev/leds.h"
#include "net/rpl/rpl.h"
#include "sys/etimer.h"
#include "dev/cc2538-sensors.h"

/*---------------------------------------------------------------------------*/
#define UIP_IP_BUF   ((struct uip_ip_hdr *)&uip_buf[UIP_LLH_LEN])
#define UIP_UDP_BUF  ((struct uip_udp_hdr *)&uip_buf[uip_l2_l3_hdr_len])

#define MAX_PAYLOAD_LEN 120

#define UDP_SERVER_PORT	54321
#define UDP_CLIENT_PORT 5678
#define PERIOD		60
#define SEND_INTERVAL 	(10*CLOCK_SECOND)
/*---------------------------------------------------------------------------*/
static struct uip_udp_conn *server_conn;
static struct uip_udp_conn *client_conn;
static uip_ipaddr_t server_ipaddr;
static char buf[MAX_PAYLOAD_LEN];
static char cmd[6];
static uint16_t len;
static char *str_report="report to sink\n";
static process_event_t	event_data_ready;
/*---------------------------------------------------------------------------*/
PROCESS(udp_echo_server_process, "UDP echo server process");
PROCESS(udp_report_process, "UDP report process");
AUTOSTART_PROCESSES(&udp_echo_server_process,&udp_report_process);
/*---------------------------------------------------------------------------*/

static void
send_packet(void *ptr)
{
  static int seq_id;
  char buf[MAX_PAYLOAD_LEN];

  seq_id++;
  PRINTF("DATA send to %d 'SeqNum:%d'\n",
         server_ipaddr.u8[sizeof(server_ipaddr.u8) - 1], seq_id);
//  sprintf(buf, "Hello %d from the client", seq_id);
  sprintf(buf,"Seq=%d&Vol=%d&Temp=%d&LED=%d",seq_id,vdd3_sensor.value(CC2538_SENSORS_VALUE_TYPE_CONVERTED), cc2538_temp_sensor.value(CC2538_SENSORS_VALUE_TYPE_CONVERTED),leds_get()&0x01);
  uip_udp_packet_sendto(client_conn, buf, strlen(buf),
                        &server_ipaddr, UIP_HTONS(UDP_SERVER_PORT));
//  printf("LED status: %0.2X|%d|%d\n", leds_get(),leds_get(),leds_get()&0x01);
}

static void
set_global_address(void)
{
  uip_ipaddr_t ipaddr;

  uip_ip6addr(&ipaddr, 0xaaaa, 0, 0, 0, 0, 0, 0, 0);
  uip_ds6_set_addr_iid(&ipaddr, &uip_lladdr);
  uip_ds6_addr_add(&ipaddr, 0, ADDR_AUTOCONF);

//   uip_ip6addr(&server_ipaddr, 0xbbbb, 0, 0, 0, 0xe08e, 0x9dbf, 0x8135, 0xd324);
   uip_ip6addr(&server_ipaddr, 0xbbbb, 0, 0, 0, 0x020c, 0x29ff, 0xfe86, 0xeee0);
}



static void
tcpip_handler(void)
{
  memset(buf, 0, MAX_PAYLOAD_LEN);
  memset(cmd, 0, 5);
  cmd[5]='\0';
  if(uip_newdata()) {
//    leds_on(LEDS_RED);
//    leds_on(LEDS_GREEN);
    len = uip_datalen();
    memcpy(buf, uip_appdata, len);
    memcpy(cmd, uip_appdata, 5);
    PRINTF("%u bytes from [", len);
    PRINT6ADDR(&UIP_IP_BUF->srcipaddr);
    PRINTF("]:%u\n", UIP_HTONS(UIP_UDP_BUF->srcport));
    PRINTF("oo:%s \n",buf);
    
    
    sprintf(buf,"%d mV:%d mC:\n",vdd3_sensor.value(CC2538_SENSORS_VALUE_TYPE_CONVERTED), cc2538_temp_sensor.value(CC2538_SENSORS_VALUE_TYPE_CONVERTED));
    
//    vdd_value = vdd3_sensor.value(CC2538_SENSORS_VALUE_TYPE_CONVERTED);
//    temp_value= cc2538_temp_sensor.value(CC2538_SENSORS_VALUE_TYPE_CONVERTED);
//    sprintf(buf,"%d mV:%d mC:\n",vdd_value,temp_value);

    uip_ipaddr_copy(&server_conn->ripaddr, &UIP_IP_BUF->srcipaddr);
    server_conn->rport = UIP_UDP_BUF->srcport;

//    uip_udp_packet_send(server_conn, buf, len);
    uip_udp_packet_send(server_conn, buf, 18);
    uip_create_unspecified(&server_conn->ripaddr);
    server_conn->rport = 0;
	
    printf("cmd buf: %s \n",cmd);

    if(strcmp(cmd, "blink") == 0){
	PRINTF("Cmd: blink\n");
	leds_blink();
    }else if(strcmp(cmd, "toggl") == 0){
	PRINTF("Cmd: toggle\n");
	leds_toggle(LEDS_RED);
    }else if(strcmp(cmd, "repor") == 0){
	PRINTF("Cmd: report\n");
    }else {
        PRINTF("Cmd: others\n");
    }
  }
  return;
}
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(udp_echo_server_process, ev, data)
{
  static int valid_message;

  PROCESS_BEGIN();

  event_data_ready = process_alloc_event();
  
  PRINTF("Starting UDP echo server\n");

  server_conn = udp_new(NULL, UIP_HTONS(0), NULL);
  udp_bind(server_conn, UIP_HTONS(3000));

  PRINTF("Listen port: 3000, TTL=%u\n", server_conn->ttl);

  while(1) {
    PROCESS_YIELD();
    if(ev == tcpip_event) {
      tcpip_handler();
      printf("event post \n");
      process_post(&udp_report_process, event_data_ready,&valid_message);
    }
  }

  PROCESS_END();
}

PROCESS_THREAD(udp_report_process, ev, data)
{
  static struct etimer periodic;
  PROCESS_BEGIN();
  PROCESS_PAUSE();
 
  set_global_address();
  PRINTF("udp report process started\n");

  client_conn = udp_new(NULL, UIP_HTONS(UDP_SERVER_PORT), NULL);
  if(client_conn == NULL){
     printf("No UDP connection available, exiting the process!\n");
     PROCESS_EXIT();
  }
  udp_bind(client_conn, UIP_HTONS(UDP_CLIENT_PORT));
  PRINTF("Creadted a connection with the server ");
  PRINT6ADDR(&client_conn->ripaddr);
  PRINTF("local/remote port %u/%u\n",
      UIP_HTONS(client_conn->lport),UIP_HTONS(client_conn->rport));
  etimer_set(&periodic, SEND_INTERVAL);
  while(1){
    
    PROCESS_WAIT_EVENT_UNTIL(ev == PROCESS_EVENT_TIMER || ev == event_data_ready);
    if(etimer_expired(&periodic)){
    
	etimer_reset(&periodic);
	send_packet(str_report);
	PRINTF("send packet \n");
	}else if(ev == event_data_ready){
	printf("event received\n");
	send_packet(str_report);
	}
    }

    

    PROCESS_END();
}

/*---------------------------------------------------------------------------*/
/**
 * @}
 * @}
 */
