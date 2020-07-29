/*-----------------------------------------------------------------------------
 * TITLE: CORE_PORT6064.H - prototypes for CORE_PORT6064.C.
 *
 * Copyright (c) 1983-2009, Insyde Software Corporation. All Rights Reserved.
 *
 * You may not reproduce, distribute, publish, display, perform, modify, adapt,
 * transmit, broadcast, present, recite, release, license or otherwise exploit
 * any part of this publication in any form, by any means, without the prior
 * written permission of Insyde Software Corporation.
 *---------------------------------------------------------------------------*/
#ifndef CORE_PORT6064_H
#define CORE_PORT6064_H


/*-----------------------------------------------------------------------------
 * Wait Host Delay Parameter
 *---------------------------------------------------------------------------*/
#define WaitKBDataDelay         0x200
#define WaitECDataDelay         0x200
#define WaitPM2DataDelay        0x200

/*-----------------------------------------------------------------------------
 * Command Parameter
 *---------------------------------------------------------------------------*/
#define _CMD_BYTE1              0x02
#define _CMD_BYTE2              0x03
#define _CMD_BYTE3              0x04
#define _CMD_BYTE4              0x05

/*-----------------------------------------------------------------------------
 * External Calling Prototype
 *---------------------------------------------------------------------------*/
extern void Service_PCI(void);
extern void vKeyboardCmd(BYTE data_byte);

extern void Write_KCCB(BYTE NewKCCB);
extern bit  SetGetPort60Data(void);
extern bit  SetGetPort60MultiData(void);
extern BYTE WaitAndGetPort60Data(void);

extern void Cmd_A7(void);
extern void Cmd_A8(void);
extern void Cmd_AD(void);
extern void Cmd_AE(void);

extern BYTE Read_Output_Port_2(void);

/*-----------------------------------------------------------------------------
 * Internal Process Prototype
 *---------------------------------------------------------------------------*/
static void Cmd_0X(void);
static void Cmd_1X(void);
static void Cmd_2X(void);
static void Cmd_3X(void);
static void Cmd_4X(void);
static void Cmd_5X(void);
static void Cmd_6X(void);
static void Cmd_7X(void);
static void Cmd_8X(void);
static void Cmd_9X(void);
static void Cmd_AX(void);
static void Cmd_BX(void);
static void Cmd_CX(void);
static void Cmd_DX(void);
static void Cmd_EX(void);
static void Cmd_FX(void);

static void Cmd_A1(void);
static void Cmd_A4(void);
static void Cmd_A5(void);
static void Cmd_A5Data(void);

static void Cmd_A6(void);
static void Cmd_A9(void);
static void Cmd_AA(void);
static void Cmd_AB(void);
static void Cmd_AC(void);
static void Cmd_D3(void);
static void Cmd_D4(void);

//-----------------------------------------------------------------------------
#endif
