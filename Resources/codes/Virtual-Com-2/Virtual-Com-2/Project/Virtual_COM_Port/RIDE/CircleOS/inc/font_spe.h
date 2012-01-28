/****************** COPYRIGHT (C) 2007-2010 RAISONANCE S.A.S. *****************/
/**
*
* @file     font_spe.h
* @brief    Font structures declaration.
* @author   CR
* @date     06/2010
* @note     STM32
*
**/
/******************************************************************************/

#define MAX_FONT_NB 20

enum ENUM_FontID
{
    FONT_PRIMER     = 0, // Default font
    FONT_PRIMERX2   = 1,
    FONT_MEDIUM     = 2,
    FONT_MEDIUMX2   = 3,
    FONT_NUMBERS    = 4
};

typedef struct 
{
    enum ENUM_FontID    ID;
    u8                  width;
    u8                  height;
    u8                  FontCoeff;
    u8                  ASCII_start;
    u8                  ASCII_end;
    u8                  *font;
    u8                  title[16];
} tFontDef;

typedef struct
{
    u8          nb;
    tFontDef fonts[MAX_FONT_NB];
} tFontTable;
