/**
 * @file hv_task.h
 * @brief Файл с прототипами функций, отвечающих за работу
 * высоковольтной системы, управление высоким напряжением
 * и получение обратной связи по высокому напряжению
 */

#ifndef SRC_HV_ADC_TASK_HV_ADC_TASK_H_
#define SRC_HV_ADC_TASK_HV_ADC_TASK_H_

#include "stm32f4xx_hal.h"
#include <stdint.h>

/**
 * @brief Инициализирует периферию высоковольтной системы
 * - ЦАП для управления входом ВВ источника и
 *  АЦП обратной связи по высокому напряжению
 */
void hv_task_init();
/**
 * @brief Возвращает текущее значение высокого напряжения на канале A
 * @return Высокое напряжения канала A, В
 */
int hv_get_output_A();
/**
 * @brief Возвращает текущее значение высокого напряжения на канале B
 * @return Высокое напряжения канала B, В
 */
int hv_get_output_B();
/**
 * @brief Устанавливает значение высокого напряжения на канале A
 * @param[in] volt Высокое напряжения канала A, В
 */
void hv_set_output_A(int volt);
/**
 * @brief Устанавливает значение высокого напряжения на канале B
 * @param[in] volt Высокое напряжения канала B, В
 */
void hv_set_output_B(int volt);
/**
 * @brief Возвращает температуру внутреннего объема устройства
 * @return Температура, *C
 */
int hv_get_output_temp();
/**
 * @brief Обработчик готовности данных АЦП обратной связи по высокому напряжению
 * @param[in] GPIO_Pin Пин готовности данных xDRDY
 */
void hv_adc_xdrdy_callback(uint16_t GPIO_Pin);

#endif /* SRC_HV_ADC_TASK_HV_ADC_TASK_H_ */
