/**
  ******************************************************************************
  * @file    stm3210e_open4.h
  * @author  hs
  * @version V4.2.0
  * @date    /1/23/2011
  * @brief   This file contains definitions for STM3210E_OPEN4's Leds, push-buttons
  *          COM port and Temperature Sensor
  *          hardware resources.  
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  */ 
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM3210E_OPEN4_H
#define __STM3210E_OPEN4_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
#include "stm32_eval.h"

/** @addtogroup Utilities
  * @{
  */ 

/** @addtogroup STM32_OPEN4
  * @{
  */  
  
/** @addtogroup STM3210E_OPEN4
  * @{
  */ 

/** @addtogroup STM3210E_OPEN4_LOW_LEVEL
  * @{
  */ 
  
/** @defgroup STM3210E_OPEN4_LOW_LEVEL_Exported_Types
  * @{
  */
/**
  * @}
  */ 

/** @defgroup STM3210E_OPEN4_LOW_LEVEL_Exported_Constants
  * @{
  */ 
/** @addtogroup STM3210E_OPEN4_LOW_LEVEL_LED
  * @{
  */
#define LEDn                             4

#define LED1_PIN                         GPIO_Pin_0
#define LED1_GPIO_PORT                   GPIOE
#define LED1_GPIO_CLK                    RCC_APB2Periph_GPIOE  
  
#define LED2_PIN                         GPIO_Pin_1
#define LED2_GPIO_PORT                   GPIOE
#define LED2_GPIO_CLK                    RCC_APB2Periph_GPIOE  

#define LED3_PIN                         GPIO_Pin_11
#define LED3_GPIO_PORT                   GPIOE
#define LED3_GPIO_CLK                    RCC_APB2Periph_GPIOE  

#define LED4_PIN                         GPIO_Pin_12
#define LED4_GPIO_PORT                   GPIOE
#define LED4_GPIO_CLK                    RCC_APB2Periph_GPIOE

/**
  * @}
  */
  
/** @addtogroup STM3210E_OPEN4_LOW_LEVEL_BUTTON
  * @{
  */  
#define BUTTONn                          8

/**
 * @brief Wakeup push-button
 */
/* OPEN4: not implemented
*/
#define WAKEUP_BUTTON_PIN                GPIO_Pin_0
#define WAKEUP_BUTTON_GPIO_PORT          GPIOA
#define WAKEUP_BUTTON_GPIO_CLK           RCC_APB2Periph_GPIOA
#define WAKEUP_BUTTON_EXTI_LINE          EXTI_Line0
#define WAKEUP_BUTTON_EXTI_PORT_SOURCE   GPIO_PortSourceGPIOA
#define WAKEUP_BUTTON_EXTI_PIN_SOURCE    GPIO_PinSource0
#define WAKEUP_BUTTON_EXTI_IRQn          EXTI0_IRQn 
/**
 * @brief Tamper push-button
 */
/* OPEN4: not implemented
*/
#define TAMPER_BUTTON_PIN                GPIO_Pin_0
#define TAMPER_BUTTON_GPIO_PORT          GPIOC
#define TAMPER_BUTTON_GPIO_CLK           RCC_APB2Periph_GPIOC
#define TAMPER_BUTTON_EXTI_LINE          EXTI_Line13
#define TAMPER_BUTTON_EXTI_PORT_SOURCE   GPIO_PortSourceGPIOC
#define TAMPER_BUTTON_EXTI_PIN_SOURCE    GPIO_PinSource13
#define TAMPER_BUTTON_EXTI_IRQn          EXTI15_10_IRQn 
/**
 * @brief Key push-button
 */
/* OPEN4: not implemented
*/
#define KEY_BUTTON_PIN                   GPIO_Pin_0
#define KEY_BUTTON_GPIO_PORT             GPIOG
#define KEY_BUTTON_GPIO_CLK              RCC_APB2Periph_GPIOG
#define KEY_BUTTON_EXTI_LINE             EXTI_Line8
#define KEY_BUTTON_EXTI_PORT_SOURCE      GPIO_PortSourceGPIOG
#define KEY_BUTTON_EXTI_PIN_SOURCE       GPIO_PinSource8
#define KEY_BUTTON_EXTI_IRQn             EXTI0_IRQn 
/**
 * @brief Joystick Right push-button
 */
#define RIGHT_BUTTON_PIN                 GPIO_Pin_9
#define RIGHT_BUTTON_GPIO_PORT           GPIOD
#define RIGHT_BUTTON_GPIO_CLK            RCC_APB2Periph_GPIOD
#define RIGHT_BUTTON_EXTI_LINE           EXTI_Line9
#define RIGHT_BUTTON_EXTI_PORT_SOURCE    GPIO_PortSourceGPIOG
#define RIGHT_BUTTON_EXTI_PIN_SOURCE     GPIO_PinSource9
#define RIGHT_BUTTON_EXTI_IRQn           EXTI9_5_IRQn
/**
 * @brief Joystick Left push-button
 */    
