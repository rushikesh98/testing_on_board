/****************** COPYRIGHT (C) 2007-2010 RAISONANCE S.A.S. *****************/
/**
*
* @file     pointer.c
* @brief    Various utilities for the pointer management.
* @author   FL
* @date     07/2007
* @version  1.1
* @date     10/2007
* @version  1.5 various corrections reported by Ron Miller to suppress jittery
*
* @date     10/2008
* @version  3.0 Add joystick use for Primer 2
* @date     10/2009
* @version  4.0 Add Open4 Primer
*
**/
/******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "circle.h"

/// @cond Internal

/* Private define ------------------------------------------------------------*/
#define POS_MIN                     0
#define POS_MAX                     (Screen_Width - POINTER_WIDTH - 1)
#define POINTER_DIVIDER             50
#define POINTER_DEFAULT_COLOR       RGB_BLUE

// defines for pointer move
#define ANGLEPAUSE                  500
#define DEFAULT_ANGLESTART          25
#define MIN_ANGLE_FOR_SHIFT_UP      (ANGLEPAUSE+CurrentAngleStart)
#define MIN_ANGLE_FOR_SHIFT_DOWN    (ANGLEPAUSE-CurrentAngleStart)
#define MIN_ANGLE_FOR_SHIFT_RIGHT   (signed)(0+CurrentAngleStart)
#define MIN_ANGLE_FOR_SHIFT_LEFT    (signed)(0-CurrentAngleStart)
#define DEFAULT_SPEED_ON_ANGLE      60

// define for joystick management
#define JOY_MAXBTIME    20
#define JOY_HIGH_SPEED  5
#define JOY_LOW_SPEED   1

/* Private variables ---------------------------------------------------------*/
CONST_DATA          u8 BallPointerBmp[POINTER_WIDTH] = { 0x38, 0x7C, 0xFF, 0xFF, 0xFF, 0x7C, 0x38 } ;

tAppPtrMgr Application_Pointer_Mgr = 0;

//u8                  locbuf[POINTER_WIDTH];
u8                  DefaultAreaStore[2 * POINTER_WIDTH* POINTER_WIDTH];

// Variables for pointer.
const u8*           CurrentPointerBmp                = 0;
coord_t             CurrentPointerWidth              = 0;
coord_t             CurrentPointerHeight             = 3;
s16                 CurrentSpeedOnAngle              = DEFAULT_SPEED_ON_ANGLE;
s32                 CurrentAngleStart                = DEFAULT_ANGLESTART;
u8*                 ptrAreaStore                     = DefaultAreaStore;
color_t             CurrentPointerColor              = POINTER_DEFAULT_COLOR;
enum POINTER_mode   Pointer_Mode                     = POINTER_UNDEF;
enum POINTER_state  Pointer_State                    = POINTER_S_UNDEF;

coord_t             OUT_X;
coord_t             OUT_Y;

// Variables for joystick
enum JOYSTICK_state JoyPos;
u16                 JoyDelay                         = 0;
u16                 JoyInc                           = 1;

