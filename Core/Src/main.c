/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "fdcan.h"
#include "gpio.h"
#include "tim.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "config.h"
#include "functions.h"
#include "pedal_map.h"
#include "sensors.h"
#include "virtual_sensors.h"
#include <stdio.h>
#include <string.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
Sensor sensors[SENSOR_NUM];

uint16_t ADC1Data[SENSOR_NUM / 2];
uint16_t ADC2Data[SENSOR_NUM / 2];
uint16_t all_raw_data[SENSOR_NUM][ROLLING_AVE];

uint8_t AVE_POS = 0;
uint16_t CAN_interval = 1;
uint16_t init_can_id = 1;
volatile uint32_t millis;
uint8_t CAN_enable = 0;
extern volatile uint8_t CANRxReady;

FDCAN_TxHeaderTypeDef TxHeader;
FDCAN_RxHeaderTypeDef RxHeader;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
  Config_Setup();
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_FDCAN1_Init();
  MX_TIM3_Init();
  MX_ADC2_Init();
  /* USER CODE BEGIN 2 */
  if (HAL_TIM_Base_Start_IT(&htim3) != HAL_OK)
  {
    Error_Handler();
  };
  if (HAL_ADC_Start_DMA(&hadc1, (uint32_t*)ADC1Data, hadc1.Init.NbrOfConversion) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_ADC_Start_DMA(&hadc2, (uint32_t*)ADC2Data, hadc2.Init.NbrOfConversion) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  while (1)
  {
    if (CAN_enable)
    {
      if (CANRxReady)
      {
        CAN_Message msg;
        __disable_irq();
        msg = RxMessage;
        CANRxReady = 0U;
        __enable_irq();
        decode(msg);
      }
      // run the scheduler once per millisecond, not once per loop pass
      static uint32_t last_tick = UINT32_MAX;
      uint32_t now = millis;
      if (now != last_tick)
      {
        last_tick = now;

        if (now % CAN_interval == 0)
        {
          calibration();
        }

        // each sensor is sent at its own interval
        for (uint8_t i = 0; i < SENSOR_NUM; i++)
        {
          if (sensors[i].CAN_interval && (now + i) % sensors[i].CAN_interval == 0)
          {
            print(i);
          }
        }

        if (now % 250 == 0)
        {
          HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);
        }
      }
    }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
   */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
   * in the RCC_OscInitTypeDef structure.
   */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
   */
  RCC_ClkInitStruct.ClockType =
      RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
  // Use a static flag to ensure both ADCs have finished a conversion cycle.
  // Bit 0 for ADC1, Bit 1 for ADC2.
  static uint8_t adc_done = 0;

  if (hadc->Instance == ADC1)
  {
    // Store ADC1 conversion results into sensors[0..5]
    for (uint8_t j = 0; j < hadc->Init.NbrOfConversion; j++)
    {
      all_raw_data[j][AVE_POS] = ADC1Data[j];
    }
    adc_done |= 0x01; // Mark ADC1 as done for this cycle.
  }
  else if (hadc->Instance == ADC2)
  {
    // Store ADC2 conversion results into sensors[6..11]
    for (uint8_t j = 0; j < hadc->Init.NbrOfConversion; j++)
    {
      all_raw_data[j + 6][AVE_POS] = ADC2Data[j];
    }
    adc_done |= 0x02; // Mark ADC2 as done for this cycle.
  }

  // Once both ADC1 and ADC2 have provided data:
  if (adc_done == 0x03)
  {
    adc_done = 0; // Reset the flag for the next conversion cycle.
    AVE_POS++;    // Advance the sample index.

    // When we have collected enough samples for a rolling average...
    if (AVE_POS >= ROLLING_AVE)
    {
      // For each sensor (total SENSOR_NUM = 12 sensors)
      for (int i = 0; i < SENSOR_NUM; i++)
      {
        uint32_t sum = 0;
        for (int z = 0; z < ROLLING_AVE; z++)
        {
          sum += all_raw_data[i][z];
        }
        // Store the average in the sensor structure.
        sensors[i].averages = sum / ROLLING_AVE;
      }
      AVE_POS = 0; // Reset the sample index for the next averaging period.
    }
  }
}
/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
