/*
 * w5500_task.h
 *
 *  Created on: Dec 23, 2025
 *      Author: Kirill
 */

#include "w5500.h"
#include "wiznet_api.h"
#include "socket.h"
#include <stdint.h>

#include <tcp_server_rx.h>
#include <tcp_server_tx.h>

#ifndef SRC_W5500_TASK_W5500_TASK_H_
#define SRC_W5500_TASK_W5500_TASK_H_

void w5500_task_init();
void w5500_task_loop();

// other
int w5500_task_get_ip();
uint8_t w5500_get_socket_status();

#endif /* SRC_W5500_TASK_W5500_TASK_H_ */
