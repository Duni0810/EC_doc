/*-----------------------------------------------------------------------------
 * Filename: CORE_MEMORY.C     For Chipset: ITE.IT85XX
 *
 * Function: Memory Definition for extern all code base reference
 *
 * [Memory Map Description]
 *
 * =Chip Internal= 0x00-0xFF
 * 0x00-0xFF     For Kernel and Chip Level use
 *
 ***************************************************
 * Chip 8500       : External ram 0x000-0x7FF
 * Chip Other 85XX : External ram 0x000-0xFFF
 ***************************************************
 * Memory Map :
 * 0x000-0x0FF   For Kernel Core/Module
 * 0x100-0x1FF   OEM RAM        
 * 0x200-0x2FF   OEM RAM
 * 0x300-0x3FF   OEM RAM        
 * 0x400-0x4FF   OEM RAM
 * 0x500-0x5FF   OEM RAM        
 * 0x600-0x6FF   For ITE flash utility,EC SPI rom read write function, and ramdebug function.
 * 0x700-0x7FF   For Keil C Complier and Ram Code Space (for SPI programming).
 * 0x800-0x8FF   OEM RAM 
 * 0x800-0x9FF   OEM RAM 
 * 0xA00-0xAFF   OEM RAM 
 * 0xB00-0xBFF   OEM RAM  
 * 0xC00-0xCFF   OEM RAM 
 * 0xD00-0xDFF   OEM RAM 
 * 0xE00-0xEFF   OEM RAM 
 * 0xF00-0xFFF   OEM RAM    
 *
 * Copyright (c) 2006-2009, ITE Tech. Inc. All Rights Reserved.
 *---------------------------------------------------------------------------*/

//-----------------------------------------------------------------------------
// Include Definition
//-----------------------------------------------------------------------------
#ifndef OEM_MEMORY_H
#define OEM_MEMORY_H

//-----------------------------------------------------------------------------
/*****************************************************************************/
/* PUBLIC EXTERN OEM_MEMORY.C AREA     ***************************************/
/*****************************************************************************/
//-----------------------------------------------------------------------------
extern void Clear_RAM_Range(WORD nAddr, WORD nSize);

