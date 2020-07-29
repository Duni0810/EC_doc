/*-----------------------------------------------------------------------------
 * Filename: CORE_MEMORY.H     For Chipset: ITE.IT85XX
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
// Common Header Include
//-----------------------------------------------------------------------------
#ifndef CORE_MEMORY_H
#define CORE_MEMORY_H

/*****************************************************************************/
/* COMMON DEFINE *************************************************************/
/*****************************************************************************/

//-----------------------------------------------------------------------------

/*****************************************************************************/
/* 8032 BANK REGISTER RAM AREA  (0x00 ~ 0x1F) ********************************/
/*****************************************************************************/
// 0x00 - 0x1F for Chip R0-R7 Bank 0-3
/*****************************************************************************/
/* INTERNAL BIT ADDRESS RAM AREA  (0x20 ~ 0x2F) ******************************/
/*****************************************************************************/
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
extern BBYTE Ccb42;             //_at_ 0x20;
extern bit Ccb42_INTR_KEY;
extern bit Ccb42_INTR_AUX;
extern bit Ccb42_SYS_FLAG;
extern bit Ccb42_RV1;
extern bit Ccb42_DISAB_KEY;
extern bit Ccb42_DISAB_AUX;
extern bit Ccb42_XLATE_PC;
extern bit Ccb42_RV2;

#define INTR_KEY     bit0
#define INTR_AUX     bit1
#define SYS_FLAG     bit2

#define DISAB_KEY    bit4
#define DISAB_AUX    bit5
#define XLATE_PC     bit6

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//-----------------------------------------------------------------------------
// Extended control byte.  Accessed through Commands E9h and EAh.
//-----------------------------------------------------------------------------
extern BBYTE Ext_Cb0;           //_at_ 0x21;
extern bit Ext_Cb0_ENABLE_SCAN;     // 1 = Enable Internal Keyboard.
extern bit Ext_Cb0_EN_AUX_PORT1;    // 1 = Enable Auxiliary Port 1.
extern bit Ext_Cb0_EN_AUX_PORT2;    // 1 = Enable Auxiliary Port 2.
extern bit Ext_Cb0_EN_AUX_PORT3;    // 1 = Enable Auxiliary Port 3.
extern bit Ext_Cb0_EN_AUX_PORT4;    // 1 = Enable Auxiliary Port 4.
extern bit Ext_Cb0_PS2_AT;
extern bit Ext_Cb0_PORT_CONTROL;
extern bit Ext_Cb0_AUTO_SWITCH;

#define ENABLE_SCAN  bit0   // 1 = Enable Internal Keyboard.
#define EN_AUX_PORT1 bit1   // 1 = Enable Auxiliary Port 1.
#define EN_AUX_PORT2 bit2   // 1 = Enable Auxiliary Port 2.
#define EN_AUX_PORT3 bit3   // 1 = Enable Auxiliary Port 3.
#define EN_AUX_PORT4 bit4   // 1 = Enable Auxiliary Port 4.
#define PS2_AT       bit5
#define PORT_CONTROL bit6
#define AUTO_SWITCH  bit7

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
extern BBYTE Ext_Cb1;           //_at_ 0x22;
extern bit FastA20;
extern bit SST_SPI_FLAG;
extern bit FORCE_NO_XLATE;
extern bit bNumPad_OnlyWithFn;
extern bit bNumPad_UpLayer;
extern bit bKB_MATRIX_SENSE;
extern bit PS2_next_data;
extern bit bWaitED_Command;

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
extern BBYTE MULPX;             //_at_ 0x23
extern bit EnableTP;
extern bit MULPX_Mx_state2;
extern bit MULPX_Mx_state3;
extern bit MULPX_Multiplex;
extern bit Read_MSDriver_ID;
extern bit MULPX_RV1;    
extern bit MULPX_RV2;    
extern bit MULPX_RV3;     

#define DISABLE_NKEY bit0  // 1 = Simple Matrix, 0 = Nkey.
#define OVL_STATE    bit1  // 1 = Overlay state on.
#define DISABLE_TB   bit2  // 1 = Internal pointing device Disabled.
#define ACPI_ENABLED bit3  // 1 = ACPI mode, 0 = Legacy mode.
#define FN_CANCEL    bit4  // 1 = Any shift key clears Fn state.
#define NumLockC_OVR bit5  // 1 = Overlay controlled by NumLock.
#define DIAG_MODE    bit6  // 1 = Row/Col output, 0 = scan output.
#define ENABLE_HRDW  bit7  // 1 = Enable Auxiliary Port 4.

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//----------------------------------------------------------------------------
// tate of local keyboard LEDs.
//----------------------------------------------------------------------------
extern BBYTE Led_Data;          //_at_ 0x24;
extern bit Led_Data_SCROLL;
extern bit Led_Data_NUM;
extern bit Led_Data_CAPS;
extern bit Led_Data_OVL;
extern bit Led_Data_LED4;
extern bit Led_Data_LED5;
extern bit Led_Data_LED6;
extern bit Led_Data_LED7;

