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
// Common Header Include
//-----------------------------------------------------------------------------
#include "CORE_INCLUDE.H"
#include "OEM_INCLUDE.H"

/*****************************************************************************/
/* 8032 BANK REGISTER RAM AREA  (0x00 ~ 0x1F) ********************************/
/*****************************************************************************/
// 0x00 - 0x1F for Chip R0-R7 Bank 0-3

/*****************************************************************************/
/* INTERNAL BIT ADDRESS RAM AREA  (0x20 ~ 0x2F) ******************************/
/*****************************************************************************/
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
volatile unsigned char bdata Ccb42          _at_ 0x20;
sbit Ccb42_INTR_KEY         = Ccb42^0;
sbit Ccb42_INTR_AUX         = Ccb42^1;
sbit Ccb42_SYS_FLAG         = Ccb42^2;
sbit Ccb42_RV1              = Ccb42^3;
sbit Ccb42_DISAB_KEY        = Ccb42^4;
sbit Ccb42_DISAB_AUX        = Ccb42^5;
sbit Ccb42_XLATE_PC         = Ccb42^6;
sbit Ccb42_RV2              = Ccb42^7;

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
volatile unsigned char bdata Ext_Cb0        _at_ 0x21;
sbit Ext_Cb0_ENABLE_SCAN    = Ext_Cb0^0;
sbit Ext_Cb0_EN_AUX_PORT1   = Ext_Cb0^1;
sbit Ext_Cb0_EN_AUX_PORT2   = Ext_Cb0^2;
sbit Ext_Cb0_EN_AUX_PORT3   = Ext_Cb0^3;
sbit Ext_Cb0_EN_AUX_PORT4   = Ext_Cb0^4;
sbit Ext_Cb0_PS2_AT         = Ext_Cb0^5;
sbit Ext_Cb0_PORT_CONTROL   = Ext_Cb0^6;
sbit Ext_Cb0_AUTO_SWITCH    = Ext_Cb0^7;

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
volatile unsigned char bdata Ext_Cb1        _at_ 0x22;
sbit FastA20                = Ext_Cb1^0;
sbit SST_SPI_FLAG           = Ext_Cb1^1;
sbit FORCE_NO_XLATE         = Ext_Cb1^2;
sbit bNumPad_OnlyWithFn     = Ext_Cb1^3;
sbit bNumPad_UpLayer        = Ext_Cb1^4;
sbit bKB_MATRIX_SENSE       = Ext_Cb1^5;
sbit PS2_next_data          = Ext_Cb1^6;
sbit bWaitED_Command        = Ext_Cb1^7;

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
volatile unsigned char bdata MULPX          _at_ 0x23;
sbit EnableTP      		    = MULPX^0;
sbit MULPX_Mx_state2        = MULPX^1;
sbit MULPX_Mx_state3        = MULPX^2;
sbit MULPX_Multiplex        = MULPX^3;
sbit Read_MSDriver_ID       = MULPX^4;
sbit MULPX_RV1              = MULPX^5;
sbit MULPX_RV2              = MULPX^6;
sbit MULPX_RV3              = MULPX^7;

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
volatile unsigned char bdata Led_Data       _at_ 0x24;
sbit Led_Data_SCROLL        = Led_Data^0;
sbit Led_Data_NUM           = Led_Data^1;
sbit Led_Data_CAPS          = Led_Data^2;
sbit Led_Data_OVL           = Led_Data^3;
sbit Led_Data_LED4          = Led_Data^4;
sbit Led_Data_LED5          = Led_Data^5;
sbit Led_Data_LED6          = Led_Data^6;
sbit Led_Data_LED7          = Led_Data^7;

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
volatile unsigned char bdata Scanner_State  _at_ 0x25;
sbit Scanner_State_NUM_LOCK = Scanner_State^0;
sbit Scanner_State_LEFT     = Scanner_State^1;
sbit Scanner_State_RIGHT    = Scanner_State^2;
sbit Scanner_State_ALT      = Scanner_State^3;
sbit Scanner_State_CONTROL  = Scanner_State^4;
sbit Scanner_State_OVERLAY  = Scanner_State^5;
sbit Scanner_State_FN       = Scanner_State^6;
sbit Scanner_State_FN_SILENT= Scanner_State^7;

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
volatile unsigned char bdata Service        _at_ 0x26;
sbit F_Service_PCI          = Service^0;
sbit F_Service_UNLOCK       = Service^1;
sbit F_Service_SEND         = Service^2;
sbit F_Service_Send_PS2     = Service^3;
sbit F_Service_PS2_IRQ      = Service^4;
sbit F_Service_PCI2         = Service^5;
sbit F_Service_MS_5         = Service^6;
sbit F_Service_KEYSCAN      = Service^7;

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
volatile unsigned char bdata Service1       _at_ 0x27;
sbit F_Service_PCI3         = Service1^0;
sbit F_Service_CIR          = Service1^1;
sbit F_Service_OEM_SRVC_3   = Service1^2;
sbit F_Service_MainLoTimer  = Service1^3;
sbit F_Service_OEM_LoTimer  = Service1^4;
sbit F_Service_LPCRST       = Service1^5;
sbit F_Service_OEM_SRVC_1   = Service1^6;
sbit F_Service_OEM_SRVC_2   = Service1^7;

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
volatile unsigned char bdata AuxScanFlagS   _at_ 0x28;
sbit SendtoAUXFlag			= AuxScanFlagS^0;	
sbit ECSendtoAUXFlag        = AuxScanFlagS^1;
sbit StartScanAUXDevice     = AuxScanFlagS^2;
sbit StartENAUXDevice       = AuxScanFlagS^3;
sbit AUXInterfaceBusy       = AuxScanFlagS^4;
sbit AUXKBIFBusy            = AuxScanFlagS^5;
sbit AuxScanFlagS_RV1       = AuxScanFlagS^6;
sbit AuxScanFlagS_RV2       = AuxScanFlagS^7;

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
volatile unsigned char bdata Gen_Info       _at_ 0x29;
sbit Gen_Info_LED_CHANGE    = Gen_Info^0;
sbit Gen_Info_TYPM_CHANGE   = Gen_Info^1;
sbit bKB_CommandWaitData    = Gen_Info^2;
sbit Gen_Info_SCAN_LOCKED   = Gen_Info^3;
sbit Gen_Info_PASS_ENABLE   = Gen_Info^4;
sbit Gen_Info_BREAK_AUXD    = Gen_Info^5;
sbit Gen_Info_BREAK_AUXK    = Gen_Info^6;
sbit Gen_Info_BREAK_SCAN    = Gen_Info^7;

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
volatile unsigned char bdata AuxTestFlagS   _at_ 0x2A;
sbit PS2StartBit            = AuxTestFlagS^0;
sbit MouseDriverIn          = AuxTestFlagS^1;
sbit KeyboardDriverIn       = AuxTestFlagS^2;
sbit AUX2ByteCommand        = AuxTestFlagS^3;
sbit AuxTestFlagS_RV1       = AuxTestFlagS^4;
sbit AuxTestFlagS_RV2       = AuxTestFlagS^5;
sbit AuxTestFlagS_RV3       = AuxTestFlagS^6;
sbit AuxTestFlagS_RV4       = AuxTestFlagS^7;

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
volatile unsigned char bdata SMBusFLAGs     _at_ 0x2B;
sbit bSMBA_GetData          = SMBusFLAGs^0;
sbit bSMBB_GetData          = SMBusFLAGs^1;
sbit bSMBC_GetData          = SMBusFLAGs^2;
sbit bSMBD_GetData          = SMBusFLAGs^3;
sbit SMBusFLAGs_RV1         = SMBusFLAGs^4;
sbit SMBusFLAGs_RV2         = SMBusFLAGs^5;
sbit SMBusFLAGs_RV3         = SMBusFLAGs^6;
sbit SMBusFLAGs_RV4         = SMBusFLAGs^7;

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
volatile unsigned char bdata AuxCmdFLAGs    _at_ 0x2C;
sbit bAUX_CMDF3_Invalid     = AuxCmdFLAGs^0;
sbit bAUX_CMDD4_Invalid     = AuxCmdFLAGs^1;
sbit bAUX1_TX_FAILED        = AuxCmdFLAGs^2;
sbit bAUX1_CHECK_DATA       = AuxCmdFLAGs^3;
sbit bAUX2_TX_FAILED        = AuxCmdFLAGs^4;
sbit bAUX2_CHECK_DATA       = AuxCmdFLAGs^5;
sbit bAUX3_TX_FAILED        = AuxCmdFLAGs^6;
sbit bAUX3_CHECK_DATA       = AuxCmdFLAGs^7;

