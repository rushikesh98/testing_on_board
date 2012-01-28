/****************** COPYRIGHT (C) 2007-2010 RAISONANCE S.A.S. *****************/
/**
*
* @file     toolbar.c
* @brief    Common various utilities for touchscreen toolbar
* @author   FL / YRT
* @date     10/2008
*
* @date     01/2009
* @version  3.3     Replace two colors buttons by true color pictures
* @date     10/2009
* @version  4.0 Add Open4 Primer
**/
/******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "circle.h"

/// @cond Internal

/* Private define ------------------------------------------------------------*/

/* Extern variables ---------------------------------------------------------*/

//-- System toolbar shown by default
extern const u16 ButtonMenuBmp[];
extern const u16 ButtonLowSoundHPBmp[];
extern const u16 ButtonHighSoundHPBmp[];
extern const u16 ButtonMuteHPBmp[];
extern const u16 ButtonSoundHPBmp[];
extern const u16 ButtonLowSoundLSBmp[];
extern const u16 ButtonHighSoundLSBmp[];
extern const u16 ButtonMuteLSBmp[];
extern const u16 ButtonSoundLSBmp[];

/* Private functions ---------------------------------------------------------*/
enum MENU_code fLowSound();
enum MENU_code fHighSound();
enum MENU_code fMuteSound();
enum MENU_code fDefaultAction();

/* Private variables ---------------------------------------------------------*/
s32 ScreenButton = -1;       // Current pushed button (on the touchscreen)
s32 CurOrientation = V12;    // Current orientation of the screen
s32 OldOrientation = V12;
s32 RedrawToolbarRequest;
divider_t Max_cnt_tb = 100;       // System tick divider

tToolbar*         CurrentToolbar    = 0;
tToolbarItem*     CurrentTbCommand  = 0;


tToolbar DefaultToolbar =
{
    TB_NB_BUTTONS,
    0,
    {
        { ButtonMenuBmp,        fDefaultAction  },
        { ButtonLowSoundLSBmp,  fLowSound  },
        { ButtonHighSoundLSBmp, fHighSound },
        { ButtonMuteLSBmp,      fMuteSound },
    }
};

/*******************************************************************************
*
*                                TOOLBAR_Init
*
*******************************************************************************/
/**
*
*  Called by the CircleOS scheduler to initialize the toolbar.
*
*  @attention  This function must <b>NOT</b> be called by the user.
*
**/
/******************************************************************************/
NODEBUG2 void TOOLBAR_Init( void )
{
    LCD_FillRect_Circle( 0, PHYS_SCREEN_HEIGHT-BUTTON_HEIGHT, PHYS_SCREEN_WIDTH, BUTTON_HEIGHT, RGB_WHITE );
    TOOLBAR_SetDefaultToolbar();
}

