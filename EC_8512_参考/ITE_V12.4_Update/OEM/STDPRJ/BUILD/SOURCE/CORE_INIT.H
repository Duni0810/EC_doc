/*-----------------------------------------------------------------------------
 * TITLE: CORE_INIT.H
 *
 * Copyright (c) 1983-2009, Insyde Software Corporation. All Rights Reserved.
 *
 * You may not reproduce, distribute, publish, display, perform, modify, adapt,
 * transmit, broadcast, present, recite, release, license or otherwise exploit
 * any part of this publication in any form, by any means, without the prior
 * written permission of Insyde Software Corporation.
 *---------------------------------------------------------------------------*/
#ifndef CORE_INIT_H
#define CORE_INIT_H

/*-----------------------------------------------------------------------------
 * Data Parameter definition
 *---------------------------------------------------------------------------*/
#define EXT_CB2_INIT        0x22
#define EXT_CB3_INIT        0x01
#define RAM_INIT_WAKE1      0x00
#define RAM_INIT_WAKE2      0x00
#define RAM_INIT_WAKE3      0x40

/*-----------------------------------------------------------------------------
 * Local Data Structure Prototype
 *---------------------------------------------------------------------------*/
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
struct REG_INIT_DEF
{
    XBYTE   *address;
    BYTE    initdata;
};
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
typedef struct DMAScratchSRAM
{
    XBYTE   *scarh;
    XBYTE   *scarm;
    XBYTE   *scarl;
} sDMAScratchSRAM;
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

/*-----------------------------------------------------------------------------
 * External Calling Prototype
 *---------------------------------------------------------------------------*/
extern void CacheDma(BYTE sramnum,WORD addr);

/*-----------------------------------------------------------------------------
 * Internal Process Prototype
 *---------------------------------------------------------------------------*/
static void Reset_Initialization(void);
static void Init_Regs(void);
static void Init_PS2(void);
static void Cold_Reset(void);
static void Common_Reset(void);
static void Init_Cache(void);
extern void Init_ClearRam(void);

//-----------------------------------------------------------------------------
#endif