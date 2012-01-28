/****************** COPYRIGHT (C) 2007-2010 RAISONANCE S.A.S. *****************/
/**
*
* @file     button.c
* @brief    Common Button and Joystick initialization and management.
* @author   FL
* @date     07/2007
*
* @date     10/2008
* @version  3.0 Add joystick for Primer 2
* @date     10/2009
* @version  4.0 Add Open4 Primer
**/
/******************************************************************************/
/// @cond Internal

/* Includes ------------------------------------------------------------------*/
#include "circle.h"

/* Private define ------------------------------------------------------------*/
#define  BUTTON_DIVIDER          10 /*!< Passes in BUTTON_Handler needed to enter it */
#define  SHUTDOWNCOUNT           200

/* Private variables ---------------------------------------------------------*/
bool                fWaitForRelease    = 0;
enum BUTTON_state   Button_State       = BUTTON_UNDEF;
enum BUTTON_mode    Button_Mode        = BUTTON_DISABLED;
enum JOYSTICK_state Joystick_State     = JOYSTICK_UNDEF;
counter_t           Joystick_Counter   = 0;
bool                fJoyWaitForRelease = 0;
counter_t           Button_LongCounter = 0;
counter_t           Button_Counter     = 0;


/* Extern variables ----------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
void JOYSTICK_Handler( void );

/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
*
*                                BUTTON_Handler
*
*******************************************************************************/
/**
*
*  Called by the CircleOS scheduler to manage BUTTON tasks such as the anti-bouncing.
*
*  @attention  This function must <b>NOT</b> be called by the user.
*
**/
/******************************************************************************/
void BUTTON_Handler( void )
{
    u8 state;
    enum BUTTON_state old_state = Button_State;
    enum BUTTON_state new_state;

    static divider_t divider = 0;

    if ( ++divider % BUTTON_DIVIDER )
    {
        return;
    }

    state = BUTTON_GetNewState();

    /* Manage shutdown by a long pressing.*/
    if ( state )
    {
        Button_LongCounter++;

        if ( Button_LongCounter > WEIGHTED_TIME( SHUTDOWNCOUNT ) )
        {
            LCD_SetOffset( OFFSET_OFF );
            LCD_ClearAllScreen( RGB_BLACK );
            DRAW_SetDefaultColor();
            DRAW_RestoreCharMagniCoeff();
            DRAW_DisplayStringWithMode( 0, 100, "Shutting down !", ALL_SCREEN, INVERTED_TEXT, CENTER );
            DRAW_DisplayStringWithMode( 0, 50, "Release", ALL_SCREEN, INVERTED_TEXT, CENTER );
            DRAW_DisplayStringWithMode( 0, 20, "the button !", ALL_SCREEN, INVERTED_TEXT, CENTER );
            DRAW_SetCharMagniCoeff( 1 );

            while ( 1 )
            {
                SHUTDOWN_Action();
            }
        }
    }
    else
    {
        Button_LongCounter = 0;
    }

    if (( Button_Mode == BUTTON_ONOFF ) || ( Button_Mode == BUTTON_ONOFF_FORMAIN ) )
    {
        new_state = state ? BUTTON_PUSHED : BUTTON_RELEASED;

        if (( Button_Counter == 0 )  && ( new_state != old_state ) )
        {
            Button_State = new_state;
            Button_Counter++;
        }
        else if (( Button_Counter < WEIGHTED_TIME( ANTI_BOUNCING_COUNT ) )  && fWaitForRelease )
        {
            Button_Counter++;
        }
        else
        {
            if ( fWaitForRelease && ( new_state == BUTTON_PUSHED ) )
            {
                /* Restart again until release.*/
                Button_Counter = 1;

                return;
            }
            else
            {
                Button_Counter    = 0;
                fWaitForRelease   = 0;
                Button_State      = new_state;
            }
        }
    }

    /* Call the Joystick management if available */
#if JOYSTICK_AVAIL
    JOYSTICK_Handler();
#endif
}

#if JOYSTICK_AVAIL
/*******************************************************************************
*
*                                JOYSTICK_Handler
*
*******************************************************************************/
/**
*
*  Called by the CircleOS scheduler to manage Joystick tasks such as the anti-bouncing.
*
*  @attention  This function must <b>NOT</b> be called by the user.
*
**/
/******************************************************************************/
void JOYSTICK_Handler( void )
{
    enum JOYSTICK_state old_state_joy = Joystick_State;
    enum JOYSTICK_state new_state_joy;

    new_state_joy = JOYSTICK_GetNewState();

    if (( Joystick_Counter == 0 )  && ( new_state_joy != old_state_joy ) )
    {
        Joystick_State = new_state_joy;
        Joystick_Counter ++;
    }
    else if (( Joystick_Counter < WEIGHTED_TIME( ANTI_BOUNCING_COUNT / 3 ) ) && fJoyWaitForRelease )
    {
        Joystick_Counter++;
    }
    else
    {
        if ( fJoyWaitForRelease && ( new_state_joy != JOYSTICK_RELEASED ) )
        {
            /* Restart again until release.*/
            Joystick_Counter = 1;
        }
        else
        {
            Joystick_Counter   = 0;
            fJoyWaitForRelease = 0;
            Joystick_State     = new_state_joy;
        }
    }
}


