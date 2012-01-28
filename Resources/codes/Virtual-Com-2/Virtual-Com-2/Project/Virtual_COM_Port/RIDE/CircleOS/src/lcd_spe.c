/****************** COPYRIGHT (C) 2007-2010 RAISONANCE S.A.S. *****************/
/**
*
* @file     lcd_spe.c
* @brief    The LCD driver for the ILI9325.
* @author   YRT
* @date     09/2009
* @note     Platform = Open4 STM32E Daughter Board
**/
/******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "circle.h"
#include "stm32f10x_fsmc.h"
#include "stm32f10x_tim.h"

/// @cond Internal

/* Private define ------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
//const u16 primerUpic[] =
//{
//#include "bmp\Open4_EvoPrimer_screen_180x174_RLE256.h"
//};

#define RAISO_HEIGHT     49
#define RAISO_WIDTH      200
const u16 raiso_bw [] = {
#include "small_raisonance_innovation.h"
};

/* Vars for timer dedicated for lcd backlight */
static TIM_TimeBaseInitTypeDef      TIM_TimeBaseStructure;
static TIM_OCInitTypeDef            TIM_OCInitStructure;

/* Public variables ---------------------------------------------------------*/
uint_t Current_CCR_BackLightStart = DEFAULT_CCR_BACKLIGHTSTART;
const unsigned freqTIM2[ 6 ] = { 136, 136, 182, 273, 364,  546 } ;

/* External variable ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/


/*******************************************************************************
*
*                                LCD_Interface_Init
*
*******************************************************************************/
/**
*
*  @attention  This function must <b>NOT</b> be called by the user.
*
**/
/******************************************************************************/
void LCD_Interface_Init( void )
{
    FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
    FSMC_NORSRAMTimingInitTypeDef  p;
    GPIO_InitTypeDef GPIO_InitStructure;

    /*-- GPIO Configuration ------------------------------------------------------*/

    /* SRAM Data lines configuration */
    RCC_APB2PeriphClockCmd( GPIO_LCD_D1_PERIPH, ENABLE );
    RCC_APB2PeriphClockCmd( GPIO_LCD_D2_PERIPH, ENABLE );

    GPIO_InitStructure.GPIO_Pin =  LCD_D0 | LCD_D1 | LCD_D2 | LCD_D3 ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init( GPIOx_D1_LCD, &GPIO_InitStructure );

    GPIO_InitStructure.GPIO_Pin =  LCD_D4 | LCD_D5 | LCD_D6 | LCD_D7 ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init( GPIOx_D2_LCD, &GPIO_InitStructure );

    /* NOE, NWE and NE1 configuration */
    RCC_APB2PeriphClockCmd( GPIO_LCD_CTRL_PERIPH, ENABLE );
    GPIO_InitStructure.GPIO_Pin = CtrlPin_RD | CtrlPin_WR | CtrlPin_RS;
    GPIO_Init( GPIOx_CTRL_LCD, &GPIO_InitStructure );

    /* Reset : configured as regular GPIO, is not FSMC-controlled */
    RCC_APB2PeriphClockCmd( GPIO_LCD_RST_PERIPH, ENABLE );
    GPIO_InitStructure.GPIO_Pin = CtrlPin_RST;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init( GPIOx_RST_LCD, &GPIO_InitStructure );

    /* CS : configured as regular GPIO, is not FSMC-controlled */
    RCC_APB2PeriphClockCmd( GPIO_LCD_CS_PERIPH, ENABLE );
    GPIO_InitStructure.GPIO_Pin = CtrlPin_CS;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init( GPIOx_CS_LCD, &GPIO_InitStructure );

    GPIO_WriteBit( GPIOx_RST_LCD, CtrlPin_RST, Bit_SET );   /* Reset active at LOW */
    GPIO_WriteBit( GPIOx_CS_LCD, CtrlPin_CS, Bit_RESET );   /* CS active at LOW */


    /*-- FSMC Configuration ------------------------------------------------------*/

    /* Enable the FSMC Clock */
    RCC_AHBPeriphClockCmd( RCC_AHBPeriph_FSMC, ENABLE );

    p.FSMC_AddressSetupTime = 2;
    p.FSMC_AddressHoldTime = 2;
    p.FSMC_DataSetupTime = 2;
    p.FSMC_BusTurnAroundDuration = 5;
    p.FSMC_CLKDivision = 5;
    p.FSMC_DataLatency = 5;
    p.FSMC_AccessMode = FSMC_AccessMode_A;
    
    FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM3;
    FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;
    FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_SRAM;
    FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_8b;
    FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
    FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;  /* cf RM p363 + p384*/
    FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;
    FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
    FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;
    FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
    FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;
    FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
    FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &p;
    FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &p;

    FSMC_NORSRAMInit( &FSMC_NORSRAMInitStructure );

    /* Enable FSMC Bank1_SRAM Bank */
    FSMC_NORSRAMCmd( FSMC_Bank1_NORSRAM3, ENABLE );
}


