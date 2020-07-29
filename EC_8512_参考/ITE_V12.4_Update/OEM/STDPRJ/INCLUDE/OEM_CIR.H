/*-----------------------------------------------------------------------------
 * Filename: OEM_CIR.H
 * Function: OEM_CIR.C Function prototype include definition
 *
 * Copyright (c) 2006-2009, ITE Tech. Inc. All Rights Reserved.
 *---------------------------------------------------------------------------*/
#ifndef OEM_CIR_H
#define OEM_CIR_H


/*-----------------------------------------------------------------------------
 * External Calling Prototype
 *---------------------------------------------------------------------------*/
extern void Service_CIR(void);
extern void Init_CIR(void);
extern void Reset_CIR(void);
extern void bClearCirFIFO(void);
extern void bConfirmCirRIM(void);
extern void service_cir(void);
extern void ClearCIR(void);
extern bit  bCIRWakeUp(void);
extern void CIRS4S5WakeUp(void);
extern void CIRS3WakeUp(void);

/*-----------------------------------------------------------------------------
 * Internal Process Prototype
 *---------------------------------------------------------------------------*/



//-----------------------------------------------------------------------------
#endif  //OEM_CIR_H
