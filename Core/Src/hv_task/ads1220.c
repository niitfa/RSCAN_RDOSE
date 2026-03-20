/*
 * ads1220.c
 *
 *  Created on: Feb 17, 2026
 *      Author: Kirill
 */
#include "ads1220.h"
#include <string.h>
#include <math.h>

/* imprecise small delay */
__STATIC_INLINE void delayUs(volatile uint32_t us)
{
	us *= (SystemCoreClock / 1000000);
	while (us--);
}

static void spi_command(ads1220_t* self, uint8_t cmd);
static void spi_select(ads1220_t* self);
static void spi_deselect(ads1220_t* self);

static void reset(ads1220_t* self);
static void comm_wreg(ads1220_t* self, uint8_t rr, uint8_t nn);
static void setup_reg0(ads1220_t* self, uint8_t mux, uint8_t gain, uint8_t pga_bypass);
static void setup_reg1(ads1220_t* self, uint8_t dr, uint8_t mode, uint8_t cm, uint8_t ts, uint8_t bcs);
static void setup_reg2(ads1220_t* self, uint8_t vref, uint8_t filt, uint8_t psw, uint8_t idac);
static void setup_reg3(ads1220_t* self, uint8_t i1mux, uint8_t i2mux, uint8_t drdym);

static void comm_startsync(ads1220_t* self);
static void read_dout(ads1220_t* self);
static void check_negative_24_to_32(int* val);
static int16_t get_complement_code_16bit(int16_t val);

int ads1220_init(ads1220_t *self)
{
	memset(self, 0, sizeof(*self));
	// spi
	self->hspi = NULL;
	// cs
	self->portCS = NULL;
	self->pinCS = 0;
	// xdrdy
	self->portXDRDY = NULL;
	self->pinXDRDY = 0;
	// Vref
	self->Vref_neg = 0;
	self->Vref_pos = 2.048;

	self->measureMode = MEASURE_CH_A;
	self->lastOutputValue_A = 0;
	self->lastOutputValue_B = 0;
	self->lastOutputValue_Temp = 0;

	self->maxOutputValue = (int)pow(2, 24);

	self->dataRate = 0b000;
	self->gain = 0b000;
	return 0;
}

void ads1220_set_spi(ads1220_t *self, SPI_HandleTypeDef* spi)
{
	self->hspi = spi;
}

void ads1220_set_xdrdy(ads1220_t *self, GPIO_TypeDef* port, uint16_t pin)
{
	self->portXDRDY = port;
	self->pinXDRDY = pin;
}

void ads1220_set_cs(ads1220_t *self, GPIO_TypeDef* port, uint16_t pin)
{
	self->portCS = port;
	self->pinCS = pin;
}

void ads1220_set_reference_voltage(ads1220_t *self, double negative, double positive)
{
	self->Vref_neg = negative;
	self->Vref_pos = positive;
}

void ads1220_setup(ads1220_t *self)
{
	reset(self);
	delayUs(1000);

	// configuration register 0
	spi_select(self);
	comm_wreg(self, 0, 0);
	setup_reg0(self, 0b0000, self->gain, 0b0); // channel
	spi_deselect(self);
	delayUs(10);

	// configuration register 1
	spi_select(self);
	comm_wreg(self, 1, 0);
	setup_reg1(self, self->dataRate, 0b01, 0b1, 0b0, 0b0); // temp
	spi_deselect(self);
	delayUs(10);

	// configuration register 2
	spi_select(self);
	comm_wreg(self, 2, 0);
	setup_reg2(self, 0b00, 0b00, 0b0, 0b000);
	spi_deselect(self);
	delayUs(10);

	// configuration register 3
	spi_select(self);
	comm_wreg(self, 3, 0);
	setup_reg3(self, 0b000, 0b000, 0b0);
	spi_deselect(self);
	delayUs(10);

	// command start sync
	spi_select(self);
	comm_startsync(self);
	spi_deselect(self);
	delayUs(10);
}

void ads1220_update(ads1220_t *self)
{
	// spi read data
	spi_select(self);
	//comm_rdata(self);
	read_dout(self);
	spi_deselect(self);
	// command to register to switch mux

}

int ads1220_get_output_A(ads1220_t *self)
{
	return self->lastOutputValue_A;
}

int ads1220_get_output_B(ads1220_t *self)
{
	return self->lastOutputValue_B;
}

int16_t ads1220_get_output_temp(ads1220_t *self)
{
	int16_t temp14bit = (int16_t)(self->lastOutputValue_Temp >> 10);
	uint8_t msb = (temp14bit >> 13) % 2; // 13-th bit
	if(msb) // negative value
	{
		temp14bit = get_complement_code_16bit(temp14bit);
	}
	temp14bit &= 0b0011111111111111;
	if(msb)
	{
		temp14bit = -temp14bit;
	}
	return temp14bit >> 5;
}

uint16_t ads1220_get_xdrdy_pin(ads1220_t *self)
{
	return self->pinXDRDY;
}

