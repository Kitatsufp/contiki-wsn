#include "contiki.h"
#include "net/ip/uip.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ip/uip-udp-packet.h"
#include "net/rpl/rpl.h"
#include "dev/serial-line.h"
#if CONTIKI_TARGET_Z1
#include "dev/uart0.h"
#else
#include "dev/uart1.h"
#endif
#include "collect-common.h"
#include "collect-view.h"

#include <stdio.h>
#include <string.h>


/*------------------------ADDITION PART------------------*/
#include "sys/energest.h"
#include "net/mac/csma.h"
#include "net/nbr-table.h"
/*-------------------------------------------------------*/

#define UDP_CLIENT_PORT 8775
#define UDP_SERVER_PORT 5688


/*------------------------ADDITION PART------------------*/
#define ETX_SLOT   4
#define RANK_SLOT  5
#define DROP_SLOT  6
#define RDC_SLOT   7
#define QLEN_SLOT  8
#define FWD_SLOT   9
/*-------------------------------------------------------*/

#define DEBUG DEBUG_PRINT
#include "net/ip/uip-debug.h"

static struct uip_udp_conn *client_conn;
static uip_ipaddr_t server_ipaddr;
extern int32_t rpl_combined_cost(rpl_parent_t *p);
/*---------------------------------------------------------------------------*/
PROCESS(udp_client_process, "UDP client process");
AUTOSTART_PROCESSES(&udp_client_process, &collect_common_process);
/*---------------------------------------------------------------------------*/
void
collect_common_set_sink(void)
{
  /* A udp client can never become sink */
}
/*---------------------------------------------------------------------------*/

