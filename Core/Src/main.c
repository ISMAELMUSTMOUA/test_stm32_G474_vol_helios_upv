/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>
//defineciones
#define MODER_PC ((3U << (0 * 2)) | (3U << (1 * 2)) | (3U << (2 * 2))| (3U << (3 * 2)) | (3U << (4 * 2)) | (3U << (5 * 2)) | (3U << (6 * 2)) | (3U << (7 * 2))| (3U << (8 * 2)) | (3U << (9 * 2)))
#define PUPDR_PC ((2U << (0 * 2)) | (2U << (1 * 2)) | (2U << (2 * 2))| (2U << (3 * 2)) | (2U << (4 * 2)) | (2U << (5 * 2)) | (2U << (6 * 2)) | (2U << (7 * 2))| (2U << (8 * 2)) | (2U << (9 * 2)))

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

COM_InitTypeDef BspCOMInit;
__IO uint32_t BspButtonState = BUTTON_RELEASED;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void botton_init(){
	//0.abelertsr el reloj de PAy PB, PC
		RCC->AHB2ENR |= (RCC_AHB2ENR_GPIOAEN | RCC_AHB2ENR_GPIOBEN | RCC_AHB2ENR_GPIOCEN);      // Reloj para PA1, PA4 y PA5, PB5, y los de PC

	// 1.Configurar PA1, 5, 4 y PB4 como Entrada con resistencia Pull-Down
	    GPIOA->MODER &= ~((3U << (1 * 2)) | (3U << (4 * 2)) | (3U << (5 * 2))); // Modo Entrada (00) PA
		GPIOA->PUPDR &= ~((3U << (1 * 2)) | (3U << (4 * 2)) | (3U << (5 * 2))); // Limpiar configuración previa
		GPIOA->PUPDR |=  ((2U << (1 * 2)) | (2U << (4 * 2)) | (2U << (5 * 2))); // Activar resistencia Pull-Down (0)

	    GPIOB->MODER &= ~(3U << (5 * 2));         // Modo Entrada (00) para PB4
		GPIOB->PUPDR &= ~(3U << (5 * 2));         // Limpiar configuración previa
		GPIOB->PUPDR |=  (2U << (5 * 2));         // Activar resistencia Pull-Down (0)
	//2. Configuracion PC0 hasta PC9 como entradas PULL-Down
		GPIOC->MODER &= ~(MODER_PC); // Modo Entrada (00) PA
		GPIOC->PUPDR &= ~(MODER_PC); // Limpiar configuración previa
		GPIOC->PUPDR |=  (PUPDR_PC); // Activar resistencia Pull-Down (0)

}
// Función que escanea todos los botones del volante
uint8_t Leer_Botones(void) {

	//--- Los 4 Botones Traseras ---
		if ((GPIOA->IDR & (1 << 1)) != 0) return 1; // PA1 -> Botón 1
		if ((GPIOA->IDR & (1 << 4)) != 0) return 2; // PA4 -> Botón 2
		if ((GPIOA->IDR & (1 << 5)) != 0) return 3; // PA5 -> Botón 3
		if ((GPIOB->IDR & (1 << 5)) != 0) return 4; // PB4 -> Botón 4

    // --- LOS 5 DE LA IZQUIERDA (PC0 a PC4) ---
        if ((GPIOC->IDR & (1 << 0)) != 0) return 5; // PC0
        if ((GPIOC->IDR & (1 << 1)) != 0) return 6; // PC1
        if ((GPIOC->IDR & (1 << 2)) != 0) return 7; // PC2
        if ((GPIOC->IDR & (1 << 3)) != 0) return 8; // PC3
        if ((GPIOC->IDR & (1 << 4)) != 0) return 9; // PC4

    // --- LOS 5 DE LA DERECHA (PC5 a PC9) ---
        if ((GPIOC->IDR & (1 << 5)) != 0) return 10; // PC5
        if ((GPIOC->IDR & (1 << 6)) != 0) return 11; // PC6
        if ((GPIOC->IDR & (1 << 7)) != 0) return 12; // PC7
        if ((GPIOC->IDR & (1 << 8)) != 0) return 13; // PC8
        if ((GPIOC->IDR & (1 << 9)) != 0) return 14; // PC9

    return 0; // Si llega aquí, es que no hay nada pulsado
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */
	uint8_t current_state[4] = {1, 1, 1, 1};
	uint8_t previous_state[4] = {1, 1, 1, 1};

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
  /* USER CODE BEGIN 2 */
  	botton_init(); // <--- IMPORTANTE: Inicializa los 14 pines
    printf("--- DEBUG DE 14 BOTONES HELIOS INICIADO ---\r\n");
  /* USER CODE END 2 */

  /* Initialize led */
  //BSP_LED_Init(LED_GREEN);

  /* Initialize USER push-button, will be used to trigger an interrupt each time it's pressed.*/
  // BSP_PB_Init(BUTTON_USER, BUTTON_MODE_EXTI);

  /* Initialize COM1 port (115200, 8 bits (7-bit data + 1 stop bit), no parity */
  BspCOMInit.BaudRate   = 115200;
  BspCOMInit.WordLength = COM_WORDLENGTH_8B;
  BspCOMInit.StopBits   = COM_STOPBITS_1;
  BspCOMInit.Parity     = COM_PARITY_NONE;
  BspCOMInit.HwFlowCtl  = COM_HWCONTROL_NONE;
  if (BSP_COM_Init(COM1, &BspCOMInit) != BSP_ERROR_NONE)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN BSP */

  /* -- Sample board code to send message over COM1 port ---- */
  printf("Welcome to STM32 world !\n\r");
  int boton_anterior = 0;
  /* -- Sample board code to switch on led ---- */
 /* BSP_LED_On(LED_GREEN);*/

  /* USER CODE END BSP */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

    /* -- Sample board code for User push-button in interrupt mode ---- */
   /* if (BspButtonState == BUTTON_PRESSED)
    {
      /* Update button state */
      //BspButtonState = BUTTON_RELEASED;
      /* -- Sample board code to toggle led ---- */
      //BSP_LED_Toggle(LED_GREEN);

      /* ..... Perform your action ..... */
    //}
    /* USER CODE END WHILE */
    /* USER CODE BEGIN 3 */
  /*  // 1. Leer los 4 pines
        current_state[0] = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1); // PA1
        current_state[1] = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_4); // PA4
        current_state[2] = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_5); // PA5
        current_state[3] = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_5); // PB5

    // 2. Comprobar si algún botón se ha pulsado o soltado
      for (int i = 0; i < 4; i++) {
          if (current_state[i] != previous_state[i]) {
              switch(i) {
                  case 0:
                      printf("PA1: %d\r\n", current_state[0]);
                      break;
                  case 1:
                      printf("PA4: %d\r\n", current_state[1]);
                      break;
                  case 2:
                      printf("PA5: %d\r\n", current_state[2]);
                      break;
                  case 3:
                      printf("PB5: %d\r\n", current_state[3]);
                      break;
              }
          }
      }
    // 3. Guardar el estado actual
      for (int i = 0; i < 4; i++) {
          previous_state[i] = current_state[i];
      }*/
      /*
        if (current_state[0] == previous_state[0] ||
            current_state[1] == previous_state[1] ||
            current_state[2] == previous_state[2] ||
            current_state[3] == previous_state[3])
        {
            // 3. ¡LA MAGIA DEL BSP! Usamos printf directamente para enviar a Hercules
            printf("PA1: %d | PA4: %d | PA5: %d | PB5: %d\r\n",
                   current_state[0], current_state[1], current_state[2], current_state[3]);

            // 4. Guardar el estado actual para la siguiente vuelta
            previous_state[0] = current_state[0];
            previous_state[1] = current_state[1];
            previous_state[2] = current_state[2];
            previous_state[3] = current_state[3];
        }

        // 5. Pequeña pausa para evitar rebotes eléctricos del botón
        HAL_Delay(10);*/

  // 1. Escaneamos todos los botones (PA1-5, PB5, PC0-9)
      int boton_actual = Leer_Botones();

      // 2. Si el botón que leo es diferente al que leí antes...
      if (boton_actual != boton_anterior)
      {
        if (boton_actual != 0)
        {
          // Se ha pulsado un botón
          printf("BOTON PULSADO: %d ", boton_actual);

          // Debug específico para que sepas qué pin es en el terminal
          if (boton_actual <= 3) printf("(Puerto A)\r\n");
          else if (boton_actual == 4) printf("(Puerto B)\r\n");
          else if (boton_actual <= 9) printf("(Puerto C IZQ)\r\n");
          else printf("(Puerto C DER)\r\n");
        }
        else
        {
          // Se ha soltado el botón
          printf("BOTON SOLTADO (Ninguno activo)\r\n");
        }

        // 3. Actualizamos el estado anterior
        boton_anterior = boton_actual;
      }

      // 4. Pausa de 20ms para el "Debounce" (evita que el ruido eléctrico mande 100 mensajes)
      HAL_Delay(20);

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
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1_BOOST);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV4;
  RCC_OscInitStruct.PLL.PLLN = 85;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pins : BTN1_Pin BTN2_Pin BTN3_Pin */
  GPIO_InitStruct.Pin = BTN1_Pin|BTN2_Pin|BTN3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : BTN4_Pin */
  GPIO_InitStruct.Pin = BTN4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(BTN4_GPIO_Port, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief BSP Push Button callback
  * @param Button Specifies the pressed button
  * @retval None
  */
void BSP_PB_Callback(Button_TypeDef Button)
{
  if (Button == BUTTON_USER)
  {
    BspButtonState = BUTTON_PRESSED;
  }
}

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