// Init pointer Info Structure (structure definition in circle.h)
tPointer_Info       POINTER_Info;

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
*
*                                POINTER_JoyTch
*
*******************************************************************************/
/**
*
*   Move the pointer according to the joystick or touchscreen, if they are
*   selected as input.
*
*   @return
**/
/******************************************************************************/
NODEBUG2 s32 POINTER_JoyTch( void )
{
#if JOYSTICK_AVAIL
    //-- Option management of the pointer by the joystick
    if ( JoystickAsInput )
    {
        JoyPos = JOYSTICK_GetState();
        switch ( JoyPos )
        {
        case JOYSTICK_DOWN:
            POINTER_Info.shift_PosX = 0;
            POINTER_Info.shift_PosY = -JoyInc ;
            break;
        case JOYSTICK_UP:
            POINTER_Info.shift_PosX = 0;
            POINTER_Info.shift_PosY =  JoyInc;
            break;
        case JOYSTICK_LEFT:
            POINTER_Info.shift_PosX = -JoyInc;
            POINTER_Info.shift_PosY = 0;
            break;
        case JOYSTICK_RIGHT_UP:
            POINTER_Info.shift_PosX =  JoyInc;
            POINTER_Info.shift_PosY =  JoyInc;
            break;
        case JOYSTICK_RIGHT:
            POINTER_Info.shift_PosX =  JoyInc;
            POINTER_Info.shift_PosY = 0;
            break;
        case JOYSTICK_RIGHT_DOWN:
            POINTER_Info.shift_PosX =  JoyInc;
            POINTER_Info.shift_PosY = -JoyInc;
            break;
        case JOYSTICK_LEFT_UP:
            POINTER_Info.shift_PosX = -JoyInc;
            POINTER_Info.shift_PosY =  JoyInc;
            break;
        case JOYSTICK_LEFT_DOWN:
            POINTER_Info.shift_PosX = -JoyInc;
            POINTER_Info.shift_PosY = -JoyInc;
            break;
        default:
            break;
        }

        if ( JoyPos != JOYSTICK_RELEASED )
            JoyDelay++;
        else
            JoyDelay = 0;

        if ( JoyDelay >= WEIGHTED_TIME( JOY_MAXBTIME ) )
            JoyInc = JOY_HIGH_SPEED;
        else
            JoyInc = JOY_LOW_SPEED;
    }
#endif //JOYSTICK_AVAIL

#if TOUCHSCREEN_AVAIL

    //-- Option management of the pointer by the Touchscreen
    if ( TchscrAsInput )
    {
        if (( TOUCHSCR_Info.xPos > 2 ) || ( TOUCHSCR_Info.yPos > 2 ) )
        {
            switch ( LCD_GetScreenOrientation() )
            {
            case V3 :
                if ( TOUCHSCR_Info.xPos < PHYS_SCREEN_WIDTH + CurrentPointerWidth )
                {
                    TOUCHSCR_SetMode( TS_POINTER );
                    POINTER_Info.xPos = TOUCHSCR_Info.xPos - 2 * POINTER_WIDTH / 3 ; // To have the bottom left hand corner point of the pointer
                    POINTER_Info.yPos = TOUCHSCR_Info.yPos - 2 * POINTER_WIDTH / 3 ; // To have the bottom left hand corner point of the pointer
                }
                break;
            case V6 :
                if ( TOUCHSCR_Info.yPos > 0 )
                {
                    TOUCHSCR_SetMode( TS_POINTER );
                    POINTER_Info.xPos = TOUCHSCR_Info.xPos - 2 * POINTER_WIDTH / 3 ; // To have the bottom left hand corner point of the pointer
                    POINTER_Info.yPos = TOUCHSCR_Info.yPos - 2 * POINTER_WIDTH / 3 ; // To have the bottom left hand corner point of the pointer
                }
                break;
            case V9 :
                if ( TOUCHSCR_Info.xPos > 0 )
                {
                    TOUCHSCR_SetMode( TS_POINTER );
                    POINTER_Info.xPos = TOUCHSCR_Info.xPos - 2 * POINTER_WIDTH / 3 ; // To have the bottom left hand corner point of the pointer
                    POINTER_Info.yPos = TOUCHSCR_Info.yPos - 2 * POINTER_WIDTH / 3 ; // To have the bottom left hand corner point of the pointer
                }
                break;

            case V12 :
                if ( TOUCHSCR_Info.yPos < PHYS_SCREEN_HEIGHT + CurrentPointerHeight )
                {
                    TOUCHSCR_SetMode( TS_POINTER );
                    POINTER_Info.xPos = TOUCHSCR_Info.xPos - 2 * POINTER_WIDTH / 3 ; // To have the bottom left hand corner point of the pointer
                    POINTER_Info.yPos = TOUCHSCR_Info.yPos - 2 * POINTER_WIDTH / 3 ; // To have the bottom left hand corner point of the pointer
                }
                break;
            }
        }
    }

#endif //TOUCHSCREEN_AVAIL

    //-- Option management of the pointer also by the mems
    if ((( MemsAsInput && ( JoyPos == JOYSTICK_RELEASED ) ) || !JoystickAsInput ) && !( TchscrAsInput ) )
    {
        return TRUE;
    }

#if JOYSTICK_AVAIL
    return FALSE;
#else
    return TRUE;
#endif
}


