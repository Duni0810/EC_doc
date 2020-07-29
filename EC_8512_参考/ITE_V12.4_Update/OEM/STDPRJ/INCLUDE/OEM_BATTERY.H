/*-----------------------------------------------------------------------------
 * Filename: OEM_BATTERY.H
 * Function: OEM_BATTERY.C Include Definition
 *
 * Copyright (c) 2006-2009, ITE Tech. Inc. All Rights Reserved.
 *---------------------------------------------------------------------------*/
#ifndef OEM_BATTERY_H
#define OEM_BATTERY_H

/*-----------------------------------------------------------------------------
 * Parameter Definition
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * Data Structure Prototype
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * External Calling Process
 *---------------------------------------------------------------------------*/
extern void CheckBatteryPlugInOut(void);


/*-----------------------------------------------------------------------------
 * Internal Process Definition
 *---------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
 * Reference Smart Battery Data Specification
 * The following definition summarizes the Smart Battery command set.
 *---------------------------------------------------------------------------*/
#define _SMB_BatteryAddr            0x16
#define _CMD_ManufacturerAccess     0x00
#define _CMD_RemainingCapacityAlarm 0x01
#define _CMD_RemainingTimeAlarm     0x02
#define _CMD_BatteryMode            0x03
#define _CMD_AtRate                 0x04
#define _CMD_AtRateTimeToFull       0x05
#define _CMD_AtRateTimeToEmpty      0x06
#define _CMD_AtRateOK               0x07
#define _CMD_Temperature            0x08
#define _CMD_Voltage                0x09
#define _CMD_Current                0x0A
#define _CMD_AverageCurrent         0x0B
#define _CMD_MaxError               0x0C
#define _CMD_RelativeStateOfCharge  0x0D
#define _CMD_AbsoluteStateOfCharge  0x0E
#define _CMD_RemainingCapacity      0x0F
#define _CMD_FullChargeCapacity     0x10
#define _CMD_RunTimeToEmpty         0x11
#define _CMD_AverageTimeToEmpty     0x12
#define _CMD_AverageTimeToFull      0x13
#define _CMD_ChargingCurrent        0x14
#define _CMD_ChargingVoltage        0x15
#define _CMD_BatteryStatus          0x16
#define _CMD_CycleCount             0x17
#define _CMD_DesignCapacity         0x18
#define _CMD_DesignVoltage          0x19
#define _CMD_SpecificationInfo      0x1A
#define _CMD_ManufactureDate        0x1B
#define _CMD_SerialNumber           0x1C
#define _CMD_ManufacturerName       0x20
#define _CMD_DeviceName             0x21
#define _CMD_DeviceChemistry        0x22
#define _CMD_ManufacturerData       0x23
#define _CMD_OptionalMfgFunction5   0x2F
#define _CMD_OptionalMfgFunction4   0x3C
#define _CMD_OptionalMfgFunction3   0x3D
#define _CMD_OptionalMfgFunction2   0x3E
#define _CMD_OptionalMfgFunction1   0x3F


#define _CMD_ChargerInputCurrent    0x3F

//-----------------------------------------------------------------------------
#endif