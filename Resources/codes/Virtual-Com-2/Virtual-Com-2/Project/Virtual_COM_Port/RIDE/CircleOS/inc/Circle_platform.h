/****************** COPYRIGHT (C) 2007-2010 RAISONANCE S.A.S. *****************/
/**
*
* @file     circle_platform.h
* @brief    Open4 STM32E specific general header for the CircleOS project.
* @author   YRT
* @date     09/2009
* @version  4.0
*
* @Note     Split form circle.h
*
**/
/******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __PLATFORM_H
#define __PLATFORM_H

#include "stm32f10x.h"

/*-------------------------------- Type definitions --------------------------*/
#include "circle_types.h"

typedef void ( *tAppPtrMgr )( coord_t sposX, coord_t sposY );

/*------------------------------ Hardware configuration ----------------------*/
#define JOYSTICK_AVAIL          1   // Joystick is managed or not
#define LED_INV                 1   // LED are active at level 0 or not
#define POWER_MNGT              1   // Battery charger present or not
#define SDCARD_AVAIL            1   // SDCard is managed or not
#define TOUCHSCREEN_AVAIL       1   // Touchscreeen is managed or not
#define AUDIO_AVAIL             1   // A codec is used, or only buzzer
#define MEMS_POINTER            0   // Menus are managed by Mems or not
#define BACKLIGHT_INTERFACE     1   // The backlight level is managed or not
#define MEMS_SPEC_MNGT          1   // The MEMS handler is called by special timer or not
#define MEMS_FULLINFO           1   // The MEMS handler computes all filters
#define SDCARD_SDIO             1   // SDCard is manage by SDIO peripheral or not
#define DISPLAY_TEMP            1   // Display internal CPU temperature
#define EXT_FONT                1   // Extended font management (several system fonts)
#define BUZZER_VOL_CONF         0   // Buzzer volume configurable or not

/*------------------------------ General -------------------------------------*/
#define PRIMER_TYPE 4

#define NODEBUG __attribute__ ((section(".non_debuggable_code")))
#define NODEBUG2
#define IRQ __attribute__ ((interrupt ("IRQ")))
#define CONST_DATA const

/* Defines to ensure platform dependance ------------*/
#define CIRCLEOS_FAT_ADDRESS (0x08000104)
#define RAM_BASE             (0x20000000)

// Data below moved to CircleStartup_spe.s
//#define FA_TABLE             (0x0800C000)   // 48 Ko flash for CircleOS)
//#define FLASH_SIZE           (512-132)      // 132 Ko flash for constants
//#define RAM_SIZE             (64-12)        // RAM available for applications

extern unsigned long _sdata;     /* start address for the .data section. defined in linker script */
#define CIRCLEOS_RAM_OFS  ( (unsigned long)&_sdata - RAM_BASE )
#define CIRCLEOS_RAM_BASE (RAM_BASE + CIRCLEOS_RAM_OFS)

#define APP_VOID ((tMenuItem*)(-1))

#define RCC_PERIPH_GPIO_CLOCK_CMD RCC_APB2PeriphClockCmd

/* Variables  ----------------------------------------------------------------*/
extern RCC_ClocksTypeDef RCC_ClockFreq;

/*-------------------------------   UTIL  ------------------------------------*/
extern CONST_DATA unsigned freqTIM2[6];

/* The following macro allows to adapt 'delay' defined by a number of tick to the real frequency of the tick.*/
/* we abusively adapt it using freqTIM2 because freqTIM2 and systick have always the same ratio*/
#define WEIGHTED_TIME(x)    ( ( (x) * freqTIM2[CurrentSpeed]) / freqTIM2[SPEED_MEDIUM] )

#define SPI_TIMEOUT                200

len_t       my_strlen( const u8* p );

/*-------------------------------   MEMS  ------------------------------------*/
#define RCC_APBxPERIPH_GPIOX_MEMS_CTRL      RCC_APB2Periph_GPIOA
#define RCC_APBxPERIPH_GPIOX_MEMS_CS        RCC_APB2Periph_GPIOA
#define SPIX_MEMS                           SPI1
#define RCC_APBxPERIPH_SPIX_MEMS            RCC_APB2Periph_SPI1
#define RCC_APBxPERIPH_MEMS_CLOCK_CMD       RCC_APB2PeriphClockCmd
#define GPIOX_MEMS_CTRL                     GPIOA
#define GPIOX_MEMS_CS                       GPIOA
#define GPIO_PIN_MEMS_CS                    GPIO_Pin_4
#define GPIO_PIN_MEMS_SCK                   GPIO_Pin_5
#define GPIO_PIN_MEMS_MISO                  GPIO_Pin_6
#define GPIO_PIN_MEMS_MOSI                  GPIO_Pin_7

