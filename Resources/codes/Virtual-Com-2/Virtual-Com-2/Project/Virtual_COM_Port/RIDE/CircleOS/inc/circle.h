/****************** COPYRIGHT (C) 2007-2010 RAISONANCE S.A.S. *****************/
/**
*
* @file     circle.h
* @brief    General header for the CircleOS project.
* @author   FL
* @date     07/2007
* @version  1.5
*
* It contains the list of the utilities functions organized by sections
* (MEMS, LCD, POINTER, ...)
*
* @date     10/2007
* @version  1.5 types of OutX_F64 and OutX_F256 changed to u32 (same for Y and Z)
* @date     10/2008
* @version  3.0 add Primer 2 and ST lib v2.0.3
* @date     08/2009
* @version  3.8 add ST lib v3.1.0
* @date     09/2009
* @version  4.0 add Open 4
* @date     08/2010
* @version  4.1 add new graphical and font functions
* @date     02/2011
* @version  4.2 add STM32L and STM3220G platforms
**/
/******************************************************************************/

#include "Circle_platform.h"


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CIRCLE_H
#define __CIRCLE_H
//typedef enum {FALSE = 0, TRUE = !FALSE} bool;


/*------------------------------ General -------------------------------------*/
// This macro is a helper for counting the number of elements in an array
#define COUNTOF(array) (sizeof(array) / sizeof((array)[0]))

// General-use min and max macros
#define MAX(i,j) (((i)>(j))?(i):(j))
#define MIN(i,j) (((i)<(j))?(i):(j))

// This macro helps removing compiler warnings for unreferenced parameter
#define UNREFERENCED_PARAMETER(c) ((c)=(c))

typedef void ( *tHandler )( void );
extern tHandler SchHandler[16+1];

/* Variables  ----------------------------------------------------------------*/
extern volatile bool fFirstStartup;  /* Indicates whether we are starting on a first powerup */
/* after switching the battery*/
extern volatile bool fIsStandAlone;  /* Indicates whether we are connected to base or in standalone mode */

extern volatile delay_t TimingDelay;
extern volatile bool fInitDone;
extern bool AutorunOn;               /* Application autorun or not */


/*CurrentSpeed is dynamically defined by the user (config menu).*/
extern enum eSpeed
{
    SPEED_VERY_LOW    = 1,
    SPEED_LOW         = 2,
    SPEED_MEDIUM      = 3,
    SPEED_HIGH        = 4,
    SPEED_VERY_HIGH   = 5
                    } CurrentSpeed;

enum eSchHandler
{
#if (MEMS_POINTER && !MEMS_SPEC_MNGT)     
    MEMS_SCHHDL_ID     = 0,
#else
    UNUSED0_SCHHDL_ID  = 0,
#endif
    LED_SCHHDL_ID      = 1,
    BUTTON_SCHHDL_ID   = 2,
    BUZZER_SCHHDL_ID   = 3,
    MENU_SCHHDL_ID     = 4,
    POINTER_SCHHDL_ID  = 5,
    LCD_SCHHDL_ID      = 6,
    DRAW_SCHHDL_ID     = 7,
    RTC_SCHHDL_ID      = 8,

#if AUDIO_AVAIL
    AUDIO_SCHHDL_ID    = 9,
#else
    UNUSED1_SCHHDL_ID  = 9,
#endif

#if TOUCHSCREEN_AVAIL
    TOUCHSCR_SCHHDL_ID = 10,
    TOOLBAR_SCHHDL_ID  = 11,
#else
    UNUSED2_SCHHDL_ID  = 10,
    UNUSED3_SCHHDL_ID  = 11,
#endif

#if POWER_MNGT
    POWER_SCHHDL_ID    = 12,
#else
    UNUSED4_SCHHDL_ID  = 12,
#endif

    UNUSED5_SCHHDL_ID = 13,
    UNUSED6_SCHHDL_ID = 14,
    UNUSED7_SCHHDL_ID = 15,
    UNUSED8_SCHHDL_ID = 16
//   } dummy_var; /*for doxygen*/ // YRT20100107 for STM8 compatibility
};

#define NULL_SCHHDL (0)
#define LAST_SCHHDL ((tHandler)(-1))

/* Versioning ----------------------------------------------------------------*/
#define  STR_OSVERSION  "V 4.2"             /*!< CircleOS version string. */
#define  OSVERSION      0x00042000          /*!< CircleOS version number: 0x00MM00mm where MM is the major number and mm the minor number. */

/* Scheduler -----------------------------------------------------------------*/
void        SysTick_Handler( void );

/* Utilities -----------------------------------------------------------------*/

// Non-exported functions (not in the API)
void        vbattoa( u8* ptr, u16 X );
void        Delayms( delay_t count );
void        starting_delay( void );
void        SysTick_Configuration( void );
void        POWER_Init( void );
void        Sleep_Call( void );

