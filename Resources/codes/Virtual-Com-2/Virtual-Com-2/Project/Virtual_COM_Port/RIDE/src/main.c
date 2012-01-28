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
char * buttonSelected[7] = {"SELECT BUTTON",
                            "  UP BUTTON  ", 
                            " DOWN BUTTON ",
                            " RIGHT BUTTON",
                            " LEFT BUTTON ",
                            " UNDEFINED   ",
                            "\r\n"};

uint8_t msg_buf1[2];
                  

/* Extern variables ----------------------------------------------------------*/
extern uint32_t count_out;                              //usb_endp.c
extern uint8_t buffer_out[VIRTUAL_COM_PORT_DATA_SIZE];  //usb_des.h

/* Private function prototypes -----------------------------------------------*/
void LED_Init();
void LCD_Message_Init();
void StopAllPeripherals();
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
    DRAW_DisplayStringWithMode( 0, 180, "Complete Demo", ALL_SCREEN, NORMAL_TEXT, CENTER );
    DRAW_DisplayStringWithMode( 0, 160, "of all Examples", ALL_SCREEN, NORMAL_TEXT, CENTER );

    DRAW_DisplayStringWithMode( 50, 120, "Fully explained code", 21, NORMAL_TEXT, LEFT );
    DRAW_SetTextColor( RGB_BLUE );

    DRAW_DisplayStringWithMode( 0,  80, "Wouldn't that be Nice", ALL_SCREEN, NORMAL_TEXT, CENTER );
    DRAW_DisplayStringWithMode( 0,  60, "If Only someone would do it!", ALL_SCREEN, NORMAL_TEXT, CENTER );
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

  while (1)
    {
    // count_out >0 indicates that there is data in the USB Buffer
    // bDeviceState == CONFIGURED indicates the USB device status
    if ((count_out != 0) && (bDeviceState == CONFIGURED))
    {
    // USB_SIL_READ function gets data from the USB that was sent to
    // the EP3_OUT endpoint from the PC and is then put in the msg_buf1
        USB_SIL_Read(EP3_OUT, msg_buf1);

    // USB_SIL_WRITE function stores data in msg_buf1 along with the 
    // number of data bytes to be sent to the PC.  In this case only byte 
    // that was just received is echoed back to hyperterminal
        USB_SIL_Write(EP1_IN, msg_buf1,1);
    
    //  SetEPTxValid function sets the EP_TX_VALID allowing for the data 
    //  and size to be sent to the PC using USB endpoint ENDP1
        SetEPTxValid(ENDP1);
    
    //  Resetting count_out prevents the repeat of the same data being sent
        count_out = 0;
        
    //  The input from Hyperterminal is tested to determine if one of the 4
    //  keys that we want has been pressed and assigns the buttonPressed a
    //  value that is equal to the GPIO bit used in the EVO.  This allows the
    //  same code to be used for both the EVO button and keyboard input
        testKeyboard = 1;
        if (msg_buf1[0] == 's') buttonPressed = 0x8000;
        if (msg_buf1[0] == 'u') buttonPressed = 0x400;
        if (msg_buf1[0] == 'd') buttonPressed = 0x800;
        if (msg_buf1[0] == 'r') buttonPressed = 0x200;
        if (msg_buf1[0] == 'l') buttonPressed = 0x100;

    }
    // If no keys where pressed on the keyboard then the buttons on the EVO
    // are read to see if any of the joystick or select button was pressed
    if(!testKeyboard){
    
    // the first statement reads the Joystick buttons using PORTD and masks
    // the bits so that only one of the bits corresponding to the Joystick
    // are tested
        buttonPressed = GPIO_ReadInputData(GPIOD) & 0x0F00;
    
    // Since the Joystick and Select button are on two separate PORTS, the
    // bit on the GPIOE is tested. If it has been pressed, then the buttonPressed
    // is assigned this value, otherwise it is assigned the value in the
    // previous statement
        if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_15)) buttonPressed = 0x8000;
    }
 
    DRAW_SetCharMagniCoeff( 2 );
    DRAW_SetTextColor( RGB_RED );

    // Test if either a button on the EVOPrimer was pressed or a Keyboard input
    if(buttonPressed || testKeyboard)
    {
        switch(buttonPressed) 
        {
            // SELECT Button or 's' Pressed and assign number for mesg to allow
            // the displays of the same message on the EVO LCD and Hyperterminal 
            case 0x8000:
                mesg = 0;
                DRAW_DisplayStringWithMode( 0,  5, buttonSelected[0],
                                        ALL_SCREEN, NORMAL_TEXT, CENTER );
            break;
            // UP Button or 'u' Pressed
            case 0x400:
                mesg = 1;
                DRAW_DisplayStringWithMode( 0,  5, buttonSelected[1],
                                        ALL_SCREEN, NORMAL_TEXT, CENTER );
            break;   
            // DOWN Button or 'd' Pressed
            case 0x800:
                mesg = 2;
                DRAW_DisplayStringWithMode( 0,  5, buttonSelected[2],
                                        ALL_SCREEN, NORMAL_TEXT, CENTER );
            break;    
            // RIGHT Button or 'r' Pressed
            case 0x200:
                mesg = 3;
                DRAW_DisplayStringWithMode( 0,  5, buttonSelected[3],
                                        ALL_SCREEN, NORMAL_TEXT, CENTER );
            break;
            // LEFT Button or 'l' Pressed            
            case 0x100:
                mesg = 4;
                DRAW_DisplayStringWithMode( 0,  5, buttonSelected[4],
                                        ALL_SCREEN, NORMAL_TEXT, CENTER );
            break;     
            // UNDEFINED keyboard input
            default:
                mesg = 5;
                DRAW_DisplayStringWithMode( 0,  5, buttonSelected[5],
                                        ALL_SCREEN, NORMAL_TEXT, CENTER );
            break;    
        }

        // reset the testKeyboard and buttonPressed bits so that the 
        // information is only displayed once
        testKeyboard = 0;       
        buttonPressed = 0;
    
        // The USB_SIL_Write function then sends the message of which key
        // or button has been pressed, the messages are all a fixed length
        USB_SIL_Write(EP1_IN, buttonSelected[mesg],13);
        SetEPTxValid(ENDP1);
 
        //  Display on LCD
        DRAW_SetCharMagniCoeff( 2 );
        DRAW_SetTextColor( RGB_RED );
        DRAW_DisplayStringWithMode( 0,  30, "You Pressed the", 
                                    ALL_SCREEN, NORMAL_TEXT, CENTER );
        DRAW_SetCharMagniCoeff( 1 );

        // toggle LED on EVO
        STM_OPEN4_LEDToggle(LED1);

        // Test the Joystick buttons, if pressed the EVO waits, providing
        // a debouncing of the button
        while(GPIO_ReadInputData(GPIOD) & 0x0F00){}
        
        // Test if the SELECT button has been pressed and starts counting
        // to see how long it has been held for
        while(STM_OPEN4_PBGetState(BUTTON_SEL))
            {
        // The Button_HoldCounter counts how long the button has been held
        // for.  If it is greater than the SHUTDOWNCOUNT then the standard
        // shutdown screen message is displayed
                Button_HoldCounter++;

                if ( Button_HoldCounter > SHUTDOWNCOUNT )
                {
                    LCD_SetOffset( OFFSET_OFF );
                    LCD_ClearAllScreen( RGB_BLACK );
                    LCD_SetBackLight(BACKLIGHTMIN);
                    DRAW_SetDefaultColor();
                    DRAW_RestoreCharMagniCoeff();
                    DRAW_DisplayStringWithMode( 0, 100, 
                        "Shutting down !", ALL_SCREEN, INVERTED_TEXT, CENTER );
                    DRAW_DisplayStringWithMode( 0, 50, 
                        "Release", ALL_SCREEN, INVERTED_TEXT, CENTER );
                    DRAW_DisplayStringWithMode( 0, 20, 
                        "the button !", ALL_SCREEN, INVERTED_TEXT, CENTER );
                    DRAW_SetCharMagniCoeff( 1 );
                    STM_OPEN4_LEDToggle(LED2);
                    
                    //SHUTDOWN_Action(); *** doesn't really shutdown
                // The select button is tested and when released will 
                // clear the screen to black, but doesn't shutdown the EVO
                // ... still trying to figure that one out!
                    while(STM_OPEN4_PBGetState(BUTTON_SEL)){}
                    LCD_SetBackLightOff();
                    LCD_ClearAllScreen( RGB_BLACK );
                }
            
            }
            // Sends a carriage return and line feed to the Hyperterminal
            USB_SIL_Write(EP1_IN, "\r\n",3);
            SetEPTxValid(ENDP1);

        } 
    }  
}


void StopAllPeripherals( void )
{
// PC13 = SHUTDOWN     = to "SHUTDOWN"        = 3 Output Push-pull  50Mhz
//   GPIO_SetBits(GPIOC, 0x00002000); 
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