#define LEFT_BUTTON_PIN                  GPIO_Pin_8
#define LEFT_BUTTON_GPIO_PORT            GPIOD
#define LEFT_BUTTON_GPIO_CLK             RCC_APB2Periph_GPIOD
#define LEFT_BUTTON_EXTI_LINE            EXTI_Line8
#define LEFT_BUTTON_EXTI_PORT_SOURCE     GPIO_PortSourceGPIOG
#define LEFT_BUTTON_EXTI_PIN_SOURCE      GPIO_PinSource8
#define LEFT_BUTTON_EXTI_IRQn            EXTI9_5_IRQn
/**
 * @brief Joystick Up push-button
 */
#define UP_BUTTON_PIN                    GPIO_Pin_10
#define UP_BUTTON_GPIO_PORT              GPIOD
#define UP_BUTTON_GPIO_CLK               RCC_APB2Periph_GPIOD
#define UP_BUTTON_EXTI_LINE              EXTI_Line10
#define UP_BUTTON_EXTI_PORT_SOURCE       GPIO_PortSourceGPIOG
#define UP_BUTTON_EXTI_PIN_SOURCE        GPIO_PinSource10
#define UP_BUTTON_EXTI_IRQn              EXTI15_10_IRQn  
/**
 * @brief Joystick Down push-button
 */   
#define DOWN_BUTTON_PIN                  GPIO_Pin_11
#define DOWN_BUTTON_GPIO_PORT            GPIOD
#define DOWN_BUTTON_GPIO_CLK             RCC_APB2Periph_GPIOD
#define DOWN_BUTTON_EXTI_LINE            EXTI_Line11
#define DOWN_BUTTON_EXTI_PORT_SOURCE     GPIO_PortSourceGPIOD
#define DOWN_BUTTON_EXTI_PIN_SOURCE      GPIO_PinSource11
#define DOWN_BUTTON_EXTI_IRQn            EXTI15_10_IRQn  
/**
 * @brief Joystick Sel push-button
 */  
#define SEL_BUTTON_PIN                   GPIO_Pin_15
#define SEL_BUTTON_GPIO_PORT             GPIOE
#define SEL_BUTTON_GPIO_CLK              RCC_APB2Periph_GPIOE
#define SEL_BUTTON_EXTI_LINE             EXTI_Line15
#define SEL_BUTTON_EXTI_PORT_SOURCE      GPIO_PortSourceGPIOG
#define SEL_BUTTON_EXTI_PIN_SOURCE       GPIO_PinSource15
#define SEL_BUTTON_EXTI_IRQn             EXTI15_10_IRQn  
/**
  * @}
  */ 

/** @addtogroup STM3210E_OPEN4_LOW_LEVEL_COM
  * @{
  */
#define COMn                             1

/**
 * @brief Definition for COM port1, connected to USART2
 */ 
#define OPEN4_COM1                       USART2
#define OPEN4_COM1_CLK                   RCC_APB1Periph_USART2
#define OPEN4_COM1_TX_PIN                GPIO_Pin_2
#define OPEN4_COM1_TX_GPIO_PORT          GPIOA
#define OPEN4_COM1_TX_GPIO_CLK           RCC_APB2Periph_GPIOA
#define OPEN4_COM1_RX_PIN                GPIO_Pin_3
#define OPEN4_COM1_RX_GPIO_PORT          GPIOA
#define OPEN4_COM1_RX_GPIO_CLK           RCC_APB2Periph_GPIOA
#define OPEN4_COM1_IRQn                  USART2_IRQn

/**
  * @}
  */ 

/** @addtogroup STM3210E_OPEN4_LOW_LEVEL_SD_FLASH
  * @{
  */
/**
  * @brief  SD FLASH SDIO Interface
  */ 

#define SD_DETECT_PIN                    GPIO_Pin_11                 /* PF.11 */
#define SD_DETECT_GPIO_PORT              GPIOF                       /* GPIOF */
#define SD_DETECT_GPIO_CLK               RCC_APB2Periph_GPIOF

#define SDIO_FIFO_ADDRESS                ((uint32_t)0x40018080)
/** 
  * @brief  SDIO Intialization Frequency (400KHz max)
  */
#define SDIO_INIT_CLK_DIV                ((uint8_t)0xB2)
/** 
  * @brief  SDIO Data Transfer Frequency (25MHz max) 
  */