#define all         byte
#define SCROLL      bit0
#define NUM         bit1
#define CAPS        bit2
#define OVL         bit3
#define LED4        bit4
#define LED5        bit5
#define LED6        bit6
#define LED7        bit7

//----------------------------------------------------------------------------
// Scanner_State
//----------------------------------------------------------------------------
extern BBYTE Scanner_State;     //_at_ 0x25;
extern bit Scanner_State_NUM_LOCK;
extern bit Scanner_State_LEFT;
extern bit Scanner_State_RIGHT;
extern bit Scanner_State_ALT;
extern bit Scanner_State_CONTROL;
extern bit Scanner_State_OVERLAY;
extern bit Scanner_State_FN;
extern bit Scanner_State_FN_SILENT;

//-----------------------------------------------------------------------------
// Events that require service in main.
//-----------------------------------------------------------------------------
extern BBYTE Service;           //_at_ 0x26;
extern bit F_Service_PCI;
extern bit F_Service_UNLOCK;
extern bit F_Service_SEND;
extern bit F_Service_Send_PS2;
extern bit F_Service_PS2_IRQ;
extern bit F_Service_PCI2;
extern bit F_Service_MS_5;
extern bit F_Service_KEYSCAN;

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
extern BBYTE Service1;          //_at_ 0x27;
extern bit F_Service_PCI3;
extern bit F_Service_CIR;
extern bit F_Service_OEM_SRVC_3;
extern bit F_Service_MainLoTimer;
extern bit F_Service_OEM_LoTimer;
extern bit F_Service_LPCRST;
extern bit F_Service_OEM_SRVC_1;
extern bit F_Service_OEM_SRVC_2;

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
extern BBYTE AuxScanFlagS;      //_at_ 0x28;
extern bit SendtoAUXFlag;	
extern bit ECSendtoAUXFlag;
extern bit StartScanAUXDevice;
extern bit StartENAUXDevice;
extern bit AUXInterfaceBusy;
extern bit AUXKBIFBusy;
extern bit AuxScanFlagS_RV1;
extern bit AuxScanFlagS_RV2;

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

//----------------------------------------------------------------------------
// Gen_Info
//----------------------------------------------------------------------------
extern BBYTE Gen_Info;          //_at_ 0x29;
extern bit Gen_Info_LED_CHANGE;
extern bit Gen_Info_TYPM_CHANGE;
extern bit bKB_CommandWaitData;
extern bit Gen_Info_SCAN_LOCKED;
extern bit Gen_Info_PASS_ENABLE;
extern bit Gen_Info_BREAK_AUXD;
extern bit Gen_Info_BREAK_AUXK;
extern bit Gen_Info_BREAK_SCAN;

#define LED_CHANGE   bit0  /* 1 = LED change in progress.           */
#define TYPM_CHANGE  bit1  /* 1 = Typmatic change in progress.      */
#define CODE_CHANGE  bit2  /* 1 = Scan code change.                 */
#define SCAN_LOCKED  bit3  /* 1 = Scanner disabled.                 */
#define PASS_ENABLE  bit4  /* 1 = Password is activated.            */
#define BREAK_AUXD   bit5  /* Used when keypad is on mouse port.    */
#define BREAK_AUXK   bit6
#define BREAK_SCAN   bit7  /* Break states for scanner/serial.      */

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
extern BBYTE AuxTestFlagS;      //_at_ 0x2A;
extern bit PS2StartBit;
extern bit MouseDriverIn;
extern bit KeyboardDriverIn;
extern bit AUX2ByteCommand;
extern bit AuxTestFlagS_RV1;
extern bit AuxTestFlagS_RV2;
extern bit AuxTestFlagS_RV3;
extern bit AuxTestFlagS_RV4;

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
extern BBYTE SMBusFLAGs;        //_at_ 0x2B;
extern bit bSMBA_GetData;
extern bit bSMBB_GetData;
extern bit bSMBC_GetData;
extern bit bSMBD_GetData;
extern bit SMBusFLAGs_RV1;      
extern bit SMBusFLAGs_RV2;    
extern bit SMBusFLAGs_RV3; 
extern bit SMBusFLAGs_RV4;  

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
extern BBYTE AuxCmdFLAGs;       //_at_ 0x2C;
extern bit bAUX_CMDF3_Invalid;
extern bit bAUX_CMDD4_Invalid;
extern bit bAUX1_TX_FAILED;
extern bit bAUX1_CHECK_DATA;
extern bit bAUX2_TX_FAILED;
extern bit bAUX2_CHECK_DATA;
extern bit bAUX3_TX_FAILED;
extern bit bAUX3_CHECK_DATA;
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