/*******************************************************************************
*
*                                Pointer_Move
*
*******************************************************************************/
/**
*  Moves LCD pointer according to Mems indications.
*
*  @retval 0 if the pointer is within the screen limits, else -1 if it touches
*          the screen edges.
*
**/
/******************************************************************************/
static bool POINTER_Move( void )
{
    s16 oldPointer_xPos        = POINTER_Info.xPos;
    s16 oldPointer_yPos        = POINTER_Info.yPos;
    signed outx                = MEMS_Info.OutX;
    signed outy                = MEMS_Info.OutY;

    POINTER_Info.shift_PosX  =  POINTER_Info.shift_PosY  = 0;

    if ( POINTER_JoyTch() != 0 )  // If Joystick used as input
    {
        // The move depends on the screen orientation
        switch ( LCD_GetScreenOrientation() )
        {
            // north
        case V12 :
            MEMS_Info.RELATIVE_X = outx;
            MEMS_Info.RELATIVE_Y = outy;

            if ( outx > MIN_ANGLE_FOR_SHIFT_RIGHT )
            {
                POINTER_Info.shift_PosX = ( outx - MIN_ANGLE_FOR_SHIFT_RIGHT );
            }
            else if ( outx < MIN_ANGLE_FOR_SHIFT_LEFT )
            {
                POINTER_Info.shift_PosX  = ( outx - MIN_ANGLE_FOR_SHIFT_LEFT );
            }

            if ( outy < -MIN_ANGLE_FOR_SHIFT_UP )
            {
                POINTER_Info.shift_PosY = ( outy + MIN_ANGLE_FOR_SHIFT_UP );
            }
            else if ( outy > -MIN_ANGLE_FOR_SHIFT_DOWN )
            {
                POINTER_Info.shift_PosY = ( outy + MIN_ANGLE_FOR_SHIFT_DOWN );
            }
            break;

            // West
        case V9 :
            MEMS_Info.RELATIVE_X = -( outy );
            MEMS_Info.RELATIVE_Y = outx;

            if ( outy > MIN_ANGLE_FOR_SHIFT_RIGHT )
            {
                POINTER_Info.shift_PosX = -( outy - MIN_ANGLE_FOR_SHIFT_RIGHT );
            }
            else if ( outy < MIN_ANGLE_FOR_SHIFT_LEFT )
            {
                POINTER_Info.shift_PosX = -( outy - MIN_ANGLE_FOR_SHIFT_LEFT );
            }

            if ( outx < -MIN_ANGLE_FOR_SHIFT_UP )
            {
                POINTER_Info.shift_PosY = ( outx + MIN_ANGLE_FOR_SHIFT_UP );
            }
            else if ( outx > -MIN_ANGLE_FOR_SHIFT_DOWN )
            {
                POINTER_Info.shift_PosY = ( outx + MIN_ANGLE_FOR_SHIFT_DOWN );
            }
            break;

            // South
        case V6 :
            MEMS_Info.RELATIVE_X = -( outx );
            MEMS_Info.RELATIVE_Y = -( outy );

            if ( outx > MIN_ANGLE_FOR_SHIFT_RIGHT )
            {
                POINTER_Info.shift_PosX = ( MIN_ANGLE_FOR_SHIFT_RIGHT - outx );
            }
            else if ( outx < MIN_ANGLE_FOR_SHIFT_LEFT )
            {
                POINTER_Info.shift_PosX = ( MIN_ANGLE_FOR_SHIFT_LEFT - outx );
            }

            if ( outy > MIN_ANGLE_FOR_SHIFT_UP )
            {
                POINTER_Info.shift_PosY = -( outy - MIN_ANGLE_FOR_SHIFT_UP );
            }
            else if ( outy < MIN_ANGLE_FOR_SHIFT_DOWN )
            {
                POINTER_Info.shift_PosY = +( MIN_ANGLE_FOR_SHIFT_DOWN - outy );
            }
            break;

            // East
        case V3 :
            MEMS_Info.RELATIVE_X = outy;
            MEMS_Info.RELATIVE_Y = -( outx );

            if ( outy > MIN_ANGLE_FOR_SHIFT_RIGHT )
            {
                POINTER_Info.shift_PosX = ( outy - MIN_ANGLE_FOR_SHIFT_RIGHT );
            }
            else if ( outy < MIN_ANGLE_FOR_SHIFT_LEFT )
            {
                POINTER_Info.shift_PosX = ( outy - MIN_ANGLE_FOR_SHIFT_LEFT );
            }

            if ( outx > MIN_ANGLE_FOR_SHIFT_UP )
            {
                POINTER_Info.shift_PosY = ( MIN_ANGLE_FOR_SHIFT_UP - outx );
            }
            else if ( outx < MIN_ANGLE_FOR_SHIFT_DOWN )
            {
                POINTER_Info.shift_PosY = ( MIN_ANGLE_FOR_SHIFT_DOWN - outx );
            }

        default :
            break;
        }

        POINTER_Info.shift_PosX /= CurrentSpeedOnAngle;
        POINTER_Info.shift_PosY /= CurrentSpeedOnAngle;
    }

    if ( Pointer_Mode == POINTER_APPLICATION )
    {
        if ( Application_Pointer_Mgr )
        {
            Application_Pointer_Mgr( POINTER_Info.shift_PosX, POINTER_Info.shift_PosY );
        }

        return 0;
    }

    POINTER_Info.xPos += POINTER_Info.shift_PosX;
    POINTER_Info.yPos += POINTER_Info.shift_PosY;

    if ( POINTER_Info.xPos < POINTER_Info.X_PosMin )
    {
        POINTER_Info.xPos = POINTER_Info.X_PosMin;
    }

    if ( POINTER_Info.xPos > POINTER_Info.X_PosMax )
    {
        POINTER_Info.xPos = POINTER_Info.X_PosMax;
    }

    if ( POINTER_Info.yPos < POINTER_Info.Y_PosMin )
    {
        POINTER_Info.yPos = POINTER_Info.Y_PosMin;
    }

    if ( POINTER_Info.yPos > POINTER_Info.Y_PosMax )
    {
        POINTER_Info.yPos = POINTER_Info.Y_PosMax;
    }

    if (( Pointer_Mode != POINTER_MENU ) && ( Pointer_Mode != POINTER_RESTORE_LESS ) &&
            (( oldPointer_xPos != POINTER_Info.xPos ) || ( oldPointer_yPos != POINTER_Info.yPos ) ) )
    {
        // Use default area.
        POINTER_SetCurrentAreaStore( 0 );

        // Restore previously drawn area.
        POINTER_Restore( oldPointer_xPos, oldPointer_yPos, POINTER_WIDTH, POINTER_WIDTH );

        // Save new area and draw pointer
        POINTER_Save( POINTER_Info.xPos, POINTER_Info.yPos, POINTER_WIDTH, POINTER_WIDTH );
        POINTER_Draw( POINTER_Info.xPos, POINTER_Info.yPos, POINTER_WIDTH, POINTER_WIDTH, CurrentPointerBmp );
    }

    if (( Pointer_Mode == POINTER_RESTORE_LESS ) &&
            (( oldPointer_xPos != POINTER_Info.xPos ) || ( oldPointer_yPos != POINTER_Info.yPos ) ) )
    {
        // Use default area.
        POINTER_SetCurrentAreaStore( 0 );

        // Restore previously drawn area.
        POINTER_Restore( oldPointer_xPos, oldPointer_yPos, CurrentPointerWidth, CurrentPointerHeight );

        // Save new area and draw pointer
        POINTER_Save( POINTER_Info.xPos, POINTER_Info.yPos, CurrentPointerWidth, CurrentPointerHeight );
        POINTER_Draw( POINTER_Info.xPos, POINTER_Info.yPos, CurrentPointerWidth, CurrentPointerHeight, CurrentPointerBmp );
    }

    // Is the pointer touching one edge of the screen ?
    if (( POINTER_Info.xPos == POS_MIN ) || ( POINTER_Info.yPos == POS_MIN ) ||
            ( POINTER_Info.xPos == POS_MAX ) || ( POINTER_Info.yPos == POS_MAX ) )
    {
        return -1;
    }

    return 0;
}

