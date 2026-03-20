/*
 * hv_adc_task.c
 *
 *  Created on: Jan 12, 2026
 *      Author: Kirill
 */

#include "hv_task.h"
#include "ads1220.h"
#include "main.h"

#include <string.h>
#include <math.h>

extern SPI_HandleTypeDef ADS1220_SPI;
extern DAC_HandleTypeDef hdac;

typedef struct
{
	ads1220_t adc;
	int outputVoltageDivider;
	double adcRefVoltageNegative;
	double adcRefVoltagePositive;

	int dacMaxValue;
	int V_high_max;
	int k_in;
	int k_out;
} hv_task_t;

static hv_task_t task;

void hv_task_init()
{
	memset(&task, 0, sizeof(task));
	task.outputVoltageDivider = 500; //
	task.adcRefVoltageNegative = 0;
	task.adcRefVoltagePositive = 2.048;
	task.k_in = pow(2, 23) / (task.adcRefVoltagePositive - task.adcRefVoltageNegative) / task.outputVoltageDivider;


	// init adc
	ads1220_t* adc = &task.adc;
	ads1220_init(adc);
	ads1220_set_spi(adc, &ADS1220_SPI);
	ads1220_set_cs(adc, ADS1220_CS_GPIO_Port, ADS1220_CS_Pin);
	ads1220_set_xdrdy(adc, ADS1220_XDRDY_GPIO_Port, ADS1220_XDRDY_Pin);
	ads1220_set_reference_voltage(
			adc,
			task.adcRefVoltageNegative,
			task.adcRefVoltagePositive
			);
	ads1220_setup(adc);
	HAL_NVIC_EnableIRQ(ADS1220_XDRDY_EXTI_IRQn);

	// dac
	task.dacMaxValue = (int)round(4095 * 3 / 3.3); // 3723
	task.V_high_max = 500;
	task.k_out = 500; //
	HAL_DAC_Start(&hdac,DAC_CHANNEL_1);
	HAL_DAC_Start(&hdac,DAC_CHANNEL_2);

	hv_set_output_A(0);
	hv_set_output_B(0);
}

int hv_get_output_A()
{
	ads1220_t* adc = &task.adc;
	return ads1220_get_output_A(adc) / task.k_in;
}

int hv_get_output_B()
{
	ads1220_t* adc = &task.adc;
	return ads1220_get_output_B(adc) / task.k_in;
}

void hv_set_output_A(int volt)
{
	if(volt >= 0 && volt <= task.V_high_max)
	{
		int val = task.dacMaxValue * volt /  task.V_high_max;
		HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, (uint32_t)val);
	}
}

void hv_set_output_B(int volt)
{
	if(volt >= 0 && volt <= task.V_high_max)
	{
		int val = task.dacMaxValue * volt /  task.V_high_max;
		HAL_DAC_SetValue(&hdac, DAC_CHANNEL_2, DAC_ALIGN_12B_R, (uint32_t)val);
	}
}

int hv_get_output_temp()
{
	ads1220_t* adc = &task.adc;
	return ads1220_get_output_temp(adc);
}

void hv_adc_xdrdy_callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin == ads1220_get_xdrdy_pin(&task.adc))
	{
		ads1220_update(&task.adc);
	}
}






