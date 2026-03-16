/*
 * tcp_server_rx.h
 *
 *  Created on: Dec 25, 2025
 *      Author: Kirill
 */

#ifndef SRC_W5500_TASK_TCP_SERVER_RX_H_
#define SRC_W5500_TASK_TCP_SERVER_RX_H_

#include <stdint.h>
#include "command_code.h"

void tcp_server_rx_init();
uint8_t* tcp_server_rx_get();
int tcp_server_rx_size();
void tcp_server_rx_handle();

// other
int tcp_server_rx_get_last_command_code();

#endif /* SRC_W5500_TASK_TCP_SERVER_RX_H_ */
