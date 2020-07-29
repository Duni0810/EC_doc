/*-----------------------------------------------------------------------------
 * Filename: PROJECT.H
 * Function: Project Code Function Setting
 *
 * Copyright (c) 2006-2009, ITE Tech. Inc. All Rights Reserved.
 *---------------------------------------------------------------------------*/
#ifndef PROJECT_H
#define PROJECT_H

//-----------------------------------------------------------------------------
// ITE EC kernal code version define
//-----------------------------------------------------------------------------
#define KernelCode_Version      0x12

//-----------------------------------------------------------------------------
// ITE EC Function define
//-----------------------------------------------------------------------------
//#define HSPI
//#define PECI_Support
//#define ITE_EVBoard 

//-----------------------------------------------------------------------------
// EC Firmware OEM Version
//
#define EC_OEM_VER              12      //= V0.12.04
#define EC_SUB_VER              04      //= SUB Version
#define EC_TEST_VER             0x00    //= TEST Version
#define EC_MODEL_VER            0xED    //= Model Code ED
#define EC_PCB_VER              0xC0    //= PCB Rev.C0

//-----------------------------------------------------------------------------
// EC Boot Block Firmware OEM Version
#define EC_BBK_VER              0xB1    //= Ver B.1 Sample
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// Boot Block Module Setting (ITE_BOOT.C/OEM_BBLOCK.C)

#define SUPPORTED_EC_BOOT_BLOCK 0       //Enable Boot Block Module
#define BB_Service_EC           0       //Enable service EC Command/Data
#define BB_Service_PM2          0       //Enable service PMC2 Command/Data
#define BB_RamCode_WDT          0       //RamCode suppport WDT
#define BB_RamCode_useKBC       0       //RamCode use KBC Command/Data
#define BB_RamCode_useEC        0       //RamCode use EC Command/Data
#define BB_RamCode_usePM2       0       //RamCode use PM Channel-2 Command/Data

//-----------------------------------------------------------------------------
// DEBUG Environment Setting

// Boot-Block Debug
#define SUPPORTED_BB_RECORDER   1       //Remove Record Code from Source
#define BB_DebugRamAddr         0x0600
#define BB_DebugRamRange        0xFF
#define En_RecordBB60           0       //Enable Debug Port 60 Recorder
#define En_RecordBB64           0       //Enable Debug Port 64 Recorder
#define En_RecordBB62           0       //Enable Debug Port 62 Recorder
#define En_RecordBB66           0       //Enable Debug Port 66 Recorder
#define En_RecordBB68           0       //Enable Debug Port 68 Recorder
#define En_RecordBB6C           0       //Enable Debug Port 6C Recorder

//-----------------------------------------------------------------------------
// Model Difference Switch 
//
// ~Reserved for customer own

//-----------------------------------------------------------------------------
// Control SuperIO Register Table Handle by OEM
//
#ifdef HSPI
#define _Init_FMSSR				0x0E	// 0x1F 16M Bytes Share Rom 	
#else
//define _Init_FMSSR            0x1F    /* For 2M Bytes Share ROM */
#define _Init_FMSSR             0x0F    /* For 1M Bytes Share ROM */
//#define _Init_FMSSR           0x07    /* For 512K Bytes Share ROM */
#endif

/* Base address select 0x00:2E/2F 0x01:4E/4F 0x02:User define */
#define _Init_BADR              0x01    /* 1:4E/4F */
#define _Init_SWCBAHR           0x00    /* I/O Port High Byte */
#define _Init_SWCBALR           0x6E    /* I/O Port Low  Byte */
#define _Init_I2EC              0x03    /* Enable I2EC R/W */
#define OEM_InitSIO_Table       0       /* 1:OEM_MAIN.C control SIO Config.*/
#define _Init_PMC3              0       /* Enable PM Channel-3 */

