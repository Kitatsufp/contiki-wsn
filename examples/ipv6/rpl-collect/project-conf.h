#ifndef PROJECT_CONF_H_
#define PROJECT_CONF_H_

#ifndef WITH_NON_STORING
#define WITH_NON_STORING 0 /* Set this to run with non-storing mode */
#endif /* WITH_NON_STORING */

#undef NBR_TABLE_CONF_MAX_NEIGHBORS
#undef UIP_CONF_MAX_ROUTES

#ifdef TEST_MORE_ROUTES
/* configure number of neighbors and routes */
#define NBR_TABLE_CONF_MAX_NEIGHBORS     16
#define UIP_CONF_MAX_ROUTES   30
#else
/* configure number of neighbors and routes */
#define NBR_TABLE_CONF_MAX_NEIGHBORS     16
#define UIP_CONF_MAX_ROUTES   20
#endif /* TEST_MORE_ROUTES */

#undef NETSTACK_CONF_RDC
#define NETSTACK_CONF_RDC   contikimac_driver
#undef NULLRDC_CONF_802154_AUTOACK
#define NULLRDC_CONF_802154_AUTOACK       1

/* Define as minutes */
#define RPL_CONF_DEFAULT_LIFETIME_UNIT   60

/* 10 minutes lifetime of routes */
#define RPL_CONF_DEFAULT_LIFETIME        10

#define RPL_CONF_DEFAULT_ROUTE_INFINITE_LIFETIME 1

/* Save some ROM */
#undef UIP_CONF_TCP
#define UIP_CONF_TCP                   0

#undef SICSLOWPAN_CONF_FRAG
#define SICSLOWPAN_CONF_FRAG           0

#if WITH_NON_STORING
#undef RPL_NS_CONF_LINK_NUM
#define RPL_NS_CONF_LINK_NUM 40 /* Number of links maintained at the root. Can be set to 0 at non-root nodes. */
#undef UIP_CONF_MAX_ROUTES
#define UIP_CONF_MAX_ROUTES 0 /* No need for routes */
#undef RPL_CONF_MOP
#define RPL_CONF_MOP RPL_MOP_NON_STORING /* Mode of operation*/
#endif /* WITH_NON_STORING */

/*----------------------ADDING PART-----------------*/
/* Bat bo dem thong ke IP (de doc uip_stat.ip.forwarded) */
#undef UIP_CONF_STATISTICS
#define UIP_CONF_STATISTICS 1

/* Ep dung CSMA lam MAC (csma_queue_length() can csma_driver) */
#undef NETSTACK_CONF_MAC
#define NETSTACK_CONF_MAC csma_driver

/* Gioi han hang doi cua CSMA */
#undef CSMA_CONF_MAX_NEIGHBOR_QUEUES
#define CSMA_CONF_MAX_NEIGHBOR_QUEUES 4

#undef QUEUEBUF_CONF_NUM
#define QUEUEBUF_CONF_NUM 6

#undef CSMA_CONF_MAX_PACKET_PER_NEIGHBOR
#define CSMA_CONF_MAX_PACKET_PER_NEIGHBOR 6
/*--------------------------------------------------*/

#endif /* PROJECT_CONF_H_ */
