/*
 * uart.c
 *
 *  Created on: 2017年9月28日
 *      Author: lucius
 */

#include "x6818/uart.h"
#include "x6818/gpio.h"
#include "x6818/tieoff.h"
#include "x6818/iprst.h"
#include "x6818/clkgen.h"

#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>


struct s5p6818_uart_reg {
	volatile uint32_t ulcon;
	volatile uint32_t ucon;
	volatile uint32_t ufcon;
	volatile uint32_t umcon;
	volatile uint32_t utrstat;
	volatile uint32_t uerstat;
	volatile uint32_t ufstat;
	volatile uint32_t umstat;
	volatile uint32_t utxh;
	volatile uint32_t urxh;
	volatile uint32_t ubrdiv;
	volatile uint32_t ufracval;
	volatile uint32_t uintp;
	volatile uint32_t uints;
	volatile uint32_t uintm;
};

#define UART0_BASE (0xC00A1000)
#define UART1_BASE (0xC00A0000)
#define UART2_BASE (0xC00A2000)
#define UART3_BASE (0xC00A3000)
#define UART4_BASE (0xC006D000)
#define UART5_BASE (0xC006F000)

#define UART0 ((struct s5p6818_uart_reg *)UART0_BASE)
#define UART1 ((struct s5p6818_uart_reg *)UART1_BASE)
#define UART2 ((struct s5p6818_uart_reg *)UART2_BASE)
#define UART3 ((struct s5p6818_uart_reg *)UART3_BASE)
#define UART4 ((struct s5p6818_uart_reg *)UART4_BASE)

#define UART_ULCON	 	 		(0x00)
#define UART_UCON	 	 		(0x04)
#define UART_UFCON	 	 		(0x08)
#define UART_UMCON	 	 		(0x0C)
#define UART_UTRSTAT			(0x10)
#define UART_UERSTAT			(0x14)
#define UART_UFSTAT				(0x18)
#define UART_UMSTAT				(0x1C)
#define UART_UTXH				(0x20)
#define UART_URXH				(0x24)
#define UART_UBRDIV				(0x28)
#define UART_UFRACVAL			(0x2C)
#define UART_UINTP				(0x30)
#define UART_UINTSP				(0x34)
#define UART_UINTM				(0x38)

#define UART_UFSTAT_TXFULL		(1<<24)
#define UART_UFSTAT_RXFULL		(1<<8)
#define UART_UFSTAT_TXCOUNT		(0xFF<<16)
#define UART_UFSTAT_RXCOUNT		(0xFF<<0)
#define UART_UTRSTAT_TXE	  	(1<<2)
#define UART_UTRSTAT_TXFE		(1<<1)
#define UART_UTRSTAT_RXDR		(1<<0)
#define UART_UERSTAT_OVERRUN	(1<<0)
#define UART_UERSTAT_PARITY		(1<<1)
#define UART_UERSTAT_FRAME		(1<<2)
#define UART_UERSTAT_BREAK		(1<<3)
#define UART_UMSTAT_CTS	  		(1<<0)
#define UART_UMSTAT_DCTS		(1<<4)

/*uart tieoff config */
#define TIEOFF_SMARTCARD_UART0 (1<<20)
#define TIEOFF_SMARTCARD_TX_UART0 (1<<21)
#define TIEOFF_SMARTCARD_RX_UART0 (1<<22)

/* uart iprst */
#define UART0_IPRST_INDEX ((uint32_t )1)
#define UART0_IPRST_OFFSET ((uint32_t )18)

static inline void uart_gpio_init(void)
{
	gpio_set_cfg(GPIOD, 18, 0x1);
	gpio_set_cfg(GPIOD, 14, 0x1);
	gpio_set_output(GPIOD, 20);
	gpio_set_input(GPIOD, 16);
}

static inline void uart_tieoff_init(void)
{
	struct s5p6818_tieoff_reg * tieoff = TIEOFF;

	tieoff->con[4] &= (~TIEOFF_SMARTCARD_RX_UART0);
	tieoff->con[4] &= (~TIEOFF_SMARTCARD_TX_UART0);
	tieoff->con[4] &= (~TIEOFF_SMARTCARD_UART0);
}

void uart_init(void)
{
	struct s5p6818_uart_reg * uart = UART0;

	uint32_t val = 0;
	uint32_t src_clk_freq;

	uart_gpio_init();
	//tieoff
	uart_tieoff_init();
	// ip reset
	iprst_do_reset(50/32, 50%32);

	//clkgen config
	clk_gen_enable(CLK_GEN_UART0);
	clk_gen_set_src(CLK_GEN_UART0, CLK_GEN_SRC_PLL2);
	src_clk_freq = clk_gen_get_freq(CLK_GEN_UART0);

	//uart config
	val = (src_clk_freq / (115200 * 16)) - 1;
	uart->ubrdiv = val;
	val = src_clk_freq/115200;
	val &= 0xF;
//  val %= (115200 * 16 + 16/2);	// 0.xxxxxx
//  val <<= 4;	// mul by 64 (0.xxxxxx * 64 = xx.xxxxxx)
//  val += 32;	// + 0.5 (32/64) ( half add )
//  val /= (115200 * 16 + 16/2); // xx.000000
	uart->ufracval = val;
	uart->ulcon = 0x3;
	uart->ucon = 0x113345;
	uart->ufcon = 0x441;
	uart->umcon = 0;

	uart_printf("%u\n", src_clk_freq);
}

void uart_write(char *str, int len) {
	struct s5p6818_uart_reg * uart = UART0;
	int i = 0;
	for (i = 0; i < len; i++) {
		while (uart->ufstat & UART_UFSTAT_TXFULL)
			;
		if (str[i] != '\n') {
			uart->utxh = str[i];
		} else {
//			uart->utxh = '\r';
//			while (uart->ufstat & UART_UFSTAT_TXFULL)
//				;
			uart->utxh = str[i];
		}
	}
}
static char buf[2048];

//static spin_lock_t uart_printf_lock = 0;

int uart_printf(char *fmt, ...) {
	va_list va_arg;
	va_start(va_arg, fmt);

	int len = vsnprintf(buf, 2048, fmt, va_arg);
	buf[len] = '\0';

//	spin_lock(&uart_printf_lock);
	uart_write(buf, len);
//	spin_unlock(&uart_printf_lock);

	va_end(va_arg);
	return len;
}

int uart_is_busy(void) {
	struct s5p6818_uart_reg * uart = UART0;

	if ((uart->ufstat >> 16) & 0x1FF)
		return 1;
	else
		return (uart->utrstat & UART_UTRSTAT_TXE) ? 0 : 1;
}
