/****************** (C) Matthias *******************************************************/
/* optimized for PRIMER2, extended and amended by Matthias email webmaster@tg-info.de  */
/* -------- Includes ------------------------------------------------------------------*/
#include    "stm32f10x.h"

/* -------- Used Global variables -----------------------------------------------------*/

/* -------- Create Global variables ---------------------------------------------------*/
void        Led_Set_Value_u8(u8 Value);              // Bit 0 is green, Bit 1 is red
void        Led_Green_On(void);                      // The name says it all
void        Led_Green_OFF(void);                     // The name says it all
void        Led_Red_On(void);                        // The name says it all
void        Led_Red_OFF(void);                       // The name says it all
void        Led_Green_and_Red_On(void);              // The name says it all
void        Led_Green_and_Red_Off(void);             // The name says it all

/* -------- Private define ------------------------------------------------------------*/

/* -------- Private variables ---------------------------------------------------------*/

/* -------- Prototyp local ------------------------------------------------------------*/

/* -------- Code ----------------------------------------------------------------------*/
void Led_Set_Value_u8(u8 Value)
{
   // PE 0 = LED0       = to LED           = 3 Output Push-pull  50Mhz
   // PE 1 = LED1       = to LED           = 3 Output Push-pull  50Mhz
   GPIOE->BSRR = (Value & 0x3) | ( ( (~Value) & 0x3 ) << 16 ) ;
}
void Led_Green_On(void)
{
   GPIOE->BSRR = 0x00000001;
}
void Led_Green_OFF(void)
{
   GPIOE->BRR  = 0x00000001;
}
void Led_Red_On(void)
{
   GPIOE->BSRR = 0x00000002;
}
void Led_Red_OFF(void)
{
   GPIOE->BRR  = 0x00000002;
}
void Led_Green_and_Red_On(void)
{
   GPIOE->BSRR = 0x00000003;
}
void Led_Green_and_Red_Off(void)
{
   GPIOE->BRR  = 0x00000003;
}
