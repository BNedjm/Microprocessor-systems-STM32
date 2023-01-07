/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>
#include "led_config.h"
#include "btn_config.h"
#include "serial_api_config.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define TASK 6
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
char msg[SERIAL_API_LED_MSG_LEN + 1] = { 0, };
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

int _write(int file, char *ptr, int len)
{
  HAL_UART_Transmit(&huart3, (uint8_t*)ptr, len, 0xFFFF);
  return len;
}

int _read(int file, char *ptr, int len)
{
  int msg_len = 0;
  while(msg_len <= len)
  {
    HAL_UART_Receive(&huart3, (uint8_t*)ptr, 1, 0xFFFF);
    msg_len++;
    if(*ptr == '\r')
      break;
    ptr++;
  }
  return msg_len;
}


/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

#if TASK == 1

	puts("Hello World!");

#endif

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
  MX_USART3_UART_Init();
  /* USER CODE BEGIN 2 */
#if TASK == 1

  char msg[] = "Hello, Nucleo!\r\n";
   if(HAL_UART_Transmit(&huart3, (uint8_t*)msg, strlen(msg), 10) == HAL_OK)
     LED_GPIO_On(&hld1);

#endif

#if TASK == 2

	int PressCounter = 0;
	_Bool ButtonState = 0;
	_Bool ButtonPreState = 0;

#endif

#if TASK == 3 || TASK == 4

	uint8_t echo;

#endif

#if TASK == 4

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{

	if (huart->Instance == USART3)
	{
		HAL_UART_Receive(&huart3, &echo, 1, HAL_MAX_DELAY);
		HAL_UART_Transmit(&huart3, &echo, 1, HAL_MAX_DELAY);
	}

}

#endif

#if TASK == 5

	char cmd[4];

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if (huart->Instance == USART3)
	{
		memset(cmd, '\0', 4);
		scanf("%s", cmd);
		puts(cmd);

		for (int i=0; i<3; i++){
		  if(strncmp(cmd, "LD1", 3)==0){
			  if(cmd[3]=='1')
				  HAL_GPIO_WritePin(LD1_GPIO_Port, LD1_Pin, 1);
			  else if (cmd[3]=='0')
				  HAL_GPIO_WritePin(LD1_GPIO_Port, LD1_Pin, 0);
		  } else if (strcmp(cmd, "LD20")==0 || strcmp(cmd, "LD21")==0){
			  if(cmd[3]=='1')
				  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, 1);
			  else if (cmd[3]=='0')
				  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, 0);
		  } else if (strcmp(cmd, "LD30")==0 || strcmp(cmd, "LD31")==0){
			  if(cmd[3]=='1')
				  HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, 1);
			  else if (cmd[3]=='0')
				  HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, 0);
		  } else if (strcmp(cmd, "LD4")==0){
			  HAL_GPIO_TogglePin(LD4_GPIO_Port, LD4_Pin);
		  }
		}
	}

}

#endif

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

#if TASK == 1

	if(BTN_GPIO_EdgeDetected(&hbtn1) == BTN_PRESSED_EDGE)
	  {
	  int t_on = 100; // [ms]

	  LED_GPIO_On(&hld1);
	  HAL_Delay(t_on);
	  LED_GPIO_Off(&hld1);

	  LED_GPIO_On(&hld2);
	  HAL_Delay(t_on);
	  LED_GPIO_Off(&hld2);

	  LED_GPIO_On(&hld3);
	  HAL_Delay(t_on);
	  LED_GPIO_Off(&hld3);
	  }
	  else
		HAL_Delay(10);

#endif

#if TASK == 2

	char buffer [50] = "Hello ";

	ButtonState = HAL_GPIO_ReadPin(USER_Btn_GPIO_Port, USER_Btn_Pin);
	if (ButtonState == 1 && ButtonPreState == 0){
		PressCounter++;
		sprintf (buffer, "Hello %d", PressCounter);
		printf ("%s\n",buffer);
	}
	ButtonPreState = ButtonState;

#endif

#if TASK == 3

	//receive 1 byte
	HAL_UART_Receive(&huart3, &echo, 1, HAL_MAX_DELAY);
	//Sends back the byte
	HAL_UART_Transmit(&huart3, &echo, 1, HAL_MAX_DELAY);

#endif

#if TASK == 4 || TASK == 5

	HAL_UART_RxCpltCallback(&huart3);

#endif

#if TASK == 6

	//    [{"id":"LD1","state":1},{"id":"LD2","state":1},{"id":"LD3","state":1},{"id":"LD4","state":1}]

	memset(msg, '\0', SERIAL_API_LED_MSG_LEN);
	scanf("%s", msg);

	int hleds_len = sizeof(hleds) / sizeof(hleds[0]);
	int status = SERIAL_API_LED_ReadMsg(msg, hleds, hleds_len);

	if(status == 0)
	{
	  for(int i = 0; i < hleds_len; i++)
		LED_GPIO_Write(hleds[i].Led, hleds[i].State);
	}

	puts(msg);
	puts("\r\n");

#endif

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

  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 216;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 3;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Activate the Over-Drive mode
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV4;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7) != HAL_OK)
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