//-----------------------------------------------------------------------------
/*****************************************************************************/
/* EXTERNAL RAM AREA      (0x0100 ~ 0x01FF) **********************************/
/*****************************************************************************/
//-----------------------------------------------------------------------------
extern XBYTE    BAT1_ManufacturerAccess;    //_at_ BAT1_MemBase+0x00;
extern XBYTE    BAT1_BatteryMode;           //_at_ BAT1_MemBase+0x02;
extern XBYTE    BAT1_Temperature;           //_at_ BAT1_MemBase+0x04;
extern XBYTE    BAT1_Voltage;               //_at_ BAT1_MemBase+0x06;
extern XBYTE    BAT1_Current;               //_at_ BAT1_MemBase+0x08;
extern XBYTE    BAT1_AverageCurrent;        //_at_ BAT1_MemBase+0x0A;
extern XBYTE    BAT1_MaxError;              //_at_ BAT1_MemBase+0x0C;
extern XBYTE    BAT1_RelativeStateOfChg;    //_at_ BAT1_MemBase+0x0E;
extern XBYTE    BAT1_AbsoluteOfCharge;      //_at_ BAT1_MemBase+0x10;
extern XBYTE    BAT1_RemainingCapacity;     //_at_ BAT1_MemBase+0x12;
extern XBYTE    BAT1_FullChargeCapacity;    //_at_ BAT1_MemBase+0x14;
extern XBYTE    BAT1_ChargingCurrent;       //_at_ BAT1_MemBase+0x16;
extern XBYTE    BAT1_ChargingVoltage;       //_at_ BAT1_MemBase+0x18;
extern XBYTE    BAT1_BatteryStatus;         //_at_ BAT1_MemBase+0x1A;
extern XBYTE    BAT1_CycleCount;            //_at_ BAT1_MemBase+0x1C;
extern XBYTE    BAT1_DesignCapacity;        //_at_ BAT1_MemBase+0x1E;
extern XBYTE    BAT1_DesignVoltage;         //_at_ BAT1_MemBase+0x20;
extern XBYTE    BAT1_SpecificationInfo;     //_at_ BAT1_MemBase+0x22;
extern XBYTE    BAT1_ManufactureDate;       //_at_ BAT1_MemBase+0x24;
extern XBYTE    BAT1_SerialNumber;          //_at_ BAT1_MemBase+0x26;
extern XBYTE    BAT1_ManufacturerName[];    //_at_ BAT1_MemBase+0x28;
extern XBYTE    BAT1_DeviceName[];          //_at_ BAT1_MemBase+0x36;
extern XBYTE    BAT1_DeviceChemistry[];     //_at_ BAT1_MemBase+0x43;
extern XBYTE    BAT1_ManufacturerData;      //_at_ BAT1_MemBase+0x49;

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
/*****************************************************************************/
/* EXTERNAL RAM AREA      (0x0200 ~ 0x02FF) **********************************/
/*****************************************************************************/
//-----------------------------------------------------------------------------
//------ SMBus 2 Host registers
#define ECSMB2Base      0x2D8
#define SMB2_PRTC       (* (XBYTE * )( ECSMB2Base + 0x00))
#define SMB2_STS        (* (XBYTE * )( ECSMB2Base + 0x01))
#define SMB2_ADDR       (* (XBYTE * )( ECSMB2Base + 0x02))
#define SMB2_CMD        (* (XBYTE * )( ECSMB2Base + 0x03))
#define SMB2_DATA       (* (XBYTE * )( ECSMB2Base + 0x04))
#define SMB2_BCNT       (* (XBYTE * )( ECSMB2Base + 0x24))
// SBS Only Alarm registers     RAM:325h-327h
#define SMB2_ALRA       (* (XBYTE * )( ECSMB2Base + 0x25))
#define SMB2_ALRD0      (* (XBYTE * )( ECSMB2Base + 0x26))
#define SMB2_ALRD1      (* (XBYTE * )( ECSMB2Base + 0x27))
// Non SBS Alarm registers      RAM:328h-32Ah
#define SMB22ALRA       (* (XBYTE * )( ECSMB2Base + 0x28))
#define SMB22ALRD0      (* (XBYTE * )( ECSMB2Base + 0x29))
#define SMB22ALRD1      (* (XBYTE * )( ECSMB2Base + 0x2A))

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
/*****************************************************************************/
/* EXTERNAL RAM AREA      (0x0300 ~ 0x03FF) **********************************/
/*****************************************************************************/
//-----------------------------------------------------------------------------
#define ECSpace         0x300

// EC Space Register (ECS/UNIWILL)
//#define EC_Bt1BIF_PU    (* (XBYTE * )( ECSpace + 0x00))
//#define EC_Bt1BST_ST    (* (XBYTE * )( ECSpace + 0x2A))
//#define EC_AC1PSR_ST    (* (XBYTE * )( ECSpace + 0x3C))

//=============================================================================
//------ SMBus Host registers
#define ECSMBBase       0x0300
#define SMB_PRTC        (* (XBYTE * )( ECSMBBase + 0x00))
#define SMB_STS         (* (XBYTE * )( ECSMBBase + 0x01))
#define SMB_ADDR        (* (XBYTE * )( ECSMBBase + 0x02))
#define SMB_CMD         (* (XBYTE * )( ECSMBBase + 0x03))
#define SMB_DATA        (* (XBYTE * )( ECSMBBase + 0x04))
#define SMB_DATA1       (* (XBYTE * )( ECSMBBase + 0x05))
#define SMB_BCNT        (* (XBYTE * )( ECSMBBase + 0x24))
// SBS Only Alarm registers     RAM:2FDh-2FFh
#define SMB_ALRA        (* (XBYTE * )( ECSMBBase + 0x25))
#define SMB_ALRD0       (* (XBYTE * )( ECSMBBase + 0x26))
#define SMB_ALRD1       (* (XBYTE * )( ECSMBBase + 0x27))
// Non SBS Alarm registers      RAM:300h-302h
#define SMB2ALRA        (* (XBYTE * )( ECSMBBase + 0x28))
#define SMB2ALRD0       (* (XBYTE * )( ECSMBBase + 0x29))
#define SMB2ALRD1       (* (XBYTE * )( ECSMBBase + 0x2A))

