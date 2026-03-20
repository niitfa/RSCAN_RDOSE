/*
 * ads1246.h
 *
 *  Created on: Mar 16, 2026
 *      Author: Kirill
 */

#ifndef SRC_SIGNAL_TASK_ADS1246_H_
#define SRC_SIGNAL_TASK_ADS1246_H_

#include "stm32f4xx_hal.h"

#define ADS1246_RX_BUFF_SIZE 4
#define ADS1246_TX_BUFF_SIZE 4

typedef struct
{
	SPI_HandleTypeDef* hspi;
	GPIO_TypeDef* portCS;
	uint16_t pinCS;
	GPIO_TypeDef* portXDRDY;
	uint16_t pinXDRDY;
	double Vref_neg;
	double Vref_pos;
	int32_t lastOutputValue;
	int32_t maxOutputValue;
	uint8_t SYS0_conf;
	uint8_t rxBuff[ADS1246_RX_BUFF_SIZE];
	uint8_t txBuff[ADS1246_TX_BUFF_SIZE];
	uint8_t valueBuff[4];
} ads1246_t;

void ads1246_init(ads1246_t* self);
void ads1246_set_spi(ads1246_t* self, SPI_HandleTypeDef* hspi);
void ads1246_set_cs_pin(ads1246_t* self, GPIO_TypeDef* port, uint16_t pin);
void ads1246_set_xdrdy_pin(ads1246_t* self, GPIO_TypeDef* port, uint16_t pin);
void ads1246_set_reference_voltage(ads1246_t* self, double negative, double positive);
void ads1246_setup(ads1246_t* self);
void ads1246_update(ads1246_t* self);
void ads1246_spi_dma_cplt(ads1246_t *self);
int ads1246_get_output(ads1246_t *self);
uint16_t ads1246_get_xdrdy_pin(ads1246_t *self);



#endif /* SRC_SIGNAL_TASK_ADS1246_H_ */
