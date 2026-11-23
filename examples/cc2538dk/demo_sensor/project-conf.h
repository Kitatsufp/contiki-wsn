/*
 * Copyright (c) 2015, Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#ifndef PROJECT_CONF_H_
#define PROJECT_CONF_H_


#undef 	IEEE802154_CONF_PANID
#define IEEE802154_CONF_PANID		0xCAFE


#ifndef WITH_NON_STORING
#define WITH_NON_STORING 	1 /* Set this to run with non-storing mode */
#endif /* WITH_NON_STORING */

#if WITH_NON_STORING
#undef 	RPL_NS_CONF_LINK_NUM
#define RPL_NS_CONF_LINK_NUM 		40 /* Number of links maintained at the root. Can be set to 0 at non-root nodes. */
#undef 	UIP_CONF_MAX_ROUTES
#define UIP_CONF_MAX_ROUTES 		0 /* No need for routes */
#undef 	RPL_CONF_MOP
#define RPL_CONF_MOP RPL_MOP_NON_STORING /* Mode of operation*/
#endif /* WITH_NON_STORING */


#undef NBR_TABLE_CONF_MAX_NEIGHBORS
#undef UIP_CONF_MAX_ROUTES

#ifdef TEST_MORE_ROUTES
/* configure number of neighbors and routes */
#define NBR_TABLE_CONF_MAX_NEIGHBORS     10
#define UIP_CONF_MAX_ROUTES   40
#else
/* configure number of neighbors and routes */
#define NBR_TABLE_CONF_MAX_NEIGHBORS     10
#define UIP_CONF_MAX_ROUTES   10
#endif /* TEST_MORE_ROUTES */



/* define RDC and MAC here */
#undef 	NETSTACK_CONF_MAC
#define NETSTACK_CONF_MAC     	csma_driver			// nullmac_driver, csma_driver

#undef 	NETSTACK_CONF_RDC
#define NETSTACK_CONF_RDC     	nullrdc_driver 	//nullrdc_driver, cxmac_driver, contikimac_driver


#define NETSTACK_CONF_RDC_CHANNEL_CHECK_RATE 	64

#undef 	NULLRDC_CONF_802154_AUTOACK
#define NULLRDC_CONF_802154_AUTOACK       1


/* Define as minutes */
//#define RPL_CONF_DEFAULT_LIFETIME_UNIT   60

/* 10 minutes lifetime of routes */
//#define RPL_CONF_DEFAULT_LIFETIME        10

//#define RPL_CONF_DEFAULT_ROUTE_INFINITE_LIFETIME 	1


/* son define */
#ifndef UIP_CONF_ROUTER
#define UIP_CONF_ROUTER  			1
#endif

#define UIP_CONF_IPV6_RPL  			1
/* ND and Routing */
#define UIP_CONF_ND6_SEND_RA        0
#define UIP_CONF_IP_FORWARD         0


/* Low Power Mode */
#define LPM_CONF_ENABLE       		0		/**< Set to 0 to disable LPM entirely */
#define LPM_CONF_MAX_PM       		1


//if using CC2592 PA, set this to TRUE
#define CC2538DK_WITH_CC2592 		0

/* RF parameters define*/
#define RF_CHANNEL 					26 		/**< Default RF channel */
#define CC2538_RF_CONF_TX_POWER		0xFF	// +7dBm
#define CC2538_RF_CONF_AUTOACK 		1 
//#define CC2538_RF_CONF_CHANNEL    26


//#ifdef  DEBUG
//#undef 	DEBUG
#define DEBUG 	1					//	DEBUG_NONE=0;	 DEBUG_PRINT=1
//#endif

#ifndef STARTUP_CONF_VERBOSE
#define STARTUP_CONF_VERBOSE        1 /**< Set to 0 to decrease startup verbosity */
#endif


#define PRINT_SENSOR		1


#define SLS_USING_CC2538DK

#define CC2538DK_HAS_SENSOR  	1

#if (CC2538DK_HAS_SENSOR==1)
#define CC2538DK_HAS_SHIELD
#endif


/* This data structure is used to store the packet content (payload) */
struct net_struct_t {
	uint8_t			channel;	
	int8_t			rssi;
	int8_t			lqi;
	int8_t			tx_power;
	uint16_t		panid;
	uint16_t		node_addr;
};

/*---------------------------------------------------------------------------*/
//	used in the future
struct env_struct_t {
	uint16_t	id;			/*0011xxxx xxxxxxxx */
	uint8_t		status;
	/* data of device */
	uint16_t	temp;
	uint16_t	light;
	uint16_t	pressure;
	uint16_t	humidity;
	uint16_t	pir;
	uint16_t	rain;
	
} __attribute__((packed));



typedef struct net_struct_t		net_struct_t;
typedef struct env_struct_t		env_struct_t;
	


#endif