/*******************************************************************************
*
*                                TOOLBAR_Handler
*
*******************************************************************************/
/**
*
*  Called by the CircleOS scheduler to manage the toolbar.
*
*  @attention  This function must <b>NOT</b> be called by the user.
*
**/
/******************************************************************************/
void TOOLBAR_Handler( void )
{
    static divider_t divider_coord = 0;
    s32 OldButton = ScreenButton;
    s32 X, Y;
    vs32 i;

    // Don't execute, if it's not time
    if (( divider_coord++ % Max_cnt_tb ) != 0 )
        return;

    // Don't execute, if touchscreen not operational (calibration...)
    if ( TOUCHSCR_GetMode() == TS_CALIBRATION )
        return;

    // See if a hit on the touchscreen has been done
    if ( TOUCHSCR_IsPressed() )
    {
        // Get the position
        X = TOUCHSCR_Info.xAbsPos;
        Y = TOUCHSCR_Info.yAbsPos;

        // First test whether a button is pressed...
        if ( Y > APP_SCREEN_HEIGHT )
        {
            // A button is pushed
            ScreenButton = X / BUTTON_WIDTH;
            if ( ScreenButton > ( CurrentToolbar->NbItems - 1 ) )
            {
                ScreenButton = CurrentToolbar->NbItems - 1;
            }
            if ( ScreenButton < 0 )
            {
                ScreenButton = 0;
            }
        }
        else
            // No button selected
            ScreenButton = -1;
    }
    else
    {
        // See if a redraw is requested
        if ( RedrawToolbarRequest )
        {
            if ( (RedrawToolbarRequest >> 4) )
            {
                // System toolbar requested
                CurrentToolbar = &DefaultToolbar;
                if ( AUDIO_SpeakerOn )
                {
                    TOOLBAR_ChangeButton( 0, ButtonMenuBmp, fDefaultAction );       // restore the 'config' button
                    TOOLBAR_ChangeButton( 1, ButtonLowSoundLSBmp, fLowSound );      // restore the 'sound --' button
                    TOOLBAR_ChangeButton( 2, ButtonHighSoundLSBmp, fHighSound );    // restore the 'sound ++' button
                    TOOLBAR_ChangeButton( 3, AUDIO_IsMute() ?  ButtonMuteLSBmp : ButtonSoundLSBmp, fMuteSound );    // restore the 'mute' button
                }
                else
                {
                    TOOLBAR_ChangeButton( 0, ButtonMenuBmp, fDefaultAction );       // restore the 'config' button
                    TOOLBAR_ChangeButton( 1, ButtonLowSoundHPBmp, fLowSound );      // restore the 'sound --' button
                    TOOLBAR_ChangeButton( 2, ButtonHighSoundHPBmp, fHighSound );    // restore the 'sound ++' button
                    TOOLBAR_ChangeButton( 3, AUDIO_IsMute() ? ButtonMuteHPBmp : ButtonSoundHPBmp, fMuteSound ); // restore the 'mute' button
                }
            }
        
           // At least 1 button redraw requested
            if ( RedrawToolbarRequest & 0x0F)
            {
                // Redraw the buttons
                for ( i = 0 ; i < CurrentToolbar->NbItems ; i++ )
                {
                    if( (RedrawToolbarRequest >> i) & 1 )
                        TOOLBAR_UnSelectButton( i );
                }
            }
        
            // Requested treated
            RedrawToolbarRequest = 0;
        }   // End if( RedrawToolbarRequest )

        // Memorize state
        OldOrientation = CurOrientation;

        // No touch, no button selected
        ScreenButton = -1;

    } // End if TOUCHSCR_IsPressed

    // Button selection management
    if ( OldButton != ScreenButton )
    {
        if ( OldButton != -1 )
            TOOLBAR_UnSelectButton( OldButton );

        if ( ScreenButton != -1 )
            TOOLBAR_SelectButton( ScreenButton );
    }

}


/*******************************************************************************
*
*                                TOOLBAR_UnselectButton
*
*******************************************************************************/
/**
*   Draw the icon of a specified button with unselect colors.
*
*   @param[in] : button = index of the button to draw (0 to TB_NB_BUTTONS)
*
**/
/******************************************************************************/
void TOOLBAR_UnSelectButton( u16 button )
{
    TOOLBAR_Button( button, DEFAULT_BUTTON_BGND, DEFAULT_BUTTON_BGND, FALSE );
}

/*******************************************************************************
*
*                                TOOLBAR_SelectButton
*
*******************************************************************************/
/**
*   Draw the icon of a specified button with select colors.
*   Launch the associated function
*
*   @param[in] : button = index of the button to draw (0 to TB_NB_BUTTONS)
*
**/
/******************************************************************************/
void TOOLBAR_SelectButton( u16 button )
{
    s32 index;

    // Draw the button in selected mode
    TOOLBAR_Button( button, DEFAULT_BUTTON_BGND, ACTIVE_BUTTON_BGND, TRUE ) ;

    // Beep
    if ( BUZZER_GetMode() == BUZZER_OFF )
    {
        BUZZER_SetMode( BUZZER_SHORTBEEP );
    }

    // Launch the function corresponding to the absolute index of the item
    index = ( button + CurrentToolbar->FirstDispItem ) % CurrentToolbar->NbItems;
    if ( CurrentToolbar->Items[index].Fct_Manage )
    {
#if POWER_MNGT
        POWER_Reset_Time();
#endif
        CurrentToolbar->Items[index].Fct_Manage();
#if POWER_MNGT
        POWER_Set_Time();
#endif
    }
}

