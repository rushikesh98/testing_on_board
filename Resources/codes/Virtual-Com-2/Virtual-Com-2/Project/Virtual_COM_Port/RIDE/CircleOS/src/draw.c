/****************** COPYRIGHT (C) 2007-2010 RAISONANCE S.A.S. *****************/
/**
*
* @file     draw.c
* @brief    Various utilities for drawings (characters, ..)
* @author   FL
* @author   IB
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
#define ROTATE_DIVIDER  127

#define abs(X) ( ( (X) < 0 ) ? -(X) : (X) )
#define sgn(X) ( ( (X) < 0 ) ? -1 : 1 )


/* Private variables ---------------------------------------------------------*/
mag_t   CharMagniCoeff = 1;               // !< Current character magnify coefficient
color_t BGndColor = RGB_WHITE;            // !< Current background color
color_t TextColor = RGB_BLACK;            // !< Current text color

coord_t PosCurX = 0;                      // !< Current cursor X position
coord_t PosCurY = 0;                      // !< Current cursor Y position

coord_t LeftMarginX = 0;                  // !< Current left margin in X
coord_t RightMarginX = APP_SCREEN_WIDTH; // !< Current right margin in X
coord_t HighMarginY = APP_SCREEN_HEIGHT; // !< Current high margin in Y
coord_t LowMarginY = 0;                   // !< Current low margin in Y

bool fDisplayTime = 0;
u16 BatState;
u16 OldBatState;
s16 OldTemp = -1;                // Stores current temperature

coord_t xBat;
coord_t yBat;
coord_t widthBat;
coord_t heightBat;

divider_t divider_coord = 0;

/* Screen orientation management*/
divider_t rotate_counter             = 0;

/*Initialise the direction to +4 at reset (that forces a redraw)*/
Rotate_H12_V_Match_TypeDef previous_orient            = V12;
Rotate_H12_V_Match_TypeDef previous_previous_orient   = V12;
Rotate_H12_V_Match_TypeDef new_orient                 = V12;
Rotate_H12_V_Match_TypeDef CurrentScreenOrientation   = V12;
bool                       CurrentRotateScreen        = 1;

/* Extern variables ----------------------------------------------------------*/
extern coord_t XInit;
extern coord_t YInit;

/* Private functions ---------------------------------------------------------*/
void DRAW_Clear_Text( coord_t x, coord_t y, len_t len, bool mode );

/*******************************************************************************
*
*                                DRAW_Clear_Text
*
*******************************************************************************/
/**
*
*  This function is used to display clear a text
*  at given X, Y coordinates on the LCD display.
*  Note:
*  This function is the user interface to use the LCD driver.
*
*  @param[in] x      The horizontal coordinate of the text.
*  @param[in] y      The vertical coordinate of the text.
*  @param[in] len    Text length i.e., number of characters to display.
*  @param[in] mode   Display mode: 0 normal, 1 inverted colors.
*
*  @note          Characters are displayed as 7x14 pixels blocks.
*  @warning       The (0x0) point in on the low left corner.
*
**/
/******************************************************************************/
void DRAW_Clear_Text( coord_t x, coord_t y, len_t len, bool mode )
{
    len_t i;
    coord_t ref_x = x;

    /* Display each character on LCD */
    for ( i = 0 ; i < len ; i++ )
    {
        /* Display one character on LCD */
        LCD_DisplayChar( ref_x, y, ' ', mode ? BGndColor : TextColor,  mode ? TextColor : BGndColor, CharMagniCoeff );

        /* Increment the column position by character width */
        ref_x += ( Char_Width * CharMagniCoeff );
    }
}


/* Public functions for CircleOS ---------------------------------------------*/

/*******************************************************************************
*
*                                DRAW_Init
*
*******************************************************************************/
/**
*
*  Initialize GUI drawing. Called at CircleOS startup.
*
*  @attention  This function must <b>NOT</b> be called by the user.
*
**/
/******************************************************************************/
NODEBUG2 void DRAW_Init( void )
{

#if MEMS_POINTER     // Not necessary if no MEMS menu handling
    MEMS_GetRotation( &CurrentScreenOrientation );
#endif

    LCD_SetScreenOrientation( CurrentScreenOrientation );

    xBat        = Screen_Width - 30;
    yBat        = 3;
    OldBatState = 10;
    PosCurY = Screen_Height - Char_Height;
    RightMarginX = Screen_Width;
    HighMarginY = Screen_Height;

    /* Clear LCD and draw black and white logo*/
    DRAW_SetDefaultColor();
    DRAW_Clear();
//    LCD_Batt( xBat, yBat, fDisplayTime, BatState, &OldBatState, divider_coord, &widthBat, &heightBat );
//    POINTER_Init();
}

/*******************************************************************************
*
*                                DRAW_Handler
*
*******************************************************************************/
/**
*
*  Called by CircleOS to manage DRAW tasks such as screen orientation.
*
*  @attention  This function must <b>NOT</b> be called by the user.
*
**/
/******************************************************************************/
void DRAW_Handler( void )
{

    BatState = UTIL_GetBat();
    LCD_Batt( xBat, yBat, fDisplayTime, BatState, &OldBatState, divider_coord, &widthBat, &heightBat );

#if MEMS_POINTER     // Not necessary if no MEMS menu handling
    /* Rotate LCD screen*/
    if ( LCD_GetRotateScreen() )
    {
        rotate_counter++;

        if ( rotate_counter == ROTATE_DIVIDER )
        {
            MEMS_GetRotation( &new_orient );

            if ( new_orient != previous_previous_orient )
            {
                if (( new_orient == previous_orient ) && ( new_orient != CurrentScreenOrientation  ) )
                {
                    CurrentScreenOrientation = new_orient;
                    LCD_SetScreenOrientation( CurrentScreenOrientation );
                    MEMS_GetPosition( &XInit, &YInit );
#if TOUCHSCREEN_AVAIL
                    TOOLBAR_RedrawToolbar(TOOLBAR_REDRAW);
#endif
                    DRAW_Clear();
                    if ( CurrentMenu != 0 )
                    {
                        MENU_Set( CurrentMenu );
                    }
                }
            }

            previous_previous_orient = previous_orient;
            previous_orient          = new_orient;
            rotate_counter           = 0;
        }
    }
#endif
}

