/****************** COPYRIGHT (C) 2007-2010 RAISONANCE S.A.S. *****************/
/**
*
* @file     lcd.c
* @brief    The common LCD functions.
* @author   FL
* @date     07/2007
* @version  4.0 Add Open4 Primer
* @date     10/2009
*
**/
/******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "circle.h"

/// @cond Internal

/* Private define ------------------------------------------------------------*/
#define BACKLIGHT_DIVIDER           500      /*!< LCD handler step.           */

/* Private variables ---------------------------------------------------------*/
divider_t   HandlerDivider = 0;
static coord_t CharFilter_XMin = 0;                     /* Restricted area when painting characters */
static coord_t CharFilter_XMax = APP_SCREEN_WIDTH;      /* Restricted area when painting characters */
static coord_t CharFilter_YMin = 0;                     /* Restricted area when painting characters */
static coord_t CharFilter_YMax = APP_SCREEN_HEIGHT;     /* Restricted area when painting characters */


/* External variable ---------------------------------------------------------*/

/* Public variables ---------------------------------------------------------*/
coord_t     Screen_Width;
coord_t     Screen_Height;
ENUM_Offset LCD_Offset;
u8          Char_Width = 7;
u8          Char_Height = 14;
u8          Menu_Font = 0;
u8          DeltaY;
u8          Transparency;

const u8* CurrentFont;
tFontDef* CurrentFontDef = (tFontDef*) &Font_Table.fonts[0];
color_t bmpTmp[LCD_DMA_SIZE];        /* 16 bits words : buffer to scroll display */

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
*
*                                LCD_DrawCharSetFilter
*
*******************************************************************************/
/**
*  Define a restriction area for LCD_DrawChar (used for the first/last line when scrolling lists).
*
*  @param[in]  xmin        The first authorized line
*  @param[in]  xmax        The first non-authorized line
*  @param[in]  ymin        The first authorized column
*  @param[in]  ymax        The first non-authorized column
*
**/
/******************************************************************************/
void LCD_DrawCharSetFilter( coord_t xmin, coord_t xmax, coord_t ymin, coord_t ymax )
{
    CharFilter_XMin = xmin;
    CharFilter_XMax = xmax;
    CharFilter_YMin = ymin;
    CharFilter_YMax = ymax;
}


