/*
 * uart.h
 *
 *  Created on: 2017年9月28日
 *      Author: lucius
 */

#ifndef X6818_UART_H_
#define X6818_UART_H_

#include <stdint.h>

void uart_init(void);
void uart_write(char *str, int len);
int uart_printf(char *fmt, ...);
int uart_is_busy(void);

#endif /* X6818_UART_H_ */
