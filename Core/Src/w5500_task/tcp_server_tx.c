/*
 * trc_server_tx.c
 *
 *  Created on: Dec 25, 2025
 *      Author: Kirill
 */

#include "tcp_server_tx.h"
#include "hv_task.h"
#include "signal_task.h"
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include "main.h"

// For other projects change
// 1) TCP_TX_BUFF_SIZE - buffer size
// 2) value_code_t - message content
// 3) tx_values[VALUE_SIZE] - values` addresses and sizes
// 4) tcp_server_tx_update() - update data

#define TCP_TX_BUFF_SIZE 64

static void emulate_data();

typedef enum
{
	VALUE_MESSAGE_NO,
	VALUE_DOSE_A,
	VALUE_DOSE_B,
	VALUE_HV_A,
	VALUE_HV_B,
	VALUE_TEMP_A,
	VALUE_TEMP_B,
	VALUE_SENSITIVITY_A,
	VALUE_SENSITIVITY_B,
	VALUE_HV_POLARITY_A,
	VALUE_HV_POLARITY_B,
	VALUE_HV_ENABLED_A,
	VALUE_HV_ENABLED_B,
	VALUE_SIZE
} value_code_t;

typedef struct
{
	uint16_t offset;
	uint16_t size;
} value_address_t;

static void tcp_server_tx_set_value(value_code_t code, void* src);

typedef struct
{
	uint8_t buff[TCP_TX_BUFF_SIZE];
	value_address_t tx_values[VALUE_SIZE];
	uint32_t message_id;
	int32_t dose_A;
	int32_t dose_B;
	int32_t hv_A;
	int32_t hv_B;
	int32_t temp_A;
	int32_t temp_B;
	uint8_t sensitivity_A;
	uint8_t sensitivity_B;
	uint8_t polarity_A;
	uint8_t polarity_B;
	uint8_t hv_enabled_A;
	uint8_t hv_enabled_B;
} tcp_server_tx_t;

static tcp_server_tx_t data;

void tcp_server_tx_init()
{
	memset (&data, 0, sizeof (data));

	data.tx_values[VALUE_MESSAGE_NO] 	= (value_address_t){0, 4};
	data.tx_values[VALUE_DOSE_A] 		= (value_address_t){4, 4};
	data.tx_values[VALUE_DOSE_B] 		= (value_address_t){8, 4};
	data.tx_values[VALUE_HV_A] 			= (value_address_t){12, 4};
	data.tx_values[VALUE_HV_B] 			= (value_address_t){16, 4};
	data.tx_values[VALUE_TEMP_A] 		= (value_address_t){20, 4};
	data.tx_values[VALUE_TEMP_B] 		= (value_address_t){24, 4};
	data.tx_values[VALUE_SENSITIVITY_A] = (value_address_t){28, 1};
	data.tx_values[VALUE_SENSITIVITY_B]	= (value_address_t){29, 1};
	data.tx_values[VALUE_HV_POLARITY_A] = (value_address_t){30, 1};
	data.tx_values[VALUE_HV_POLARITY_B] = (value_address_t){31, 1};
	data.tx_values[VALUE_HV_ENABLED_A] = (value_address_t){32, 1};
	data.tx_values[VALUE_HV_ENABLED_B] = (value_address_t){33, 1};
}

uint8_t* tcp_server_tx_get()
{
	return data.buff;
}

int tcp_server_tx_size()
{
	return TCP_TX_BUFF_SIZE;
}

void tcp_server_tx_update()
{
	// update data in message
	// in general it depends on rx but in this project message content is constant
	// uint8_t* rx_message = tcp_server_rx_get();...

	data.message_id++;
	//emulate_data();

	data.dose_A = signal_get_output_A();
	data.dose_B = signal_get_output_B();
	data.hv_enabled_A = HAL_GPIO_ReadPin(HV_EN_A_GPIO_Port, HV_EN_A_Pin);
	data.hv_enabled_B = HAL_GPIO_ReadPin(HV_EN_B_GPIO_Port, HV_EN_B_Pin);
	data.hv_A = hv_get_output_A();  // raw data
	data.hv_B = hv_get_output_B();  // raw data
	data.temp_A = hv_get_output_temp();
	data.temp_B = hv_get_output_temp();
	data.sensitivity_A = HAL_GPIO_ReadPin(SENSITIVITY_A_GPIO_Port, SENSITIVITY_A_Pin);
	data.sensitivity_B = HAL_GPIO_ReadPin(SENSITIVITY_B_GPIO_Port, SENSITIVITY_B_Pin);
	data.polarity_A = HAL_GPIO_ReadPin(HV_POL_A_GPIO_Port, HV_POL_A_Pin);
	data.polarity_B = HAL_GPIO_ReadPin(HV_POL_B_GPIO_Port, HV_POL_B_Pin);
	data.hv_enabled_A = HAL_GPIO_ReadPin(HV_EN_A_GPIO_Port, HV_EN_A_Pin);
	data.hv_enabled_B = HAL_GPIO_ReadPin(HV_EN_B_GPIO_Port, HV_EN_B_Pin);

	tcp_server_tx_set_value(VALUE_MESSAGE_NO, &data.message_id);
	tcp_server_tx_set_value(VALUE_DOSE_A, &data.dose_A);
	tcp_server_tx_set_value(VALUE_DOSE_B, &data.dose_B);
	tcp_server_tx_set_value(VALUE_HV_A, &data.hv_A);
	tcp_server_tx_set_value(VALUE_HV_B, &data.hv_B);
	tcp_server_tx_set_value(VALUE_TEMP_A, &data.temp_A);
	tcp_server_tx_set_value(VALUE_TEMP_B, &data.temp_B);
	tcp_server_tx_set_value(VALUE_SENSITIVITY_A, &data.sensitivity_A);
	tcp_server_tx_set_value(VALUE_SENSITIVITY_B, &data.sensitivity_B);
	tcp_server_tx_set_value(VALUE_HV_POLARITY_A, &data.polarity_A);
	tcp_server_tx_set_value(VALUE_HV_POLARITY_B, &data.polarity_B);
	tcp_server_tx_set_value(VALUE_HV_ENABLED_A, &data.hv_enabled_A);
	tcp_server_tx_set_value(VALUE_HV_ENABLED_B, &data.hv_enabled_B);
}

int tcp_server_tx_get_msg_id()
{
	return data.message_id;
}

static void tcp_server_tx_set_value(value_code_t code, void* src)
{
	uint8_t* dest = data.buff + data.tx_values[code].offset;
	uint16_t size = data.tx_values[code].size;
	memcpy(dest, src, size);
}

static void emulate_data()
{
	data.dose_A = 1000000 + 100000 * sin( 0.05 * data.message_id);
	data.dose_B = 1000000 + 100000 * sin( 0.2 * data.message_id);
	data.hv_A = 100;
	data.hv_B = 200;
	data.temp_A = 25000 + rand() % 100;
	data.temp_B = 25000 + rand() % 100;
	data.sensitivity_A = 0;
	data.sensitivity_B = 1;
	data.polarity_A = 0;
	data.polarity_B = 1;
}
