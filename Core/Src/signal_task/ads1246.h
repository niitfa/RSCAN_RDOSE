/**
 * @file ads1246.h
 * @brief Файл с прототипами функций и определением структуры, отвечающих за работу
 * микросхемы АЦП ADS1246 в режиме DMA
 */
#ifndef SRC_SIGNAL_TASK_ADS1246_H_
#define SRC_SIGNAL_TASK_ADS1246_H_

#include "stm32f4xx_hal.h"

#define ADS1246_RX_BUFF_SIZE 4 ///< Размер буфера чтения
#define ADS1246_TX_BUFF_SIZE 4 ///< Размер буфера записи

/**
 * @brief Основная структура ADS1246
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
	int32_t lastOutputValue; ///< Последнее измеренное значение выхода АЦП
	int32_t maxOutputValue; ///< Максимальное значение выходя АЦП
	uint8_t SYS0_conf; ///< Параметр регистра SYS0
	uint8_t rxBuff[ADS1246_RX_BUFF_SIZE]; ///< Буфер чтения
	uint8_t txBuff[ADS1246_TX_BUFF_SIZE]; ///< Буфер записи
} ads1246_t;
/**
 * @brief Инициализирует АЦП. Присваивает значение полей структуру по умолчанию
 * @param[in] self Указатель на структуру ADS1246
 */
void ads1246_init(ads1246_t* self);
/**
 * @brief Устанавливает SPI контроллера, по которому происходит обмен с ADS1246
 * @param[in] self Указатель на структуру ADS1246
 * @param[in] hspi Указатель на структуру SPI
 */
void ads1246_set_spi(ads1246_t* self, SPI_HandleTypeDef* hspi);
/**
 * @brief Устанавливает вывод CS контроллера, подключенный с соответствующему входу ADS1246
 * @param[in] self Указатель на структуру ADS1246
 * @param[in] port Порт CS
 * @param[in] pin Пин CS
 */
void ads1246_set_cs_pin(ads1246_t* self, GPIO_TypeDef* port, uint16_t pin);
/**
 * @brief Устанавливает вывод xDRDY контроллера, подключенный с соответствующему выходу ADS1246
 * @param[in] self Указатель на структуру ADS1246
 * @param[in] port Порт xDRDY
 * @param[in] pin Пин xDRDY
 */
void ads1246_set_xdrdy_pin(ads1246_t* self, GPIO_TypeDef* port, uint16_t pin);
/**
 * @brief Устанавливает опорное напряжение ADS1246
 * @param[in] self Указатель на структуру ADS1246
 * @param[in] negative Нижнее опорное напряжение (REFN)
 * @param[in] positive Верхнее опорное напряжение (REFP)
 */
void ads1246_set_reference_voltage(ads1246_t* self, double negative, double positive);
/**
 * @brief Перезагружает микросхему и модифицирует ее конфигурационные регистры
 * @param[in] self Указатель на структуру ADS1246
 */
void ads1246_setup(ads1246_t* self);
/**
 * @brief Читает по SPI шине выходное значение
 * @param[in] self Указатель на структуру ADS1246
 */
void ads1246_update(ads1246_t* self);
/**
 * @brief Обработчик завершения чтения выхода АЦП по SPI шине в режиме DMA.
 * Сохраняет полученное значение в структуру
 * @param[in] self Указатель на структуру ADS1246
 */
void ads1246_spi_dma_cplt(ads1246_t *self);
/**
 * @brief Возвращает последнее прочитанное значение выхода АЦП
 * @param[in] self Указатель на структуру ADS1246
 * @return Значение выхода АЦП
 */
int ads1246_get_output(ads1246_t *self);
/**
 * @brief Возвращает номер пина xDRDY контроллера, подключенный с соответствующему выходу ADS1246
 * @param[in] self Указатель на структуру ADS1246
 * @return Пин готовности данных xDRDY
 * @see ads1246_set_xdrdy_pin
 */
uint16_t ads1246_get_xdrdy_pin(ads1246_t *self);

#endif /* SRC_SIGNAL_TASK_ADS1246_H_ */
