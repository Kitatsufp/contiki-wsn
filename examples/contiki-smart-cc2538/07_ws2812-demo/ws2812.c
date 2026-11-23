#include "contiki.h"
#include "clock.h"

#include "dev/leds.h"
#include <stdio.h>


#define SPI_MOSI_PORT GPIO_D_BASE
#define SPI_MOSI_BIT  (1 << 2)
#define SPI_MOSI      SPI_MOSI_PORT, SPI_MOSI_BIT

#ifndef	delay_400ns
#define	delay_400ns()	_NOP()
#endif


static void
gpio_set(int port, int bit)
{
  REG((port | GPIO_DATA) + (bit << 2)) = bit;
}
/*---------------------------------------------------------------------------*/
static void
gpio_reset(int port, int bit)
{
  REG((port | GPIO_DATA) + (bit << 2)) = 0;
}
/*---------------------------------------------------------------------------*/
static int
gpio_get(int port, int bit)
{
  return REG((port | GPIO_DATA) + (bit << 2));
}

/*---------------------------------------------------------------------------*/
static void
delay_400n(void)
{
  int num=0;
  int i=0;
  for(i=0;i<2;i++){
	asm("nop");
	}


}
delay_800n(void)
{
  int num=0;
  int i=0;
  for(i=0;i<4;i++){
	asm("nop");
	}

}
/*---------------------------------------------------------------------------*/
void 
ws2812_init(void)
{
      GPIO_SET_OUTPUT(SPI_MOSI_PORT, SPI_MOSI_BIT);
      GPIO_SET_PIN(SPI_MOSI_PORT, SPI_MOSI_BIT);
}

void 
ws2812_reset(void)
{
  int i;
//      GPIO_SET_OUTPUT(SPI_MOSI_PORT, SPI_MOSI_BIT);
//      GPIO_CLR_PIN(SPI_MOSI_PORT, SPI_MOSI_BIT);
//      clock_delay_usec(100);
//      GPIO_SET_PIN(SPI_MOSI_PORT, SPI_MOSI_BIT);
	gpio_reset(SPI_MOSI);
	for(i = 0;i < 1000; i++){
	  asm("nop");
	}
	gpio_set(SPI_MOSI);
}

uint8_t
ws2812_write(uint8_t output)
{
  int i;
  uint8_t input;

  input = 0;

  for(i = 0; i < 8; i++) {

    /* Write data on MOSI pin */
    if(output & 0x80) {
      //gpio_set(SPI_MOSI);
      delay_800n();
      gpio_reset(SPI_MOSI);
      delay_400n();
      gpio_set(SPI_MOSI);
    } else {
      //gpio_set(SPI_MOSI);
      //delay_400n();
      gpio_reset(SPI_MOSI);
      delay_800n();
      delay_800n();
      gpio_set(SPI_MOSI);
    }
    output <<= 1;

  }
//     gpio_set(SPI_MOSI);
  return 1;
}

