/*
 ============================================================================
 Name        : main.c
 Author      : Lucius
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C
 ============================================================================
 */

#include <stdio.h>
#include <stdint.h>

#include "x6818/uart.h"
#include "x6818/clkpwr.h"

/*
 *
 * Print a greeting message on standard output and exit.
 *
 * On embedded platforms this might require semi-hosting or similar.
 *
 * For example, for toolchains derived from GNU Tools for Embedded,
 * to enable semi-hosting, the following was added to the linker:
 *
 * --specs=rdimon.specs -Wl,--start-group -lgcc -lc -lc -lm -lrdimon -Wl,--end-group
 *
 * Adjust it for other toolchains.
 *
 */

uint32_t boot_main(uint32_t cpu_id)
{
	(void )cpu_id;
	uart_init();
	uart_printf("uart init!\n");
  while(uart_is_busy());
  clkpwr_pll_init();
	uart_init();
  uart_printf("pll inited\n");
  while(uart_is_busy());

  while(1)
  {

  }
  return 0;
}

int __attribute__ ((weak)) _write(int file, char *ptr, int len) {
	(void )file;
	(void )ptr;
	(void )len;
  return len;
}

uint32_t sub_cpu_main(uint32_t cpu_id)
{
	(void )cpu_id;

	return 0;
}