void
collect_common_net_print(void)
{
  rpl_dag_t *dag;
  uip_ds6_route_t *r;

  /* Let's suppose we have only one instance */
  dag = rpl_get_any_dag();
  if(dag->preferred_parent != NULL) {
    PRINTF("Preferred parent: ");
    PRINT6ADDR(rpl_get_parent_ipaddr(dag->preferred_parent));
    PRINTF("\n");
  }
  for(r = uip_ds6_route_head();
      r != NULL;
      r = uip_ds6_route_next(r)) {
    PRINT6ADDR(&r->ipaddr);
  }
  PRINTF("---\n");
}
/*---------------------------------------------------------------------------*/
static void
tcpip_handler(void)
{
  if(uip_newdata()) {
    /* Ignore incoming data */
  }
}
/*---------------------ADDITION PART------------------------------------------*/
static uint16_t
get_rdc_permille(void)
{
  static unsigned long l_tx, l_rx, l_cpu, l_lpm;
  unsigned long tx, rx, cpu, lpm, total;
  uint16_t rdc;

  energest_flush();
  tx  = energest_type_time(ENERGEST_TYPE_TRANSMIT);
  rx  = energest_type_time(ENERGEST_TYPE_LISTEN);
  cpu = energest_type_time(ENERGEST_TYPE_CPU);
  lpm = energest_type_time(ENERGEST_TYPE_LPM);

  total = (cpu - l_cpu) + (lpm - l_lpm);
  rdc = total ? (uint16_t)((((tx - l_tx) + (rx - l_rx)) * 1000UL) / total) : 0;

  l_tx = tx; l_rx = rx; l_cpu = cpu; l_lpm = lpm;
  return rdc;
}
/*---------------------ADDITION PART------------------------------------------*/
static uint16_t
get_forwarded_count(void)
{
  return (uint16_t)uip_stat.ip.forwarded;
}
/*---------------------------------------------------------------------------*/
void
collect_common_send(void)
{
  static uint8_t seqno;
  struct {
    uint8_t seqno;
    uint8_t for_alignment;
    struct collect_view_data_msg msg;
  } msg;
  /* struct collect_neighbor *n; */
  uint16_t parent_etx;
  uint16_t rtmetric;
  uint16_t num_neighbors;
  uint16_t beacon_interval;
  rpl_parent_t *preferred_parent;
  linkaddr_t parent;
  rpl_dag_t *dag;
  uint16_t link_etx = 0;
  if(client_conn == NULL) {
    /* Not setup yet */
    return;
  }
  memset(&msg, 0, sizeof(msg));
  seqno++;
  if(seqno == 0) {
    /* Wrap to 128 to identify restarts */
    seqno = 128;
  }
  msg.seqno = seqno;

  linkaddr_copy(&parent, &linkaddr_null);
  parent_etx = 0;

  /* Let's suppose we have only one instance */
  dag = rpl_get_any_dag();
  if(dag != NULL) {
   preferred_parent = dag->preferred_parent;
         {
      rpl_parent_t *p;
      rpl_parent_t *best = NULL;
      int32_t c, best_c = 0, pref_c = 0;

      /* pass 1: find the cheapest candidate */
      for(p = nbr_table_head(rpl_parents);
          p != NULL;
          p = nbr_table_next(rpl_parents, p)) {
        c = rpl_combined_cost(p);
        if(best == NULL || c < best_c) {
          best = p;
          best_c = c;
        }
        if(p == preferred_parent) {
          pref_c = c;
        }
      }
      /* pass 2: print every candidate */
      for(p = nbr_table_head(rpl_parents);
          p != NULL;
          p = nbr_table_next(rpl_parents, p)) {
        uint16_t e = rpl_get_parent_link_metric(p);
        printf("NBR %u etx %u.%02u rank %u q %u.%u cost %ld%s%s\n",
               rpl_get_parent_ipaddr(p)->u8[15],
               e / 128, (unsigned)(((e % 128) * 100UL) / 128),
               (unsigned)p->rank,
               (unsigned)(p->queue / 16),
               (unsigned)((p->queue % 16) * 10 / 16),
               (long)rpl_combined_cost(p),
               (p == preferred_parent) ? " *" : "",
               (p == best) ? " <best" : "");
      }
      /* one summary line */
      if(preferred_parent != NULL && best != NULL) {
        printf("PICK pref %u cost %ld best %u cost %ld diff %ld myrank %u\n",
               rpl_get_parent_ipaddr(preferred_parent)->u8[15], (long)pref_c,
               rpl_get_parent_ipaddr(best)->u8[15], (long)best_c,
               (long)(pref_c - best_c), (unsigned)dag->rank);
      }
    }
    if(preferred_parent != NULL) {
      uip_ds6_nbr_t *nbr;
      nbr = uip_ds6_nbr_lookup(rpl_get_parent_ipaddr(preferred_parent));
      if(nbr != NULL) {
        /* Use parts of the IPv6 address as the parent address, in reversed byte order. */
        parent.u8[LINKADDR_SIZE - 1] = nbr->ipaddr.u8[sizeof(uip_ipaddr_t) - 2];
        parent.u8[LINKADDR_SIZE - 2] = nbr->ipaddr.u8[sizeof(uip_ipaddr_t) - 1];
        parent_etx = rpl_get_parent_rank((uip_lladdr_t *) uip_ds6_nbr_get_ll(nbr)) / 2;
      }
	link_etx = rpl_get_parent_link_metric(preferred_parent);
    }
    rtmetric = dag->rank;
    beacon_interval = (uint16_t) ((2L << dag->instance->dio_intcurrent) / 1000);
    num_neighbors = uip_ds6_nbr_num();
  } else {
    rtmetric = 0;
    beacon_interval = 0;
    num_neighbors = 0;
  }

  /* num_neighbors = collect_neighbor_list_num(&tc.neighbor_list); */
  collect_view_construct_message(&msg.msg, &parent,
                                 link_etx, rtmetric,
                                 num_neighbors, beacon_interval);

/*---------------------ADDITION PART------------------------------------------*/
msg.msg.sensors[DROP_SLOT] = csma_drop_count;
msg.msg.sensors[RDC_SLOT]  = get_rdc_permille();
msg.msg.sensors[QLEN_SLOT] = csma_queue_length();
//msg.msg.sensors[QLEN_SLOT] = csma_qlen_max;
//csma_qlen_max = csma_queue_length();
msg.msg.sensors[FWD_SLOT]  = get_forwarded_count();
msg.msg.sensors[ETX_SLOT]  = link_etx;
msg.msg.sensors[RANK_SLOT] = rtmetric;

  printf("HIST %u %u %u %u %u %u %u\n", csma_hist[0], csma_hist[1],
         csma_hist[2], csma_hist[3], csma_hist[4], csma_hist[5], csma_hist[6]);
/*---------------------------------------------------------------------------*/

  uip_udp_packet_sendto(client_conn, &msg, sizeof(msg),
                        &server_ipaddr, UIP_HTONS(UDP_SERVER_PORT));
}
/*---------------------------------------------------------------------------*/
void
collect_common_net_init(void)
{
#if CONTIKI_TARGET_Z1
  uart0_set_input(serial_line_input_byte);
#else
  uart1_set_input(serial_line_input_byte);
#endif
  serial_line_init();
}
/*---------------------------------------------------------------------------*/
static void
print_local_addresses(void)
{
  int i;
  uint8_t state;

  PRINTF("Client IPv6 addresses: ");
  for(i = 0; i < UIP_DS6_ADDR_NB; i++) {
    state = uip_ds6_if.addr_list[i].state;
    if(uip_ds6_if.addr_list[i].isused &&
       (state == ADDR_TENTATIVE || state == ADDR_PREFERRED)) {
      PRINT6ADDR(&uip_ds6_if.addr_list[i].ipaddr);
      PRINTF("\n");
      /* hack to make address "final" */
      if (state == ADDR_TENTATIVE) {
        uip_ds6_if.addr_list[i].state = ADDR_PREFERRED;
      }
    }
  }
}
/*---------------------------------------------------------------------------*/
static void
set_global_address(void)
{
  uip_ipaddr_t ipaddr;

  uip_ip6addr(&ipaddr, UIP_DS6_DEFAULT_PREFIX, 0, 0, 0, 0, 0, 0, 0);
  uip_ds6_set_addr_iid(&ipaddr, &uip_lladdr);
  uip_ds6_addr_add(&ipaddr, 0, ADDR_AUTOCONF);

  /* set server address */
  uip_ip6addr(&server_ipaddr, UIP_DS6_DEFAULT_PREFIX, 0, 0, 0, 0, 0, 0, 1);

}
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(udp_client_process, ev, data)
{
  PROCESS_BEGIN();

  PROCESS_PAUSE();

  set_global_address();

  PRINTF("UDP client process started\n");

  print_local_addresses();

  /* new connection with remote host */
  client_conn = udp_new(NULL, UIP_HTONS(UDP_SERVER_PORT), NULL);
  udp_bind(client_conn, UIP_HTONS(UDP_CLIENT_PORT));

  PRINTF("Created a connection with the server ");
  PRINT6ADDR(&client_conn->ripaddr);
  PRINTF(" local/remote port %u/%u\n",
        UIP_HTONS(client_conn->lport), UIP_HTONS(client_conn->rport));

  while(1) {
    PROCESS_YIELD();
    if(ev == tcpip_event) {
      tcpip_handler();
    }
  }

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
