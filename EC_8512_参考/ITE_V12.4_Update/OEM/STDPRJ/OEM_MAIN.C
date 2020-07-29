/*-----------------------------------------------------------------------------
 * Filename: OEM_MAIN.C - OEM specific code
 * Function: OEM Hook from Kernel, Usually is timer handle
 *
 * Copyright (c) 2006-2009, ITE Tech. Inc. All Rights Reserved.
 *---------------------------------------------------------------------------*/
#include "CORE_INCLUDE.H"
#include "OEM_INCLUDE.H"

/*-----------------------------------------------------------------------------
 * Local Parameter Definition
 *---------------------------------------------------------------------------*/
#define SUPPORT_GET_FAN1_RPM            1
#define SUPPORT_GET_FAN2_RPM            0

//-----------------------------------------------------------------------------
// Oem 5ms Events/Hook Here
//-----------------------------------------------------------------------------
void Hook_Timer5msEvent(void)
{
    Oem_SysPowerContrl();   //OEM_POWER.C: Oem Handle System Power Control
    HandleSMBusData();

    #if USE_SMBUS_CHANNEL_A
    SMBusA_Scan_Center();
    #endif

    #if USE_SMBUS_CHANNEL_B
    SMBusB_Scan_Center();
    #endif

    #if USE_SMBUS_CHANNEL_C
    SMBusC_Scan_Center();
    #endif

    #if USE_SMBUS_CHANNEL_D
    SMBusD_Scan_Center();
    #endif

}

//-----------------------------------------------------------------------------
// Oem 10ms Events/Hook Here
//-----------------------------------------------------------------------------
void Hook_Timer10msEvent(void)
{
    CoreAutoScanADC();

}

//-----------------------------------------------------------------------------
// Oem 50ms Events/Hook Here
//-----------------------------------------------------------------------------
void Hook_Timer50msEventA(void)
{

}

//-----------------------------------------------------------------------------
void Hook_Timer50msEventB(void)
{

}

//-----------------------------------------------------------------------------
void Hook_Timer50msEventC(void)
{
    CheckBatteryPlugInOut();
}

//-----------------------------------------------------------------------------
// Oem 100ms Events/Hook Here
//-----------------------------------------------------------------------------
void Hook_Timer100msEventA(void)
{


}
//-----------------------------------------------------------------------------
void Hook_Timer100msEventB(void)
{


}
//-----------------------------------------------------------------------------
void Hook_Timer100msEventC(void)
{
    //Service_LED_Indicator();

}
//-----------------------------------------------------------------------------
// Oem 125ms Events/Hook Here
//-----------------------------------------------------------------------------
void Hook_Timer125msEvent(void)
{

}
//-----------------------------------------------------------------------------
// Oem 250ms Events/Hook Here
//-----------------------------------------------------------------------------
void Hook_Timer250msEvent(void)
{

}
//-----------------------------------------------------------------------------
// Oem 500ms Events/Hook Here
//-----------------------------------------------------------------------------
void Hook_Timer500msEventA(void)
{
    Service_LED_Indicator();

}
//-----------------------------------------------------------------------------
void Hook_Timer500msEventB(void)
{

}
//-----------------------------------------------------------------------------
void Hook_Timer500msEventC(void)
{

}
//-----------------------------------------------------------------------------
// Oem 1sec Events/Hook Here
//-----------------------------------------------------------------------------
void Hook_Timer1SecEventA(void)
{
    #if SUPPORT_GET_FAN1_RPM
    if( (GPIO_D6_CTRL&0xC0) == PALT)
    {
        Fan1RPM = GetFanRPM(PWM_F1TMRR,PWM_F1TLRR);
    }
    #endif
}
//-----------------------------------------------------------------------------
void Hook_Timer1SecEventB(void)
{

    #if SUPPORT_GET_FAN2_RPM
    if( (GPIO_D7_CTRL&0xC0) == PALT)
    {
      Fan2RPM=GetFanRPM(PWM_F2TMRR,PWM_F2TLRR);
    }
    #endif
}
//-----------------------------------------------------------------------------
void Hook_Timer1SecEventC(void)
{
    Oem_ColdBootTester();   // OEM_POWER.C: Auto-Cold Boot Tester

}

//-----------------------------------------------------------------------------
// 1 min events
//-----------------------------------------------------------------------------
void Hook_Timer1MinEvent(void)
{
    ;
}

//-----------------------------------------------------------------------------
// End of Handle Timer Events
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// FUNCTION: Service_OEM_LoTimer (Low Level 5ms handle)
//
//-----------------------------------------------------------------------------
void Service_OEM_LoTimer(void)
{
    ;
}