/*******************************************************************************
*
*                                LCD_DrawChar
*
*******************************************************************************/
/**
*  Draw a character on the LCD screen.
*
*  @param[in]  x           The line where to display the character shape.
*  @param[in]  y           The column start address.
*  @param[in]  width       The number of columns (dots) in a character width.
*  @param[in]  bmp         The character (monochrome) bitmap. A pointer of the dot matrix data.
*  @param[in]  textColor   The character color.
*  @param[in]  bGndColor   The character background color.
*  @param[in]  charMagniCoeff The character magnifying coefficient.
*
*  @warning    The (0x0) point in on the low left corner.
*
**/
/******************************************************************************/
void LCD_DrawChar( coord_t x, coord_t y, coord_t width, const u8* bmp, color_t textColor, color_t bGndColor, mag_t charMagniCoeff )
{
    coord_t i;
    coord_t i0 = 0;
    coord_t i1 = Char_Width;
    counter_t j;
    mag_t k1;
    mag_t k2;
    coord_t lin0;
    coord_t lin1;
    coord_t x0 = MAX( x, CharFilter_XMin );
    coord_t y0 = MAX( y, CharFilter_YMin );
    static coord_t width0;
    static coord_t height0;
    u8 j2;

    UNREFERENCED_PARAMETER( width );

    width0 = MIN( Char_Width * charMagniCoeff,  CharFilter_XMax - x0 );
    height0 = MIN( Char_Height * charMagniCoeff,  CharFilter_YMax - y0 );


    if ( y0 > y )
        height0 -= ( y0 - y );

    if ( x0 > x )
        width0 -= ( x0 - x );

    if (( width0 <= 0 ) || ( height0 <= 0 ) )
        return;

    i0 = ( x0 - x ) / charMagniCoeff;
    i1 = width0 / charMagniCoeff;
    lin0 = ( y0 - y ) ;
    lin1 = height0 + lin0;
    
    for ( i = i0; i < i1; i++ )
    {
        if ( Transparency && (charMagniCoeff < 4) ) // save the column pixels if transparency is on
            LCD_RectRead(x0+(charMagniCoeff*i), y0, charMagniCoeff, height0, (u8*) &bmpTmp );

        /* Select the area for LCD output.*/
        LCD_SetRect_For_Cmd( x0+(charMagniCoeff*i), y0, charMagniCoeff, height0 );

        /* Select LCD output mode.*/
        LCD_SendLCDCmd_RAM_Access();

        for ( k1 = 0; k1 < charMagniCoeff; k1++ )
        {
            coord_t n = 0;
            j2 = 0;
            for ( j = 0x80; j; j >>= 1 , j2++ ) // For all 8 bits
            {
                for ( k2 = 0; k2 < charMagniCoeff; k2++ , n++ )
                {
                    if (( n < lin0 ) || ( n >= lin1 ) )
                        continue;
                    if ( (Transparency == 0) || (charMagniCoeff>3) )
                    {
                        LCD_SendLCDData(( bmp[2 * i] & j ) ? ( textColor & 255 ) : ( bGndColor &  255 ) );
                        LCD_SendLCDData(( bmp[2 * i] & j ) ? ( textColor >> 8 ) : ( bGndColor >> 8 ) );
                    }
                    else
                    {
                        if ( ( bmp[2 * i] & j ) )
                        {
                            LCD_SendLCDData( textColor & 255  );
                            LCD_SendLCDData( textColor >> 8 );
                        }
                        else
                        {
                            LCD_SendLCDData(bmpTmp[height0*k1+n]&0xff);
                            LCD_SendLCDData(bmpTmp[height0*k1+n]>>8);
                        }
                    }
                }
            }

            for ( j = 0x80; j > (16-Char_Height); j >>= 1 , j2++ ) // 8
            {
                for ( k2 = 0; k2 < charMagniCoeff; k2++ , n++ )
                {
                    if (( n < lin0 ) || ( n >= lin1 ) )
                        continue;

                    if ( (Transparency == 0) || (charMagniCoeff>3) )
                    {
                        LCD_SendLCDData(( bmp[2 * i + 1] & j ) ? ( textColor & 255 ) : ( bGndColor &  255 ) );
                        LCD_SendLCDData(( bmp[2 * i + 1] & j ) ? ( textColor >> 8 ) : ( bGndColor >> 8 ) );
                    }
                    else
                    {
                        if ( ( bmp[2 * i + 1] & j ) )
                        {
                            LCD_SendLCDData( textColor & 255  );
                            LCD_SendLCDData( textColor >> 8 );
                        }
                        else
                        {
                            LCD_SendLCDData(bmpTmp[height0*k1+n]&0xff);
                            LCD_SendLCDData(bmpTmp[height0*k1+n]>>8);
                        }
                    }
                }
            }
        }
        /* End of select screen area to access.*/
        LCD_SendLCDCmd_RAM_Access_End();
    }

}


/* Public functions for CircleOS ---------------------------------------------*/


/*******************************************************************************
*
*                                LCD_Handler
*
*******************************************************************************/
/**
*
*  Called by the CircleOS scheduler to manage LCD tasks.
*
*  @attention  This function must <b>NOT</b> be called by the user.
*
**/
/******************************************************************************/
void LCD_Handler( void )
{
#if BACKLIGHT_INTERFACE
    if ( ++HandlerDivider % BACKLIGHT_DIVIDER )
    {
        return;
    }

    LCD_BackLightChange();
#endif // BACKLIGHT_INTERFACE
}


