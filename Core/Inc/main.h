/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define ADS1246_XDRDY_B_Pin GPIO_PIN_3
#define ADS1246_XDRDY_B_GPIO_Port GPIOE
#define ADS1246_CS_B_Pin GPIO_PIN_4
#define ADS1246_CS_B_GPIO_Port GPIOE
#define DAC_OUT_A_Pin GPIO_PIN_4
#define DAC_OUT_A_GPIO_Port GPIOA
#define DAC_OUT_B_Pin GPIO_PIN_5
#define DAC_OUT_B_GPIO_Port GPIOA
#define ADS1220_XDRDY_Pin GPIO_PIN_11
#define ADS1220_XDRDY_GPIO_Port GPIOB
#define ADS1220_CS_Pin GPIO_PIN_12
#define ADS1220_CS_GPIO_Port GPIOB
#define ADS1246_XDRDY_A_Pin GPIO_PIN_8
#define ADS1246_XDRDY_A_GPIO_Port GPIOC
#define ADS1246_CS_A_Pin GPIO_PIN_9
#define ADS1246_CS_A_GPIO_Port GPIOC
#define RANEG_A_Pin GPIO_PIN_2
#define RANEG_A_GPIO_Port GPIOD
#define RANGE_B_Pin GPIO_PIN_3
#define RANGE_B_GPIO_Port GPIOD
#define HV_EN_A_Pin GPIO_PIN_4
#define HV_EN_A_GPIO_Port GPIOD
#define HV_EN_B_Pin GPIO_PIN_5
#define HV_EN_B_GPIO_Port GPIOD
#define HV_POL_A_Pin GPIO_PIN_6
#define HV_POL_A_GPIO_Port GPIOD
#define HV_POL_B_Pin GPIO_PIN_7
#define HV_POL_B_GPIO_Port GPIOD
#define W5500_RST_Pin GPIO_PIN_6
#define W5500_RST_GPIO_Port GPIOB
#define W5500_CS_Pin GPIO_PIN_7
#define W5500_CS_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