/*****************************************************************************/
/* EXTERNAL ITE DEVICE SWITCH AREA *******************************************/
/*****************************************************************************/
#define ExternalGPIO_8301       0       /* 1:External GPIO ITE IT8301.*/
#define ENABLE_ITE_IT723X       0       /* 1:External ITE Cap Sensor IT723X.*/ 

/*****************************************************************************/
/* CODE CONTROL SWITCH AREA **************************************************/
/*****************************************************************************/
//-----------------------------------------------------------------------------
// PS2 INTERFACE FUNCTION
//
#define TouchPad_only           0   // 1 : Only touch pad device in this project
                                    /* Note : If TouchPad_only is set, remeber to call 
                                              vPS2DeviceInactive() function in power off sequnce
                                              include (S3, S4, and S5)¡C*/       
//-----------------------------------------------------------------------------
// KEYBOARD MATRIX FUNCTION
//
#define SUPPORT_MATRIX_UTILITY  1   //Enabled KU.EXE Matrix Utility Support
#define SUPPORT_KB_MATRIX_SCAN  1   //Enabled KB Matrix Scan Module
#define SUPPORT_KB_RAM_TABLE    0   //Enabled Matrix RAM Table Support
#define ENABLE_S5_KEYBOARD_INTR 0   //Enabled KB Interrupt when system S5
#define ENABLE_S3_KEYBOARD_INTR 1   //Enabled KB Interrupt when system S3
#define ENABLE_S3_ANKEY_WAKEUP  1   //Enabled KB Wake-Up when system S3
#define SUPPORT_KB_MATRIX_ID1   0
#define SUPPORT_KB_MATRIX_ID2   0
#define SUPPORT_KB_MATRIX_ID3   0
#define NUMPAD_USE_OEM          0
#define NUMPAD_MATH_USE_OEM     1
#define OEM_HOOK_KB_DATA        0

//-----------------------------------------------------------------------------
// SMBus Module Setting
//
#define USE_SMBUS_CHANNEL_A     1   //Set 1 if the pin PB.3/PB.4 for SMBus.
#define EN_S5_SMBUS_CHANNEL_A   1   //Set 1 if SMBus active in System S5.
#define USE_SMBUS_CHANNEL_B     1   //Set 1 if the pin PC.1/PC.2 for SMBus.
#define EN_S5_SMBUS_CHANNEL_B   1   //Set 1 if SMBus active in System S5.
#define USE_SMBUS_CHANNEL_C     0   //Set 1 if the pin PF.6/PF.7 for SMBus.
#define EN_S5_SMBUS_CHANNEL_C   0   //Set 1 if SMBus active in System S5.
#define USE_SMBUS_CHANNEL_D     0   //Set 1 if the pin PH.2/PH.1 for SMBus.
#define EN_S5_SMBUS_CHANNEL_D   0   //Set 1 if SMBus active in System S5.

//-----------------------------------------------------------------------------
// Power Switch Function Setting (x5ms)
//
#define T_POWSW_INTR    100     // 500ms timing for interrupt sequence
#define T_POWSW_OVER    50      //PowerSwitch override timeout debounce
#define T_POWSW_ToBTN   10      //PowerSwitch to PM PWRBTN# debounce
#define T_POWSW_LockON  200     // 1 Sec timer for PowerSwitch lock
#define T_POWSW_ONDB    20      // 100ms for PowerSwitch on debounce
#define T_POWSW_BBK     200     //Boot Block Entry debounce


//-----------------------------------------------------------------------------
// TIMERS (CORE_TIMER.CORE_ADC)
//
#define T_Timer_150us       (0xFFFF-167)
#define T_Timer_SEND        T_Timer_2ms 
#define T_Timer_1ms         (0xFFFF-767)
#define T_Timer_2ms         (0xFFFF-1534)
#define T_Timer_5ms         (0xFFFF-3833)
#define T_Timer_10ms        (0xFFFF-7660)
#define T_Timer_25ms        (0xFFFF-19165)
#define T_Timer_30ms        (0xFFFF-22998)
#define T_Timer_85ms        (0xFFFF-65534)
#define T_ACPI_BurstTime    T_Timer_2ms
#define T_ADC_Timeout       5

