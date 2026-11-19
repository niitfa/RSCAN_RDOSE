/**
 * @file flash_data.h
 * @brief ���� � ������������ ������� ��� ������ � ������� �� flash
 * ������ �����������
 */

#ifndef SRC_FLASH_MANAGER_FLASH_DATA_H_
#define SRC_FLASH_MANAGER_FLASH_DATA_H_

#include <stdint.h>
/**
 * @brief ���������� �� flash ����� IP ����������
 * @param[in] ip_src ����� � IP �������� �� ����� 4 ����
 */
void flash_data_set_ip(uint8_t* ip_src);
/**
 * @brief ������ �� flash � ��������� ������� IP ����������
 * �� ����������� ������
 * @param[out] ip_dest ����� �������� �� ����� 4 ����
 */
void flash_data_get_ip(uint8_t* ip_dest);
/**
 * @brief ���������� �� flash ����� ���� ����������
 * @param[in] port ����
 */
void flash_data_set_port(uint16_t port);
/**
 * @brief ������ �� flash � ���������� ������� ���� ����������
 * @return ������� ����
 */
uint16_t flash_data_get_port();
/**
 * @brief ���������� �� flash ����� ���� � IP ����������
 * @param[in] ip_src ����� � IP �������� �� ����� 4 ����
 * @param[in] port ����
 */
void flash_data_set_ip_and_port(uint8_t* ip_src, uint16_t port);
/**
 *
 */
void flash_data_set_k_voltage_positive_A(uint16_t k);
void flash_data_set_k_voltage_negative_A(uint16_t k);
void flash_data_set_k_voltage_positive_B(uint16_t k);
void flash_data_set_k_voltage_negative_B(uint16_t k);

uint16_t flash_data_get_k_voltage_positive_A();
uint16_t flash_data_get_k_voltage_negative_A();
uint16_t flash_data_get_k_voltage_positive_B();
uint16_t flash_data_get_k_voltage_negative_B();

#endif /* SRC_FLASH_MANAGER_FLASH_DATA_H_ */
