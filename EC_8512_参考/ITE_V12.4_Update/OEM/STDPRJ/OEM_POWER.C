/*-----------------------------------------------------------------------------
 * Filename: OEM_POWER.C
 * Function: OEM USE EC CONTROL SYSTEM POWER
 *
 * Copyright (c) 2006-2009, ITE Tech. Inc. All Rights Reserved.
 *---------------------------------------------------------------------------*/
#include "CORE_INCLUDE.H"
#include "OEM_INCLUDE.H"

//-----------------------------------------------------------------------------
//OEM_POWER.H Parameter
//define    _PowrUp_S5      0
//define    _SYSTEM_S0      1
//define    _SYSTEM_S1      2
//define    _SYSTEM_S3      3
//define    _SYSTEM_S4      4
//define    _SYSTEM_S5      5
//define    _GO_POWER_ON    6
//define    _GO_RESUME_ON   7
//define    _GO_SUSPNED_OFF 8
//define    _GO_POWER_OFF   9
//-----------------------------------------------------------------------------

/*-----------------------------------------------------------------------------
 * Internal Process Definition
 *---------------------------------------------------------------------------*/
static void Oem_CheckPowSwitch(void);
static void Oem_PowSwToPWRBTN(void);
static void Oem_ChkS3Suspend(void);
static void Oem_ChkS5Shutdown(void);
static void Oem_ChkSysWakeUp(void);
static void Oem_ChkS3WakeUp(void);
static void Oem_SysOnSequence(void);
static void Oem_SysOffSequence(void);
static void Oem_SysResumeSequence(void);
static void Oem_SysSuspendSequence(void);
static void Oem_SysSuspendInit(void);
static void Oem_SysResumeInit(void);
static void Oem_PowSwitchIntr(void);
static void Oem_PowSwOver5Sec(void);


