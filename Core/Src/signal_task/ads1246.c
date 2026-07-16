/**
 * @file ads1246.c
 * @brief Файл с определениями функций, отвечающих за работу
 * микросхемы АЦП ADS1246 в режиме DMA
 */
#include "ads1246.h"
#include <string.h>
#include <math.h>

static const uint8_t CMD_WAKEUP = 0x00; ///< Код команды на пробуждение
static const uint8_t CMD_NOP 	= 0xFF; ///< Код пустой команды
static const uint8_t CMD_RDATA 	= 0x12; ///< Код команды на чтение выхода
static const uint8_t CMD_WREG 	= 0x40; ///< Код команды на запись значения в регистр
static const uint8_t REG_SYS0 	= 0x03; ///< Код регистра SYS0

#define ADC_ADS1246_SPI_TIMEOUT 10 ///< Timeout для блокирующего режима SPI
/**
 * @brief Задержка в микросекундах
 * @param[in] us Величина задержки, мкс
 */
__STATIC_INLINE void delayUs(volatile uint32_t us)
{
	us *= (SystemCoreClock / 1000000);
	while (us--);
}
/**
 * @brief Отправка 1 байта по SPI в блокирующем режиме
 * @param[in] self Указатель на структуру ADS1246
 * @param[in] cmd Байт для отправки
 */
static void spi_command(ads1246_t* self, uint8_t cmd);
/**
 * @brief SPI select (CS 1->0)
 * @param[in] self Указатель на структуру ADS1246
 */
static void spi_select(ads1246_t* self);
/**
 * @brief SPI select (CS 0->1)
 * @param[in] self Указатель на структуру ADS1246
 */
static void spi_deselect(ads1246_t* self);
/**
 * @brief Привести 24-битное значение в соответствие к 32-битному
 * (заполение старших битов в зависимости от знака входной 24-битной величины)
 * @param[in] val Указатель на 24-битное значение
 */
static void check_negative_24_to_32(int32_t* val);

void ads1246_init(ads1246_t* self)
{
	memset(self, 0, sizeof(*self));
	self->hspi = NULL;
	self->portCS = NULL;
	self->pinCS = 0;
	self->portXDRDY = NULL;
	self->pinXDRDY = 0;
	self->Vref_neg = 0;
	self->Vref_pos = 2.048;
	self->lastOutputValue = 0;
	self->maxOutputValue = (int)pow(2, 24);
	uint8_t PGA = 0b000;
	uint8_t DR = 0b0010; // was 0b0010
	self->SYS0_conf = DR | (PGA << 4);

	self->txBuff[0] = CMD_RDATA;
	self->txBuff[1] = CMD_NOP;
	self->txBuff[2] = CMD_NOP;
	self->txBuff[3] = CMD_NOP;
}


void ads1246_set_spi(ads1246_t* self, SPI_HandleTypeDef* hspi)
{
	self->hspi = hspi;
}

void ads1246_set_cs_pin(ads1246_t* self, GPIO_TypeDef* port, uint16_t pin)
{
	self->portCS = port;
	self->pinCS = pin;
}

void ads1246_set_xdrdy_pin(ads1246_t* self, GPIO_TypeDef* port, uint16_t pin)
{
	self->portXDRDY = port;
	self->pinXDRDY = pin;
}

void ads1246_set_reference_voltage(ads1246_t* self, double negative, double positive)
{
	self->Vref_neg = negative;
	self->Vref_pos = positive;
}

void ads1246_setup(ads1246_t* self)
{
	spi_deselect(self);
	delayUs(10000);

	spi_select(self);
	spi_command(self, CMD_WAKEUP);
	spi_deselect(self);
	delayUs(100);

	spi_select(self);
	spi_command(self, CMD_WREG | REG_SYS0);
	spi_command(self, 0); // 1 byte
	spi_command(self, self->SYS0_conf);
	spi_deselect(self);
}

void ads1246_update(ads1246_t* self)
{
	spi_select(self);
	HAL_SPI_TransmitReceive_DMA(self->hspi, self->txBuff, self->rxBuff, ADS1246_RX_BUFF_SIZE);
}

void ads1246_spi_dma_cplt(ads1246_t *self)
{
	spi_deselect(self);

	memset(((uint8_t*)&self->lastOutputValue) + 0, 0, 4);
	memcpy(((uint8_t*)&self->lastOutputValue) + 0, self->rxBuff + 3, 1);
	memcpy(((uint8_t*)&self->lastOutputValue) + 1, self->rxBuff + 2, 1);
	memcpy(((uint8_t*)&self->lastOutputValue) + 2, self->rxBuff + 1, 1);
	check_negative_24_to_32((int32_t*)&self->lastOutputValue);
}

int ads1246_get_output(ads1246_t *self)
{
	return self->lastOutputValue;
}

uint16_t ads1246_get_xdrdy_pin(ads1246_t *self)
{
	return self->pinXDRDY;
}

static void spi_command(ads1246_t* self, uint8_t cmd)
{
	if(self->hspi)
	{
		HAL_SPI_Transmit(self->hspi, &cmd, 1, ADC_ADS1246_SPI_TIMEOUT);
		while(HAL_SPI_GetState(self->hspi) != HAL_SPI_STATE_READY)
			;
	}
}

static void spi_select(ads1246_t* self)
{
	if(self->portCS)
	{
		HAL_GPIO_WritePin(
				self->portCS,
				self->pinCS,
				GPIO_PIN_RESET
				);
	}
}

static void spi_deselect(ads1246_t* self)
{
	if(self->portCS)
	{
		HAL_GPIO_WritePin(
				self->portCS,
				self->pinCS,
				GPIO_PIN_SET
				);
	}
}

static void check_negative_24_to_32(int32_t* val)
{
	if ((*val >> 23) & (int)1)
	{
		*val |= 0xFF000000;
	}
}

#undef ADC_ADS1246_SPI_TIMEOUT
