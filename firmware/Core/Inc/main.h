/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include "stm32f1xx_hal.h"

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
#define SW2_Pin GPIO_PIN_13
#define SW2_GPIO_Port GPIOC
#define SW1_Pin GPIO_PIN_14
#define SW1_GPIO_Port GPIOC
#define SW0_Pin GPIO_PIN_15
#define SW0_GPIO_Port GPIOC
#define MOTOR_EN2_Pin GPIO_PIN_3
#define MOTOR_EN2_GPIO_Port GPIOA
#define MOTOR_RST2_Pin GPIO_PIN_4
#define MOTOR_RST2_GPIO_Port GPIOA
#define MOTOR_FAULT2_Pin GPIO_PIN_7
#define MOTOR_FAULT2_GPIO_Port GPIOA
#define LED1_Pin GPIO_PIN_2
#define LED1_GPIO_Port GPIOB
#define MOTOR_FAULT1_Pin GPIO_PIN_10
#define MOTOR_FAULT1_GPIO_Port GPIOB
#define MOTOR_RST1_Pin GPIO_PIN_11
#define MOTOR_RST1_GPIO_Port GPIOB
#define MOTOR_EN1_Pin GPIO_PIN_12
#define MOTOR_EN1_GPIO_Port GPIOB
#define LED0_Pin GPIO_PIN_15
#define LED0_GPIO_Port GPIOA

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
