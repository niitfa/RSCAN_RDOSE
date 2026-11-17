/*
 * uart_rx_handler.c
 *
 *  Created on: Apr 7, 2025
 *      Author: Kirill
 */
#include "uart_rx_handler.h"
#include "flash_data.h"
#include "w5500_task.h"
#include <string.h>

#define UART_RX_MESSAGE_SIZE 7
#define UART_TX_MESSAGE_SIZE 7

extern UART_HandleTypeDef huart1;

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart == &huart1)
	{
		uart_handle_rx_message();
		uart_handle_receive();
	}
}

static uint8_t txBuff[UART_TX_MESSAGE_SIZE];
static uint8_t rxBuff[UART_RX_MESSAGE_SIZE];

static const int TIMEOUT = 50;

typedef enum
{
	COMM_EMPTY = 0x00,
	COMM_SET_CONF = 0x01,
	COMM_GET_CONF = 0x02,
	COMM_PING = 0x03
} uart_command_code_t;

typedef enum
{
	RESP_OK = 0x01,
	RESP_ERR = 0x02
} response_code_t;

void uart_handle_receive()
{
	memset(rxBuff, 0, UART_RX_MESSAGE_SIZE);
	HAL_UART_Receive_IT(&huart1, rxBuff, UART_RX_MESSAGE_SIZE);
}

void uart_handle_rx_message()
{
	// 0 byte - command
	// 1 - 6 bytes - parameters
	// msg:
	// 0 - ip[0]
	// 1 - ip[1]
	// 2 - ip[2]
	// 3 - ip[3]
	// 4-5 - port
	uart_command_code_t command_code = rxBuff[0];
	int ip = 0;
	uint16_t port = 0;

	switch(command_code)
	{
	case COMM_EMPTY:
		break;
	case COMM_SET_CONF:
		// execution
		memcpy(&ip, rxBuff + 1, 4);
		flash_data_set_ip((uint8_t*)&ip);
		memcpy(&port, rxBuff + 5, 2);
		flash_data_set_port(port);

		// response, set tx buff
		memset(txBuff, 0, UART_TX_MESSAGE_SIZE);
		memcpy(txBuff + 1, &command_code, 1);
		HAL_UART_Transmit(&huart1, txBuff, UART_TX_MESSAGE_SIZE, TIMEOUT);
		break;
	case COMM_GET_CONF:
		// execution
		ip = w5500_task_get_ip();
		port = w5500_task_get_port();

		// response, set tx buff
		memset(txBuff, 0, UART_TX_MESSAGE_SIZE);
		memcpy(txBuff + 0, &command_code, 1);
		memcpy(txBuff + 1, &ip, 4);
		memcpy(txBuff + 5, &port, 2);
		HAL_UART_Transmit(&huart1, txBuff, UART_TX_MESSAGE_SIZE, TIMEOUT);
		break;
	case COMM_PING:
		// response, set tx buff
		memset(txBuff, 0, UART_TX_MESSAGE_SIZE);
		memcpy(txBuff, &command_code, 1);
		HAL_UART_Transmit(&huart1, txBuff, UART_TX_MESSAGE_SIZE, TIMEOUT);
		break;
	}

}

#undef UART_RX_MESSAGE_SIZE
#undef UART_TX_MESSAGE_SIZE

