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
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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
extern char flag_from_interapt = 0;


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void Buzzer_Start(void)
{
	LL_GPIO_TogglePin(GPIOE, LL_GPIO_PIN_9);
	LL_mDelay(100);
	LL_GPIO_TogglePin(GPIOE, LL_GPIO_PIN_9);
	LL_mDelay(100);
	LL_GPIO_TogglePin(GPIOE, LL_GPIO_PIN_9);
	LL_mDelay(100);
	LL_GPIO_TogglePin(GPIOE, LL_GPIO_PIN_9);
	LL_mDelay(100);
}
void Buzzer_setpoint_selection(void)
{
	LL_GPIO_TogglePin(GPIOE, LL_GPIO_PIN_9);
	LL_mDelay(50);
	LL_GPIO_TogglePin(GPIOE, LL_GPIO_PIN_9);
	LL_mDelay(50);
	LL_GPIO_TogglePin(GPIOE, LL_GPIO_PIN_9);
	LL_mDelay(50);
	LL_GPIO_TogglePin(GPIOE, LL_GPIO_PIN_9);
	LL_mDelay(500);
	LL_GPIO_TogglePin(GPIOE, LL_GPIO_PIN_9);
	LL_mDelay(50);
	LL_GPIO_TogglePin(GPIOE, LL_GPIO_PIN_9);
	LL_mDelay(50);
	LL_GPIO_TogglePin(GPIOE, LL_GPIO_PIN_9);
	LL_mDelay(50);
	LL_GPIO_TogglePin(GPIOE, LL_GPIO_PIN_9);
	LL_mDelay(500);
	LL_GPIO_TogglePin(GPIOE, LL_GPIO_PIN_9);
	LL_mDelay(50);
	LL_GPIO_TogglePin(GPIOE, LL_GPIO_PIN_9);
	LL_mDelay(50);
	LL_GPIO_TogglePin(GPIOE, LL_GPIO_PIN_9);
	LL_mDelay(50);
	LL_GPIO_TogglePin(GPIOE, LL_GPIO_PIN_9);
	LL_mDelay(500);
	LL_GPIO_TogglePin(GPIOE, LL_GPIO_PIN_9);
	LL_mDelay(50);
	LL_GPIO_TogglePin(GPIOE, LL_GPIO_PIN_9);
	LL_mDelay(50);
	LL_GPIO_TogglePin(GPIOE, LL_GPIO_PIN_9);
	LL_mDelay(50);
	LL_GPIO_TogglePin(GPIOE, LL_GPIO_PIN_9);
	LL_mDelay(500);
}

void end_sensor_signal(void)
{
	LL_GPIO_SetOutputPin(GPIOD, LD4_Pin|LD3_Pin|LD5_Pin|LD6_Pin);
	LL_mDelay(300);
	LL_GPIO_ResetOutputPin(GPIOD, LD4_Pin|LD3_Pin|LD5_Pin|LD6_Pin);
	LL_mDelay(300);
	LL_GPIO_SetOutputPin(GPIOD, LD4_Pin|LD3_Pin|LD5_Pin|LD6_Pin);
	LL_mDelay(300);
	LL_GPIO_ResetOutputPin(GPIOD, LD4_Pin|LD3_Pin|LD5_Pin|LD6_Pin);
	LL_mDelay(300);
	LL_GPIO_TogglePin(GPIOE, LL_GPIO_PIN_9);
	LL_mDelay(150);
	LL_GPIO_TogglePin(GPIOE, LL_GPIO_PIN_9);
}
void calibration(void)
{
	  LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_1); // Left moving
	  LL_TIM_OC_SetCompareCH2(TIM2,0);            // Left moving
	  LL_TIM_OC_SetCompareCH3(TIM2,40);           // Left moving
}
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

	int setpoint = 0;
	int PID_p = 0;
	long enc_pendulum = 0;
	long enc_carriage = 0;
	long right_carriage_border = 0;
	long left_carriage_border = 0;
	long center_carriage_border = 0;

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

  /* System interrupt init*/
  NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_0);

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM2_Init();
  MX_TIM4_Init();
  MX_TIM8_Init();
  /* USER CODE BEGIN 2 */
  Buzzer_Start();
  TIM4->CR1 |= (1<<0); // Start timer 4 to read encoder
  TIM8->CR1 |= (1<<0); // Start timer 8 to read encoder

    LL_TIM_CC_EnableChannel(TIM2, LL_TIM_CHANNEL_CH2);
    LL_TIM_CC_EnableChannel(TIM2, LL_TIM_CHANNEL_CH3);
    LL_TIM_EnableCounter(TIM2);
  //LL_mDelay(1);
 // LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_1);// Stop moving
 // LL_mDelay(1);
  //LL_TIM_OC_SetCompareCH2(TIM2,0);            // Stop
  //LL_TIM_OC_SetCompareCH3(TIM2,0);           // Stop
  //LL_mDelay(1);

  // calibration();