// Reserved 0x2D,
/*****************************************************************************/
//      NOTE : 
/*****************************************************************************/
// Don't use bit addressble 0x2E and 0x2F.
/*****************************************************************************/

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
IBYTE       _R1                             _at_ 0x30;
IBYTE       _R2                             _at_ 0x31;
IBYTE       _R3                             _at_ 0x32;
IBYTE       _R4                             _at_ 0x33;
IBYTE       _R5                             _at_ 0x34;
IBYTE       _R6                             _at_ 0x35;
IWORD       _W1                             _at_ 0x36;
//-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
BITS_8      CORE_PMFLAG                     _at_ 0x38;
BITS_8      CORE_OSFLAG                     _at_ 0x39;
BITS_8      CORE_CONFIG1                    _at_ 0x3A;
BITS_8      CORE_CONFIG2                    _at_ 0x3B;
//-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
// Flag
//-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
FLAGS       Flag                            _at_ 0x3C;  // 2 bytes
TIMER       Timer_A                         _at_ 0x3E;
TIMER       Timer_B                         _at_ 0x3F;
//-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
data struct Sscan scan                      _at_ 0x40;
data struct Sscan ex_scan                   _at_ 0x43;
data union  Unew_keyl new_keyl              _at_ 0x46;
data union  Unew_keyl ex_new_keyl           _at_ 0x47;
data union  KEY new_keyh                    _at_ 0x48;
data union  KEY ex_new_keyh                 _at_ 0x49;
data union  KEY typematic                   _at_ 0x4A;
data union  KEY ex_typematic                _at_ 0x4B;
IBYTE       Fn_Make_Key1                    _at_ 0x4C;
IBYTE       HostCMD                         _at_ 0x4D;
IBYTE       SaveKBHICR                      _at_ 0x4E;

                // Reserved 0x4F                
