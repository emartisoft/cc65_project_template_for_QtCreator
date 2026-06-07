/* Based on cc65's c64.h, patched for clangd compat */

#ifndef _C64_H
#define _C64_H

#if !defined(__C64__)
#  error "This module is for the C64 only!"
#endif

#define CH_F1                   133
#define CH_F2                   137
#define CH_F3                   134
#define CH_F4                   138
#define CH_F5                   135
#define CH_F6                   139
#define CH_F7                   136
#define CH_F8                   140

#define COLOR_BLACK             0x00
#define COLOR_WHITE             0x01
#define COLOR_RED               0x02
#define COLOR_CYAN              0x03
#define COLOR_VIOLET            0x04
#define COLOR_PURPLE            COLOR_VIOLET
#define COLOR_GREEN             0x05
#define COLOR_BLUE              0x06
#define COLOR_YELLOW            0x07
#define COLOR_ORANGE            0x08
#define COLOR_BROWN             0x09
#define COLOR_LIGHTRED          0x0A
#define COLOR_GRAY1             0x0B
#define COLOR_GRAY2             0x0C
#define COLOR_LIGHTGREEN        0x0D
#define COLOR_LIGHTBLUE         0x0E
#define COLOR_GRAY3             0x0F

#define JOY_UP_MASK             0x01
#define JOY_DOWN_MASK           0x02
#define JOY_LEFT_MASK           0x04
#define JOY_RIGHT_MASK          0x08
#define JOY_BTN_1_MASK          0x10

#include <_vic2.h>
#define VIC     (*(struct __vic2*)0xD000)

#include <_sid.h>
#define SID     (*(struct __sid*)0xD400)

#include <_6526.h>
#define CIA1    (*(struct __6526*)0xDC00)
#define CIA2    (*(struct __6526*)0xDD00)

#define COLOR_RAM       ((unsigned char*)0xD800)

#define C64_OS_US       0xAA
#define C64_OS_PET64    0x64
#define C64_OS_SX64     0x43
#define C64_EU_NEW      0x03
#define C64_EU_OLD      0x00
#define C64_DTV         0xFF

extern int c64_65816_emd[];
extern int c64_c256k_emd[];
extern int c64_dqbb_emd[];
extern int c64_georam_emd[];
extern int c64_isepic_emd[];
extern int c64_ram_emd[];
extern int c64_ramcart_emd[];
extern int c64_reu_emd[];
extern int c64_vdc_emd[];
extern int dtv_himem_emd[];
extern int c64_hitjoy_joy[];
extern int c64_numpad_joy[];
extern int c64_ptvjoy_joy[];
extern int c64_stdjoy_joy[];
extern int c64_1351_mou[];
extern int c64_joy_mou[];
extern int c64_inkwell_mou[];
extern int c64_pot_mou[];
extern int c64_swlink_ser[];
extern int c64_hi_tgi[];

unsigned char get_ostype (void);

#endif