/*----------------------------------   LED ------------------------------------*/
// Note : Inversion versus Primer2 => mistake on the schematics
#define RCC_APBxPeriph_GPIOx_LED    RCC_APB2Periph_GPIOE
#define GPIOx_LED                   GPIOE
#define GPIO_Pin_LED0               GPIO_Pin_1  // Red
#define GPIO_Pin_LED1               GPIO_Pin_0  // Green

#define RCC_APBxPeriph_GPIOx_DB_LED RCC_APB2Periph_GPIOE
#define GPIOx_DB_LED                   GPIOE
#define GPIO_Pin_DB_LED1               GPIO_Pin_11
#define GPIO_Pin_DB_LED2               GPIO_Pin_12


/*--------------------------------   ADC  ------------------------------------*/
#define ADC1_DR_Address    ((u32)0x4001244C)
#define ADC_NB_SAMPLES    20
#define ADC_NB_CHANNELS   5 /* = vbat + 3 touchscreen + temp*/
#define ADC_DMA_SIZE     ( ADC_NB_CHANNELS * ADC_NB_SAMPLES )

extern u16 ADC_ConvertedValue[ADC_DMA_SIZE];

#define RCC_APBxPeriph_GPIOx_TOUCH      RCC_APB2Periph_GPIOC
#define GPIOx_TOUCH                     GPIOC
#define GPIO_Pin_TOUCH_R                GPIO_Pin_1
#define GPIO_Pin_TOUCH_U                GPIO_Pin_2
#define GPIO_Pin_TOUCH_D                GPIO_Pin_0
#define GPIO_Pin_TOUCH_L                GPIO_Pin_0

#define GPIO_VBAT_PERIPH                RCC_APB2Periph_GPIOB
#define GPIOx_VBAT                      GPIOB
#define GPIO_VBAT_PIN                   GPIO_Pin_1

/*--------------------------------   SHUTDOWN  -------------------------------*/

/*--------------------------------   BUTTON  ---------------------------------*/
#define  GPIO_BUTTON_PIN         GPIO_Pin_15
#define  GPIOx_BUTTON            GPIOE
#define  GPIO_BUTTON_PERIPH      RCC_APB2Periph_GPIOE

/*--------------------------------  JOYSTICK  --------------------------------*/
#define  GPIO_JOYSTICK_L_PIN     GPIO_Pin_8
#define  GPIO_JOYSTICK_R_PIN     GPIO_Pin_9
#define  GPIO_JOYSTICK_U_PIN     GPIO_Pin_10
#define  GPIO_JOYSTICK_D_PIN     GPIO_Pin_11
#define  GPIOx_JOYSTICK          GPIOD
#define  GPIO_JOYSTICK_PERIPH    RCC_APB2Periph_GPIOD

/*--------------------------------   POINTER  --------------------------------*/

/*----------------------------------   LCD   ---------------------------------*/
#define LCD_HIGH_DEF 1            // 1 => magnitude 2 for system strings

#include "lcd_spe.h"
#include "font_spe.h"

/*----------------------------------   DRAW   --------------------------------*/

/*--------------------------------   BUZZER  ---------------------------------*/
#define SYSTEM_CLOCK RCC_ClockFreq.SYSCLK_Frequency

/*--------------------------------   AUDIO CODEC  ----------------------------*/
#define CONTROL_REGISTER_0 0
#define SPI2_I2SPR (SPI2_BASE + 0x20)
#define AUDIO_MIN_ATTENUATION 4        /* Min attenuation in dB*/

/*---------------------------------   MENU   -----------------------------------*/
#define MENU_MAXITEM 7
#define MAX_APP_MENU_SIZE 10
#define MAXAPP  64
#define MAX_MENUAPP_SIZE 3

#define MAXBTIME     4           /*!< The stability in the menu.           */
#define MAXLEVEL     5
#define LEFT_MARGIN  60

#define WIDTH_SQUARE 40
#define BAR_BOTTOM   80
#define BAR_LEFT     15

#define TIME_MARGIN  60
#define TIME_BOTTOM  80
#define TIME_TITLE_X 50
#define TIME_TITLE_Y 216
#define TIME_TITLE_H 40

/*--------------------------------   BACKLIGHT  ------------------------------*/

/*--------------------------------   TOUCHSCR  -------------------------------*/
#define OFS_CHANNEL_TS 2    // Offset of the first TS info in the analog buffer

/*--------------------------------   TOOLBAR  --------------------------------*/
#define TOOLBAR_MAXITEMS 4
#define TB_NB_BUTTONS    4
#define BUTTON_HEIGHT    80
#define BUTTON_WIDTH     60
#define ICON_HEIGHT      60
#define ICON_WIDTH       60

