/****************** COPYRIGHT (C) 2007-2011 RAISONANCE S.A.S. *****************/
/**
*
* @file     Util.c
* @brief    Various common utilities.
* @author   YRT
* @date     04/2011
*
**/
/******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "circle.h"
#include "FreeRTOS.h"

/* Private defines -----------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
RCC_ClocksTypeDef    RCC_ClockFreq;
enum eSpeed CurrentSpeed;

/* Private function prototypes -----------------------------------------------*/
static void _int2str( u8* ptr, int_t X, len_t digit, bool flagunsigned, bool fillwithzero );

/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
*
*                    _int2str
*
*******************************************************************************/
/**
*
*  Translate a integer into a string.
*
*  @param[in,out] ptr            A pointer to a string large enough to contain
*                                the translated 32 bit word.
*  @param[in]     X              The integer to translate.
*  @param[in]     digit          The amount of digits wanted in the result string.
*  @param[in]     flagunsigned   Is the input word unsigned?
*  @param[in]     fillwithzero   Fill with zeros or spaces.
*
**/
/******************************************************************************/
static void _int2str( u8* ptr, int_t X, len_t digit, bool flagunsigned, bool fillwithzero )
{
    u8      c;
    bool    fFirst   = 0;
    bool    fNeg     = 0;
    uint_t  DIG      = 1;
    len_t   i;
    int_t   r;

    for ( i = 1; i < digit; i++ )
    {
        DIG *= 10;
    }

    if ( !flagunsigned && ( X < 0 ) )
    {
        fNeg = 1;
        r    = -X;
    }
    else
    {
        r = X;
    }

    for ( i = 0; i < digit; i++, DIG /= 10 )
    {
        c  = ( r / DIG );
        r -= ( c * DIG );

        if ( fillwithzero || fFirst || c || ( i == ( digit - 1 ) ) )
        {
            if (( fFirst == 0 ) && !flagunsigned )
            {
                *ptr++ = fNeg ? '-' : ' ';
            }

            *ptr++ = ( c % 10 ) + '0';
            fFirst = 1;
        }
        else
        {
            *ptr++ = ' ';
        }
    }

    *ptr++ = '\0';
}


/* Public functions ----------------------------------------------------------*/


/*******************************************************************************
*
*                   UTIL_uint2str
*
*******************************************************************************/
/**
*
*  Convert an <b>unsigned</b> integer into a string.
*  Using this function leads to much smaller code than using the sprintf()
*  function from the C library.
*
*  @param [out]  ptr    The output string.
*  @param [in]   X      The unsigned value to convert.
*  @param [in]   digit  The number of digits in the output string.
*  @param [in]   fillwithzero  \li 0   fill with blanks.
*                              \li 1   fill with zeros.
*
**/
/********************************************************************************/
void UTIL_uint2str( u8* ptr, uint_t X, len_t digit, bool fillwithzero )
{
    _int2str( ptr, X, digit, 1, fillwithzero );
}

/*******************************************************************************
*
*                   UTIL_int2str
*
*******************************************************************************/
/**
*
*  Convert a <b>signed</b> integer into a string.
*  Using this function leads to much smaller code than using the sprintf()
*  function from the C library.
*
*  @param [out]  ptr    The output string.
*  @param [in]   X      The unsigned value to convert.
*  @param [in]   digit  The number of digits in the output string.
*  @param [in]   fillwithzero  \li 0   fill with blanks.
*                              \li 1   fill with zeros.
*
**/
/******************************************************************************/
void UTIL_int2str( u8* ptr, int_t X, len_t digit, bool fillwithzero )
{
    _int2str( ptr, X, digit, 0, fillwithzero );
}


/*******************************************************************************
*
*                    delayms
*
*******************************************************************************/
void fct_dummy() {;}
void Delayms( u32 count )
{
//    vTaskDelay( nTime / (portTICK_RATE_MS + 1));
    s32 i, j;
    for ( i = 0; i < count; i++ )
        //for ( j = 0; j < 1300; j++ )
        for ( j = 0; j < 2600; j++ )
            fct_dummy();
}

void delay( s32 count )
{
    s32 i, j;
    for ( i = 0; i < count; i++ )
        //for ( j = 0; j < 1300; j++ )
        for ( j = 0; j < 2600; j++ )
            fct_dummy();
}

/*******************************************************************************
*
*                                my_strlen
*
*******************************************************************************/
/**
*  Behaves as the standard ANSI "strlen", return provided string length.
*
*  @param[in]  p The string to evaluate the length.
*
*  @return  The length of the provided string.
*
**/
/******************************************************************************/
len_t my_strlen( const u8* p )
{
    len_t len = 0;

    for ( ; *p; p++ )
    {
        len++;
    }

    return len;
}

/*******************************************************************************
*
*                                UTIL_ReadBackupRegister
*
*******************************************************************************/
/**
*
*  Reads data from the specified Data Backup Register.
*
*  @param[in]  BKP_DR   Specifies the Data Backup Register. This parameter can be BKP_DRx where x:[1, 10]
*
*  @return  The content of the specified Data Backup Register.
*
**/
/******************************************************************************/
backup_t UTIL_ReadBackupRegister( index_t BKP_DR )
{
    if ( BKP_DR < 11 )
    {
        return ( *( vu16* )( BKP_BASE + 4 * BKP_DR ) );
    }
    else if ( BKP_DR < 43 )
    {
        return ( *( vu16* )( BKP_BASE + 4 * ( BKP_DR + 5 ) ) );
    }
    else return 0;
}

/*******************************************************************************
*
*                                UTIL_WriteBackupRegister
*
*******************************************************************************/
/**
*
*  Writes data to the specified Data Backup Register.
*
*  @param[in]  BKP_DR   Specifies the Data Backup Register. This parameter can be BKP_DRx where x:[1, 10]
*  @param[in]  Data     The data to write.
*
**/
/********************************************************************************/
void UTIL_WriteBackupRegister( index_t BKP_DR, backup_t Data )
{
    if ( BKP_DR < 11 )
    {
        *( vu16* )( BKP_BASE + 4 * BKP_DR ) = Data;
    }
    else if ( BKP_DR < 43 )
    {
        *( vu16* )( BKP_BASE + 4 *( BKP_DR + 5 ) ) = Data;
    }
}


