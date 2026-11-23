/*
 * Copyright (c) 2010, Loughborough University - Computer Science
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
/**
 * \file
 *         Definition of a fake RDC driver to be used with passive
 *         examples. The sniffer will never send packets and it will never
 *         push incoming packets up the stack. We do this by defining this
 *         driver as our RDC. We then drop everything
 *
 * \author
 *         George Oikonomou - <oikonomou@users.sourceforge.net>
 */
#include "net/mac/mac.h"
#include "net/mac/rdc.h"
#include "net/packetbuf.h"
#include "dev/leds.h"
#include "rf-packet.h"
//#include "dev/console.h"
#include <stdio.h>

#define FRAME_BEACON	0
#define FRAME_DATA		1
#define FRAME_ACK		2
#define FRAME_CMD		3

//#if DBG_CONF_USB
//#include "usb/usb-serial.h"
//#define write_byte(b)	usb_serial_writeb(b)
//#define flush()			usb_serial_flush()
//#else
//#include "dev/uart.h"
//#define write_byte(b)	uart_write_byte(DBG_CONF_UART, b)
//#define flush()
//#endif


//static unsigned char icon[8] = {
//  Z1_ZigBee_ON, R5_24G_ON, R4_Power_99_ON, K9_dB_ON, K10_m1_ON
//};
/*---------------------------------------------------------------------------*/
static void
send(mac_callback_t sent, void *ptr)
{
  if(sent) {
    sent(ptr, MAC_TX_OK, 1);
  }
}
/*---------------------------------------------------------------------------*/
static void
send_list(mac_callback_t sent, void *ptr, struct rdc_buf_list *list)
{
  if(sent) {
    sent(ptr, MAC_TX_OK, 1);
  }
}
/*---------------------------------------------------------------------------*/
static void
input(void)
{
  // const uint8_t mhrlen = 9;   /* mac 层帧头长度9字节 */
  // uint16_t datalen = packetbuf_datalen() - mhrlen;
  int8_t rssi = (int8_t) packetbuf_attr(PACKETBUF_ATTR_RSSI);
  uint8_t *ptr = packetbuf_hdrptr();
  uint16_t recv_seq;
  if(rf.mode == UNDEFINE)
    return;


  switch (*ptr & 0x7) {
  case FRAME_BEACON:
    break;
  case FRAME_DATA:
    if(rf.mode == RECEIVER) {
      /* 在收到第一个包之前,不算丢包情况 */
      if(rf.recv_num++) {
        recv_seq = *(ptr + 2);
        rf.lost_num +=
          recv_seq >
          rf.seq ? (uint8_t) (recv_seq - rf.seq - 1) : (uint8_t) (recv_seq +
                                                                  256 -
                                                                  rf.seq - 1);
      }
      rf.seq = *(ptr + 2);
//	  go_to_x_y(1, 6);	/*光标在第6行第1列*/
      printf("lost %6d packets.\n", rf.lost_num);
      leds_toggle(LEDS_BLUE);
    }
    break;
  case FRAME_ACK:
    if(rf.mode == SENDER) {
//	  go_to_x_y(1, 7);	/*光标在第6行第1列*/
      printf("success: %6d packets\n", ++rf.success_num);
      leds_toggle(LEDS_BLUE);
    }
    break;
  case FRAME_CMD:
    break;
  }

  printf("rssi = %ddB\n", rssi);

}
/*---------------------------------------------------------------------------*/
static int
on(void)
{
  return 1;
}
/*---------------------------------------------------------------------------*/
static int
off(int keep_radio_on)
{
  return keep_radio_on;
}
/*---------------------------------------------------------------------------*/
static unsigned short
cca(void)
{
  return 0;
}
/*---------------------------------------------------------------------------*/
static void
init(void)
{
}
/*---------------------------------------------------------------------------*/
const struct rdc_driver stub_rdc_driver = {
  "stub-rdc",
  init,
  send,
  send_list,
  input,
  on,
  off,
  cca,
};
/*---------------------------------------------------------------------------*/