//=============================================================================
#define POWER_FLAG1     (*(XBYTE *)0x0340)  // (byte)
//=============================================================================
#define adapter_in      BIT0        // 1 AC adapter exist
#define power_on        BIT1        // 1 system power on
#define enter_S3        BIT2        // 1 host enter suspend to RAM mode
#define wait_PSW_off    BIT3        // 1 waiting for PSW released
#define enter_S4        BIT4        // 1 host enter suspend to HD mode
#define system_on       BIT5        // 1 system on (SWPG issued)
#define wait_HWPG       BIT6        // 1 wait hardware power good
#define rvcc_on         BIT7        // 1 RVCC turnned on

//=============================================================================
#define POWER_FLAG2     (*(XBYTE *)0x0341)  // (byte)
//=============================================================================
#define susb_on         BIT0        // 1 On SUSB Status
#define susc_on         BIT1        // 1 On SUSC Status
#define fan_on          BIT2        // 1 Fan on


//=============================================================================
#define SYS_FLAG1       (* (XBYTE * )( ECSpace + 0x042))
//=============================================================================
#define issue_SMI       BIT(0x00)
#define in_standby      BIT(0x01)
#define enable_SMI      BIT(0x02)
#define LCM_update      BIT(0x03)
#define flash_WaitLED   BIT(0x04)
#define main_sel        BIT(0x05)
#define delay_PCUSMI    BIT(0x06)
#define polling_BAT     BIT(0x07)

//=============================================================================
#define SYS_MISC1       (* (XBYTE * )( ECSpace + 0x043))
//=============================================================================
#define T_sensor0       BIT(0x00)
#define T_sensor1       BIT(0x01)
#define ACPI_OS         BIT(0x02)
#define T_sensor2       BIT(0x03)
#define EC_bank0        BIT(0x04)
#define EC_bank1        BIT(0x05)
#define EC_bank2        BIT(0x06)
#define EC_bank3        BIT(0x07)

#define BT1I            (* (XWORD * )( ECSpace + 0x046))
#define BT1C            (* (XWORD * )( ECSpace + 0x048))
#define BT1V            (* (XWORD * )( ECSpace + 0x04A))

#define CPU_temp        (* (XBYTE * )( ECSpace + 0x058))
#define System_temp     (* (XBYTE * )( ECSpace + 0x059))

#define SELECTOR_STATE1 (* (XBYTE * )( ECSpace + 0x063))
#define SELECTOR_STATE2 (* (XBYTE * )( ECSpace + 0x064))
#define OLD_SELSTATE1   (* (XBYTE * )( ECSpace + 0x06A))
#define OLD_SELSTATE2   (* (XBYTE * )( ECSpace + 0x06B))

#define ABAT_STATUS     (* (XBYTE * )( ECSpace + 0x071))
//=============================================================================
#define abat_in         BIT(0x00)
#define abat_full       BIT(0x01)
#define abat_destroy    BIT(0x02)
#define abat_trickle    BIT(0x03)
#define abat_RTalarm    BIT(0x04)
#define abat_low        BIT(0x05)
#define abat_dischg     BIT(0x06)
#define abat_dead       BIT(0x07)

#define ACHARGE_STATUS  (* (XBYTE * )( ECSpace + 0x072))
//=============================================================================
#define abat_charge     BIT(0x00)
#define abat_prechg     BIT(0x01)
#define abat_overtemp   BIT(0x02)
#define abat_NiMH       BIT(0x03)
#define abat_wakeup     BIT(0x04)
#define abat_InCharge   BIT(0x05)
#define abat_WakeChg    BIT(0x06)
#define abat_level3     BIT(0x07)

#define ABAT_MISC       (* (XBYTE * )( ECSpace + 0x073))
//=============================================================================
#define force_chgA      BIT(0x00)
#define force_discA     BIT(0x01)
#define abat_valid      BIT(0x02)
//EQU BIT2        ;
#define abat_next       BIT(0x03)
#define abat_inUSE      BIT(0x04)
#define abat_AutoLearn  BIT(0x05)
#define abat_ValidFull  BIT(0x06)
#define abat_LiHi       BIT(0x07)

#define ABAT_MISC2      (* (XBYTE * )( ECSpace + 0x074))
//=============================================================================
#define bat_HiVolt      BIT(0x00)
#define bat_IntCC       BIT(0x01)
//EQU BIT2        ;
//EQU BIT3        ;
//EQU BIT4        ;
//EQU BIT5        ;
//EQU BIT6        ;
//EQU BIT7        ;

