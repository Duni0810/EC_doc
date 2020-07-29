/*-----------------------------------------------------------------------------
 * Filename: OEM_SMBUS.C
 * Function: OEM SMBUS HANDLE
 *
 * Copyright (c) 2006-2009, ITE Tech. Inc. All Rights Reserved.
 *---------------------------------------------------------------------------*/
#include "CORE_INCLUDE.H"
#include "OEM_INCLUDE.H"

/*-----------------------------------------------------------------------------
 * Local Parameter Definition
 *---------------------------------------------------------------------------*/
#define SUPPORT_SOFTWARE_I2C        0
//-----------------------------------------------------------------------------
#define SMBA_DEV1_ADDR              0x16    //Smart Battery
#define SMBA_DEV1_POLL_SET          20
#define SMBA_DEV2_ADDR              0x98    //Thermal Sensor IC
#define SMBA_DEV2_POLL_SET          200
#define SMBA_DEV3_ADDR              0x12    //Smart Charger
#define SMBA_DEV3_POLL_SET          5
//-----------------------------------------------------------------------------
#define SMBB_DEV1_ADDR              0x98    //Thermal Sensor IC
#define SMBB_DEV1_POLL_SET          200
#define SMBB_DEV2_ADDR              0x8C    //ITE Cap Sensor
#define SMBB_DEV2_POLL_SET          5
//-----------------------------------------------------------------------------
#define SMBC_DEV1_ADDR              0x8C    //ITE Cap Sensor
#define SMBC_DEV1_POLL_SET          40
 //-----------------------------------------------------------------------------
#define _SMB_QK_BYTE                0x40
#define _SMB_TX_BYTE                0x44
#define _SMB_RX_BYTE                0x45
#define _SMB_RD_BYTE                0x48
#define _SMB_WR_BYTE                0x49
#define _SMB_RD_WORD                0x4C
#define _SMB_WR_WORD                0x4D
#define _SMB_RD_BLCK                0x54
#define _SMB_WR_BLCK                0x55
//-----------------------------------------------------------------------------

#define IC1_LocalTemperature        (*(XBYTE*)0x0580)
#define IC1_ExternalTemperature     (*(XBYTE*)0x0581)
#define IC2_LocalTemperature        (*(XBYTE*)0x0582)
#define IC2_ExternalTemperature     (*(XBYTE*)0x0583)



//-----------------------------------------------------------------------------
// OEM Handle -- (Hook form CORE_ACPI.C)
// ACPI Spec.13.9 SMBus Host Controller In terface via Embedded Controller
//-----------------------------------------------------------------------------"
BYTE Oem_ACPISMBusviaEC(void)
{
    #if SUPPORTED_ACPI_SMB_EC
    /*
    //Selector command sample code
    if( (SMB_PRTC != 0x00) && (SMB_ADDR == 0x14) )
    {
        SMB_STS &= 0x00;
        SMB_PRTC &= 0x0F;
        (ECSelectorComd_table[SMB_PRTC])();
        SMB_STS |= 0x80;
        SMB_PRTC = 0x00;
        WriteSCI_QueryValue(EC_SMB_SCI_Number);
        return 0x01;    //Return Service Flag
    }
    */
    #endif

    return 0x00;    //Return Service Flag

}

/* ----------------------------------------------------------------------------
 * FUNCTION: SMBusA_Data_Center
 *
 * ------------------------------------------------------------------------- */
