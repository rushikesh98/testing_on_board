/****************** COPYRIGHT (C) 2007-2010 RAISONANCE S.A.S. *****************/
/**
*
* @file     lcd_spe_IL9325.c
* @brief    Common functions for Open4 platforms with ILI9325 controller.
* @author   YRT
* @date     08/2010
* @note     Platform = Open4
**/
/******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "circle.h"


/// @cond Internal

/* Private define ------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
u16 LCDStatus = 0;

/* Public variables ---------------------------------------------------------*/

/* External variable ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
*
*                                LCD_Init
*
*******************************************************************************/
/**
*
*  Initialize LCD. Called at CircleOS startup.
*
*  @attention  This function must <b>NOT</b> be called by the user.
*
**/
/******************************************************************************/
void LCD_Init( void )
{
    Screen_Width  = APP_SCREEN_WIDTH;
    Screen_Height = APP_SCREEN_HEIGHT;

    // Starting delay (for LCD startup)
    Delayms( 10 );

    /* Configure microcontroller interface for LCD management */
    LCD_Interface_Init();

    LCD_Reset();

    LCD_Controller_init();

    LCD_SetOffset( OFFSET_OFF );
    LCD_SetDefaultFont();
    LCD_SetTransparency(0);

    LCD_ClearAllScreen( RGB_WHITE );

    /* Init BackLight*/
#if BACKLIGHT_INTERFACE    
    LCD_SetBackLight( UTIL_ReadBackupRegister( BKP_BKLIGHT ) );
#endif    
    LCD_BackLightConfig();

    LCDStatus = LCD_CheckLCDStatus();    //for test
}

