/*
 * signal_task.h
 *
 *  Created on: Mar 16, 2026
 *      Author: Kirill
 */

#ifndef SRC_SIGNAL_TASK_SIGNAL_TASK_H_
#define SRC_SIGNAL_TASK_SIGNAL_TASK_H_

#include "stm32f4xx_hal.h"
#include <stdint.h>
#include "ads1246.h"

// task
void signal_task_init();

// interface
int signal_get_output_A();
int signal_get_output_B();
void signal_adc_xdrdy_callback(uint16_t GPIO_Pin);
void signal_adc_spi_dma_callback(SPI_HandleTypeDef* hspi);


#endif /* SRC_SIGNAL_TASK_SIGNAL_TASK_H_ */