// Old code for Analog-to-Digital Converter. When we tried to use circular potentiometers instead of encoders =)
//**************************************************************************************************************************************
  /*
   LL_ADC_Enable(ADC1); // Start of ADC 1 (ADC1->CR2 |= (1«0);)
   LL_ADC_REG_StartConversionSWStart(ADC1); // Sample from ADC
   while (LL_ADC_IsActiveFlag_EOCS(ADC1) != 1) {;} // Waiting for the end of the sample. When flag will be on
   LL_ADC_ClearFlag_EOCS(ADC1); // Make flag off to allow to make another ample
   setpoint = LL_ADC_REG_ReadConversionData12(ADC1); // Put data from ADC
   setpoint = setpoint/41;

   It was in while section:
   LL_ADC_REG_StartConversionSWStart(ADC1); // Sample from ADC
   while (LL_ADC_IsActiveFlag_EOCS(ADC1) != 1) {;} // Waiting for the end of the sample. When flag will be on
   LL_ADC_ClearFlag_EOCS(ADC1); // Make flag off to allow to make another ample
   ADC_1_data = LL_ADC_REG_ReadConversionData12(ADC1); // Put data from ADC
   ADC_1_data = ADC_1_data / 41;
  */
//**************************************************************************************************************************************


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
// Encoders information:
// enc_carriage from edge to edge have 1365 ticks
// enc_pendulum have 1000 ticks for 1 full circle rotation



//if (flag_from_interapt == 0)
//{
//	enc_pendulum = LL_TIM_GetCounter(TIM4); // Information about position of pendulum
	//enc_carriage = LL_TIM_GetCounter(TIM8); // Information about position of carriage
	//int x = 0;




/*
	PID_p = abs(((0.05 * (setpoint - ADC_1_data))));

	if (ADC_1_data==setpoint){LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_1);} else{LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_1);}

	if (ADC_1_data>setpoint)
	{
		LL_TIM_OC_SetCompareCH2(TIM2,0);
		LL_TIM_OC_SetCompareCH3(TIM2, PID_p);
	}
	if(ADC_1_data<setpoint)
	{
		LL_TIM_OC_SetCompareCH3(TIM2,0);
		LL_TIM_OC_SetCompareCH2(TIM2, PID_p);
	}

	if (ADC_1_data==0) // Если маятник упал
	{
	flag_from_interapt = 1;
	LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_2);
	LL_TIM_OC_SetCompareCH3(TIM2,100);
	LL_GPIO_SetOutputPin(GPIOD, LD4_Pin|LD3_Pin|LD5_Pin|LD6_Pin);
	}
	*/



//}

	// Moving test
	/*
	LL_mDelay(4000);
	LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_1); // Enable moving
 // LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_1); // Restrict moving
	LL_TIM_OC_SetCompareCH3(TIM2,0);            // Right moving
	LL_TIM_OC_SetCompareCH2(TIM2,30);           // Right moving
	LL_mDelay(600);
	LL_TIM_OC_SetCompareCH2(TIM2,0);            // Left moving
	LL_TIM_OC_SetCompareCH3(TIM2,30);           // Left moving
	LL_mDelay(1000000);
	*/

	  LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_1);
	  LL_mDelay(1000000);
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
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_5);
  while(LL_FLASH_GetLatency()!= LL_FLASH_LATENCY_5)
  {
  }
  LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE1);
  LL_RCC_HSE_Enable();

   /* Wait till HSE is ready */
  while(LL_RCC_HSE_IsReady() != 1)
  {

  }
  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_8, 336, LL_RCC_PLLP_DIV_2);
  LL_RCC_PLL_Enable();

   /* Wait till PLL is ready */
  while(LL_RCC_PLL_IsReady() != 1)
  {

  }
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_4);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_2);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);

   /* Wait till System clock is ready */
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
  {

  }
  LL_Init1msTick(168000000);
  LL_SetSystemCoreClock(168000000);
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