/* Public functions for CircleOS ---------------------------------------------*/

/*******************************************************************************
*
*                                POINTER_Init
*
*******************************************************************************/
/**
*  Initialize pointer. Called at CircleOS startup. Set default pointer at the
*  middle of the screen and allows it to move into the whole screen.
*
*  @attention  This function must <b>NOT</b> be called by the user.
*
**/
/******************************************************************************/
NODEBUG2 void POINTER_Init( void )
{
    POINTER_Info.xPos = ( Screen_Width / 2 ) - ( POINTER_WIDTH / 2 ) - 1;
    POINTER_Info.yPos = ( Screen_Width / 2 ) - ( POINTER_WIDTH / 2 ) - 1;
    POINTER_Info.shift_PosX = 0;
    POINTER_Info.shift_PosY = 0;

    // Increase pointer sensibility.
    POINTER_SetCurrentSpeedOnAngle( DEFAULT_SPEED_ON_ANGLE );
    POINTER_SetCurrentAngleStart( DEFAULT_ANGLESTART );
    POINTER_SetRectScreen();
    POINTER_SetCurrentPointer( POINTER_WIDTH, POINTER_WIDTH, BallPointerBmp );
    POINTER_SetMode( POINTER_ON );
//    POINTER_SetPos( 0x3c, 0x3c );   //FL081103 very first drawing was dirty!
    // YRT091120 : second pointer appears

    CurrentPointerColor = POINTER_DEFAULT_COLOR;
}

