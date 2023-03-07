/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "ds18b20.h"
#include "oled.h"
#include "stdio.h"
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
	uint8_t x = 0;//屏幕字符横坐标
	uint8_t y = 0;//屏幕字符纵坐标
	
	float Temprature_Value;//温度值
	
	int Temprature_Value_Integer;//温度值整数位
	int Temprature_Value_Decimal;//温度值小数位
	
	float Temprature_Threshold_Low = 5;//温度值下限
	float Temprature_Threshold_High = 30;//温度值上限
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_ADC1_Init();
  MX_I2C1_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
	OLED_Init();//OLED屏幕初始化
	OLED_Clear();//OLED清屏
	
	//汉字显示
	OLED_ShowChinese(x + 32, y, 0);//测
	OLED_ShowChinese(x + 32 + 16 * 1, y, 1);//温
	OLED_ShowChinese(x + 32 + 16 * 2, y, 2);//系
	OLED_ShowChinese(x + 32 + 16 * 3, y, 3);//统
	
	OLED_ShowChinese(x, y + 2 * 1, 4);//当
	OLED_ShowChinese(x + 16 * 1, y + 2 * 1, 5);//前
	OLED_ShowChinese(x + 16 * 2, y + 2 * 1, 6);//温
	OLED_ShowChinese(x + 16 * 3, y + 2 * 1, 7);//度
	OLED_ShowChar(x + 16 * 4, y + 2 * 1, ':', 16);//冒号
	
	OLED_ShowChinese(x, y + 2 * 2, 6);//温
	OLED_ShowChinese(x + 16 * 1, y + 2 * 2, 7);//度
	OLED_ShowChinese(x + 16 * 2, y + 2 * 2, 8);//上
	OLED_ShowChinese(x + 16 * 3, y + 2 * 2, 9);//限
	OLED_ShowChar(x + 16 * 4, y + 2 * 2, ':', 16);//冒号
	
	OLED_ShowChinese(x, y + 2 * 3, 6);//温
	OLED_ShowChinese(x + 16 * 1, y + 2 * 3, 7);//度
	OLED_ShowChinese(x + 16 * 2, y + 2 * 3, 10);//下
	OLED_ShowChinese(x + 16 * 3, y + 2 * 3, 11);//限
	OLED_ShowChar(x + 16 * 4, y + 2 * 3, ':', 16);//冒号
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
		Temprature_Value = DS18B20_1_GetTemperture();//获取温度值
		Temprature_Value_Integer = (int)Temprature_Value;//强制转换，获取整数位
		Temprature_Value_Decimal = 10 * (Temprature_Value - (int)Temprature_Value);//获取小数位
		
		OLED_ShowNum(x + 16 * 5, y + 2 * 1, Temprature_Value_Integer, 2, 16);//整数位显示
		OLED_ShowChar(x + 16 * 6, y + 2 * 1, '.', 16);
		OLED_ShowNum(x + 13 * 8, y + 2 * 1, Temprature_Value_Decimal, 1, 16);//小数位显示
		OLED_ShowChar(x + 14 * 8, y + 2 * 1, 'C', 16);
		HAL_Delay(1000);
		
		OLED_ShowNum(x + 16 * 5, y + 2 * 2, Temprature_Threshold_High, 2, 16);//温度上限
		OLED_ShowChar(x + 16 * 6, y + 2 * 2, 'C', 16);
		OLED_ShowNum(x + 16 * 5, y + 2 * 3, Temprature_Threshold_Low, 2, 16);//温度下限
		OLED_ShowChar(x + 16 * 6, y + 2 * 3, 'C', 16);
		HAL_Delay(1000);
		
		if(Temprature_Value>Temprature_Threshold_High || Temprature_Value<Temprature_Threshold_Low)
		{
			HAL_GPIO_WritePin(Buzzer_GPIO_Port, Buzzer_Pin, GPIO_PIN_SET);//蜂鸣器函数使能
		}
		
		if(HAL_GPIO_ReadPin(Key_Up_GPIO_Port, Key_Up_Pin) == GPIO_PIN_RESET)
		{
			Temprature_Threshold_High+=2;//按键调整温度上限
		}
		else if(HAL_GPIO_ReadPin(Key_Down_GPIO_Port, Key_Down_Pin) == GPIO_PIN_RESET)
		{
			Temprature_Threshold_Low-=2;//按键调整温度下限
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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

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

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