/*******************************************************************************
*
*                                LCD_FillRect_Circle
*
*******************************************************************************/
/**
*
*  Fill a rectangle with a provided color.
*
*  @param[in]  x        The horizontal coordinate of the rectangle low left corner.
*  @param[in]  y        The vertical coordinate of the rectangle low left corner.
*  @param[in]  width    The rectangle width in pixels.
*  @param[in]  height   The rectangle height in pixels.
*  @param[in]  color    The RGB color to fill the rectangle with.
*
*  @warning    The (0x0) point in on the low left corner.
*
*  Rq : this function does not check parameters validity
*
**/
/******************************************************************************/
void LCD_FillRect_Circle( coord_t x, coord_t y, coord_t width, coord_t height, color_t color )
{
    coord_t Line;
    coord_t Column;

    /* Select LCD screen area. */
    LCD_SetRect_For_Cmd( x, y, width, height );

    /* Send LCD RAM write command. */
    LCD_SendLCDCmd_RAM_Access();

    /* Fill selected LCD screen area with provided color. */
    for ( Line = width; Line > 0; --Line )
    {
        for ( Column = height; Column > 0; Column-- )
        {
            LCD_SendLCDData( color & 0xff );
            LCD_SendLCDData(( color >> 8 ) & 0xff );
        }
    }

    /* End of select screen area to access.*/
    LCD_SendLCDCmd_RAM_Access_End();
}

/*******************************************************************************
*
*                                LCD_ClearAllScreen
*
*******************************************************************************/
/**
*
*  Fill all the screen (toolbar included) with a specified color.
*
*  @param[in]  color   The color to fill the screen.
*
*  @warning    The (0x0) point in on the low left corner.
*
**/
/******************************************************************************/
NODEBUG2 void LCD_ClearAllScreen( color_t color )
{
    // Backup orientation
    Rotate_H12_V_Match_TypeDef backupOrientation = LCD_GetScreenOrientation();

    // Clear the application and toolbar screen
    LCD_SetScreenOrientation( V12 );
    LCD_FillRect_Circle( 0, 0, PHYS_SCREEN_WIDTH, PHYS_SCREEN_HEIGHT, color );

    // Restore orientation
    LCD_SetScreenOrientation( backupOrientation );
}

/// @endcond

/* Public functions ----------------------------------------------------------*/


/*******************************************************************************
*
*                                LCD_FillRect
*
*******************************************************************************/
/**
*
*  Fill a rectangle with a provided color.
*
*  @param[in]  x        The horizontal coordinate of the rectangle low left corner.
*  @param[in]  y        The vertical coordinate of the rectangle low left corner.
*  @param[in]  width    The rectangle width in pixels.
*  @param[in]  height   The rectangle height in pixels.
*  @param[in]  color    The RGB color to fill the rectangle with.
*
*  @warning    The (0x0) point in on the low left corner.
*
**/
/******************************************************************************/
void LCD_FillRect( coord_t x, coord_t y, coord_t width, coord_t height, color_t color )
{

    /* Rq : this fuction is just a wrapper with parameter limit checks*/

    if( (LCD_GetScreenOrientation()%2) == 1 )
        {
        if ( x > Screen_Height )
            x = Screen_Height;
        if (( x + width ) > Screen_Height )
            width = Screen_Height - x;

        /* Check the y parameters*/
        if ( y > Screen_Width )
            y = Screen_Width;
        if (( y + height ) > Screen_Width )
            height = Screen_Width - y;
        }
    else
        {
        /* Check the x parameters*/
        if ( x > Screen_Width )
            x = Screen_Width;
        if (( x + width ) > Screen_Width )
            width = Screen_Width - x;

        /* Check the y parameters*/
        if ( y > Screen_Height )
            y = Screen_Height;
        if (( y + height ) > Screen_Height )
            height = Screen_Height - y;
        }


    /* Call the native function*/
    LCD_FillRect_Circle( x, y, width, height, color );
}