/*****************************************************************************/
/*****************************************************************************/
//      BIT ADDRESS END MARK
//                          >>----------->
//                                          DIRECT START MARK
/*****************************************************************************/
/*****************************************************************************/
/* INTERNAL DIRECT RAM AREA       (0x30 ~ 0x7F) ******************************/
/*****************************************************************************/
//-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
extern  IBYTE       _R1;    //_at_ 0x30;
extern  IBYTE       _R2;    //_at_ 0x31;
extern  IBYTE       _R3;    //_at_ 0x32;
extern  IBYTE       _R4;    //_at_ 0x33;
extern  IBYTE       _R5;    //_at_ 0x34;
extern  IBYTE       _R6;    //_at_ 0x35;
extern  IWORD       _W1;    //_at_ 0x36;0x37
//-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
extern  BITS_8      CORE_PMFLAG;    //_at_ 0x38;
#define _CORE_PMFLAG    CORE_PMFLAG.byte
#define CORE_PMFLAG_S0  CORE_PMFLAG.fbit.bit0
#define CORE_PMFLAG_S1  CORE_PMFLAG.fbit.bit1
#define CORE_PMFLAG_S3  CORE_PMFLAG.fbit.bit2
#define CORE_PMFLAG_S4  CORE_PMFLAG.fbit.bit3
#define CORE_PMFLAG_S5  CORE_PMFLAG.fbit.bit4
//#define CORE_PMFLAG    CORE_PMFLAG.fbit.bit5
//#define CORE_PMFLAG    CORE_PMFLAG.fbit.bit6
//#define CORE_FLASHED   CORE_PMFLAG.fbit.bit7

extern  BITS_8      CORE_OSFLAG;    //_at_ 0x39;
//#define CORE_OSFLAG    CORE_OSFLAG.fbit.bit0
//#define CORE_OSFLAG    CORE_OSFLAG.fbit.bit1
//#define CORE_OSFLAG    CORE_OSFLAG.fbit.bit2
//#define CORE_OSFLAG    CORE_OSFLAG.fbit.bit3
//#define CORE_OSFLAG    CORE_OSFLAG.fbit.bit4
//#define CORE_OSFLAG    CORE_OSFLAG.fbit.bit5
#define WAIT_SCI_CENTER     CORE_OSFLAG.fbit.bit6
#define OS_ACPI_Mode        CORE_OSFLAG.fbit.bit7


extern  BITS_8      CORE_CONFIG1;   //_at_ 0x3A;
#define DISABLE_WINKEY    CORE_CONFIG1.fbit.bit0
#define KB_USE_RAM_TABLE  CORE_CONFIG1.fbit.bit1
//#define CORE_CONFIG1    CORE_CONFIG1.fbit.bit2
//#define CORE_CONFIG1    CORE_CONFIG1.fbit.bit3
//#define CORE_CONFIG1    CORE_CONFIG1.fbit.bit4
//#define CORE_CONFIG1    CORE_CONFIG1.fbit.bit5
//#define CORE_CONFIG1    CORE_CONFIG1.fbit.bit6
//#define bAUX_MULTIPLEX    CORE_CONFIG1.fbit.bit7

extern  BITS_8      CORE_CONFIG2;   //_at_ 0x3B;
//#define CORE_CONFIG2    CORE_CONFIG2.fbit.bit0
//#define CORE_CONFIG2    CORE_CONFIG2.fbit.bit1
//#define CORE_CONFIG2    CORE_CONFIG2.fbit.bit2
//#define CORE_CONFIG2    CORE_CONFIG2.fbit.bit3
//#define CORE_CONFIG2    CORE_CONFIG2.fbit.bit4
//#define CORE_CONFIG2    CORE_CONFIG2.fbit.bit5
//#define CORE_CONFIG2    CORE_CONFIG2.fbit.bit6
//#define CORE_CONFIG2    CORE_CONFIG2.fbit.bit7
//-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
// Flag
//-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
extern  FLAGS   Flag;          //_at_ 0x3C;  // 2 bytes
#define SCAN_INH         bit0  // 1 = Scanner transmission inhibited.
#define SENDING_SCAN_RC  bit1  // For diagnostic mode.
#define PASS_READY       bit2  // Password loaded.
#define NEW_PRI_K        bit3  // Switch to new primary keyboard.
#define NEW_PRI_M        bit4  // Switch to new primary mouse.
#define VALID_SCAN       bit5
#define LED_ON           bit6
#define ENTER_LOW_POWER  bit7  // Go into low power mode.
#define ACPI_IN_PROGRESS bit8
#define ACPI_SMB_ALARM   bit9
#define LEGACY_SMB_ALARM bit10
#define ALT_PRESSED      bit11 // External keyboard ALT key state.
#define CTRL_PRESSED     bit12 // External keyboard CTRL key state.
#define DETECT_EC_B0     bit13 // Used to allow Insyde Software's
#define DETECT_EC_B1     bit14 // embedded controller to be detected.
#define Aux_Not_RESP     bit15

