#include "contiki.h"
#include "contiki-lib.h"
#include "contiki-net.h"
#include "net/ip/uip.h"
#include "net/rpl/rpl.h"
#include "net/linkaddr.h"

#include "net/netstack.h"
#include "dev/button-sensor.h"
#include "dev/serial-line.h"
#if CONTIKI_TARGET_Z1
#include "dev/uart0.h"
#else
#include "dev/uart1.h"
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "collect-common.h"
#include "collect-view.h"

/* DEBUG_NONE removes the PRINTF strings to save ROM. STAT and LOAD use
   plain printf, so they are still printed. */
#define DEBUG DEBUG_NONE
#include "net/ip/uip-debug.h"

#define UIP_IP_BUF   ((struct uip_ip_hdr *)&uip_buf[UIP_LLH_LEN])

#define UDP_CLIENT_PORT 8775
#define UDP_SERVER_PORT 5688

/*---------------------ADDITION PART------------------------------------------*/
/* Slots of msg.msg.sensors[]. Must be identical in sender.c */
#define MAX_NODES  32
#define ETX_SLOT   4
#define RANK_SLOT  5
#define DROP_SLOT  6
#define RDC_SLOT   7
#define QLEN_SLOT  8
#define FWD_SLOT   9
/*---------------------------------------------------------------------------*/

/*---------------------ADDITION PART------------------------------------------*/
struct node_stat {
  uint8_t  used, last_seq;
  uint16_t id;
  uint32_t rx, expected;
  uint32_t last_rx, last_expected;
  uint16_t rdc, qlen, fwd, drop;
  uint16_t etx, rank;
};
static struct node_stat nodes[MAX_NODES];
/*---------------------------------------------------------------------------*/
static struct uip_udp_conn *server_conn;

PROCESS(udp_server_process, "UDP server process");
AUTOSTART_PROCESSES(&udp_server_process,&collect_common_process);
/*---------------------------------------------------------------------------*/
void
collect_common_set_sink(void)
{
}
/*---------------------------------------------------------------------------*/
void
collect_common_net_print(void)
{
  printf("I am sink!\n");
}
/*---------------------------------------------------------------------------*/
void
collect_common_send(void)
{
  /* Server never sends */
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

  PRINTF("I am sink!\n");
}
/*---------------------------------------------------------------------------*/
static void
print_local_addresses(void)
{
  int i;
  uint8_t state;

  PRINTF("Server IPv6 addresses: ");
  for(i = 0; i < UIP_DS6_ADDR_NB; i++) {
    state = uip_ds6_if.addr_list[i].state;
    if(state == ADDR_TENTATIVE || state == ADDR_PREFERRED) {
      PRINT6ADDR(&uip_ds6_if.addr_list[i].ipaddr);
      PRINTF("\n");
      /* hack to make address "final" */
      if (state == ADDR_TENTATIVE) {
        uip_ds6_if.addr_list[i].state = ADDR_PREFERRED;
      }
    }
  }
}

/*---------------------ADDITION PART------------------------------------------*/
static struct node_stat *
node_get(uint16_t id)
{
  int i, free_i = -1;
  for(i = 0; i < MAX_NODES; i++) {
    if(nodes[i].used && nodes[i].id == id) {
      return &nodes[i];
    }
    if(!nodes[i].used && free_i < 0) {
      free_i = i;
    }
  }
  if(free_i < 0) {
    return NULL;
  }
  memset(&nodes[free_i], 0, sizeof(nodes[free_i]));
  nodes[free_i].used = 1;
  nodes[free_i].id = id;
  return &nodes[free_i];
}
/*---------------------ADDITION PART------------------------------------------*/
static void
sink_stats_update(const linkaddr_t *s, uint8_t seqno,
                  struct collect_view_data_msg *m)
{
  struct node_stat *n = node_get(((uint16_t)s->u8[1] << 8) | s->u8[0]);
  uint16_t delta;

