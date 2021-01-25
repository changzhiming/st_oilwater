/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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
#define DAC_LD_Pin GPIO_PIN_13
#define DAC_LD_GPIO_Port GPIOC
#define DAC_DATA_Pin GPIO_PIN_14
#define DAC_DATA_GPIO_Port GPIOC
#define DAC_CLK_Pin GPIO_PIN_15
#define DAC_CLK_GPIO_Port GPIOC
#define R595_SCK_Pin GPIO_PIN_0
#define R595_SCK_GPIO_Port GPIOC
#define R595_RCK_Pin GPIO_PIN_1
#define R595_RCK_GPIO_Port GPIOC
#define R595_OE_Pin GPIO_PIN_2
#define R595_OE_GPIO_Port GPIOC
#define R595_DATA_Pin GPIO_PIN_3
#define R595_DATA_GPIO_Port GPIOC
#define DI_IN7_Pin GPIO_PIN_12
#define DI_IN7_GPIO_Port GPIOB
#define DI_IN6_Pin GPIO_PIN_13
#define DI_IN6_GPIO_Port GPIOB
#define DI_IN5_Pin GPIO_PIN_14
#define DI_IN5_GPIO_Port GPIOB
#define DI_IN4_Pin GPIO_PIN_15
#define DI_IN4_GPIO_Port GPIOB
#define DI_IN3_Pin GPIO_PIN_6
#define DI_IN3_GPIO_Port GPIOC
#define DI_IN2_Pin GPIO_PIN_7
#define DI_IN2_GPIO_Port GPIOC
#define DI_IN1_Pin GPIO_PIN_8
#define DI_IN1_GPIO_Port GPIOC
#define DI_IN0_Pin GPIO_PIN_9
#define DI_IN0_GPIO_Port GPIOC
#define APP_RUN_Pin GPIO_PIN_8
#define APP_RUN_GPIO_Port GPIOA
#define RELAY_EN_Pin GPIO_PIN_12
#define RELAY_EN_GPIO_Port GPIOA
#define D_OUT1_Pin GPIO_PIN_12
#define D_OUT1_GPIO_Port GPIOC
#define D_OUT2_Pin GPIO_PIN_2
#define D_OUT2_GPIO_Port GPIOD
#define D_OUT3_Pin GPIO_PIN_3
#define D_OUT3_GPIO_Port GPIOB
#define D_OUT4_Pin GPIO_PIN_4
#define D_OUT4_GPIO_Port GPIOB
#define D_OUT5_Pin GPIO_PIN_5
#define D_OUT5_GPIO_Port GPIOB
#define D_OUT6_Pin GPIO_PIN_6
#define D_OUT6_GPIO_Port GPIOB
#define D_OUT7_Pin GPIO_PIN_7
#define D_OUT7_GPIO_Port GPIOB
#define D_OUT8_Pin GPIO_PIN_8
#define D_OUT8_GPIO_Port GPIOB
#define DAC_LOAD_Pin GPIO_PIN_9
#define DAC_LOAD_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
