/*
 * ads1220.h
 *
 *  Created on: Feb 17, 2026
 *      Author: Kirill
 */

#ifndef SRC_HV_ADC_TASK_ADS1220_H_
#define SRC_HV_ADC_TASK_ADS1220_H_

#include "stm32f4xx_hal.h"

// SPI frequency up to 20 MHz
// SPI mode - CPOL = LOW, CPHA = 2

typedef enum
{
	MEASURE_CH_A,
	MEASURE_CH_B,
	MEASURE_TEMP
} ads1220_measure_mode_t;

typedef struct
{
	// spi
	SPI_HandleTypeDef* hspi;

	// cs
	GPIO_TypeDef* portCS;
	uint16_t pinCS;

	// drdy
	GPIO_TypeDef* portXDRDY;
	uint16_t pinXDRDY;

	double Vref_pos;
	double Vref_neg;

	ads1220_measure_mode_t measureMode;
	int lastOutputValue_A;
	int lastOutputValue_B;
	int lastOutputValue_Temp;
	int maxOutputValue;

	// config register options
	uint8_t dataRate;
	uint8_t gain;
} ads1220_t;

int ads1220_init(ads1220_t *self);
void ads1220_set_spi(ads1220_t *self, SPI_HandleTypeDef* spi);
void ads1220_set_xdrdy(ads1220_t *self, GPIO_TypeDef* port, uint16_t pin);
void ads1220_set_cs(ads1220_t *self, GPIO_TypeDef* port, uint16_t pin);
void ads1220_set_reference_voltage(ads1220_t *self, double negative, double positive);
void ads1220_setup(ads1220_t *self);
void ads1220_update(ads1220_t *self);
int ads1220_get_output_A(ads1220_t *self);
int ads1220_get_output_B(ads1220_t *self);
int16_t ads1220_get_output_temp(ads1220_t *self);

uint16_t ads1220_get_xdrdy_pin(ads1220_t *self);




#endif /* SRC_HV_ADC_TASK_ADS1220_H_ */