/*******************************************************************************
*
*                                DRAW_Batt
*
*******************************************************************************/
/**
*
*  Draw the battery at xBat and yBat set in DRAW_Ini
*
**/
/******************************************************************************/
NODEBUG2 void DRAW_Batt( void )
{
    widthBat = 24;
    heightBat = 8;

    LCD_FillRect_Circle( xBat, yBat, widthBat, heightBat, RGB_BLUE );
    LCD_FillRect_Circle( xBat + 1 , yBat + 1 , widthBat - 2 , heightBat - 2 , RGB_WHITE );

    widthBat = 2;
    heightBat = 4;

    LCD_FillRect_Circle( xBat - 2 , yBat + 2 , widthBat , heightBat, RGB_BLUE );
    LCD_FillRect_Circle( xBat - 1 , yBat + 3 , widthBat - 1 , heightBat - 2 , RGB_WHITE );
}


/*******************************************************************************
*
*                                DRAW_Line_Circle
*
*******************************************************************************/
/**
*  Draw a line on the LCD screen. Optimized for horizontal/vertical lines,
*  and use the Bresenham algorithm for other cases.
*
*  @param[in]  x1          The x-coordinate of the first line endpoint.
*  @param[in]  x2          The x-coordinate of the second line endpoint.
*  @param[in]  y1          The y-coordinate of the first line endpoint.
*  @param[in]  y2          The y-coordinate of the second line endpoint.
*  @param[in]  color       The line color.
*
* Rq : this fucntion does not check parameter validity
*
**/
/******************************************************************************/
NODEBUG2 void DRAW_Line_Circle( coord_t x1, coord_t y1, coord_t x2, coord_t y2, color_t color )
{
    s16 i;
    coord_t dx,    dy;
    coord_t dxabs, dyabs;
    coord_t sdx,   sdy;
    coord_t x,     y;
    coord_t px,    py;

    dx = x2 - x1;       /* the horizontal distance of the line */

    if ( dx == 0 )     /* Vertical Line*/
    {
        if ( y1 > y2 ) /* We assume y2>y1 and invert if not*/
        {
            i = y2;
            y2 = y1;
            y1 = i;
        }
        LCD_FillRect_Circle( x1, y1, 1, y2 - y1 + 1, color );
        return;
    }

    dy = y2 - y1;       /* the vertical distance of the line */

    if ( dy == 0 )     /* Horizontal Line*/
    {
        if ( x1 > x2 ) /* We assume x2>x1 and we swap them if not*/
        {
            i = x2;
            x2 = x1;
            x1 = i;
        }
        LCD_FillRect_Circle( x1, y1, x2 - x1 + 1, 1, color );
        return;
    }

    dxabs = abs( dx );
    dyabs = abs( dy );
    sdx = sgn( dx );
    sdy = sgn( dy );
    x = dyabs >> 1;
    y = dxabs >> 1;
    px = x1;
    py = y1;

    if ( dxabs >= dyabs ) /* the line is more horizontal than vertical */
    {
        for ( i = 0; i < dxabs; i++ )
        {
            y += dyabs;
            if ( y >= dxabs )
            {
                y -= dxabs;
                py += sdy;
            }
            px += sdx;
            LCD_DrawPixel( px, py, color );
        }
    }
    else /* the line is more vertical than horizontal */
    {
        for ( i = 0; i < dyabs; i++ )
        {
            x += dxabs;
            if ( x >= dyabs )
            {
                x -= dyabs;
                px += sdx;
            }
            py += sdy;
            LCD_DrawPixel( px, py, color );
        }
    }
}

/******************************************************************************
*
*                                DRAW_RestoreCharMagniCoeff
*
*******************************************************************************/
/**
*
*  Restore the default system magnifying value for the characters
*
**/
/******************************************************************************/
NODEBUG2 void DRAW_RestoreCharMagniCoeff( void )
{
#if LCD_HIGH_DEF
    DRAW_SetCharMagniCoeff( 2 );
#else
    DRAW_SetCharMagniCoeff( 1 );
#endif
}

/*******************************************************************************
*
*                                DRAW_Cross_Absolute
*
*******************************************************************************/
/**
*
*       Draw a cross on the screen.
*       @param[in] x1, y1 : coordinates of the crow
*       @param[in] color : color of the crow
*       @param[in] CrossSize : size of the cross in pixels
**/
/******************************************************************************/
NODEBUG2 void DRAW_Cross_Absolute( coord_t x1, coord_t y1, color_t color, u16 CrossSize )
{
    DRAW_Line( x1 - CrossSize, y1, x1 + CrossSize, y1, color );
    DRAW_Line( x1, y1 - CrossSize, x1, y1 + CrossSize, color );
}

/*******************************************************************************
*
*                                DRAW_DisplayTemp
*
*******************************************************************************/
/**
*
*  This function is used to display the current temperature in ascii.
*  The choice between Celcius and Fahrenheit is fixed by UTIL_SetModeTemp()
*
*  @param[in]  x  The horizontal coordinate of the displayed string.
*  @param[in]  y  The vertical coordinate of the display string.
*
*  @warning       The (0x0) point in on the low left corner.
*  @warning       Temperature is displayed *only* if it has changed since last
*                 call or if DRAW_Clear() has been called in between.
*
**/
/******************************************************************************/
NODEBUG2 void DRAW_DisplayTemp( coord_t x, coord_t y )
    {
    s16 Temp;
    u8 c;
    u16 r = 0;
    u8 TempTextBuffer[5];
    bool firstTime = 0;

    if (OldTemp == -1)
        {
        /* Get the intial value */
        Temp = OldTemp = UTIL_GetTemp();
        firstTime = 1;
        }
    else
        {
        /* Get Time and filter it with the previous value*/
        Temp = ( UTIL_GetTemp() + OldTemp * 7 ) / 8;
        OldTemp = Temp;
        }

    // Display value if changed or first call
    if ( (Temp != OldTemp ) || firstTime )
        {
        // Display modified temperature
        /* 1 000 digit*/
        c = (( Temp - r ) / 100 );
        r = r + ( c * 100 );
        TempTextBuffer[0] = c + 0x30;

        /* 100 digit*/
        c = (( Temp - r ) / 10 );
        r = r + ( c * 10 );
        TempTextBuffer[1] = c + 0x30;

        /* Dot*/
        TempTextBuffer[2] = '.';

        /* 10 digit*/
        c = (( Temp - r ));
        TempTextBuffer[3] = c + 0x30;

        /* Volt*/
        TempTextBuffer[4] = fTemperatureInFahrenheit ? 'F' : 'C';
        TempTextBuffer[5] = 0;    
        
        DRAW_DisplayString( x, y, TempTextBuffer, 5 );
        }
    }