//#define ABAT_ALARM    (* (XWORD * )( ECSpace + 0x075))
#define ABAT_ALARM      (* (XBYTE * )( ECSpace + 0x075))
//=============================================================================
#define SMBerror0       BIT(0x00)
#define SMBerror1       BIT(0x01)
#define SMBerror2       BIT(0x02)
#define SMBerror3       BIT(0x03)
#define FullyDsg        BIT(0x04)
#define FullyChg        BIT(0x05)
#define Dsg             BIT(0x06)
#define Initialized     BIT(0x07)
#define ABAT_ALARM2     (* (XBYTE * )( ECSpace + 0x076))
#define RemTime_alarm   BIT(0x00)
#define RemCap_alarm    BIT(0x01)
//EQU BITA        ;
#define TermDsg_alarm   BIT(0x03)
#define OverTemp_alarm  BIT(0x04)
//EQU BITD        ;
#define TermChg_alarm   BIT(0x06)
#define OverChg_alarm   BIT(0x07)

#define ABAT_CAP        (* (XBYTE * )( ECSpace + 0x077))
#define ABAT_VOLT       (* (XWORD * )( ECSpace + 0x078))
#define ABAT_CURRENT    (* (XWORD * )( ECSpace + 0x07A))
#define ABAT_TEMP       (* (XWORD * )( ECSpace + 0x07C))
#define ABAT_RCAP       (* (XWORD * )( ECSpace + 0x07E))
//#define ABAT_CELLS        (* (XBYTE * )( ECSpace + 0x07E))

//-----------------------------------------------------------------------------
//SECONDARY BATTERY INFORMATION NAMESPACES
//-----------------------------------------------------------------------------
#define BBAT_MODEL      (* (XBYTE * )( ECSpace + 0x080))

#define BBAT_STATUS     (* (XBYTE * )( ECSpace + 0x081))
//=============================================================================
#define bbat_in         BIT(0x00)
#define bbat_full       BIT(0x01)
#define bbat_destroy    BIT(0x02)
#define bbat_trickle    BIT(0x03)
#define bbat_RTalarm    BIT(0x04)
#define bbat_low        BIT(0x05)
#define bbat_dischg     BIT(0x06)
#define bbat_dead       BIT(0x07)

#define BCHARGE_STATUS  (* (XBYTE * )( ECSpace + 0x082))
//=============================================================================
#define bbat_charge     BIT(0x00)
#define bbat_prechg     BIT(0x01)
#define bbat_overtemp   BIT(0x02)
#define bbat_NiMH       BIT(0x03)
#define bbat_wakeup     BIT(0x04)
#define bbat_InCharge   BIT(0x05)
#define bbat_WakeChg    BIT(0x06)
#define bbat_level3     BIT(0x07)

#define BBAT_MISC       (* (XBYTE * )( ECSpace + 0x083))
//=============================================================================
#define force_chgB      BIT(0x00)
#define force_discB     BIT(0x01)
#define bbat_valid      BIT(0x02)
//EQU BIT2        ;
#define bbat_next       BIT(0x03)
#define bbat_inUSE      BIT(0x04)
#define bbat_AutoLearn  BIT(0x05)
#define bbat_ValidFull  BIT(0x06)
#define bbat_LiHi       BIT(0x07)

#define BBAT_MISC2      (* (XBYTE * )( ECSpace + 0x084))
//#define BBAT_ALARM    (* (XWORD * )( ECSpace + 0x085))
#define BBAT_ALARM      (* (XBYTE * )( ECSpace + 0x085))
#define BBAT_ALARM2     (* (XBYTE * )( ECSpace + 0x086))
#define BBAT_CAP        (* (XBYTE * )( ECSpace + 0x087))
#define BBAT_VOLT       (* (XWORD * )( ECSpace + 0x088))
#define BBAT_CURRENT    (* (XWORD * )( ECSpace + 0x08A))
#define BBAT_TEMP       (* (XWORD * )( ECSpace + 0x08C))
//#define BBAT_CELLS    (* (XBYTE * )( ECSpace + 0x08E))
#define BBAT_RCAP       (* (XWORD * )( ECSpace + 0x08E))