/*******************************************************************************
*
*                                LCD_Controller_init
*
*******************************************************************************/
/**
*  Initialization of the controller registers.
*
**/
/******************************************************************************/
void LCD_Controller_init( void )
{

    LCD_Write_Reg( ILI9325_TIMING_CTRL_1, 0x3008 );     // Set internal timing
    LCD_Write_Reg( ILI9325_TIMING_CTRL_2, 0x0012 );     // Set internal timing
    LCD_Write_Reg( ILI9325_TIMING_CTRL_3, 0x1231 );     // Set internal timing

    LCD_Write_Reg( ILI9325_START_OSC, 0x0001 );         // Start oscillator
    Delayms( 50 );

    LCD_Write_Reg( ILI9325_DRV_OUTPUT_CTRL_1, 0x0100 ); // Set SS=1 (Shift direction = 720->1) and SM=0 bit
    LCD_Write_Reg( ILI9325_LCD_DRV_CTRL, 0x0700 );      // Set 1 line inversion

    LCD_Write_Reg( ILI9325_ENTRY_MOD, V12_MADCTRVAL );  // TRI=0/DFM=0 (2x8 bits), BGR=1 (RGB swap)
    // HWM=0 (no high speed), ORG=0, AM=1, I/D=10

    LCD_Write_Reg( ILI9325_RESIZE_CTRL, 0x0000 );       // Resize register = no resizing
    LCD_Write_Reg( ILI9325_DIS_CTRL_2, 0x0202 );        // Set the back porch and front porch = 2 lines
    LCD_Write_Reg( ILI9325_DIS_CTRL_3, 0x0000 );        // Set non-display area refresh cycle ISC[3:0]
    LCD_Write_Reg( ILI9325_DIS_CTRL_4, 0x0000 );        // FMARK function
    LCD_Write_Reg( ILI9325_RGB_CTRL_1, 0x0000 );        // RGB interface setting DM=00 (internal clock)
    // RM=0  (RAM acces by system interface)
    LCD_Write_Reg( ILI9325_FRAME_MARKER_POS, 0x0000 );  // Frame marker Position
    LCD_Write_Reg( ILI9325_RGB_CTRL_2, 0x0000 );        // RGB interface polarity (not concerned)

    // Power ON sequence
    LCD_Power_ON();

    // Gamma adjustement
    LCD_Gamma_Adjust();

    /* Set GRAM area */
    LCD_Write_Reg( ILI9325_HOR_ADDR_START, 0x0000 );    // Horizontal GRAM Start Address
    LCD_Write_Reg( ILI9325_HOR_ADDR_END, 0x00EF );      // Horizontal GRAM End Address (239)
    LCD_Write_Reg( ILI9325_VET_ADDR_START, 0x0000 );    // Vertical GRAM Start Address
    LCD_Write_Reg( ILI9325_VET_ADDR_END, 0x013F );      // Vertical GRAM End Address (319)

    LCD_Write_Reg( ILI9325_DRV_OUTPUT_CTRL_2, 0xA700 ); // Gate Scan Line : GS=1, NL=0X27 (320 lines),
    // SCN=00000 (start scan G320)
    LCD_Write_Reg( ILI9325_BASE_IMG_CTRL, 0x0001 );     // NDL=0 (non display level), VLE=0 (vertical scroll disable), REV=1 (non grayscale inversion)
    LCD_Write_Reg( ILI9325_VSCROLL_CTRL, 0x0000 );      // Set scrolling line

    // Partial image configuration
    LCD_Write_Reg( ILI9325_PAR_IMG1_POS, 0 );
    LCD_Write_Reg( ILI9325_PAR_IMG1_START, 0 );
    LCD_Write_Reg( ILI9325_PAR_IMG1_END, 0 );
    LCD_Write_Reg( ILI9325_PAR_IMG2_POS, 0 );
    LCD_Write_Reg( ILI9325_PAR_IMG2_START, 0 );
    LCD_Write_Reg( ILI9325_PAR_IMG2_END, 0 );

    // Panel interface control
    LCD_Write_Reg( ILI9325_PAN_CTRL_1, 0x0010 );        // RTNI=10000 DIVI=00 (16 clocks/horizontal line, internal sync mode)
    LCD_Write_Reg( ILI9325_PAN_CTRL_2, 0x0000 );        // NOWI=000(non overlap period = 0 clocks)
    LCD_Write_Reg( ILI9325_PAN_CTRL_3, 0x0003 );        // ???
    LCD_Write_Reg( ILI9325_PAN_CTRL_4, 0x0110 );        // RTNE=010000 DIVE=01 (DOTCLK ratio 4, 16 clocks/horizontal line, RGB mode)
    LCD_Write_Reg( ILI9325_PAN_CTRL_5, 0x0000 );        // ???
    LCD_Write_Reg( ILI9325_PAN_CTRL_6, 0x0000 );        // ???

    // Color mode display
    LCD_Write_Reg( ILI9325_DIS_CTRL_1, 0x0133 );        // D0/D1=11, BASEE=1, GON=1 (display ON), CL=0 (262K colors)
    // PTDE=00 (no partial image)
    LCD_Write_Reg( 0xE4, 0x1430 );     // Set full frame read enable

    // Prepare write into graphical RAM
    LCD_Write_Reg( ILI9325_GRAM_HADDR, 0x0000 );        // GRAM horizontal Address
    LCD_Write_Reg( ILI9325_GRAM_VADDR, 0x0000 );        // GRAM Vertical Address


}