//-----------------------------------------------------------------------------
// FUNCTION: Service_WaitPS2_Handle
//
//-----------------------------------------------------------------------------
void Service_WaitPS2_Handle(void)
{
}

//-----------------------------------------------------------------------------
// FUNCTION: OemSetWakeUpSource
//           TRUE:  Set WakeUp and enable sleep
//           FALSE: Skip(Disable) sleep.
//-----------------------------------------------------------------------------
bit OemSetWakeUpSource(void)
{
    IER0 |= 0x00;
    IER2 |= 0x02;   // LPCRST
    IER3 |= 0x01;   // KBC_IBF
    //-------------------------------------------------------------------------
    if( CORE_PMFLAG_S0 )
    {
        IER1  = 0x08;   // KBS interrupt
    }
    else
    {
        IER1 |= 0x88;   // CIR, KBS interrupt
    }
    //-------------------------------------------------------------------------
    //-------------------------------------------------------------------------
    return TRUE;
    //-------------------------------------------------------------------------
}
//-----------------------------------------------------------------------------

/* ----------------------------------------------------------------------------
 * FUNCTION: vCheckChipCanDeepSleep
 *           TRUE: Allow enter Deep Sleep / FALSE: skip entry
 * ------------------------------------------------------------------------- */
bit vCheckChipCanDeepSleep(void)
{
    //if( IS_ADAPTER_IN() || IF_VIA_SUS_ON()
    //  ||  (STATE_PowerSW() == PowerSW_Active) )
    {   // Skip Deep-Sleep if Adapter On-Line
        DeepSleepCountDown = 5;     //5 sec Deep-Sleep timer
        return  FALSE;
    }
    //-------------------------------------------------------------------------
    if( DeepSleepCountDown > 0 || !CORE_PMFLAG_S5 )
    {
        return  FALSE;
    }
    //-------------------------------------------------------------------------
    return  TRUE;
}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// FUNCTION: InitEnterDeepSleep
//
//-----------------------------------------------------------------------------
const sInitGPIOReg code InitEnterDeepReg[] =
{
    { &ADCCFG,          0x00    },
    { &DACPDREG,        0x3F    },
    { &PWM_ZTIER,       0x00    },
    { &KSOL,            0x00    },
    { &KSOH1,           0x00    },
    { &KSICTRL,         0x00    },
    { &GPIO_I0_CTRL,    INPUT   },
    //Wake-Up GPIO Config.
    { &GPIO_D0_CTRL,    PALT+PULL_UP    },  //Enable WU22(WUI2) (INT21)-> AC IN
    { &GPIO_E4_CTRL,    PALT+PULL_UP    },  //Enable WU25(PWRSW)(INT14)-> PWRSW
    //Wake-Up Edge Mode Register Setting
    { &WUEMR2,          0x21    },  //Bit5:WU25(PWRSW) Bit0:AC_IN Low Active
    //Wake-Up Sense Register Clear
    { &WUESR1,          0xFF    },
    { &WUESR2,          0xFF    },
    { &WUESR3,          0xFF    },
    { &WUESR4,          0xFF    },
    { &WUESR6,          0xFF    },
    //Wake-Up Enable Register Setting
    { &WUENR2,          0x21    },      //Bit5:WU25(PWRSW) Bit1:WU21(LID) Bit0:WU20(ACIN)
    //Interrupt Polarity Register Setting
    { &IPOLR0,          0x00    },
    { &IPOLR1,          0x00    },
    //Interrupt Status Register Clear
    { &ISR0,            0xFF    },
    { &ISR1,            0xFF    },
    { &ISR2,            0xFF    },
    { &ISR3,            0xFF    },
    //Interrupt Enable Register Setting
    { &IER0,            0x02    },  //Bit5:INT5/WKINTAD Bit1:INT1(AC_IN)
    { &IER1,            0x40    },  //Bit7:CIR Interrupt Bit6:INT14 (PWRSW)
    { &IER2,            0x20    },
    { &IER3,            0x00    },
};
//-----------------------------------------------------------------------------
void InitEnterDeepSleep(void)
{
    BYTE iTemp;

    #if SUPPORT_EC_WDT_RESET
    EWDCNTLR = T_WDTMR_SLEEP;       // Watch Dog Timeout Values(sec)
    #endif  //SUPPORT_EC_WDT_RESET
    //#if SUPPORT_CIR_DEVICE
    //Init_CIR();
    //#endif

    iTemp = 0x00;
    while( iTemp<(sizeof(InitEnterDeepReg)/3) )
    {
        *InitEnterDeepReg[iTemp].reg=InitEnterDeepReg[iTemp].value;
        iTemp++;
    }
}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// FUNCTION: InitWakeFromDeepSleep
//
//-----------------------------------------------------------------------------
const sInitGPIOReg code InitWakeUpDeepReg[] =
{
    { &DeepSleepCountDown,  0x05            },
    { &KSICTRL,             0x04            },
    { &ADCCFG,              (BIT0 + BIT5)   },
    { &DACPDREG,            0x00            },
    { &PWM_ZTIER,           0x02            },
    //GPIO Config.
    { &GPIO_I0_CTRL,        PALT            },
    //Wake-Up GPIO Config.
    { &GPIO_D0_CTRL,        INPUT+PULL_UP   },  //WU20(WUI0) (INT1) -> AC_IN
    { &GPIO_E4_CTRL,        INPUT+PULL_UP   },  //SWU25(PWRSW)(INT14)-> PWR SW
    //Wake-Up Enable Register Setting
    { &IER0,                0x00            },
    { &IER1,                0x00            },
    { &IER2,                0x00            },
    //Interrupt Status Register Clear
    { &ISR0,                0xFF            },
    { &ISR1,                0xFF            },
    { &ISR2,                0xFF            },
    { &ISR3,                0xFF            },

};
//-----------------------------------------------------------------------------
void InitWakeFromDeepSleep(void)
{
    BYTE iTemp;

    iTemp = 0x00;
    while( iTemp<(sizeof(InitWakeUpDeepReg)/3) )
    {
        *InitWakeUpDeepReg[iTemp].reg=InitWakeUpDeepReg[iTemp].value;
        iTemp++;
    }
    #if SUPPORT_CIR_DEVICE
    Init_CIR();
    #endif
    /*
    if( (STATE_PowerSW() == PowerSW_Active) && (IF_LID_SW_OPEN()) )
    {
        if( CheckBatteryCanBoot() )
        {
            Oem_TriggerPowerOn();
        }
    }
    */
}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// FUNCTION: Service_LPCRST
//           ISR for INT17 WUC Interrupt WUI4 for LPCRST Process
//-----------------------------------------------------------------------------
void Service_LPCRST(void)
{
    GATEA20_ON();
    OS_ACPI_Mode = 0;
    Ccb42 = 0x38;
}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// End of Service
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Oem Cold Reset Initialize
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#if (OEM_InitSIO_Table)  /*<------<< PROJECT.H */
//-----------------------------------------------------------------------------
const BYTE code initsio_table[]=
{
    /* Configure and Enable Logical Device 06h(KBC) */
    0x07 ,0x06,     // Select Logical Device 06h(KBC)
    0x70 ,0x01,     // Set IRQ=01h for Logical Device 06h(KBC)
    0x30 ,0x01,     // Enable Logical Device 06h(KBC)
    /* Configure and Enable Logical Device 05h(Mouse) */
    0x07 ,0x05,     // Select Logical Device 05h(Mouse)
    0x70 ,0x0C,     // Set IRQ=0Ch for Logical Device 05h(Mouse)
    0x30 ,0x01,     // Enable Logical Device 05h(Mouse)
    /* Enable Logical Device 11h(PM1) */
    0x07 ,0x11,     // Select Logical Device 11h(PM1)
    0x70 ,0x00,     // Clear IRQ=0 for  Logical Device 11h(PM1)
    0x30 ,0x01,     // Enable Logical Device 11h(PM1)
    /* Enable Logical Device 12h(PM2) */
    0x07 ,0x12, // Select Logical Device 12h(PM2)
    0x70 ,0x00, // Clear IRQ=0 for  Logical Device 12h(PM2)
    0x30 ,0x01, // Enable Logical Device 12h(PM2)
    /* Enable Logical Device 04h(MSWC) */
    0x07 ,0x04, // Logical Device 04h(MSWC)
    0x30 ,0x01, // Enable MSWC
    /* Enable Logical Device 0Ah(CIR) */
#if SUPPORT_CIR_DEVICE
	0x07 ,0x0A, // Enable Logical Device 0Ah(CIR)
	0x60 ,0x03, // IO Port 300		
	0x61 ,0x00, //
	0x70 ,0x0A, // IRQ=10 for  CIR
	0x30 ,0x01, // Enable CIR
#endif
    /* Enable Logical Device 10h(BRAM) */
    0x07, 0x10,     // Logical Device 10h(BRAM)
    0x60, 0x09,     // I/O Base Address 912h
    0x61, 0x12,     // I/O Base Address 913h
    0x62, 0x09,     // I/O Base Address 910h
    0x63, 0x10,     // I/O Base Address 911h
    0x70, 0x00,     // Set Without IRQ
    0x30, 0x01,     // Enable BRAM(MailBox)
    /* Enable Logical Device 0Fh(Shared Memory) */
    0x07 ,0x0F, // Logical Device 0Fh(Shared Memory)
    0x30 ,0x01, // MSWC Shared Memory
    0xF4 ,0x09, // MSWC Shared Memory

};
//----------------------------------------------------------------------------
// Init super IO function
//----------------------------------------------------------------------------
void InitSio(void)
{
    BYTE code *data_pntr;
    BYTE cnt;

    SET_BIT( LSIOHA, 0 );
    SET_BIT( IBMAE,  0 );
    SET_BIT( IBCTL,  0 );

    cnt=0;
    data_pntr = initsio_table;
    while( cnt < (sizeof(initsio_table)/2) )
    {
        IHIOA=0;              // Set indirect Host I/O Address
        IHD=*data_pntr;
        while( IS_BIT_SET(IBCTL,2) );
        data_pntr ++;

        IHIOA=1;              // Set indirect Host I/O Address
        IHD=*data_pntr;
        while( IS_BIT_SET(IBCTL,2) );
        data_pntr ++;
        cnt ++;
    }

    CLEAR_BIT( LSIOHA, 0 );
    CLEAR_BIT( IBMAE,  0 );
    CLEAR_BIT( IBCTL,  0 );

}
//-----------------------------------------------------------------------------
#endif      /*<------<< PROJECT.H */
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Oem Cold Reset Initialize
//-----------------------------------------------------------------------------
// The function of init. registers
//-----------------------------------------------------------------------------
const struct REG_INIT_DEF code oem_reg_init_table[] =
{
    //SWUC
    { &SPCTRL1,     _Init_I2EC      },  // Enable I2EC R/W
    { &BADRSEL,     _Init_BADR      },
    { &SWCBALR,     _Init_SWCBALR   },
    { &SWCBAHR,     _Init_SWCBAHR   },
    // PWM
    { &PWM_ZTIER,       0x00    },
    { &PWM_CTR,         0xFF    },
    { &PWM_PCFSR,       0x43    },
    { &PWM_C0CPRS,      0x0F    },  // EC_PWM_2 34KHz
    { &PWM_C4CPRS,      0xAF    },  // INVERTER 200Hz(B2)
    { &PWM_C6CPRS,      0x7F    },
    { &PWM_C7CPRS,      0x01    },
    { &PWM_PCSSGL,      0x00    },
    { &PWM_PCSSGH,      0x04    },  // Set CH_GROUP 4
    { &PWM_ZTIER,       0x02    },
    // ETWD - External Timer & External Watchdog
    { &ET1PSR,          0x00    },  // 32.768K
    { &ET1CNTLHR,       0x80    },  // 32768 for 1sec counter
    { &ET1CNTLLR,       0x00    },
    // ECPM - EC Clock & Power Management controller
    { &CGCTRL1R,        0x00    },  // Enabled ADC, ADC clock.
    { &CGCTRL2R,        0x80    },  // TMKBC clock gating
    // Keyboard Matrix
    { &KSOCTRL,         BIT0+BIT2   },

};
//-----------------------------------------------------------------------------
const struct REG_INIT_DEF code SMBus_REG_init_table[] =
{
    //SMBus
    { &SMB_4P7USL,      0x2F    },  // 50K:02Fh,80K:02Fh,100K:02Fh
    { &SMB_4P0USH,      0x5A    },  // 50K:05Ah,80K:028h,100K:028H
    { &SMB_300NSREG,    0x03    },  // 50K:003h 80K:003h,100K:003h
    { &SMB_250NSREG,    0x30    },  // 50K:030h 80K:020h,100K:003h
    { &SMB_25MSREG,     0x19    },  // 50K:019h 80K:019h,100K:005h
    { &SMB_45P3USLREG,  0xC5    },  // 50K:0C5h 80K:0C5h,100K:0C5h
    { &SMB_45P3USHREG,  0x01    },  // 50K:001h 80K:001h,100K:001h
    //SMBUS-A
    { &HOCTL2_A,        0x01    },
    { &RESLADR_A,       0x08    },
    { &HOCTL_A,         0x03    },
    { &HOCTL_A,         0x01    },
    { &HOSTA_A,         0xFF    },
    { &SICR_A,          0x01    },
    { &SLSTA_A,         0xFF    },
    //SMBUS-B
    { &HOCTL2_B,        0x01    },
    { &RESLADR_B,       0x08    },
    { &HOCTL_B,         0x03    },
    { &HOCTL_B,         0x01    },
    { &HOSTA_B,         0xFF    },
    { &SICR_B,          0x01    },
    { &SLSTA_B ,        0xFF    },
    //SMBUS-C
    { &HOCTL2_C,        0x01    },
    { &HOCTL_C,         0x03    },
    { &HOCTL_C,         0x01    },
    { &HOSTA_C,         0xFF    },

};
//-----------------------------------------------------------------------------
// Oem Init SMBus Register
//-----------------------------------------------------------------------------
void OEM_Init_SMBus(void)
{
    BYTE index = 0x00;
    while( index < (sizeof(SMBus_REG_init_table)/3) )
    {
        Tmp_XPntr  = SMBus_REG_init_table[index].address;
        *Tmp_XPntr = SMBus_REG_init_table[index].initdata;
        index++;
    }
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    #if EN_S5_SMBUS_CHANNEL_A
    GPIO_B3_CTRL = PALT;
    GPIO_B4_CTRL = PALT;
    #endif
    #if EN_S5_SMBUS_CHANNEL_B
    GPIO_C1_CTRL = PALT;
    GPIO_C2_CTRL = PALT;
    #endif
    #if EN_S5_SMBUS_CHANNEL_C
    GPIO_F6_CTRL = PALT;
    GPIO_F7_CTRL = PALT;
    #endif
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

}
//-----------------------------------------------------------------------------
void OEM_ColdResetInit(void)
{
    BYTE index  = 0x00;
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    #if SUPPORT_EC_WDT_RESET
    ETPSR = 0x00;       // SELECT 32.768Khz
    ETCNTLHR = 0x80;    // Set 1Sec Count
    ETCNTLLR = 0x00;    // Set 1Sec Count
    EWDCNTLR = T_WDTMR_COLD;    // Watch Dog Timeout Values(sec)
    ETWCFG   = 0x27;    //LOCK WATCHDOG REGISTER
    #endif  //SUPPORT_EC_WDT_RESET
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    Oem_CodeVer = EC_OEM_VER;
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    #if SUPPORT_CIR_DEVICE
    Init_CIR();
    #endif
    while( index < (sizeof(oem_reg_init_table)/3) )
    {
        Tmp_XPntr  = oem_reg_init_table[index].address;
        *Tmp_XPntr = oem_reg_init_table[index].initdata;
        index++;
    }
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    OEM_Init_SMBus();
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    SET_BIT( IELMR2, 1 );   //LPCRST
    SET_BIT( IPOLR2, 1 );   //LPCRST
    EX1   = 1;
    IER2 |= 0x02;           //LPCRST
    IER1 |= 0x08;
    VCH0CTL = 0x80;
    VCH1CTL = 0x81;
    VCH2CTL = 0x82;
    VCH3CTL = 0x83;
    Oem_SysOffInit();
    // Don't clear 225F-227F for Power Log.
    Clear_RAM_Range(0x2200,0x5E);   // Clear MailBox(BANK0) Reg.
    Clear_RAM_Range(0x2280,0x3F);   // Clear MailBox(BANK1) Reg.
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

}
//-----------------------------------------------------------------------------
// Oem Init ACPI EC Spaces
//-----------------------------------------------------------------------------
const unsigned char code _ACPI_INI_Table[] =
{
    0x01,0x00,0x30,0x11,0x30,0x11,0x01,0x00,
    0x5C,0x2B,0x1E,0x01,0x9A,0x00,0x9A,0x00,
    0x96,0x10,
    'I','T','E','0','0','0','0','1',
    '0','0','0','1',0x00,0x00,0x00,0x00,
    'L','i','O','N',0x00,0x00,0x00,0x00,
    'O','E','M',0x00,0x00,0x00,0x00,0x00,
    0x02,0x00,0xD0,0x07,0xD0,0x07,0x5C,0x2B,
    0x00,0x00,0x05,0x00,
};
//-----------------------------------------------------------------------------
void OEM_INIT_ACPI(void)
{
    //Sample Code
    //BYTE index = 0x00;

    //while(index < ( sizeof(_ACPI_INI_Table) ) )
    //{
    //    Tmp_XPntr = ( ECSpace + index );
    //  *Tmp_XPntr = _ACPI_INI_Table[index];
    //    index ++;
    //}

}
//-----------------------------------------------------------------------------

//*****************************************************************************
// OEM
//*****************************************************************************

/*-----------------------------------------------------------------------------
 * End
 *---------------------------------------------------------------------------*/