#define BT2I            (* (XWORD * )( ECSpace + 0x0C8))
#define BT2C            (* (XWORD * )( ECSpace + 0x0CA))
#define BT2V            (* (XWORD * )( ECSpace + 0x0CC))
#define BT2S            (* (XBYTE * )( ECSpace + 0x0CE))

#define FanDACLevel     (* (XBYTE * )( ECSpace + 0x0F0))

//=============================================================================
#define DebugFlag       (* (XBYTE * )( ECSpace + 0x0F1))
//=============================================================================
#define StopReadTemp    BIT0    // 1: to stop reading temperature
#define StopFlashLED    BIT1    // 1: to stop flashing LED


/*****************************************************************************/
/* EXTERNAL RAM AREA      (0x0400 ~ 0x04FF) **********************************/
/*****************************************************************************/
//-----------------------------------------------------------------------------
#define SHQRAM          0x0400

#define SEL_BAT_MODEL   (* (XBYTE * )( SHQRAM + 0x000))

//=============================================================================
#define SEL_BAT_STATUS  (* (XBYTE * )( SHQRAM + 0x001))
//=============================================================================
#define bat_in          BIT(0x00)
#define bat_full        BIT(0x01)
#define bat_destroy     BIT(0x02)
#define bat_trickle     BIT(0x03)
#define bat_RTalarm     BIT(0x04)
#define bat_low         BIT(0x05)
#define bat_dischg      BIT(0x06)
#define bat_dead        BIT(0x07)

//=============================================================================
#define SEL_CHARGE_STATUS   (* (XBYTE * )( SHQRAM + 0x002))
//=============================================================================
#define bat_charge      BIT(0x00)
#define bat_prechg      BIT(0x01)
#define bat_overtemp    BIT(0x02)
#define bat_NiMH        BIT(0x03)
#define bat_wakeup      BIT(0x04)
#define bat_InCharge    BIT(0x05)
#define bat_WakeChg     BIT(0x06)
#define bat_level3      BIT(0x07)

//=============================================================================
#define TIMER_FLAG      (* (XBYTE * )( SHQRAM + 0x020))
//=============================================================================

