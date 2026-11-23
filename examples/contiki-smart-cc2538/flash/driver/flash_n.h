/*
 * Copyright (c) 2012, Thingsquare, http://www.thingsquare.com/.
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
 *      flash.h
 *   \author
 *      Marcus Lunden <marcus@thingsquare.com>>
 *   \desc
 *      Flash driver for CC2538
 */

#ifndef FLASH_H_
#define FLASH_H_
/*---------------------------------------------------------------------------*/
/* erase a page in flash based on an address anywhere in that page */
void flash_clear(unsigned long addr);

/* write a 32-bit word to flash. */
void flash_write(unsigned long addr, uint32_t data);

/* write a byte to flash. */
void flash_write_single_byte(unsigned long addr, uint8_t byte);

/* read a 32-bit word from flash */
unsigned long flash_read(unsigned long addr);

/* empty legacy API function */
void flash_setup(void);

/* empty legacy API function */
void flash_done(void);
/*---------------------------------------------------------------------------*/
/* The flash controller registers */
#define FLASH_CTRL_FCTL           0x400D3008
#define FLASH_CTRL_FADDR          0x400D300C
#define FLASH_CTRL_FWDATA         0x400D3010
#define FLASH_CTRL_DIECFG0		  0x400D3014
#define FLASH_CTRL_DIECFG1		  0x400D3018
#define FLASH_CTRL_DIECFG2		  0x400D301C

/* a few of the bitfields in the FCTL register */
#define FLASH_BITFIELD_UPAGE_ACCESS       ((uint32_t) (1<<9))
#define FLASH_BITFIELD_SEL_INFOP          ((uint32_t) (1<<8))
#define FLASH_BITFIELD_BUSY               ((uint32_t) (1<<7))
#define FLASH_BITFIELD_FULL               ((uint32_t) (1<<6))
#define FLASH_BITFIELD_WRITE              ((uint32_t) (1<<1))
#define FLASH_BITFIELD_ERASE              ((uint32_t) (1<<0))

#define FLASH_CTRL_DIECFG0_CHIPID_M				(0xFFFF << 16)
#define FLASH_CTRL_DIECFG0_CHIPID_S				16
#define FLASH_CTRL_DIECFG0_CLK_SEL_GATE_EN_N_M	(1 << 10)
#define FLASH_CTRL_DIECFG0_CLK_SEL_GATE_EN_N_S	10
#define FLASH_CTRL_DIECFG0_SRAM_SIZE_M			(0x7 << 7)
#define FLASH_CTRL_DIECFG0_SRAM_SIZE_S			7
#define FLASH_CTRL_DIECFG0_FLASH_SIZE_M			(0x7 << 4)
#define FLASH_CTRL_DIECFG0_FLASH_SIZE_S			4
#define FLASH_CTRL_DIECFG0_USB_ENABLE_M			(1 << 3)
#define FLASH_CTRL_DIECFG0_USB_ENABLE_S			3
#define FLASH_CTRL_DIECFG0_MASS_ERASE_ENABLE_M	(1 << 2)
#define FLASH_CTRL_DIECFG0_MASS_ERASE_ENABLE_S	2
#define FLASH_CTRL_DIECFG0_LOCK_FWT_N_M			(1 << 1)
#define FLASH_CTRL_DIECFG0_LOCK_FWT_N_S			1
#define FLASH_CTRL_DIECFG0_LOCK_IP_M			(1 << 0)
#define FLASH_CTRL_DIECFG0_LOCK_IP_S			0