/*--------------------------------   POWER  ---------------------------------*/
#define GPIO_PWR_PERIPH         RCC_APB2Periph_GPIOC
#define GPIOx_PWR               GPIOC
#define GPIO_PWR_PERIPH_STAT    RCC_APB2Periph_GPIOC
#define GPIOx_PWR_STAT          GPIOC
#define GPIO_PWR_SHUTDOWN_PIN   GPIO_Pin_13
#define GPIO_PWR_LOADING_PIN    GPIO_Pin_6  // Status1
#define GPIO_PWR_DONE_PIN       GPIO_Pin_7  // Status2
#define BAT_FULL_VOLTAGE        4200
#define BAT_LOW_VOLTAGE         3750
#define BAT_EMPTY_VOLTAGE       3500
#define NO_BAT_VOLTAGE          3000

void POWER_Reset_Time( void );
void POWER_Set_Time( void );

/*--------------------------------   LIST  -----------------------------------*/
#define LIST_MAXITEM    64
#define DELTA_Y_MAX     1                                  /* Number of lines moved during 1 move*/
#define LCD_DMA_SIZE (APP_SCREEN_WIDTH * DELTA_Y_MAX * 2) /* 240 pixels width * DELTA_Y pixels height * (2 bytes / pixel)*/

/*--------------------------------   RTC  ------------------------------------*/

/*---------------------------   Backup registers  ----------------------------*/
#define BKP_SYS1     1          /* current application*/
#define BKP_SYS2     2          /* speed, joystick...*/
#define BKP_SYS3     3          /* backlight value*/
#define BKP_SYS4     4          /* menu font */
#define BKP_SYS5     5
#define BKP_SYS6     6
#define BKP_USER1    7
#define BKP_USER2    8
#define BKP_USER3    9
#define BKP_USER4    10
#define BKP_SYS7     11         /* Touchscreen option */
#define BKP_SYS8     12         /* Touchscreen calibration points */
#define BKP_SYS9     13
#define BKP_SYS10    14
#define BKP_SYS11    15
#define BKP_SYS12    16
#define BKP_SYS13    17

#define BKPMASK_S2_PLL      0x0007
#define BKPMASK_S2_SPEAKER  0x0008
#define BKPMASK_S2_JOYSTICK 0x0010
#define BKPMASK_S2_MEMS     0x0020
#define BKPMASK_S2_MUTE     0x0040
#define BKPMASK_S2_AUTORUN  0x0080
#define BKPMASK_S2_VOLUME   0x0F00
#define BKPMASK_S2_BUZZER   0x1000
#define BKPMASK_S7_TCHSCR   0x0200
#define BKPMASK_S4_FONT     0x0003

#define BKP_BKLIGHT  (BKP_SYS3)

#define BKP_TS_X0    (BKP_SYS8)
#define BKP_TS_Y0    (BKP_SYS9)
#define BKP_TS_X1    (BKP_SYS10)
#define BKP_TS_Y1    (BKP_SYS11)
#define BKP_TS_X2    (BKP_SYS12)
#define BKP_TS_Y2    (BKP_SYS13)

/*--------------------------------- USB ------------------------------*/
#define USB_DISCONNECT_PORT               GPIOD
#define USB_DISCONNECT_PIN                GPIO_Pin_3
#define RCC_APBxPeriph_GPIO_DISCONNECT    RCC_APB2Periph_GPIOD

/*--------------------------------- SDIO -----------------------------*/
#define SDIO_INIT_CLK_DIV                  ((uint8_t)0xB2)
#define SDIO_TRANSFER_CLK_DIV              ((uint8_t)0x1) 
#define SDIO_FIFO_ADDRESS                ((uint32_t)0x40018080)

/*--------------------------------- Application ------------------------------*/
#ifndef _MENU_CODE_DEFINED
#define _MENU_CODE_DEFINED
enum  MENU_code
{
    MENU_LEAVE  = 0, MENU_CONTINUE = 1, MENU_REFRESH = 2,
    MENU_CHANGE = 3, MENU_CONTINUE_COMMAND = 4, MENU_LEAVE_AS_IT = 5,
    MENU_RESTORE_COMMAND = 6
};

typedef struct
{
    const u8* Text;
    enum MENU_code( *Fct_Init )( void );
    enum MENU_code( *Fct_Manage )( void );
    enumset_t fMenuFlag;
} tMenuItem;

extern tMenuItem*(( *ApplicationTable )[] );

#endif //_MENU_CODE_DEFINED


#endif /*__PLATFORM_H */
