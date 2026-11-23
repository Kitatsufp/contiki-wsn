#ifndef _UTIL_H_
#define _UTIL_H_

/*---------------------------------------------------------------------------*/
/*************************|  UTIL  (START) |                                 */
/*---------------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#define CBC 1
#define ECB 0

#if ECB
#define MAX_LEN_ECB 16
#define MAX_PAYLOAD_LEN MAX_LEN_ECB
#endif

#if CBC
#define MAX_LEN_CBC 64
#define MAX_PAYLOAD_LEN MAX_LEN_CBC
#endif

#include "aes.h"

#define POLY 0x8408
/*
//                                      16   12   5
// this is the CCITT CRC 16 polynomial X  + X  + X  + 1.
// This works out to be 0x1021, but the way the algorithm works
// lets us use 0x8408 (the reverse of the bit pattern).  The high
// bit is always assumed to be set, thus we only use 16 bits to
// represent the 17 bit value.
*/
typedef enum {false=0, true=1} bool;

// LENGTH = LENGTH REAL DATA + 2BYTE CRC
unsigned short gen_crc16(uint8_t *data_p, unsigned short length); 
bool check_crc16(uint8_t *data_p, unsigned short length);

void phex_16(uint8_t* data_16);
void phex_64(uint8_t* data_64);

void decrypt_cbc(uint8_t* data_decrypted, uint8_t* data_encrypted, const uint8_t* key, const uint8_t* iv);
void encrypt_cbc(uint8_t* data_encrypted, uint8_t* data, const uint8_t* key, const uint8_t* iv);

/*---------------------------------------------------------------------------*/
/*                        |   UTIL  (END) |***********************************/
/*---------------------------------------------------------------------------*/

#endif /*_UTIL_H_*/