#define FLASH_CTRL_DIECFG1_I2C_EN_M				(1 << 24)
#define FLASH_CTRL_DIECFG1_I2C_EN_S				24
#define FLASH_CTRL_DIECFG1_UART1_EN_M			(1 << 17)
#define FLASH_CTRL_DIECFG1_UART1_EN_S			17
#define FLASH_CTRL_DIECFG1_UART0_EN_M			(1 << 16)
#define FLASH_CTRL_DIECFG1_UART0_EN_S			16
#define FLASH_CTRL_DIECFG1_SSI1_EN_M			(1 << 9)
#define FLASH_CTRL_DIECFG1_SSI1_EN_S			9
#define FLASH_CTRL_DIECFG1_SSI0_EN_M			(1 << 8)
#define FLASH_CTRL_DIECFG1_SSI0_EN_S			8
#define FLASH_CTRL_DIECFG1_GPTM3_EN_M			(1 << 3)
#define FLASH_CTRL_DIECFG1_GPTM3_EN_S			3
#define FLASH_CTRL_DIECFG1_GPTM2_EN_M			(1 << 2)
#define FLASH_CTRL_DIECFG1_GPTM2_EN_S			2
#define FLASH_CTRL_DIECFG1_GPTM1_EN_M			(1 << 1)
#define FLASH_CTRL_DIECFG1_GPTM1_EN_S			1
#define FLASH_CTRL_DIECFG1_GPTM0_EN_M			(1 << 0)
#define FLASH_CTRL_DIECFG1_GPTM0_EN_S			0

#define FLASH_CTRL_DIECFG2_DIE_MAJOR_REVISION_M			(0xF << 12)
#define FLASH_CTRL_DIECFG2_DIE_MAJOR_REVISION_S			12
#define FLASH_CTRL_DIECFG2_DIE_MINOR_REVISION_M			(0xF << 8)
#define FLASH_CTRL_DIECFG2_DIE_MINOR_REVISION_S			8
#define FLASH_CTRL_DIECFG2_RF_CORE_EN_M			(1 << 2)
#define FLASH_CTRL_DIECFG2_RF_CORE_EN_S			2
#define FLASH_CTRL_DIECFG2_AES_EN_M				(1 << 1)
#define FLASH_CTRL_DIECFG2_AES_EN_S				1
#define FLASH_CTRL_DIECFG2_PKA_EN_M				(1 << 0)
#define FLASH_CTRL_DIECFG2_PKA_EN_S				0

	

/* 
 * CPU					FLASH_SIZE
 * CC2538{S,N}F53		512KB 
 * CC2538{S,N}F23		256KB
 * CC2538NF11			128KB  
  
 */
#ifndef FLASH_CONF_START
#define FLASH_START           FLASH_CONF_ORIGIN		/*in cc2538.lds*/
#else
#define	FLASH_START			  FLASH_CONF_START
#endif  /* FLASH_CONF_START */

#ifndef FLASH_CONF_SIZE
#define FLASH_SIZE            ((uint32_t) 0x00080000)
#else
#define	FLASH_SIZE			  FLASH_CONF_SIZE
#endif  /* FLASH_CONF_SIZE */

#define FLASH_CCA_LENGTH 44
#define FLASH_LENGTH     (FLASH_SIZE - FLASH_CCA_LENGTH)

#ifndef FLASH_CONF_END
#define FLASH_END             (FLASH_START + FLASH_LENGTH) /* LENGTH = 0x0007FFD4 */
#else
#define FLASH_END			  FLASH_CONF_END
#endif	/* FLASH_CONF_END */
#define FLASH_PAGE_SIZE       ((uint32_t) 2048)           /* Bytes */
#define FLASH_MIN_WRITE       ((uint32_t) 4)              /* Bytes */
#define FLASH_MIN_ERASE       FLASH_PAGE_SIZE

/*
 * macros for getting the page number, start and end addresses a page from an 
 * address located in that page.
 */
#define FLASH_PAGE_NO(addr)       ((uint16_t) ((addr - FLASH_START) / FLASH_PAGE_SIZE))
#define FLASH_PAGE_START(addr)    (FLASH_START + FLASH_PAGE_SIZE * FLASH_PAGE_NO(addr))
#define FLASH_PAGE_END(addr)      (FLASH_PAGE_START(addr) + FLASH_PAGE_SIZE)
/*---------------------------------------------------------------------------*/
#endif /* FLASH_H_ */