extern  TIMER       Timer_A;    //_at_ 0x3E;
#define HIF2_SAFETY  count      // Secondary host interface timer.
#define TMR_SCAN     bit4       // Scanner interval enabled.
#define TMR_GENTMO   bit5       // General timeouts.

extern  TIMER       Timer_B;    //_at_ 0x3F;
#define SEND_DELAY   count      // Internal device send delay count
                                // used with SEND_ENABLE.
#define SEND_ENABLE  bit4       // Internal device send request.
#define INH_CLEAR    bit5       // Delay to enable serial clock lines.
#define UNLOCK_TMO   bit6       // Delay for unlock request.
//                   bit7

//-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
extern  data struct Sscan scan;                         //_at_ 0x40;
//FIELD kbf_head:  4; /* Keyboard buffer head (0-15).           */
//FIELD kbf_tail:  4; /* Keyboard buffer tail (0-15).           */
//FIELD kbf_mark:  4; /* Keyboard buffer overflow marker (0-15).*/
//FIELD TMscale:   4; /* Typematic scale.                       */
//FIELD scan_line: 4;
//FIELD saf_make:  1; /* Scan activity flag - make.         */
//FIELD saf_break: 1; /* Scan activity flag - break.        */
//FIELD saf_keys:  1; /* Scan activity flag - keys active.  */

extern  data struct Sscan ex_scan;                      //_at_ 0x43;
extern  data union  Unew_keyl new_keyl;                 //_at_ 0x46;
extern  data union  Unew_keyl ex_new_keyl;              //_at_ 0x47;

// Holds new scan key address and debounce status.
extern  data union  KEY new_keyh;                       //_at_ 0x48;
// Holds new_keyh for typematic action.
extern  data union  KEY ex_new_keyh;                    //_at_ 0x49;
extern  data union  KEY typematic;                      //_at_ 0x4A;
extern  data union  KEY ex_typematic;                   //_at_ 0x4B;
extern  IBYTE       Fn_Make_Key1;                       //_at_ 0x4C;
extern  IBYTE       HostCMD;                            //_at_ 0x4D;
extern  IBYTE       SaveKBHICR;                         //_at_ 0x4E;
                // Reserved 0x4F   
//-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
extern  IBYTE       SMBA_PinError;                      //_at_ 0x50;
extern  IBYTE       SMBB_PinError;                      //_at_ 0x51;
extern  IBYTE       SMBC_PinError;                      //_at_ 0x52;
extern  IBYTE       SMBD_PinError;                      //_at_ 0x53;
extern  IBYTE       SMB_Check_Timer;                    //_at_ 0x54;
extern  IBYTE       Tmp_Load;                           //_at_ 0x55;
extern  IBYTE       PM3Cmd;                             //_at_ 0x56;
extern  IBYTE       PM3Step;                            //_at_ 0x57;
extern  IBYTE       PM3Data;                            //_at_ 0x58;
extern  IBYTE       Timer0_Cnt;                         //_at_ 0x59;
extern  IBYTE       KB_Command;                         //_at_ 0x5A;
extern  IBYTE       PS2_ReTryCunt;                      //_at_ 0x5B; 
                    // 0x5C ~ 0x5E

extern  IBYTE       Emulate8042Port;                    //_at_ 0x5F;
//-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
extern  IBYTE       KBHICmd;                            //_at_ 0x60;
extern  IBYTE       KBHIStep;                           //_at_ 0x61;
extern  IBYTE       KBHIData;                           //_at_ 0x62;
extern  IBYTE       PM1Cmd;                             //_at_ 0x63;
extern  IBYTE       PM1Step;                            //_at_ 0x64;
extern  IBYTE       PM1StepSP;                          //_at_ 0x65;
extern  IBYTE       PM1Data;                            //_at_ 0x66;
extern  IBYTE       PM1Data1;                           //_at_ 0x67;
extern  IBYTE       PM1Data2;                           //_at_ 0x68;
extern  IBYTE       PM2Cmd;                             //_at_ 0x69;
extern  IBYTE       PM2Step;                            //_at_ 0x6A;
extern  IBYTE       PM2Data;                            //_at_ 0x6B;
//-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
extern  IBYTE       *data Tmp_Pntr;                     //_at_ 0x6C;
//-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

extern  IBYTE       bscan_matrix[];                     // _at_ 0x6D;
// 19 Bytes for key scan (0x6D ~ 0x7F)
//-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

