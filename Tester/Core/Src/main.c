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
#include "usart.h"
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

double l = 0.591;
double m = 0.117;
double M = 0.466;
//double dt = 0.02;
double g = 9.8;
double coef[] ={36.899038, 35.906461, 173.199145, 63.277879};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void moving_of_carriage(int32_t speed)
{
/***********************************************************************************
   Function for speed control of DC motor
   moving_of_carriage(0); // Stop moving
   moving_of_carriage(-number); // Right moving
   moving_of_carriage(+number); // Left moving

   Center is 32000 at TIM8 after calibration
   Left corner is Left edge position = 38000 tics at TIM8
   Right corner is Right edge position = 26000 tics at TIM8
***********************************************************************************/

  LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_1); // Enable moving

  if (speed == 0)
  {
      LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_1); // Stop moving
      LL_TIM_OC_SetCompareCH3(TIM2, 0);             // Stop moving
      LL_TIM_OC_SetCompareCH2(TIM2, 0);             // Stop moving
  }
  else if (speed < 0)
  {
    if (speed < -500)
      speed = -500;
//    else if (speed > -70)
//    		speed = -70;

    LL_TIM_OC_SetCompareCH3(TIM2, 0);            // Right moving
    LL_TIM_OC_SetCompareCH2(TIM2, (-((int32_t)speed)) + 17);       // Right moving

  }
  else if (speed > 0)
  {
    if (speed > 500)
    		speed = 500;
//    else if (speed < 70)
//            speed = 70;

    LL_TIM_OC_SetCompareCH2(TIM2, 0);            // Left moving
    LL_TIM_OC_SetCompareCH3(TIM2, (uint32_t)(speed + 50));        // Left moving
  }

}
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
void getPosition(uint16_t* car, uint16_t* pend){
	*car = TIM8->CNT; // carridge
	*pend = TIM4->CNT; // pendulume
}

int32_t getCarriageSpeed(){
	TIM5->CNT = 0;
	uint16_t zeroPos = TIM8->CNT;
	while (zeroPos == TIM8->CNT);
	return TIM5->CNT * (TIM8->CNT - zeroPos);
}

int32_t getPendulumSpeed(){
	TIM5->CNT = 0;
	uint16_t zeroPos = TIM4->CNT;
	while (zeroPos == TIM4->CNT);
	return (1000000 / TIM5->CNT * (TIM4->CNT - zeroPos));
}

//double LQRControl(){
//	int32_t x = TIM8->CNT - 32000;
//	int32_t alp = TIM4->CNT - 32000;
//	LL_mDelay(20);
//	int32_t x_new = TIM8->CNT - 32000;
//	int32_t alp_new = TIM4->CNT - 32000;
//	double force = 0;
//	force = (x_new * coef[0] + (x_new - x) / dt * coef[1]) / 13000.0;
//	force += (((alp_new * coef[2] + (alp_new - alp) * coef[3]) / 1000.0) * 2) * 3.14;
//	double ac = 0;
//	ac = force / (M + m);
//	double speed = 0;
//	speed = (((x_new - x) / dt) /13000.0 + (ac * dt)) * 13000.0;
//	return speed;
//}

