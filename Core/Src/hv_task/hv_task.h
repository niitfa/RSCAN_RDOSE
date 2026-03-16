/*
 * hv_adc_task.h
 *
 *  Created on: Jan 12, 2026
 *      Author: Kirill
 */

#ifndef SRC_HV_ADC_TASK_HV_ADC_TASK_H_
#define SRC_HV_ADC_TASK_HV_ADC_TASK_H_

#include "stm32f4xx_hal.h"
#include <stdint.h>

// task
void hv_task_init();

// interface
int hv_get_output_A();
int hv_get_output_B();

void hv_set_output_A(int volt);
void hv_set_output_B(int volt);

int hv_get_output_temp();
void hv_adc_xdrdy_callback(uint16_t GPIO_Pin);

#endif /* SRC_HV_ADC_TASK_HV_ADC_TASK_H_ */