void SMBusA_Scan_Center(void)
{
    //-------------------------------------------------------------------------
    if( bSMBA_GetData || HOSTA_A > 0 )     return;
    //-------------------------------------------------------------------------
    if( (SMBPCTL_A&0x03) != 0x03 )
    {
        return;
    }
    //-------------------------------------------------------------------------
    switch(SMBA_DEV_DIR)
    {
        case 0: if( bSMBA_Dev1_OnLine )
                {
                    if( SMBA_Dev1_POLL_SPEED > 0 )
                    {
                        SMBA_Dev1_POLL_SPEED--;
                    }
                    else
                    {
                        SMBusA_Device1();
                        if( bSMBA_Dev1_InitOK )
                        {
                            SMBA_Dev1_POLL_SPEED = SMBA_DEV1_POLL_SET;
                        }
                        else
                        {
                            SMBA_Dev1_POLL_SPEED = 1;
                        }
                    }
                }
                SMBA_DEV_DIR = 1;
                break;

        case 1: if( SMBA_Dev2_POLL_SPEED > 0 )
                {
                    SMBA_Dev2_POLL_SPEED--;
                }
                else
                {
                    SMBusA_Device2();
                    SMBA_Dev2_POLL_SPEED = SMBA_DEV2_POLL_SET;
                }
                SMBA_DEV_DIR = 0;
                //SMBA_DEV_DIR = 2;
                break;
        //case 2: if( SMBusA_Device3() )    SMBA_DEV_DIR = x;
        //        break;
        //case 3: if( SMBusA_Device4() )    SMBA_DEV_DIR = x;
        //        break;
        default:
                SMBA_DEV_DIR = 0;
                break;
    }
    //-------------------------------------------------------------------------
}
//-----------------------------------------------------------------------------

/* ----------------------------------------------------------------------------
 * FUNCTION: SMBusB_Data_Center
 *
 * ------------------------------------------------------------------------- */
void SMBusB_Scan_Center(void)
{
    //-------------------------------------------------------------------------
    if( bSMBB_GetData || HOSTA_B > 0 )     return;
    //-------------------------------------------------------------------------
    if( (SMBPCTL_B&0x03) != 0x03 )
    {
        return;
    }
    //-------------------------------------------------------------------------
    switch(SMBB_DEV_DIR)
    {
        case 0:
                if( bSMBB_Dev1_OnLine )
                {
                    if( SMBB_Dev1_POLL_SPEED > 0 )
                    {
                        SMBB_Dev1_POLL_SPEED--;
                    }
                    else
                    {
                        SMBusB_Device1();
                        if( bSMBB_Dev1_InitOK )
                        {
                            SMBB_Dev1_POLL_SPEED = SMBB_DEV1_POLL_SET;
                        }
                        else
                        {
                            SMBB_Dev1_POLL_SPEED = 1;
                        }
                    }
                }
                SMBB_DEV_DIR = 1;
                break;
        /*
        case 1:
                if( SMBB_Dev2_POLL_SPEED > 0 )
                {
                    SMBB_Dev2_POLL_SPEED--;
                }
                else
                {
                    SMBusB_Device2();
                    SMBB_Dev2_POLL_SPEED = SMBB_DEV2_POLL_SET;
                }
                SMBB_DEV_DIR = 0;
                //SMBB_DEV_DIR = 2;
                break;
        */
        //case 2: if( SMBusB_Device3() )    SMBB_DEV_DIR = x;
        //        break;
        //case 3: if( SMBusB_Device4() )    SMBB_DEV_DIR = x;
        //        break;
        default:
                SMBB_DEV_DIR = 0;
                break;
    }
    //-------------------------------------------------------------------------

}
//-----------------------------------------------------------------------------

/* ----------------------------------------------------------------------------
 * FUNCTION: SMBusA_Device1
 *
 * ------------------------------------------------------------------------- */
