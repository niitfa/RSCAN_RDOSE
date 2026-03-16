/*
 * tcp_server_tx.h
 *
 *  Created on: Dec 25, 2025
 *      Author: Kirill
 */

#ifndef SRC_W5500_TASK_TCP_SERVER_TX_H_
#define SRC_W5500_TASK_TCP_SERVER_TX_H_

#include <stdint.h>

void tcp_server_tx_init();
uint8_t* tcp_server_tx_get();
int tcp_server_tx_size();
void tcp_server_tx_update();

// other
int tcp_server_tx_get_msg_id();

#endif /* SRC_W5500_TASK_TCP_SERVER_TX_H_ */