/*******************************************************************************
*
*                                POINTER_Handler
*
*******************************************************************************/
/**
*
*  Called by the CircleOS scheduler to manage the pointer.
*
*  @attention  This function must <b>NOT</b> be called by the user.
*
**/
/******************************************************************************/
void POINTER_Handler( void )
{
    static divider_t dividr = 0;

    // Goes into handler only every POINTER_DIVIDER calls.
    if ( ++dividr % WEIGHTED_TIME( POINTER_DIVIDER ) )
    {
        return;
    }

    switch ( Pointer_Mode )
    {
        // Nothing to do!
    case POINTER_OFF  :
    case POINTER_UNDEF:
        return;
    }

#if MEMS_POINTER     // Not necessary if no MEMS menu handling
    // Where is the MEMS ?
    MEMS_GetPosition( &OUT_X, &OUT_Y );
#endif

    /*
    DRAW_DisplayString  ( 20, 200, "X=" , 3);
    UTIL_int2str  ( buffer,  OUT_X, 4, 0);
    DRAW_DisplayString  ( 40, 200, buffer , 4);

    DRAW_DisplayString  ( 20, 180, "Y=" , 3);
    UTIL_int2str  ( buffer,  OUT_Y, 4, 0);
    DRAW_DisplayString  ( 40, 180, buffer , 4);
    */

    POINTER_Move();
}

/// @endcond

/* Public functions ----------------------------------------------------------*/

/*******************************************************************************
*
*                                POINTER_SetCurrentPointer
*
*******************************************************************************/
/**
*
*  Set the dimension and the bitmap of the pointer.
*  @note The bitmap is a monochrome one!
*
*  @param[in]  width    width of the pointer  (u8)
*  @param[in]  height   height of the pointer (u8)
*  @param[in]  bmp      pointer to an array of width * height bits.
*
**/
/********************************************************************************/
void POINTER_SetCurrentPointer( coord_t width, coord_t height, const u8* bmp )
{
    if ( !bmp )
    {
        bmp = BallPointerBmp;
    }

    // Update the area limits for the new pointer depending on its size
    POINTER_SetRect( POINTER_Info.X_PosMin, POINTER_Info.Y_PosMin,
                     POINTER_Info.X_PosMax - POINTER_Info.X_PosMin + CurrentPointerWidth - width,
                     POINTER_Info.Y_PosMax - POINTER_Info.Y_PosMin + CurrentPointerHeight - height );

    CurrentPointerWidth  = width;
    CurrentPointerHeight = height;
    CurrentPointerBmp    = bmp;
}

/*******************************************************************************
*
*                                POINTER_GetCurrentAngleStart
*
*******************************************************************************/
/**
*
*  Get the current minimal angle to move pointer
*
*  @return  current minimal angle.
*
**/
/******************************************************************************/
s16 POINTER_GetCurrentAngleStart( void )
{
    return CurrentAngleStart;
}

/*******************************************************************************
*
*                                POINTER_SetCurrentAngleStart
*
*******************************************************************************/
/**
*
*  Set the current minimal angle to move pointer
*
*  @param[in]  newangle The new minimal angle to move pointer.
*
**/
/******************************************************************************/
void POINTER_SetCurrentAngleStart( u16 newangle )
{
    CurrentAngleStart = newangle;
}

/*******************************************************************************
*
*                                POINTER_GetCurrentSpeedOnAngle
*
*******************************************************************************/
/**
*
*  Return the current speed/angle ratio.
*
*  @return  current ratio.
*
**/
/******************************************************************************/
u16 POINTER_GetCurrentSpeedOnAngle( void )
{
    return CurrentSpeedOnAngle;
}