/*******************************************************************************
*
*                                TOOLBAR_Button
*
*******************************************************************************/
/**
*   Draw the icon of a specified button with specified colors.
*
*   @param[in] button : index of the button to draw (0 to TB_NB_BUTTONS)
*   @param[in] bgnd_color : background color of the icon button in not selected state
*   @param[in] bgnd_color_sel : background color of the icon button in selected state
*   @param[in] sel : indicates if the button in selected or not
*
**/
/******************************************************************************/
void TOOLBAR_Button( u16 button, u16 bgnd_color, u16 bgnd_color_sel, bool sel )
{
    s32 index, deltaWidth, deltaHeight, Icon_Width, Icon_Height;
    coord_t Save_Screen_Width, Save_Screen_Height, Save_LCD_Offset;

    // Find the absolute index of the item
    index = ( button + CurrentToolbar->FirstDispItem ) % CurrentToolbar->NbItems;

    // Find the size of the icon in the header (only for BMP icons)
    if ( ( CurrentToolbar->Items[index].icon[0] == 0x424D ) && ( CurrentToolbar->Items[index].icon[3] == 0) )
        {
        Icon_Width = (CurrentToolbar->Items[index].icon[9]) >> 8;
        Icon_Height = (CurrentToolbar->Items[index].icon[11]) >> 8;
        }
    else
        {
        Icon_Width = ICON_WIDTH;
        Icon_Height = ICON_HEIGHT;
        }
    
    // Calculate the size difference for center icons
    deltaWidth = ( BUTTON_WIDTH - Icon_Width ) / 2;
    deltaHeight = ( BUTTON_HEIGHT - Icon_Height ) / 2;

    // Save screen current virtual values, if application with offset running
    Save_Screen_Width = LCD_GetScreenWidth();
    Save_Screen_Height = LCD_GetScreenHeight();
    Save_LCD_Offset = LCD_Offset;

    // Restore real values for DRAW and LCD functions
    Screen_Width = APP_SCREEN_WIDTH;
    Screen_Height = APP_SCREEN_HEIGHT;
    LCD_Offset = 0;

    switch ( LCD_GetScreenOrientation() )
    {
    case V12:
        LCD_FillRect_Circle( BUTTON_WIDTH * button, APP_SCREEN_HEIGHT, BUTTON_WIDTH, BUTTON_HEIGHT, RGB_WHITE );

        if ( sel )
            DRAW_SetImageSel( CurrentToolbar->Items[index].icon, ( BUTTON_WIDTH * button ) + deltaWidth, APP_SCREEN_HEIGHT + deltaHeight, Icon_Height, Icon_Width,
                              bgnd_color, bgnd_color_sel );
        else
            DRAW_SetImage( CurrentToolbar->Items[index].icon, ( BUTTON_WIDTH * button ) + deltaWidth, APP_SCREEN_HEIGHT + deltaHeight, Icon_Height, Icon_Width );

        DRAW_Line_Circle( BUTTON_WIDTH * button, APP_SCREEN_HEIGHT, BUTTON_WIDTH *( button + 1 ) - 1, APP_SCREEN_HEIGHT, RGB_BUTTON_SEPARATOR );

        if ( index != ( CurrentToolbar->NbItems - 1 ) )
            DRAW_Line_Circle( BUTTON_WIDTH *( button + 1 ) - 1, APP_SCREEN_HEIGHT, BUTTON_WIDTH *( button + 1 ) - 1, APP_SCREEN_HEIGHT + BUTTON_HEIGHT - 1, RGB_BUTTON_SEPARATOR );
        break;

    case V3:
        LCD_FillRect_Circle( APP_SCREEN_WIDTH, BUTTON_WIDTH *( CurrentToolbar->NbItems - button - 1 ), BUTTON_HEIGHT, BUTTON_WIDTH, RGB_WHITE );

        if ( sel )
            DRAW_SetImageSel( CurrentToolbar->Items[index].icon, APP_SCREEN_WIDTH + deltaHeight, BUTTON_WIDTH *( CurrentToolbar->NbItems - button - 1 ) + deltaWidth, Icon_Width, Icon_Height,
                              bgnd_color, bgnd_color_sel );
        else
            DRAW_SetImage( CurrentToolbar->Items[index].icon, APP_SCREEN_WIDTH + deltaHeight, BUTTON_WIDTH *( CurrentToolbar->NbItems - button - 1 ) + deltaWidth, Icon_Width, Icon_Height );

        DRAW_Line_Circle( APP_SCREEN_WIDTH, BUTTON_WIDTH *( CurrentToolbar->NbItems - button - 1 ), APP_SCREEN_WIDTH, BUTTON_WIDTH *( CurrentToolbar->NbItems - button ), RGB_BUTTON_SEPARATOR );

        if ( index != ( CurrentToolbar->NbItems - 1 ) )
            DRAW_Line_Circle( APP_SCREEN_WIDTH + BUTTON_HEIGHT, BUTTON_WIDTH *( CurrentToolbar->NbItems - button - 1 ), APP_SCREEN_WIDTH, BUTTON_WIDTH *( CurrentToolbar->NbItems - button - 1 ), RGB_BUTTON_SEPARATOR );
        break;

    case V6:
        LCD_FillRect_Circle( BUTTON_WIDTH*( CurrentToolbar->NbItems - button - 1 ), -BUTTON_HEIGHT, BUTTON_WIDTH, BUTTON_HEIGHT, RGB_WHITE );

        if ( sel )
            DRAW_SetImageSel( CurrentToolbar->Items[index].icon, ( BUTTON_WIDTH *( CurrentToolbar->NbItems - button - 1 ) ) + deltaWidth, -BUTTON_HEIGHT + deltaHeight, Icon_Height, Icon_Width,
                              bgnd_color, bgnd_color_sel );
        else
            DRAW_SetImage( CurrentToolbar->Items[index].icon, ( BUTTON_WIDTH *( CurrentToolbar->NbItems - button - 1 ) ) + deltaWidth, -BUTTON_HEIGHT + deltaHeight,  Icon_Height, Icon_Width );

        DRAW_Line_Circle( BUTTON_WIDTH *( CurrentToolbar->NbItems - button - 1 ), -1,  BUTTON_WIDTH *( CurrentToolbar->NbItems - button ), -1, RGB_BUTTON_SEPARATOR );

        if ( index != ( CurrentToolbar->NbItems - 1 ) )
            DRAW_Line_Circle( BUTTON_WIDTH *( CurrentToolbar->NbItems - button - 1 ), -BUTTON_HEIGHT, BUTTON_WIDTH *( CurrentToolbar->NbItems - button - 1 ), -1, RGB_BUTTON_SEPARATOR );
        break;

    case V9:
        LCD_FillRect_Circle( -BUTTON_HEIGHT, BUTTON_WIDTH * button, BUTTON_HEIGHT, BUTTON_WIDTH, RGB_WHITE );

        if ( sel )
            DRAW_SetImageSel( CurrentToolbar->Items[index].icon, -BUTTON_HEIGHT + deltaHeight, ( BUTTON_WIDTH * button ) + deltaWidth, Icon_Height, Icon_Width,
                              bgnd_color, bgnd_color_sel );
        else
            DRAW_SetImage( CurrentToolbar->Items[index].icon, -BUTTON_HEIGHT  + deltaHeight, ( BUTTON_WIDTH * button ) + deltaWidth,  Icon_Height, Icon_Width );

        DRAW_Line_Circle( -1, BUTTON_WIDTH * button, -1, BUTTON_WIDTH *( button + 1 ) - 1, RGB_BUTTON_SEPARATOR );

        if ( index != ( CurrentToolbar->NbItems - 1 ) )
            DRAW_Line_Circle( -1, BUTTON_WIDTH *( button + 1 ) - 1, -BUTTON_HEIGHT, BUTTON_WIDTH *( button + 1 ) - 1, RGB_BUTTON_SEPARATOR );
        break;
    }

    // Restore virtual values for DRAW and LCD functions
    Screen_Width = Save_Screen_Width;
    Screen_Height = Save_Screen_Height;
    LCD_Offset = Save_LCD_Offset;
}