/* UTIL ----------------------------------------------------------------------*/
void        UTIL_uint2str( u8* ptr, uint_t X, len_t digit, bool fillwithzero );
void        UTIL_int2str( u8* ptr, int_t X, len_t digit, bool fillwithzero );
bool        UTIL_isBackupRegisterConfigured( void );
backup_t    UTIL_ReadBackupRegister( index_t BKP_DR );
void        UTIL_WriteBackupRegister( index_t BKP_DR, backup_t Data );
u16         UTIL_GetBat( void );
u8          UTIL_GetUsb( void );
u16         UTIL_GetTemp( void );
void        UTIL_SetTempMode( bool mode );
void        UTIL_SetIrqHandler( s32, tHandler );
tHandler    UTIL_GetIrqHandler( s32 );
void        UTIL_SetSchHandler( enum eSchHandler, tHandler );
tHandler    UTIL_GetSchHandler( enum eSchHandler );
void        UTIL_SetPll( enum eSpeed speed );
const u8*   UTIL_GetVersion( void );
enum eSpeed UTIL_GetPll( void );
u16         UTIL_GetPrimerType( void );
uint_t      UTIL_GetAppAddress( const u8* AppName );
void        UTIL_LoadApp( index_t app );
bool        UTIL_IsStandAloneMode(void);

extern bool      fTemperatureInFahrenheit;  /*!< 1 : Fahrenheit, 0 : Celcius (default). */

/* MEMS ----------------------------------------------------------------------*/
typedef enum
{
    V12 = 0, // upright
    V3 = 1,  // on the right side
    V6 = 2,  // upside down
    V9 = 3,  // on the left side
    V12BMP = 4,
    V3BMP = 5,
    V6BMP = 6,
    V9BMP = 7
} Rotate_H12_V_Match_TypeDef;

#if MEMS_FULLINFO
typedef struct
{
    s16 OutX;
    s16 OutX_F4;
    s16 OutX_F16;
    s32 OutX_F64;
    s32 OutX_F256;
    s16 OutY;
    s16 OutY_F4;
    s16 OutY_F16;
    s32 OutY_F64;
    s32 OutY_F256;
    s16 OutZ;
    s16 OutZ_F4;
    s16 OutZ_F16;
    s32 OutZ_F64;
    s32 OutZ_F256;
    s16 Shocked;
    s16 RELATIVE_X;
    s16 RELATIVE_Y;
    s16 DoubleClick;
} tMEMS_Info;
#else
typedef struct
{
    s16 OutX;
    s16 OutY;
    s16 OutZ;
    s16 RELATIVE_X;
    s16 RELATIVE_Y;
    bool DoubleClick;
    bool Shocked;
} tMEMS_Info;
#endif

extern tMEMS_Info MEMS_Info;
extern coord_t XInit;
extern coord_t YInit;

void            MEMS_Init( void );
void            MEMS_Handler( void );
void            MEMS_GetPosition( coord_t* pX, coord_t* pY );
void            MEMS_SetNeutral( void );
void            MEMS_GetRotation( Rotate_H12_V_Match_TypeDef* H12 );
tMEMS_Info*     MEMS_GetInfo( void );
u8              MEMS_ReadID( void );


/* ADC -----------------------------------------------------------------------*/
void ADConverter_Init( void );


/* Shutdown ------------------------------------------------------------------*/
void SHUTDOWN_Action( void );

/* BUTTON --------------------------------------------------------------------*/
enum BUTTON_mode
{
    BUTTON_DISABLED         = -1,
    BUTTON_ONOFF            = 0,
    BUTTON_ONOFF_FORMAIN    = 1,
    BUTTON_WITHCLICK        = 2
};

enum BUTTON_state
{
    BUTTON_UNDEF            = -1,
    BUTTON_RELEASED         = 0,
    BUTTON_PUSHED           = 1,
    BUTTON_PUSHED_FORMAIN   = 2,
    BUTTON_CLICK            = 3,
    BUTTON_DBLCLICK         = 4
};

void            BUTTON_Init( void );
void            BUTTON_Handler( void );
enum            BUTTON_state BUTTON_GetState( void );
void            BUTTON_SetMode( enum BUTTON_mode mode );
enum            BUTTON_mode BUTTON_GetMode( void );
void            BUTTON_WaitForRelease( void );
u8              BUTTON_GetNewState( void );

/* LED -----------------------------------------------------------------------*/
#if LED_INV
#define BIT_LED_SET Bit_RESET
#define BIT_LED_RESET Bit_SET
#else
#define BIT_LED_SET Bit_SET
#define BIT_LED_RESET Bit_RESET
#endif

/* Exported types ------------------------------------------------------------*/
enum LED_mode 
{ 
    LED_UNDEF = -1,
    LED_Off = 0,
    LED_On = 1,
    LED_BLINKING_LF = 2,
    LED_BLINKING_HF = 3
};
enum LED_id { LED_GREEN = 0, LED_RED = 1};

void LED_Init( void );
void LED_Handler_hw( enum LED_id id );
void LED_Handler( void );
void LED_Set( enum LED_id id, enum LED_mode mode );
/* JOYSTICK ------------------------------------------------------------------*/
#define  ANTI_BOUNCING_COUNT     25