/*******************************************************************************
*
*                                DRAW_GetIndexedColorBMP
*
*******************************************************************************/
/**
*
*  This function is used to get a color from a BMP palette.
*
*  @param[in] color_table   Pointer of the BMP color palette.
*  @param[in] i             Position of the first color byte.
*
*  @return                  The color formated for the screen.
*
**/
/******************************************************************************/
NODEBUG2 color_t DRAW_GetIndexedColorBMP( u8* color_table, u16 i )
{
    color_t color;
    color =  (color_table[i] & 0x1c) << 11;   //green (low)
    color |= (color_table[i + 1] & 0xf8) <<5; //blue
    color |= (color_table[i + 3] & 0xf8);     //red
    color |= (color_table[i] & 0xe0) >> 5;    //green (high)
    return color;
}


/// @endcond

/* Public functions ----------------------------------------------------------*/

/*******************************************************************************
*
*                                DRAW_SetCharMagniCoeff
*
*******************************************************************************/
/**
*
*  Set the magnifying value for the characters (should be 1 or 2)
*
*  @param[in]  Coeff The new magnifying coefficent.
*
**/
/******************************************************************************/
void DRAW_SetCharMagniCoeff( mag_t Coeff )
{
    CharMagniCoeff = Coeff;
}

/******************************************************************************
*
*                                DRAW_GetCharMagniCoeff
*
*******************************************************************************/
/**
*
*  Return the current magnifying value for the characters
*
*  @return  Current magnifying value.
*
**/
/******************************************************************************/
mag_t DRAW_GetCharMagniCoeff( void )
{
    return CharMagniCoeff;
}

/******************************************************************************
*
*                                DRAW_GetTextColor
*
*******************************************************************************/
/**
*
*  Return current text color.
*
*  @return  The current RGB color used to draw text.
*
**/
/******************************************************************************/
color_t DRAW_GetTextColor( void )
{
    return TextColor;
}

/*******************************************************************************
*
*                                DRAW_SetTextColor
*
*******************************************************************************/
/**
*
*  Set current text color.
*
*  @param[in]  Color The new RGB color used when drawing text.
*
**/
/******************************************************************************/
void DRAW_SetTextColor( color_t Color )
{
    TextColor = Color ;
}

/*******************************************************************************
*
*                                DRAW_GetBGndColor
*
*******************************************************************************/
/**
*
*  Return current background color.
*
*  @return  The current RGB color used for the background.
*
**/
/******************************************************************************/
color_t DRAW_GetBGndColor( void )
{
    return BGndColor;
}

/*******************************************************************************
*
*                                DRAW_SetBGndColor
*
*******************************************************************************/
/**
*
*  Set current background color
*
*  @param[in]  Color The new RGB color for background.
*
**/
/******************************************************************************/
void DRAW_SetBGndColor( color_t Color )
{
    BGndColor = Color;
}

/*******************************************************************************
*
*                                DRAW_Clear
*
*******************************************************************************/
/**
*
*  Clear the LCD display. Draw Battery and background image if main display.
*
**/
/******************************************************************************/
void DRAW_Clear( void )
{
//    enum POINTER_mode ptr_bak = POINTER_GetMode() ;

//    POINTER_SetMode( POINTER_OFF );
    if( (LCD_GetScreenOrientation()%2) == 1 )
        LCD_FillRect_Circle( 0, 0, Screen_Height, Screen_Width, BGndColor );
    else
        LCD_FillRect_Circle( 0, 0, Screen_Width, Screen_Height, BGndColor );

//    if (( CurrentMenu == 0 ) && ( CurrentCommand == 0 )
//            && ( BUTTON_GetMode() == BUTTON_ONOFF_FORMAIN ) )
    {
        DRAW_SetDefaultColor();
        DRAW_SetLogoBW();
//        LCD_Clear( xBat, yBat );

        OldBatState  = 10;
        OldTemp = -1;
        fDisplayTime = 1;
    }

//    POINTER_SetMode( ptr_bak );
}

/*******************************************************************************
*
*                                DRAW_SetLogoBW
*
*******************************************************************************/
/**
*
*  Draw the butterfly logo
*
**/
/******************************************************************************/
void DRAW_SetLogoBW( void )
{
    LCD_SetLogoBW();
}

/*******************************************************************************
*
*                                DRAW_SetImage
*
*******************************************************************************/
/**
*
*  The provided bitmap is made width * height 2 byte words. Each 2 byte word contains
*  the RGB color of a pixel.
*
*  @brief      Draw a color bitmap at the provided coordinates.
*  @param[in]  imageptr    A pointer to an array of width * height 2 byte words.
*  @param[in]  x           The horizontal coordinate of the low left corner of the bitmap.
*  @param[in]  y           The vertical coordinate of the low left corner of the bitmap.
*  @param[in]  width       The bitmap width.
*  @param[in]  height      The bitmap height.
*
*  @warning       The (0x0) point in on the low left corner.
*
**/
/******************************************************************************/
void DRAW_SetImage( const color_t* imageptr, coord_t x, coord_t y, coord_t width, coord_t height )
{
    DRAW_SetImageSel( imageptr, x, y, width, height, 0xFFFF, 0xFFFF );
}