#define _SIZE_InitBatteryATable         24
const sRSmbusStruct code _InitBatteryATable[] =
{
    { _SMB_RD_WORD, _CMD_ManufacturerAccess,    &BAT1_ManufacturerAccess    },
    { _SMB_RD_WORD, _CMD_BatteryMode,           &BAT1_BatteryMode           },
    { _SMB_RD_WORD, _CMD_Temperature,           &BAT1_Temperature           },
    { _SMB_RD_WORD, _CMD_Voltage,               &BAT1_Voltage               },
    { _SMB_RD_WORD, _CMD_Current,               &BAT1_Current               },
    { _SMB_RD_WORD, _CMD_AverageCurrent,        &BAT1_AverageCurrent        },
    { _SMB_RD_WORD, _CMD_MaxError,              &BAT1_MaxError              },
    { _SMB_RD_WORD, _CMD_RelativeStateOfCharge, &BAT1_RelativeStateOfChg    },
    { _SMB_RD_WORD, _CMD_AbsoluteStateOfCharge, &BAT1_AbsoluteOfCharge      },
    { _SMB_RD_WORD, _CMD_RemainingCapacity,     &BAT1_RemainingCapacity     },
    { _SMB_RD_WORD, _CMD_FullChargeCapacity,    &BAT1_FullChargeCapacity    },
    { _SMB_RD_WORD, _CMD_ChargingCurrent,       &BAT1_ChargingCurrent       },
    { _SMB_RD_WORD, _CMD_ChargingVoltage,       &BAT1_ChargingVoltage       },
    { _SMB_RD_WORD, _CMD_BatteryStatus,         &BAT1_BatteryStatus         },
    { _SMB_RD_WORD, _CMD_CycleCount,            &BAT1_CycleCount            },
    { _SMB_RD_WORD, _CMD_DesignCapacity,        &BAT1_DesignCapacity        },
    { _SMB_RD_WORD, _CMD_DesignVoltage,         &BAT1_DesignVoltage         },
    { _SMB_RD_WORD, _CMD_SpecificationInfo,     &BAT1_SpecificationInfo     },
    { _SMB_RD_WORD, _CMD_ManufactureDate,       &BAT1_ManufactureDate       },
    { _SMB_RD_WORD, _CMD_SerialNumber,          &BAT1_SerialNumber          },
    { _SMB_RD_BLCK, _CMD_ManufacturerName,      &BAT1_ManufacturerName      },
    { _SMB_RD_BLCK, _CMD_DeviceName,            &BAT1_DeviceName            },
    { _SMB_RD_BLCK, _CMD_DeviceChemistry,       &BAT1_DeviceChemistry       },
    { _SMB_RD_WORD, _CMD_ManufacturerData,      &BAT1_ManufacturerData      },
};
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
#define _SIZE_ReadBatteryATable         12
const sRSmbusStruct code _ReadBatteryATable[] =
{
    { _SMB_RD_WORD, _CMD_ManufacturerAccess,    &BAT1_ManufacturerAccess    },
    { _SMB_RD_WORD, _CMD_Temperature,           &BAT1_Temperature           },
    { _SMB_RD_WORD, _CMD_Voltage,               &BAT1_Voltage               },
    { _SMB_RD_WORD, _CMD_Current,               &BAT1_Current               },
    { _SMB_RD_WORD, _CMD_AverageCurrent,        &BAT1_AverageCurrent        },
    { _SMB_RD_WORD, _CMD_RelativeStateOfCharge, &BAT1_RelativeStateOfChg    },
    { _SMB_RD_WORD, _CMD_AbsoluteStateOfCharge, &BAT1_AbsoluteOfCharge      },
    { _SMB_RD_WORD, _CMD_RemainingCapacity,     &BAT1_RemainingCapacity     },
    { _SMB_RD_WORD, _CMD_FullChargeCapacity,    &BAT1_FullChargeCapacity    },
    { _SMB_RD_WORD, _CMD_ChargingCurrent,       &BAT1_ChargingCurrent       },
    { _SMB_RD_WORD, _CMD_ChargingVoltage,       &BAT1_ChargingVoltage       },
    { _SMB_RD_WORD, _CMD_BatteryStatus,         &BAT1_BatteryStatus         },
};
//-----------------------------------------------------------------------------
bit SMBusA_Device1(void)
{
    //-------------------------------------------------------------------------
    if( !bSMBB_Dev1_InitOK )
    {
        if( SMBA_Dev1_POLL_DIR >= _SIZE_InitBatteryATable )
        {
            SMBA_Dev1_POLL_DIR = 0;
            bSMBA_Dev1_InitOK  = 1;
        }
        SMBA_DEV_CTL = _InitBatteryATable[SMBA_Dev1_POLL_DIR].address;
        SMBA_DEV_CMD = _InitBatteryATable[SMBA_Dev1_POLL_DIR].cmd;
        SMBA_DEV_MEM = _InitBatteryATable[SMBA_Dev1_POLL_DIR].smbdata;
    }
    else
    {
        if( SMBA_Dev1_POLL_DIR >= _SIZE_ReadBatteryATable )
        {
            SMBA_Dev1_POLL_DIR = 0;
        }
        SMBA_DEV_CTL = _ReadBatteryATable[SMBA_Dev1_POLL_DIR].address;
        SMBA_DEV_CMD = _ReadBatteryATable[SMBA_Dev1_POLL_DIR].cmd;
        SMBA_DEV_MEM = _ReadBatteryATable[SMBA_Dev1_POLL_DIR].smbdata;
    }
    //-------------------------------------------------------------------------
    HOCMD_A  = SMBA_DEV_CMD;
    TRASLA_A = SMBA_DEV1_ADDR;

    if( (SMBA_DEV_CTL&0x01) == 0 )      TRASLA_A |= 0x01;

    HOCTL_A  = (SMBA_DEV_CTL&0xFC);     //Start
    SMBA_Dev1_POLL_DIR++;
    bSMBA_GetData = 1;
    SMBA_BYTE_CNT = 0;
    SMBA_DEV_NO   = 1;
    return TRUE;
}
//-----------------------------------------------------------------------------