//-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
IBYTE       SMBA_PinError                   _at_ 0x50;
IBYTE       SMBB_PinError                   _at_ 0x51;
IBYTE       SMBC_PinError                   _at_ 0x52;
IBYTE       SMBD_PinError                   _at_ 0x53;
IBYTE       SMB_Check_Timer                 _at_ 0x54;
IBYTE       Tmp_Load                        _at_ 0x55;
IBYTE       PM3Cmd                          _at_ 0x56;
IBYTE       PM3Step                         _at_ 0x57;
IBYTE       PM3Data                         _at_ 0x58;
IBYTE       Timer0_Cnt                      _at_ 0x59;
IBYTE       KB_Command                      _at_ 0x5A;
IBYTE       PS2_ReTryCunt                   _at_ 0x5B;
                // Reserved 0x5C ~ 0x5E
IBYTE       Emulate8042Port                 _at_ 0x5F;
//-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
IBYTE       KBHICmd                         _at_ 0x60;
IBYTE       KBHIStep                        _at_ 0x61;
IBYTE       KBHIData                        _at_ 0x62;
IBYTE       PM1Cmd                          _at_ 0x63;
IBYTE       PM1Step                         _at_ 0x64;
IBYTE       PM1StepSP                       _at_ 0x65;
IBYTE       PM1Data                         _at_ 0x66;
IBYTE       PM1Data1                        _at_ 0x67;
IBYTE       PM1Data2                        _at_ 0x68;
IBYTE       PM2Cmd                          _at_ 0x69;
IBYTE       PM2Step                         _at_ 0x6A;
IBYTE       PM2Data                         _at_ 0x6B;
//-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
IBYTE       *data Tmp_Pntr                  _at_ 0x6C;
//-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
#define MAX_SCAN_LINES      19      //(0x6D ~ 0x7F)
IBYTE       bscan_matrix[MAX_SCAN_LINES]    _at_ 0x6D;
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
IIBYTE		AUXInactiveCounter				_at_ 0x80;
IIBYTE		CheckAUXIndex					_at_ 0x81;
IIBYTE		TPResolution					_at_ 0x82;
IIBYTE		TPSamplingRate					_at_ 0x83;
IIBYTE		PS2_IRQ_Channel					_at_ 0x84;
IIBYTE		TPACKCounter					_at_ 0x85;
IIBYTE 	    AUXCmdFFCnt					    _at_ 0x86;
IIBYTE 	    AUXPortNum					    _at_ 0x87;
IIBYTE 	    AuxFlags[3] 				    _at_ 0x88;	
IIBYTE	    MousePendingBuf					_at_ 0x8B;
IIBYTE	    ScanAUXStepIndex                _at_ 0x8C; 
IIBYTE 	    AuxFlags2[3]                    _at_ 0x8D;  