/*******************************************************************************
*
*                                Default system functions
*
*******************************************************************************/
enum MENU_code fLowSound( void )
{
    AUDIO_Dec_Volume( 2 );
    return MENU_CONTINUE;
}

enum MENU_code fHighSound( void )
{
    AUDIO_Inc_Volume( 2 );
    return MENU_CONTINUE;
}

enum MENU_code fMuteSound( void )
{
    AUDIO_MUTE_OnOff( AUDIO_IsMute() ? 0 : 1 );

    if ( AUDIO_SpeakerOn )
        TOOLBAR_ChangeButton( 3, AUDIO_IsMute() ? ButtonMuteLSBmp : ButtonSoundLSBmp, fMuteSound );    // restore the 'mute' button
    else
        TOOLBAR_ChangeButton( 3, AUDIO_IsMute() ? ButtonMuteHPBmp : ButtonSoundHPBmp, fMuteSound );    // restore the 'mute' button

    return MENU_CONTINUE;
}

enum MENU_code fDefaultAction( void )
{
    index_t CurrentApplicationIndex = -1;

    if ( fInitDone  == FALSE )
        return MENU_CONTINUE;

    // Retrieve the current appli index
    CurrentApplicationIndex = UTIL_ReadBackupRegister( BKP_SYS1 );