/*******************************************************************************
*
*                                LCD_DrawRect
*
*******************************************************************************/
/**
*
*  Draw a rectangle with a provided color.
*
*  @param[in]  x        The horizontal coordinate of the rectangle low left corner.
*  @param[in]  y        The vertical coordinate of the rectangle low left corner.
*  @param[in]  width    The rectangle width in pixels.
*  @param[in]  height   The rectangle height in pixels.
*  @param[in]  color    The RGB color to draw the rectangle with.
*
*  @warning    The (0x0) point in on the low left corner.
*
**/
/******************************************************************************/
void LCD_DrawRect( coord_t x, coord_t y, coord_t width, coord_t height, color_t color )
{
    /* Draw horizontal sides.*/
    LCD_FillRect( x, y,              width, 1, color );
    LCD_FillRect( x, y + height - 1, width, 1, color );

    /* Draw vertical sides.*/
    LCD_FillRect( x,              y, 1, height, color );
    LCD_FillRect( x + width - 1,  y, 1, height, color );
}


/*******************************************************************************
*
*                                LCD_DisplayChar
*
*******************************************************************************/
/**
*
*  Display at provided coordinates the provided ASCII character with the provided
*  text and background colors and with the provided magnify coefficient.
*
*  @param[in]  x              The horizontal coordinate of the character.
*  @param[in]  y              The vertical coordinate of the character.
*  @param[in]  ascii          The ASCII code of the character to display.
*                             @n Ascii must be higher than 31 and lower than 255.
*  @param[in]  textColor      The color used to draw the character.
*  @param[in]  bGndColor      The background color of the drawn character.
*  @param[in]  charMagCoeff   The magnify coefficient used to draw the character.
*
*  @warning    The (0x0) point in on the low left corner.
*
**/
/******************************************************************************/
void LCD_DisplayChar( coord_t x, coord_t y, u8 ascii, color_t  textColor, color_t  bGndColor, mag_t charMagCoeff )
{
    /* Display the selected bitmap according to the provided ASCII character.*/
    if ( (ascii >= CurrentFontDef->ASCII_start) && (ascii <= CurrentFontDef->ASCII_end) )
        LCD_DrawChar( x, y, Char_Width, ( u8* )&CurrentFont[( ascii-CurrentFontDef->ASCII_start ) * (2*Char_Width)], textColor, bGndColor, charMagCoeff );
}

#if BACKLIGHT_INTERFACE
/*******************************************************************************
*
*                                LCD_SetBackLight
*
*******************************************************************************/
/**
*
*  Modify the PWM rate. Any value below BACKLIGHTMIN reset the value to the
*  default value (DEFAULT_CCR_BACKLIGHTSTART).
*
*  @param[in]  newBacklightStart The new PWM rate.
*
**/
/******************************************************************************/
void LCD_SetBackLight( backlight_t newBacklightStart )
{
    if ( newBacklightStart >= BACKLIGHTMIN )
    {
        Current_CCR_BackLightStart = newBacklightStart;
    }
    else
    {
        Current_CCR_BackLightStart = DEFAULT_CCR_BACKLIGHTSTART;
    }
}

/*******************************************************************************
*
*                                LCD_SetBackLightOff
*
*******************************************************************************/
/**
*
*  Switch the LCD back light off.
*
**/
/******************************************************************************/
void LCD_SetBackLightOff( void )
{
    Current_CCR_BackLightStart = 0;
}

/*******************************************************************************
*
*                                LCD_SetBackLightOn
*
*******************************************************************************/
/**
*
*  Switch the LCD back light on.
*
**/
/******************************************************************************/
void LCD_SetBackLightOn( void )
{
    Current_CCR_BackLightStart = DEFAULT_CCR_BACKLIGHTSTART;
}