/*******************************************************************************
*
*                                JOYSTICK_CircularPermutation
*
*******************************************************************************/
/**
*
*
*  Permutate clockwise the joystick state :
*
*  left -> up -> right -> down -> left.
    left_up-> right_up -> right_down -> left_down -> left_up.
*
* @param abs_direction joystick direction to permute.
* @param iter number of permutations (90 degrees right).
* @return The new direction value.
*
*
**/
/******************************************************************************/
enum JOYSTICK_state JOYSTICK_CircularPermutation( enum JOYSTICK_state abs_direction, s8 iter )
{
    s8 i;
    enum JOYSTICK_state direction = abs_direction;

//    for(i = 0 ; i < iter ; i++)
    for ( i = iter % 4; i > 0; --i )
    {
        switch ( direction )
        {
        case JOYSTICK_LEFT :
            direction = JOYSTICK_UP;
            break;
        case JOYSTICK_UP :
            direction = JOYSTICK_RIGHT;
            break;
        case JOYSTICK_RIGHT :
            direction = JOYSTICK_DOWN;
            break;
        case JOYSTICK_DOWN :
            direction = JOYSTICK_LEFT;
            break;
        case JOYSTICK_LEFT_UP :
            direction = JOYSTICK_RIGHT_UP;
            break;
        case JOYSTICK_RIGHT_UP :
            direction = JOYSTICK_RIGHT_DOWN;
            break;
        case JOYSTICK_RIGHT_DOWN :
            direction = JOYSTICK_LEFT_DOWN;
            break;
        case JOYSTICK_LEFT_DOWN :
            direction = JOYSTICK_LEFT_UP;
            break;
        }
    }

    return direction;
}

#endif // JOYSTICK_AVAIL

/// @endcond

/* Public functions ----------------------------------------------------------*/


/*******************************************************************************
*
*                                BUTTON_GetMode
*
*******************************************************************************/
/**
*
*  Return current button mode.
*
*  @return Current button mode.
*
**/
/******************************************************************************/
enum BUTTON_mode BUTTON_GetMode( void )
{
    return Button_Mode;
}

/*******************************************************************************
*
*                                BUTTON_SetMode
*
********************************************************************************/
/**
*
*  Set new button mode.
*
*  @param[in]  mode  The new button mode.
*
**/
/********************************************************************************/
void BUTTON_SetMode( enum BUTTON_mode mode )
{
    Button_Mode = mode;

#if POWER_MNGT
    if ( mode == BUTTON_ONOFF_FORMAIN )
    {
        POWER_Set_Time();
    }
    else
    {
        POWER_Reset_Time();
    }
#endif /* POWER_MNGT */
}


/*******************************************************************************
*
*                                BUTTON_GetState
*
*******************************************************************************/
/**
*
*  Return current button state.
*
*  @return  Current button state.
*
**/
/******************************************************************************/
enum BUTTON_state BUTTON_GetState( void )
{
    if ( fWaitForRelease )
    {
        return BUTTON_UNDEF;
    }

    if ( Button_State == BUTTON_PUSHED )
    {
        if ( Button_Mode == BUTTON_ONOFF_FORMAIN )
        {
            return BUTTON_PUSHED_FORMAIN;
        }
    }

    return Button_State;
}

/*******************************************************************************
*
*                                BUTTON_WaitForRelease
*
*******************************************************************************/
/**
*
*  Disable temporarily any new button event.
*
**/
/******************************************************************************/
void BUTTON_WaitForRelease( void )
{
    fWaitForRelease = 1;
}

#if JOYSTICK_AVAIL
/*******************************************************************************
*
*                                 JOYSTICK_GetState
*
*******************************************************************************/
/**
*  Decodes the Joystick direction.
*
*  @return       The direction value.
*
*******************************************************************************/
enum JOYSTICK_state JOYSTICK_GetState( void )
{
    if ( fJoyWaitForRelease )
        return JOYSTICK_UNDEF;
    else
        return Joystick_State;
}


/*******************************************************************************
*
*                                JOYSTICK_WaitForRelease
*
*******************************************************************************/
/**
*
*  Disable temporarily any new button event.
*
**/
/******************************************************************************/
void JOYSTICK_WaitForRelease( void )
{
    fJoyWaitForRelease = 1;
}

#endif // JOYSTICK_AVAIL
