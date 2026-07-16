/**
 * @file ads1220.h
 * @brief Файл с прототипами функций и определением структуры, отвечающих за работу
 * микросхемы АЦП ADS1220 в режиме DMA. Частота SPI - до 20 МГц,
 * режим SPI - CPOL = LOW, CPHA = 2.
 */

#ifndef SRC_HV_ADC_TASK_ADS1220_H_
#define SRC_HV_ADC_TASK_ADS1220_H_

#include "stm32f4xx_hal.h"

/**
 * @brief Измеряемый канал ADS1220
 */
typedef enum
{
	MEASURE_CH_A, ///< Канал A
	MEASURE_CH_B, ///< Канал B
	MEASURE_TEMP ///< Внутренний канал температуры
} ads1220_measure_mode_t;

/**
 * @brief Основная структура ADS1220
 */
typedef struct
{
	SPI_HandleTypeDef* hspi; ///< SPI
	GPIO_TypeDef* portCS; ///< Порт CS
	uint16_t pinCS; ///< Пин CS
	GPIO_TypeDef* portXDRDY; ///< Порт xDRDY
	uint16_t pinXDRDY; ///< Пин xDRDY
	double Vref_neg; ///< Нижнее опорное напряжение (REFN)
	double Vref_pos; ///< Верхнее опорное напряжение (REFP)
	ads1220_measure_mode_t measureMode; ///< Текущий измеряемый канал ADS1220
	int lastOutputValue_A;  ///< Последнее значение канала A
	int lastOutputValue_B; ///< Последнее значение канала B
	int lastOutputValue_Temp; ///< Последнее значение канала температуры
	int maxOutputValue; ///< Максимальные выходное значение
	uint8_t dataRate; ///< Код частоты обновления данных АЦП
	uint8_t gain; ///< Код внутреннего усиления АЦП
} ads1220_t;

/**
 * @brief Инициализация структуры АЦП - присваивание параметров по умолчанию
 * @param[in] Указатель на структуру ADS1220
 */
int ads1220_init(ads1220_t *self);
/**
 * @brief Устанавливает SPI контроллера, по которому происходит обмен с ADS1220
 * @param[in] self Указатель на структуру ADS1220
 * @param[in] hspi Указатель на структуру SPI
 */
void ads1220_set_spi(ads1220_t *self, SPI_HandleTypeDef* spi);
/**
 * @brief Устанавливает вывод xDRDY контроллера, подключенный с соответствующему выходу ADS1220
 * @param[in] self Указатель на структуру ADS1220
 * @param[in] port Порт xDRDY
 * @param[in] pin Пин xDRDY
 */
void ads1220_set_xdrdy(ads1220_t *self, GPIO_TypeDef* port, uint16_t pin);
/**
 * @brief Устанавливает вывод CS контроллера, подключенный с соответствующему входу ADS1220
 * @param[in] self Указатель на структуру ADS1220
 * @param[in] port Порт CS
 * @param[in] pin Пин CS
 */
void ads1220_set_cs(ads1220_t *self, GPIO_TypeDef* port, uint16_t pin);
/**
 * @brief Устанавливает опорное напряжение ADS1220
 * @param[in] self Указатель на структуру ADS1220
 * @param[in] negative Нижнее опорное напряжение (REFN)
 * @param[in] positive Верхнее опорное напряжение (REFP)
 */
void ads1220_set_reference_voltage(ads1220_t *self, double negative, double positive);
/**
 * @brief Перезагружает микросхему и модифицирует ее конфигурационные регистры
 * @param[in] self Указатель на структуру ADS1220
 */
void ads1220_setup(ads1220_t *self);
/**
 * @brief Читает по SPI шине выходное значение текущего канала.
 * Каналы переключаются циклически.
 * @see ads1220_measure_mode_t
 * @param[in] self Указатель на структуру ADS1220
 */
void ads1220_update(ads1220_t *self);
/**
 * @brief Возвращает последнее прочитанное значение выхода АЦП с канала A
 * @param[in] self Указатель на структуру ADS1220
 * @return Значение выхода АЦП с канала A
 */
int ads1220_get_output_A(ads1220_t *self);
/**
 * @brief Возвращает последнее прочитанное значение выхода АЦП с канала B
 * @param[in] self Указатель на структуру ADS1220
 * @return Значение выхода АЦП с канала B
 */
int ads1220_get_output_B(ads1220_t *self);
/**
 * @brief Возвращает последнее прочитанное значение выхода АЦП
 * с внутреннего канала температуры
 * @param[in] self Указатель на структуру ADS1220
 * @return Значение выхода АЦП с внутреннего канала температуры
 */
int16_t ads1220_get_output_temp(ads1220_t *self);
/**
 * @brief Возвращает номер пина xDRDY контроллера, подключенный с соответствующему выходу ADS1220
 * @param[in] self Указатель на структуру ADS1220
 * @return Пин готовности данных xDRDY
 * @see ads1220_set_xdrdy
 */
uint16_t ads1220_get_xdrdy_pin(ads1220_t *self);

#endif /* SRC_HV_ADC_TASK_ADS1220_H_ */
