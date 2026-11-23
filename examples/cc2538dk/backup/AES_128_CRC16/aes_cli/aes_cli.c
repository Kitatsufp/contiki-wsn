#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/time.h>

#include "aes_test.h"
#include "aes_cli.h"
#include "util.h"
#include "aes.h"

static  int     rev_bytes;
static  struct  sockaddr_in6 rev_sin6;
static  int     rev_sin6len;
static  char    rev_buffer[MAXBUF];
static  int     port;
static  char    dst_ipv6addr[50];
static  char    str_port[5];
static  char    cmd[10];

static uint8_t data_str       [2*MAX_LEN_CBC];
static uint8_t data_byte      [2*MAX_LEN_CBC];


static uint8_t data_decrypted [MAX_LEN_CBC];

static uint8_t data_encrypted [MAX_LEN_CBC];
static uint8_t data_reply     [MAX_LEN_CBC];

static uint8_t key[16] = { 0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, \
                           0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c };

static uint8_t iv[16]  = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, \
                           0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f };

static unsigned short crc16_check;

static  char *p;

/* delay measurement */
struct timeval t0;
struct timeval t1;
float elapsed;

/*prototype definition */

/*--------------------------------------------------------------------------------------*/
float timedifference_msec(struct timeval t0, struct timeval t1){
    return (t1.tv_sec - t0.tv_sec) * 1000.0f + (t1.tv_usec - t0.tv_usec) / 1000.0f;
}
/*--------------------------------------------------------------------------------------*/

int main(int argc, char* argv[]) {
  int c;
  int sock;
  int status, i, len_byte;
  struct addrinfo sainfo, *psinfo;
  struct sockaddr_in6 sin6;
  int sin6len;
  uint8_t data_received  [MAXBUF];
  static const char filename[] = "data.txt";
  FILE *file; 

  sin6len = sizeof(struct sockaddr_in6);

	port = 3000;
	
  if(argc != 4) {
    printf("example: ./aes_cli aaaa::....  3000  AES_TEST\n"), exit(1);
	}  else if (argc==4) {
    sprintf(dst_ipv6addr,"%s",argv[1]);      
    strcpy(str_port,argv[2]);
    strcpy(cmd,argv[3]);  
    port = atoi(str_port);

    /* REQ-TYPE*/
    if (strcmp(cmd,AES_TEST)==0) {
      printf("AES TEST ....\n");
    }  else {
      printf("Unknown cmd \n");
      exit(1);
    }  
	}

  sock = socket(PF_INET6, SOCK_DGRAM,0);

  memset(&sin6, 0, sizeof(struct sockaddr_in6));
  sin6.sin6_port = htons(port);
  sin6.sin6_family = AF_INET6;
  sin6.sin6_addr = in6addr_any;

  status = bind(sock, (struct sockaddr *)&sin6, sin6len);

  if(-1 == status)
    perror("bind"), exit(1);

  memset(&sainfo, 0, sizeof(struct addrinfo));
  memset(&sin6, 0, sin6len);

  sainfo.ai_flags = 0;
  sainfo.ai_family = PF_INET6;
  sainfo.ai_socktype = SOCK_DGRAM;
  sainfo.ai_protocol = IPPROTO_UDP;

  i = 0;
  file= fopen ( filename, "r" );
  if ( file != NULL ) {
    while((c = fgetc(file)) != EOF) {
      if (c != 0x0a){
        data_str[i] = c;
        i++;
      }
    }
  }  else {
    perror(filename);
    exit(1);
  }

  len_byte = convert_str2array(data_str, data_byte, MAX_CMD_DATA_LEN);
  if (len_byte == MAX_CMD_DATA_LEN){
    status = getaddrinfo(dst_ipv6addr, str_port, &sainfo, &psinfo);
    crc16_check = gen_crc16(data_byte, MAX_CMD_DATA_LEN);
    data_byte [MAX_CMD_DATA_LEN - 2] = (char) (crc16_check >> 8) & 0xff;
    data_byte [MAX_CMD_DATA_LEN - 1] = (char) crc16_check & 0xff;

    encrypt_cbc(data_encrypted, data_byte, key, iv);

    gettimeofday(&t0, 0);


    status = sendto(sock, data_encrypted, MAX_CMD_DATA_LEN, 0,(struct sockaddr *)psinfo->ai_addr, sin6len);
    printf("\nSend REQUEST (%d bytes) to [%s]:%s\nDone .... ",status, dst_ipv6addr,str_port);
  }  else {
    printf("error length data\n");
    exit(1);
  }


  /*wait for a reply */
	rev_bytes = recvfrom((int)sock, rev_buffer, MAXBUF, 0,(struct sockaddr *)(&rev_sin6), (socklen_t *) &rev_sin6len);
	if (rev_bytes<0) {
    perror("Problem in recvfrom \n");
    exit(1);
  }  else if (rev_bytes == 64) {
    printf("Got REPLY (%d bytes):\n",rev_bytes); 

    decrypt_cbc(data_decrypted, rev_buffer, key, iv);
    crc16_check = gen_crc16(data_decrypted, MAX_CMD_DATA_LEN);
    printf("CRC16 = 0x%04x\n", crc16_check);
      
    if (check_crc16(data_decrypted, MAX_CMD_DATA_LEN)) {
      printf("CRC true\n");
    }  else {
    printf("CRC fail\n");
    exit(1);
    }
    phex_64(data_decrypted);

    gettimeofday(&t1, 0);
    elapsed = timedifference_msec(t0, t1);
    printf("Cmd execution delay %.2f (ms) \n\n", elapsed);
  }  else printf("error rev_byte\n");

  shutdown(sock, 2);
  close(sock); 

   // free memory
  freeaddrinfo(psinfo);
  psinfo = NULL;
  return 0;
}