/*
;;-----------------------------------------------------------------------------
;; List of ADM1032 Registers (Reference Specification)
;;-----------------------------------------------------------------------------
;; READ|WRITE|
;; ADDR|ADDR | Name                                    Power-On Default
;;-----+-----------------------------------------------------------------------
;;  00 | Not | Local Temperature Value                 0000 0000 (00h)
;;  01 | Not | External Temperature Value High Byte    0000 0000 (00h)
;;  02 | Not | Status Undefined
;;  03 | 09  | Configuration                           0000 0000 (00h)
;;  04 | 0A  | Conversion Rate                         0000 1000 (08h)
;;  05 | 0B  | Local Temperature High Limit            0101 0101 (55h)(+85¢XC)
;;  06 | 0C  | Local Temperature Low Limit             0000 0000 (00h)(0¢XC)
;;  07 | 0D  | External Temperature High Limit Hi Byte 0101 0101 (55h)(+85¢XC)
;;  08 | 0E  | External Temperature Low Limit Hi Byte  0000 0000 (00h)(0¢XC)
;;  10 | Not | Applicable External Temperature Lo Byte 0000 0000
;;  11 | 11  | External Temperature Offset High Byte   0000 0000
;;  12 | 12  | External Temperature Offset Low Byte    0000 0000
;;  13 | 13  | External Temperature High Limit Lo Byte 0000 0000
;;  14 | 14  | External Temperature Low Limit Lo Byte  0000 0000
;;  19 | 19  | External THERM Limit                    0101 0101 (55h)(+85¢XC)
;;  20 | 20  | Local THERM Limit                       0101 0101 (55h)(+85¢XC)
;;  21 | 21  | THERM Hysteresis                        0000 1010 (0Ah)(+10¢XC)
;;  22 | 22  | Consecutive ALERT                       0000 0001 (01h)
;;-----+-----------------------------------------------------------------------
*/
#define _THR_LocalTemperature           0x00
#define _THR_ExternalTemperature        0x01
#define _THR_Configuration              0x03
#define _THR_ConversionRate             0x04
#define _THR_LocalTemp_HighLimit        0x05
#define _THR_LocalTemp_LowLimit         0x06
#define _THR_ExternalTemp_HighLimit     0x07
#define _THR_ExternalTemp_LowLimit      0x08
#define _THW_Configuration              0x09
#define _THW_ConversionRate             0x0A
#define _THW_LocalTemp_HighLimit        0x0B
#define _THW_LocalTemp_LowLimit         0x0C
#define _THW_ExternalTemp_HighLimit     0x0D
#define _THW_ExternalTemp_LowLimit      0x0E

/* ----------------------------------------------------------------------------
 * FUNCTION: SMBusA_Device2
 *
 * ------------------------------------------------------------------------- */