/*****************************************************************************/
/*****************************************************************************/
//      DIRECT END MARK
//                          >>----------->
//                                          INDIRECT START MARK
/*****************************************************************************/
/* INTERNAL INDIRECT RAM AREA     (0x80 ~ 0xFF) ******************************/
/*****************************************************************************/
//----------------------------------------------------------------------------
// Internal ram variables (Indirect)[0x80-0xCF]
//----------------------------------------------------------------------------
//PS2 Port Service
extern 	IIBYTE		AUXInactiveCounter;     // 0x80
extern	IIBYTE		CheckAUXIndex;          // 0x81
extern  IIBYTE		TPResolution;           // 0x82
extern	IIBYTE		TPSamplingRate;         // 0x83
extern 	IIBYTE      PS2_IRQ_Channel;	    // 0x84
extern  IIBYTE		TPACKCounter;           // 0x85
extern  IIBYTE 	    AUXCmdFFCnt;	        // 0x86
extern  IIBYTE 	    AUXPortNum;			    // 0x87
extern  IIBYTE 	    AuxFlags[];             // 0x88 ~ 0x8A
#define DEVICE_IS_MOUSE             BIT7
#define DEVICE_IS_KEYBOARD          BIT6
#define DEVICE_IS_MAIN_MOUSE        BIT5
#define DEVICE_IS_MAIN_KB           BIT4
#define DEVICE_NEED_CONFIG          BIT3
#define DEVICE_MS_DRIVER_NO_MATCH   BIT2
#define DEVICE_NEED_INIT            BIT1
#define DEVICE_IS_ATTACHED          BIT0

extern  IIBYTE	    MousePendingBuf;		// 0x8B
extern  IIBYTE	    ScanAUXStepIndex;       // 0x8C
extern  IIBYTE 	    AuxFlags2[];            // 0x8D ~ 0x8F
#define DEVICE_POWER_DOWN           BIT7
#define DEVICE_ENABLED_TIME_OUT     BIT6
#define DEVICE_IS_ENABLED           BIT5
#define DEVICE_NEED_ENABLED         BIT4

#define DEVICE_DISABLE_TIME_OUT     BIT2
#define DEVICE_IS_DISABLED          BIT1
#define DEVICE_NEED_DISABLED        BIT0

extern  IIBYTE      MS_ID_CUNT;             // 0x90
extern  IIBYTE      MS_DRIVER_TYPE;         // 0x91
extern  IIBYTE      KB_ACK_DELAY_CUNT;      // 0x92
extern  IIBYTE      KB_CodeSet;             // 0x93
extern  IIBYTE      PS2KB_Typematic;        // 0x94
extern  IIBYTE      PS2IFAck;               // 0x95
extern  IIBYTE      Save_Typematic;         // 0x96
extern  IIBYTE 	    PS2PortxData[3];        // 0x97
extern  IIBYTE 	    KB_Typematic;           // 0x98

extern  IIBYTE 	    Main_MOUSE_CHN;         // 0x9B
extern  IIBYTE 	    Main_KB_CHN;            // 0x9C
extern  IIBYTE 	    Main_KB_ACK_CUNT;       // 0x9D
extern  IIBYTE 	    AUX_SCAN_INDEX;         // 0x9E
extern  IIBYTE 	    AUX_SCAN_INDEX_HS;      // 0x9F
extern  IIBYTE 	    AUX_SCAN_TIMEOUT_CUNT;  // 0xA0
extern  IIBYTE 	    SKIP_AUX_SCAN_CUNT;     // 0xA1
extern  IIBYTE 	    AuxFlags_KB_CMD[]; 		// 0xA2 ~ 0xA4
#define KB_CMD_0xF5     BIT0
#define KB_CMD_0xF4     BIT1
#define KB_CMD_0xF3     BIT2
#define KB_CMD_0xED     BIT3

extern  IIBYTE 	    AuxFlags_MS_CMD[];      // 0xA5 ~ 0xA7
extern  IIBYTE 	    AuxScanWDT;             // 0xA8;

//--- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- -
extern  IIBYTE      SMBUS_STATUS;           //_at_ 0xA9
extern  IIBYTE		PS2_SSIRQ_Channel;      //_at_ 0xAA
extern  IIBYTE		PS2_KBCmd_BK;           //_at_ 0xAB
//--- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- -
extern  IIBYTE      _FlashCMD;              //_at_ 0xAC
extern  IIBYTE      _SEND;                  //_at_ 0xAD
extern  BYTE code 	*idata Tmp_code_pointer;//_at_ 0xAE
extern  IIBYTE      RamCodePort;            //_at_ 0xB0
extern  IIBYTE      RunCodeInRamCount;      //_at_ 0xB1
extern  IIBYTE      ExitRamCodeCommand;     //_at_ 0xB2

//--- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- -
//Key Scan Service
extern  EXT_CB2     Ext_Cb2;                //_at_ 0xB4
extern  key_state   temp_scanner_state;     //_at_ 0xB5
extern  XWORD *idata  Tmp_WPntr;            //_at_ 0xB6
extern  XBYTE *idata  Scanner_RAM_Pntr;     //_at_ 0xB8  //  2 bytes
extern  XBYTE *idata  Extendkey_RAM_Pntr;   //_at_ 0xBA  //  2 bytes
//--- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- -
extern  INT_VAR     Int_Var;                //_at_ 0xBC
//--- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- -
extern  XBYTE *idata    Tmp_XPntr;          //_at_ 0xBE
//--- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- -
//16 Bytes for flashing SPI         0xC0 - 0xCF
extern  IIBYTE      SPIFlashData[];         //_at_ 0xC0

