/**
 * @file slave_uart.c
 * @brief Файл с определениями функций и структуры, отвечающих за взаимодействие
 * контроллера с внешними устройствами по UART.
 */

#include <string.h>
#include "slave_uart.h"
#include "main.h"

static const int TIMEOUT = 10; ///< Timeout для блокирующей отправки UART
static const int RX_BYTE_COMMAND_CODE_POS = 0; ///< Позиция кода команды в пакете
static const int RX_BYTE_COMMAND_VALUE_POS = 1;///< Размер кода команды в пакете
static const int RX_BYTE_COMMAND_CODE_SIZE = 1; ///< Позиция параметра команды в пакете
static const int RX_BYTE_COMMAND_VALUE_SIZE = 4; ///< Размер параметра команды в пакете
/**
 * @brief Основная структура модуля UART, содержащая
 * информацию о входящем и исходящем сообщениях
 */
typedef struct
{
	UART_HandleTypeDef* huart;
	GPIO_TypeDef* 	portDE;
	uint16_t 		pinDE;
	uint8_t rx_buff[SLAVE_UART_RX_BUFF_SIZE];
	uint8_t tx_buff[SLAVE_UART_TX_BUFF_SIZE];
	int32_t msgNo;
} slave_uart_t;
/**
 * @brief Единственный экземпляр структуры slave_uart_t.
 * Работа всех определенных в файле функций происходит только с ней
 */
static slave_uart_t data;
/**
 * @brief Возвращает указатель на буфер исходящего сообщения
 * @return Указатель на буфер исходящего сообщения
 */
static uint8_t* slave_uart_tx_buffer();
/**
 * @brief Очищает буфер исходящего сообщения
 */
static void slave_uart_clear_tx_buffer();
/**
 * @brief Отправляет сообщение из исходящего буфера по UART
 */
static void slave_uart_transmit();

void slave_uart_init(UART_HandleTypeDef* huart)
{
	memset(&data, 0, sizeof(data));
	data.huart = huart;
}

uint8_t* slave_uart_rx_buffer()
{
	return data.rx_buff;
}

void slave_uart_clear_rx_buffer()
{
	memset(slave_uart_rx_buffer(), 0, SLAVE_UART_RX_BUFF_SIZE);
}

void slave_uart_handle_rx_message()
{
	command_code_t command_code = *(slave_uart_rx_buffer() + RX_BYTE_COMMAND_CODE_POS);
	//int value = *(int*)(slave_uart_rx_buffer() + RX_BYTE_COMMAND_VALUE_POS); // supress warning
	int sendVal = 0;

	slave_uart_clear_tx_buffer();
	switch(command_code)
	{
	case COMMAND_EMPTY:
		break;
	case COMMAND_ENABLE_HV_A:
		break;
	case COMMAND_ENABLE_HV_B:
		break;
	case COMMAND_DISABLE_HV_A:
		break;
	case COMMAND_DISABLE_HV_B:
		break;
	case COMMAND_SET_VOLTAGE_A:
		break;
	case COMMAND_SET_VOLTAGE_B:
		break;
	case COMMAND_SET_HV_POLARITY_A:
		break;
	case COMMAND_SET_HV_POLARITY_B:
		break;
	case COMMAND_SET_SENSITIVITY_A:
		break;
	case COMMAND_SET_SENSITIVITY_B:
		break;

		// getters
	case COMMAND_GET_MESSAGE_NO:
		break;
	case COMMAND_GET_DOSE_A:
		break;
	case COMMAND_GET_DOSE_B:
		break;
	case COMMAND_GET_HV_A:
		break;
	case COMMAND_GET_HV_B:
		break;
	case COMMAND_GET_HV_POL_A:
		break;
	case COMMAND_GET_HV_POL_B:
		break;
	case COMMAND_GET_HV_EN_A:
		break;
	case COMMAND_GET_HV_EN_B:
		break;
	case COMMAND_GET_SENSITIVITY_A:
		break;
	case COMMAND_GET_SENSITIVITY_B:
		break;
	case COMMAND_GET_TEMP:
		break;
	default:
		break;
	}
	memcpy(slave_uart_tx_buffer() + RX_BYTE_COMMAND_VALUE_POS, (uint8_t*)&sendVal, RX_BYTE_COMMAND_VALUE_SIZE);
	memcpy(slave_uart_tx_buffer() + RX_BYTE_COMMAND_CODE_POS, (uint8_t*)&command_code, RX_BYTE_COMMAND_CODE_SIZE);
	slave_uart_transmit();
}


static uint8_t* slave_uart_tx_buffer()
{
	return data.tx_buff;
}

static void slave_uart_clear_tx_buffer()
{
	memset(data.tx_buff, 0, SLAVE_UART_TX_BUFF_SIZE);
}

static void slave_uart_transmit()
{
	if(data.huart)
	{
		HAL_UART_Transmit(data.huart, slave_uart_tx_buffer(), SLAVE_UART_TX_BUFF_SIZE, TIMEOUT);
	}
}