/*******************************************************************************
*
*                                DRAW_SetImageSel
*
*******************************************************************************/
/**
*
*  The provided bitmap is made width * height 2 byte words. Each 2 byte word contains
*  the RGB color of a pixel.
*  All pixels with the oldBgndColor are replaced with the newBgndColor
*
*  @brief      Draw a color bitmap at the provided coordinates, with selected background color.
*  @param[in]  imageptr     A pointer to an array of width * height 2 byte words.
*  @param[in]  x            The horizontal coordinate of the low left corner of the bitmap.
*  @param[in]  y            The vertical coordinate of the low left corner of the bitmap.
*  @param[in]  width        The bitmap width.
*  @param[in]  height       The bitmap height.
*  @param[in]  oldBgndColor The color of the background to replace.
*  @param[in]  newBgndColor The new color.
*
*  @warning       The (0x0) point in on the low left corner.
*
**/
/******************************************************************************/
void DRAW_SetImageSel( const color_t* imageptr, coord_t x, coord_t y, coord_t width, coord_t height, color_t oldBgndColor, color_t newBgndColor )
{
    s32 j;
    coord_t i, k;
    color_t color;
    color_t* imagetemp;
    u8* color_table;
    Rotate_H12_V_Match_TypeDef OrientationSave;

    if ( imageptr )
    {
        j = width * height; // pixel counter
        //TODO ASSERT((u32)width * height < 0xFFFF);    // Ensure i can be a short

        // BMP header test
        if ( (imageptr[0] == 0x424D) && (imageptr[3] == 0) )
        {
            // Regular BMP file
            OrientationSave = LCD_GetScreenOrientation();
            imagetemp = (color_t*)(imageptr) + (imageptr[5]>>9) + ((imageptr[5]&0xff)<<7) ; // Offset of the image (header size)

            LCD_SetScreenOrientation(OrientationSave+4); // Change to V*BMP orientation
            LCD_SetRect_For_Cmd( x, y, width, height );
            LCD_SendLCDCmd_RAM_Access();

            switch( ((imageptr[14]>>8) + (imageptr[15]&0xff00)) ) // colour depth + compression methode
            {
                case 0x0010:    // 16 bits and no compression
                case 0x0310:
                    imageptr += 0x36;
                    for ( i = 0; i < j ; i++ )
                    {
                        if ( imageptr[i] != oldBgndColor )
                        {
                            LCD_SendLCDData( imageptr[i] & 0xff );
                            LCD_SendLCDData(( imageptr[i] >> 8 ) & 0xff );
                        }
                        else
                        {
                            LCD_SendLCDData( newBgndColor & 0xff );
                            LCD_SendLCDData(( newBgndColor >> 8 ) & 0xff );
                        }
                    }
                    break;
                
                case 0x0008:    // 8 bits (256 colors) and no compression
                    color_table = (u8*)imageptr + 0x36;
                    for ( i = 0; (2*i) < j ; i++ )
                    {
                        color = DRAW_GetIndexedColorBMP( color_table, 4*(imagetemp[i]>>8) );
                        if ( color == oldBgndColor )
                            color = newBgndColor;
                        LCD_SendLCDData( color & 0xff );
                        LCD_SendLCDData(( color >> 8 ) & 0xff );
                        color = DRAW_GetIndexedColorBMP( color_table, 4*(imagetemp[i]&0xff) );
                        LCD_SendLCDData( color & 0xff );
                        LCD_SendLCDData(( color >> 8 ) & 0xff );
                    }
                    break;
                
                case 0x0108:    // 8 bits (256 colors) and RLE compression
                    color_table = (u8*)imageptr + 0x36;
                    i = 0; // file cursor (in words)
                    while ( j > 0 ) // while there is still pixels to write (j : amount of remaining pixels)
                        {
                    
                        if ( (imagetemp[i]>>8) == 0 )
                            {
                            k = imagetemp[i] & 0xff;
                            i++; // move the file cursor

                            if ( k==0 ) // End of line
                                {
                                color = newBgndColor;
                                while ( (j%width)!=0 ) // fill with the background color to the end of line
                                    {
                                    j--; // remove each pixel from the pixel counter
                                    LCD_SendLCDData( color & 0xff );
                                    LCD_SendLCDData(( color >> 8 ) & 0xff );
                                    }
                                }

                            else if ( k==1) // End of file
                                break;
                            else
                                {
                                // If not either end of file or end of line : streak of single values
                                // in this case k is the length of the streak
                                j -= k ; // remove the streak length from the pixel counter
                                while ( k > 0)
                                    {
                                    color = DRAW_GetIndexedColorBMP( color_table , 4*(imagetemp[i]>>8) );
                                    LCD_SendLCDData( color & 0xff );
                                    LCD_SendLCDData(( color >> 8 ) & 0xff );
                                    k--;
                                    if ( k > 0 )
                                        {
                                        color = DRAW_GetIndexedColorBMP( color_table , 4*(imagetemp[i]&0xff) );
                                        if ( color == oldBgndColor ) // if the color should be replaced
                                            color = newBgndColor;
                                        LCD_SendLCDData( color & 0xff );
                                        LCD_SendLCDData(( color >> 8 ) & 0xff );
                                        k--;
                                        }
                                    i++; // move the file cursor
                                    }
                                }
                            } // End of : if ( (imagetemp[i]>>8) == 0 )
                        else
                            {
                            // Streak of same values
                            color = DRAW_GetIndexedColorBMP( color_table , 4*(imagetemp[i]&0xff) );
                            if ( color == oldBgndColor ) // if the color should be replaced
                                color = newBgndColor;
                            k = imagetemp[i]>>8; // length of the streak
                            j -= k; // remove the streak length from the pixel counter
                            for(; k>0 ; k--)
                                {
                                LCD_SendLCDData( color & 0xff );
                                LCD_SendLCDData(( color >> 8 ) & 0xff );
                                }
                            i++; // move the file cursor
                            }
                        } // End of : while ( j > 0 )
                    break;
                    
                }   // End of : switch( ((imageptr[14]>>8) + (imageptr[15]&0xff00)) )
        
            /* End of select screen area to access.*/
            LCD_SendLCDCmd_RAM_Access_End();
        
            LCD_SetScreenOrientation(OrientationSave);
        }
        else
        // Non regular BMP file (old Primer format)
        {
            /* Select screen area to access.*/
            LCD_SetRect_For_Cmd( x, y, width, height );
            LCD_SendLCDCmd_RAM_Access();

            for ( i = 0; i < j ; i++ )
            {
                if ( imageptr[i] != oldBgndColor )
                {
                    LCD_SendLCDData( imageptr[i] & 0xff );
                    LCD_SendLCDData(( imageptr[i] >> 8 ) & 0xff );
                }
                else
                {
                    LCD_SendLCDData( newBgndColor & 0xff );
                    LCD_SendLCDData(( newBgndColor >> 8 ) & 0xff );
                }
            }
            /* End of select screen area to access.*/
            LCD_SendLCDCmd_RAM_Access_End();
        }
    }
}