enum JOYSTICK_state
{
    JOYSTICK_UNDEF      = -1,
    JOYSTICK_RELEASED   = 0,
    JOYSTICK_LEFT       = 1,
    JOYSTICK_UP         = 2,
    JOYSTICK_RIGHT      = 3,
    JOYSTICK_DOWN       = 4,
    JOYSTICK_RIGHT_UP   = 11,  /*JOYSTICK_RIGHT | JOYSTICK_UP*/
    JOYSTICK_LEFT_UP    = 5,   /*JOYSTICK_LEFT  | JOYSTICK_UP*/
    JOYSTICK_RIGHT_DOWN = 13,  /*JOYSTICK_RIGHT | JOYSTICK_DOWN*/
    JOYSTICK_LEFT_DOWN  = 7    /*JOYSTICK_LEFT  | JOYSTICK_DOWN */
};

enum JOYSTICK_state JOYSTICK_GetState( void );
void                JOYSTICK_WaitForRelease( void );
enum JOYSTICK_state JOYSTICK_GetNewState( void );
enum JOYSTICK_state JOYSTICK_CircularPermutation( enum JOYSTICK_state abs_direction, s8 iter );

/* POINTER -------------------------------------------------------------------*/
enum POINTER_mode  { POINTER_UNDEF  = -1, POINTER_OFF = 0, POINTER_ON = 1, POINTER_MENU  = 2, POINTER_APPLICATION = 3, POINTER_RESTORE_LESS = 4 };

enum POINTER_state { POINTER_S_UNDEF  = -1,  POINTER_S_DISABLED = 0, POINTER_S_ENABLED = 1 };

#define POINTER_WIDTH 7

typedef struct
{
    coord_t xPos;
    coord_t yPos;
    coord_t shift_PosX;
    coord_t shift_PosY;
    coord_t X_PosMin;
    coord_t Y_PosMin;
    coord_t X_PosMax;
    coord_t Y_PosMax;
} tPointer_Info;

extern tPointer_Info POINTER_Info;
extern CONST_DATA u8 BallPointerBmp[POINTER_WIDTH];
extern const u8* CurrentPointerBmp;
extern color_t CurrentPointerColor;
extern color_t BallColor;
extern coord_t POINTER_X_PosMin;
extern coord_t POINTER_Y_PosMin;
extern coord_t POINTER_X_PosMax;
extern coord_t POINTER_Y_PosMax;
extern coord_t CurrentPointerWidth;
extern coord_t CurrentPointerHeight;

void                POINTER_Init( void );
void                POINTER_Handler( void );
s16                 POINTER_GetCurrentAngleStart( void );
void                POINTER_SetCurrentAngleStart( u16 );
u16                 POINTER_GetCurrentSpeedOnAngle( void );
void                POINTER_SetCurrentSpeedOnAngle( u16 newspeed );
void                POINTER_SetMode( enum POINTER_mode mode );
void                POINTER_SetCurrentPointer( coord_t width, coord_t height, const u8* bmp );
enum                POINTER_state POINTER_GetState( void );
void                POINTER_Draw( coord_t Line, coord_t Column, coord_t Width, coord_t Height, const u8* Bmp );
void                POINTER_SetRect( coord_t x, coord_t y, coord_t width, coord_t height );
void                POINTER_SetRectScreen( void );
void                POINTER_Save( coord_t Line, coord_t Column, coord_t Width, coord_t Height );
void                POINTER_Restore( coord_t Line, coord_t Column, coord_t Width, coord_t Height );
u16                 POINTER_GetPos( void );
s32                 TOUCHSCR_GetPosX( void );
s32                 TOUCHSCR_GetPosY( void );
void                POINTER_SetPos( coord_t x, coord_t y );
void                POINTER_SetApplication_Pointer_Mgr( tAppPtrMgr mgr );
tPointer_Info*      POINTER_GetInfo( void );
color_t             POINTER_GetColor( void );
void                POINTER_SetColor( color_t color );
enum POINTER_mode   POINTER_GetMode( void );
void                POINTER_SetCurrentAreaStore( u8* ptr );

extern bool JoystickAsInput;
extern bool MemsAsInput;
extern bool TchscrAsInput;

/* LCD -----------------------------------------------------------------------*/
/* Characters Infos */
#define CHAR_WIDTH            7         // Default values for compatibility
#define CHAR_HEIGHT           14        // Default values for compatibility

/* PWM rates. */
#define BACKLIGHTMIN                0x1000   /*!< Minimal PWM rate.           */
#define DEFAULT_CCR_BACKLIGHTSTART  0x8000   /*!< Default PWM rate.           */

