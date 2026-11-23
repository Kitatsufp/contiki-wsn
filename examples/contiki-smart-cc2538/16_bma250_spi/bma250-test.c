
/**
 * \file
 *         A very simple Contiki application showing how Contiki programs look
 * \author
 *         Adam Dunkels <adam@sics.se>
 */

#include "contiki.h"
#include "cfs/cfs.h"

#include "sys/etimer.h"
#include "enc28j60-arch.h"

#include <stdio.h> /* For printf() */


//#define SPI_SEL_PORT_BASE 	GPIO_PORT_TO_BASE(SPI_SEL_PORT)  //CS RF2.10 PD5
//#define SPI_SEL_PIN_MASK 	GPIO_PIN_MASK(SPI_SEL_PIN)

#define ACC_PWR_PORT    GPIO_D_NUM
#define ACC_PWR_PIN     4


#define ACC_PWR_PORT_BASE 	GPIO_PORT_TO_BASE(ACC_PWR_PORT)  //CS RF2.8  PD4
#define ACC_PWR_PIN_MASK 	GPIO_PIN_MASK(ACC_PWR_PIN)  

static struct etimer et;

/*---------------------------------------------------------------------------*/
PROCESS(hello_world_process, "Hello world process");
AUTOSTART_PROCESSES(&hello_world_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(hello_world_process, ev, data)
{

  int8_t d[6]={0};
  int16_t x,y,z;

  	PROCESS_BEGIN();
	etimer_set(&et, 0.1*CLOCK_SECOND);

	
   while(1){
	PROCESS_YIELD();
	if(ev == PROCESS_EVENT_TIMER){
	printf("spi init\n");
	
//	GPIO_SET_OUTPUT(SPI_SEL_PORT_BASE,SPI_SEL_PIN_MASK);
	GPIO_SET_OUTPUT(ACC_PWR_PORT_BASE,ACC_PWR_PIN_MASK);
//	GPIO_CLR_PIN(ACC_PWR_PORT_BASE,ACC_PWR_PIN_MASK);
//	GPIO_SET_PIN(SPI_SEL_PORT_BASE,SPI_SEL_PIN_MASK);
	clock_delay_usec(1);
//	GPIO_CLR_PIN(SPI_SEL_PORT_BASE,SPI_SEL_PIN_MASK);
	GPIO_SET_PIN(ACC_PWR_PORT_BASE,ACC_PWR_PIN_MASK);
	clock_delay_usec(5);
	
	enc28j60_arch_spi_init();
	
	enc28j60_arch_spi_select();
//	spix_cs_init(SPI_SEL_PORT,SPI_SEL_PIN); // that one to activate SPI
//	spix_cs_init(ACC_PWR_PORT,ACC_PWR_PIN); // that one to power on the acc
	printf("spi cs enable\n");
	
	clock_delay_usec(2);

	// On doit mettre à la masse le SS pour indiquer le début de la
	//communication avec l'accéléromètre
	//GPIO_CLR_PIN(SPI_SEL_PORT_BASE,SPI_SEL_PIN_MASK);
	// 0x80 : lecture, 0x02 : adresse du registre
	enc28j60_arch_spi_write(0x80|0x02);
	clock_delay_usec(5);
	//SPI_FLUSH();
	d[0]=enc28j60_arch_spi_read();
	d[1]=enc28j60_arch_spi_read();
	d[2]=enc28j60_arch_spi_read();
	d[3]=enc28j60_arch_spi_read();
	d[4]=enc28j60_arch_spi_read();
	d[5]=enc28j60_arch_spi_read();
	printf("read finished\n");
	// On indique la fin de la communication en mettant à 1 le SS
//	GPIO_SET_PIN(SPI_SEL_PORT_BASE,SPI_SEL_PIN_MASK);
	
	x = (((int16_t)(d[1]) << 2) | ((d[0] >> 6) & 0x03));
	y = (((int16_t)(d[3]) << 2) | ((d[2] >> 6) & 0x03));
	z = (((int16_t)(d[5]) << 2) | ((d[4] >> 6) & 0x03));
	
	printf("x-axis : %d\ny-axis : %d\nz-axis : %d\n",x,y,z);
	clock_delay_usec(10);

	 etimer_set(&et, CLOCK_SECOND);
	}
}	


  
  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
