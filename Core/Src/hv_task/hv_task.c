/**
 * @file hv_task.c
 * @brief Файл с определениями функций и структуры, отвечающих за работу
 * высоковольтной системы, управление высоким напряжением
 * и получение обратной связи по высокому напряжению
 */

#include "hv_task.h"
#include "ads1220.h"
#include "main.h"
#include "voltage_corrector.h"
#include "flash_data.h"

#include <string.h>
#include <math.h>
#include <stdlib.h>

/**
 * @brief Читает текущее значение высокого напряжения на канале A (callback для корректора напряжения)
 * @return Высокое напряжения канала A, В
 */
static int read_voltage_A();
/**
 * @brief Читает текущее значение высокого напряжения на канале B (callback для корректора напряжения)
 * @return Высокое напряжения канала B, В
 */
static int read_voltage_B();
/**
 * @brief Устанавливает значение высокого напряжения на канале A
 * @param[in] volt Высокое напряжения канала A, В
 */
static void set_voltage_A(int volt);
/**
 * @brief Устанавливает значение высокого напряжения на канале B
 * @param[in] volt Высокое напряжения канала B, В
 */
static void set_voltage_B(int volt);
/**
 * @brief Инициализирует поправочные коэффициенты высокого напряжения
 */
static void init_voltage_coeffs();

extern SPI_HandleTypeDef ADS1220_SPI; ///< SPI АЦП обратной связи по напряжению
extern DAC_HandleTypeDef hdac; ///< Интегрированный в МК ЦАП, отвечающий за управление
								///< входом ВВ источника
/**
 * @brief Основная структура модуля, содержащая
 * данные об состоянии и характеристиках ВВ системы
 */
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
	voltage_corrector_t correctorA;
	voltage_corrector_t correctorB;

	uint16_t kVoltage_positive_A;
	uint16_t kVoltage_negative_A;
	uint16_t kVoltage_positive_B;
	uint16_t kVoltage_negative_B;

} hv_task_t;
/**
 * @brief Единственный экземпляр структуры hv_task_t.
 * Работа всех определенных в файле функций происходит только с ней
 */
static hv_task_t task;

void hv_task_init(int updatePeriod_ms)
{
	memset(&task, 0, sizeof(task));
	task.outputVoltageDivider = 500;
	task.adcRefVoltageNegative = 0;
	task.adcRefVoltagePositive = 2.048;
	task.k_in = pow(2, 23) / (task.adcRefVoltagePositive - task.adcRefVoltageNegative) / task.outputVoltageDivider;

	init_voltage_coeffs();

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

	voltage_corrector_init(&task.correctorA, set_voltage_A, read_voltage_A, updatePeriod_ms);
	voltage_corrector_init(&task.correctorB, set_voltage_B, read_voltage_B, updatePeriod_ms);
}

int hv_get_output_A()
{
	return voltage_corrector_get_actual_voltage(&task.correctorA);
}

int hv_get_output_B()
{
	return voltage_corrector_get_actual_voltage(&task.correctorB);
}

void hv_set_output_A(int volt)
{
	voltage_corrector_run(&task.correctorA, volt);
}

void hv_set_output_B(int volt)
{
	voltage_corrector_run(&task.correctorB, volt);
}

int hv_get_output_temp()
{
	ads1220_t* adc = &task.adc;
	return ads1220_get_output_temp(adc);
}

void hv_reset_correction_A()
{
	voltage_corrector_stop(&task.correctorA);
}

void hv_reset_correction_B()
{
	voltage_corrector_stop(&task.correctorB);
}

uint8_t hv_get_correction_state_A()
{
	return voltage_corrector_get_state(&task.correctorA);
}

uint8_t hv_get_correction_state_B()
{
	return voltage_corrector_get_state(&task.correctorB);
}

void hv_correction_tick()
{
	voltage_corrector_tick(&task.correctorA);
	voltage_corrector_tick(&task.correctorB);
}

void hv_adc_xdrdy_callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin == ads1220_get_xdrdy_pin(&task.adc))
	{
		ads1220_update(&task.adc);
	}
}

static int read_voltage_A()
{
	ads1220_t* adc = &task.adc;

	int voltage = abs(ads1220_get_output_A(adc) / task.k_in);
	uint8_t polarity = HAL_GPIO_ReadPin(HV_POL_A_GPIO_Port, HV_POL_A_Pin);
	float k = (polarity ? task.kVoltage_positive_A : task.kVoltage_negative_A) / 1000.;
	voltage = voltage * k;

	return voltage;
}
static int read_voltage_B()
{
	ads1220_t* adc = &task.adc;

	int voltage = abs(ads1220_get_output_B(adc) / task.k_in);
	uint8_t polarity = HAL_GPIO_ReadPin(HV_POL_B_GPIO_Port, HV_POL_B_Pin);
	float k = (polarity ? task.kVoltage_positive_B : task.kVoltage_negative_B) / 1000;
	voltage = voltage * k;

	return voltage;
}

static void set_voltage_A(int volt)
{
	if(volt >= 0 && volt <= task.V_high_max)
	{
		// common part
		int val = task.dacMaxValue * volt /  task.V_high_max;
		HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, (uint32_t)val);
	}
}
static void set_voltage_B(int volt)
{
	if(volt >= 0 && volt <= task.V_high_max)
	{
		// common part
		int val = task.dacMaxValue * volt / task.V_high_max;
		HAL_DAC_SetValue(&hdac, DAC_CHANNEL_2, DAC_ALIGN_12B_R, (uint32_t)val);
	}
}

static void init_voltage_coeffs()
{
	uint16_t val = 1;
	// positive A
	val = flash_data_get_k_voltage_positive_A();
	if(val == 0xFFFF) { val = 1000; }
	task.kVoltage_positive_A = val;
	// negative A
	val = flash_data_get_k_voltage_negative_A();
	if(val == 0xFFFF) { val = 1000; }
	task.kVoltage_negative_A = val;
	// positive B
	val = flash_data_get_k_voltage_positive_B();
	if(val == 0xFFFF) { val = 1000; }
	task.kVoltage_positive_B = val;
	// negative B
	val = flash_data_get_k_voltage_negative_B();
	if(val == 0xFFFF) { val = 1000; }
	task.kVoltage_negative_B = val;
}






