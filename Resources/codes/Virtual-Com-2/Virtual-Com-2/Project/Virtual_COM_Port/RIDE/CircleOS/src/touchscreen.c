/****************** COPYRIGHT (C) 2007-2010 RAISONANCE S.A.S. *****************/
/**
*
* @file     touchscreen.c
* @brief    Common touchscreen routines
* @author   YRT
* @date     10/2008
* @version  4.0 Add Open4 Primer
* @date     10/2009
*
**/
/******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "circle.h"

/// @cond Internal

/* Private defines -----------------------------------------------------------*/
#define TS_TOLERANCE 40

/* Private variables ---------------------------------------------------------*/
u16 TS_Sensibility = 3100;  // Sensibility of the touch

// Position informations
tTOUCHSCR_Info TOUCHSCR_Info = {0};
tPOINT TS_ScreenSample[3] = {0};

// Calibration informations
tTOUCHSCR_Cal TS_CalibrationSetup =
{
    10960,    0, - 821752,  // Default setup :
    147, 7946, -1088093,    // good for most Primers
    100000                  // out of the box
};

/* External variables ---------------------------------------------------------*/
extern const tPOINT TS_RefSample[3];

/* Private functions ----------------------------------------------------------*/

/*******************************************************************************
*
*                                MedianFilter
*
*******************************************************************************/
/**
*
*  Filter algorithm.
*  Sorts the acquisition values lowest to highest,
*  then keep the median value, in order to reject voltage spikes
*  and keeps values steady.
*
*  @param[in]  values : array of acquisition measures by DMA.
*
*
**/
/******************************************************************************/
NODEBUG2 u32 MedianFilter( const u16* values )
{
    u16 Sorted[ADC_NB_SAMPLES]; //We only need so sort half of array
    u16 v;
    u32 i, j;

    Sorted[0] = values[0];

    for ( i = 1; i < ADC_NB_SAMPLES; i++ )
    {
        v = values[i*ADC_NB_CHANNELS];
        j = i;
        for ( ; j > 0; j-- )
        {
            if ( v > Sorted[j-1] )
                break;
            Sorted[j] = Sorted[j-1];
        }
        Sorted[j] = v;
    }

    return Sorted[( ADC_NB_SAMPLES+1 ) >> 1];
}

/*******************************************************************************
*
*                                TOUCHSCR_SetMode
*
*******************************************************************************/
/**
*
*  Change the mode of the touchscreen.
*
*  @param[in]  TS_NORMAL, TS_DRAWING  or TS_CALIBRATION, if pending.
*
*
**/
/******************************************************************************/
NODEBUG2 void TOUCHSCR_SetMode( TOUCHSCR_Mode_enum  mode )
{
    switch ( mode )
    {
    case TS_POINTER:
    case TS_NORMAL:
    case TS_DRAWING:
        TOUCHSCR_Info.Mode = mode;
        break;
    case TS_CALIBRATION:
        break;   // set this mode only in special cases..
    }
}

/*******************************************************************************
*
*                                TOUCHSCR Drawing
*
*******************************************************************************/
/**
*
*  This function provides a mini "scribble" functionality.
*
**/
/******************************************************************************/
NODEBUG2 void TOUCHSCREEN_Drawing( void )
{
    static s32 old_x = 0;
    static s32 old_y = 0;
    static s32 old_Pressed = 0;
    s32 xpos = 0, ypos = 0;

    // Get the coordinates of the touch point
    if ( TOUCHSCR_Info.TouchPress )
    {
        xpos = TOUCHSCR_Info.xPos;
        ypos = TOUCHSCR_Info.yPos;
        if ( xpos < 0 ) xpos = 0;
        if ( ypos < 0 ) ypos = 0;
        if ( xpos > ( Screen_Width - 1 ) ) xpos = ( Screen_Width - 1 );
        if ( ypos > ( Screen_Height - 1 ) ) ypos = ( Screen_Height - 1 );

        // To eliminate  the interference
        if ( old_Pressed )
        {
            if ((( xpos - old_x )*( xpos - old_x ) + ( ypos - old_y ) *( ypos - old_y ) )  > ( TS_TOLERANCE * TS_TOLERANCE ) )
            {
                old_Pressed = 0;
            }
            else
            {
                xpos = ( xpos + old_x ) / 2; // integrate the position to
                ypos = ( ypos + old_y ) / 2;
            }
        }

        // Draw line between the n and n-1 points
        if ( old_Pressed )
        {
            DRAW_Line_Circle( old_x, old_y, xpos, ypos, RGB_BLACK );
        }
    }
    old_x = xpos;
    old_y = ypos;
    old_Pressed = TOUCHSCR_Info.TouchPress;
}

