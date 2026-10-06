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
#include "stm32g4xx_hal.h"

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
#define ROLLING_AVE 32
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define ADC1_in0_Pin GPIO_PIN_0
#define ADC1_in0_GPIO_Port GPIOA
#define ADC2_in1_Pin GPIO_PIN_1
#define ADC2_in1_GPIO_Port GPIOA
#define ADC1_in2_Pin GPIO_PIN_2
#define ADC1_in2_GPIO_Port GPIOA
#define ADC1_in3_Pin GPIO_PIN_3
#define ADC1_in3_GPIO_Port GPIOA
#define ADC2_in4_Pin GPIO_PIN_4
#define ADC2_in4_GPIO_Port GPIOA
#define ADC2_in5_Pin GPIO_PIN_5
#define ADC2_in5_GPIO_Port GPIOA
#define ADC2_in6_Pin GPIO_PIN_6
#define ADC2_in6_GPIO_Port GPIOA
#define ADC2_in7_Pin GPIO_PIN_7
#define ADC2_in7_GPIO_Port GPIOA
#define ADC2_in8_Pin GPIO_PIN_4
#define ADC2_in8_GPIO_Port GPIOC
#define ADC2_in15_Pin GPIO_PIN_5
#define ADC2_in15_GPIO_Port GPIOC
#define ADC1_in14_Pin GPIO_PIN_0
#define ADC1_in14_GPIO_Port GPIOB
#define ADC1_in13_Pin GPIO_PIN_1
#define ADC1_in13_GPIO_Port GPIOB
#define LED1_Pin GPIO_PIN_15
#define LED1_GPIO_Port GPIOB
#define LED2_Pin GPIO_PIN_6
#define LED2_GPIO_Port GPIOC

/* USER CODE BEGIN Private defines */
  /*
  extern FDCAN_TxHeaderTypeDef TxHeader;
  extern FDCAN_RxHeaderTypeDef RxHeader;

  extern FDCAN_HandleTypeDef hfdcan1;
  */
  extern uint32_t averages[16];
  extern uint16_t transfer_functions[14];
  extern uint16_t all_raw_data[12][ROLLING_AVE];
  extern uint16_t ADC1Data[6];
  extern uint16_t ADC2Data[6];
  extern uint8_t AVE_POS;
  extern uint16_t CAN_interval;
  extern volatile uint32_t millis;
  extern uint8_t CAN_enable;
  extern uint8_t CAL_counter;
  extern uint8_t CAL_ON;
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
