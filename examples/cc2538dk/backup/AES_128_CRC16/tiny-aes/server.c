#include "contiki.h"
#include "contiki-lib.h"
#include "contiki-net.h"

#include <string.h>

#define DEBUG DEBUG_PRINT
#include "net/ip/uip-debug.h"
#include "dev/watchdog.h"
#include "dev/leds.h"
#include "net/rpl/rpl.h"
#include "dev/button-sensor.h"
#include "debug.h"

#include "util.h"

/* demo sai */
#define DATA_RECEIVED_WRONG   0
#define CRC_FAIL              0

#define UIP_IP_BUF   ((struct uip_ip_hdr *)&uip_buf[UIP_LLH_LEN])
#define UIP_UDP_BUF  ((struct uip_udp_hdr *)&uip_buf[uip_l2_l3_hdr_len])

static struct uip_udp_conn *server_conn;
static uint16_t len;

/* Reply msg? */                         
#define SERVER_REPLY          1

static uint8_t data_received  [MAX_LEN_CBC];
static uint8_t data_decrypted [MAX_LEN_CBC];

static uint8_t data_encrypted [MAX_LEN_CBC];
static uint8_t data_reply     [MAX_LEN_CBC];

static uint8_t key[16] = { 0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, \
                           0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c };

static uint8_t iv[16]  = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, \
                           0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f };

static unsigned short crc16_check;

/*---------------------------------------------------------------------------*/
PROCESS(udp_server_process, "UDP server process");
AUTOSTART_PROCESSES(&udp_server_process);
/*---------------------------------------------------------------------------*/

static void 
tcpip_handler(void)
{
  memset(data_received, 0, MAX_LEN_CBC);

  if(uip_newdata()) {
    leds_on(LEDS_RED);
    len = uip_datalen();

    if(len == MAX_LEN_CBC){
      memcpy(data_received, uip_appdata, len);

#if DATA_RECEIVED_WRONG
      data_received[63] ^= 0X80;
#endif /*DATA_RECEIVED_WRONG*/
      
      PRINTF("%u bytes from [", len);
      PRINT6ADDR(&UIP_IP_BUF->srcipaddr);
      PRINTF("]:%u\n", UIP_HTONS(UIP_UDP_BUF->srcport));

  		decrypt_cbc(data_decrypted, data_received, key, iv);
      crc16_check = gen_crc16(data_decrypted, len);
      
#if CRC_FAIL
      crc16_check ^= 0x0400;
#endif /* CRC_FAIL*/

      printf("CRC16 = 0x%04x\n", crc16_check);
      
      if (check_crc16(data_decrypted, len)) {
        printf("CRC true\n");

#if SERVER_REPLY
        uip_ipaddr_copy(&server_conn->ripaddr, &UIP_IP_BUF->srcipaddr);
        server_conn->rport = UIP_UDP_BUF->srcport;

        memcpy(data_reply, data_decrypted, len);

        encrypt_cbc(data_encrypted, data_reply, key, iv);
        uip_udp_packet_send(server_conn, data_encrypted, len);
        /* Restore server connection to allow data from any node */
        uip_create_unspecified(&server_conn->ripaddr);
        server_conn->rport = 0;
#endif /* SERVER_REPLY */

      } else printf("CRC fail\n"); // check_crc fail
    } /* endif len == MAX_LEN_CBC */
  }/* endif uip_newdata */
  leds_off(LEDS_RED);
  return;
}
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(udp_server_process, ev, data)
{
  PROCESS_BEGIN();
  printf("Starting UDP server\n");

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
/*---------------------------------------------------------------------------*/