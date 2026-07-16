/**
 * @file wiznet_api.h
 * @brief Файл с макросами, определяющими цифровой интерфейс, функциями,
 * определяющими работу с портами чипа w5500, функцией инициазизации w5500
 */

#ifndef SRC_W5500_ETHERNET_WIZNET_API_H_
#define SRC_W5500_ETHERNET_WIZNET_API_H_

#include "wizchip_conf.h"
#include "stm32f4xx_hal.h"
#include "main.h"

extern SPI_HandleTypeDef hspi1; ///< ссылка на SPI структуру, к которой подключенен w5500

#define WIZNET_SPI_INSTANCE 	&hspi1 ///< ссылка на SPI структуру, к которой подключенен w5500
#define WIZNET_CS_PORT 			W5500_CS_GPIO_Port ///< порт вывода Chip Select (CS)
#define WIZNET_CS_PIN			W5500_CS_Pin ///< пин вывода Chip Select (CS)
#define WIZNET_RST_PORT			W5500_RESET_GPIO_Port ///< порт вывода Reset (RST)
#define WIZNET_RST_PIN			W5500_RESET_Pin ///< пин вывода Reset (RST)
#define WIZNET_INT_PORT			NULL ///< порт вывода Interrrupt (INT)
#define WIZNET_INT_PIN			0 ///< пин вывода Interrrupt (INT)
/**
 * @brief Изменение сигнала CS 1->0
 */
void 	W5500_Select(void);
/**
 * @brief Изменение сигнала CS 0->1
 */
void 	W5500_Deselect(void);
/**
 * @brief Чтение шины SPI
 * @param[out] buff Буфер для хранения результата
 * @param[in] len Размер буфера
 */
void 	W5500_ReadBuff(uint8_t* buff, uint16_t len);
/**
 * @brief Запись на шину SPI
 * @param[in] buff Буфер с данными
 * @param[in] len Размер буфера
 */
void 	W5500_WriteBuff(uint8_t* buff, uint16_t len);
/**
 * @brief Чтение 1 байта по SPI
 * @return Результат чтения
 */
uint8_t W5500_ReadByte(void);
/**
 * @brief Запись 1 байта по SPI
 * @param[in] byte Байт для записи
 */
void 	W5500_WriteByte(uint8_t byte);
/**
 * @brief Инициализация чипа w5500. Формат ip_0.ip_1.ip_2.ip_3
 * @param[in] ip_0 ip_0.xxx.xxx.xxx
 * @param[in] ip_1 xxx.ip_1.xxx.xxx
 * @param[in] ip_2 xxx.xxx.ip_2.xxx
 * @param[in] ip_3 xxx.xxx.xxx.ip_3
 */
void	W5500_Init(uint8_t ip_0, uint8_t ip_1, uint8_t ip_2, uint8_t ip_3);

#endif /* SRC_W5500_ETHERNET_WIZNET_API_H_ */