/*******************************************************************************
*
*                                LCD_Power_ON
*
*******************************************************************************/
/**
*  LCD Power ON sequence.
*
**/
/******************************************************************************/
void LCD_Power_ON( void )
{
    // Reset registers
    LCD_Write_Reg( ILI9325_POW_CTRL_1, 0x0000 );    // SAP=0, BT[3:0]=0000, AP=0, DSTB=0, SLP=0, STB=0
    LCD_Write_Reg( ILI9325_POW_CTRL_2, 0x0007 );    // DC1[2:0]=000, DC0[2:0]=000 (osc/1), VC[2:0]=111 (Vci/1)
    LCD_Write_Reg( ILI9325_POW_CTRL_3, 0x0000 );    // VREG1OUT voltage = halt
    LCD_Write_Reg( ILI9325_POW_CTRL_4, 0x0000 );    // VDV[4:0]=00000 for VCOM amplitude = VREG1OUT * 0.7
    Delayms( 200 );                                 // Dis-charge capacitor power voltage

    LCD_Write_Reg( ILI9325_POW_CTRL_1, 0x1690 );    // SAP=1  (soure driver enable), BT[3:0]=101, AP=001 (driver =1),
    // APE=1 (power supply enbale), DSTB=0, SLP=0, STB=0
    LCD_Write_Reg( ILI9325_POW_CTRL_2, 0x0221 );    // DC1[2:0]=010 (f/4), DC0[2:0]=010 (f/16), VC[2:0]=001 (Vci*0.9 = 2,8V)
    Delayms( 50 );

    LCD_Write_Reg( ILI9325_POW_CTRL_3, 0x0018 );    // PON=1 (Power ON), VCIRE=0 (external reference)
    Delayms( 50 );                                  // VRH=1000 (VREG1OUT voltage = Vci * 1,60  = 4,96V)

    LCD_Write_Reg( ILI9325_POW_CTRL_4, 0x1800 );    // VDV[4:0]=11000 (VCOM amplitude = VRegout * 1,10 = 5,45V)
    LCD_Write_Reg( ILI9325_POW_CTRL_7, 0x002A );    // VCM[5:0]=101010 (VCOMH =  VREG1 * 0,895 =  4,44V)
    LCD_Write_Reg( ILI9325_FRM_RATE_COLOR, 0x000B ); /* Frame Rate = 70Hz  */
    Delayms( 50 );

    return;
}

/*******************************************************************************
*
*                                LCD_Gamma_Adjust
*
*******************************************************************************/
/**
*  Adjust the gamma colors of the LCD.
*
**/
/*****************************************************************************/
void LCD_Gamma_Adjust( void )
{
    LCD_Write_Reg( ILI9325_GAMMA_CTRL_1, 0x0004 );
    LCD_Write_Reg( ILI9325_GAMMA_CTRL_2, 0x0007 );
    LCD_Write_Reg( ILI9325_GAMMA_CTRL_3, 0x0006 );
    LCD_Write_Reg( ILI9325_GAMMA_CTRL_4, 0x0206 );
    LCD_Write_Reg( ILI9325_GAMMA_CTRL_5, 0x0408 );
    LCD_Write_Reg( ILI9325_GAMMA_CTRL_6, 0x0507 );
    LCD_Write_Reg( ILI9325_GAMMA_CTRL_7, 0x0200 );
    LCD_Write_Reg( ILI9325_GAMMA_CTRL_8, 0x0707 );
    LCD_Write_Reg( ILI9325_GAMMA_CTRL_9, 0x0504 );
    LCD_Write_Reg( ILI9325_GAMMA_CTRL_10, 0x0F02 );
    return;
}


/*******************************************************************************
*
*                                LCD_CheckLCDStatus
*
*******************************************************************************/
/**
*  Check whether LCD is busy or not. Read the LCD ID
*
**/
/******************************************************************************/
u16 LCD_CheckLCDStatus( void )
{
    /* Read LCD device IDs*/
    u16 IDcode = LCD_Read_Reg( ILI9325_DRV_CODE );  // Must return 0x9325
    return IDcode;
}


