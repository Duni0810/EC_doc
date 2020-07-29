/*-----------------------------------------------------------------------------
 * Filename: OEM_BBLOCK.H
 * Function: OEM_BBLOCK.C Include Definition
 *
 *---------------------------------------------------------------------------*/
#ifndef OEM_BBLOCK_H
#define OEM_BBLOCK_H

/*-----------------------------------------------------------------------------
 * External Calling Process
 *---------------------------------------------------------------------------*/
extern void BB_SysOnSequence(void);
extern void BB_InitGPIO(void);
extern void BB_InitRegs(void);
extern void BB_OemInit(void);
extern void BB_InitShareMemory(void);
extern bit  BB_CheckEntry(void);
extern void BB_OemService(void);
extern void BB_OemCheckKBCmd(void);
extern void BB_OemProcessKBData(void);
extern void BB_OemCheckECCmd(void);
extern void BB_OemProcessECData(void);
extern void BB_OemCheckPM2Cmd(void);
extern void BB_OemProcessPM2Data(void);

/*-----------------------------------------------------------------------------
 * Internal Process Definition
 *---------------------------------------------------------------------------*/
static void BB_Oem_SysOnInit(void);


//-----------------------------------------------------------------------------
#endif