//--- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- -
//Stack                             0xD0 - 0xFF
//
// Don't use Idata (0xD0 ~ 0xFF).
// Idata (0xD0 ~ 0xFF). for stack point.
//--- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- -

/*****************************************************************************/
/*****************************************************************************/
//      INTERNAL END MARK
//                          >>----------->
//                                          EXTERNAL START MARK
/*****************************************************************************/
/* EXTERNAL RAM AREA      (0x0000 ~ 0x00FF) **********************************/
/*****************************************************************************/
extern  XBYTE   CORE_Version;               //_at_ 0x0000;
extern  XBYTE   Oem_BBKVer;                 //_at_ 0x0001;
extern  XBYTE   Oem_CodeVer;                //_at_ 0x0002;
extern  XBYTE   Oem_ModelVer;               //_at_ 0x0003;
extern  XBYTE   Oem_SubVer;                 //_at_ 0x0004;
extern  XBYTE   Oem_PCBVer;                 //_at_ 0x0005;
extern  XBYTE   Oem_TestVer;                //_at_ 0x0006;
extern  XBYTE   KeyboardMatrixID;           //_at_ 0x0007;
extern  XBYTE   RSTStatus;                  //_at_ 0x0008;

//-----------------------------------------------------------------------------
//#define SCI_MemBase           0x0010  //-0x001F
#define EVENT_BUFFER_SIZE   9
extern  XBYTE   SCI_Event_Buffer[];         //_at_ 0x0010;
extern  XBYTE   SCI_Event_In_Index;         //_at_ 0x001A;
extern  XBYTE   SCI_Event_Out_Index;        //_at_ 0x001B;
extern  XBYTE   SCI_StepTimer;              //_at_ 0x001D;
extern  XBYTE   SCI_LastQueryEvent;         //_at_ 0x001E;
extern  XBYTE   SCI_QueryEvent;             //_at_ 0x001F;

//-----------------------------------------------------------------------------
// #define SMI_MemBase          0x0020  //-0x002F
//define EVENT_BUFFER_SIZE   9
extern  XBYTE   SMI_Event_Buffer[];         //_at_ 0x0020;
extern  XBYTE   SMI_Event_In_Index;         //_at_ 0x002A;
extern  XBYTE   SMI_Event_Out_Index;        //_at_ 0x002B;
extern  XBYTE   SMI_StepTimer;              //_at_ 0x002D;
extern  XBYTE   SMI_LastQueryEvent;         //_at_ 0x002E;
extern  XBYTE   SMI_QueryEvent;             //_at_ 0x002F;

//-----------------------------------------------------------------------------
// #define Record_MemBase       0x0030  //-0x003F
extern  XBYTE   SCI_SMI_Event_Record[];     //_at_ 0x0030;
//-----------------------------------------------------------------------------
// #define KBF_MemBase          0x0040  //-0x005F
#define KBF_SIZE            16
// Scanned keyboard buffer storage.
extern  XBYTE   bKEY_BUFF[];                //_at_ 0x0040;
// Keyboard password buffer storage.
extern  XBYTE   Pass_Buff[];                //_at_ 0x0050;
extern  XBYTE   Pass_Buff_Idx;              //_at_ 0x0058;
extern  XBYTE   Pass_On;                    //_at_ 0x0059;
extern  XBYTE   Pass_Off;                   //_at_ 0x005A;
extern  XBYTE   Pass_Make1;                 //_at_ 0x005B;
extern  XBYTE   Pass_Make2;                 //_at_ 0x005C;
extern  XBYTE   diode_key;                  //_at_ 0x005D;
extern  XBYTE   PS_Debug_DeviceToEC;        //_at_ 0x005E;
extern  XBYTE   PS_Debug_ECToHost;          //_at_ 0x005F;
//-----------------------------------------------------------------------------
// #define KBS_MemBase          0x0060  //-0x006F
extern  BYTE code *Scanner_Table_Pntr;      //_at_ 0x0060;
extern  BYTE code *Extendkey_Table_Pntr;    //_at_ 0x0062;
extern  XBYTE   bTMrepeat;                  //_at_ 0x0064;
extern  XBYTE   bTMdelay;                   //_at_ 0x0065;
extern  XBYTE   bTMcount;                   //_at_ 0x0066;
extern  XBYTE   PendingSendTimer;           //_at_ 0x0067;
extern  XBYTE   KeyboardSendTimer;          //_at_ 0x0068;
extern  XBYTE   SCI_SMI_Index_Record;       //_at_ 0x0069;
extern  XBYTE   PM3UnProcessCnt;            //_at_ 0x006A;
extern  XBYTE   PM2UnProcessCnt;            //_at_ 0x006B;
extern  XBYTE   ECUnProcessRec;             //_at_ 0x006C;
extern  XBYTE   ECUnProcessCnt;             //_at_ 0x006D;
extern  XBYTE   KBCUnProcessRec;            //_at_ 0x006E;
extern  XBYTE   KBCUnProcessCnt;            //_at_ 0x006F;
//-----------------------------------------------------------------------------
//#define PS2_MemBase         0x0070  //-0x007F
extern  XBYTE   KBDataPending[];            //_at_ 0x0070;
extern  XBYTE   MSDataPending[];            //_at_ 0x0074;
extern  XBYTE   MXPortPending[];            //_at_ 0x0078;
extern  XBYTE   KBPendingRXCount;           //_at_ 0x007C;
extern  XBYTE   KBPendingTXCount;           //_at_ 0x007D;
extern  XBYTE   MSPendingRXCount;           //_at_ 0x007E;
extern  XBYTE   MSPendingTXCount;           //_at_ 0x007F;
//-----------------------------------------------------------------------------
extern 	XBYTE	SPIIDBuf[];				    // 	_at_ 0x80
extern 	XBYTE	SPIIndex;				    // 	_at_ 0x84	
extern	XBYTE	SPIAddrCycle[];			    //	_at_ 0x85
extern	XBYTE	SPIAEraseCmd;			    //	_at_ 0x88
extern	XBYTE	SPIReadStatus;			    //	_at_ 0x89	
extern	XBYTE	SPIWriteStatus;			    //	_at_ 0x8A	
extern	XBYTE	*SPIDataPointer;		    //	_at_ 0x8B
#define SPIDataPointerLowByte (*(XBYTE*)( 0x08C))	//	_at_ 0x8C	overlay SPIDataPointer low byte
extern 	XBYTE	SPIID;					    // 	_at_ 0x8D
extern  XBYTE	SPIReadIDCmd;               //  _at_ 0x8E
extern 	XBYTE	SSTDeviceID;			    // 	_at_ 0x8F