/*******************************************************************************
*
*                                LCD_GetBackLight
*
*******************************************************************************/
/**
*
*  Returns le LCD PWM rate.
*
*  @return The current LCD PWM rate.
*
**/
/******************************************************************************/
backlight_t LCD_GetBackLight( void )
{
    return Current_CCR_BackLightStart;
}

#endif // BACKLIGHT_INTERFACE

/*******************************************************************************
*
*                                LCD_SetRotateScreen
*
*******************************************************************************/
/**
*
*  Enable or disable the ability of the screen display to rotate according to
*  the MEMs information.
*
*  @param[in]  RotateScreen 0 to disable screen rotation and 1 to enable.
*
**/
/******************************************************************************/
void LCD_SetRotateScreen( bool RotateScreen )
{
    CurrentRotateScreen = RotateScreen;
}

/*******************************************************************************
*
*                                LCD_GetRotateScreen
*
*******************************************************************************/
/**
*
*  Return the screen rotation mode.
*
*  @retval 0 screen rotation is disabled.
*  @retval 1 screen rotation is enabled.
*
**/
/******************************************************************************/
bool LCD_GetRotateScreen( void )
{
    return CurrentRotateScreen;
}

/*******************************************************************************
*
*                                LCD_SetScreenOrientation
*
*******************************************************************************/
/**
*
*  Set the screen orientation.
*
*  @param[in]  ScreenOrientation The new screen orientation.
*
**/
/******************************************************************************/
void LCD_SetScreenOrientation( Rotate_H12_V_Match_TypeDef ScreenOrientation )
{
    CurrentScreenOrientation = ScreenOrientation;

    LCD_DisplayRotate( CurrentScreenOrientation );
}

/*******************************************************************************
*
*                                LCD_GetScreenOrientation
*
*******************************************************************************/
/**
*
*  Return current screen orientation.
*
*  @return   A Rotate_H12_V_Match_TypeDef telling the current screen orientation.
*
**/
/******************************************************************************/
Rotate_H12_V_Match_TypeDef LCD_GetScreenOrientation( void )
{
    return CurrentScreenOrientation;
}

/*******************************************************************************
*
*                                LCD_SetFont
*
*******************************************************************************/
/**
*   Change the current font, with the specified new one.
*
*   @param[in] NewFont : pointer on new font array
*
**/
/******************************************************************************/
void LCD_SetFont( const u8* NewFont )
{
    CurrentFont = NewFont;
}

/*******************************************************************************
*
*                                LCD_SetFontDef
*
*******************************************************************************/
/**
*   Change the current font definitition to fit with a new current font.
*
*   @param[in] FontDef : Pointer of the new font definition structure
*
**/
/******************************************************************************/
void LCD_SetFontDef( tFontDef* FontDef )
{
    if(FontDef)
    {
        CurrentFontDef = FontDef;
        Char_Width = CurrentFontDef->width;
        Char_Height = CurrentFontDef->height;
        DRAW_SetCharMagniCoeff(CurrentFontDef->FontCoeff);
        //Set DeltaY to prevent scrolling bugs (if odd height or magnification coefficient > 1 )
        DeltaY = ((( Char_Height % 2 == 0 ) || ( DRAW_GetCharMagniCoeff()==2 ) ) ? 2 : 1 );
        
    }
}

/*******************************************************************************
*
*                                LCD_GetFontDef
*
*******************************************************************************/
/**
*   Get the current font definitition.
*
*   @return : Pointer of the curent font definition structure
*
**/
/******************************************************************************/
tFontDef* LCD_GetFontDef( void )
{
    return CurrentFontDef;
}

/*******************************************************************************
*
*                                LCD_SetDefaultFont
*
*******************************************************************************/
/**
*   Restore the default system font.
*
*
**/
/******************************************************************************/
void LCD_SetDefaultFont( void )
{
    LCD_ChangeFont(0);
}

