/*-----------------------------------------------------------------------------
 * Filename: OEM_SMBUS.H
 * Function: OEM_SMBUS.C Include Definition
 *
 * Copyright (c) 2006-2009, ITE Tech. Inc. All Rights Reserved.
 *---------------------------------------------------------------------------*/
#ifndef OEM_SMBUS_H
#define OEM_SMBUS_H

/*-----------------------------------------------------------------------------
 * Parameter Definition
 *---------------------------------------------------------------------------*/
#define CPUThermalAddr  0x98
#define CPULocalCmd     0x00    // Read Local temprature
#define CPURemoteCmd    0x01    // Read remote temprature
#define CPURemoteOVTCmd 0x19    // HW shutdown command
#define CPULocalOVTCmd  0x20    // HW shutdown command

#define CPU_Local_OVT   80      // Set HW shutdown point of CPU local temperature
#define CPU_Remote_OVT  106     // Set HW shutdown point of CPU remote temperature
#define VGA_Local_OVT   85      // Set HW shutdown point of VGA local temperature
#define VGA_Remote_OVT  106     // Set HW shutdown point of VGA remote temperature
#define dummy_data      0x00

/*-----------------------------------------------------------------------------
 * External Calling Process
 *---------------------------------------------------------------------------*/
extern BYTE Oem_ACPISMBusviaEC(void);
extern BYTE bInitThermalChip(void);

extern void SMBusA_Scan_Center(void);
extern bit  SMBusA_Device1_Init(void);
extern bit  SMBusA_Device1(void);
extern bit  SMBusA_Device2_Init(void);
extern bit  SMBusA_Device2(void);

extern void SMBusB_Scan_Center(void);
extern bit  SMBusB_Device1_Init(void);
extern bit  SMBusB_Device1(void);
extern bit  SMBusB_Device2_Init(void);
extern bit  SMBusB_Device2(void);

extern bit  HandleSMBusData(void);
extern bit  SMBusDataToMemory( BYTE SMB_NO );

/*-----------------------------------------------------------------------------
 * Internal Process Definition
 *---------------------------------------------------------------------------*/


//-----------------------------------------------------------------------------
#endif