/*******************************************************************************
*
*                                DRAW_SetImageBW
*
*******************************************************************************/
/**
*
*  The provided bitmap is made of width * height bits where a set bit means a pixel
*  drawn in the current text color, whereas an unset bit means a pixel drawn in the
*  current background color.
*
*  @brief      Draw a monochrome bitmap at the provided coordinates.
*  @param[in]  imageptr    A pointer to an array of width * height bits.
*  @param[in]  x           The horizontal coordinate of the low left corner of the bitmap.
*  @param[in]  y           The vertical coordinate of the low left corner of the bitmap.
*  @param[in]  width       The bitmap width.
*  @param[in]  height      The bitmap height.
*
*  @warning       The (0x0) point in on the low left corner.
*
**/
/******************************************************************************/
void DRAW_SetImageBW( const u8* imageptr, coord_t x, coord_t y, coord_t width, coord_t height )
{
    coord_t i, j, k;

    k = ( width * height ) >> 3;

    /* Select screen area to access.*/
    LCD_SetRect_For_Cmd( x, y, width, height );

    /* Send command to write data on the LCD screen.*/
    LCD_SendLCDCmd_RAM_Access();

    /* Loop on all bitmap bytes.*/
    for ( i = 0; i < k; i++ )
    {
        /* Loop on all byte bits.*/
        for ( j = 0; j < 8; j++ )
        {
            /* Bit set: draw pixel with text color.*/
            if ( imageptr && ((( imageptr[i] ) >> ( 7 - j ) ) & 1 ) )
            {
                LCD_SendLCDData( TextColor & 0xff );
                LCD_SendLCDData(( TextColor >> 8 ) & 0xff );
            }
            else
            {
                /* Bit not set: draw pixel with background color.*/
                LCD_SendLCDData( BGndColor & 0xff );
                LCD_SendLCDData(( BGndColor >> 8 ) & 0xff );
            }
        }
    }

    /* End of select screen area to access.*/
    LCD_SendLCDCmd_RAM_Access_End();
}

/*******************************************************************************
*
*                                DRAW_DisplayVbat
*
*******************************************************************************/
/**
*
*  This function is used to display vbat in ascii
*
*  @param[in]  x  The horizontal coordinate of the displayed string.
*  @param[in]  y  The vertical coordinate of the display string.
*
*  @warning       The (0x0) point in on the low left corner.
*
**/
/******************************************************************************/
void DRAW_DisplayVbat( coord_t x, coord_t y )
{
    u8 text[17];
    u8* p = text;
    u16 vbat = UTIL_GetBat();

    *p++ = 'B';
    *p++ = 'A';
    *p++ = 'T';
    *p++ = '=';

    /* Translate vbat into p.*/
    vbattoa( p, vbat );

    /* Display text at the provided coordinates.*/
    DRAW_DisplayString( x, y, ( u8* )text, 10 );
}

/*******************************************************************************
*
*                                DRAW_DisplayTime
*
*******************************************************************************/
/**
*
*  This function is used to display time in ascii on LCD
*
*  @param[in]  x  The horizontal coordinate of the displayed string.
*  @param[in]  y  The vertical coordinate of the display string.
*
*  @warning       The (0x0) point in on the low left corner.
*
**/
/******************************************************************************/
void DRAW_DisplayTime( coord_t x, coord_t y )
{
    u8 THH;
    u8 TMM;
    u8 TSS;
    static u8 OldTHH = 100;
    static u8 OldTMM = 100;
    static u8 OldTSS = 100;

    static u8 TimeTextBuffer[9];       // Trailing '\0' is NOT included

    /* Get Time*/
    RTC_GetTime( &THH, &TMM, &TSS );

    /* Update time display if seconds have changed */
    if ( THH != OldTHH )
    {
        OldTHH = THH;
        UTIL_uint2str( &TimeTextBuffer[0], OldTHH, 2, 1 );
        TimeTextBuffer[2] = ':';
    }
    if ( TMM != OldTMM )
    {
        OldTMM = TMM;
        UTIL_uint2str( &TimeTextBuffer[3], OldTMM, 2, 1 );
        TimeTextBuffer[5] = ':';
    }
    if ( TSS != OldTSS )
    {
        OldTSS = TSS;
        UTIL_uint2str( &TimeTextBuffer[6], OldTSS, 2, 1 );
        TimeTextBuffer[8] = '\0';
        DRAW_DisplayString( x , y, TimeTextBuffer, 8 );
    }
}

/*******************************************************************************
*
*                                DRAW_DisplayStringWithMode
*
*******************************************************************************/
/**
*
*  This function is used to display a character string
*  at given X, Y coordinates on the LCD display.
*  Note:
*  This function is the user interface to use the LCD driver.
*
*  @param[in] x      The horizontal coordinate of the string.
*  @param[in] y      The vertical coordinate of the string.
*  @param[in] *ptr   Pointer to the string.
*  @param[in] len    String length i.e., number of characters to display:
*                     if 0 = len(string), if ALL_SCREEN = width screen.
*  @param[in] mode   Display mode: 0 normal, 1 inverted colors.
*  @param[in] align  Alignment mode: 0 normal, 1 centered.
*
*  @note          The string length is automatic is set to 0.
*  @warning       The (0x0) point in on the low left corner.
*  @warning       The input string must be null-terminated.
*
*  @see           DRAW_DisplayString
*  @see           DRAW_DisplayStringInverted
*
**/
/******************************************************************************/
void DRAW_DisplayStringWithMode( coord_t x, coord_t y, const u8* ptr, len_t len, bool mode, enumset_t align )
{
    coord_t ref_x;
    coord_t lx;
    len_t i;
    len_t lgc;
    u8 CharWidth;
    u16 Display_Width;

    // Swap the max coordinates according to the orientation
    if( (LCD_GetScreenOrientation()%2) == 1 )
        {
        Display_Width = Screen_Height;
        }
    else
        {
        Display_Width = Screen_Width;
        }
        
    // Check the length validity
    if ( len == 0 )
        len = my_strlen( ptr );
    if ( len == ALL_SCREEN )
        len = Display_Width / (Char_Width * CharMagniCoeff );
    
    // Check and limit the length of the string to display
    CharWidth = Char_Width * CharMagniCoeff;
    lx = x + ( len * CharWidth );
    if ( lx > Display_Width )
    {
        if ((( lx - Display_Width ) % CharWidth ) == 0 )
            len -= (( lx - Display_Width ) / CharWidth );
        else
            len -= ((( lx - Display_Width ) / CharWidth ) + 1 );
    }

    // Clear the area
    if ( (Transparency == 0) || (CharMagniCoeff>3) )
        DRAW_Clear_Text( x, y, len, mode );

    // Locate the string into the area
    ref_x = x;
    lgc = my_strlen( ptr );
    if ( lgc > len )
        lgc = len;

    switch ( align )
    {
    case CENTER:
        ref_x += (( len - lgc ) * CharWidth ) / 2;
        break;
    case RIGHT:
        ref_x += ( len - lgc ) * CharWidth;
        break;
    case LEFT:
    default:
        break;
    }

    /* Display each character on LCD */
    for ( i = 0 ; i < lgc; i++ )
    {
        /* Display one character on LCD */
        LCD_DisplayChar( ref_x, y, *ptr++, mode ? BGndColor : TextColor,  mode ? TextColor : BGndColor, CharMagniCoeff );

        /* Increment the column position by CharWidth pixels */
        ref_x += CharWidth;
    }
}