IIBYTE      MS_ID_CUNT                      _at_ 0x90; 
IIBYTE      MS_DRIVER_TYPE                  _at_ 0x91; 
IIBYTE      KB_ACK_DELAY_CUNT               _at_ 0x92;
IIBYTE      KB_CodeSet                      _at_ 0x93;
IIBYTE      PS2KB_Typematic                 _at_ 0x94;
IIBYTE      PS2IFAck                        _at_ 0x95;   
IIBYTE      Save_Typematic                  _at_ 0x96;
IIBYTE 	    PS2PortxData[3] 			    _at_ 0x97;	
IIBYTE 	    KB_Typematic                    _at_ 0x9A;
IIBYTE 	    Main_MOUSE_CHN                  _at_ 0x9B;
IIBYTE 	    Main_KB_CHN                     _at_ 0x9C;
IIBYTE 	    Main_KB_ACK_CUNT                _at_ 0x9D;
IIBYTE 	    AUX_SCAN_INDEX                  _at_ 0x9E;
IIBYTE 	    AUX_SCAN_INDEX_HS               _at_ 0x9F;

IIBYTE 	    AUX_SCAN_TIMEOUT_CUNT           _at_ 0xA0;
IIBYTE 	    SKIP_AUX_SCAN_CUNT              _at_ 0xA1;
IIBYTE 	    AuxFlags_KB_CMD[3] 			    _at_ 0xA2;	
IIBYTE 	    AuxFlags_MS_CMD[3] 			    _at_ 0xA5;	
IIBYTE 	    AuxScanWDT			            _at_ 0xA8;	
IIBYTE      SMBUS_STATUS                    _at_ 0xA9;
IIBYTE		PS2_SSIRQ_Channel			    _at_ 0xAA;
IIBYTE		PS2_KBCmd_BK			        _at_ 0xAB;
//--- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- -
IIBYTE      _FlashCMD                       _at_ 0xAC;
IIBYTE      _SEND                           _at_ 0xAD;
BYTE code 	*idata Tmp_code_pointer 	    _at_ 0xAE;

IIBYTE      RamCodePort                     _at_ 0xB0;
IIBYTE      RunCodeInRamCount               _at_ 0xB1;
IIBYTE      ExitRamCodeCommand              _at_ 0xB2;
//--- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- -
//Key Scan Service
EXT_CB2       Ext_Cb2                       _at_ 0xB4;
key_state     temp_scanner_state            _at_ 0xB5;
XWORD *idata  Tmp_WPntr                     _at_ 0xB6;  //  2 bytes
XBYTE *idata  Scanner_RAM_Pntr              _at_ 0xB8;  //  2 bytes
XBYTE *idata  Extendkey_RAM_Pntr            _at_ 0xBA;  //  2 bytes
//--- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- -
INT_VAR       Int_Var                       _at_ 0xBC;  //  2 bytes
//--- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- -
XBYTE *idata  Tmp_XPntr                     _at_ 0xBE;  //  2 bytes
//--- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- -
//16 Bytes for flashing SPI         0xC0 - 0xCF         // 16 bytes
IIBYTE      SPIFlashData[16]                _at_ 0xC0;

//--- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- -
//Stack                             0xD0 - 0xFF         // 48 bytes
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
XBYTE   CORE_Version                        _at_ 0x0000;
XBYTE   Oem_BBKVer                          _at_ 0x0001;
XBYTE   Oem_CodeVer                         _at_ 0x0002;
XBYTE   Oem_ModelVer                        _at_ 0x0003;
XBYTE   Oem_SubVer                          _at_ 0x0004;
XBYTE   Oem_PCBVer                          _at_ 0x0005;
XBYTE   Oem_TestVer                         _at_ 0x0006;
XBYTE   KeyboardMatrixID                    _at_ 0x0007;
XBYTE   RSTStatus                           _at_ 0x0008;    // EC reset source. From RSTS Register (0x2006)
// ~Reserved


