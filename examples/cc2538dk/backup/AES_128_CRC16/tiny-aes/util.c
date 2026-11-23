#include "util.h"
#include "aes.h"
/*---------------------------------------------------------------------------*/
/*************************| AES128 (START) |                                 */
/*---------------------------------------------------------------------------*/
unsigned short 
gen_crc16(uint8_t *data_p, unsigned short length)
{
    unsigned char i;
    unsigned int data;
    unsigned int crc = 0xffff;
    length -= 2;

    if (length == 0)
          return (~crc);

    do
    {
          for (i=0, data=(unsigned int)0xff & *data_p++;
               i < 8; 
               i++, data >>= 1)
          {
                if ((crc & 0x0001) ^ (data & 0x0001))
                      crc = (crc >> 1) ^ POLY;
                else  crc >>= 1;
          }
    } while (--length);

    crc = ~crc;
    data = crc;
    crc = (crc << 8) | (data >> 8 & 0xff);

    return (crc);
}
/*---------------------------------------------------------------------------*/
bool  
check_crc16(uint8_t *data_p, unsigned short length)
{
    unsigned short crc16_check;
    crc16_check = gen_crc16(data_p, length);

    if ((data_p[length - 2] == (uint8_t) (crc16_check>>8)) 
              && (data_p[length - 1] == (uint8_t) (crc16_check & 0x00ff))) {
        //printf("CRC true\n");
        return true;
    } else  {
        return false;
          //printf("CRC fail\n");
    }

}
/*---------------------------------------------------------------------------*/
void 
phex_16(uint8_t* data_16) 
{ // in chuoi hex 16 bytes
    unsigned char i;
    for(i = 0; i < 16; ++i)
        printf("%.2x ", data_16[i]);
    printf("\n");
}
/*---------------------------------------------------------------------------*/
void 
phex_64(uint8_t* data_64) 
{ // in chuoi hex 64 bytes
    unsigned char i;
    for(i = 0; i < 4; ++i) 
        phex_16(data_64 + (i*16));
    printf("\n");
}
/*---------------------------------------------------------------------------*/
// ma hoa 64 bytes
void 
encrypt_cbc(uint8_t* data_encrypted, uint8_t* data,const uint8_t* key, const uint8_t* iv) 
{ // ma hoa 64 bytes
    uint8_t data_temp[MAX_LEN_CBC];

    memcpy(data_temp, data, MAX_LEN_CBC);
    printf("\nData: \n");
    phex_64(data);

    AES128_CBC_encrypt_buffer(data_encrypted, data, 64, key, iv);

    printf("\nData encrypted: \n");
    phex_64(data_encrypted);
}
/*---------------------------------------------------------------------------*/
// giai ma 64 bytes
void 
decrypt_cbc(uint8_t* data_decrypted, uint8_t* data_encrypted, const uint8_t* key, const uint8_t* iv) 
{
  uint8_t data_temp[MAX_LEN_CBC];

  memcpy(data_temp, data_encrypted, MAX_LEN_CBC);
	printf("\nData encrypted: \n");
  phex_64(data_encrypted);

  AES128_CBC_decrypt_buffer(data_decrypted+0,  data_encrypted+0,  16, key, iv);
  AES128_CBC_decrypt_buffer(data_decrypted+16, data_encrypted+16, 16, 0, 0);
  AES128_CBC_decrypt_buffer(data_decrypted+32, data_encrypted+32, 16, 0, 0);
  AES128_CBC_decrypt_buffer(data_decrypted+48, data_encrypted+48, 16, 0, 0);

  printf("Data decrypt: \n");
	phex_64(data_decrypted);
}
/*---------------------------------------------------------------------------*/
/*                        |  AES128 (END) |***********************************/
/*---------------------------------------------------------------------------*/