#define _SIZE_ReadThermailIC1Table      2
const sRSmbusStruct code _ReadThermalIC1_Table[]=
{
    { _SMB_RD_BYTE, _THR_LocalTemperature,      &IC1_LocalTemperature       },
    { _SMB_RD_BYTE, _THR_ExternalTemperature,   &IC1_ExternalTemperature    },
};
//-----------------------------------------------------------------------------
bit SMBusA_Device2(void)
{
    //-------------------------------------------------------------------------
    if( !CORE_PMFLAG_S0 )       return TRUE;
    //-------------------------------------------------------------------------
    if( SMBA_Dev2_POLL_DIR >= _SIZE_ReadThermailIC1Table )
    {
        SMBA_Dev2_POLL_DIR = 0;
    }

    SMBA_DEV_CTL = _ReadThermalIC1_Table[SMBA_Dev2_POLL_DIR].address;
    SMBA_DEV_CMD = _ReadThermalIC1_Table[SMBA_Dev2_POLL_DIR].cmd;
    SMBA_DEV_MEM = _ReadThermalIC1_Table[SMBA_Dev2_POLL_DIR].smbdata;
    HOCMD_A  = SMBA_DEV_CMD;
    TRASLA_A = SMBA_DEV2_ADDR;

    if( (SMBA_DEV_CTL&0x01) == 0 )      TRASLA_A |= 0x01;

    HOCTL_A  = (SMBA_DEV_CTL&0xFC);     //Start
    SMBA_Dev2_POLL_DIR++;
    bSMBA_GetData = 1;
    SMBA_BYTE_CNT = 0;
    SMBA_DEV_NO   = 2;
    return TRUE;
}
//-----------------------------------------------------------------------------

/* ----------------------------------------------------------------------------
 * FUNCTION: SMBusA_Device2
 *
 * ------------------------------------------------------------------------- */
#define _SIZE_ReadThermailIC2Table      2
const sRSmbusStruct code _ReadThermalIC2_Table[]=
{
    { _SMB_RD_BYTE, _THR_LocalTemperature,      &IC2_LocalTemperature       },
    { _SMB_RD_BYTE, _THR_ExternalTemperature,   &IC2_ExternalTemperature    },
};
//-----------------------------------------------------------------------------
bit SMBusB_Device1(void)
{
    //-------------------------------------------------------------------------
    if( !CORE_PMFLAG_S0 )       return TRUE;
    //-------------------------------------------------------------------------
    if( SMBB_Dev1_POLL_DIR >= _SIZE_ReadThermailIC2Table )
    {
        SMBB_Dev1_POLL_DIR = 0;
    }
    //-------------------------------------------------------------------------
    SMBB_DEV_CTL = _ReadThermalIC2_Table[SMBB_Dev2_POLL_DIR].address;
    SMBB_DEV_CMD = _ReadThermalIC2_Table[SMBB_Dev2_POLL_DIR].cmd;
    SMBB_DEV_MEM = _ReadThermalIC2_Table[SMBB_Dev2_POLL_DIR].smbdata;
    //-------------------------------------------------------------------------
    HOCMD_B  = SMBB_DEV_CMD;
    TRASLA_B = SMBB_DEV2_ADDR;

    if( (SMBB_DEV_CTL&0x01) == 0 )      TRASLA_B |= 0x01;

    HOCTL_B  = (SMBB_DEV_CTL&0xFC);     //Start
    SMBB_Dev1_POLL_DIR++;
    bSMBB_GetData = 1;
    SMBB_BYTE_CNT = 0;
    SMBB_DEV_NO   = 2;

    return TRUE;

}
//-----------------------------------------------------------------------------

/* ----------------------------------------------------------------------------
 * FUNCTION: HandleSMBusData
 *
 * ------------------------------------------------------------------------- */
