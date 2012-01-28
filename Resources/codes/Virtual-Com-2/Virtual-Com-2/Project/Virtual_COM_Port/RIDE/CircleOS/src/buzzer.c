/****************** COPYRIGHT (C) 2007-2010 RAISONANCE S.A.S. *****************/
/**
*
* @file     buzzer.c
* @brief    Common Buzzer functions with RTTTL format support.
 @author    IB
* @date     07/2007
*
* @date     09/2008
* @version  3.0 Add audio version for Primer 2
* @date     10/2009
* @version  4.0 Add Open4 Primer
**/
/******************************************************************************/


/* Includes ------------------------------------------------------------------*/
#include "circle.h"

/// @cond Internal

/* Private typedef -----------------------------------------------------------*/

/*! Octaves */
enum eOctave
{
    OCT_440  = 0,  /*!< o = 5 */
    OCT_880  = 1,  /*!< o = 6 */
    OCT_1760 = 2,  /*!< o = 7 */
    OCT_3520 = 3,  /*!< o = 8 */
    OCT_7040 = 4   /*!< o = 9 */
} octave;

/*! Notes */
enum eNotes
{
    NOTE_PAUSE = 0,    /*!< P  */
    NOTE_LA    = 1,    /*!< A  */
    NOTE_LA_H  = 8 + 1, /*!< A# */
    NOTE_SI    = 2,    /*!< B  */
    NOTE_DO    = 3,    /*!< C  */
    NOTE_DO_H  = 8 + 3, /*!< C# */
    NOTE_RE    = 4,    /*!< D  */
    NOTE_RE_H  = 8 + 4, /*!< D# */
    NOTE_MI    = 5,    /*!< E  */
    NOTE_FA    = 6,    /*!< F  */
    NOTE_FA_H  = 8 + 6, /*!< F# */
    NOTE_SOL   = 7,    /*!< G  */
    NOTE_SOL_H = 8 + 7 /*!< G# */
} note;

/* Private define ------------------------------------------------------------*/
#define BUZZER_SHORTBEEP_DURATION   100
#define BUZZER_LONGBEEP_DURATION    1000
#define RTTTL_SEP                   ':'

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
enum BUZZER_mode                 Buzzer_Mode          = BUZZER_UNDEF;
counter_t                        Buzzer_Counter       = 0;

/* For the melody.*/
const u8*                        CurrentMelody        = 0;
const u8*                        CurrentMelodySTART   = 0;
u8                               DefaultOctave        = OCT_880;
u8                               DefaultDuration      = 4;
u16                              DefaultBeats         = 63;

CONST_DATA u16 Note_Freq[16] =
{
    0,    /*pause*/
    440,  /*A=LA*/
    494,  /*B=SI*/
    524,  /*C=DO*/
    588,  /*D=RE*/
    660,  /*E=MI*/
    698,  /*F=FA*/
    784,  /*G=SOL*/
    0,    /* "8+n" for "NOTE#"*/
    466,  /*A#=LA#*/
    0,
    544,  /*C#=DO#*/
    622,  /*D#=RE#*/
    0,
    740,  /*F#=FA#*/
    830   /*G#=SOL#*/
};


/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/* External variables ---------------------------------------------------------*/