void mesureCarriageSpeed(){
	uint8_t flag;
			   // отослать данное назад

		  while ((UART5->SR & USART_SR_RXNE) == 0) {} // Ждем пустого регистра
				 flag = UART5->DR;

		  int32_t data = getCarriageSpeed(); // 0

		  while ((UART5->SR & USART_SR_TXE) == 0) {} // Ждем пустого регистра
				  UART5->DR = flag;
		  while ((UART5->SR & USART_SR_TXE) == 0) {} // Ждем пустого регистра
				UART5->DR = data & 0xFF;
		  while ((UART5->SR & USART_SR_TXE) == 0) {} // Ждем пустого регистра
				UART5->DR = (data>>8) & 0xFF;
		  while ((UART5->SR & USART_SR_TXE) == 0) {} // Ждем пустого регистра
				UART5->DR = (data>>16) & 0xFF;
		  while ((UART5->SR & USART_SR_TXE) == 0) {} // Ждем пустого регистра
				UART5->DR = (data>>24) & 0xFF;
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
  MX_UART5_Init();
  MX_TIM5_Init();
  /* USER CODE BEGIN 2 */
//  Buzzer_Start();

  TIM4->CR1 |= (1<<0); // Start timer 4 to read encoder
  TIM8->CR1 |= (1<<0); // Start timer 8 to read encoder
  TIM5->CR1 |= (1<<0); // Start timer 3
  UART5->CR1 |= USART_CR1_TE | USART_CR1_RE ; // разрешаем приемник и передатчик
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
  Buzzer_Start();
//  TIM5->CNT = 0;
//  TIM8->CNT = 0;
//  moving_of_carriage(500);
//  LL_mDelay(100);
//  moving_of_carriage(0);
//  uint32_t t = TIM5->CNT;
//  uint32_t e = TIM8->CNT;

  while(abs(TIM4->CNT - 32000) != 500);
  TIM4->CNT = 32000;

//  uint32_t x = 0;
//  uint32_t last_x = 0;
//  uint32_t last_alp = 0;
//  uint32_t lastTimeMicros = 0;

  double PIDp_pend = 30;
  double PIDd_pend = 45;
  double PIDi_pend = 1.6;

//  double PIDp_car = 1;
//  double PIDd_car = 0;
//
  double speed = 0;
  uint32_t neededPos = 32000;
  double err = 0;
  double last_err = 0;
  int32_t last_speed = 0;
  int32_t sum_err = 0;
  double mean_speed[] = {0, 0, 0, 0, 0};
  double mean = 0;
//
//  double b = -18096.64;
//  double k = 766.94;
//  moving_of_carriage(70);
//  LL_mDelay(1000);
//  moving_of_carriage(0);
  while (1)
  {
// Encoders information:
// enc_carriage from edge to edge have 1365 ticks
// enc_pendulum have 1000 ticks for 1 full circle rotation
//	  	long now = TIM5->CNT;
//	  	double dt = 1.0 * (now - lastTimeMicros) / 2000000;
//		double x = (TIM8->CNT - 32000) * 1.0 * (3.14 * 0.012) / 1000;
//		double alp = (TIM4->CNT - 32000) * 1.0 * 2 * 3.14 / 1000;
//		double v = (x - last_x) / dt;
//		double w = (alp - last_alp) /dt;
//		double force = (x * coef[0] + v * coef[1] + alp * coef[2] + w * coef[3]);
//		double ac = force / (M + m);
//		double speed = (v + ac * dt) * 1000 / (3.14 * 0.012);
//		if (speed < 0)
//			speed = (speed - 179) / 57;
//		else
//			speed = (speed + 179) / 57;
//		moving_of_carriage(speed);
//
//		last_x = x;
//		last_alp = alp;
//		lastTimeMicros = now;
//		LL_mDelay(20);
//	  while(abs(TIM4->CNT - 32000) > 6 && abs(TIM8->CNT - 32000) < 3000){
		  while(abs(TIM4->CNT - 32000) != 500 && abs(TIM8->CNT - 32000) < 4500){
			  err = (double)(neededPos) - (double)(TIM4->CNT);
			  sum_err += err;
			  speed = PIDp_pend * err + PIDd_pend * (err - last_err) + PIDi_pend * sum_err;
			  mean_speed[0] = mean_speed[1];
			  mean_speed[1] = mean_speed[2];
			  mean_speed[2] = mean_speed[3];
			  mean_speed[3] = mean_speed[4];
			  mean_speed[4] = speed;


			  mean = (mean_speed[0] + mean_speed[1] + mean_speed[2] + mean_speed[3] + mean_speed[4]) / 5;
			  speed = rint(mean);
			  moving_of_carriage((int32_t)(speed));
			  last_err = err;
			  LL_mDelay(9);
		  }
		  moving_of_carriage(0);

//		  LL_mDelay(10);
//	  }
//	  LL_mDelay(10);
//	  while(abs(TIM8->CNT - 32000) > 10000 && abs(TIM4->CNT - 32000) < 8){
//		  last_err = err;
//		  err = neededPos - TIM8->CNT;
//		  int32_t speed = err * PIDp_car + (err - last_err) * PIDd_car;
//		  if (abs(speed) > 100)
//			  GPIOD->ODR |= (1<<15);
//		  else
//			  GPIOD->ODR &= ~(1<<15);
//		  moving_of_carriage(speed);
//	  }




//if (flag_from_interapt == 0)
//{
//	getPosition(&enc_carriage, &enc_pendulum);





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

//	  LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_1);
//	  LL_mDelay(1000000);
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