// private
static void spi_command(ads1220_t* self, uint8_t cmd)
{
	if(self->hspi)
	{
		HAL_SPI_Transmit(self->hspi, &cmd, 1, 10);
		while(HAL_SPI_GetState(self->hspi) != HAL_SPI_STATE_READY);
	}
}

static void spi_select(ads1220_t* self)
{
	if(self->portCS)
	{
		HAL_GPIO_WritePin(self->portCS, self->pinCS, GPIO_PIN_RESET);
	}
}

static void spi_deselect(ads1220_t* self)
{
	if(self->portCS)
	{
		HAL_GPIO_WritePin(self->portCS, self->pinCS, GPIO_PIN_SET);
	}
}

static void reset(ads1220_t* self)
{
	spi_select(self);
	spi_command(self, 0b00000110);
	spi_deselect(self);
}

static void comm_wreg(ads1220_t* self, uint8_t rr, uint8_t nn)
{
	spi_command(self, 0b01000000 | (rr << 2) | nn );
}

static void setup_reg0(ads1220_t* self, uint8_t mux, uint8_t gain, uint8_t pga_bypass)
{
	spi_command(self, (mux << 4) | (gain << 1) | pga_bypass);
}

static void setup_reg1(ads1220_t* self, uint8_t dr, uint8_t mode, uint8_t cm, uint8_t ts, uint8_t bcs)
{
	spi_command(self, (dr << 5) | (mode << 3) | (cm << 2) | (ts << 1) | bcs);
}

static void setup_reg2(ads1220_t* self, uint8_t vref, uint8_t filt, uint8_t psw, uint8_t idac)
{
	spi_command(self, (vref << 6) | (filt << 4) | (psw << 3) | idac);
}

static void setup_reg3(ads1220_t* self, uint8_t i1mux, uint8_t i2mux, uint8_t drdym)
{
	spi_command(self, (i1mux << 5) | (i2mux << 2) | (drdym << 1));
}

static void comm_startsync(ads1220_t* self)
{
	spi_command(self, 0b00001000);
}

static void read_dout(ads1220_t* self)
{
	const uint8_t kDataSizeBytes = 3;
	const uint8_t kBufferSizeBytes = 4;
	uint8_t rxBytes [kBufferSizeBytes];

	uint8_t txBytes [kBufferSizeBytes];
	uint8_t cr0_chA = (self->gain << 1) | 0b01010000;
	uint8_t cr0_chB = (self->gain << 1) | 0b00000000;
	uint8_t cr1_temp = (self->dataRate << 5) | 0b00001110;
	uint8_t cr1_ch =  (self->dataRate << 5) |0b00001100;
	uint8_t wreg = 0b01000001;
	memset(txBytes, 0, kDataSizeBytes);

	switch(self->measureMode)
	{
	case MEASURE_CH_A:
		memcpy(txBytes + 0, &wreg, 1);
		memcpy(txBytes + 1, &cr0_chB, 1);
		memcpy(txBytes + 2, &cr1_ch, 1);
		break;
	case MEASURE_CH_B:
		memcpy(txBytes + 0, &wreg, 1);
		memcpy(txBytes + 1, &cr0_chB, 1); // not imp
		memcpy(txBytes + 2, &cr1_temp, 1);
		break;
	case MEASURE_TEMP:
		memcpy(txBytes + 0, &wreg, 1);
		memcpy(txBytes + 1, &cr0_chA, 1);
		memcpy(txBytes + 2, &cr1_ch, 1);
		break;
	}

	if(self->hspi)
	{
		memset(rxBytes, 0, kBufferSizeBytes);
		int i;
		for(i = 0; i < kDataSizeBytes; ++i)
		{
			HAL_SPI_TransmitReceive(
					self->hspi, txBytes + i, rxBytes + kDataSizeBytes - i - 1, 1, 10
				);
			while(HAL_SPI_GetState(self->hspi) != HAL_SPI_STATE_READY)
				;
		}
	}

	int res = *(int32_t*)rxBytes;
	check_negative_24_to_32(&res);

	switch(self->measureMode)
	{
	case MEASURE_CH_A:
		self->lastOutputValue_A = res;
		break;
	case MEASURE_CH_B:
		self->lastOutputValue_B = res;
		break;
	case MEASURE_TEMP:
		self->lastOutputValue_Temp = res;
		break;
	}

	self->measureMode = (self->measureMode + 1) % 3;
}

static void check_negative_24_to_32(int* val)
{
	if ((*val >> 23) & (int)1)
	{
		*val |= 0xFF000000;
	}
}

static int16_t get_complement_code_16bit(int16_t val)
{
	val--;
	int16_t res = 0;
	int i;
	for(i = 0; i < 16; i++)
	{
		if((val >> i) % 2) // i-th bit 1
		{
		}
		else  // i-th bit 0
		{
			res |= ((int16_t)1 << i);
		}
	}
	return res;
}







