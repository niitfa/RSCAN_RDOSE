/*
 * rdose_uart.h
 *
 *  Created on: Feb 4, 2026
 *      Author: Kirill
 */

#ifndef SRC_SLAVE_UART_RDOSE_UART_H_
#define SRC_SLAVE_UART_RDOSE_UART_H_

#include <stdint.h>
#include "stm32f4xx_hal.h"
#include "command_code.h"

#define SLAVE_UART_TX_BUFF_SIZE 5
#define SLAVE_UART_RX_BUFF_SIZE SLAVE_UART_TX_BUFF_SIZE

void slave_uart_init();
uint8_t* slave_uart_rx_buffer();
void slave_uart_clear_rx_buffer();
void slave_uart_handle_rx_message();
void slave_uart_receive_it();
UART_HandleTypeDef* slave_uart_get();

#endif /* SRC_RDOSE_UART_RDOSE_UART_H_ */
