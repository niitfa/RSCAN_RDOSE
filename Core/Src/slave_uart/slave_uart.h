/**
 * @file slave_uart.h
 * @brief Файл с прототипами функций, отвечающих за взаимодействие
 * контроллера с внешними устройствами по UART. Контроллер ожидает
 * от внешнего устройства сообщение и при получении отвечает на него
 */

#ifndef SRC_SLAVE_UART_RDOSE_UART_H_
#define SRC_SLAVE_UART_RDOSE_UART_H_

#include <stdint.h>
#include "stm32f4xx_hal.h"
#include "command_code.h"

#define SLAVE_UART_TX_BUFF_SIZE 5 ///< Размер буфера исходящего сообщения
#define SLAVE_UART_RX_BUFF_SIZE SLAVE_UART_TX_BUFF_SIZE /// Размер буфера входящего сообщения

/**
 * @brief Инициализирует UART модуль
 * @param[in] huart Указатель на структуру UART, через которую будет осуществляться связь
 */
void slave_uart_init(UART_HandleTypeDef* huart);
/**
 * @brief Возвращает указатель на буфер входящего сообщения
 * @return Указатель на буфер входящего сообщения
 */
uint8_t* slave_uart_rx_buffer();
/**
 * @brief Очищает входящий буфер
 */
void slave_uart_clear_rx_buffer();
/**
 * @brief Обрабатывает входящее сообщение
 */
void slave_uart_handle_rx_message();
/**
 * @brief Запускает прием сообщения в режиме прерывания
 */
void slave_uart_receive_it();
/**
 * @brief Возвращает указатель на UART структуру, через которую осуществляется связь
 * @return Указатель на UART структуру
 */
UART_HandleTypeDef* slave_uart_get();

#endif /* SRC_RDOSE_UART_RDOSE_UART_H_ */