  if(n == NULL) {
    return;
  }
  if(n->expected == 0) {
    delta = 1;
  } else if(seqno > n->last_seq) {
    delta = seqno - n->last_seq;
  } else if(seqno >= 128 && seqno < n->last_seq) {
    delta = 128 - (n->last_seq - seqno);
  } else if(seqno < 128) {
    delta = seqno;
  } else {
    return;
  }
  n->expected += delta;
  n->rx++;
  n->last_seq = seqno;
  n->rdc  = m->sensors[RDC_SLOT];
  n->qlen = m->sensors[QLEN_SLOT];
  n->fwd  = m->sensors[FWD_SLOT];
  n->drop = m->sensors[DROP_SLOT];
  n->etx  = m->sensors[ETX_SLOT];
  n->rank = m->sensors[RANK_SLOT];
}
/*---------------------ADDITION PART------------------------------------------*/
/* Jain index in 1/1000 using integer math only (no float, saves ROM). */
static unsigned
jain_permille(void)
{
  int i;
  uint32_t load, max = 0, sum = 0, sumsq = 0, cnt = 0, n, d;
  uint8_t shift = 0;

  /* pass 1: largest load */
  for(i = 0; i < MAX_NODES; i++) {
    if(!nodes[i].used || nodes[i].expected == 0) {
      continue;
    }
    load = nodes[i].expected + nodes[i].fwd;
    if(load > max) {
      max = load;
    }
  }
  /* scale so the largest load is at most 255 and nothing overflows 32 bits */
  while((max >> shift) > 255) {
    shift++;
  }
  /* pass 2: sums */
  for(i = 0; i < MAX_NODES; i++) {
    if(!nodes[i].used || nodes[i].expected == 0) {
      continue;
    }
    load = (nodes[i].expected + nodes[i].fwd) >> shift;
    sum += load;
    sumsq += load * load;
    cnt++;
  }
  n = sum * sum;
  d = cnt * sumsq;
  if(d == 0) {
    return 0;
  }
  if(n <= 4294967UL) {
    return (unsigned)((n * 1000UL) / d);
  }
  return (unsigned)(n / (d / 1000UL));
}
/*---------------------ADDITION PART------------------------------------------*/
static void
sink_stats_print(void)
{
  int i;
  unsigned j;
  uint32_t d_rx, d_exp;

  for(i = 0; i < MAX_NODES; i++) {
    if(!nodes[i].used || nodes[i].expected == 0) {
      continue;
    }
    d_rx  = nodes[i].rx - nodes[i].last_rx;
    d_exp = nodes[i].expected - nodes[i].last_expected;
    nodes[i].last_rx = nodes[i].rx;
    nodes[i].last_expected = nodes[i].expected;

    printf("STAT node %u PDR %lu/1000", (nodes[i].id)/257,
           (nodes[i].rx * 1000UL) / nodes[i].expected);
    if(d_exp > 0) {
      printf(" WPDR %lu/1000", (d_rx * 1000UL) / d_exp);
    } else {
      printf(" WPDR none");
    }
    printf(" RDC %u/1000 QLEN %u DROP %u FWD %u",
           nodes[i].rdc, nodes[i].qlen, nodes[i].drop, nodes[i].fwd);
    printf(" ETX %u.%02u RANK %u\n",
           nodes[i].etx / 128,
           (unsigned)(((nodes[i].etx % 128) * 100UL) / 128),
           nodes[i].rank);
  }
  j = jain_permille();
  if(j > 0) {
    printf("LOAD jain %u/1000\n", j);
  }
}
/*---------------------------------------------------------------------------*/
static void
tcpip_handler(void)
{
  uint8_t *appdata;
  linkaddr_t sender;
  uint8_t seqno;
  uint8_t hops;

  if(uip_newdata()) {
    appdata = (uint8_t *)uip_appdata;
    sender.u8[0] = UIP_IP_BUF->srcipaddr.u8[15];
    sender.u8[1] = UIP_IP_BUF->srcipaddr.u8[14];
    seqno = *appdata;
    hops = uip_ds6_if.cur_hop_limit - UIP_IP_BUF->ttl + 1;
/*---------------------ADDITION PART------------------------------------------*/
    sink_stats_update(&sender, seqno,
                      (struct collect_view_data_msg *)(appdata + 2));
/*---------------------------------------------------------------------------*/
    collect_common_recv(&sender, seqno, hops,
                        appdata + 2, uip_datalen() - 2);
  }
}
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(udp_server_process, ev, data)
{
  uip_ipaddr_t ipaddr;
  static struct etimer stat_timer;
  struct uip_ds6_addr *root_if;

  PROCESS_BEGIN();

  PROCESS_PAUSE();

  SENSORS_ACTIVATE(button_sensor);

  /* Boot marker: proves the new firmware is the one running in Cooja */
  printf("SINK V3\n");

  PRINTF("UDP server started\n");

#if UIP_CONF_ROUTER
  uip_ip6addr(&ipaddr, UIP_DS6_DEFAULT_PREFIX, 0, 0, 0, 0, 0, 0, 1);
  /* uip_ds6_set_addr_iid(&ipaddr, &uip_lladdr); */
  uip_ds6_addr_add(&ipaddr, 0, ADDR_MANUAL);
  root_if = uip_ds6_addr_lookup(&ipaddr);
  if(root_if != NULL) {
    rpl_dag_t *dag;
    dag = rpl_set_root(RPL_DEFAULT_INSTANCE,(uip_ip6addr_t *)&ipaddr);
    uip_ip6addr(&ipaddr, UIP_DS6_DEFAULT_PREFIX, 0, 0, 0, 0, 0, 0, 0);
    rpl_set_prefix(dag, &ipaddr, 64);
    PRINTF("created a new RPL dag\n");
  } else {
    PRINTF("failed to create a new RPL DAG\n");
  }
#endif /* UIP_CONF_ROUTER */

  print_local_addresses();

  /* The data sink runs with a 100% duty cycle in order to ensure high
     packet reception rates. */
  NETSTACK_RDC.off(1);

  server_conn = udp_new(NULL, UIP_HTONS(UDP_CLIENT_PORT), NULL);
  udp_bind(server_conn, UIP_HTONS(UDP_SERVER_PORT));

  PRINTF("Created a server connection with remote address ");
  PRINT6ADDR(&server_conn->ripaddr);
  PRINTF(" local/remote port %u/%u\n", UIP_HTONS(server_conn->lport),
         UIP_HTONS(server_conn->rport));

  /* 10 s while debugging. Use 60 s for the real measurements. */
  etimer_set(&stat_timer, CLOCK_SECOND * 10);
  while(1) {
    PROCESS_YIELD();
    if(ev == tcpip_event) {
      tcpip_handler();
    } else if (ev == sensors_event && data == &button_sensor) {
      PRINTF("Initiaing global repair\n");
      rpl_repair_root(RPL_DEFAULT_INSTANCE);
    } else if(ev == PROCESS_EVENT_TIMER && data == &stat_timer) {
      sink_stats_print();
      etimer_reset(&stat_timer);
    }
  }

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
