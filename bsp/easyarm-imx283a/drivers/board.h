/*
 * File      : board.h
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2006, RT-Thread Develop Team
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * Change Logs:
 * Date           Author       Notes
 * 2011-01-13     weety      add board.h to this bsp
 */

#ifndef __BOARD_H__
#define __BOARD_H__

#include "common.h"
#include "regs-uartdbg.h"
#include "regs-digctl.h"
#include "regs-timrot.h"
#include "regs-uartdbg.h"
#include "regs-uartapp.h"
#include "regs-clkctrl.h"
#include "regs-ocotp.h"
#include "regs-enet.h"
#include "regs-rtc.h"
#include "regs-lcdif.h"
#include "regs-pwm.h"
#include "regs-lradc.h"
#include "pinctrl.h"

//	<i>Default: 64
#define HEAP_BEGIN        0x40400000
#define HEAP_END          0x43F00000

#define RT_USING_DBGU
#define RT_USING_UART1
#define RT_USING_UART2
//#define RT_USING_UART3
//#define RT_USING_UART4
//#define RT_USING_UART5

#define PIN_WDT		PINID_GPMI_RDY1
#define MXS280A
#ifndef MXS280A
#define PIN_BEEP		PINID_LCD_D21
#define PIN_RUN		PINID_LCD_D22
#define PIN_ERR		PINID_LCD_D23
#define PIN_NET0	PINID_LCD_D16
#else
#define PIN_BEEP		PINID_SSP0_DATA6
#define PIN_RUN		PINID_SSP0_DATA7
#define PIN_ERR		PINID_SAIF0_MCLK
#define PIN_NET0	PINID_LCD_RS
#endif

#define CONSOLE_DEVICE "dbgu"
#define FINSH_DEVICE_NAME "dbgu"

void rt_hw_board_init(void);
void rt_hw_interrupt_init(void);
void rt_hw_usart_init(void);
void rt_hw_timer_init(void);
void rt_hw_spi_init(void);
void rt_hw_rtc_init(void);
void rt_hw_mtd_nand_init(void);

extern volatile int eth_wtdog;
extern volatile int eth_linkstatus;
extern volatile int wtdog_count;
extern volatile int sys_stauts;
void inittmppath(void);

#ifndef _MSC_VER
extern void __delay(int loops);
extern void __bad_udelay(void);
extern void __udelay(unsigned long usecs);
extern void __const_udelay(unsigned long);

#define HZ 1000
#define MAX_UDELAY_MS 2

#define udelay(n)							\
	(__builtin_constant_p(n) ?					\
	  ((n) > (MAX_UDELAY_MS * 1000) ? __bad_udelay() :		\
			__const_udelay((n) * ((2199023U*HZ)>>11))) :	\
	  __udelay(n))

#include <rtthread.h>
static inline int mxs_reset_clock(u32 hwreg, int is_enable)
{
	int timeout;
#define SFTRST 0x80000000
#define CLKGATE 0x40000000

	/* the process of software reset of IP block is done
	   in several steps:

	   - clear SFTRST and wait for block is enabled;
	   - clear clock gating (CLKGATE bit);
	   - set the SFTRST again and wait for block is in reset;
	   - clear SFTRST and wait for reset completion.
	 */
	/* clear SFTRST */
	REG_CLR_ADDR(hwreg, SFTRST);

	for (timeout = 1000000; timeout > 0; timeout--)
		/* still in SFTRST state ? */
		if ((REG_RD_ADDR(hwreg) & SFTRST) == 0)
			break;
		if (timeout <= 0) {
			rt_kprintf("%s(%p): timeout when enabling\n",
				__func__, hwreg);
			return -1;
	}

	/* clear CLKGATE */
	REG_CLR_ADDR(hwreg, CLKGATE);

	if (is_enable) {
		/* now again set SFTRST */
		REG_SET_ADDR(hwreg, SFTRST);
		for (timeout = 1000000; timeout > 0; timeout--)
			/* poll until CLKGATE set */
			if (REG_RD_ADDR(hwreg) & CLKGATE)
				break;
		if (timeout <= 0) {
			rt_kprintf("%s(%p): timeout when resetting\n",
				__func__, hwreg);
			return -1;
		}

		REG_CLR_ADDR(hwreg, SFTRST);
		for (timeout = 1000000; timeout > 0; timeout--)
			/* still in SFTRST state ? */
			if ((REG_RD_ADDR(hwreg) & SFTRST) == 0)
				break;
		if (timeout <= 0) {
			rt_kprintf("%s(%p): timeout when enabling "
				"after reset\n", __func__, hwreg);
			return -1;
		}

		/* clear CLKGATE */
		REG_CLR_ADDR(hwreg, CLKGATE);
	}
	for (timeout = 1000000; timeout > 0; timeout--)
		/* still in SFTRST state ? */
		if ((REG_RD_ADDR(hwreg) & CLKGATE) == 0)
			break;

	if (timeout <= 0) {
		rt_kprintf("%s(%p): timeout when unclockgating\n",
			__func__, hwreg);
		return -1;
	}

	return 0;
}

static inline void mxs_clock_enable(u32 hwreg, u32 gate)
{
	unsigned int reg = readl(hwreg);
	reg &= ~gate;
	writel(reg, hwreg);
}

static inline void mxs_clock_disable(u32 hwreg, u32 gate)
{
	unsigned int reg = readl(hwreg);
	reg |= gate;
	writel(reg, hwreg);
}

#endif
#endif