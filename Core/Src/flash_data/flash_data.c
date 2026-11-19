/**
 * @file flash_data.c
 * @brief ���� � ������������� ������� ��� ������ � ������� �� flash,
 * � ����� �������� � ������� �������� ��������� ������
 * ������ �����������
 */

#include "flash_data.h"
#include <string.h>

#include "stm32f4xx_hal.h"

/* Sector 8 0x08080000 - 0x0809FFFF */
#define FLASH_DATA_SECTOR FLASH_SECTOR_8 ///< ������ Flash (��. ������� �����������)
#define FLASH_DATA_BASE_ADDR 0x08080000 ///< ����� ������� Flash (��. ������� �����������)

/**
 * @brief �������� ������ ��� �������� �� flash ������
 */
typedef struct
{
	uint8_t ip[4]; ///< IP ����������
	uint16_t port; ///< ���� ����������
	uint16_t kVoltage_positive_A;
	uint16_t kVoltage_negative_A;
	uint16_t kVoltage_positive_B;
	uint16_t kVoltage_negative_B;
} flash_data_t;

/**
 * @brief ��������� ����� �������� flash_data_t ��� ������
 * � ������� �� flash ������
 */
static flash_data_t data;
/**
 * @brief ���������� ������ �� ���������� ������ �� flash
 * @see data
 */
static void flash_data_write();
/**
 * @brief ��������� ������ �� flash � ��������� �����
 * @see data
 */
static void flash_data_read();
/**
 * @brief ������� ������ flash ������, �������� ��������������� ������.
 * ���������� ����� ������� ����� ������ � ���� ������
 */
static void flash_data_erase_sector();

void flash_data_set_ip(uint8_t* ip_src)
{
	if(ip_src)
	{
		flash_data_read();
		memcpy(data.ip, ip_src, 4);
		flash_data_write();
	}
}
void flash_data_get_ip(uint8_t* ip_dest)
{
	if(ip_dest)
	{
		flash_data_read();
		memcpy(ip_dest, data.ip, 4);
	}
}
void flash_data_set_port(uint16_t port)
{
	flash_data_read();
	data.port = port;
	flash_data_write();
}
uint16_t flash_data_get_port()
{
	flash_data_read();
	return data.port;
}

void flash_data_set_ip_and_port(uint8_t* ip_src, uint16_t port)
{
	flash_data_read();
	data.port = port;
	if(ip_src)
	{
		memcpy(data.ip, ip_src, 4);
	}
	flash_data_write();
}

void flash_data_set_k_voltage_positive_A(uint16_t k)
{
	flash_data_read();
	data.kVoltage_positive_A = k;
	flash_data_write();
}
void flash_data_set_k_voltage_negative_A(uint16_t k)
{
	flash_data_read();
	data.kVoltage_negative_A = k;
	flash_data_write();
}
void flash_data_set_k_voltage_positive_B(uint16_t k)
{
	flash_data_read();
	data.kVoltage_positive_B = k;
	flash_data_write();

}
void flash_data_set_k_voltage_negative_B(uint16_t k)
{
	flash_data_read();
	data.kVoltage_negative_B = k;
	flash_data_write();
}

uint16_t flash_data_get_k_voltage_positive_A()
{
	flash_data_read();
	return data.kVoltage_positive_A;
}
uint16_t flash_data_get_k_voltage_negative_A()
{
	flash_data_read();
	return data.kVoltage_negative_A;
}
uint16_t flash_data_get_k_voltage_positive_B()
{
	flash_data_read();
	return data.kVoltage_positive_B;
}
uint16_t flash_data_get_k_voltage_negative_B()
{
	flash_data_read();
	return data.kVoltage_negative_B;
}

// private
static void flash_data_erase_sector()
{
	FLASH_EraseInitTypeDef FlashErase;
	uint32_t sectorError = 0;
	__disable_irq();
	HAL_FLASH_Unlock();

	FlashErase.TypeErase = FLASH_TYPEERASE_SECTORS;
	FlashErase.NbSectors = 1;
	FlashErase.Sector = FLASH_DATA_SECTOR;
	FlashErase.VoltageRange = VOLTAGE_RANGE_3;
	if (HAL_FLASHEx_Erase(&FlashErase, &sectorError) != HAL_OK)
	{
		HAL_FLASH_Lock();
        __enable_irq();
	}
	HAL_FLASH_Lock();
    __enable_irq();
}

static void flash_data_write()
{
	flash_data_erase_sector();
	HAL_StatusTypeDef status;
	__disable_irq();
	status = HAL_FLASH_Unlock();
	uint32_t addr = FLASH_DATA_BASE_ADDR;
	uint8_t* ptr = (uint8_t*)&data;
	int i;
	for(i = 0; i < sizeof(flash_data_t); i++)
	{
		status += HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, addr, ptr[i]);
		addr++;
	}
	__enable_irq();
	HAL_FLASH_Lock();
}

static void flash_data_read()
{
	memset((void*)&data, 0, sizeof(data));
	uint8_t* ptr = (uint8_t*)&data;
	uint32_t addr = FLASH_DATA_BASE_ADDR;
	int i;
	for(i = 0; i < sizeof(flash_data_t); i++)
	{
		*(ptr + i) = *(__IO uint8_t*)addr;
		addr++;
	}
}