#define ShutDnCause     (*(XBYTE *)0x0423)
#define LED_FLASH_CNT   (* (XWORD * )( SHQRAM + 0x039))
#define PSW_COUNTER     (*(XWORD *)0x044E)  // (word) PSW override counter
#define VeryHotCnt      (* (XBYTE * )( SHQRAM + 0x0AF))
#define SysVeryHotCnt   (* (XBYTE * )( SHQRAM + 0x0B9))
#define POLLING_STEP    (* (XBYTE * )( SHQRAM + 0x040))

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/*****************************************************************************/
/* EXTERNAL RAM AREA      (0x0500 ~ 0x05FF) **********************************/
/*****************************************************************************/
//-----------------------------------------------------------------------------
//#define SMBA_MemBase                        0x0500
//-----------------------------------------------------------------------------
extern XBYTE        SMBA_DEV_DIR;           //_at_ 0x0500;
extern XBYTE        SMBA_DEV_CTL;           //_at_ 0x0501;
extern XBYTE        SMBA_DEV_CMD;           //_at_ 0x0502;
extern XWORD        SMBA_DEV_MEM;           //_at_ 0x0503;
extern XBYTE        SMBA_DEV_NO;            //_at_ 0x0505;
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
extern FLAGReg8     SMBA_DEV_STATUS;        //_at_ 0x0506;
#define _SMBA_DEV_STATUS        SMBA_DEV_STATUS.byte        //0x0506.Byte
#define bSMBA_Dev1_OnLine       SMBA_DEV_STATUS.fbit.bit0   //0x0506.Bit0
#define bSMBA_Dev1_InitOK       SMBA_DEV_STATUS.fbit.bit1   //0x0506.Bit1
#define bSMBA_Dev2_OnLine       SMBA_DEV_STATUS.fbit.bit2   //0x0506.Bit2
#define bSMBA_Dev2_InitOK       SMBA_DEV_STATUS.fbit.bit3   //0x0506.Bit3
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
extern XBYTE        SMBA_BYTE_CNT;          //_at_ 0x0507;
extern XBYTE        SMBA_GET_TIMER;         //_at_ 0x0508;
extern XBYTE        SMBA_Dev1_POLL_DIR;     //_at_ 0x0509;
extern XBYTE        SMBA_Dev1_POLL_SPEED;   //_at_ 0x050A;
extern XBYTE        SMBA_Dev2_POLL_DIR;     //_at_ 0x050B;
extern XBYTE        SMBA_Dev2_POLL_SPEED;   //_at_ 0x050C;
extern XBYTE        SMBA_Dev3_POLL_DIR;     //_at_ 0x050D;
extern XBYTE        SMBA_Dev3_POLL_SPEED;   //_at_ 0x050E;
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
extern FLAGReg8     SMBA_DEBUG_FLAG;        //_at_ 0x050F;
#define _SMBA_DEBUG_FLAG        SMBA_DEBUG_FLAG.byte        //0x50F.Byte
#define bSMBA_Dev1_Stop         SMBA_DEBUG_FLAG.fbit.bit0   //0x50F.Bit0
#define bSMBA_Dev2_Stop         SMBA_DEBUG_FLAG.fbit.bit1   //0x50F.Bit1
#define bSMBA_Dev3_Stop         SMBA_DEBUG_FLAG.fbit.bit2   //0x50F.Bit2
#define bSMBA_Dev4_Stop         SMBA_DEBUG_FLAG.fbit.bit3   //0x50F.Bit3
//#define bSMBA_Dev1_           SMBA_DEBUG_FLAG.fbit.bit4   //0x50F.Bit4
//#define bSMBA_Dev2_           SMBA_DEBUG_FLAG.fbit.bit5   //0x50F.Bit5
//#define bSMBA_Dev3_           SMBA_DEBUG_FLAG.fbit.bit6   //0x50F.Bit6
//#define bSMBA_Dev4_           SMBA_DEBUG_FLAG.fbit.bit7   //0x50F.Bit7
//-----------------------------------------------------------------------------
//#define SMBB_MemBase                        0x0510
//-----------------------------------------------------------------------------
extern XBYTE        SMBB_DEV_DIR;           //_at_ 0x0510;
extern XBYTE        SMBB_DEV_CTL;           //_at_ 0x0511;
extern XBYTE        SMBB_DEV_CMD;           //_at_ 0x0512;
extern XWORD        SMBB_DEV_MEM;           //_at_ 0x0513;
extern XBYTE        SMBB_DEV_NO;            //_at_ 0x0515;
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
extern FLAGReg8     SMBB_DEV_STATUS;        //_at_ 0x0516;
#define _SMBB_DEV_STATUS        SMBB_DEV_STATUS.byte        //0x0516.Byte
#define bSMBB_Dev1_OnLine       SMBB_DEV_STATUS.fbit.bit0   //0x0516.Bit0
#define bSMBB_Dev1_InitOK       SMBB_DEV_STATUS.fbit.bit1   //0x0516.Bit1
#define bSMBB_Dev2_OnLine       SMBB_DEV_STATUS.fbit.bit2   //0x0516.Bit2
#define bSMBB_Dev2_InitOK       SMBB_DEV_STATUS.fbit.bit3   //0x0516.Bit3
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
extern XBYTE        SMBB_BYTE_CNT;          //_at_ 0x0517;
extern XBYTE        SMBB_GET_TIMER;         //_at_ 0x0518;
extern XBYTE        SMBB_Dev1_POLL_DIR;     //_at_ 0x0519;
extern XBYTE        SMBB_Dev1_POLL_SPEED;   //_at_ 0x051A;
extern XBYTE        SMBB_Dev2_POLL_DIR;     //_at_ 0x051B;
extern XBYTE        SMBB_Dev2_POLL_SPEED;   //_at_ 0x051C;
extern XBYTE        SMBB_Dev3_POLL_DIR;     //_at_ 0x051D;
extern XBYTE        SMBB_Dev3_POLL_SPEED;   //_at_ 0x051E;
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
extern FLAGReg8     SMBB_DEBUG_FLAG;        //_at_ 0x051F;
#define _SMBB_DEBUG_FLAG        SMBB_DEBUG_FLAG.byte        //0x51F.Byte
#define bSMBB_Dev1_Stop         SMBB_DEBUG_FLAG.fbit.bit0   //0x51F.Bit0
#define bSMBB_Dev2_Stop         SMBB_DEBUG_FLAG.fbit.bit1   //0x51F.Bit1
#define bSMBB_Dev3_Stop         SMBB_DEBUG_FLAG.fbit.bit2   //0x51F.Bit2
#define bSMBB_Dev4_Stop         SMBB_DEBUG_FLAG.fbit.bit3   //0x51F.Bit3
//#define bSMBB_Dev1_           SMBB_DEBUG_FLAG.fbit.bit4   //0x51F.Bit4
//#define bSMBB_Dev2_           SMBB_DEBUG_FLAG.fbit.bit5   //0x51F.Bit5
//#define bSMBB_Dev3_           SMBB_DEBUG_FLAG.fbit.bit6   //0x51F.Bit6
//#define bSMBB_Dev4_           SMBB_DEBUG_FLAG.fbit.bit7   //0x51F.Bit7
//-----------------------------------------------------------------------------
//#define SMBC_MemBase                        0x0520
//-----------------------------------------------------------------------------
extern XBYTE        SMBC_DEV_DIR;           //_at_ 0x0520;
extern XBYTE        SMBC_DEV_CTL;           //_at_ 0x0521;
extern XBYTE        SMBC_DEV_CMD;           //_at_ 0x0522;
extern XWORD        SMBC_DEV_MEM;           //_at_ 0x0523;
extern XBYTE        SMBC_DEV_NO;            //_at_ 0x0525;
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
extern FLAGReg8     SMBC_DEV_STATUS;        //_at_ 0x0526;
#define _SMBC_DEV_STATUS        SMBC_DEV_STATUS.byte        //0x0526.Byte
#define bSMBC_Dev1_OnLine       SMBC_DEV_STATUS.fbit.bit0   //0x0526.Bit0
#define bSMBC_Dev1_InitOK       SMBC_DEV_STATUS.fbit.bit1   //0x0526.Bit1
#define bSMBC_Dev2_OnLine       SMBC_DEV_STATUS.fbit.bit2   //0x0526.Bit2
#define bSMBC_Dev2_InitOK       SMBC_DEV_STATUS.fbit.bit3   //0x0526.Bit3
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
extern XBYTE        SMBC_BYTE_CNT;          //_at_ 0x0527;
extern XBYTE        SMBC_GET_TIMER;         //_at_ 0x0528;
extern XBYTE        SMBC_Dev1_POLL_DIR;     //_at_ 0x0529;
extern XBYTE        SMBC_Dev1_POLL_SPEED;   //_at_ 0x052A;
extern XBYTE        SMBC_Dev2_POLL_DIR;     //_at_ 0x052B;
extern XBYTE        SMBC_Dev2_POLL_SPEED;   //_at_ 0x052C;
extern XBYTE        SMBC_Dev3_POLL_DIR;     //_at_ 0x052D;
extern XBYTE        SMBC_Dev3_POLL_SPEED;   //_at_ 0x052E;
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
extern FLAGReg8     SMBC_DEBUG_FLAG;        //_at_ 0x052F;
#define _SMBC_DEBUG_FLAG        SMBC_DEBUG_FLAG.byte        //0x52F.Byte
#define bSMBC_Dev1_Stop         SMBC_DEBUG_FLAG.fbit.bit0   //0x52F.Bit0
#define bSMBC_Dev2_Stop         SMBC_DEBUG_FLAG.fbit.bit1   //0x52F.Bit1
#define bSMBC_Dev3_Stop         SMBC_DEBUG_FLAG.fbit.bit2   //0x52F.Bit2
#define bSMBC_Dev4_Stop         SMBC_DEBUG_FLAG.fbit.bit3   //0x52F.Bit3
//#define bSMBC_Dev1_           SMBC_DEBUG_FLAG.fbit.bit4   //0x52F.Bit4
//#define bSMBC_Dev2_           SMBC_DEBUG_FLAG.fbit.bit5   //0x52F.Bit5
//#define bSMBC_Dev3_           SMBC_DEBUG_FLAG.fbit.bit6   //0x52F.Bit6
//#define bSMBC_Dev4_           SMBC_DEBUG_FLAG.fbit.bit7   //0x52F.Bit7
//-----------------------------------------------------------------------------
//#define SMBD_MemBase                        0x0530
//-----------------------------------------------------------------------------
extern XBYTE        SMBA_Dev1_FailCount;    //_at_ 0x0530
extern XBYTE        SMBA_Dev2_FailCount;    //_at_ 0x0531
extern XBYTE        SMBA_Dev3_FailCount;    //_at_ 0x0532
extern XBYTE        SMBB_Dev1_FailCount;    //_at_ 0x0533
extern XBYTE        SMBB_Dev2_FailCount;    //_at_ 0x0534
extern XBYTE        SMBB_Dev3_FailCount;    //_at_ 0x0535
extern XBYTE        SMBC_Dev1_FailCount;    //_at_ 0x0536
extern XBYTE        SMBC_Dev2_FailCount;    //_at_ 0x0537
extern XBYTE        SMBC_Dev3_FailCount;    //_at_ 0x0538
extern XBYTE        SMBX_ACCESS_CHANNEL;    //_at_ 0x0539
extern XBYTE        SMBX_ACCESS_DEV_ADDR;   //_at_ 0x053A
extern XBYTE        SMBX_ACCESS_DEV_CTL;    //_at_ 0x053B
extern XBYTE        SMBX_ACCESS_DEV_CMD;    //_at_ 0x053C
extern XWORD        SMBX_ACCESS_DEV_MEM;    //_at_ 0x053D
extern XBYTE        SMBX_DEBUG_MODE_ENABLE; //_at_ 0x053F
//-----------------------------------------------------------------------------
extern XBYTE   SMB_BUF[];               //_at_ SMBX_MemBase+0x00;
extern XBYTE   SMB_BUF2[];
//-----------------------------------------------------------------------------
#define SysPowState         (*(XBYTE *)0x0580)
#define CtrlPowStep         (*(XBYTE *)0x0581)
#define PowSeqDelay         (*(XWORD *)0x0582)
#define PowSeqWait          (*(XBYTE *)0x0584)
#define AutoColdBootTimer   (*(XBYTE *)0x0583)
#define ColdBootTimer       (*(XBYTE *)0x0584)
#define LockSwitchPowerOn   (*(XWORD *)0x0588)
#define PowSwOverTimer      (*(XWORD *)0x058A)
#define DeepSleepCountDown  (*(XBYTE *)0x058C)
#define DEBUG_RAM1          (*(XBYTE *)0x058F)