/*******************************************************************************
*
*                                DRAW_DisplayString
*
*******************************************************************************/
/**
*
*  This function is used to display a character string (ALL_SCREEN u8 max)
*  at given X, Y coordinates on the LCD display.
*
*  @param[in] x      The horizontal coordinate of the string.
*  @param[in] y      The vertical coordinate of the string.
*  @param[in] *ptr   Pointer to the string.
*  @param[in] len    String length i.e., number of characters to display.
*
*  @note          Characters are displayed as 7x14 pixels blocks.
*  @warning       The (0x0) point in on the low left corner.
*  @warning       The input string must be null-terminated.
*
**/
/******************************************************************************/
void DRAW_DisplayString( coord_t x, coord_t y, const u8* ptr, len_t len )
{
    DRAW_DisplayStringWithMode( x, y, ptr, len, NORMAL_TEXT, LEFT );
}

/*******************************************************************************
*
*                                DRAW_DisplayStringInverted
*
*******************************************************************************/
/**
*
*  This function is used to display a character string (ALL_SCREEN char max)
*  at given X, Y coordinates on the LCD display, with inverted colors.
*
*  @param[in] x      The horizontal coordinate of the string.
*  @param[in] y      The vertical coordinate of the string.
*  @param[in] *ptr   Pointer to the string.
*  @param[in] len    String length i.e., number of characters to display.
*
*  @note          Characters are displayed as 7x14 pixels blocks.
*  @warning       The (0x0) point in on the low left corner.
*  @warning       The input string must be null-terminated.
*
**/
/******************************************************************************/
void DRAW_DisplayStringInverted( coord_t x, coord_t y, const u8* ptr, len_t len )
{
    //BackGround and Text Colors are inverted
    DRAW_DisplayStringWithMode( x, y, ptr, len, INVERTED_TEXT, LEFT );
}

/*******************************************************************************
*
*                                DRAW_SetDefaultColor
*
*******************************************************************************/
/**
*
*  Reset text and background colors to their default values.
*
**/
/******************************************************************************/
void DRAW_SetDefaultColor( void )
{
    BGndColor = RGB_WHITE;
    TextColor = RGB_BLACK;
}


/*******************************************************************************
*
*                                DRAW_Line
*
*******************************************************************************/
/**
*  Draw a line on the LCD screen. Optimized for horizontal/vertical lines,
*  and use the Bresenham algorithm for other cases.
*
*  @param[in]  x1          The x-coordinate of the first line endpoint.
*  @param[in]  x2          The x-coordinate of the second line endpoint.
*  @param[in]  y1          The y-coordinate of the first line endpoint.
*  @param[in]  y2          The y-coordinate of the second line endpoint.
*  @param[in]  color       The line color.
*
*  @note                   This function is just a wrapper with limit checking
*
**/
/******************************************************************************/
void DRAW_Line( coord_t x1, coord_t y1, coord_t x2, coord_t y2, color_t color )
{
    /* Check the x parameters*/
    if ( x1 < 0 )
        x1 = 0;
    if ( x2 < 0 )
        x2 = 0;

    if ( x1 > Screen_Width - 1 )
        x1 = Screen_Width - 1;
    if ( x2 > Screen_Width - 1 )
        x2 = Screen_Width - 1;

    /* Check the y parameters*/
    if ( y1 < 0 )
        y1 = 0;
    if ( y2 < 0 )
        y2 = 0;

    if ( y1 > Screen_Height - 1 )
        y1 = Screen_Height - 1;
    if ( y2 > Screen_Height - 1 )
        y2 = Screen_Height - 1;

    /* Call the native function*/
    DRAW_Line_Circle( x1, y1, x2, y2, color );
}

/*******************************************************************************
*
*                                DRAW_Putc
*
*******************************************************************************/
/**
*
*  Display at current coordinates the provided ASCII character with the current
*  text and background colors and with the current magnify coefficient.
*
*  @param[in]  Ascii          The ASCII code of the character to display.
*                             @n Ascii must be higher than 31 and lower than 255.
*                             '\n' managed as CRLF.
*
*  @note          If the low Y margin is reached, the text is scrolled into
*                 the window defined by the X and Y margins
*  @see           DRAW_SetCursorMargin

**/
/******************************************************************************/
void DRAW_Putc( u8 Ascii )
{
    u16 Char_Y = Char_Height * CharMagniCoeff;

    /* CRLF ? */
    if (( Ascii == '\n' ) )
    {
        PosCurX = LeftMarginX;
        if ( PosCurY >= LowMarginY )
        {
            PosCurY -= Char_Y;
        }
        return;
    }

    /* Line change, if end of line reached*/
    if (( PosCurX + ( Char_Width * CharMagniCoeff ) ) > RightMarginX )
    {
        PosCurX = LeftMarginX;
        PosCurY -= Char_Y;
    }

    /* Normal print if end of screen not reached*/
    if ( PosCurY >= LowMarginY )
    {
        /* Display the selected bitmap according to the provided ASCII character.*/
        LCD_DisplayChar( PosCurX, PosCurY, Ascii, TextColor, BGndColor, CharMagniCoeff );
    }
    else
    {
        LCD_Scroll( Ascii, PosCurX, &PosCurY, RightMarginX, LeftMarginX, HighMarginY, LowMarginY, BGndColor, TextColor, CharMagniCoeff );
    }

    PosCurX += ( Char_Width * CharMagniCoeff );
}

/*******************************************************************************
*
*                                DRAW_Puts
*
*******************************************************************************/
/**
*
*  This function is used to display a character string
*  at current coordinates on the LCD display.
*  Note:
*  This function is the user interface to use the LCD driver.
*
*  @param[in] *ptr   Pointer to the string.
*
*  @note          Characters are displayed as 7x14 pixels blocks.
*  @warning       The (0x0) point in on the low left corner.
*
*  @see           DRAW_DisplayString
*  @see           DRAW_DisplayStringInverted
*
**/
/******************************************************************************/
void DRAW_Puts( const u8* ptr )
{
    /* Display each character on LCD */
    while ( *ptr != 0 )
    {
        /* Display the character on LCD */
        DRAW_Putc( *ptr );

        /* Point to the next character */
        ptr++;
    }
}