//-----------------------------------------------------------------------------
// ACPI Module Setting
//
#define SUPPORTED_ACPI_SMB_EC   0       //Enable ACPI SMBus via EC
#define EC_SMB_SCI_Number       0x20    // ACPI EC SMBus Event
#define SUPPORT_ACPI_SMI        0       //Use SMI in ACPI Mode(No recommend)
#define USE_SERVICE_EVENT_CENTER    1
/* ACPI EC SCI Interrupt Response Timing ( 8-16uSec).*/
#define SCI_IS_PD3_ALT          0       // Set 0 if use GPO mode for SCI
#define T_SCI_RESPONSE          5       // 0:10-12us,2:18-20us,4:26-28us
#define T_SCI_EVENT             25      // ~100us
#define T_SCI_STEP              40      //40x5ms=200ms step timing


//-----------------------------------------------------------------------------
// Legacy SMI Setting
//
#define SMI_IS_PD4_ALT          0       // Set 0 if use GPO mode for SMI
#define T_SMI_EVENT             25      // ~100us
#define T_SMI_STEP              40      //40x5ms=200ms step timing

//-----------------------------------------------------------------------------
// LED Function Setting
//
#define LED_SCROLLOCK_SUPPORTED 1       //OEM_LED.C
#define LED_NUMLOCK_SUPPORTED   1       //OEM_LED.C
#define LED_CAPSLOCK_SUPPORTED  1       //OEM_LED.C


//-----------------------------------------------------------------------------
// Flash Ram Code Channel
//
#define Enable_RamCode_B5_B6    0
#define ITE_EC_CHIP_8510        0

#define RamCode_useKBC          1       //RamCode use KBC Command/Data
#define RamCode_useEC           0       //RamCode use EC Command/Data
#define RamCode_usePM2          0       //RamCode use PM Channel-2 Command/Data

#define RamCode_ExitCMD1        0x55    //Setting Quit RamCode Command 1
#define RamCode_ExitCMD2        0x95    //Setting Quit RamCode Command 2
#define RamCode_ResetCMD        0xFE    //Setting RamCode Reset Chip(WDT)

#define RamCode_Sizes           0xFF    //RamCode Sizes

//-----------------------------------------------------------------------------
// CIR Device Function
//
#define SUPPORT_CIR_DEVICE      0

//-----------------------------------------------------------------------------
// One-Wire Device Function
//
#define SUPPORT_ONE_WIRE_MODULE     0
#define _T_ONE_500          200
#define _T_ONE_RDB          18
#define _T_ONE_WRB          22
#define _T_ONE_1us          1

#define DCache              (*(XBYTE *)0x1033)
#define OneWirePinCont      GPIO_F5_CTRL
#define OneWirePin          _GPIOF._Pin5
#define OneWireInput        OneWirePinCont=0x80
#define OneWireOutput       OneWirePinCont=0x44
#define OneWireHigh         OneWirePin=1
#define OneWireLow          OneWirePin=0

//-----------------------------------------------------------------------------
// ADC Module Setting (Kernel provided)
//
#define SUPPORTED_CORE_ADC  1       //Enable ADC CORE_ADC.C

#define SUPPORTED_VCH0_Scan 0
#define SetVCH0_ScanA       0x9F    //Set VCH0 1st Scan channel
#define SetVCH0_ScanB       0x9F    //Set VCH0 2nd Scan channel
#define SetVCH0_ScanC       0x9F    //Set VCH0 3rd Scan channel
#define SetVCH0_ScanD       0x9F    //Set VCH0 4th Scan channel
    