/*******************************************************************************
*
*                                POINTER_SetCurrentSpeedOnAngle
*
*******************************************************************************/
/**
*
*  Set the current speed/angle ratio.
*
*  @param[in]  newspeed New speed/angle ratio.
*
**/
/******************************************************************************/
void POINTER_SetCurrentSpeedOnAngle( u16 newspeed )
{
    CurrentSpeedOnAngle = newspeed;
}

/*******************************************************************************
*
*                                POINTER_SetCurrentAreaStore
*
*******************************************************************************/
/**
*
*  Change the current storage area. If the provided value is NULL, the default
*  storage area will be used.
*
*  @param[in]  ptr New storage area (may be null).
*
*  @warning    Memory space pointed by the provided pointer must be large enough
*              to store a color bitmap corresponding to the pointer area.
*              In other words, space must be width * height * 2 bytes large.
*
**/
/******************************************************************************/
void POINTER_SetCurrentAreaStore( u8* ptr )
{
    ptrAreaStore = ( ptr == 0 ) ? DefaultAreaStore : ptr;
}

/*******************************************************************************
*
*                                POINTER_SetMode
*
*******************************************************************************/
/**
*
*  Change the current mode of the pointer management.
*
*  @note Must be called only ONCE!!
*
*  @param[in]  mode New pointer management mode.
*
**/
/******************************************************************************/
void POINTER_SetMode( enum POINTER_mode mode )
{
    u16* ptr;
    u16  i;
    u16  color;

    if ( Pointer_Mode == mode )
        return;

    switch ( mode )
    {
    case POINTER_APPLICATION:
        ptr   = ( u16* )DefaultAreaStore;
        color = DRAW_GetBGndColor();

        for ( i = 0; i < ( CurrentPointerWidth * CurrentPointerHeight ) ; i++ )
        {
            *ptr++ = color;
        }

        POINTER_Draw( POINTER_Info.xPos, POINTER_Info.yPos, CurrentPointerWidth, CurrentPointerHeight, CurrentPointerBmp );
        break;

    case POINTER_RESTORE_LESS:
        POINTER_Draw( POINTER_Info.xPos, POINTER_Info.yPos, CurrentPointerWidth, CurrentPointerHeight, CurrentPointerBmp );
        break;

    case POINTER_ON:
        POINTER_SetCurrentAreaStore( 0 );
        POINTER_Save( POINTER_Info.xPos, POINTER_Info.yPos, POINTER_WIDTH, POINTER_WIDTH );
        POINTER_Draw( POINTER_Info.xPos, POINTER_Info.yPos, CurrentPointerWidth, CurrentPointerHeight, CurrentPointerBmp );
        POINTER_Draw( POINTER_Info.xPos, POINTER_Info.yPos, CurrentPointerWidth, CurrentPointerHeight, CurrentPointerBmp );
        break;

    case POINTER_OFF:
        POINTER_Info.xPos = ( Screen_Width - POINTER_WIDTH ) / 2;
        POINTER_Info.yPos = ( Screen_Width - POINTER_WIDTH ) / 2;

    case POINTER_MENU:
        if ( Pointer_Mode == POINTER_ON )
        {
            POINTER_SetCurrentAreaStore( 0 );
            POINTER_Restore( POINTER_Info.xPos, POINTER_Info.yPos, POINTER_WIDTH, POINTER_WIDTH );
        }
        break;
    }

    Pointer_Mode = mode;
}

/*******************************************************************************
*
*                                POINTER_GetMode
*
*******************************************************************************/
/**
*
*  Return the current mode of the pointer management
*
*  @return  Current pointer management mode.
*
**/
/******************************************************************************/
enum POINTER_mode POINTER_GetMode( void )
{
    return Pointer_Mode;
}

/*******************************************************************************
*
*                                POINTER_GetState
*
*******************************************************************************/
/**
*
*  Return current pointer state.
*
*  @return  Current pointer state.
*
**/
/******************************************************************************/
enum POINTER_state POINTER_GetState( void )
{
    return Pointer_State;
}

