/*-----------------------------------------------------------------------------
 * TITLE: CORE_ACPI.H
 *
 * Copyright (c) 1983-2009, Insyde Software Corporation. All Rights Reserved.
 *
 * You may not reproduce, distribute, publish, display, perform, modify, adapt,
 * transmit, broadcast, present, recite, release, license or otherwise exploit
 * any part of this publication in any form, by any means, without the prior
 * written permission of Insyde Software Corporation.
 *---------------------------------------------------------------------------*/
#ifndef CORE_ACPI_H
#define CORE_ACPI_H

/*-----------------------------------------------------------------------------
 * Code Parameter
 *---------------------------------------------------------------------------*/
#define _PM1_STEP_1         1
#define _PM1_STEP_2         2
#define _PM1_STEP_3         3
#define _PM1_STEP_4         4
#define _PM1_STEP_5         5
#define _PM1_STEP_6         6
#define _PM1_STEP_7         7

/*-----------------------------------------------------------------------------
 * External Calling Process
 *---------------------------------------------------------------------------*/
extern void Service_PCI2(void);
extern void Service_PCI2_Main(void);

extern void EC6266_CMD_80(void);
extern void EC6266_CMD_81(void);
extern void EC6266_CMD_82(void);
extern void EC6266_CMD_83(void);
extern void EC6266_CMD_84(void);
extern void EC_Cmd_8X(void);

extern void Service_EventCenter(void);
extern void SCI_RESPONSE(void);
extern void SCI_INTERRUPT(void);
extern void SMI_INTERRUPT(void);
extern void Loop_Delay(BYTE delay);
extern BYTE ReadSCI_QueryValue(void);
extern BYTE WriteSCI_QueryValue(BYTE NewSCI);
extern BYTE ReadSMI_QueryValue(void);
extern BYTE WriteSMI_QueryValue(BYTE NewSMI);
extern void ClearEventBuffer(void);
extern bit  CheckBurstMode(void);

extern void vSetTotalBurstTime(void);
extern void ACPISMBusviaEC(void);

/*-----------------------------------------------------------------------------
 * Internal Calling Prototype
 *---------------------------------------------------------------------------*/
static BYTE ACPI_SMB_NULL(void);
static BYTE ACPI_SMB_WQuickCMD(void);
static BYTE ACPI_SMB_RQuickCMD(void);
static BYTE ACPI_SMB_SendBYTE(void);
static BYTE ACPI_SMB_ReciveBYTE(void);
static BYTE ACPI_SMB_WriteBYTE(void);
static BYTE ACPI_SMB_ReadBYTE(void);
static BYTE ACPI_SMB_WriteWORD(void);
static BYTE ACPI_SMB_ReadWORD(void);
static BYTE ACPI_SMB_WriteBLOCK(void);
static BYTE ACPI_SMB_ReadBLOCK(void);
static BYTE ACPI_SMB_ProcessCall(void);
static BYTE ACPI_SMB_BLOCKCall(void);

//-----------------------------------------------------------------------------
#endif
