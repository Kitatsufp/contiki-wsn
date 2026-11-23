/*
 * Copyright (c) 2013, Thingsquare, http://www.thingsquare.com/.
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
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
/*
 *  \file
 *      flash.c
 *   \author
 *      Marcus Lunden <marcus@thingsquare.com>>
 *   \desc
 *      Flash driver for CC2538
 */

#include <stdio.h>
#include <stdint.h>
#include "contiki.h"
#include "reg.h"
#include "flash_n.h"
#include "cpu.h"
#include "dev/watchdog.h"
#include "converters.h"
/*---------------------------------------------------------------------------*/
/*
 * erase a flash page; a page (2048 bytes) is the minimum erasable unit
 * in CC2538. The code for erasing pages can reside in flash; the CPU is stalled
 * while the flash controller is performing the erase, then resuming at the next
 * instruction. A page erase takes 20 ms and ca 20 mA. Any long-running
 * application must pat the watchdog between calls.
 */
void
flash_clear(unsigned long addr)
{
  uint32_t page;

  /* sanity check address; refer to the datasheet */
  if(addr >= FLASH_END || addr < FLASH_START) {
    /* invalid address */
    printf("*** ErasePage address error. FST: %lu addr: %lu FEND: %lu \n", (unsigned long)FLASH_START, addr, (unsigned long)FLASH_END);
    return;
  }

  /* find the proper page to erase */
  page = (addr - FLASH_START) / FLASH_PAGE_SIZE;    /* page is in range 0..255  */
  //printf("PE @ 0x%04lx (page %lu", addr, page);
  /*
   * XXX NB the data sheet states two different locations for the address field, 
   * shifted 9 vs 11 places left. Empirically, found that 11 places is correct.
   */
  page = page << 11;
  //printf(" - 0x%lx)\n", page);

  /* Erase one flash page: erase, wait until done, clean up */
  INTERRUPTS_DISABLE();
  while(REG(FLASH_CTRL_FCTL) & FLASH_BITFIELD_BUSY);

  REG(FLASH_CTRL_FADDR) = page;
  REG(FLASH_CTRL_FCTL) |= FLASH_BITFIELD_ERASE;
  while(REG(FLASH_CTRL_FCTL) & FLASH_BITFIELD_BUSY);

  INTERRUPTS_ENABLE();
}
/*---------------------------------------------------------------------------*/
/*
 * Write a word (32-bits) to an address in flash. Assumes location is cleared.
 * A word/32-bits is the minimum writeable unit in CC2538.
 * 
 * Note that on the CC2538 the watchdog cannot be stopped once
 * enabled, thus it is the responibility of the calling application to periodically
 * pat the watchdog (watchdog_periodic()) as WDT will reset the CPU otherwise.
 */

/* if in data-section (ie RAM) this function cannot call functions that are not
  in RAM while the flash-controller is busy (but functions in RAM works fine) */
#define PUT_IN_DATA_SECTION     0

#if PUT_IN_DATA_SECTION
__attribute__((section(".data")))
#endif

