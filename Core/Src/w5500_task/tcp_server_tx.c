/**
 * @file tcp_server_tx.c
 * @brief Файл содержит информацию об исходящем пакете и
 * определения соответствующих функций и структур данных
 * @details Для адаптации этого модуля к другим проектам
 * 1) Изменить размер буфера TCP_TX_BUFF_SIZE
 * 2) Изменить содержимое перечисления value_code_t - структуру сообщения
 * 3) Изменить структуру массива tcp_server_tx_t.tx_values - оффсеты и размеры полей сообщения
 * 4) Переписать содержимое функции tcp_server_tx_update() - модификацию сообщения
 * @see TCP_TX_BUFF_SIZE value_code_t tcp_server_tx_t tcp_server_tx_update()
 */

#include "tcp_server_tx.h"
#include "hv_task.h"
#include "signal_task.h"
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include "main.h"

#include "version.h"

#define TCP_TX_BUFF_SIZE 64 ///< Размер исходящего сообщения
/**
 * @brief Эмулирует данные исходящего пакета
 */
static void emulate_data();
/**
 * @brief Перечисление описывает структуру сообщения
 */
typedef enum
{
	VALUE_MESSAGE_NO, ///< Порядковый номер сообщения
	VALUE_DOSE_A, ///< Сигнал АЦП камеры канала A
	VALUE_DOSE_B, ///< Сигнал АЦП камеры канала B
	VALUE_HV_A, ///< Величина высокого напряжения канала A
	VALUE_HV_B, ///< Величина высокого напряжения канала B
	VALUE_TEMP_A, ///< Температура внутреннего объема устройства с датчика A
	VALUE_TEMP_B, ///< Температура внутреннего объема устройства с датчика B
	VALUE_SENSITIVITY_A, ///< Код текущей чувствительности для канала A
	VALUE_SENSITIVITY_B, ///< Код текущей чувствительности для канала B
	VALUE_HV_POLARITY_A, ///< Полярность высокого напряжения с канала A
	VALUE_HV_POLARITY_B, ///< Полярность высокого напряжения с канала B
	VALUE_HV_ENABLED_A, ///< Высокого напряжение вкл/выкл на канале A
	VALUE_HV_ENABLED_B, ///< Высокого напряжение вкл/выкл на канале B
	VALUE_VERSION_MAJOR, ///< Major версия прошивки
	VALUE_VERSION_MINOR, ///< Minor версия прошивки
	VALUE_SIZE ///< Количество элементов в сообщении
} value_code_t;

/**
 * @brief Структура хранит для конкретного поля в пакете - оффсет и размер
 */
typedef struct
{
	uint16_t offset;
	uint16_t size;
} value_address_t;

/**
 * @brief Копирует данные в указанное поле исходящего пакета
 * @param[in] code Код поля сообщения (оффсет и размер подтягиваются из массива data.tx_values)
 * @param[in] src Указатель на входные данные
 */
static void tcp_server_tx_set_value(value_code_t code, void* src);
/**
 * @brief Основная структура модуля, содержащая всю
 * информацию об исходящем пакете
 */
typedef struct
{
	uint8_t buff[TCP_TX_BUFF_SIZE]; ///< Буфер исходящего сообщения
	value_address_t tx_values[VALUE_SIZE]; ///< Массив, хранящий оффсеты и размеры всех полей пакета
	uint32_t message_id; ///< Порядковый номер сообщения
	int32_t dose_A; ///< Величина сигнала с камеры канала A
	int32_t dose_B; ///< Величина сигнала с камеры канала B
	int32_t hv_A;  ///< Величина высокого напряжения с канала A, B
	int32_t hv_B;  ///< Величина высокого напряжения с канала B, B
	int32_t temp_A;  ///< Температура внутреннего объема устройства с датчика A, *C
	int32_t temp_B; ///< Температура внутреннего объема устройства с датчика B, *C
	uint8_t sensitivity_A; ///< Код текущей чувствительности для канала A
	uint8_t sensitivity_B;///< Код текущей чувствительности для канала B
	uint8_t polarity_A;///< Полярность высокого напряжения с канала A
	uint8_t polarity_B;///< Полярность высокого напряжения с канала B
	uint8_t hv_enabled_A; ///< Высокого напряжение вкл/выкл на канале A
	uint8_t hv_enabled_B; ///< Высокого напряжение вкл/выкл на канале B
} tcp_server_tx_t;
/**
 * @brief Единственный экземпляр структуры tcp_server_tx_t.
 * Работа всех определенных в файле функций происходит только с ней
 */
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
	data.tx_values[VALUE_VERSION_MAJOR] = (value_address_t){62, 1};
	data.tx_values[VALUE_VERSION_MINOR] = (value_address_t){63, 1};
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
	data.message_id++;
	data.dose_A = signal_get_output_A();
	data.dose_B = signal_get_output_B();
	data.hv_enabled_A = HAL_GPIO_ReadPin(HV_EN_A_GPIO_Port, HV_EN_A_Pin);
	data.hv_enabled_B = HAL_GPIO_ReadPin(HV_EN_B_GPIO_Port, HV_EN_B_Pin);
	data.hv_A = hv_get_output_A();
	data.hv_B = hv_get_output_B();
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

	uint8_t val = (int)VERSION_MAJOR;
	tcp_server_tx_set_value(VALUE_VERSION_MAJOR, &val);
	val = (int)VERSION_MINOR;
	tcp_server_tx_set_value(VALUE_VERSION_MINOR, &val);
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