    // Try to launch the current appli
    if ( APP_LaunchAppli( CurrentApplicationIndex ) == MENU_LEAVE )
    {
        return fConfig();
    }
    else
    {
        return MENU_CONTINUE;
    }
}

/*******************************************************************************
*
*                                TOOLBAR_RedrawToolbar
*
*******************************************************************************/
/**
*   Request a toolbar redraw.
*
*   @param[in]  Request    Redraw request : bit 0 = button 0,
*                                           bit 1 = button 1,
*                                           bit 2 = button 2,
*                                           bit 3 = button 3,
*                                           bit 4 = redraw with default toolbar,
*                                           0 = nothing
*
**/
/******************************************************************************/
void TOOLBAR_RedrawToolbar( s32 Request )
{
    RedrawToolbarRequest |= Request;
}

/// @endcond

/*******************************************************************************
*
*                                TOOLBAR_Set
*
*******************************************************************************/
/**
*   Change the current toolbar, with new the specified new one .
*
*   @param[in] NewToolbar : pointer on new toolbar structure
*
**/
/******************************************************************************/
void TOOLBAR_Set( const tToolbar* NewToolbar )
{
    CurrentToolbar = ( tToolbar* ) NewToolbar;
    TOOLBAR_RedrawToolbar( TOOLBAR_REDRAW );         // To redraw...
}

/*******************************************************************************
*
*                                TOOLBAR_SetDefaultToolbar
*
*******************************************************************************/
/**
*   Restore the default system toolbar.
*
*
**/
/******************************************************************************/
void TOOLBAR_SetDefaultToolbar( void )
{
    TOOLBAR_RedrawToolbar(TOOLBAR_DEFAULT);
}

/*******************************************************************************
*
*                                TOOLBAR_ChangeButton
*
*******************************************************************************/
/**
*   Change a single button within the current toolbar.
*
*   @param[in] button   : index of the button to change
*   @param[in] *newicon : pointer on new icone associated to the button
*   @param[in] *newfct  : pointer on new function associated to the button
*
**/
/******************************************************************************/
void TOOLBAR_ChangeButton( s32 button, const u16* newicon, enum MENU_code( *newfct )( void ) )
{
    CurrentToolbar->Items[button].icon       = newicon;
    CurrentToolbar->Items[button].Fct_Manage = newfct;
    TOOLBAR_RedrawToolbar( 1 << button );
}



