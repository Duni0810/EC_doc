/*----------------------------------------------------------------------------
 * TITLE: CORE_TIMERS.H - Header File for Timer.C
 *
 * Copyright (c) 1983-2009, Insyde Software Corporation. All Rights Reserved.
 *
 * You may not reproduce, distribute, publish, display, perform, modify, adapt,
 * transmit, broadcast, present, recite, release, license or otherwise exploit
 * any part of this publication in any form, by any means, without the prior
 * written permission of Insyde Software Corporation.
 *----------------------------------------------------------------------------*/
#ifndef CORE_TIMERS_H
#define CORE_TIMERS_H

/*-----------------------------------------------------------------------------
 * External Calling Process
 *---------------------------------------------------------------------------*/
extern void Load_Timer_A(void);
extern void Load_Timer_B(void);
extern void Stop_Timer_B(void);

#ifndef BOOT_BLOCK_BUILD
extern void Service_MainLoTimer(void);
extern void Stop_Timer_A(void);
extern void Enable_Timer_A(void);
extern void Init_Timers(void);
extern void EnableSendToHostTimer(void);
extern void Microsecond_Delay(WORD delay);
extern void vDelay1MS (WORD bMS);
extern void vDelayXms(BYTE bMS);
extern void Init_UART(void);
#endif

//-----------------------------------------------------------------------------
#endif
