/******************** (C) COPYRIGHT 2010 STMicroelectronics ********************
* File Name          : main.c
* Author             : MCD Application Team
* Version            : V3.2.1
* Date               : 07/05/2010
* Description        : Virtual Com Port Demo main file
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

// **** READ THIS!!!! **** READ THIS!!!! **** READ THIS!!!!   **** READ THIS!!!!
//_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-
// This programs is strickly a simple demo just to have the same message appear
// on the EvoPrimer screen and Hyperteminal.  

//         DO NOT USE THIS CODE AS AN EXAMPLE FOR HOW TO PROGRAM!!!!!!!

//_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "usb_lib.h"
#include "usb_desc.h"
#include "hw_config.h"
#include "usb_pwr.h"
#include "circle.h"
#include "stm3210e_open4.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_dma.h"
#include <string.h>
#include <stdio.h>


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/



/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

uint8_t  * msg_buf[2];
GPIO_InitTypeDef GPIO_InitStructure;
                  

/* Extern variables ----------------------------------------------------------*/
extern uint32_t count_out;                              //usb_endp.c
extern uint8_t buffer_out[VIRTUAL_COM_PORT_DATA_SIZE];  //usb_des.h

/* Private function prototypes -----------------------------------------------*/
void LED_Init();
void LCD_Message_Init();
void Delay(vu32 nCount);

/* Private functions ---------------------------------------------------------*/

// This function is found in the STM3210e_OPEN4.c file                        
void LED_Init(void){
    STM_OPEN4_LEDInit(LED1);
    STM_OPEN4_LEDInit(LED2);
    STM_OPEN4_LEDInit(LED3);
    STM_OPEN4_LEDInit(LED4);

    STM_OPEN4_LEDOff(LED3);
    STM_OPEN4_LEDOff(LED1);
    STM_OPEN4_LEDOn(LED4);
    STM_OPEN4_LEDOn(LED2);
 
}

void LCD_Message_Init(void){
    // Configure display
    LCD_FullScreen( 1 );
    LCD_SetScreenOrientation( V12 );
    DRAW_SetDefaultColor();

    // Display logo on top of the screen
    LCD_SetLogoBW();

    // Display background
    DRAW_SetCharMagniCoeff( 2 );
    DRAW_SetTextColor( RGB_LIGHTPURPLE );
    DRAW_DisplayStringWithMode( 0, 200, "Virtual Com", ALL_SCREEN, NORMAL_TEXT, CENTER );
    DRAW_SetCharMagniCoeff( 1 );
    DRAW_SetTextColor( RGB_BLACK );
    DRAW_DisplayStringWithMode( 0, 180, "GPIO JTAGE", ALL_SCREEN, NORMAL_TEXT, CENTER );
    DRAW_DisplayStringWithMode( 0, 160, "REMAP DEMO", ALL_SCREEN, NORMAL_TEXT, CENTER );

    DRAW_DisplayStringWithMode( 0, 120, "Press Select Button", ALL_SCREEN, NORMAL_TEXT, CENTER );
    DRAW_SetTextColor( RGB_BLUE );

    DRAW_DisplayStringWithMode( 0,  80, "To REMAP JTAG Pins for", ALL_SCREEN, NORMAL_TEXT, CENTER );
    DRAW_DisplayStringWithMode( 0,  60, "Set DMM to Hz or Oscilloscope", ALL_SCREEN, NORMAL_TEXT, CENTER );
}

/*******************************************************************************
* Function Name  : main.
* Description    : Main routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
int main(void)
{
#define  SHUTDOWNCOUNT           0xFFFFF

counter_t Button_HoldCounter = 0;
counter_t NumberOfTimes = 0;
uint16_t buttonPressed = 0;
uint8_t mesg = 0;
uint8_t testKeyboard = 0;

// Initialize system and clocks
    Set_System();

/* Enable GPIOA, GPIOB and AFIO clocks */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB |
                         RCC_APB2Periph_AFIO, ENABLE);


// Setup USB controller
    Set_USBClock();
    USB_Interrupts_Config();
    USB_Init();

// Initialize LED
    LED_Init();

// Initialization LCD
    LCD_Init();

// LCD Startup Message
    LCD_Message_Init();
  
