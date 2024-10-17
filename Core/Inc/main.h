/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
void Can_Task(void const * argument);
void RS232_Task(void const * argument);
void Init_Pins(void);
void DefaultCFGx01(/*sys_config *cfg*/);
/* USER CODE BEGIN EFP */
void Print_Loading();
/* USER CODE END EFP */

#define ISNT_AUDIO_COEFF 16
#define AVT04_AUDIO_COEFF 32
/* Private defines -----------------------------------------------------------*/
#define BUTTON_ON     GPIO_PIN_13
#define BUTTON_ON_Port GPIOC

#define BUTTON_ISNT_ON     GPIO_PIN_13
#define BUTTON_ISNT_ON_Port GPIOE

#define SPK_ON GPIO_PIN_5
#define SPK_ON_Port GPIOA

#define  MIC_ON GPIO_PIN_6
#define  MIC_ON_Port GPIOA


#define  LOW_VOL_ON GPIO_PIN_14
#define  LOW_VOL_ON_Port GPIOD

#define  LOW_CUR_ON GPIO_PIN_15
#define  LOW_CUR_Port GPIOD



#define  SOS_ON GPIO_PIN_9
#define  SOS_ON_Port GPIOE

#define  BUT_CUR_ON GPIO_PIN_11
#define  BUT_CUR_ON_Port GPIOE

#define KL15_ON GPIO_PIN_12
#define KL15_ON_Port GPIOF

#define   MUTE_CTRL GPIO_PIN_14
#define   MUTE_CTRL_Port GPIOF




#define PW_ON GPIO_PIN_13
#define PW_ON_Port GPIOF

#define KL30_OFF_MIN 500
#define KL30_OFF_MAX 1000

#define  KL3016VADC_MIN  3000
#define  KL3016VADC_MAX  4000


#define  KL3016VADC_CUR_MIN  230
#define  KL3016VADC_CUR_MAX  4500

#define   KL3016V_MIN 700
#define   KL3016V_MAX  900

#define   KL3016V_ISNT_MIN 1300
#define   KL3016V_ISNT_MAX  1700

#define   KL3016V_ISNT_24_MIN 500
#define   KL3016V_ISNT_24_MAX  1000

#define  KL308V_MIN  400
#define  KL308V_MAX  700

#define  KL308V_ISNT_MIN  700
#define  KL308V_ISNT_MAX  900


#define  KL308V_ISNT_24B_MIN  300
#define  KL308V_ISNT_24B_MAX  600


#define  LEDADC_LOW_MIN 2700
#define  LEDADC_LOW_MAX  3100


#define  LEDADC_LOW_ISNT_MIN 1800
#define  LEDADC_LOW_ISNT_MAX  2000

#define  LEDADC_HIGH_MIN 2200
#define  LEDADC_HIGH_MAX  2600

#define  LEDADC_HIGH_ISNT_MIN 2400
#define  LEDADC_HIGH_ISNT_MAX  2800

#define  BATT_MIN 1170
#define  BATT_MAX  1440

#define  KL15_OFF  300

#define MIC_UDS_AVT04_OFF 600
#define MIC_UDS_AVT04_MAX 430
#define MIC_UDS_AVT04_MIN 200


#define MUTE_Mmin_OFF_MIN    2500
#define MUTE_Mmin_OFF_MAX    4000


#define MUTE_Mplus_OFF_MIN    150
#define MUTE_Mplus_OFF_MAX    500

#define MIC_UDS_MAX 400
#define MIC_UDS_MIN 100

#define MIC_UDS_ISNT_MAX 1800
#define MIC_UDS_ISNT_MIN 1400

#define MIC_UDS_OFF 50
//#define MIC_UDS_MIN 100



//#define LOW_VOLTAGE_MIN 1500
//#define LOW_VOLTAGE_MAX  2000

#define USER_Btn_Pin GPIO_PIN_13
#define USER_Btn_GPIO_Port GPIOC
#define MCO_Pin GPIO_PIN_0
#define MCO_GPIO_Port GPIOH
#define RMII_MDC_Pin GPIO_PIN_1
#define RMII_MDC_GPIO_Port GPIOC
#define RMII_REF_CLK_Pin GPIO_PIN_1
#define RMII_REF_CLK_GPIO_Port GPIOA
#define RMII_MDIO_Pin GPIO_PIN_2
#define RMII_MDIO_GPIO_Port GPIOA
#define RMII_CRS_DV_Pin GPIO_PIN_7
#define RMII_CRS_DV_GPIO_Port GPIOA
#define RMII_RXD0_Pin GPIO_PIN_4
#define RMII_RXD0_GPIO_Port GPIOC
#define RMII_RXD1_Pin GPIO_PIN_5
#define RMII_RXD1_GPIO_Port GPIOC
#define LD1_Pin GPIO_PIN_0
#define LD1_GPIO_Port GPIOB

#define LD3_Pin GPIO_PIN_14
#define LD3_GPIO_Port GPIOB
#define STLK_RX_Pin GPIO_PIN_8
#define STLK_RX_GPIO_Port GPIOD
#define STLK_TX_Pin GPIO_PIN_9
#define STLK_TX_GPIO_Port GPIOD
#define USB_PowerSwitchOn_Pin GPIO_PIN_6
#define USB_PowerSwitchOn_GPIO_Port GPIOG
#define USB_OverCurrent_Pin GPIO_PIN_7
#define USB_OverCurrent_GPIO_Port GPIOG
#define USB_SOF_Pin GPIO_PIN_8
#define USB_SOF_GPIO_Port GPIOA
#define USB_VBUS_Pin GPIO_PIN_9
#define USB_VBUS_GPIO_Port GPIOA
#define USB_ID_Pin GPIO_PIN_10
#define USB_ID_GPIO_Port GPIOA
#define USB_DM_Pin GPIO_PIN_11
#define USB_DM_GPIO_Port GPIOA
#define USB_DP_Pin GPIO_PIN_12
#define USB_DP_GPIO_Port GPIOA
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define RMII_TX_EN_Pin GPIO_PIN_11
#define RMII_TX_EN_GPIO_Port GPIOG
#define RMII_TXD0_Pin GPIO_PIN_13
#define RMII_TXD0_GPIO_Port GPIOG
#define LD2_Pin GPIO_PIN_7
#define LD2_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */
 void MX_ADC1_Init(void);
 void MX_ADC2_Init(uint32_t num);
 void MX_ADC3_Init(uint32_t num);
 uint32_t   InOutADC_Data(ADC_HandleTypeDef *hadc, uint32_t num);
 uint32_t   InOutADC_Data_Fast(ADC_HandleTypeDef *hadc, uint32_t num);
// void  Play_Audio_Rec(uint32_t sec , uint32_t freq);
 void MX_DAC_Init();
 void HAL_DAC_MspInit(DAC_HandleTypeDef* hdac);

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