/*******************************************************************************
*
*                                LCD_Batt
*
*******************************************************************************/
/**
*   Draw the battery
*
**/
/******************************************************************************/
void LCD_Batt( coord_t xBat, coord_t yBat, bool fDisplayTime, u16 BatState,
               u16* OldBatState, divider_t _div, coord_t* widthBat, coord_t* heightBat )
{
    color_t bat_color;
    static counter_t last_time = 0;
    counter_t new_time = RTC_GetCounter();
    u8 fill;
    bool fBlink;

    UNREFERENCED_PARAMETER( BatState );
    UNREFERENCED_PARAMETER( OldBatState );
    UNREFERENCED_PARAMETER( _div );

    if (( fDisplayTime == 1 ) && ( last_time != new_time ) )
    {
        last_time = new_time;
        switch ( PWR_CurrentState )
        {
        case PWR_STATE_UNDEF:
            return;

        case PWR_STATE_NOBAT:
            bat_color = RGB_PINK;
            fill = 100;
            fBlink = TRUE;
            break;

        case PWR_STATE_CHARGING:
            bat_color = RGB_BLUE;
            fill = 50;
            fBlink = TRUE;
            break;

        case PWR_STATE_FULL:
            bat_color = RGB_GREEN;
            fill = 100;
            fBlink = FALSE;
            break;

        case PWR_STATE_NORMAL:
            bat_color = RGB_BLACK;
            fBlink = FALSE;
            fill = PWR_BatteryLevel;
            break;

        case PWR_STATE_LOW:
            bat_color = RGB_YELLOW;
            fBlink = TRUE;
            fill = PWR_BatteryLevel;
            break;

        case PWR_STATE_EMPTY:
            bat_color = RGB_RED;
            fill = 100;
            fBlink = TRUE;
            break;
        }

        *widthBat = ( 20 * fill ) / 100;

        if ( *widthBat > 20 )
        {
            *widthBat = 20;
        }

        if ( fBlink && ( new_time & 1 ) )
        {
            LCD_FillRect_Circle(( xBat + 22 ) - *widthBat , yBat + 2 , *widthBat , *heightBat , ( PWR_CurrentState == PWR_STATE_LOW ) ? bat_color : RGB_WHITE );
            LCD_FillRect_Circle( xBat + 2 , yBat + 2 , 20 - *widthBat , *heightBat , ( PWR_CurrentState == PWR_STATE_LOW ) ? bat_color : RGB_WHITE );
        }
        else
        {
            LCD_FillRect_Circle(( xBat + 22 ) - *widthBat , yBat + 2 , *widthBat , *heightBat , ( PWR_CurrentState == PWR_STATE_LOW ) ? RGB_WHITE : bat_color );
            LCD_FillRect_Circle( xBat + 2 , yBat + 2 , 20 - *widthBat , *heightBat , ( PWR_CurrentState == PWR_STATE_LOW ) ? RGB_RED : RGB_WHITE );
        }
    }
}

/*******************************************************************************
*
*                                LCD_Clear
*
*******************************************************************************/
/**
*   Draw the battery and the toolbar
*
**/
/******************************************************************************/
void LCD_Clear( u16 xBat, u16  yBat )
{
    UNREFERENCED_PARAMETER( xBat );
    UNREFERENCED_PARAMETER( yBat );

#if TOUCHSCREEN_AVAIL
    if ( TOUCHSCR_GetMode() != TS_CALIBRATION )
    {
        TOUCHSCR_SetMode( TS_DRAWING );
    }
#endif

    DRAW_Batt();

#if TOUCHSCREEN_AVAIL
    TOOLBAR_SetDefaultToolbar();
#endif
}

/*******************************************************************************
*
*                                LCD_DisplayRotate
*
*******************************************************************************/
/**
*  Configure the LCD controller for a given orientation.
*
*  @param[in]  H12 The new screen orientation.
*
**/
/******************************************************************************/
void LCD_DisplayRotate( Rotate_H12_V_Match_TypeDef H12 )
{

    /* Memory Access Control */
    LCD_SendLCDCmd_Rotation_Access();

    // Apply desired direction
    switch ( H12 )
    {
    case V3  :
        LCD_SendLCDData16( V3_MADCTRVAL >> 8, V3_MADCTRVAL & 0xFF );

        break;

    case V6  :
        LCD_SendLCDData16( V6_MADCTRVAL >> 8, V6_MADCTRVAL & 0xFF );

        break;

    case V9  :
        LCD_SendLCDData16( V9_MADCTRVAL >> 8, V9_MADCTRVAL & 0xFF );

        break;

    case V12 :
    default  :
        LCD_SendLCDData16( V12_MADCTRVAL >> 8, V12_MADCTRVAL & 0xFF );
        break;

    case V12BMP :
        LCD_SendLCDData16( V12BMP_MADCTRVAL >> 8, V12BMP_MADCTRVAL & 0xFF );

        break;

    case V3BMP :
        LCD_SendLCDData16( V3BMP_MADCTRVAL >> 8, V3BMP_MADCTRVAL & 0xFF );

        break;

    case V6BMP :
        LCD_SendLCDData16( V6BMP_MADCTRVAL >> 8, V6BMP_MADCTRVAL & 0xFF );

        break;

    case V9BMP :
        LCD_SendLCDData16( V9BMP_MADCTRVAL >> 8, V9BMP_MADCTRVAL & 0xFF );
        break;
    }
}