//-----------------------------------------------------------------------------
// Handle System Power Control
//-----------------------------------------------------------------------------
void Oem_SysPowerContrl(void)
{
    switch (SysPowState)
    {
        case 0: // EC Initial Boot
                Oem_CheckPowSwitch();
                break;

        case 1: // System S0
                Oem_PowSwToPWRBTN();
                Oem_PowSwOver5Sec();
                Oem_ChkS3Suspend();
                Oem_ChkS5Shutdown();
                if ( !STATE_PCIRST() )
                {
                    CLEAR_BIT(KBHISR,2);
                    GATEA20_ON();
                }
                break;

        case 2: // System S1
                Oem_PowSwToPWRBTN();
                Oem_PowSwOver5Sec();
                //Oem_ChkSysWakeUp();
                Oem_ChkS5Shutdown();
                break;

        case 3: // System S3
                Oem_PowSwToPWRBTN();
                Oem_PowSwOver5Sec();
                Oem_ChkS3WakeUp();
                Oem_ChkS5Shutdown();
                break;

        case 4: // System S4

        case 5: // System S5
                //Oem_ChkSysWakeUp();
                //Oem_PowSwToPWRBTN();
                Oem_CheckPowSwitch();
                break;

        case 6: // S5 -> S0  system power on sequence
                Oem_SysOnSequence();
                Oem_PowSwitchIntr();
                break;

        case 7: // S3 -> S0 resume power sequence
                Oem_SysResumeSequence();
                break;

        case 8: // S0 -> S3  system suspend sequence
                Oem_SysSuspendSequence();
                break;

        case 9: // S0 -> S5 system power off sequence
                Oem_SysOffSequence();
                break;

        default:
                SysPowState=0;
                return;
    }
    //-------------------------------------------------------------------------

}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Automatic Cold Boot Test Service (Suggestion: 1 Sec Hook)
//-----------------------------------------------------------------------------
void Oem_ColdBootTester(void)
{
    if ( AutoColdBootTimer > 0 )
    {
        if ( SysPowState == _SYSTEM_S5 )
        {
            if ( ColdBootTimer == 0x00 )
            {
                ColdBootTimer = AutoColdBootTimer;
            }
            else
            {
                ColdBootTimer--;

                if ( ColdBootTimer == 0x00 )
                {
                    Oem_TriggerPowerOn();
                }
            }

        }
        else
        {
            ColdBootTimer = AutoColdBootTimer;

        }
    }

}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Power Switch Control
//-----------------------------------------------------------------------------
void Oem_CheckPowSwitch(void)
{
    BYTE BTemp;
    if ( LockSwitchPowerOn > 0 )
    {
        LockSwitchPowerOn--;
        return;
    }
    #if PowerSW_Active
    //InputDebounce(FLAG PIN_STATUS,FLAG _ACTIVE,BYTE Counter_ID,BYTE ReloadCnt)
    BTemp = InputDebounce(STATE_PowerSW(),HighActive,0x00,T_POWSW_ONDB);
    //Return_0x00:NoAcitve 0x01:Active 0x02:Active&Debounced 0x03:RepeatActive
    #else
    BTemp = InputDebounce(STATE_PowerSW(),LowActive,0x00,T_POWSW_ONDB);
    #endif

    if ( BTemp == 0x02 )        // Active and Debounced
    {
        Oem_TriggerPowerOn();
    }

}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Trigger Control System Power On
//-----------------------------------------------------------------------------
void Oem_TriggerPowerOn(void)
{
    CtrlPowStep = 0;    //Reset Power Step Control Register
    PowSeqDelay = 0;    //Clear Sequence Delay Register
    SysPowState = _GO_POWER_ON; // Trigger S5->S0 system power on sequence

}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Trigger Control System Power Off
//-----------------------------------------------------------------------------
void Oem_TriggerPowerOff(void)
{
    CtrlPowStep = 0;    //Reset Power Step Control Register
    PowSeqDelay = 0;    //Clear Sequence Delay Register
    SysPowState = _GO_POWER_OFF;    // Trigger S5 system power off sequence
    LockSwitchPowerOn = T_POWSW_LockON;    // Lock Power timer

}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Power Switch routine to PWRBTN# Control
//-----------------------------------------------------------------------------
void Oem_PowSwToPWRBTN(void)
{
    BYTE    BTemp;

    #if PowerSW_Active
    //InputDebounce(FLAG PIN_STATUS,FLAG _ACTIVE,BYTE Counter_ID,BYTE ReloadCnt)
    BTemp = InputDebounce(STATE_PowerSW(),HighActive,0x00,T_POWSW_ToBTN);
    //Return_0x00:NoAcitve 0x01:Active 0x02:Active&Debounced 0x03:RepeatActive
    #else
    BTemp = InputDebounce(STATE_PowerSW(),LowActive,0x00,T_POWSW_ToBTN);
    #endif

    if ( BTemp == 0x03 )
    {
        PM_PWRBTN_LO();
    }
    else
    {
        PM_PWRBTN_HI();
    }

}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Power Switch Interrupt Sequence Control
//-----------------------------------------------------------------------------
void Oem_PowSwitchIntr(void)
{
    BYTE BTemp;

    #if PowerSW_Active
    //InputDebounce(FLAG PIN_STATUS,FLAG _ACTIVE,BYTE Counter_ID,BYTE ReloadCnt)
    BTemp = InputDebounce(STATE_PowerSW(),HighActive,0x00,T_POWSW_INTR);
    //Return_0x00:NoAcitve 0x01:Active 0x02:Active&Debounced 0x03:RepeatActive
    #else
    BTemp = InputDebounce(STATE_PowerSW(),LowActive,0x00,T_POWSW_INTR);
    #endif
    if ( BTemp == 0x02 )        // Active and Debounced
    {
        Oem_TriggerPowerOff();
    }

}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Power Switch Control
//-----------------------------------------------------------------------------
void Oem_PowSwOver5Sec(void)
{
    BYTE BTemp;
    if ( (SysPowState == _SYSTEM_S4) || (SysPowState == _SYSTEM_S5) || (SysPowState == _PowrUp_S5) )
    {
        return;
    }
    #if PowerSW_Active
    //InputDebounce(FLAG PIN_STATUS,FLAG _ACTIVE,BYTE Counter_ID,BYTE ReloadCnt)
    BTemp = InputDebounce(STATE_PowerSW(),HighActive,0x00,T_POWSW_OVER);
    //Return_0x00:NoAcitve 0x01:Active 0x02:Active&Debounced 0x03:RepeatActive
    #else
    BTemp = InputDebounce(STATE_PowerSW(),LowActive,0x00,T_POWSW_OVER);
    #endif

    if ( BTemp == 0x03 )        // Active and Debounced
    {
        PowSwOverTimer++;
        if ( PowSwOverTimer > 1000 )
        {
            Oem_TriggerPowerOff();
        }
    }
    else
    {
        PowSwOverTimer = 0;
    }

}