#define SUPPORTED_VCH1_Scan 1
#define SetVCH1_ScanA       0x80    //Set VCH1 1st Scan channel
#define SetVCH1_ScanB       0x81    //Set VCH1 2nd Scan channel
#define SetVCH1_ScanC       0x82    //Set VCH1 3rd Scan channel
#define SetVCH1_ScanD       0x83    //Set VCH1 4th Scan channel

#define SUPPORTED_VCH2_Scan 1
#define SetVCH2_ScanA       0x84    //Set VCH2 1st Scan channel
#define SetVCH2_ScanB       0x85    //Set VCH2 2nd Scan channel
#define SetVCH2_ScanC       0x86    //Set VCH2 3rd Scan channel
#define SetVCH2_ScanD       0x9F    //Set VCH2 4th Scan channel

#define SUPPORTED_VCH3_Scan 1
#define SetVCH3_ScanA       0x87    //Set VCH3 1st Scan channel
#define SetVCH3_ScanB       0x8A    //Set VCH3 2nd Scan channel
#define SetVCH3_ScanC       0x8B    //Set VCH3 3rd Scan channel
#define SetVCH3_ScanD       0x9F    //Set VCH3 4th Scan channel

//-----------------------------------------------------------------------------
// DAC Module Setting
//
#define _Init_DAC           0x0F    /* DACCTRL register power-up init.*/

//-----------------------------------------------------------------------------
// Fan Control & Fan Tachometer Parameter
//
//#define   _FAN_TACH_BASE  0x107370/2          //8 Pole
#define _FAN_TACH_BASE      0x107370*2          //4 Pole
#define _FAN_TACH_BASE1     _FAN_TACH_BASE>>12
#define _FAN_TACH_BASE2     ((_FAN_TACH_BASE>>8)&0x0F)
#define _FAN_TACH_BASE3     ((_FAN_TACH_BASE>>4)&0x0F)
#define _FAN_TACH_BASE4     (_FAN_TACH_BASE&0x0F)

//-----------------------------------------------------------------------------
/*****************************************************************************/
/* EXTENDED CONTROL SWITCH AREA **********************************************/
/*****************************************************************************/
//-----------------------------------------------------------------------------
// Kernel Code
//
#define SET_EC_FULL_RUN             0   //1:EC not idle. always loop service.
#define SUPPORT_ExitRamCode_KEEP_OS 1
#define SUPPORT_EC_DEEP_SLEEP       0   //1:Enabled EC Deep Sleep.
#define SUPPORT_EC_WDT_RESET        0
#define T_WDTMR_COLD                10  //Default
#define T_WDTMR_OEM                 8   //Oem Handle Reset Default
#define T_WDTMR_FLASH               240 //Flash "RamCode" WatchDog Timeout
#define T_WDTMR_SLEEP               255 //Idle Timeout
#define SUPPORT_EC_SPI_MODULE       0
#define SUPPORT_EC_FLASH_CODE       1
#define SUPPORT_OEM_LOW_TIMER       1
#define DELAY_1MS_USE_WKCNR         1

//-----------------------------------------------------------------------------
// OEM_KBC6064 Decode Setting
//
#define SUPPORTED_KBC_8X    1   //Enable KBC Command 0x80 - 0x8F
#define SUPPORTED_KBC_9X    1   //Enable KBC Command 0x90 - 0x9F
#define SUPPORTED_KBC_AX    1   //Enable KBC Command 0xA0 - 0xAF
#define SUPPORTED_KBC_BX    1   //Enable KBC Command 0xB0 - 0xBF
#define SUPPORTED_KBC_CX    1   //Enable KBC Command 0xC0 - 0xCF
#define SUPPORTED_KBC_DX    1   //Enable KBC Command 0xD0 - 0xDF
#define SUPPORTED_KBC_EX    1   //Enable KBC Command 0xE0 - 0xEF

