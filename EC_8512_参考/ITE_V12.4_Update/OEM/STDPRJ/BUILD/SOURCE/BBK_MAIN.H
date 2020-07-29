/*-----------------------------------------------------------------------------
 * Filename: BBK_MAIN.C
 * Function: Non-update for Firmware Recovery (Boot-block for crisis)
 *
 * Copyright (c) 2006-2009, ITE Tech. Inc. All Rights Reserved.
 *---------------------------------------------------------------------------*/
#ifndef BBK_MAIN_H
#define BBK_MAIN_H 

/*-----------------------------------------------------------------------------
 * Wait Host Delay Parameter
 *---------------------------------------------------------------------------*/
#define WaitKBDataDelay         0x800
#define WaitECDataDelay         0x800
#define WaitPM2DataDelay        0x800

/*-----------------------------------------------------------------------------
 * External Calling Process
 *---------------------------------------------------------------------------*/
extern void BB_Main(void);
extern void BB_Delay1MS(BYTE bMS);
extern void BB_InitGPIO(void);
extern void BB_InitSio(void);
extern void BB_KBDataToHostWaitRx(BYTE nDATA);
extern void BB_RunCodeInRAM(void);


/*-----------------------------------------------------------------------------
 * Internal Process Definition
 *---------------------------------------------------------------------------*/
static void BB_Check64PortCmd(void);
static void BB_Check66PortCmd(void);
static void BB_Check6CPortCmd(void);
static void BB_Timers_Init(void);
static void BB_InitRegs(void);
static void BB_RamDebug(unsigned char dbgcode);


//-----------------------------------------------------------------------------
#endif