/*-----------------------------------------------------------------------------
 * Filename: OEM_POWER.H
 * Function: OEM_POWER.C Function prototype include definition
 *
 * Copyright (c) 2006-2009, ITE Tech. Inc. All Rights Reserved.
 *---------------------------------------------------------------------------*/
#ifndef OEM_POWER_H
#define OEM_POWER_H

/*-----------------------------------------------------------------------------
 * Parameter Definition
 *---------------------------------------------------------------------------*/
#define _PowrUp_S5      0
#define _SYSTEM_S0      1
#define _SYSTEM_S1      2
#define _SYSTEM_S3      3
#define _SYSTEM_S4      4
#define _SYSTEM_S5      5
#define _GO_POWER_ON    6
#define _GO_RESUME_ON   7
#define _GO_SUSPNED_OFF 8
#define _GO_POWER_OFF   9


/*-----------------------------------------------------------------------------
 * External Calling Process
 *---------------------------------------------------------------------------*/
extern void Oem_SysPowerContrl(void);
extern void Oem_ColdBootTester(void);
extern void Oem_ForceSystemOff(void);
extern void Oem_TriggerPowerOn(void);
extern void Oem_TriggerPowerOff(void);
extern void Oem_SysOnInit(void);
extern void Oem_SysOffInit(void);
extern void Oem_CommonInit(void);
extern void InitShareMemory(void);
extern void Oem_ChkS5WakeUp(void);

/*-----------------------------------------------------------------------------
 * Internal Process Definition
 *---------------------------------------------------------------------------*/



//-----------------------------------------------------------------------------
#endif