while(1){

  /* Test SELECT Button GPIO Pin level (SELECT Joystick button on EVOPrimer pressed) */
  if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_15))       
  {
    // Reconfigure display
    DRAW_SetCharMagniCoeff( 2 );
    DRAW_SetTextColor( RGB_RED );
    LCD_SetOffset( OFFSET_OFF );
    LCD_ClearAllScreen( RGB_BLACK );
    LCD_SetBackLight(BACKLIGHTMIN);
    DRAW_DisplayStringWithMode( 0, 210, 
        "!!!DEBUG SWO", ALL_SCREEN, INVERTED_TEXT, LEFT );
    DRAW_DisplayStringWithMode( 0, 180, 
        "NOW LOST !!!", ALL_SCREEN, INVERTED_TEXT, CENTER );
    DRAW_SetDefaultColor();
    DRAW_RestoreCharMagniCoeff();
    DRAW_DisplayStringWithMode( 0, 140, 
        "Switching to", ALL_SCREEN, INVERTED_TEXT, CENTER );
    DRAW_DisplayStringWithMode( 0, 100, 
        "JTAG GPIO Remap", ALL_SCREEN, INVERTED_TEXT, CENTER );
    DRAW_DisplayStringWithMode( 0, 60, 
        "Use DMM or Scope to", ALL_SCREEN, INVERTED_TEXT, CENTER );
    DRAW_DisplayStringWithMode( 0, 20, 
        "Measure Frequency", ALL_SCREEN, INVERTED_TEXT, CENTER );
    DRAW_SetCharMagniCoeff( 1 );
  
    STM_OPEN4_LEDToggle(LED2);
    
    // USB_SIL_WRITE function stores data in msg_buf1 along with the number
    // number of data bytes to be sent to the PC.  In this case only byte 
    // that was just received is echoed back to hyperterminal
    USB_SIL_Write(EP1_IN, "DEBUG LOST... Remap JTAG Pins\r\n",32);
    
    //  SetEPTxValid function sets the EP_TX_VALID allowing for the data 
    //  and size to be sent to the PC using USB endpoint ENDP1
    SetEPTxValid(ENDP1);
    
    //  Resetting count_out prevents the repeat of the same data being sent
    count_out = 0;

    /* Disable the Serial Wire Jtag Debug Port SWJ-DP */
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);

    /* Configure PA.13 (JTMS/SWDAT), PA.14 (JTCK/SWCLK) and PA.15 (JTDI) as 
    output push-pull */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* Configure PB.03 (JTDO) and PB.04 (JTRST) as output push-pull */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
  
    while (1) // *** endless loop ***
        {
            /* Toggle JTMS/SWDAT pin */
            GPIO_WriteBit(GPIOA, GPIO_Pin_13, (BitAction)(1 - GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_13)));
            /* Insert delay */
            Delay(0x5FFFF);

            /* Toggle JTCK/SWCLK pin */
            GPIO_WriteBit(GPIOA, GPIO_Pin_14, (BitAction)(1 - GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_14)));
            /* Insert delay */
            Delay(0x5FFFF);

            /* Toggle JTDI pin */
            GPIO_WriteBit(GPIOA, GPIO_Pin_15, (BitAction)(1 - GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_15)));
            /* Insert delay */
            Delay(0x5FFFF);

            /* Toggle JTDO pin */
            GPIO_WriteBit(GPIOB, GPIO_Pin_3, (BitAction)(1 - GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_3)));
            /* Insert delay */
            Delay(0x5FFFF);

            /* Toggle JTRST pin */
            GPIO_WriteBit(GPIOB, GPIO_Pin_4, (BitAction)(1 - GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_4)));
            /* Insert delay */
            Delay(0x5FFFF);
           
        }// end While for toggle
    }// end if statement for testing button
  }// end While(1)
}//end main

/*******************************************************************************
* Function Name  : Delay
* Description    : Inserts a delay time.
* Input          : nCount: specifies the delay time length.
* Output         : None
* Return         : None
*******************************************************************************/
void Delay(vu32 nCount)
{
  for(; nCount != 0; nCount--);
}

#ifdef USE_FULL_ASSERT
/*******************************************************************************
* Function Name  : assert_failed
* Description    : Reports the name of the source file and the source line number
*                  where the assert_param error has occurred.
* Input          : - file: pointer to the source file name
*                  - line: assert_param error line source number
* Output         : None
* Return         : None
*******************************************************************************/
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
 
}
#endif

/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
