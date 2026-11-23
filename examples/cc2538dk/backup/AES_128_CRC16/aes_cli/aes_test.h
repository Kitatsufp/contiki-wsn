#ifndef AES_H_
#define AES_H_

//#define IEEE802154_CONF_PANID		0xDCBA
#define AES_PAN_ID	 IEEE802154_CONF_PANID


enum {
	AES_NORMAL_PORT			= 3000,
	AES_EMERGENCY_PORT		= 3001,
};

/*---------------------------------------------------------------------------*/
/* This is the UDP port used to receive data */
/* Response will be echoed back to DST port */
#define UDP_SERVER_LISTEN_PORT   	SLS_NORMAL_PORT
#define UDP_CLIENT_SEND_PORT   		SLS_EMERGENCY_PORT

//redefine leds
#define BLUE		LEDS_ORANGE
#define RED			LEDS_GREEN
#define GREEN		LEDS_BLUE


#define MAX_CMD_DATA_LEN	64	
#define MAXBUF				120


union float_byte_convert {
    float f;
    uint8_t bytes[4];
};

#endif /* AES_H_ */