/**
 * @file flash_data.h
 * @brief Файл с объявлениями функций для работы с данными во flash
 * памяти контроллера
 */

#ifndef SRC_FLASH_MANAGER_FLASH_DATA_H_
#define SRC_FLASH_MANAGER_FLASH_DATA_H_

#include <stdint.h>
/**
 * @brief Записывает во flash новый IP устройства
 * @param[in] ip_src Буфер с IP размером не менее 4 байт
 */
void flash_data_set_ip(uint8_t* ip_src);
/**
 * @brief Читает из flash и сохраняет текущий IP устройства
 * по переданному адресу
 * @param[out] ip_dest Буфер размером не менее 4 байт
 */
void flash_data_get_ip(uint8_t* ip_dest);
/**
 * @brief Записывает во flash новый порт устройства
 * @param[in] port Порт
 */
void flash_data_set_port(uint16_t port);
/**
 * @brief Читает из flash и возвращает текущий порт устройства
 * @return Текущий порт
 */
uint16_t flash_data_get_port();
/**
 * @brief Записывает во flash новый порт и IP устройства
 * @param[in] ip_src Буфер с IP размером не менее 4 байт
 * @param[in] port Порт
 */
void flash_data_set_ip_and_port(uint8_t* ip_src, uint16_t port);

#endif /* SRC_FLASH_MANAGER_FLASH_DATA_H_ */