void
flash_write(unsigned long addr32, uint32_t data32)
{	
  uint32_t temp32;
  
  if ((addr32 & 0x3) != 0) {
	printf("error: 地址需要4字节对齐\n");
	return;
  }
	 
  /* sanity check address; refer to the datasheet */
  if((addr32 + FLASH_MIN_WRITE) >= FLASH_END || addr32 < FLASH_START) {
#if PUT_IN_DATA == 0
    printf("*** WW address error. FST: %lu addr: %lu FEND: %lu \n",
           (unsigned long)FLASH_START, addr32, (unsigned long)FLASH_END);
#endif
    /* invalid address */
    return;
  }
  
  // if(flash_read(addr32) != 0xFFFFFFFF) {
	// printf("error 请先擦除\n");
	// return;
  // }
    
//  temp32 = Le32(data32);				/* 转换字节序 定义在contiki-master\cpu\cc2538\converters.h*/
  temp32 = data32;


  /* check that the flash controller is not currently in use */
  while(REG(FLASH_CTRL_FCTL) & FLASH_BITFIELD_BUSY);

  INTERRUPTS_DISABLE();

  /*  The flash-write sequence algorithm is as follows:*/
  /*  1. Set FADDR to the start address.*/
  REG(FLASH_CTRL_FADDR) = addr32;

  /*  2. Set FCTL.WRITE to 1. This starts the write-sequence state machine.*/
  REG(FLASH_CTRL_FCTL) |= FLASH_BITFIELD_WRITE;

  /*  3. Write the 32-bit data to FWDATA (since the last time FCTL.FULL became 0,*/
  /*  if not first iteration). FCTL.FULL goes high after writing to FWDATA*/
  REG(FLASH_CTRL_FWDATA) = temp32;

  /*  4. Wait until FCTL.FULL goes low. (The flash controller has started programming*/
  /*  the 4 bytes written in step 3 and is ready to buffer the next 4 bytes).*/
  while(REG(FLASH_CTRL_FCTL) & FLASH_BITFIELD_FULL);

  INTERRUPTS_ENABLE();
}
/*---------------------------------------------------------------------------*/
void
flash_write_single_byte(unsigned long addr, uint8_t byte)
{
  uint32_t data32, mask;
  unsigned long addr32;
  int i;

  /* sanity check address; refer to the datasheet */
  if((addr + FLASH_MIN_WRITE) >= FLASH_END || addr < FLASH_START) {
#if PUT_IN_DATA == 0
    printf("*** WW address error. FST: %lu addr: %lu FEND: %lu \n",
           (unsigned long)FLASH_START, addr, (unsigned long)FLASH_END);
#endif
    /* invalid address */
    return;
  }
  /* 对传入的地址按4字节对齐 */
  addr32 = addr & 0xfffffffc;
  /* 先把原来的数值取出来 */
  memcpy(&data32, (void *)(intptr_t)addr32, sizeof(data32));

  i = addr - addr32;				//  算出传入的地址和对齐的地址偏移的字节数		
  // for(i = 0; i < 4; i++) {
	  // uint32_t mask = 0xff << (i<<3);
	  // if((byte & mask) == byte) {		
		  // data32 &= byte | ~mask ;
	  // }    
	  // printf("debug_  i=%d %08X: %08X: %08X\n",i, byte, data32, mask);
  // }
  mask = 0xff << (i<<3);					// 根据这个字节数将掩码移动到对应的位置
											// (i左移动3位,意思是字节数乘以8,一个字节长) 
  data32 &= (byte << (i<<3)) | ~mask;		// 先把data移动到正确的位置,其他位填补成全1
  

  /* check that the flash controller is not currently in use */
  while(REG(FLASH_CTRL_FCTL) & FLASH_BITFIELD_BUSY);

  INTERRUPTS_DISABLE();

  /*  The flash-write sequence algorithm is as follows:*/
  /*  1. Set FADDR to the start address.*/
  REG(FLASH_CTRL_FADDR) = addr32;

  /*  2. Set FCTL.WRITE to 1. This starts the write-sequence state machine.*/
  REG(FLASH_CTRL_FCTL) |= FLASH_BITFIELD_WRITE;

  /*  3. Write the 32-bit data32 to FWDATA (since the last time FCTL.FULL became 0,*/
  /*  if not first iteration). FCTL.FULL goes high after writing to FWDATA*/
  REG(FLASH_CTRL_FWDATA) = data32;

  /*  4. Wait until FCTL.FULL goes low. (The flash controller has started programming*/
  /*  the 4 bytes written in step 3 and is ready to buffer the next 4 bytes).*/
  while(REG(FLASH_CTRL_FCTL) & FLASH_BITFIELD_FULL);

  INTERRUPTS_ENABLE();
  
  memcpy(&data32, (void *)(intptr_t)addr32, sizeof(data32));
  printf("check: %08X\n", (unsigned int)data32);
}
/*---------------------------------------------------------------------------*/

uint32_t
flash_read(unsigned long addr)
{
  return *((unsigned long *)addr);
}
/*---------------------------------------------------------------------------*/
/* empty legacy API function */
void
flash_setup(void)
{
  ;
}
/*---------------------------------------------------------------------------*/
/* empty legacy API function */
void
flash_done(void)
{
  ;
}
/*---------------------------------------------------------------------------*/