//-----------------------------------------------------------------------------
//#define HSPI_MemBase        0x0090  //-0x0097
extern 	XBYTE   HSPI_SelfRamCode_Index;     // HSPI_MemBase+0x00;
extern 	XBYTE   HSPI_tempcmd;               // HSPI_MemBase+0x01;
extern 	XBYTE   HSPI_FPT_timeout;           // HSPI_MemBase+0x02;
extern 	XBYTE   HSPI_Misc1;                 // HSPI_MemBase+0x03;
#define HSPI_ReLoadHSPIRamCode  BIT0        // 

extern  XBYTE   Service_HSPI;               // HSPI_MemBase+0x04;

//-----------------------------------------------------------------------------
//#define PECI_MemBase        0x0098  //-0x009F
extern 	XBYTE   PECI_CRC8;                  // PECI_MemBase+0x00; 
extern 	XBYTE   PECI_CompletionCode;        // PECI_MemBase+0x01;
    
//-----------------------------------------------------------------------------
// #define CIR_MemBase          0x00B1  //-0x00B4
extern  XBYTE   CIRCounter;                 //_at_ 0x00B1;
extern  XBYTE   CIRPresentKey;              //_at_ 0x00B2;
extern  XBYTE   CIRKeyOld;                  //_at_ 0x00B3;
extern  XBYTE   CIRKeyChange;               //_at_ 0x00B4;
//-----------------------------------------------------------------------------
// #define OEMHook_MemBase      0x00B5  //-0x00BF
extern  XBYTE   Timer5msCnt;                //_at_ 0x00B5;
extern  XBYTE   Timer5msCntB;               //_at_ 0x00B6;
extern  XBYTE   Timer100msCnt;              //_at_ 0x00B7;
extern  XBYTE   Timer100msCntB;             //_at_ 0x00B8;
extern  XBYTE   Timer100msCntC;             //_at_ 0x00B9;
extern  XBYTE   Timer1SecCnt;               //_at_ 0x00BA;
extern  XBYTE   Timer1MinCnt;               //_at_ 0x00BB;
extern  XBYTE   Timer125msCnt;              //_at_ 0x00BC;
extern  XBYTE   Timer250msCnt;              //_at_ 0x00BD;
extern  XBYTE   TimerOEM_Cnt0;              //_at_ 0x00BE;
extern  XBYTE   TimerOEM_Cnt1;              //_at_ 0x00BF;
//-----------------------------------------------------------------------------
// #define GPIO_MemBase         0x00C0  //-0x00CF
extern  XBYTE   DEBOUNCE_FLAG1;             //_at_ 0X00C0;
extern  XBYTE   DEBOUNCE_FLAG2;             //_at_ 0X00C1;
extern  XBYTE   DEBOUNCE_CONT0;             //_at_ 0X00C2;
extern  XBYTE   DEBOUNCE_CONT1;             //_at_ 0X00C3;
extern  XBYTE   DEBOUNCE_CONT2;             //_at_ 0X00C4;
extern  XBYTE   DEBOUNCE_CONT3;             //_at_ 0X00C5;
extern  XBYTE   DEBOUNCE_CONT4;             //_at_ 0X00C6;
extern  XBYTE   DEBOUNCE_CONT5;             //_at_ 0X00C7;
extern  XBYTE   DEBOUNCE_CONT6;             //_at_ 0X00C8;
extern  XBYTE   DEBOUNCE_CONT7;             //_at_ 0X00C9;
//      ~Reserved

