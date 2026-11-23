#ifndef	RF_PACKET_H_
#define	RF_PACKET_H_

struct RF {
  uint8_t menu;
  uint8_t channel;
  uint8_t power;
  uint8_t size;
  uint8_t rate_l;
  uint8_t rate;
  uint8_t run;
  uint8_t mode;
  unsigned int num;
  uint8_t power_val;
  unsigned int recv_num;
  unsigned int success_num;
  unsigned int cur_num;
  unsigned int lost_num;
  uint8_t last_status;
  uint8_t seq;
  int8_t rssi;
};

enum {
  UNDEFINE,
  SENDER,
  RECEIVER,
};

extern struct RF rf;

#endif	/* RF_PACKET_H_ */