/*******************************************************************************
*
*                                LCD_ChangeFont
*
*******************************************************************************/
/**
*   Change the current font and its definitition.
*
*   @param[in] ID : the font's ID
*
**/
/******************************************************************************/
void LCD_ChangeFont( enum ENUM_FontID ID )
{
    if ( ID < Font_Table.nb )
    {
        CurrentFontDef = (tFontDef*) &Font_Table.fonts[ID];
        Char_Width = CurrentFontDef->width;
        Char_Height = CurrentFontDef->height;
        CurrentFont = CurrentFontDef->font;
        DRAW_SetCharMagniCoeff(CurrentFontDef->FontCoeff);
        //Set DeltaY to prevent scrolling bugs (if odd height or magnification coefficient > 1 )
        DeltaY = ((( Char_Height % 2 == 0 ) || ( DRAW_GetCharMagniCoeff()==2 ) ) ? 2 : 1 );
    }
}

/*******************************************************************************
*
*                                LCD_SetOffset
*
*******************************************************************************/
/**
*
*  Activate the offset
*
*   @param[in]  Offset  new offset state
*   @note OFFSET_OFF = no offset,
*         OFFSET_ON  = x pixels are turned black on each side of the screen.
*
**/
/******************************************************************************/
void LCD_SetOffset( ENUM_Offset Offset )
{

    if ( Offset == OFFSET_ON )
    {
        LCD_FillRect( 0, 0, Offset, APP_SCREEN_HEIGHT, RGB_WHITE );
        LCD_FillRect( 0, 0, APP_SCREEN_WIDTH, Offset, RGB_WHITE );
        LCD_FillRect( 0, APP_SCREEN_HEIGHT - Offset, APP_SCREEN_WIDTH, Offset, RGB_WHITE );
        LCD_FillRect( APP_SCREEN_WIDTH - Offset, 0, Offset, APP_SCREEN_HEIGHT, RGB_WHITE );
    }

    LCD_Offset = Offset;

    Screen_Width = APP_SCREEN_WIDTH - ( 2 * LCD_Offset );
    Screen_Height = APP_SCREEN_HEIGHT - ( 2 * LCD_Offset );

/*
    POINTER_Info.xPos = ( Screen_Width / 2 ) - ( POINTER_WIDTH / 2 ) - 1;
    POINTER_Info.yPos = ( Screen_Width / 2 ) - ( POINTER_WIDTH / 2 ) - 1;
    POINTER_Info.shift_PosX = 0;
    POINTER_Info.shift_PosY = 0;
*/
    PosCurY = Screen_Height - Char_Height;
    RightMarginX = Screen_Width;
    HighMarginY = Screen_Height;

    LCD_DrawCharSetFilter( 0, Screen_Width, 0, Screen_Height );

}

/*******************************************************************************
*
*                                LCD_GetScreenWidth
*
*******************************************************************************/
/**
*
*  Get the screen width
*
*   @return     Width of the screen in pixels.
*
**/
/******************************************************************************/
coord_t LCD_GetScreenWidth( void )
{
    return Screen_Width;
}

/*******************************************************************************
*
*                                LCD_GetScreenHeight
*
*******************************************************************************/
/**
*
*  Get the screen height
*
*   @return     Height of the screen in pixels.
*
**/
/******************************************************************************/
coord_t LCD_GetScreenHeight( void )
{
    return Screen_Height;
}

/*******************************************************************************
*
*                                LCD_SetTransparency
*
*******************************************************************************/
/**
*
*  Set the transparency value for the characters
*
*   @param[in] NewTransparency : new transparency value.
*
**/
/******************************************************************************/
void LCD_SetTransparency( u8 NewTransparency )
{
    Transparency = NewTransparency;
}

/*******************************************************************************
*
*                                LCD_GetTransparency
*
*******************************************************************************/
/**
*
*  Get the transparency value for the characters.
*
*   @return     Current transparency value.
*
**/
/******************************************************************************/
u8 LCD_GetTransparency( void )
{
    return Transparency;
}