/*******************************************************************************
*
*                                POINTER_SetRect
*
*******************************************************************************/
/**
*
*  Set new limits for the move of the pointer
*
*  @param[in]  x        Horizontal coordinate of the bottom left corner of the new area.
*  @param[in]  y        Vertical coordinate of the bottom left corner of the new are.
*  @param[in]  width    New area width.
*  @param[in]  height   New area height.
*
*  @warning       The (0x0) point in on the low left corner.
*
**/
/******************************************************************************/
void POINTER_SetRect( coord_t x, coord_t y, coord_t width, coord_t height )
{
    POINTER_Info.X_PosMin = x;

    if ( POINTER_Info.xPos < POINTER_Info.X_PosMin )
    {
        POINTER_Info.xPos = POINTER_Info.X_PosMin;
    }

    POINTER_Info.X_PosMax = x + width;

    if ( POINTER_Info.xPos > POINTER_Info.X_PosMax )
    {
        POINTER_Info.xPos = POINTER_Info.X_PosMax;
    }

    POINTER_Info.Y_PosMin = y;

    if ( POINTER_Info.yPos < POINTER_Info.Y_PosMin )
    {
        POINTER_Info.yPos = POINTER_Info.Y_PosMin;
    }

    POINTER_Info.Y_PosMax = y + height;

    if ( POINTER_Info.yPos > POINTER_Info.Y_PosMax )
    {
        POINTER_Info.yPos = POINTER_Info.Y_PosMax;
    }
}

/*******************************************************************************
*
*                                POINTER_SetRectScreen
*
*******************************************************************************/
/**
*
*  Allow the pointer to move on the whole screen.
*
**/
/******************************************************************************/
void POINTER_SetRectScreen( void )
{
    POINTER_SetRect( 0, 0, Screen_Width - CurrentPointerWidth, Screen_Height - CurrentPointerHeight );
}

/*******************************************************************************
*
*                                POINTER_GetPos
*
*******************************************************************************/
/**
*
*  Return the current position of the pointer (on the screen).
*
*  @return  The current pointer screen position with X in the LSB and Y in the MSB.
*
*  @warning       The (0x0) point in on the low left corner.
**/
/******************************************************************************/
u16 POINTER_GetPos( void )
{
    return ( POINTER_Info.xPos | ( POINTER_Info.yPos << 8 ) );
}

/*******************************************************************************
*
*                                POINTER_SetPos
*
*******************************************************************************/
/**
*
*  Force the screen position of the pointer.
*
*  @param[in]  x  New horizontal coordinate.
*  @param[in]  y  New vertical coordinate.
*
*  @warning       The (0x0) point in on the low left corner.
*
**/
/******************************************************************************/
void POINTER_SetPos( coord_t x, coord_t y ) //TODO GetPos uses a single u16 (u8+u8), why does SetPos use 2 u16?
{
    POINTER_Info.xPos = x;
    POINTER_Info.yPos = y;
}

/*******************************************************************************
*
*                                POINTER_Draw
*
*******************************************************************************/
/**
*
*  Draw pointer.
*
*  @param[in]  x        Horizontal coordinate of the bottom left corner of the pointer.
*  @param[in]  y        Vertical coordinate of the bottom left corner of the pointer.
*  @param[in]  width    Pointer bitmap width.
*  @param[in]  height   Pointer bitmap height.
*  @param[in]  bmp      Pointer to width * height bit array. If null used default
*                       pointer bitmap.
*
*  @note          The provided bitmap is a monochrome one.
*  @warning       The (0x0) point in on the low left corner.
*
**/
/******************************************************************************/
void POINTER_Draw( coord_t x, coord_t y, coord_t width, coord_t height, const u8* bmp )
{
    s32 i   = 0;
    s32 l   = 0;
    s32 n   = 0;
    u8* ptr = ptrAreaStore;
    u8  c;
    u16 val;

    // No bitmap provided, use the default one!
    if ( !bmp )
    {
        bmp = BallPointerBmp;
    }

    // Select the screen area were going to take care about!
    LCD_SetRect_For_Cmd( x, y, width, height );

    /* Send LCD RAM write command. */
    LCD_SendLCDCmd_RAM_Access();

    while ( n < ( width * height ) )
    {
        if ( Pointer_Mode != POINTER_RESTORE_LESS )
        {
            // Draw pixel using current storage area data for background pixels.
            c = *ptr++;

            LCD_SendLCDData(( bmp[l + ( i / 8 )] & ( 1 << ( 7 - ( i % 8 ) ) ) ) ? ( POINTER_GetColor() & 255 ) : c );

            c = *ptr++;
            LCD_SendLCDData(( bmp[l + ( i / 8 )] & ( 1 << ( 7 - ( i % 8 ) ) ) ) ? ( POINTER_GetColor() >> 8 )  : c );
        }
        else
        {
            // POINTER_RESTORE_LESS: use current background color for background color.
            c = DRAW_GetBGndColor();
            val = ( bmp[l + ( i / 8 )] & ( 1 << ( 7 - ( i % 8 ) ) ) ) ? POINTER_GetColor() : c;

            LCD_SendLCDData( val & 255 );
            LCD_SendLCDData( val >> 8 );
        }

        n++;

        i++;

        // End of line ?
        if ( i == width )
        {
            // Next line!
            l++;
            i = 0;
        }
    }

    /* End of select screen area to access.*/
    LCD_SendLCDCmd_RAM_Access_End();

}

