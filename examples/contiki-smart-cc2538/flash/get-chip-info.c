/*
 *
 */

#include "contiki.h"
#include "dev/flash.h"
#include "dev/flash_wo_cca.h"

#include <stdio.h>              /* For printf() */

/*---------------------------------------------------------------------------*/
PROCESS(eeprom_test_process, "EEPROM Test Process");
AUTOSTART_PROCESSES(&eeprom_test_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(eeprom_test_process, ev, data)
{
  static uint32_t cfg;

  PROCESS_BEGIN();

  cfg = REG(FLASH_CTRL_DIECFG0);
  
  printf("chipid = 0x%X\n", (cfg & FLASH_CTRL_DIECFG0_CHIPID_M) >> 16);
  if(cfg & FLASH_CTRL_DIECFG0_CLK_SEL_GATE_EN_N_M)
	printf("clk sel gata enable\n");
  else
	printf("clk sel gata disable\n");
  printf("sram size = 0x%X\n",(cfg & FLASH_CTRL_DIECFG0_SRAM_SIZE_M) >> FLASH_CTRL_DIECFG0_FLASH_SIZE_S );
  printf("flash size = 0x%X\n",(cfg & FLASH_CTRL_DIECFG0_FLASH_SIZE_M) >> FLASH_CTRL_DIECFG0_FLASH_SIZE_S);
  if(cfg & FLASH_CTRL_DIECFG0_USB_ENABLE_M)
	printf("USB enable\n");
  else
	printf("USB disable\n");
  if(cfg & FLASH_CTRL_DIECFG0_MASS_ERASE_ENABLE_M)
	  printf("MASS ERASE ENABLE\n");
  if(cfg & FLASH_CTRL_DIECFG0_LOCK_IP_M)
	  printf("LOCK IP\n");
  
  cfg = REG(FLASH_CTRL_DIECFG1);
  if(cfg & FLASH_CTRL_DIECFG1_I2C_EN_M)
	  printf("I2C enable\n");
   if(cfg & FLASH_CTRL_DIECFG1_UART1_EN_M)
	  printf("UART1 enable\n");
  if(cfg & FLASH_CTRL_DIECFG1_UART1_EN_M)
	  printf("UART0 enable\n");
  if(cfg & FLASH_CTRL_DIECFG1_SSI1_EN_M)
	  printf("SSI1 enable\n");
  if(cfg & FLASH_CTRL_DIECFG1_SSI0_EN_M)
	  printf("SSI0 enable\n");
  if(cfg & FLASH_CTRL_DIECFG1_GPTM3_EN_M)
	  printf("GPTM3 enable\n");
  if(cfg & FLASH_CTRL_DIECFG1_GPTM2_EN_M)
	  printf("GPTM2 enable\n");
  if(cfg & FLASH_CTRL_DIECFG1_GPTM1_EN_M)
	  printf("GPTM1 enable\n");
  if(cfg & FLASH_CTRL_DIECFG1_GPTM0_EN_M)
	  printf("GPTM0 enable\n");
  cfg = REG(FLASH_CTRL_DIECFG2);
  if(cfg & FLASH_CTRL_DIECFG2_RF_CORE_EN_M)
	  printf("RF enable\n");
  if(cfg & FLASH_CTRL_DIECFG2_AES_EN_M)
	  printf("AES enable\n");
  if(cfg & FLASH_CTRL_DIECFG2_PKA_EN_M)
	  printf("PKA enable\n");
  printf("MAJOR: 0x%X\n", (cfg & FLASH_CTRL_DIECFG2_DIE_MAJOR_REVISION_M) >> FLASH_CTRL_DIECFG2_DIE_MAJOR_REVISION_S);
  printf("MINOR: 0x%X\n", (cfg & FLASH_CTRL_DIECFG2_DIE_MINOR_REVISION_M) >> FLASH_CTRL_DIECFG2_DIE_MINOR_REVISION_S);
  
  
  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
