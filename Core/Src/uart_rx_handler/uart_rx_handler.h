/*
 * uart_rx_handler.h
 *
 *  Created on: Apr 7, 2025
 *      Author: Kirill
 */

#ifndef SRC_UART_RX_HANDLER_UART_RX_HANDLER_H_
#define SRC_UART_RX_HANDLER_UART_RX_HANDLER_H_

#include <stdint.h>
#include "stm32f4xx_hal.h"

void uart_handle_receive();
void uart_handle_rx_message();

#endif /* SRC_UART_RX_HANDLER_UART_RX_HANDLER_H_ */
