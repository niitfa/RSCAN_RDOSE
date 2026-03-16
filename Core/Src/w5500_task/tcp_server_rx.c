/*
 * tcp_server_rx.c
 *
 *  Created on: Dec 25, 2025
 *  Author: Kirill
 */

// For other projects change
// 1) TCP_RX_BUFF_SIZE - buffer size
// 2) command_code_t - list of commands from client
// 3) value_code_t - message content
// 4) rx_values[VALUE_SIZE] - values` addresses and sizes
// 5) tcp_server_rx_handle() - command execution


#include "tcp_server_rx.h"
#include <stdint.h>
#include <string.h>
#include "main.h"

#define TCP_RX_BUFF_SIZE 64

typedef enum
{
	VALUE_COMMAND_CODE,
	VALUE_COMMAND_PARAM,
	VALUE_SIZE
} value_code_t;

typedef struct
{
	uint16_t offset;
	uint8_t size;
} value_address_t;

typedef struct
{
	uint8_t buff[TCP_RX_BUFF_SIZE];
	value_address_t rx_values[VALUE_SIZE];
	command_code_t lastCommand;
} tcp_server_rx_t;

static tcp_server_rx_t data;

void tcp_server_rx_init()
{
	memset (&data, 0, sizeof (data));

	data.rx_values[VALUE_COMMAND_CODE] 	= (value_address_t){0, 4};
	data.rx_values[VALUE_COMMAND_PARAM] = (value_address_t){4, 4};
}

uint8_t* tcp_server_rx_get()
{
	return data.buff;
}

int tcp_server_rx_size()
{
	return TCP_RX_BUFF_SIZE;
}

void tcp_server_rx_handle()
{
	int command = 0;
	int parameter = 0;

	uint8_t* command_src = data.buff + data.rx_values[VALUE_COMMAND_CODE].offset;
	uint16_t command_size = data.rx_values[VALUE_COMMAND_CODE].size;

	uint8_t* parameter_src = data.buff + data.rx_values[VALUE_COMMAND_PARAM].offset;
	uint16_t parameter_size = data.rx_values[VALUE_COMMAND_PARAM].size;

	memcpy(&command, command_src, command_size);
	memcpy(&parameter, parameter_src, parameter_size);
	data.lastCommand = command;

	switch(command)
	{
	case COMMAND_ENABLE_HV_A:
		HAL_GPIO_WritePin(HV_EN_A_GPIO_Port, HV_EN_A_Pin, 1);
		break;
	case COMMAND_ENABLE_HV_B:
		HAL_GPIO_WritePin(HV_EN_B_GPIO_Port, HV_EN_B_Pin, 1);
		break;
	case COMMAND_DISABLE_HV_A:
		HAL_GPIO_WritePin(HV_EN_A_GPIO_Port, HV_EN_A_Pin, 0);
		break;
	case COMMAND_DISABLE_HV_B:
		HAL_GPIO_WritePin(HV_EN_B_GPIO_Port, HV_EN_B_Pin, 0);
		break;
	case COMMAND_SET_VOLTAGE_A:
		//
		break;
	case COMMAND_SET_VOLTAGE_B:
		//
		break;
	case COMMAND_SET_HV_POLARITY_A:
		HAL_GPIO_WritePin(HV_POL_A_GPIO_Port, HV_POL_A_Pin, (uint8_t)parameter);
		break;
	case COMMAND_SET_HV_POLARITY_B:
		HAL_GPIO_WritePin(HV_POL_B_GPIO_Port, HV_POL_B_Pin, (uint8_t)parameter);
		break;
	case COMMAND_SET_SENSITIVITY_A:
		HAL_GPIO_WritePin(SENSITIVITY_A_GPIO_Port, SENSITIVITY_A_Pin, (uint8_t)parameter);
		break;
	case COMMAND_SET_SENSITIVITY_B:
		HAL_GPIO_WritePin(SENSITIVITY_B_GPIO_Port, SENSITIVITY_B_Pin, (uint8_t)parameter);
		break;
	default:
		break;
	}
}

int tcp_server_rx_get_last_command_code()
{
	return data.lastCommand;
}