/*******************************************************************************
*
*                                DRAW_SetCursorPos
*
*******************************************************************************/
/**
*
*  This function is used to set the current position of the cursor.
*  This position is used by the DRAW_putc and DRAW_Puts functions.
*
*  @param[in] x   X new position in pixels.
*  @param[in] y   Y new position in pixels.
*
*  @warning       The (0x0) point in on the low left corner.
*
**/
/******************************************************************************/
void DRAW_SetCursorPos( coord_t x, coord_t y )
{
    if ( x < LeftMarginX )
        x = LeftMarginX;
    else if ( x > RightMarginX )
        x = RightMarginX;
    PosCurX = x;

    if ( y < LowMarginY )
        y = LowMarginY;
    else if ( y > HighMarginY - ( Char_Height * CharMagniCoeff ) )
        y = HighMarginY - ( Char_Height * CharMagniCoeff );
    PosCurY = y;
}

/*******************************************************************************
*
*                                DRAW_GetCursorPos
*
*******************************************************************************/
/**
*
*  This function is used to get the current position of the cursor.
*
*  @return  The current cursor position in pixels with X in the LSB and Y in the MSB.
*
*  @warning       The (0x0) point in on the low left corner.
*
**/
/******************************************************************************/
ret_t DRAW_GetCursorPos( void )
{
    return ( PosCurX | ( PosCurY << 8 ) );
}

/*******************************************************************************
*
*                                DRAW_SetCursorMargin
*
*******************************************************************************/
/**
*
*  This function is used to set the current margins for the cursor.
*  This position is used by the DRAW_putc and DRAW_Puts functions.
*
*  @param[in] lx   X new left margin in pixels.
*  @param[in] rx   X new right margin in pixels.
*  @param[in] hy   Y new high margin in pixels.
*  @param[in] ly   Y new low margin in pixels.
*
*  @warning       The (0x0) point in on the low left corner.
*
**/
/******************************************************************************/
void DRAW_SetCursorMargin( coord_t lx, coord_t rx, coord_t hy, coord_t ly )
{
    if ( lx < 0 )
        lx = 0;

    if ( rx > Screen_Width )
        rx = Screen_Width;
    else if ( lx > rx )      // else-if necessary in case lx==rx==Screen_Width
        lx = rx - 1;

    LeftMarginX = lx;
    RightMarginX = rx;

    if ( ly < 0 )
        ly = 0;

    if ( hy > Screen_Height )
        hy = Screen_Height;
    else if ( ly > hy )
        ly = hy - 1;

    HighMarginY = hy;
    LowMarginY = ly;
}

/*******************************************************************************
*
*                                DRAW_GetCursorMargin
*
*******************************************************************************/
/**
*
*  This function is used to get the current margins for the cursor.
*  This position is used by the DRAW_putc and DRAW_Puts functions.
*
*  @param[out] *lx   X current left margin in pixels.
*  @param[out] *rx   X current right margin in pixels.
*  @param[out] *hy   Y current high margin in pixels.
*  @param[out] *ly   Y current low margin in pixels.
*
*  @warning       The (0x0) point in on the low left corner.
*
**/
/******************************************************************************/
void DRAW_GetCursorMargin( coord_t* lx, coord_t* rx, coord_t* hy, coord_t* ly )
{
    if ( lx )
        *lx = LeftMarginX;

    if ( rx )
        *rx = RightMarginX;

    if ( hy )
        *hy = HighMarginY;

    if ( ly )
        *ly = LowMarginY;
}

/*******************************************************************************
*
*                                DRAW_Circle
*
/**
/*******************************************************************************
* Function Name  :   DRAW_Circle()
* Description    :   Implementation of Bresenham's Circle variant algorithm
*
* @param[in]     :   CENTER_x    Circle Center x position
* @param[in]     :   CENTER_y    Circle Center y position
* @param[in]     :   RADIUS_r    Circle Radius
* @param[in]     :   Color       Line color of the Circle
* @param[in]     :   Fill_Color  Background of the Circle
* @param[in]     :   Fill        Defines if the circle is filled
* @param[in]     :   Circle      Defines if the circle is drawn
* @return        :   None
*******************************************************************************/
void DRAW_Circle(coord_t CENTER_x, coord_t CENTER_y, coord_t RADIUS_r, color_t Color, color_t Fill_Color, bool Fill, bool Circle)
    {
    int ERROR = 1 - RADIUS_r;
    int ddF_x = 0;
    int ddF_y = -2 * RADIUS_r;
    int x = 0;
    int y = RADIUS_r;
  
    /* Check the x parameters*/
    if ( CENTER_x - RADIUS_r < 0 )
        CENTER_x = RADIUS_r;

    if ( CENTER_x + RADIUS_r > Screen_Width - 1 )
        CENTER_x = Screen_Width - RADIUS_r - 1;

    /* Check the y parameters*/
    if ( CENTER_y - RADIUS_r < 0 )
        CENTER_y = RADIUS_r;

    if ( CENTER_y + RADIUS_r > Screen_Height - 1 )
        CENTER_y = Screen_Height - RADIUS_r - 1;
    
    if (Fill)
        {
        if (Circle)
            {
            Fill = 0;
            DRAW_Circle(CENTER_x, CENTER_y, RADIUS_r,Color,Fill_Color, 1, 0 );
            }
        }
    
    if (Fill)
        {  
        DRAW_Line(CENTER_x + RADIUS_r, CENTER_y,CENTER_x - RADIUS_r,CENTER_y, Fill_Color );
        DRAW_Line(CENTER_x , CENTER_y - RADIUS_r,CENTER_x,CENTER_y+ RADIUS_r,Fill_Color);
        }
    
    if (Circle)
        {
        LCD_DrawPixel(CENTER_x, CENTER_y + RADIUS_r,Color);
        LCD_DrawPixel(CENTER_x, CENTER_y - RADIUS_r,Color);
        LCD_DrawPixel(CENTER_x + RADIUS_r, CENTER_y,Color);
        LCD_DrawPixel(CENTER_x - RADIUS_r, CENTER_y,Color);
        }
    
    while(x < y) 
        {
        if (ERROR >= 0) 
            {
            y--;
            ddF_y += 2;
            ERROR += ddF_y;
            }
        x++;
        ddF_x += 2;
        ERROR += ddF_x + 1; 
        if (Fill)
            {
            DRAW_Line(CENTER_x + x, CENTER_y + y, CENTER_x + x, CENTER_y - y, Fill_Color );
            DRAW_Line(CENTER_x - x, CENTER_y + y, CENTER_x - x, CENTER_y - y, Fill_Color );
            DRAW_Line(CENTER_x + y, CENTER_y + x, CENTER_x + y, CENTER_y - x, Fill_Color );
            DRAW_Line(CENTER_x - y, CENTER_y + x, CENTER_x - y, CENTER_y - x, Fill_Color );
            }
        
        if (Circle)
            {
            LCD_DrawPixel(CENTER_x + x, CENTER_y + y,Color);
            LCD_DrawPixel(CENTER_x - x, CENTER_y + y,Color);
            LCD_DrawPixel(CENTER_x + x, CENTER_y - y,Color);
            LCD_DrawPixel(CENTER_x - x, CENTER_y - y,Color);
            LCD_DrawPixel(CENTER_x + y, CENTER_y + x,Color);
            LCD_DrawPixel(CENTER_x - y, CENTER_y + x,Color);
            LCD_DrawPixel(CENTER_x + y, CENTER_y - x,Color);
            LCD_DrawPixel(CENTER_x - y, CENTER_y - x,Color);
            }
        } 
    }