#define SDIO_TRANSFER_CLK_DIV            ((uint8_t)0x1) 

/**
  * @}
  */ 
  
/** @addtogroup STM3210E_OPEN4_LOW_LEVEL_M25P_FLASH_SPI
  * @{
  */
/**
  * @brief  M25P FLASH SPI Interface pins
  */  
#define sFLASH_SPI                       SPI1
#define sFLASH_SPI_CLK                   RCC_APB2Periph_SPI1
#define sFLASH_SPI_SCK_PIN               GPIO_Pin_5                  /* PA.05 */
#define sFLASH_SPI_SCK_GPIO_PORT         GPIOA                       /* GPIOA */
#define sFLASH_SPI_SCK_GPIO_CLK          RCC_APB2Periph_GPIOA
#define sFLASH_SPI_MISO_PIN              GPIO_Pin_6                  /* PA.06 */
#define sFLASH_SPI_MISO_GPIO_PORT        GPIOA                       /* GPIOA */
#define sFLASH_SPI_MISO_GPIO_CLK         RCC_APB2Periph_GPIOA
#define sFLASH_SPI_MOSI_PIN              GPIO_Pin_7                  /* PA.07 */
#define sFLASH_SPI_MOSI_GPIO_PORT        GPIOA                       /* GPIOA */
#define sFLASH_SPI_MOSI_GPIO_CLK         RCC_APB2Periph_GPIOA
#define sFLASH_CS_PIN                    GPIO_Pin_2                  /* PB.02 */
#define sFLASH_CS_GPIO_PORT              GPIOB                       /* GPIOB */
#define sFLASH_CS_GPIO_CLK               RCC_APB2Periph_GPIOB

/**
  * @}
  */

/** @addtogroup STM3210E_OPEN4_LOW_LEVEL_TSENSOR_I2C
  * @{
  */
/**
  * @brief  LM75 Temperature Sensor I2C Interface pins
  */  
#define LM75_I2C                         I2C1
#define LM75_I2C_CLK                     RCC_APB1Periph_I2C1
#define LM75_I2C_SCL_PIN                 GPIO_Pin_6                  /* PB.06 */
#define LM75_I2C_SCL_GPIO_PORT           GPIOB                       /* GPIOB */
#define LM75_I2C_SCL_GPIO_CLK            RCC_APB2Periph_GPIOB
#define LM75_I2C_SDA_PIN                 GPIO_Pin_7                  /* PB.07 */
#define LM75_I2C_SDA_GPIO_PORT           GPIOB                       /* GPIOB */
#define LM75_I2C_SDA_GPIO_CLK            RCC_APB2Periph_GPIOB
#define LM75_I2C_SMBUSALERT_PIN          GPIO_Pin_5                  /* PB.05 */
#define LM75_I2C_SMBUSALERT_GPIO_PORT    GPIOB                       /* GPIOB */
#define LM75_I2C_SMBUSALERT_GPIO_CLK     RCC_APB2Periph_GPIOB

/**
  * @}
  */
  
/**
  * @}
  */
  
/** @defgroup STM3210E_OPEN4_LOW_LEVEL_Exported_Macros
  * @{
  */ 
/**
  * @}
  */ 

/** @defgroup STM3210E_OPEN4_LOW_LEVEL_Exported_Functions
  * @{
  */ 
void STM_OPEN4_LEDInit(Led_TypeDef Led);
void STM_OPEN4_LEDOn(Led_TypeDef Led);
void STM_OPEN4_LEDOff(Led_TypeDef Led);
void STM_OPEN4_LEDToggle(Led_TypeDef Led);
void STM_OPEN4_PBInit(Button_TypeDef Button, ButtonMode_TypeDef Button_Mode);
uint32_t STM_OPEN4_PBGetState(Button_TypeDef Button);
void STM_OPEN4_COMInit(COM_TypeDef COM, USART_InitTypeDef* USART_InitStruct);
void SD_LowLevel_DeInit(void);
void SD_LowLevel_Init(void); 
void SD_LowLevel_DMA_TxConfig(uint32_t *BufferSRC, uint32_t BufferSize);
void SD_LowLevel_DMA_RxConfig(uint32_t *BufferDST, uint32_t BufferSize);
void SD_WaitForDMAEndOfTransfer(void);
void sFLASH_LowLevel_DeInit(void);
void sFLASH_LowLevel_Init(void); 
void LM75_LowLevel_DeInit(void);
void LM75_LowLevel_Init(void); 
/**
  * @}
  */
#ifdef __cplusplus
}
#endif
  
#endif /* __STM3210E_OPEN4_H */
/**
  * @}
  */ 

/**
  * @}
  */ 

/**
  * @}
  */

/**
  * @}
  */
  
/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