/*******************************************************************************
*
*                                LCD_BackLightConfig
*
*******************************************************************************/
/**
*  Setting of the PWM that drives the backlight intensity.
*
**/
/******************************************************************************/
void LCD_BackLightConfig( void )
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /* Enable GPIO clock  */
    RCC_APB2PeriphClockCmd( GPIO_LCD_BL_PERIPH, ENABLE );

    /* GPIO Configuration : TIM in Output */
    GPIO_InitStructure.GPIO_Pin   = GPIO_BACKLIGHT_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init( GPIOx_BL_LCD, &GPIO_InitStructure );

    // Remaping TIM4 CH2 on PD13
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_AFIO, ENABLE );
    GPIO_PinRemapConfig( AFIO_MAPR_TIM4_REMAP, ENABLE );

    /* TIM Configuration -----------------------------------------------------*/
    /* TIMCLK = 12 MHz, Prescaler = 0x0 */

    /* Enable TIM clock */
    RCC_APB1PeriphClockCmd( TIM_LCD_BL_PERIPH, ENABLE );

    TIM_DeInit( TIM_LCD_BL );
    TIM_TimeBaseStructInit( &TIM_TimeBaseStructure );
    TIM_OCStructInit( &TIM_OCInitStructure );

    /* Time base configuration */
    TIM_TimeBaseStructure.TIM_Period          = 0xFFFF;
    TIM_TimeBaseStructure.TIM_Prescaler       = 0x00;
    TIM_TimeBaseStructure.TIM_ClockDivision   = 0x0;
    TIM_TimeBaseStructure.TIM_CounterMode     = TIM_CounterMode_Up;
    TIM_TimeBaseInit( TIM_LCD_BL, &TIM_TimeBaseStructure );

    /* Output Compare Toggle Mode configuration */
    TIM_OCInitStructure.TIM_OCMode      = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;   /* FWLib v2.0*/
    TIM_OCInitStructure.TIM_Pulse       = Current_CCR_BackLightStart;

//    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

    TIM_OCxInit( TIM_LCD_BL, &TIM_OCInitStructure );

    TIM_OCxPreloadConfig( TIM_LCD_BL, TIM_OCPreload_Disable );

    TIM_ARRPreloadConfig( TIM_LCD_BL, ENABLE );

    /* Go !!!*/
    TIM_Cmd( TIM_LCD_BL, ENABLE );

}


/*******************************************************************************
*
*                                LCD_Write_Reg
*
*******************************************************************************/
/**
*  Write a LCD register.
*
**/
/******************************************************************************/
inline void LCD_Write_Reg( u8 regadd, u16 value )
{
    // Set index of the register
    *( u8 volatile* )( LCD_CMD_MODE_ADDR ) = 0;
    *( u8 volatile* )( LCD_CMD_MODE_ADDR ) = regadd;

    // Write into the register (2 x 8 bits transfers, MSB first)
    *( u8 volatile* )( LCD_DATA_MODE_ADDR ) = value >> 8;
    *( u8 volatile* )( LCD_DATA_MODE_ADDR ) = value & 0xFF;
}

/*******************************************************************************
*
*                                LCD_Read_Reg
*
*******************************************************************************/
/**
*  Read a LCD register.
*
**/
/******************************************************************************/
u16 LCD_Read_Reg( u8 regadd )
{
    u16 temp1, temp2;

    // Set index of the register
    *( u8 volatile* )( LCD_CMD_MODE_ADDR ) = 0;
    *( u8 volatile* )( LCD_CMD_MODE_ADDR ) = regadd;

    // Read the value of the register
    temp1 = ( *( u8 volatile* )( LCD_DATA_MODE_ADDR ) ) << 8;
    temp2 = ( *( u8 volatile* )( LCD_DATA_MODE_ADDR ) );

    return ( temp1 | temp2 );
}

/*******************************************************************************
*
*                                LCD_Reset
*
*******************************************************************************/
/**
*  Hard reset of the LCD.
*
**/
/******************************************************************************/
void LCD_Reset( void )
{
    GPIO_WriteBit( GPIOx_RST_LCD, CtrlPin_RST, Bit_RESET );     /* Reset active at LOW */
    Delayms( 300 );
    GPIO_WriteBit( GPIOx_RST_LCD, CtrlPin_RST, Bit_SET );       /* Reset active at LOW */
    Delayms( 100 );

    return;
}