//-----------------------------------------------------------------------------
// Check System S5 Shutdown Signal (PM_SLP_S5 or SUSC)
// Usually from System South Bridge
//-----------------------------------------------------------------------------
void Oem_ChkS5Shutdown(void)
{
    bit bTemp;
    bTemp = STATE_PM_SLPS5();   // Get SLP_S5 Status
    if ( !bTemp )   // Low Active
    {
        Oem_TriggerPowerOff();
    }

}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Check System S3 Suspend Signal (PM_SLP_S3 or SUSB)
// Usually from System South Bridge
//-----------------------------------------------------------------------------
void Oem_ChkS3Suspend(void)
{
    bit bTemp;
    bTemp = STATE_PM_SLPS3();   // Get SLP_S3 Status
    if ( !bTemp )   // Low Active
    {
        CtrlPowStep = 0;    //Reset Power Step Control Register
        PowSeqDelay = 0;    //Clear Sequence Delay Register
        // Trigger S0 -> S3  system suspend power off sequence
        SysPowState = _GO_SUSPNED_OFF;
    }

}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Check System S3 Resume Signal (PM_SLP_S3 or SUSB)
// Usually from System South Bridge
//-----------------------------------------------------------------------------
void Oem_ChkS3WakeUp(void)
{
    bit bTemp;
    bTemp = STATE_PM_SLPS3();   // Get SLP_S3 Status
    if ( bTemp )    // Hi->Wake-Up
    {
        CtrlPowStep = 0;    //Reset Power Step Control Register
        PowSeqDelay = 0;    //Clear Sequence Delay Register
        // Trigger S3 -> S0  system resume power on sequence
        SysPowState = _GO_RESUME_ON;
    }

}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// System Power On RAM & Register configuration
//-----------------------------------------------------------------------------
void Oem_SysOnInit(void)
{
    SysPowState = _SYSTEM_S0;   // Set System S0
    CLEAR_BIT(KBHISR,2);
    //OEM_INIT_ACPI();
    POWER_FLAG1 = 0x83;
    POWER_FLAG2 = 0x03;
    IER1|=0x08;
    _CORE_PMFLAG = 0;
    CORE_PMFLAG_S0 = 1;

}
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// System Power Off RAM & Register configuration
//-----------------------------------------------------------------------------
void Oem_SysOffInit(void)
{
    SysPowState = _SYSTEM_S5;   // Set System S5
    CLEAR_BIT(KBHISR,2);
    _CORE_PMFLAG = 0;
    CORE_PMFLAG_S5 = 1;
#if TouchPad_only
    vPS2DeviceInactive();
#endif
}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// System Suspend Power Off RAM & Register configuration
//-----------------------------------------------------------------------------
void Oem_SysSuspendInit(void)
{
    SysPowState = _SYSTEM_S3;   // Set System S3
    CLEAR_BIT(KBHISR,2);
    _CORE_PMFLAG = 0;
    CORE_PMFLAG_S3 = 1;
#if TouchPad_only
    vPS2DeviceInactive();
#endif
}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// System Suspend Power On RAM & Register configuration
//-----------------------------------------------------------------------------
void Oem_SysResumeInit(void)
{
    SysPowState = _SYSTEM_S0;   // Set System S0
    POWER_FLAG1 = 0x83;
    POWER_FLAG2 = 0x03;
    SYS_MISC1   = 0x04; //OEM ACPI FLAG
    CLEAR_BIT(KBHISR,2);
    SetFlag(POWER_FLAG1,power_on);
    _CORE_PMFLAG = 0;
    CORE_PMFLAG_S0 = 1;
}

//-----------------------------------------------------------------------------
// Init Share Memory Register
//-----------------------------------------------------------------------------
void InitShareMemory(void)
{
    //UCCTRLR = 0x83;   //ATi Chipset Need (INTEL don't)
    SWCTL2 = 0;         //Set sysetm power on
    FMSSR = 0x0F;       //For 1M Bytes Share Rom
    SET_BIT(FPCFG,4);   //Enable LPC Bust mode

}