/// @endcond

/* Public functions ----------------------------------------------------------*/

/*******************************************************************************
*
*                                LCD_RectRead
*
*******************************************************************************/
/**
*
*  Save the pixels of a rectangle part of the LCD into a RAM variable.
*
*  @param[in]  x        The horizontal coordinate of the rectangle low left corner.
*  @param[in]  y        The vertical coordinate of the rectangle low left corner.
*  @param[in]  width    The rectangle width in pixels.
*  @param[in]  height   The rectangle height in pixels.
*  @param[out] bmp      The variable to store the read data into.
*
*  @warning    One pixel weights 2 bytes.
*  @warning    The (0x0) point in on the low left corner.
*
**/
/******************************************************************************/
void LCD_RectRead( coord_t x, coord_t y, coord_t width, coord_t height, u8* bmp )
{
    u32 line, col;
    u16 col0, line0;

    // Reading pixel / pixel
    switch ( CurrentScreenOrientation )
    {
    case V12:
        // Read pixels from LCD screen
        col0 = Screen_Width - x - 1 + LCD_Offset;
        line0 = y + LCD_Offset;
        for ( col = 0; col < width; col++ )
        {
            // Current GRAM horizontal position
            LCD_Write_Reg( ILI9325_GRAM_HADDR, col0 - col );

            for ( line = 0; line < height; line++ )
            {
                // Current GRAM Vertical position
                LCD_Write_Reg( ILI9325_GRAM_VADDR, line0 + line );

                // Read pixel 2 bytes
                LCD_PixelRead( &bmp );
            }
        }
        break;

    case V3:
        // Read pixels from LCD screen
        col0 = y + LCD_Offset;
        line0 = x + LCD_Offset;
        for ( line = 0; line < width; line++ )
        {
            // Current GRAM Vertical position
            LCD_Write_Reg( ILI9325_GRAM_VADDR, line0 + line );

            for ( col = 0; col < height; col++ )
            {
                // Current GRAM horizontal position
                LCD_Write_Reg( ILI9325_GRAM_HADDR, col0 + col );

                // Read pixel 2 bytes
                LCD_PixelRead( &bmp );
            }
        }
        break;

    case V6:
        // Read pixels from LCD screen
        col0 = x + LCD_Offset;
        line0 = Screen_Height - y - 1 + LCD_Offset;
        for ( col = 0; col < width; col++ )
        {
            // Current GRAM horizontal position
            LCD_Write_Reg( ILI9325_GRAM_HADDR, col0 + col );

            for ( line = 0; line < height; line++ )
            {
                // Current GRAM Vertical position
                LCD_Write_Reg( ILI9325_GRAM_VADDR, line0 - line );

                // Read pixel 2 bytes
                LCD_PixelRead( &bmp );
            }
        }
        break;

    case V9:
        // Read pixels from LCD screen
        col0 = Screen_Width - y - 1 + LCD_Offset;
        line0 = Screen_Height - x - 1 + LCD_Offset;
        for ( line = 0; line < width; line++ )
        {
            // Current GRAM Vertical position
            LCD_Write_Reg( ILI9325_GRAM_VADDR, line0 - line );

            for ( col = 0; col < height; col++ )
            {
                // Current GRAM horizontal position
                LCD_Write_Reg( ILI9325_GRAM_HADDR, col0 - col );

                // Read pixel 2 bytes
                LCD_PixelRead( &bmp );
            }
        }
        break;

    }
}

