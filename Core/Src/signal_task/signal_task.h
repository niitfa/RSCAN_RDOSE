/**
 * @file signal_task.h
 * @brief Файл с прототипами функций, отвечающих за инициализацию
 * АЦП, обрабатывающих сигналы камер каналов A и B, и сбор
 * данных с них. Циклический опрос АЦП происходит в режиме DMA
 */

#ifndef SRC_SIGNAL_TASK_SIGNAL_TASK_H_
#define SRC_SIGNAL_TASK_SIGNAL_TASK_H_

#include "stm32f4xx_hal.h"
#include <stdint.h>
#include "ads1246.h"

/**
 * @brief Инициализация АЦП каналов A и B
 */
void signal_task_init();
/**
 * @brief Возвращает последнее измеренное значение с канала A
 * @return Последнее измеренное значение с канала A
 */
int signal_get_output_A();
/**
 * @brief Возвращает последнее измеренное значение с канала B
 * @return Последнее измеренное значение с канала B
 */
int signal_get_output_B();
/**
 * @brief Возвращает адрес структуры АЦП канала A
 * @return Адрес структуры АЦП канала A
 */
ads1246_t* signal_get_adc_A();
/**
 * @brief Возвращает адрес структуры АЦП канала B
 * @return Адрес структуры АЦП канала B
 */
ads1246_t* signal_get_adc_B();
/**
 * @brief Обработчик прерывания сигнала готовности данных АЦП
 * @param[in] GPIO_Pin Пин
 */
void signal_adc_xdrdy_callback(uint16_t GPIO_Pin);
/**
 * @brief Обработчик прерывания окончания обмена с АЦП по шине SPI в
 * режиме DMA
 * @param[in] hspi SPI
 */
void signal_adc_spi_dma_callback(SPI_HandleTypeDef* hspi);

#endif /* SRC_SIGNAL_TASK_SIGNAL_TASK_H_ */