//-----------------------------------------------------------------------------
/*****************************************************************************/
/* EXTERNAL RAM AREA      (0x0600 ~ 0x06FF) **********************************/
/*****************************************************************************/
//-----------------------------------------------------------------------------
// For ITE flash utility,EC SPI rom read write function, and ramdebug function.

//-----------------------------------------------------------------------------
/*****************************************************************************/
/* EXTERNAL RAM AREA      (0x0700 ~ 0x07FF) **********************************/
/*****************************************************************************/
//-----------------------------------------------------------------------------
//For Keil C Complier and Ram Code Space (for SPI programming).

//-----------------------------------------------------------------------------
/*****************************************************************************/
/* EXTERNAL RAM AREA      (0x0800 ~ 0x08FF) **********************************/
/*****************************************************************************/
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/*****************************************************************************/
/* EXTERNAL RAM AREA      (0x0900 ~ 0x09FF) **********************************/
/*****************************************************************************/
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/*****************************************************************************/
/* EXTERNAL RAM AREA      (0x0A00 ~ 0x0AFF) **********************************/
/*****************************************************************************/
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/*****************************************************************************/
/* EXTERNAL RAM AREA      (0x0B00 ~ 0x0BFF) **********************************/
/*****************************************************************************/
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/*****************************************************************************/
/* EXTERNAL RAM AREA      (0x0C00 ~ 0x0CFF) **********************************/
/*****************************************************************************/
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/*****************************************************************************/
/* EXTERNAL RAM AREA      (0x0D00 ~ 0x0DFF) **********************************/
/*****************************************************************************/
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/*****************************************************************************/
/* EXTERNAL RAM AREA      (0x0E00 ~ 0x0EFF) **********************************/
/*****************************************************************************/
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/*****************************************************************************/
/* EXTERNAL RAM AREA      (0x0F00 ~ 0x0FFF) **********************************/
/*****************************************************************************/
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

/*****************************************************************************/
// END DEFINITION
/*****************************************************************************/

//-----------------------------------------------------------------------------
#endif