bit HandleSMBusData(void)
{
    bit bNeedData = 0;
    //-------------------------------------------------------------------------
    if( bSMBA_GetData )
    {
        bNeedData = 1;
        if( (HOSTA_A&0x1C) > 0 || (SMBA_GET_TIMER>20) )
        {   //Failed or Timeout Process
            if( SMBA_DEV_NO == 1 )
            {
                SMBA_Dev1_FailCount++;
                SMBA_Dev1_POLL_DIR = 0;     //Battery Polling Reset
            }
            else if( SMBA_DEV_NO == 2 )     SMBA_Dev2_FailCount++;
            else if( SMBA_DEV_NO == 3 )     SMBA_Dev3_FailCount++;
            bSMBA_GetData = 0;
            SMBA_GET_TIMER = 0;
            bNeedData = 0;
            HOSTA_A = 0xFE;
            if( SMBX_ACCESS_CHANNEL == 0x0A )   SMBX_ACCESS_CHANNEL = 0x00;
        }
        else if( (HOSTA_A&0x82) > 0 )
        {
            if( SMBusDataToMemory(0) )
            {
                bSMBA_GetData = 0;
                SMBA_GET_TIMER = 0;
                bNeedData = 0;
            }
            HOSTA_A = 0xFE;
            if( SMBX_ACCESS_CHANNEL == 0x0A )   SMBX_ACCESS_CHANNEL = 0x00;
        }
        else
        {
            if( SMBA_GET_TIMER < 255 )
            {
                SMBA_GET_TIMER++;
            }
        }
    }
    //-------------------------------------------------------------------------
    //if( bSMBB_GetData )
    //{
    //    bNeedData = 1;
    //    if( (HOSTA_B&0x02) > 0 )
    //    {
    //        SMBusDataToMemory( 1, D0REG_B, D1REG_B );
    //        bNeedData = 0;
    //    }
    //}
    //-------------------------------------------------------------------------
    return  bNeedData;
    //-------------------------------------------------------------------------
}
//-----------------------------------------------------------------------------

/* ----------------------------------------------------------------------------
 * FUNCTION: SMBusDataToMemory
 *
 * ------------------------------------------------------------------------- */
bit SMBusDataToMemory( BYTE SMB_NO )
{
    PORT_BYTE_PNTR MemoryPtr;

    BYTE SMBX_DEV_CTL, SMB_DAT_L, SMB_DAT_H, SMB_BLK_DAT;
    //-------------------------------------------------------------------------
    if( SMB_NO == 0 )
    {
        MemoryPtr    = (unsigned int*)SMBA_DEV_MEM;
        SMBX_DEV_CTL = SMBA_DEV_CTL;
        SMB_DAT_L    = D0REG_A;
        SMB_DAT_H    = D1REG_A;
        if( SMBX_DEV_CTL == _SMB_RD_BLCK )
        {
            SMB_BLK_DAT  = HOBDB_A;
            if( SMBA_BYTE_CNT == 0 )
            {
                SMBA_BYTE_CNT = SMB_DAT_L;
                if( SMBA_BYTE_CNT > 32 )    SMBA_BYTE_CNT = 32;
            }
            *MemoryPtr = SMB_BLK_DAT;
            SMBA_DEV_MEM++;
            SMBA_BYTE_CNT--;
            if( SMBA_BYTE_CNT > 0 )
            {
                if( SMBA_BYTE_CNT == 1 )
                {
                    HOCTL_A |= 0x20;
                }
                return FALSE;
            }
            return TRUE;
        }
    }
    //else if( SMB_NO == 1 )       MemoryPtr  = SMBB_DEV_MEM;
    //else if( SMB_NO == 2 )       MemoryPtr  = SMBC_DEV_MEM;


    //-------------------------------------------------------------------------
    switch( SMBX_DEV_CTL )
    {
        case _SMB_RD_WORD:
                *MemoryPtr = SMB_DAT_L;
                MemoryPtr++;
                *MemoryPtr = SMB_DAT_H;
                return TRUE;

        case _SMB_RD_BYTE:
                *MemoryPtr = SMB_DAT_L;
                return TRUE;

        default:
                break;
    }
    //-------------------------------------------------------------------------
    return TRUE;
    //-------------------------------------------------------------------------
}
//-----------------------------------------------------------------------------

/*-----------------------------------------------------------------------------
 * End
 *---------------------------------------------------------------------------*/