/*******************************************************************************
*
*                                LCD_BackLightChange
*
*******************************************************************************/
/**
*  Modify the PWM rate.
*
**/
/******************************************************************************/
void LCD_BackLightChange( void )
{
    /* Output Compare Toggle Mode configuration */
    TIM_OCInitStructure.TIM_Pulse = Current_CCR_BackLightStart;

    TIM_OCxInit( TIM_LCD_BL, &TIM_OCInitStructure );
}


/*******************************************************************************
*
*                                LCD_SetLogoBW
*
*******************************************************************************/
/**
*
*  Draw the logo
*
**/
/******************************************************************************/
void LCD_SetLogoBW( void )
{
    DRAW_SetImage(raiso_bw, (Screen_Width - RAISO_WIDTH) / 2, 280 - (RAISO_HEIGHT / 2) , RAISO_WIDTH, RAISO_HEIGHT) ;
//    DRAW_SetImage( primerUpic, ( Screen_Width - 180 ) / 2, ( Screen_Height - 174) / 2, 180, 174 ) ; /* 180x174 Image Size*/
}


/*******************************************************************************
*
*                                LCD_Scroll
*
*******************************************************************************/
/**
*
*  Scroll the screen each line to the top
*
**/
/******************************************************************************/
void LCD_Scroll( u8 Ascii, coord_t CurX, coord_t* CurY, coord_t RightMargX, coord_t LeftMargX, coord_t HighMargY, coord_t LowMargY, color_t bGndColor, color_t textColor, mag_t CharMagniCoeff )
{
    s16 Nblines, width, i, y;
    extern u16 bmpTmp [LCD_DMA_SIZE];

    InitListDMA();
    Nblines = ( LowMargY - *CurY ) / DeltaY;
    width = RightMargX - LeftMargX;
    for ( i = 0; i < Nblines ; i++ )
    {
        for ( y = HighMargY - DeltaY; y >= LowMargY; y -= DeltaY )
        {
            if ( width < PHYS_SCREEN_WIDTH )
                {
                LCD_RectRead( LeftMargX, y - DeltaY, width, DeltaY, ( u8* ) &bmpTmp );
                DRAW_SetImage( bmpTmp, LeftMargX, y, width, DeltaY );
                }
            else
                {
                LCD_RectRead( LeftMargX, y - DeltaY, PHYS_SCREEN_WIDTH, DeltaY, ( u8* ) &bmpTmp );
                DRAW_SetImage( bmpTmp, LeftMargX, y, PHYS_SCREEN_WIDTH, DeltaY );
                LCD_RectRead( LeftMargX + PHYS_SCREEN_WIDTH, y - DeltaY, width - PHYS_SCREEN_WIDTH, DeltaY, ( u8* ) &bmpTmp );
                DRAW_SetImage( bmpTmp, LeftMargX + PHYS_SCREEN_WIDTH, y, width - PHYS_SCREEN_WIDTH, DeltaY );
                }
//            LIST_LCD_RectRead( LeftMargX, y - DeltaY, width, DeltaY ); // No multiread data with this controller
//            LIST_DRAW_SetImage( LeftMargX, y, width, DeltaY );
        }
        LCD_FillRect( LeftMargX, LowMargY, width, DeltaY, bGndColor );
    }
    *CurY = LowMargY;

    // Display the selected bitmap according to the provided ASCII character.
    LCD_DisplayChar( CurX, *CurY, Ascii, textColor, bGndColor, CharMagniCoeff );

}


/*******************************************************************************
*
*                                LCD_SendLCDCmd_RAM_Access
*
*******************************************************************************/
/**
*   Call LCD_SendLCDCmd function for RAM access
*
*
**/
/******************************************************************************/
void LCD_SendLCDCmd_RAM_Access( void )
{
    LCD_SendLCDCmd( ILI9325_GRAM_DATA );
}

/*******************************************************************************
*
*                                LCD_SendLCDCmd_RAM_Access_End
*
*******************************************************************************/
/**
*   To be called at the end of several bytes transmission
*   (Present only for LCD SPI mode compatibility)
*
**/
/******************************************************************************/
void LCD_SendLCDCmd_RAM_Access_End( void )
{
}