/*******************************************************************************
*
*                                LCD_GetPixel
*
*******************************************************************************/
/**
*
*  Read the RGB color of the pixel the coordinate are provided in parameter.
*
*  @param[in]  x        The horizontal coordinate of the pixel.
*  @param[in]  y        The vertical coordinate of the pixel.
*  @return              An unsigned 16 bit word containing the RGB color of the pixel.
*
*  @warning    The (0x0) point in on the low left corner.
*  @see        LCD_RectRead
*
**/
/******************************************************************************/
u16 LCD_GetPixel( coord_t x, coord_t y )
{
    u16 val;
    u8 pixel[2];

    LCD_RectRead( x, y, 1, 1, &pixel[0] );
    val = ( pixel[1] << 8 ) | pixel[0];
    return val;
}

/*******************************************************************************
*
*                                LCD_SetRect_For_Cmd
*
*******************************************************************************/
/**
*
*  Define the rectangle for the next command to be applied.
*
*  @param[in]  x        The horizontal coordinate of the rectangle low left corner.
*  @param[in]  y        The vertical coordinate of the rectangle low left corner.
*  @param[in]  width    The rectangle width in pixels.
*  @param[in]  height   The rectangle height in pixels.
*
*  @warning    The (0x0) point in on the low left corner.
*
**/
/******************************************************************************/
void LCD_SetRect_For_Cmd( coord_t x, coord_t y, coord_t width, coord_t height )
{
    // Set GRAM window

    switch ( CurrentScreenOrientation%4 )
    {
    case V12:
        // Horizontal GRAM Start Address
        LCD_Write_Reg( ILI9325_HOR_ADDR_START, Screen_Width - x  - width + LCD_Offset );

        // Horizontal GRAM End Address
        LCD_Write_Reg( ILI9325_HOR_ADDR_END,  Screen_Width - x - 1 +  LCD_Offset );

        // Window Vertical GRAM Start Address
        LCD_Write_Reg( ILI9325_VET_ADDR_START, y + LCD_Offset ) ;

        // Window Vertical GRAM End Address
        LCD_Write_Reg( ILI9325_VET_ADDR_END, y + height - 1 + LCD_Offset );

        // Current GRAM horizontal position
        LCD_Write_Reg( ILI9325_GRAM_HADDR, Screen_Width - x - 1 + LCD_Offset );

        // Current GRAM Vertical position
        LCD_Write_Reg( ILI9325_GRAM_VADDR, y + LCD_Offset );
        break;

    case V3:
        // Horizontal GRAM Start Address
        LCD_Write_Reg( ILI9325_HOR_ADDR_START, y + LCD_Offset );

        // Horizontal GRAM End Address
        LCD_Write_Reg( ILI9325_HOR_ADDR_END, y + height - 1 + LCD_Offset );

        // Window Vertical GRAM Start Address
        LCD_Write_Reg( ILI9325_VET_ADDR_START, x + LCD_Offset ) ;

        // Window Vertical GRAM End Address
        LCD_Write_Reg( ILI9325_VET_ADDR_END, x + width - 1 + LCD_Offset );

        // Current GRAM horizontal position
        LCD_Write_Reg( ILI9325_GRAM_HADDR, y + LCD_Offset );

        // Current GRAM Vertical position
        LCD_Write_Reg( ILI9325_GRAM_VADDR, x + LCD_Offset );
        break;

    case V6:
        // Horizontal GRAM Start Address
        LCD_Write_Reg( ILI9325_HOR_ADDR_START, x + LCD_Offset );

        // Horizontal GRAM End Address
        LCD_Write_Reg( ILI9325_HOR_ADDR_END, x + width - 1  + LCD_Offset );

        // Window Vertical GRAM Start Address
        LCD_Write_Reg( ILI9325_VET_ADDR_START, Screen_Height - y - height + LCD_Offset ) ;

        // Window Vertical GRAM End Address
        LCD_Write_Reg( ILI9325_VET_ADDR_END, Screen_Height - y - 1 + LCD_Offset );

        // Current GRAM horizontal position
        LCD_Write_Reg( ILI9325_GRAM_HADDR, x + LCD_Offset );

        // Current GRAM Vertical position
        LCD_Write_Reg( ILI9325_GRAM_VADDR, Screen_Height - y - 1 + LCD_Offset );
        break;

    case V9:
        // Horizontal GRAM Start Address
        LCD_Write_Reg( ILI9325_HOR_ADDR_START,  Screen_Width - y - height + LCD_Offset );

        // Horizontal GRAM End Address
        LCD_Write_Reg( ILI9325_HOR_ADDR_END,  Screen_Width - y - 1 + LCD_Offset );

        // Window Vertical GRAM Start Address
        LCD_Write_Reg( ILI9325_VET_ADDR_START,  Screen_Height - x - width + LCD_Offset ) ;

        // Window Vertical GRAM End Address
        LCD_Write_Reg( ILI9325_VET_ADDR_END,  Screen_Height - x - 1 + LCD_Offset );

        // Current GRAM horizontal position
        LCD_Write_Reg( ILI9325_GRAM_HADDR, Screen_Width - y - 1 + LCD_Offset );

        // Current GRAM Vertical position
        LCD_Write_Reg( ILI9325_GRAM_VADDR, Screen_Height - x - 1 + LCD_Offset );
        break;
    }
}