//-----------------------------------------------------------------------------
// #define ADC_MemBase          0x00D0  //-0x00EF
extern  XBYTE   ADC_FLAG;                   //_at_ 0x00D0;
extern  XBYTE   ADC_FLAG2;                  //_at_ 0x00D1;
extern  XBYTE   ADC_Timer;                  //_at_ 0x00D2;
extern  XBYTE   ADC_Timer2;                 //_at_ 0x00D3;

extern  XWORD   ADC_VCC;                    //_at_ 0x00D8;
extern  XWORD   ADC_VSTBY;                  //_at_ 0x00DA;
extern  XWORD   ADC_AVCC;                   //_at_ 0x00DC;
extern  XWORD   ADC_VBS;                    //_at_ 0x00DE;
extern  XWORD   ADC_0;                      //_at_ 0x00E0;
extern  XWORD   ADC_1;                      //_at_ 0x00E2;
extern  XWORD   ADC_2;                      //_at_ 0x00E4;
extern  XWORD   ADC_3;                      //_at_ 0x00E6;
extern  XWORD   ADC_4;                      //_at_ 0x00E8;
extern  XWORD   ADC_5;                      //_at_ 0x00EA;
extern  XWORD   ADC_6;                      //_at_ 0x00EC;
extern  XWORD   ADC_7;                      //_at_ 0x00EE;

//-----------------------------------------------------------------------------
// #define FUNC_MemBase         0x00F0  //-0x00FF
extern  XWORD   Fan1RPM;                    //_at_ 0x00F0;
extern  XWORD   Fan2RPM;                    //_at_ 0x00F2;
extern  XBYTE   _HOTKEY_FLAG1S;             //_at_ 0x00F4;
extern  XBYTE   _HOTKEY_FLAG1H;             //_at_ 0x00F5;
extern  XBYTE   _HOTKEY_FLAG2S;             //_at_ 0x00F6;
extern  XBYTE   _HOTKEY_FLAG2H;             //_at_ 0x00F7;
extern  XBYTE   _HOTKEY_FLAG3S;             //_at_ 0x00F8;
extern  XBYTE   _HOTKEY_FLAG3H;             //_at_ 0x00F9;

extern  XBYTE   KeyScanKSI;                 //_at_ 0x00FA;
extern  XBYTE   KeyScanKSO;                 //_at_ 0x00FB;
extern  XBYTE   KeyScanACK;                 //_at_ 0x00FC;
extern  XBYTE   ExtKeyScanKSI;              //_at_ 0x00FD;
extern  XBYTE   ExtKeyScanKSO;              //_at_ 0x00FE;

extern  XBYTE   UtilityFlag;                //_at_ 0x00FF;
#define KBDebug         BIT6                // 1: for keyboard matrix utility
#define SMBusDebug      BIT7                // 1: for smbus utility

//-----------------------------------------------------------------------------
/*****************************************************************************/
/*****************************************************************************/
// QUICK I/O CONTROL DEFINE
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#define _Pin0           bit0
#define _Pin1           bit1
#define _Pin2           bit2
#define _Pin3           bit3
#define _Pin4           bit4
#define _Pin5           bit5
#define _Pin6           bit6
#define _Pin7           bit7
#define _HIGH           1
#define _LOW            0
//-----------------------------------------------------------------------------
extern  EC_PORT_IO     _GPIOA;          // 0x1601
extern  EC_PORT_IO     _GPIOB;          // 0x1602
extern  EC_PORT_IO     _GPIOC;          // 0x1603
extern  EC_PORT_IO     _GPIOD;          // 0x1604
extern  EC_PORT_IO     _GPIOE;          // 0x1605
extern  EC_PORT_IO     _GPIOF;          // 0x1606
extern  EC_PORT_IO     _GPIOG;          // 0x1607
extern  EC_PORT_IO     _GPIOH;          // 0x1608
extern  EC_PORT_IO     _GPIOI;          // 0x1609
extern  EC_PORT_IO     _GPIOJ;          // 0x160A
extern  EC_PORT_IO     _GPIOK;          // 0x160B
extern  EC_PORT_IO     _GPIOL;          // 0x160C
extern  EC_PORT_IO     _GPIOM;          // 0x160D
//-----------------------------------------------------------------------------
/*****************************************************************************/
// END DEFINITION
/*****************************************************************************/

//-----------------------------------------------------------------------------
#endif