//-----------------------------------------------------------------------------
// FUNCTION: System Power On Sequence (Turns On System Power)
// TimeHook: 5mSec
//-----------------------------------------------------------------------------
void Oem_SysOnSequence(void)
{
    //bit  bTemp;
    //BYTE BTemp;
    //-------------------------------------------------------------------------
    if ( PowSeqDelay > 4 )
    {
        PowSeqDelay-=5; //5mSec count base
        if ( PowSeqDelay < 4 )
        {
            PowSeqDelay=0;
        }
        else
        {
            return;
        }
    }
    //-------------------------------------------------------------------------
    //Power On Sequence step control
    //-------------------------------------------------------------------------
    switch (CtrlPowStep)    //Step 5mSec
    {
        case 0:
                //_GPIOB._Pin0 = _HIGH;
                //_GPIOB._Pin0 = _LOW;
                DNBSWON_OFF();
                Led_Data = 0x07;
                OEM_Write_Leds();
                break;

        case 1:
                InitSio();
                RSMRST_ON();
                vDelay1MS(21);
                RVCC_ON();
                vDelay1MS(10);
                RSMRST_OFF();
                vDelay1MS(25);
                //GATEA20_ON();
                //KBRST_OFF();
                break;

        case 2:
                DNBSWON_ON();
                vDelay1MS(15);
                break;

        case 3:
                if ( STATE_RCVV()!=HighActive ) return ;
                if ( STATE_SUSC()!=HighActive ) return ;

                SetFlag(POWER_FLAG2,susc_on);

                if ( STATE_SUSB()!=HighActive ) return ;

                SetFlag(POWER_FLAG2,susb_on);
                USBPWER_ON();
                LANRSET_ON();
                SUSON_ON();
                SetFlag(POWER_FLAG1,rvcc_on);
                break;

        case 4: // S3 resume power sequence
                CCDPWR_OFF();
                MAINON_ON();
                VRON_ON();
                PowSeqDelay = 80;
                break;

        case 5:
                if ( STATE_HWPG()!=HighActive)  return ;
                
                #ifdef HSPI
	            Enable_HSPI();  
                #endif
                
                break;

        case 6:
                POWEROK_ON();
                MPWROK_ON();
                GATEA20_ON();
                KBRST_OFF();
                break;

        case 7:
                SCI_OFF();
                SMI_OFF();
                DNBSWON_OFF();
                DACDAT1 = 0xFF;
                DACDAT2 = 0xFF;
                Led_Data = 0x00;
                OEM_Write_Leds();
                Oem_SysOnInit();
                break;

        default:
                //InitSio();
                //bInitThermalChip();
                //Init_CIR();
                break;
    }

    //-------------------------------------------------------------------------
    if ( PowSeqWait > 0 )
    {
        PowSeqWait--;
    }
    else
    {
        CtrlPowStep++;
    }
    //-------------------------------------------------------------------------

}


//-----------------------------------------------------------------------------
// FUNCTION: System Resume from S3 Sequence (Turns On Suspend Power)
// TimeHook: 5mSec
//-----------------------------------------------------------------------------
void Oem_SysResumeSequence(void)
{
    //bit bTemp;

    //-------------------------------------------------------------------------
    if ( PowSeqDelay > 4 )
    {
        PowSeqDelay-=5; //5mSec count base

        if ( PowSeqDelay < 4 )
        {
            PowSeqDelay=0;
        }
        else
        {
            return;
        }
    }
    //-------------------------------------------------------------------------
    //System Resume Sequence step control
    //-------------------------------------------------------------------------
    switch (CtrlPowStep)    //Step 5mSec
    {
        case 0:
                CCDPWR_ON();
                MAINON_ON();
                VRON_ON();
                PWRLED1_ON();
                DACDAT1 = 0xFF;
                DACDAT2 = 0xFF;
                break;
        case 1:
                if ( STATE_HWPG()!=HighActive )   return;
                PowSeqDelay = 80;
                break;
        case 2:
                POWEROK_ON();
                MPWROK_ON();
                GATEA20_ON();
                KBRST_OFF();
                Oem_SysResumeInit();
                break;
        case 5:
                break;
        case 6:
                break;
        case 7:
                break;
        case 8:
                //InitShareMemory();
                //InitSio();
                //bInitThermalChip();
                //Oem_SysResumeInit();
                break;
        default:
                CtrlPowStep=0;
                break;
    }

    //-------------------------------------------------------------------------
    if ( PowSeqWait > 0 )
    {
        PowSeqWait--;
    }
    else
    {
        CtrlPowStep++;
    }
    //-------------------------------------------------------------------------

}