/*******************************************************************************
*
*                                LCD_PixelRead
*
*******************************************************************************/
/**
*
*  Save the pixels of a rectangle part of the LCD into a RAM variable.
*
*  @param[out] ptbmp      Address of the pointer to store the read data into.
*
*  @warning    One pixel weights 2 bytes.
*  @warning    The (0x0) point in on the low left corner.
*
**/
/******************************************************************************/
inline void LCD_PixelRead( u8** ptbmp )
{
    u8 temp1, temp2, blue, red;

    // Send LCD RAM read command
    LCD_SendLCDCmd( ILI9325_GRAM_DATA );

    // Two first read bytes are dummy !
    LCD_ReadLCDData();
    LCD_ReadLCDData();

    // Read the two bytes of the pixel
    temp1 = ( u8 ) LCD_ReadLCDData();
    blue = ( temp1 & 0xF8 ) >> 3;
    temp2 = ( u8 ) LCD_ReadLCDData();
    red = ( temp2 & 0x1F ) << 3;

    *( *ptbmp ) = ( temp1 & 0x07 ) | red;
    *( *ptbmp + 1 ) = ( temp2 & 0xE0 ) | blue;
    *ptbmp += 2;

}

/*******************************************************************************
*
*                                LCD_SendLCDCmd_Rotation_Access
*
*******************************************************************************/
/**
*   Call LCD_SendLCDCmd function for rotation register access
*
*
**/
/******************************************************************************/
void LCD_SendLCDCmd_Rotation_Access( void )
{
    LCD_SendLCDCmd( ILI9325_ENTRY_MOD );
}


/*******************************************************************************
*
*                                LCD_SendLCDData16
*
*******************************************************************************/
/**
*
*  Send two data bytes to the LCD.
*
*  @param[in]  Data_h  an unsigned byte containing the high byte of data to send to the LCD.
*  @param[in]  Data_l  an unsigned byte containing the low byte of data to send to the LCD.
*
*
**/
/******************************************************************************/
void LCD_SendLCDData16( u8 Data_h, u8 Data_l )
{
    // Write into the current register
    *( u8 volatile* )( LCD_DATA_MODE_ADDR ) = Data_h;
    *( u8 volatile* )( LCD_DATA_MODE_ADDR ) = Data_l;

}

/// @endcond

/* Public functions ----------------------------------------------------------*/

/*******************************************************************************
*
*                                LCD_SendLCDCmd
*
*******************************************************************************/
/**
*
*  Send on command byte to the LCD.
*
*  @param[in]  Cmd   An u8 containing the user command to send to the LCD.
*
**/
/******************************************************************************/
void LCD_SendLCDCmd( u8 Cmd )
{
    // Set index of the register
    *( u8 volatile* )( LCD_CMD_MODE_ADDR ) = 0;
    *( u8 volatile* )( LCD_CMD_MODE_ADDR ) = Cmd;
}

/*******************************************************************************
*
*                                LCD_SendLCDData
*
*******************************************************************************/
/**
*
*  Send one data byte to the LCD.
*
*  @param[in]  Data  An unsigned character containing the data to send to the LCD.
*  @pre        An LCD_SendLCDCmd was done with a command waiting for data.
*
*
**/
/******************************************************************************/
void LCD_SendLCDData( u8 Data )
{
    // Write into the current register
    *( u8 volatile* )( LCD_DATA_MODE_ADDR ) = Data;
}


/***********************************************************************************
*
*                                LCD_ReadLCDData
*
************************************************************************************/
/**
*
*  Read one data byte from the LCD.
*
*  @return     An unsigned 32 bit word containing the data returned by a LCD command.
*  @pre        An LCD_SendLCDCmd was done with a command returning data.
*
**/
/********************************************************************************/
u32 LCD_ReadLCDData( void )
{
    /* Transfer data from the memory */
    return (( *( u32 volatile* )( LCD_DATA_MODE_ADDR ) ) );
}

/*******************************************************************************
*
*                                LCD_DrawPixel
*
*******************************************************************************/
/**
*
*  Draw a pixel on the LCD with the provided color.
*
*  @param[in]  XPos     The horizontal coordinate of the pixel.
*  @param[in]  YPos     The vertical coordinate of the pixel.
*  @param[in]  Color    The RGB color to draw the pixel with.
*
*  @warning    The (0x0) point in on the low left corner.
*
**/
/******************************************************************************/
void LCD_DrawPixel( coord_t XPos, coord_t YPos, color_t Color )
{
    /* Select LCD screen area. */
    LCD_SetRect_For_Cmd( XPos, YPos, 1, 1 );

    /* Send LCD RAM write command. */
    LCD_SendLCDCmd( ILI9325_GRAM_DATA );

    /* Draw pixel.*/
    LCD_SendLCDData( Color );
    LCD_SendLCDData( Color >> 8 );

}


