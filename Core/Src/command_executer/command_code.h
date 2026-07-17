/**
 * @file command_code.h
 * @brief Файл с кодами команд, принимаемыми от TCP-клиента и по запросам UART
 */

#ifndef SRC_RDOSE_COMMAND_CODE_H_
#define SRC_RDOSE_COMMAND_CODE_H_

#include <stdint.h>

/**
 * @brief Коды принимаемых команд
 */
typedef enum
{
	// executions - tcp and uart
	COMMAND_EMPTY = 0x00, ///< Пустая команда
	COMMAND_ENABLE_HV_A = 0x01, ///< Включить высокое напряжение на канале A
	COMMAND_ENABLE_HV_B = 0x02, ///< Включить высокое напряжение на канале B
	COMMAND_DISABLE_HV_A  = 0x03, ///< Выключить высокое напряжение на канале A
	COMMAND_DISABLE_HV_B = 0x04, ///< Выключить высокое напряжение на канале B
	COMMAND_SET_VOLTAGE_A = 0x05, ///< Задать высокое напряжение на канале A
	COMMAND_SET_VOLTAGE_B = 0x06, ///< Задать высокое напряжение на канале B
	COMMAND_SET_HV_POLARITY_A = 0x07, ///< Задать полярность высокого напряжения на канале A
	COMMAND_SET_HV_POLARITY_B = 0x08, ///< Задать полярность высокого напряжения на канале B
	COMMAND_SET_SENSITIVITY_A = 0x09, ///< Задать чувствительность на канале A
	COMMAND_SET_SENSITIVITY_B = 0x0A, ///< Задать чувствительность на канале B

	// telemetry request - uart only
	COMMAND_GET_MESSAGE_NO = 0x80, ///< Запрос номера фрейма измерения
	COMMAND_GET_DOSE_A = 0x81, ///< Запрос сигнала камеры с канала A
	COMMAND_GET_DOSE_B = 0x82, ///< Запрос сигнала камеры с канала B
	COMMAND_GET_HV_A = 0x83, ///< Запрос высокого напряжения на канале A
	COMMAND_GET_HV_B = 0x84, ///< Запрос высокого напряжения на канале B
	COMMAND_GET_HV_POL_A = 0x85, ///< Запрос полярности высокого напряжения на канале A
	COMMAND_GET_HV_POL_B = 0x86, ///< Запрос полярности высокого напряжения на канале B
	COMMAND_GET_HV_EN_A = 0x87, ///< Запрос вкл/выкл высокого напряжения на канале A
	COMMAND_GET_HV_EN_B = 0x88, ///< Запрос вкл/выкл высокого напряжения на канале B
	COMMAND_GET_SENSITIVITY_A = 0x89, ///< Запрос чувствительности на канале A
	COMMAND_GET_SENSITIVITY_B = 0x8A, ///< Запрос чувствительности на канале B
	COMMAND_GET_TEMP = 0x8B ///< Запрос температуры внутреннего объема
} command_code_t;

#endif /* SRC_RDOSE_COMMAND_CODE_H_ */
