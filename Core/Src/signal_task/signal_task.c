/**
 * @file signal_task.c
 * @brief Файл с определениями функций и структуры, отвечающих за инициализацию
 * АЦП, обрабатывающих сигналы камер каналов A и B, и сбор
 * данных с них
 */

#include "signal_task.h"
#include "main.h"

#include <string.h>
#include <math.h>

extern SPI_HandleTypeDef ADS1246_A_SPI; ///< SPI АЦП камеры канала A
extern SPI_HandleTypeDef ADS1246_B_SPI; ///< SPI АЦП камеры канала B
/**
 * @brief Основная структура модуля, содержащая
 * данные об задании обработки сигналов в АЦП камер
 */
typedef struct
{
	ads1246_t adc_A;
	ads1246_t adc_B;
} signal_task_t;
/**
 * @brief Единственный экземпляр структуры signal_task_t.
 * Работа всех определенных в файле функций происходит только с ней
 */
static signal_task_t task;

void signal_task_init()
{
	memset(&task, 0, sizeof(task));

	// init adc
	ads1246_t* adc = &task.adc_A;
	ads1246_init(adc);
	ads1246_set_spi(adc, &ADS1246_A_SPI);
	ads1246_set_cs_pin(adc, ADS1246_CS_A_GPIO_Port, ADS1246_CS_A_Pin);
	ads1246_set_xdrdy_pin(adc, ADS1246_XDRDY_A_GPIO_Port, ADS1246_XDRDY_A_Pin);
	ads1246_set_reference_voltage(adc, 0, 2.048);
	ads1246_wakeup(adc);
	ads1246_set_dr(adc, 0b0010);
	ads1246_set_pga(adc, 0b000);
	ads1246_setup_sys0(adc);

	adc = &task.adc_B;
	ads1246_init(adc);
	ads1246_set_spi(adc, &ADS1246_B_SPI);
	ads1246_set_cs_pin(adc, ADS1246_CS_B_GPIO_Port, ADS1246_CS_B_Pin);
	ads1246_set_xdrdy_pin(adc, ADS1246_XDRDY_B_GPIO_Port, ADS1246_XDRDY_B_Pin);
	ads1246_set_reference_voltage(adc, 0, 2.048);
	ads1246_wakeup(adc);
	ads1246_set_dr(adc, 0b0010);
	ads1246_set_pga(adc, 0b000);
	ads1246_setup_sys0(adc);

	// enable irq
	HAL_NVIC_EnableIRQ(ADS1246_XDRDY_A_EXTI_IRQn);
	HAL_Delay(25);
	HAL_NVIC_EnableIRQ(ADS1246_XDRDY_B_EXTI_IRQn);
}

int signal_get_output_A()
{
	return ads1246_get_output(&task.adc_A);
}

int signal_get_output_B()
{
	return ads1246_get_output(&task.adc_B);
}

ads1246_t* signal_get_adc_A()
{
	return &task.adc_A;
}

ads1246_t* signal_get_adc_B()
{
	return &task.adc_B;
}

void signal_adc_xdrdy_callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin == ads1246_get_xdrdy_pin(&task.adc_A))
	{
		ads1246_update(&task.adc_A);
	}
	else if (GPIO_Pin == ads1246_get_xdrdy_pin(&task.adc_B))
	{
		ads1246_update(&task.adc_B);
	}
}

void signal_adc_spi_dma_callback(SPI_HandleTypeDef* hspi)
{
	if (hspi == &ADS1246_A_SPI)
	{
	  ads1246_spi_dma_cplt(&task.adc_A);
	}
	else if (hspi == &ADS1246_B_SPI)
	{
	  ads1246_spi_dma_cplt(&task.adc_B);
	}
}