//-----------------------------------------------------------------------------
// OEM_PMC2 Code Setting (Power Management Channel-2)
//
#define SUPPORTED_PMC2_0X   1   //Enable PMC2 Command 0x00 - 0x0F
#define SUPPORTED_PMC2_1X   1   //Enable PMC2 Command 0x10 - 0x1F
#define SUPPORTED_PMC2_2X   1   //Enable PMC2 Command 0x20 - 0x2F
#define SUPPORTED_PMC2_3X   1   //Enable PMC2 Command 0x30 - 0x3F
#define SUPPORTED_PMC2_4X   1   //Enable PMC2 Command 0x40 - 0x4F
#define SUPPORTED_PMC2_5X   1   //Enable PMC2 Command 0x50 - 0x5F
#define SUPPORTED_PMC2_6X   1   //Enable PMC2 Command 0x60 - 0x6F
#define SUPPORTED_PMC2_7X   1   //Enable PMC2 Command 0x70 - 0x7F
#define SUPPORTED_PMC2_8X   1   //Enable PMC2 Command 0x80 - 0x8F
#define SUPPORTED_PMC2_9X   1   //Enable PMC2 Command 0x90 - 0x9F
#define SUPPORTED_PMC2_AX   1   //Enable PMC2 Command 0xA0 - 0xAF
#define SUPPORTED_PMC2_BX   1   //Enable PMC2 Command 0xB0 - 0xBF
#define SUPPORTED_PMC2_CX   1   //Enable PMC2 Command 0xC0 - 0xCF
#define SUPPORTED_PMC2_DX   1   //Enable PMC2 Command 0xD0 - 0xDF
#define SUPPORTED_PMC2_EX   1   //Enable PMC2 Command 0xE0 - 0xEF
#define SUPPORTED_PMC2_FX   1   //Enable PMC2 Command 0xF0 - 0XFF


//-----------------------------------------------------------------------------
// OEM Extended Matrix Contrl
//
#define ExtendMatrix    0       // The function of extend matrix
#define KeyScanKSO16    0       // Support KSO16 pin
#define KeyScanKSO17    0       // Support KSO17 pin
#define ExternMatrixGPO 0       // The function of GPO scan keyboard
#define ExtendScanPin   0       // The number of extend pin for scaning keyboard
#define GPOKeyPin1_HI() SET_BIT(GPIOJ,7)    // For extend keys
#define GPOKeyPin1_LO() CLR_BIT(GPIOJ,7)    // For extend keys
#define GPOKeyPin1Ctl   //GPIO_J7_CTRL

/*****************************************************************************/
/* DEBUG SWITCH AREA *********************************************************/
/*****************************************************************************/
//-----------------------------------------------------------------------------
// DEBUG Environment Setting
//
#define SCI_EVENT_LOG       1   //Support Record SCI Code from Source
#define SUPPORTED_RECORDER  1   //Set 0 ->Remove Record Code from Source
#define DebugRamAddr        0x0600
#define DebugRamRange       0xFF

#define En_RecordKB60   0       //Enable Debug Port 60 Recorder
#define En_Record60     0       //Enable Debug Port 60 Recorder
#define En_Record64     0       //Enable Debug Port 64 Recorder
#define En_Record62     0       //Enable Debug Port 62 Recorder
#define En_Record66     0       //Enable Debug Port 66 Recorder
#define En_Record68     0       //Enable Debug Port 68 Recorder
#define En_Record6C     0       //Enable Debug Port 6C Recorder

//-----------------------------------------------------------------------------
// UART debug interface
//-----------------------------------------------------------------------------
//#define UART_Debug    

#ifdef UART_Debug 
// 9600 bps-----------------
//#define R2HV            0xFF
//#define R2LV            0xE2  
// 9600 bps-----------------

// 19200 bps-----------------
#define R2HV            0xFF
#define R2LV            0xF1  
// 19200 bps-----------------
#endif

//-----------------------------------------------------------------------------
// PROJECT.H END HERE......
//-----------------------------------------------------------------------------
#endif