/*******************************************************************************
*
*                                CalculateCalibration
*
*******************************************************************************/
/**
*
*                       TOUCHSCR calibration calculation
*
*    Note : this routine is inpired fom the sample program written and put
*           in the public domain by Carlos E. Vidales.
*           It has been simplified by NTRF
*
*  @param[in]  : three real points Raw[]
*
*  Use the three acquired points Raw[] and thethree reference points TS_RefSample[]
*  to calculate and update the calibration matrix coefficients TS_CalibrationSetup[].
*
**/
/******************************************************************************/
void TOUCHSCR_CalculateCalibration( tPOINT* Raw )
{
    s32 Dx = Raw[0].X - Raw[2].X;
    s32 Dy = Raw[0].Y - Raw[2].Y;

    s32 Ex = Raw[1].X - Raw[2].X;
    s32 Ey = Raw[1].Y - Raw[2].Y;

    s32 Delta = Dx * Ey - Ex * Dy;
    TS_CalibrationSetup.R = Delta;

    // Three for X...
    s32 A = ( TS_RefSample[0].X - TS_RefSample[2].X ) * Ey
            - ( TS_RefSample[1].X - TS_RefSample[2].X ) * Dy;

    s32 B = ( TS_RefSample[1].X - TS_RefSample[2].X ) * Dx
            - ( TS_RefSample[0].X - TS_RefSample[2].X ) * Ex;

    // Much faster then direct computation (without accuracy loss)
    s32 C = ( TS_RefSample[2].X * Delta ) - ( A * Raw[2].X ) - ( B * Raw[2].Y );

    TS_CalibrationSetup.A = A;
    TS_CalibrationSetup.B = B;
    TS_CalibrationSetup.C = C;

    //...And three for Y
    s32 D  = ( TS_RefSample[0].Y - TS_RefSample[2].Y ) * Ey
             - ( TS_RefSample[1].Y - TS_RefSample[2].Y ) * Dy;

    s32 E  = ( TS_RefSample[1].Y - TS_RefSample[2].Y ) * Dx
             - ( TS_RefSample[0].Y - TS_RefSample[2].Y ) * Ex;

    s32 F  = ( TS_RefSample[2].Y * Delta ) - ( D * Raw[2].X ) - ( E * Raw[2].Y );

    TS_CalibrationSetup.D = D;
    TS_CalibrationSetup.E = E;
    TS_CalibrationSetup.F = F;
}

/// @endcond

/*******************************************************************************
*
*                                TOUCHSCR_GetPos
*
*******************************************************************************/
/**
*
*  Return the current position of the point touched.
*
*  @return  The current screen position with X in the LSB and Y in the MSB.
*
*  @warning       The (0x0) point in on the low left corner.
**/
/******************************************************************************/
u16 TOUCHSCR_GetPos( void )
{
    return ( TOUCHSCR_Info.xPos | ( TOUCHSCR_Info.yPos << 8 ) );
}

/*******************************************************************************
*
*                                TOUCHSCR_GetPosX
*
*******************************************************************************/
/**
*
*  Return the current horizontal position of the point touched.
*
*  @return  The current horizontal position of the point touched.
*
*  @warning       The (0x0) point in on the low left corner.
**/
/******************************************************************************/
s32 TOUCHSCR_GetPosX( void )
{
    return TOUCHSCR_Info.xPos;
}

/*******************************************************************************
*
*                                TOUCHSCR_GetPosY
*
*******************************************************************************/
/**
*
*  Return the current vertical position of the point touched.
*
*  @return  The current vertical position of the point touched.
*
*  @warning       The (0x0) point in on the low left corner.
**/
/******************************************************************************/
s32 TOUCHSCR_GetPosY( void )
{
    return TOUCHSCR_Info.yPos;
}

/*******************************************************************************
*
*                                TOUCHSCR_GetAbsPos
*
*******************************************************************************/
/**
*
*  Return the current absolute position of the point touched.
*  This position is independant with the orientation of the screen.
*
*  @return  The current screen position with X in the LSB and Y in the MSB.
*
*  @warning       The (0x0) point in on the low left corner.
**/
/******************************************************************************/
u16 TOUCHSCR_GetAbsPos( void )
{
    return ( TOUCHSCR_Info.xAbsPos | ( TOUCHSCR_Info.yAbsPos << 8 ) );
}

/*******************************************************************************
*
*                                TOUCHSCR_IsPressed
*
*******************************************************************************/
/**
*
*  Return info if the screen has been touched or not.
*
*  @return  true is the screen has been touched.
*
*
**/
/******************************************************************************/
bool TOUCHSCR_IsPressed( void )
{
    return ( TOUCHSCR_Info.TouchPress ) ;
}

/*******************************************************************************
*
*                                TOUCHSCR_GetMode
*
*******************************************************************************/
/**
*
*  Return info if the touchscreen is in calibration or not.
*
*  @return  TS_NORMAL, TS_DRAWING  or TS_CALIBRATION, if pending.
*
*
**/
/******************************************************************************/
TOUCHSCR_Mode_enum TOUCHSCR_GetMode( void )
{
    return ( TOUCHSCR_Info.Mode ) ;
}

/*******************************************************************************
*
*                                TOUCHSCR_SetSensibility
*
*******************************************************************************/
/**
*
*  Set the sensibility of the touch detection.
*
*  @param[in] sens 0 to 4095 (3000 by default).
*
*
**/
/******************************************************************************/
void TOUCHSCR_SetSensibility( u16 sens )
{
    if ( sens < 4095 )
        TS_Sensibility = sens;
    else
        TS_Sensibility = 4095;
}