//-----------------------------------------------------------------------------
// FUNCTION: Force System Power Off Sequence
//
//-----------------------------------------------------------------------------
void Oem_ForceSystemOff(void)
{
    CtrlPowStep = 0;
    while(1)
    {
        vDelay1MS(5);   // Delay 5ms
        Oem_SysOffSequence();
        if( CtrlPowStep == 0 )
        {
            break;
        }
    }

}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// FUNCTION: System Power Off Sequence
// TimeHook: 5mSec
//-----------------------------------------------------------------------------
void Oem_SysOffSequence(void)
{
    //bit   bTemp;
    //-------------------------------------------------------------------------
    if ( PowSeqDelay > 4 )
    {
        PowSeqDelay-=5; //5mSec count base

        if ( PowSeqDelay < 4 )
        {
            PowSeqDelay=0;
        }
        else
        {
            return;
        }
    }
    //-------------------------------------------------------------------------
    // System Off step contrl
    //-------------------------------------------------------------------------
    switch (CtrlPowStep)    //Step 5mSec
    {
        case 0:
                #ifdef HSPI
	            Disable_HSPI();  
                #endif	
                
                MPWROK_OFF();
                POWEROK_OFF();
                CCDPWR_ON();
                #if ENABLE_S5_KEYBOARD_INTR
                IER1&=(~0x08);
                #endif

                PWRLED1_OFF();
                DACDAT1 = 0x00;
                DACDAT2 = 0x00;
                PowSeqDelay= 10;
                break;
        case 1:
                VRON_OFF();
                PowSeqDelay= 10;
                break;
        case 2:

                break;
        case 3:
                MAINON_OFF();
                SUSON_OFF();
                USBPWER_OFF();
                LANRSET_OFF();
                PowSeqDelay= 10;
                break;
        case 4:
                RSMRST_OFF();
                PowSeqDelay= 25;
                break;
        case 5:
                RVCC_OFF();
                PowSeqDelay= 50;
                break;
        case 6:
                RSMRST_ON();
                break;
        case 7:
                break;
        case 8:
                break;
        case 9:
                break;
        case 10:
                Oem_SysOffInit();
                break;
        default:
                CtrlPowStep=0;
                return;
    }

    //-------------------------------------------------------------------------
    if ( PowSeqWait > 0 )
    {
        PowSeqWait--;
    }
    else
    {
        CtrlPowStep++;
    }
    //-------------------------------------------------------------------------

}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// FUNCTION: System Suspend Power Off Sequence
// TimeHook: 5mSec
//-----------------------------------------------------------------------------
void Oem_SysSuspendSequence(void)
{
    //bit   bTemp;

    //-------------------------------------------------------------------------
    if ( PowSeqDelay > 4 )
    {
        PowSeqDelay-=5; //5mSec count base

        if ( PowSeqDelay < 4 )
        {
            PowSeqDelay=0;
        }
        else
        {
            return;
        }
    }
    //-------------------------------------------------------------------------
    //System Suspend step control
    //-------------------------------------------------------------------------
    switch (CtrlPowStep)    //Step 5mSec
    {
        case 0:
                MPWROK_OFF();
                POWEROK_OFF();
                CCDPWR_OFF();
                PowSeqDelay = 10;    //10ms
                break;
        case 1:
                VRON_OFF();
                PowSeqDelay = 10;    //10ms
                break;
        case 2:

                break;
        case 3:
                MAINON_OFF();
                PWRLED1_OFF();
                DACDAT1 = 0x00;
                DACDAT2 = 0x00;
                break;
        case 4:
                break;
        case 5:
                break;
        case 6:
                break;
        case 7:
                break;
        case 8:
                Oem_SysSuspendInit();
                break;
        default:
                CtrlPowStep=0;
                break;
    }

    //-------------------------------------------------------------------------
    if ( PowSeqWait > 0 )
    {
        PowSeqWait--;
    }
    else
    {
        CtrlPowStep++;
    }
    //-------------------------------------------------------------------------

}
//-----------------------------------------------------------------------------

/*-----------------------------------------------------------------------------
 * End
 *---------------------------------------------------------------------------*/