// Major HTML Color codes
#define RGB_RED         RGB_MAKE(0xFF, 0x00, 0x00)  //0x00F8
#define RGB_GREEN       RGB_MAKE(0x00, 0xFF, 0x00)  //0xE007 (official name is PASTELGREEN)
#define RGB_PASTELGREEN RGB_MAKE(0x00, 0xFF, 0x00)
#define RGB_LIGHTBLUE   RGB_MAKE(0x00, 0x00, 0xFF)  //0x1F00 (was 0xFF07 in CircleOS 3)
#define RGB_BLUE        RGB_MAKE(0x00, 0x00, 0xFF)  //0x1F00 (official name is LIGHTBLUE)
#define RGB_BLACK       RGB_MAKE(0x00, 0x00, 0x00)  //0x0000
#define RGB_WHITE       RGB_MAKE(0xFF, 0xFF, 0xFF)  //0xFFFF
#define RGB_YELLOW      RGB_MAKE(0xFF, 0xFF, 0x00)  //0xE0FF
#define RGB_MAGENTA     RGB_MAKE(0xFF, 0x00, 0xFF)  //0x1FF8
#define RGB_TURQUOISE   RGB_MAKE(0x00, 0xFF, 0xFF)
#define RGB_ORANGE      RGB_MAKE(0xFF, 0x80, 0x40)  //0x08FC (was 0xE0F9 in CircleOS 3)
#define RGB_PINK        RGB_MAKE(0xFF, 0x3F, 0xFF)  //0xFFF9
#define RGB_LIGHTGREY   RGB_MAKE(0xC0, 0xC0, 0xC0)
#define RGB_DARKGREY    RGB_MAKE(0x80, 0x80, 0x80)
#define RGB_DARKBLUE    RGB_MAKE(0x00, 0x00, 0xA0)
#define RGB_LIGHTPURPLE RGB_MAKE(0xFF, 0x00, 0x80)
#define RGB_DARKPURPLE  RGB_MAKE(0x80, 0x00, 0x80)
#define RGB_BROWN       RGB_MAKE(0x80, 0x40, 0x00)
#define RGB_BURGUNDY    RGB_MAKE(0x80, 0x00, 0x00)
#define RGB_FORESTGREEN RGB_MAKE(0x80, 0x80, 0x00)
#define RGB_GRASSGREEN  RGB_MAKE(0x40, 0x80, 0x80)

extern coord_t Screen_Width;
extern coord_t Screen_Height;
extern ENUM_Offset LCD_Offset;
extern const u8* CurrentFont;
extern u8 Char_Width;
extern u8 Char_Height;
extern color_t TextColor;
extern color_t BGndColor;
extern CONST_DATA tFontTable Font_Table;
extern u8 Transparency;

void                LCD_Init( void );
void                LCD_Handler( void );
void                LCD_Reset( void );
void                LCD_Interface_Init( void );
color_t             LCD_GetPixel( coord_t x, coord_t y );
void                LCD_DrawPixel( coord_t x, coord_t y, color_t Pixel );
void                LCD_SetRect_For_Cmd( coord_t x, coord_t y, coord_t width, coord_t height );
lcdt_t              LCD_ReadLCDData( void );
void                LCD_RectRead( coord_t x, coord_t y, coord_t width, coord_t height, u8* bmp );
void                LCD_SendLCDCmd( u8 Cmd );
void                LCD_SendLCDData( u8 Data );
void                LCD_SendLCDData16( u8 Data_h, u8 Data_l );
void                LCD_SendLCDCmd_RAM_Access( void );
void                LCD_SendLCDCmd_RAM_Access_End( void );
void                LCD_SendLCDCmd_Rotation_Access( void );
void                LCD_FillRect( coord_t x, coord_t y, coord_t width, coord_t height, color_t color );
void                LCD_FillRect_Circle( coord_t x, coord_t y, coord_t width, coord_t height, color_t color );
void                LCD_DrawRect( coord_t x, coord_t y, coord_t width, coord_t height, color_t color );
void                LCD_DisplayChar( coord_t x, coord_t y, u8 ascii, color_t textColor, color_t bGndColor, mag_t charMagCoeff );
void                LCD_SetRotateScreen( bool RotateScreen );
bool                LCD_GetRotateScreen( void );
void                LCD_SetScreenOrientation( Rotate_H12_V_Match_TypeDef ScreenOrientation );
Rotate_H12_V_Match_TypeDef LCD_GetScreenOrientation( void );
void                LCD_DrawCharSetFilter( coord_t xmin, coord_t xmax, coord_t ymin, coord_t ymax );
void                LCD_DrawChar( coord_t x, coord_t y, coord_t width, const u8* bmp, color_t textColor, color_t bGndColor, mag_t charMagniCoeff );
void                LCD_BackLightChange( void );
void                LCD_BackLightConfig( void );
void                LCD_SetBackLight( backlight_t newBacklightStart );
backlight_t         LCD_GetBackLight( void );
void                LCD_SetBackLightOff( void );
void                LCD_SetBackLightOn( void );
void                LCD_SetFont( const u8* NewFont );
void                LCD_SetFontDef(  tFontDef* FontDef );
tFontDef*           LCD_GetFontDef( void );
void                LCD_SetDefaultFont( void );
void                LCD_ChangeFont( enum ENUM_FontID ID );
void                LCD_Batt( coord_t, coord_t, bool, u16, u16* , divider_t, coord_t*, coord_t* );
void                LCD_Clear( u16, u16 );
void                LCD_SetLogoBW( void );
void                LCD_Scroll( u8 Ascii, coord_t, coord_t*, coord_t, coord_t, coord_t, coord_t, color_t, color_t, mag_t );
void                LCD_SetOffset( ENUM_Offset );
coord_t             LCD_GetScreenWidth( void );
coord_t             LCD_GetScreenHeight( void );
void                LCD_DisplayRotate( Rotate_H12_V_Match_TypeDef H12 );
void                LCD_ClearAllScreen( color_t color );
void                LCD_SetTransparency( u8 NewTransparency );
u8                  LCD_GetTransparency( void );
void                LCD_FullScreen( bool FullScreenONOFF );
u16                 LCD_Read_Reg( u8 regadd );
inline void         LCD_Write_Reg( u8 regadd, u16 value );
inline void         LCD_PixelRead( u8** ptbmp );

