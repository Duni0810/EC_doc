/*-----------------------------------------------------------------------------
 * TITLE: CORE_MAIN.H
 *
 * Copyright (c) 1983-2009, Insyde Software Corporation. All Rights Reserved.
 *
 * You may not reproduce, distribute, publish, display, perform, modify, adapt,
 * transmit, broadcast, present, recite, release, license or otherwise exploit
 * any part of this publication in any form, by any means, without the prior
 * written permission of Insyde Software Corporation.
 *---------------------------------------------------------------------------*/
#ifndef CORE_MAIN_H
#define CORE_MAIN_H

#include "PROJECT.H"

/*-----------------------------------------------------------------------------
 * External Calling Prototype
 *---------------------------------------------------------------------------*/
extern void DisableAllInterrupt(void);
extern void EnableAllInterrupt(void);
extern void main(void);
extern void main_service(void);
extern void RamDebug(unsigned char dbgcode);

/*-----------------------------------------------------------------------------
 * Internal Calling Prototype
 *---------------------------------------------------------------------------*/
static void vInitEnableInterrupt(void);
static void EnableInterrupt(void);
static bit  vCheckChipCanSleep(void);
static void Service_UNLOCK(void);
extern void Service_MS_5(void);
static void Service_Reserved_1(void);
static void Service_Reserved_2(void);
static void Service_Reserved_3(void);

//-----------------------------------------------------------------------------
#endif

