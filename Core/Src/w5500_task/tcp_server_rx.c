/**
 * @file tcp_server_rx.c
 * @brief Файл содержит информацию об входящем пакете,
 * выполнении полученных команд и определения соответствующих
 * функций и структур данных
 * @details Для адаптации этого модуля к другим проектам
 * 1) Изменить размер буфера TCP_RX_BUFF_SIZE
 * 2) Изменить содержимое перечисления value_code_t - оффсеты и размеры полей сообщения
 * 3) Изменить содержимое перечисления tcp_command_code_t - коды команд
 * 4) Переписать содержимое функции tcp_server_rx_handle() - обработку сообщения
 * @see TCP_RX_BUFF_SIZE value_code_t tcp_command_code_t tcp_server_rx_handle()
 */

#include "tcp_server_rx.h"
#include "hv_task.h"
#include <stdint.h>
#include <string.h>
#include "main.h"

#include "signal_task.h"

#define TCP_RX_BUFF_SIZE 64 ///< Размер входящего сообщения
/**
 * @brief Перечисление описывает структуру входящего сообщения
 */
typedef enum
{
	VALUE_COMMAND_CODE, ///< Код команды
	VALUE_COMMAND_PARAM, ///< Параметр команды
	VALUE_SIZE ///< Количество элементов
} value_code_t;
/**
 * @brief Структура хранит для конкретного поля в пакете - оффсет и размер
 */
typedef struct
{
	uint16_t offset; ///< Оффсет поля
	uint8_t size; ///< Размер поля
} value_address_t;
/**
 * @brief Основная структура модуля,
 * содержащая информацию о входящем пакете
 */
typedef struct
{
	uint8_t buff[TCP_RX_BUFF_SIZE]; ///< Буфер входящего сообщения
	value_address_t rx_values[VALUE_SIZE];///< Массив, хранящий оффсеты и размеры всех полей пакета
	command_code_t lastCommand; ///< Код последней принятой команды
} tcp_server_rx_t;
/**
 * @brief Единственный экземпляр структуры tcp_server_rx_t.
 * Работа всех определенных в файле функций происходит только с ней
 */
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
		hv_set_output_A(0);
		break;
	case COMMAND_DISABLE_HV_B:
		HAL_GPIO_WritePin(HV_EN_B_GPIO_Port, HV_EN_B_Pin, 0);
		hv_set_output_B(0);
		break;
	case COMMAND_SET_VOLTAGE_A:
		hv_set_output_A(parameter);
		break;
	case COMMAND_SET_VOLTAGE_B:
		hv_set_output_B(parameter);
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
		// debug
	case COMMAND_SET_ADC_DR_A:
		ads1246_set_dr(signal_get_adc_A(), (uint8_t)parameter);
		ads1246_setup_sys0(signal_get_adc_A());
		break;
	case COMMAND_SET_ADC_DR_B:
		ads1246_set_dr(signal_get_adc_B(), (uint8_t)parameter);
		ads1246_setup_sys0(signal_get_adc_B());
		break;
	case COMMAND_SET_ADC_GAIN_A:
		ads1246_set_pga(signal_get_adc_A(), (uint8_t)parameter);
		ads1246_setup_sys0(signal_get_adc_A());
		break;
	case COMMAND_SET_ADC_GAIN_B:
		ads1246_set_pga(signal_get_adc_B(), (uint8_t)parameter);
		ads1246_setup_sys0(signal_get_adc_B());
		break;
	default:
		break;
	}
}