/*******************************************************************************
*
*                                DRAW_Ellipse
*
/**
/*******************************************************************************
* Function Name  :   DRAW_Ellipse()
* Description    :   This function draw an Ellipse
*
* @param[in]     :   CENTER_x   Ellipse Center x position
* @param[in]     :   CENTER_y   Ellipse Center y position
* @param[in]     :   RADIUS_a   Ellipse Radius a
* @param[in]     :   RADIUS_b   Ellipse Radius b
* @param[in]     :   Color      Line color of the Circle
* @param[in]     :   Fill_Color Background of the Circle
* @param[in]     :   Fill       Defines if the Ellipse is filled
* @param[in]     :   Ellipse    Defines if the Ellipse is drawn
* @return        :   None
*******************************************************************************/
void DRAW_Ellipse( coord_t CENTER_x, coord_t CENTER_y, coord_t RADIUS_a, coord_t RADIUS_b, color_t Color, color_t Fill_Color, bool Fill, bool Ellipse)
{
    int X, Y;
    int XChange, YChange;
    int EllipseError;
    int TwoASquare, TwoBSquare;
    int StoppingX, StoppingY;
    TwoASquare = 2*RADIUS_a*RADIUS_a;
    TwoBSquare = 2*RADIUS_b*RADIUS_b;
    X = RADIUS_a;
    Y = 0;
    XChange = RADIUS_b*RADIUS_b*(1-2*RADIUS_a);
    YChange = RADIUS_a*RADIUS_a;
    EllipseError = 0;
    StoppingX = TwoBSquare*RADIUS_a;
    StoppingY = 0;
 
    /* Check the x parameters*/
    if ( CENTER_x - RADIUS_a < 0 )
        CENTER_x = RADIUS_a;

    if ( CENTER_x + RADIUS_a > Screen_Width - 1 )
        CENTER_x = Screen_Width - RADIUS_a - 1;

    /* Check the y parameters*/
    if ( CENTER_y - RADIUS_b < 0 )
        CENTER_y = RADIUS_b;

    if ( CENTER_y + RADIUS_b > Screen_Height - 1 )
        CENTER_y = Screen_Height - RADIUS_b - 1;
    
    if (Fill)
        {
        if (Ellipse)
            {
            Fill=0;
            DRAW_Ellipse( CENTER_x, CENTER_y, RADIUS_a, RADIUS_b, Color, Fill_Color, 1,0);
            }
        }
        
    while ( StoppingX >= StoppingY )
        {
        if (Fill)
            {
            DRAW_Line(CENTER_x+ X, CENTER_y+Y, CENTER_x-X , CENTER_y+Y, Fill_Color);
            DRAW_Line(CENTER_x- X, CENTER_y-Y, CENTER_x+X , CENTER_y-Y,Fill_Color);
            }
        
        if (Ellipse)
            {
            LCD_DrawPixel(CENTER_x+X, CENTER_y+Y,Color); 
            LCD_DrawPixel(CENTER_x-X, CENTER_y+Y,Color); 
            LCD_DrawPixel(CENTER_x-X, CENTER_y-Y,Color); 
            LCD_DrawPixel(CENTER_x+X, CENTER_y-Y,Color); 
            }
        Y++;
        StoppingY+= TwoASquare;
        EllipseError+= YChange;
        YChange+= TwoASquare;
        if ((2*EllipseError + XChange) > 0 )
            {
            X--;
            StoppingX -= TwoBSquare;
            EllipseError += XChange;
            XChange += TwoBSquare;
            }
        
        } // While (StoppingX >= StoppingY)
    
    X = 0;
    Y = RADIUS_b;
    XChange = RADIUS_b*RADIUS_b;
    YChange = RADIUS_a*RADIUS_a*(1-2*RADIUS_b);
    EllipseError = 0;
    StoppingX = 0;
    StoppingY = TwoASquare*RADIUS_b;
    
    while ( StoppingX <= StoppingY )
        {
        if(Fill)
            {
            DRAW_Line(CENTER_x+ X, CENTER_y+Y, CENTER_x-X , CENTER_y+Y, Fill_Color);
            DRAW_Line(CENTER_x- X, CENTER_y-Y, CENTER_x+X , CENTER_y-Y, Fill_Color);
            }
        
        if (Ellipse)
            {
            LCD_DrawPixel(CENTER_x+X, CENTER_y+Y,Color); 
            LCD_DrawPixel(CENTER_x-X, CENTER_y+Y,Color); 
            LCD_DrawPixel(CENTER_x-X, CENTER_y-Y,Color); 
            LCD_DrawPixel(CENTER_x+X, CENTER_y-Y,Color); 
            }
        
        X++;
        StoppingX += TwoBSquare;
        EllipseError += XChange;
        XChange += TwoBSquare;
        
        if ((2*EllipseError + YChange) > 0 )
            {
            Y--;
            StoppingY -= TwoASquare;
            EllipseError += YChange;
            YChange += TwoASquare;
            }
        
        } // While (StoppingX <= StoppingY)
    }
  

/*******************************************************************************
*
*                                DRAW_Polygon
*
/**
/*******************************************************************************
* Function Name  :   DRAW_Polygon()
* Description    :   Function to draw a polygon with the given points
*
* @param[in]     :   *points     Pointer to the points of the polygon 
* @param[in]     :   NUM_Points  Number of pointscolor of the line
* @param[in]     :   Line_Color  Color of the line.
* @return        :   None
*******************************************************************************/
void DRAW_Polygon(coord_t *points, u16 NUM_Points, color_t Line_Color)
    {
    int i=1;

    if (points)
        {
        for(i=1; i < NUM_Points >>1 ;i++)
            {
            DRAW_Line(points[(i-1)*2], points[1+(i-1)*2], points[(i)*2], points[1+(i)*2], Line_Color);
            }
        }
    }
     