//-----------------------------------------------------------------------------
#define SCI_MemBase         0x0010  //-0x001F
#define EVENT_BUFFER_SIZE   9
XBYTE   SCI_Event_Buffer[10]                _at_ SCI_MemBase+0x00;
XBYTE   SCI_Event_In_Index                  _at_ SCI_MemBase+0x0A;
XBYTE   SCI_Event_Out_Index                 _at_ SCI_MemBase+0x0B;
XBYTE   SCI_StepTimer                       _at_ SCI_MemBase+0x0D;
XBYTE   SCI_LastQueryEvent                  _at_ SCI_MemBase+0x0E;
XBYTE   SCI_QueryEvent                      _at_ SCI_MemBase+0x0F;

//-----------------------------------------------------------------------------
#define SMI_MemBase         0x0020  //-0x002F
#define EVENT_BUFFER_SIZE   9
XBYTE   SMI_Event_Buffer[10]                _at_ SMI_MemBase+0x00;
XBYTE   SMI_Event_In_Index                  _at_ SMI_MemBase+0x0A;
XBYTE   SMI_Event_Out_Index                 _at_ SMI_MemBase+0x0B;
XBYTE   SMI_StepTimer                       _at_ SMI_MemBase+0x0D;
XBYTE   SMI_LastQueryEvent                  _at_ SMI_MemBase+0x0E;
XBYTE   SMI_QueryEvent                      _at_ SMI_MemBase+0x0F;

//-----------------------------------------------------------------------------
#define Record_MemBase      0x0030  //-0x003F
XBYTE   SCI_SMI_Event_Record[15]             _at_ Record_MemBase+0x00;
//-----------------------------------------------------------------------------
#define KBF_MemBase         0x0040  //-0x005F
#define KBF_SIZE            16
// Scanned keyboard buffer storage.
XBYTE   bKEY_BUFF[KBF_SIZE]                 _at_ KBF_MemBase+0x00;
// Keyboard password buffer storage.
XBYTE   Pass_Buff[8]                        _at_ KBF_MemBase+0x10;
XBYTE   Pass_Buff_Idx                       _at_ KBF_MemBase+0x18;
XBYTE   Pass_On                             _at_ KBF_MemBase+0x19;
XBYTE   Pass_Off                            _at_ KBF_MemBase+0x1A;
XBYTE   Pass_Make1                          _at_ KBF_MemBase+0x1B;
XBYTE   Pass_Make2                          _at_ KBF_MemBase+0x1C;
XBYTE   diode_key                           _at_ KBF_MemBase+0x1D;
XBYTE   PS_Debug_DeviceToEC                 _at_ KBF_MemBase+0x1E;
XBYTE   PS_Debug_ECToHost                   _at_ KBF_MemBase+0x1F;

//-----------------------------------------------------------------------------
#define KBS_MemBase         0x0060  //-0x006F
BYTE code *Scanner_Table_Pntr               _at_ KBS_MemBase+0x00;
BYTE code *Extendkey_Table_Pntr             _at_ KBS_MemBase+0x02;
XBYTE   bTMrepeat                           _at_ KBS_MemBase+0x04;
XBYTE   bTMdelay                            _at_ KBS_MemBase+0x05;
XBYTE   bTMcount                            _at_ KBS_MemBase+0x06;
XBYTE   PendingSendTimer                    _at_ KBS_MemBase+0x07;
XBYTE   KeyboardSendTimer                   _at_ KBS_MemBase+0x08;
XBYTE   SCI_SMI_Index_Record                _at_ KBS_MemBase+0x09;
XBYTE   PM3UnProcessCnt                     _at_ KBS_MemBase+0x0A;
XBYTE   PM2UnProcessCnt                     _at_ KBS_MemBase+0x0B;
XBYTE   ECUnProcessRec                      _at_ KBS_MemBase+0x0C;
XBYTE   ECUnProcessCnt                      _at_ KBS_MemBase+0x0D;
XBYTE   KBCUnProcessRec                     _at_ KBS_MemBase+0x0E;
XBYTE   KBCUnProcessCnt                     _at_ KBS_MemBase+0x0F;

//-----------------------------------------------------------------------------
#define PS2_MemBase         0x0070  //-0x007F
XBYTE   KBDataPending[3]                    _at_ PS2_MemBase+0x00;
XBYTE   MSDataPending[3]                    _at_ PS2_MemBase+0x04;
XBYTE   MXPortPending[3]                    _at_ PS2_MemBase+0x08;
XBYTE   KBPendingRXCount                    _at_ PS2_MemBase+0x0C;
XBYTE   KBPendingTXCount                    _at_ PS2_MemBase+0x0D;
XBYTE   MSPendingRXCount                    _at_ PS2_MemBase+0x0E;
XBYTE   MSPendingTXCount                    _at_ PS2_MemBase+0x0F;