/*******************************************************************************
*
*                                BUZZER_PlayNote
*
*******************************************************************************/
/**
*
*  Play the next note of the current melody.
*
**/
/******************************************************************************/
void BUZZER_PlayNote( void )
{
    u8 duration = DefaultDuration;
    u8 c;

    /* Discard blank characters*/
    while ( *CurrentMelody == ' ' )
    {
        CurrentMelody++;
    }

    /* Check whether a duration is present.*/
    if (( *CurrentMelody > '0' ) && ( *CurrentMelody < '9' ) )
    {
        duration = *CurrentMelody++ - '0';

        if (( *CurrentMelody > '0' ) && ( *CurrentMelody < '9' ) )
        {
            duration *= 10;
            duration += ( *CurrentMelody++ - '0' );
        }
    }

#if __RCSTM8__
    // Compute the number of milliseconds of the given note
//  Buzzer_Counter = 32L * 60 * 1000 / duration / DefaultBeats; // This is the THEORETICAL value
    Buzzer_Counter = 35000 / DefaultBeats * 10 / duration;      // This is the value that matches CircleOS3
#else
    // TODO verify the code and rewrite for efficiency
    Buzzer_Counter = (( 32 / duration ) * 256L * 32L ) / DefaultBeats;
    Buzzer_Counter *= ( SYSTEM_CLOCK / 12000L );
    Buzzer_Counter /= 1000L;  /*FL081103 divide in two stages to keep precision*/
#endif

    /* Read the note*/
    c = *CurrentMelody++;

    if (( c >= 'a' ) && ( c <= 'z' ) )
    {
        c += ( 'A' - 'a' );
    }

    if ( c == 'P' )
    {
        note = NOTE_PAUSE;
    }
    else if (( c >= 'A' ) && ( c <= 'G' ) )
    {
        note = ( c - 'A' ) + NOTE_LA;

        if ( *CurrentMelody == '#' )
        {
            note |= 0x8;
            CurrentMelody++;
        }
    }

    octave = DefaultOctave;
    c = *CurrentMelody;

    if (( c >= '5' ) && ( c <= '8' ) )
    {
        octave = OCT_440 + ( c - '5' );
        CurrentMelody++;
    }

    BUZZER_SetFrequency(( Note_Freq[note] *( 1 << octave ) ) );

    /* Handle special duration */
    if ( *CurrentMelody == '.' )
    {
        /* Dotted note: Add half duration */
        Buzzer_Counter += ( Buzzer_Counter / 2 );
        CurrentMelody++;
    }

    /* Discard delimiter */
    while (( c = *CurrentMelody++ ) != 0 )
    {
        if ( c == ',' )
            break;
    }

    /* Check end of melody */
    if ( c == 0 )
    {
        CurrentMelody  = 0;
        BUZZER_SetMode( BUZZER_OFF );
    }
}

/* Public functions for CircleOS ---------------------------------------------*/


/*******************************************************************************
*
*                                BUZZER_Handler
*
*******************************************************************************/
/**
*
*  Called by the CircleOS scheduler to manage Buzzer tasks.
*
*  @attention  This function must <b>NOT</b> be called by the user.
*
**/
/******************************************************************************/
void BUZZER_Handler( void )
{
    switch ( Buzzer_Mode )
    {
    case BUZZER_PLAYMUSIC :
        if ( Buzzer_Counter <= 0 )
        {
            /* Play next note*/
            BUZZER_PlayNote();
        }
        else
        {
            /* Continue the current note*/
            Buzzer_Counter--;
        }
        break;

    case BUZZER_SHORTBEEP :
        if ( Buzzer_Counter++ >= WEIGHTED_TIME( BUZZER_SHORTBEEP_DURATION ) )
        {
            BUZZER_SetMode( BUZZER_OFF );
        }
        break;

    case BUZZER_LONGBEEP :
        if ( Buzzer_Counter++ >= WEIGHTED_TIME( BUZZER_LONGBEEP_DURATION ) )
        {
            BUZZER_SetMode( BUZZER_OFF );
        }
        break;
    }
}

/// @endcond

/* Public functions ----------------------------------------------------------*/

/*******************************************************************************
*
*                                BUZZER_GetMode
*
*******************************************************************************/
/**
*
*  Get the current buzzer mode.
*
*  @return  Current buzzer mode.
*
**/
/******************************************************************************/
enum BUZZER_mode BUZZER_GetMode( void )
{
    return Buzzer_Mode;
}