/*******************************************************************************
*
*                                LCD_FullScreen
*
*******************************************************************************/
/**
*
*  Defines the area availbale for applications (full screen or not).
*
*  @param[in]  FullScreenONOFF  : ON  = full screen available for applications.
*                                 OFF = only application area available for applications.
*
*
*  @note    The screen orientation is stalled to the current orentation
*           defined before function call.
**/
/******************************************************************************/
void LCD_FullScreen( bool FullScreenONOFF )
{
    // Fullscreen requested ?
    if ( FullScreenONOFF != 0 )
        {
        // Yes, we stop the toolbar handler
        #if TOUCHSCREEN_AVAIL
//        UTIL_SetSchHandler(TOOLBAR_SCHHDL_ID, 0);
        #endif
        
        // We block rotation, offset and change limits for screen, char and pointer to whole screen
        LCD_SetRotateScreen(0);
        LCD_SetOffset(OFFSET_OFF);
        Screen_Width = PHYS_SCREEN_WIDTH;
        Screen_Height = PHYS_SCREEN_HEIGHT;
        
        // If V3 or V9 orientation, we swap width and height limits
        if( (LCD_GetScreenOrientation() % 2) == 1 )
            {
            PosCurY = PHYS_SCREEN_WIDTH - Char_Height;
            RightMarginX = PHYS_SCREEN_HEIGHT;
            HighMarginY = PHYS_SCREEN_WIDTH;
            LCD_DrawCharSetFilter( 0, PHYS_SCREEN_HEIGHT, 0, PHYS_SCREEN_WIDTH );
//            POINTER_SetRect( 0, 0, PHYS_SCREEN_HEIGHT - CurrentPointerWidth, PHYS_SCREEN_WIDTH - CurrentPointerHeight);
            }
        else
            {
            PosCurY = PHYS_SCREEN_HEIGHT - Char_Height;
            RightMarginX = PHYS_SCREEN_WIDTH;
            HighMarginY = PHYS_SCREEN_HEIGHT;
            LCD_DrawCharSetFilter( 0, PHYS_SCREEN_WIDTH, 0, PHYS_SCREEN_HEIGHT );
//            POINTER_SetRect( 0, 0, PHYS_SCREEN_WIDTH - CurrentPointerWidth, PHYS_SCREEN_HEIGHT - CurrentPointerHeight);
            }
        }
    else
        {
        // No, we come back normal mode 
        // We restore toolbar handler
        #if TOUCHSCREEN_AVAIL
//        UTIL_SetSchHandler(TOOLBAR_SCHHDL_ID, TOOLBAR_Handler);
        #endif
        
        // We allow rotation, 
        LCD_SetRotateScreen(1);
        
        // We restore screen, char and pointer to application screen limits
        LCD_SetOffset(OFFSET_ON);
        LCD_DrawCharSetFilter( 0, APP_SCREEN_WIDTH, 0, APP_SCREEN_HEIGHT );
//        POINTER_SetRectScreen();
        }

}