//-----------------------------------------------------------------------------
#define SPI_MemBase         0x0080  //-0x008F
XBYTE	SPIIDBuf[4]							_at_ SPI_MemBase+0x00;	
XBYTE	SPIIndex							_at_ SPI_MemBase+0x04;	
XBYTE	SPIAddrCycle[3]						_at_ SPI_MemBase+0x05;	
XBYTE	SPIAEraseCmd						_at_ SPI_MemBase+0x08;	
XBYTE	SPIReadStatus						_at_ SPI_MemBase+0x09;	
XBYTE	SPIWriteStatus						_at_ SPI_MemBase+0x0A;	
XBYTE	*SPIDataPointer						_at_ SPI_MemBase+0x0B;	
XBYTE	SPIID								_at_ SPI_MemBase+0x0D;	
XBYTE	SPIReadIDCmd					    _at_ SPI_MemBase+0x0E;	
XBYTE   SSTDeviceID                         _at_ SPI_MemBase+0x0F;

//-----------------------------------------------------------------------------
#define HSPI_MemBase        0x0090  //-0x0097
XBYTE   HSPI_SelfRamCode_Index              _at_ HSPI_MemBase+0x00;
XBYTE   HSPI_tempcmd                        _at_ HSPI_MemBase+0x01;
XBYTE   HSPI_FPT_timeout                    _at_ HSPI_MemBase+0x02;
XBYTE   HSPI_Misc1                          _at_ HSPI_MemBase+0x03;
XBYTE   Service_HSPI                        _at_ HSPI_MemBase+0x04;

//-----------------------------------------------------------------------------
#define PECI_MemBase        0x0098  //-0x009F
XBYTE   PECI_CRC8                           _at_ PECI_MemBase+0x00; 
XBYTE   PECI_CompletionCode                 _at_ PECI_MemBase+0x01;

//-----------------------------------------------------------------------------
#define CIR_MemBase         0x00B1  //-0x00B4
XBYTE   CIRCounter                          _at_ CIR_MemBase+0x00;
XBYTE   CIRPresentKey                       _at_ CIR_MemBase+0x01;
XBYTE   CIRKeyOld                           _at_ CIR_MemBase+0x02;
XBYTE   CIRKeyChange                        _at_ CIR_MemBase+0x03;

//-----------------------------------------------------------------------------
#define OEMHook_MemBase     0x00B5  //-0x00BF
XBYTE   Timer5msCnt                         _at_ OEMHook_MemBase+0x00;
XBYTE   Timer5msCntB                        _at_ OEMHook_MemBase+0x01;
XBYTE   Timer100msCnt                       _at_ OEMHook_MemBase+0x02;
XBYTE   Timer100msCntB                      _at_ OEMHook_MemBase+0x03;
XBYTE   Timer100msCntC                      _at_ OEMHook_MemBase+0x04;
XBYTE   Timer1SecCnt                        _at_ OEMHook_MemBase+0x05;
XBYTE   Timer1MinCnt                        _at_ OEMHook_MemBase+0x06;
XBYTE   Timer125msCnt                       _at_ OEMHook_MemBase+0x07;
XBYTE   Timer250msCnt                       _at_ OEMHook_MemBase+0x08;
XBYTE   TimerOEM_Cnt0                       _at_ OEMHook_MemBase+0x09;
XBYTE   TimerOEM_Cnt1                       _at_ OEMHook_MemBase+0x0A;
//-----------------------------------------------------------------------------
#define GPIO_MemBase        0x00C0  //-0x00CF
XBYTE   DEBOUNCE_FLAG1                      _at_ GPIO_MemBase+0x00;
XBYTE   DEBOUNCE_FLAG2                      _at_ GPIO_MemBase+0x01;
XBYTE   DEBOUNCE_CONT0                      _at_ GPIO_MemBase+0x02;
XBYTE   DEBOUNCE_CONT1                      _at_ GPIO_MemBase+0x03;
XBYTE   DEBOUNCE_CONT2                      _at_ GPIO_MemBase+0x04;
XBYTE   DEBOUNCE_CONT3                      _at_ GPIO_MemBase+0x05;
XBYTE   DEBOUNCE_CONT4                      _at_ GPIO_MemBase+0x06;
XBYTE   DEBOUNCE_CONT5                      _at_ GPIO_MemBase+0x07;
XBYTE   DEBOUNCE_CONT6                      _at_ GPIO_MemBase+0x08;
XBYTE   DEBOUNCE_CONT7                      _at_ GPIO_MemBase+0x09;
//      ~Reserved