/*******************************************************************************
*
*                                POINTER_Save
*
*******************************************************************************/
/**
*
*  Save the background of the pointer.
*
*  @param[in]  x        Horizontal coordinate of the bottom left corner of the area to save.
*  @param[in]  y        Vertical coordinate of the bottom left corner of the area to save.
*  @param[in]  width    Width of the area to save.
*  @param[in]  height   Height of the area to save.
*
*  @note          The store area must be large enough to store all the pixels (16 bits).
*  @warning       The (0x0) point in on the low left corner.
*  @see  POINTER_Restore
*  @see  POINTER_SetCurrentAreaStore
*
**/
/******************************************************************************/
void POINTER_Save( coord_t x, coord_t y, coord_t width, coord_t height )
{
    // Is this pointer management mode, don't save pointer background!
    if ( Pointer_Mode == POINTER_RESTORE_LESS )
    {
        return;
    }

    //Use standard graphic function
    LCD_RectRead( x, y, width, height, ptrAreaStore );
}

/*******************************************************************************
*
*                                POINTER_Restore
*
*******************************************************************************/
/**
*
*  Restore the background of the pointer with data saved in the current store area.
*
*  @param[in]  x        Horizontal coordinate of the bottom left corner of the area to restore.
*  @param[in]  y        Vertical coordinate of the bottom left corner of the area to restore.
*  @param[in]  width    Width of the area to restore.
*  @param[in]  height   Height of the area to restore.
*
*  @warning       The (0x0) point in on the low left corner.
*  @see  POINTER_Save
*  @see  POINTER_SetCurrentAreaStore
*
**/
/******************************************************************************/
void POINTER_Restore( coord_t x, coord_t y, coord_t width, coord_t height )
{
    u16 i;
    u8* ptr      = ptrAreaStore;
    u16 bytesize = ( width * height ) * 2;  // 2 bytes per pixel

    // Select the screen area to write.
    LCD_SetRect_For_Cmd( x, y, width, height );

    // Send the memory write command to the LCD controller.
    LCD_SendLCDCmd_RAM_Access();

    for ( i = 0; i < bytesize; i++ )
    {
        // In this mode, use background color (no data was previously saved).
        if ( Pointer_Mode == POINTER_RESTORE_LESS )
        {
            LCD_SendLCDData( DRAW_GetBGndColor() );
        }
        else
        {
            LCD_SendLCDData( *ptr++ );
        }
    }

    /* End of select screen area to access.*/
    LCD_SendLCDCmd_RAM_Access_End();

}

/*******************************************************************************
*
*                                POINTER_SetApplication_Pointer_Mgr
*
*******************************************************************************/
/**
*
*  Provides an user defined pointer manager.
*
*  @param[in]  mgr Pointer to the user defined pointer manager.
*
**/
/******************************************************************************/
void POINTER_SetApplication_Pointer_Mgr( tAppPtrMgr mgr )
{
    Application_Pointer_Mgr = mgr;
}

/*******************************************************************************
*
*                                POINTER_SetColor
*
*******************************************************************************/
/**
*
*  Set the pointer color.
*
*  @param[in]  color The new pointer color.
*
**/
/******************************************************************************/
void POINTER_SetColor( color_t color )
{
    CurrentPointerColor = color;
}

/*******************************************************************************
*
*                                POINTER_GetColor
*
*******************************************************************************/
/**
*
*  Return the current pointer color.
*
*  @return  Current pointer color.
*
**/
/******************************************************************************/
color_t POINTER_GetColor( void )
{
    return CurrentPointerColor;
}

/*******************************************************************************
*
*                                POINTER_GetInfo
*
*******************************************************************************/
/**
*
*  Get pointer informations.
*
*  @return  A pointer to a pointer information structure.
*
**/
/******************************************************************************/
tPointer_Info* POINTER_GetInfo( void )
{
    return &POINTER_Info;
}