/* DRAW ----------------------------------------------------------------------*/
enum Align { LEFT = 0, CENTER = 1, RIGHT = 2 };
#define INVERTED_TEXT   1           /*!< Text is displayed inverted.          */
#define NORMAL_TEXT     0           /*!< Text is displayed normaly.           */
#define ALL_SCREEN      -1          /* Text is displayed automatically on all the screen width */
extern bool                         CurrentRotateScreen;
extern Rotate_H12_V_Match_TypeDef   CurrentScreenOrientation;
extern uint_t                       Current_CCR_BackLightStart;
extern mag_t                        CharMagniCoeff;

void                DRAW_Init( void );
void                DRAW_Clear( void );
void                DRAW_Handler( void );
void                DRAW_SetDefaultColor( void );
void                DRAW_SetImage( const color_t* imageptr, coord_t x, coord_t y, coord_t width, coord_t height );
void                DRAW_SetImageSel( const color_t* imageptr, coord_t x, coord_t y, coord_t width, coord_t height, color_t oldBgndColor, color_t newBgndColor );
void                DRAW_SetImageBW( const u8* imageptr, coord_t x, coord_t y, coord_t width, coord_t height );
void                DRAW_SetLogoBW( void );
void                DRAW_DisplayVbat( coord_t x, coord_t y );
void                DRAW_DisplayTime( coord_t x, coord_t y );
void                DRAW_DisplayTemp( coord_t x, coord_t y );
void                DRAW_DisplayString( coord_t x, coord_t y, const u8* ptr, len_t len );
void                DRAW_DisplayStringInverted( coord_t x, coord_t y, const u8* ptr, len_t len );
void                DRAW_DisplayStringWithMode( coord_t x, coord_t y, const u8* ptr, len_t len, bool mode, enumset_t align );
mag_t               DRAW_GetCharMagniCoeff( void );
void                DRAW_SetCharMagniCoeff( mag_t Coeff );
color_t             DRAW_GetTextColor( void );
void                DRAW_SetTextColor( color_t Color );
color_t             DRAW_GetBGndColor( void );
void                DRAW_SetBGndColor( color_t Color );
void                DRAW_Batt( void );
void                DRAW_Line( coord_t x1, coord_t y1, coord_t x2, coord_t y2, color_t color );
void                DRAW_Putc( u8 Ascii );
void                DRAW_Puts( const u8* ptr );
void                DRAW_SetCursorPos( coord_t x, coord_t y );
ret_t               DRAW_GetCursorPos( void );
void                DRAW_SetCursorMargin( coord_t lx, coord_t rx, coord_t hy, coord_t ly );
void                DRAW_GetCursorMargin( coord_t* lx, coord_t* rx, coord_t* hy, coord_t* ly );
void                DRAW_RestoreCharMagniCoeff( void );
color_t             DRAW_GetIndexedColorBMP( u8* color_table, u16 i );
void                DRAW_Circle(coord_t CENTER_x, coord_t CENTER_y, coord_t RADIUS_r, color_t Color, color_t Fill_Color, bool Fill, bool Circle);
void                DRAW_Ellipse( coord_t CENTER_x, coord_t CENTER_y, coord_t RADIUS_a, coord_t RADIUS_b, color_t Color, color_t Fill_Color, bool Fill, bool Ellipse);
void                DRAW_Polygon(coord_t *points, u16 NUM_Points, color_t Line_Color);

extern bool fDisplayTime;
extern coord_t PosCurX;         /*!< Current cursor X position */
extern coord_t PosCurY;         /*!< Current cursor Y position */
extern coord_t LeftMarginX;     /*!< Current left margin in X  */
extern coord_t RightMarginX;    /*!< Current right margin in X */
extern coord_t HighMarginY;     /*!< Current left margin in Y  */
extern coord_t LowMarginY;      /*!< Current right margin in Y */

/* BUZZER --------------------------------------------------------------------*/
#define BUZZER_BEEP  BUZZER_SHORTBEEP

enum BUZZER_mode
{
    BUZZER_UNDEF            = -1,
    BUZZER_OFF              = 0,
    BUZZER_ON               = 1,
    BUZZER_SHORTBEEP        = 2,
    BUZZER_LONGBEEP         = 3,
    BUZZER_PLAYMUSIC        = 4
};