//-----------------------------------------------------------------------------
#define ADC_MemBase         0x00D0  //-0x00EF
XBYTE   ADC_FLAG                            _at_ ADC_MemBase+0x00;
XBYTE   ADC_FLAG2                           _at_ ADC_MemBase+0x01;
XBYTE   ADC_Timer                           _at_ ADC_MemBase+0x02;
XBYTE   ADC_Timer2                          _at_ ADC_MemBase+0x03;
//      ~Reserved
XWORD   ADC_VCC                             _at_ ADC_MemBase+0x08;
XWORD   ADC_VSTBY                           _at_ ADC_MemBase+0x0A;
XWORD   ADC_AVCC                            _at_ ADC_MemBase+0x0C;
XWORD   ADC_VBS                             _at_ ADC_MemBase+0x0E;
XWORD   ADC_0                               _at_ ADC_MemBase+0x10;
XWORD   ADC_1                               _at_ ADC_MemBase+0x12;
XWORD   ADC_2                               _at_ ADC_MemBase+0x14;
XWORD   ADC_3                               _at_ ADC_MemBase+0x16;
XWORD   ADC_4                               _at_ ADC_MemBase+0x18;
XWORD   ADC_5                               _at_ ADC_MemBase+0x1A;
XWORD   ADC_6                               _at_ ADC_MemBase+0x1C;
XWORD   ADC_7                               _at_ ADC_MemBase+0x1E;

//-----------------------------------------------------------------------------
#define FUNC_MemBase         0x00F0  //-0x00FF
XWORD   Fan1RPM                             _at_ FUNC_MemBase+0x00;
XWORD   Fan2RPM                             _at_ FUNC_MemBase+0x02;
XBYTE   _HOTKEY_FLAG1S                      _at_ FUNC_MemBase+0x04;
XBYTE   _HOTKEY_FLAG1H                      _at_ FUNC_MemBase+0x05;
XBYTE   _HOTKEY_FLAG2S                      _at_ FUNC_MemBase+0x06;
XBYTE   _HOTKEY_FLAG2H                      _at_ FUNC_MemBase+0x07;
XBYTE   _HOTKEY_FLAG3S                      _at_ FUNC_MemBase+0x08;
XBYTE   _HOTKEY_FLAG3H                      _at_ FUNC_MemBase+0x09;

XBYTE   KeyScanKSI                          _at_ FUNC_MemBase+0x0A;
XBYTE   KeyScanKSO                          _at_ FUNC_MemBase+0x0B;
XBYTE   KeyScanACK                          _at_ FUNC_MemBase+0x0C;
XBYTE   ExtKeyScanKSI                       _at_ FUNC_MemBase+0x0D;
XBYTE   ExtKeyScanKSO                       _at_ FUNC_MemBase+0x0E;
XBYTE   UtilityFlag                         _at_ FUNC_MemBase+0x0F;
//-----------------------------------------------------------------------------
/*****************************************************************************/
/*****************************************************************************/
// QUICK I/O CONTROL DEFINE
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
EC_PORT_IO  _GPIOA          _at_ 0x1601;
EC_PORT_IO  _GPIOB          _at_ 0x1602;
EC_PORT_IO  _GPIOC          _at_ 0x1603;
EC_PORT_IO  _GPIOD          _at_ 0x1604;
EC_PORT_IO  _GPIOE          _at_ 0x1605;
EC_PORT_IO  _GPIOF          _at_ 0x1606;
EC_PORT_IO  _GPIOG          _at_ 0x1607;
EC_PORT_IO  _GPIOH          _at_ 0x1608;
EC_PORT_IO  _GPIOI          _at_ 0x1609;
EC_PORT_IO  _GPIOJ          _at_ 0x160A;
EC_PORT_IO  _GPIOK          _at_ 0x160B;
EC_PORT_IO  _GPIOL          _at_ 0x160C;
EC_PORT_IO  _GPIOM          _at_ 0x160D;
//-----------------------------------------------------------------------------
/*****************************************************************************/
// END DEFINITION
/*****************************************************************************/

//-----------------------------------------------------------------------------