/*******************************************************************************
*
*                                BUZZER_PlayMusic
*
*******************************************************************************/
/**
*
*  Plays the provided melody that follows the RTTTL Format.
*
*  Official Specification
*  @verbatim
<ringing-tones-text-transfer-language> :=
<name> <sep> [<defaults>] <sep> <note-command>+
<name> := <u8>+ ; maximum name length 10 characters
<sep> := ":"
<defaults> :=
<def-note-duration> |
<def-note-scale> |
<def-beats>
<def-note-duration> := "d=" <duration>
<def-note-scale> := "o=" <scale>
<def-beats> := "b=" <beats-per-minute>
<beats-per-minute> := 25,28,...,900 ; decimal value
; If not specified, defaults are
;
; 4 = duration
; 6 = scale
; 63 = beats-per-minute
<note-command> :=
[<duration>] <note> [<scale>] [<special-duration>] <delimiter>
<duration> :=
"1" | ; Full 1/1 note
"2" | ; 1/2 note
"4" | ; 1/4 note
"8" | ; 1/8 note
"16" | ; 1/16 note
"32" | ; 1/32 note
<note> :=
"P"  | ; pause
"C"  |
"C#" |
"D"  |
"D#" |
"E"  |
"F"  |
"F#" |
"G"  |
"G#" |
"A"  |
"A#" |
"B"
<scale> :=
"5" | ; Note A is 440Hz
"6" | ; Note A is 880Hz
"7" | ; Note A is 1.76 kHz
"8" ; Note A is 3.52 kHz
<special-duration> :=
"." ; Dotted note
<delimiter> := ","
@endverbatim
*
*  @param[in]  melody New melody to play on buzzer.
*
**/
/******************************************************************************/
void BUZZER_PlayMusic( const u8* melody )
{
    u8    c;
    u8    default_id  = 0;
    u16   default_val = 0;

    DefaultOctave      = OCT_880;  /* Default for the default Octave.*/
    DefaultDuration    = 4;        /* Default for the default Duration.*/
    DefaultBeats       = 63;
    CurrentMelody      = melody;
    CurrentMelodySTART = melody;

    while ( *CurrentMelody != RTTTL_SEP )
    {
        if ( *CurrentMelody == 0 )
        {
            return;
        }

        /* Discard the melody name.*/
        CurrentMelody++;
    }

    /* Now read the defaults if any.*/
    for ( ++CurrentMelody; *CurrentMelody != RTTTL_SEP; CurrentMelody++ )
    {
        if ( *CurrentMelody == 0 )
        {
            return;
        }

        /* Discard any blank.*/
        while ( *CurrentMelody == ' ' )
        {
            CurrentMelody++;
        }

        c = *CurrentMelody;

        if ( c == RTTTL_SEP )
        {
            break;
        }

        if (( c >= 'a' ) && ( c <= 'z' ) )
        {
            c += ( 'A' - 'a' );
        }

        if (( c >= 'A' ) && ( c <= 'Z' ) )
        {
            default_id = c;
            continue;
        }

        if (( c >= '0' ) && ( c <= '9' ) )
        {
            default_val *= 10;
            default_val += ( c - '0' );
            c = * ( CurrentMelody + 1 );

            if (( c >= '0' ) && ( c <= '9' ) )
            {
                continue;
            }

            if ( default_id == 'D' )
            {
                DefaultDuration = default_val;
            }
            else if ( default_id == 'O' )
            {
                DefaultOctave = default_val - 5;

                if ( DefaultOctave > OCT_7040 )
                    DefaultOctave = OCT_440;
            }
            else if ( default_id == 'B' )
            {
                DefaultBeats = default_val;

                /* Check minimum BPM, so that the counter can be a 16-bit value */
                if (( DefaultBeats < 16 ) || ( DefaultBeats > 500 ) )
                    DefaultBeats = 63;
            }

            default_val = 0;
            default_id  = 0;
        }
    }

    BUZZER_SetMode( BUZZER_PLAYMUSIC );
}