extern counter_t Buzzer_Counter;

void                BUZZER_Init( void );
void                BUZZER_Handler( void );
void                BUZZER_SetMode( enum BUZZER_mode mode );
enum BUZZER_mode    BUZZER_GetMode( void );
void                BUZZER_PlayMusic( const u8* melody );
void                BUZZER_PlayNote( void );
void                BUZZER_SetFrequency( u16 freq );

/* AUDIO ---------------------------------------------------------------------*/
typedef enum { OFF, ON } ON_OFF_enum;
typedef enum { AUDIO_MODE, VOICE_MODE, AUDIO_CIRCULAR_MODE, VOICE_CIRCULAR_MODE }  AUDIO_DeviceMode_enum;
typedef enum { NO_SOUND, IS_PLAYING }                           AUDIO_Playback_status_enum;
typedef enum { NO_RECORD, IS_RECORDING }                        AUDIO_Recording_status_enum;
typedef enum { LG_8_BITS, LG_16_BITS}                           AUDIO_Length_enum;
typedef enum { FRQ_48KHZ , FRQ_44KHZ , FRQ_22KHZ , FRQ_16KHZ , FRQ_8KHZ } AUDIO_Frequency_enum;
typedef enum { MONO, STEREO }                                   AUDIO_Format_enum;
typedef enum { FULL = 0, LOW_EMPTY = 1, HIGH_EMPTY = 2 }                AUDIO_PlaybackBuffer_Status;
typedef enum { EMPTY = 0, LOW_FULL = 1, HIGH_FULL = 2 }                 AUDIO_RecordBuffer_Status;
typedef s8 sound_type;
typedef u16 voice_type;

extern volatile s8   flagWrite_AUDIO_CODEC_CRs; /* flag to manage I2C codec registers write*/
extern volatile u8   AUDIO_CODEC_CRs[];         /* audio codec I2C registers*/

extern bool AUDIO_SpeakerOn;           /* loudspeaker active or not*/
extern bool AUDIO_BuzzerOn;            /* Buzzer active or not*/
extern bool AUDIO_Mute;                /* sound has been disabled or not*/
extern const sound_type* Audio_buffer;
extern volatile s32 Audio_buffer_index;
extern volatile u16 Audio_buffer_size;
extern volatile AUDIO_PlaybackBuffer_Status  audio_buffer_fill;
extern AUDIO_PlaybackBuffer_Status bufferstatus_local;
extern voice_type*   Voice_buffer;
extern volatile u16  Voice_buffer_index;
extern volatile u16  Voice_buffer_size;
extern volatile AUDIO_RecordBuffer_Status voice_record_buffer_fill;
extern AUDIO_Playback_status_enum AUDIO_Playback_status;
extern AUDIO_Recording_status_enum AUDIO_Recording_status;
extern AUDIO_Length_enum AUDIO_Length;
extern AUDIO_Frequency_enum AUDIO_Frequency;
extern AUDIO_Format_enum AUDIO_Format;
extern s8 AUDIO_Volume;

void                    AUDIO_Init( void );
void                    AUDIO_Handler( void );
void                    AUDIO_Welcome_Msg( void );
void                    AUDIO_SetMode( AUDIO_DeviceMode_enum, AUDIO_Length_enum, AUDIO_Frequency_enum, AUDIO_Format_enum );
AUDIO_DeviceMode_enum   AUDIO_GetMode( void );
AUDIO_Playback_status_enum AUDIO_Playback_GetStatus( void );
AUDIO_Recording_status_enum AUDIO_Recording_GetStatus( void );
void                    AUDIO_Play( const sound_type*, s32 );
void                    AUDIO_Record( sound_type*, s32 );
void                    AUDIO_BUZZER_SetToneFrequency( u16 f );
void                    AUDIO_MUTE_OnOff( ON_OFF_enum mode );
bool                    AUDIO_IsMute( void );
void                    AUDIO_SPEAKER_OnOff( ON_OFF_enum mode );
void                    AUDIO_Inc_Volume( u8 dB );
void                    AUDIO_Dec_Volume( u8 dB );
void                    AUDIO_Set_Volume( void );
void                    AUDIO_Shutdown( void );
void                    AUDIO_Cpy_Mono( void );

#define SET_FLAG_WRITE_CODEC_CRS(x)   { flagWrite_AUDIO_CODEC_CRs = ( flagWrite_AUDIO_CODEC_CRs == -1 ) ? (x) : NB_REG; }
u8                      AUDIO_ReadRegister( u8 register_to_read );
void                    AUDIO_WriteRegister( u8 register_to_write, u8 data_to_write );
void                    AUDIO_Playback_Stop( void );
void                    AUDIO_Record_Stop( void );
AUDIO_PlaybackBuffer_Status AUDIO_PlaybackBuffer_GetStatus( AUDIO_PlaybackBuffer_Status value );
AUDIO_RecordBuffer_Status AUDIO_RecordBuffer_GetStatus( AUDIO_RecordBuffer_Status value );
void                    AUDIO_SetLocalBufferSize( s32 size );

