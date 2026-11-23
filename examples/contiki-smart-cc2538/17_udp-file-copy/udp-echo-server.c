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
#include "cfs/cfs.h"
#include "dev/cc2538-sensors.h"
#include "dev/button-sensor.h"
#include "loader/elfloader.h"

#include <string.h>
#include <stdio.h>

#define DEBUG DEBUG_PRINT
#include "net/ip/uip-debug.h"
#include "dev/watchdog.h"
#include "dev/leds.h"
#include "net/rpl/rpl.h"
/*---------------------------------------------------------------------------*/
#define UIP_IP_BUF   ((struct uip_ip_hdr *)&uip_buf[UIP_LLH_LEN])
#define UIP_UDP_BUF  ((struct uip_udp_hdr *)&uip_buf[uip_l2_l3_hdr_len])

#define MAX_PAYLOAD_LEN 120
#define MAX_BLOCK_SIZE 40
/*---------------------------------------------------------------------------*/
static struct uip_udp_conn *server_conn;
static char buf[MAX_PAYLOAD_LEN];
static uint16_t len;

static char *filename = "msg_file";
//static char buf[100];
static char read_buf[MAX_BLOCK_SIZE];
static int fd_write, fd_read;
/*---------------------------------------------------------------------------*/
PROCESS(udp_echo_server_process, "UDP echo server process");
PROCESS(udp_button_process, "UDP button sensor process");
AUTOSTART_PROCESSES(&udp_echo_server_process,&udp_button_process);
/*---------------------------------------------------------------------------*/
static void
tcpip_handler(void)
{
  int n;
  memset(buf, 0, MAX_PAYLOAD_LEN);
  if(uip_newdata()) {
    leds_on(LEDS_RED);
    leds_on(LEDS_GREEN);
    len = uip_datalen();
    memcpy(buf, uip_appdata, len);
 //   PRINTF("%u bytes from [", len);
 //   PRINT6ADDR(&UIP_IP_BUF->srcipaddr);
 //   PRINTF("]:%u\n", UIP_HTONS(UIP_UDP_BUF->srcport));
 //   PRINTF("%s \n",buf);
//    PRINTF("%s",buf);

    fd_write = cfs_open(filename, CFS_WRITE|CFS_APPEND);
    if(fd_write != -1){
      n = cfs_write(fd_write,buf,sizeof(buf));
      cfs_close(fd_write);
 //     printf("write to file:%s %i bytes\n",buf,n);
    }else{
      printf("Error: could not write to file.\n");
    }

    uip_ipaddr_copy(&server_conn->ripaddr, &UIP_IP_BUF->srcipaddr);
    server_conn->rport = UIP_UDP_BUF->srcport;

    uip_udp_packet_send(server_conn, buf, len);
    uip_create_unspecified(&server_conn->ripaddr);
    server_conn->rport = 0;
  }
  leds_off(LEDS_RED);
  leds_off(LEDS_GREEN);
  return;
}
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(udp_echo_server_process, ev, data)
{

  PROCESS_BEGIN();
  PRINTF("Starting UDP echo server\n");

  server_conn = udp_new(NULL, UIP_HTONS(0), NULL);
  udp_bind(server_conn, UIP_HTONS(3000));

  PRINTF("Listen port: 3000, TTL=%u\n", server_conn->ttl);

  while(1) {
    PROCESS_YIELD();
    if(ev == tcpip_event) {
      tcpip_handler();
    }
  }

  PROCESS_END();
}

PROCESS_THREAD(udp_button_process, ev, data)
{
  int len = 1;
  int elf_ret;
  PROCESS_BEGIN();
  

  while(1){
    PROCESS_YIELD();
    if(ev == sensors_event){
      if(data == &button_select_sensor) {
        printf("Sensor: button select pressed.\n");
        fd_read = cfs_open(filename, CFS_READ);
        if(fd_read != -1){
	  while(len > 0){
            len = cfs_read(fd_read,read_buf, sizeof(read_buf));
//	    printf("read: %s\n",read_buf);
	    printf("%s",read_buf);
           }
          cfs_close(fd_read);
        }else{
          printf("Error: could not open file for read.\n");
        }
      }else if(ev == &button_left_sensor){
         printf("Sensor: button left pressed.\n");
         cfs_remove(filename);
         fd_read = cfs_open(filename, CFS_READ);
         if(fd_read == -1){
            printf("successfull removed file\n");
         }else{
            printf("Error: could not remove the file.\n");
         }
       }else if(ev == &button_right_sensor){
         printf("Sensor: button right pressed.\n");
         fd_read = cfs_open(filename, CFS_READ);
         if(fd_read != -1){
	   elf_ret = elfloader_load(fd_read);
           cfs_close(fd_read);
           if(elf_ret == ELFLOADER_OK){
              printf("Elf load: Ok\n");
           }else if(elf_ret == ELFLOADER_BAD_ELF_HEADER){
              printf("Bad ELF header\n");
           }else if(elf_ret == ELFLOADER_NO_SYMTAB){
              printf("No symbol table\n");
           }else {
              printf(" Elf load fail.\n");
           }
         }
         cfs_close(fd_read);
       }
       
       } 
   } /*while*/
 
 PROCESS_END();

}

/*---------------------------------------------------------------------------*/
/**
 * @}
 * @}
 */
