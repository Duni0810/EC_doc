/*-----------------------------------------------------------------------------
 * TITLE: CORE_HSPI.H
 *
 * Copyright (c) 2009- , ITE Corporation. All Rights Reserved.
 *
 * You may not reproduce, distribute, publish, display, perform, modify, adapt,
 * transmit, broadcast, present, recite, release, license or otherwise exploit
 * any part of this publication in any form, by any means, without the prior
 * written permission of Insyde Software Corporation.
 *---------------------------------------------------------------------------*/
#ifndef CORE_HSPI_H
#define CORE_HSPI_H

#define HSPI_ECCodeSpaceAddr    0x7E    // EC code space is 0x7Exxxx

extern void GPIO_HSPI_INIT(void);
extern void Enable_HSPI(void);
extern void Disable_HSPI(void);
extern void HSPI_RamCode(void);
extern void HSPI_SelfRamCode(void);
extern void HSPI_JmpTo0XFE00(void);
extern void HSPI_Process(void);
extern void HSPI_SetReloadRamCodeFlag(void);

#endif