/* MENU ----------------------------------------------------------------------*/
#define REMOVE_MENU  0x01
#define APP_MENU     0x02

// The MENU_code is sometimes forward-defined in platform.h, depending on the
// platform requirements. The MENU_CODE_DEFINED macro avoid its redefinition.
#ifndef _MENU_CODE_DEFINED
#define _MENU_CODE_DEFINED
enum  MENU_code
{
    MENU_LEAVE              = 0,
    MENU_CONTINUE           = 1,
    MENU_REFRESH            = 2,
    MENU_CHANGE             = 3,
    MENU_CONTINUE_COMMAND   = 4,
    MENU_LEAVE_AS_IT        = 5,
    MENU_RESTORE_COMMAND    = 6
};

typedef struct
{
    const u8* Text;
    enum MENU_code( *Fct_Init )( void );
    enum MENU_code( *Fct_Manage )( void );
    enumset_t fMenuFlag;
} tMenuItem;

#endif //_MENU_CODE_DEFINED

typedef struct
{
    bool fdispTitle;
    const u8* Title;
    index_t   NbItems;
    len_t     LgMax;
    coord_n_t XPos;
    coord_n_t YPos;
    coord_n_t XSize;
    coord_n_t YSize;
    index_t   SelectedItem;
    tMenuItem Items[MENU_MAXITEM];
} tMenu;


extern tMenu SDcardMenu;
extern index_t CurrentApplication;
extern index_t LastApplication;
extern u8 Menu_Font;
extern bool MENU_BigFont;
extern tMenu MainMenu;
extern tMenu* CurrentMenu;
extern tMenuItem* CurrentCommand;
extern color_t BGndColor_Menu;
extern color_t TextColor_Menu;
extern index_t CurSel;                          // Index of the current selection in a menu
extern bool iToShutDown;

void            MENU_Init( void );
void            MENU_Set( tMenu* mptr );
void            MENU_Handler( void );
enum MENU_code  MENU_Quit( void );
void            MENU_Remove( void );
void            MENU_Question( const u8* str, bool* answer );
void            MENU_Print( const u8* str );
enum MENU_code  MENU_SetLevel_Ini( void );
enum MENU_code  MENU_SetLevel_Mgr( uint_t* value, uint_t value_range[5] );
void            MENU_ClearCurrentCommand( void );
void            MENU_SetLevelTitle( const u8* title );
void            MENU_SetTextColor( color_t TxtColor );
color_t         MENU_GetTextColor( void );
void            MENU_SetBGndColor( color_t BGndColor );
color_t         MENU_GetBGndColor( void );
void            MENU_ClearCurrentMenu( void );
void            MENU_SetAppliDivider( divider_t dividr );
void            MENU_RestoreAppliDivider( void );

/* BACKLIGHT -----------------------------------------------------------------*/
void BackLight_Configuration( void );
void ManageBackLight( void );
void BackLight_Change( void );

/* TOUCHSCREEN ---------------------------------------------------------------*/
typedef enum {TS_NORMAL, TS_DRAWING, TS_CALIBRATION, TS_POINTER} TOUCHSCR_Mode_enum;

typedef struct
{
    s32 xRaw;               /* non calibrated coordinates */
    s32 yRaw;
    s32 xAbsPos;            /* absolute coordinates*/
    s32 yAbsPos;
    s32 xPos;               /* orientation dependant coordinates*/
    s32 yPos;
    bool TouchPress;
    TOUCHSCR_Mode_enum Mode;
} tTOUCHSCR_Info;       /* position informations */

typedef struct
{
    s32 A;
    s32 B;
    s32 C;
    s32 D;
    s32 E;
    s32 F;
    s32 R;
} tTOUCHSCR_Cal;       /* calibration informations */

typedef struct
{
    s32 X;
    s32 Y;
} tPOINT;

extern tTOUCHSCR_Info TOUCHSCR_Info;

void TOUCHSCR_Init( void );
void TOUCHSCR_Handler( void );
u16  TOUCHSCR_GetPos( void );
u16  TOUCHSCR_GetAbsPos( void );
bool TOUCHSCR_IsPressed( void );
void TOUCHSCR_SetSensibility( u16 );
void TOUCHSCR_SetMode( TOUCHSCR_Mode_enum mode );
TOUCHSCR_Mode_enum TOUCHSCR_GetMode( void );
enum MENU_code TOUCHSCREEN_Calibration( void );

/* TOOLBAR -------------------------------------------------------------------*/
#if TOUCHSCREEN_AVAIL

#define DEFAULT_BUTTON_COLOUR   RGB_LIGHTGREY
#define DEFAULT_BUTTON_BGND     RGB_WHITE
#define ACTIVE_BUTTON_COLOUR    RGB_WHITE
#define ACTIVE_BUTTON_BGND      RGB_GREEN //RGB_MAGENTA
#define RGB_BUTTON_SEPARATOR    DEFAULT_BUTTON_COLOUR
#define TOOLBAR_REDRAW_BUTTON0  0x01
#define TOOLBAR_REDRAW_BUTTON1  0x02
#define TOOLBAR_REDRAW_BUTTON2  0x04
#define TOOLBAR_REDRAW_BUTTON3  0x08
#define TOOLBAR_REDRAW          0x0F
#define TOOLBAR_DEFAULT         0x10

