/**
 * @file tcp_server_rx.h
 * @brief Файл с прототипами функций, отвечающих за инициализацию
 * и обработку входящего сообщения в рамках выполнения
 * задания по tcp клиент-серверному взаимодействию
 */

#ifndef SRC_W5500_TASK_TCP_SERVER_RX_H_
#define SRC_W5500_TASK_TCP_SERVER_RX_H_

#include <stdint.h>
#include "command_code.h"
/**
 * @brief Инициализация входящего сообщения.
 * Вызывается только в w5500_task_init()
 * @see w5500_task_init()
 */
void tcp_server_rx_init();
/**
 * @brief Получение адреса последнего полученного сообщения
 * @return Адрес входящего сообщения
 */
uint8_t* tcp_server_rx_get();
/**
 * @brief Получение размера входящего сообщения
 * @return Размер входящего сообщения
 */
int tcp_server_rx_size();
/**
 * @brief Обработка входящего сообщения, выполнение команд.
 * Вызывается только в w5500_task_loop()
 */
void tcp_server_rx_handle();

#endif /* SRC_W5500_TASK_TCP_SERVER_RX_H_ */