typedef struct
{
    const u16* icon;
    enum MENU_code( *Fct_Manage )( void );
} tToolbarItem;

typedef struct
{
    s32 NbItems;
    u32 FirstDispItem;            /* index of the first displayed item (if > NBBUTTONS)*/
    tToolbarItem Items[TOOLBAR_MAXITEMS];
} tToolbar;
extern tToolbar*         CurrentToolbar;
extern tToolbar          DefaultToolbar;

void TOOLBAR_Init( void );
void TOOLBAR_Handler( void );
void TOOLBAR_Set( const tToolbar* NewToolbar );
void TOOLBAR_SetDefaultToolbar( void );
void TOOLBAR_ChangeButton( s32 button, const u16* newicon, enum MENU_code( *newfct )( void ) );
void TOOLBAR_SelectButton( u16 button );
void TOOLBAR_UnSelectButton( u16 button );
void TOOLBAR_Button( u16 button, u16 color, u16 bgndcolor, bool sel );
void TOOLBAR_RedrawToolbar( s32 Request );

#endif // TOUCHSCREEN

/* POWER ----------------------------------------------------------------------*/
#if POWER_MNGT

extern u8 PWR_BatteryLevel;

extern enum ePowerState
{
    PWR_STATE_UNDEF   = 1,   /* don't know yet*/
    PWR_STATE_NOBAT   = 2,   /* battery is not connected or dead*/
    PWR_STATE_CHARGING = 3,  /* power supplied from external source, battery is being charged*/
    PWR_STATE_FULL    = 4,   /* power supplied from external source, charge is done (full)*/
    PWR_STATE_NORMAL  = 5,   /* power from battery, normal level*/
    PWR_STATE_LOW     = 6,   /* power from battery but battery is low (warning message displaid)*/
    PWR_STATE_EMPTY   = 7    /* power from battery but battery is critically low, shutdown to be processed*/
                    } PWR_CurrentState;

extern u16 PWR_CurrentVBat;

extern counter_t PWR_ReferenceTime;

void POWER_Handler( void );

#endif // POWER

/* LIST ----------------------------------------------------------------------*/
typedef struct
{
    const u8* Text;
} tListItem;

#if TOUCHSCREEN_AVAIL
typedef struct
{
    bool         fdispTitle;
    const u8*    Title;
    index_t      NbItems;
    len_t        LgMax;
    coord_n_t    XPos, YPos;
    coord_n_t    XSize, YSize;
    index_t      NbDisp;             /* Number of lines to be displayed*/
    index_t      SelectedItem;       /* Current selected item*/
    index_t      FirstDisplayItem;   /* Index of the first displayed item*/
    tListItem    Items[LIST_MAXITEM];
} tList;                         /* Generic list structure*/

typedef struct
{
    bool         fdispTitle;
    const u8*    Title;
    index_t      NbItems;
    len_t        LgMax;
    coord_n_t    XPos, YPos;
    coord_n_t    XSize, YSize;
    index_t      NbDisp;             //Number of lines to be displayed
    index_t      SelectedItem;       // Current selected item
    index_t      FirstDisplayItem;   // Index of the first displayed item
    tListItem    Items[MENU_MAXITEM];
} tListMin;                      // Menu specific list structure

extern tList*   CurrentList;
extern u8       DeltaY;

void LIST_Set( tList* lptr, coord_t posX, coord_t posY, bool center );
index_t LIST_Manager( void );
void InitListDMA( void );
void LIST_LCD_RectRead( coord_t x, coord_t y, coord_t width, coord_t height );
void LIST_DRAW_SetImage( coord_t x, coord_t y, coord_t width, coord_t height );

#endif // TOUCHSCREEN_AVAIL

/* RTC -----------------------------------------------------------------------*/
void  RTC_Init( void );
void  RTC_SetTime( u8 THH, u8 TMM, u8 TSS );
void  RTC_GetTime( u8* THH, u8* TMM, u8* TSS );
void  RTC_DisplayTime( void );
counter_t RTC_GetCounter( void );

/* FILESYSTEM ----------------------------------------------------------------*/
#if SDCARD_AVAIL
enum MENU_code FS_Explorer_Ini( void );
enum MENU_code FS_Explorer_Handler( void);
enum MENU_code Fct_Download_Ini( void );
enum MENU_code Fct_Download_Handler( void );
#endif // FILESYSTEM

/* APPLICATION ---------------------------------------------------------------*/
#define APP_DEF_SIZE 0x10                       // Application definition in FAT
// = 1 string 8 char + spare

enum MENU_code APP_FctApp( void );
enum MENU_code APP_LaunchAppli( index_t index );
index_t        APP_FindAppIndex( index_t index );
index_t        APP_FindNbApp( void );
index_t        APP_FindFirstApp( void );
void           APP_Init_AppTable( void );

#endif /*__CIRCLE_H */
