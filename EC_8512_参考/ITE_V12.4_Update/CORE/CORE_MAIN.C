/*-----------------------------------------------------------------------------
 * TITLE: CORE_MAIN.C - Main Program for KBC firmware.
 *
 * Main processing loop (interrupt driven) for the keyboard controller/scanner.
 *
 * Copyright (c) 1983-2009, Insyde Software Corporation. All Rights Reserved.
 *
 * You may not reproduce, distribute, publish, display, perform, modify, adapt,
 * transmit, broadcast, present, recite, release, license or otherwise exploit
 * any part of this publication in any form, by any means, without the prior
 * written permission of Insyde Software Corporation.
 *---------------------------------------------------------------------------*/
#include "CORE_INCLUDE.H"
#include "OEM_INCLUDE.H"

/* ----------------------------------------------------------------------------
 * >>>----> ITE Technical Marketing Firmware Center <-----------------------<<<
 *
 *          EC Kernel Code Revision 1.1     Date: July 01, 2009 12:00:00
 * ------------------------------------------------------------------------- */
//------------------------------------------------------------------------------
// The function of disable all interrupts
//------------------------------------------------------------------------------
void DisableAllInterrupt(void)
{
  	EA = 0;
	_nop_();
	_nop_();
	_nop_();
	_nop_();
}

//------------------------------------------------------------------------------
// The function of enable all interrupts
//------------------------------------------------------------------------------
void EnableAllInterrupt(void)
{
  	EA = 1;
}

/* ----------------------------------------------------------------------------
 *  FUNCTION: main - Main service loop.
 *
 *  Wait in idle state until an IRQ causes an exit from idle.  If the IRQ
 *  handler posted a service request (via bSERVICE) then dispatch control to
 *  the appropriate service handler.  Otherwise, go back to idle state.  After
 *  all service requests have been handled, return to idle state.
 * ------------------------------------------------------------------------- */
void main(void)
{
    SP = 0xD0;      // Setting Stack Pointer (Reset Pointer)

    #if SUPPORT_ExitRamCode_KEEP_OS
    if( (ExitRamCodeCommand==0xAA) && (CORE_PMFLAG_S0) )
    {
        CLEAR_BIT(FBCFG,7);     // Disable scatch ROM
        #ifdef HSPI
        HSPI_SetReloadRamCodeFlag();    // Set flag for re-load HSPI ram code function
        #endif
        EnableAllInterrupt();   // enable global interrupt
        ExitRamCodeCommand = 0x00;
    }
    else
    #endif //SUPPORT_ExitRamCode_KEEP_OS
    {
        CLEAR_BIT(FBCFG,7);     // Disable scatch ROM
        EnableAllInterrupt();   // enable global interrupt
        ExitRamCodeCommand = 0x00;
        FPCFG = 0x3F;           // Bit7 :0 using P1[0] P1[1] as code banking
        FMSSR = _Init_FMSSR;    /*<------<< PROJECT.H */
        Reset_Initialization();
        vInitEnableInterrupt();
        #ifdef UART_Debug
        Init_UART();
        printf("EC Init OK !!!\n");
        #endif
    }

    Init_Cache();

    //-- Loop Service Start ---------------------------------------------------
    //
    while(1)
    {
        main_service();
        
        #ifdef HSPI
        if(Service_HSPI==0x01)
        {
            Service_HSPI = 0x00;
            if(IsFlag1(HSPI_Misc1, HSPI_ReLoadHSPIRamCode))
            {
                CLEAR_MASK(HSPI_Misc1, HSPI_ReLoadHSPIRamCode);
                LoadSPIFucnToRam(HSPI_RamCode);	        // Load function to ram
            }
            HSPI_Process();
        }
        #endif
        
        EnableInterrupt();
        _nop_();
        _nop_();
        if ( vCheckChipCanSleep() )
        {
            /* CHIP SLEEPING...*/
            #if SUPPORT_EC_DEEP_SLEEP
            if ( vCheckChipCanDeepSleep() )
            {
                InitEnterDeepSleep();
                EnableAllInterrupt();   // enable global interrupt
                PLLCTRL = 0x01;
                PCON    = 0x02;         /* Enter Deep Sleep mode.  */
                _nop_();                /* Wake-up delay.  */
                _nop_();
                _nop_();
                _nop_();
                _nop_();
                _nop_();
                _nop_();
                _nop_();
                InitWakeFromDeepSleep();
            }
            else
            #endif
            {
                ScanAUXDeviceStep();
                if((Service == 0x00)&&(Service1 == 0x00))
                {
                    PCON    = 0x01;     /* Enter idle mode */
                    _nop_();            /* Wake-up delay.  */
                    _nop_();
                    _nop_();
                    _nop_();
                    _nop_();
                    _nop_();
                    _nop_();
                    _nop_();
                }
            }
        }
        F_Service_UNLOCK = 1;
    }
    //
    //-- Loop Service End -----------------------------------------------------

}

/* ----------------------------------------------------------------------------
 * FUNCTION: vCheckChipCanSleep
 *           TRUE: Enabled idle / FALSE: skip idle entry
 * ------------------------------------------------------------------------- */
bit vCheckChipCanSleep(void)
{
    if( !OemSetWakeUpSource() || SET_EC_FULL_RUN )
    {
        return  FALSE;
    }

    if( ( Service > 0x00 ) || (Service1 > 0x00) )
    {
        return FALSE;
    }

    if( !CORE_PMFLAG_S0  )      return TRUE;

    return  TRUE;
}


/* ----------------------------------------------------------------------------
 * FUNCTION: vEnableInterrupt - Enable interrupt service
 *
 * ------------------------------------------------------------------------- */
void EnableInterrupt(void)
{
    IER3 |= 0x03;           // Enable IBF1,IBF2
    #if SUPPORT_CIR_DEVICE
    IER1 |= 0x80;           // Enable CIR interrupt
    #endif
    
    EnableAllInterrupt();   // enable global interrupt
    //EA = 1;             // Enable global interupt
}

/* ----------------------------------------------------------------------------
 * FUNCTION: main_service - Check for new/more service requests.
 *
 * Check for a request flag.  The check is done according to priority.  If a
 * request flag is set, handle the request and loop back to get the flags
 * again.  Do not follow through to check the next flag.  The flags are to be
 * checked in order.
 * ------------------------------------------------------------------------- */
const FUNCT_PTR_V_V code service_table[16] =
{   // Hi-Level Service
    Service_PCI,            // Host 60/64 Command/Data service
    Service_UNLOCK,         // Service Unlock
    Service_SEND,           // Send byte from KBC
    Service_Send_PS2,       // Send PS2 interface data
    Service_PS2_IRQ,        // Process PS2 interface data
    Service_PCI2,           // Secondary 62/66 Host Command/Data service
    Service_MS_5,           // 5 millisecond elapsed for OEM
    Service_KEYSCAN,        // Keyboard scanner service
    // Lo-Level Service
    Service_PCI3,           // Third Host Command/Data service
    Service_CIR,            // Service CIR.
    Service_Reserved_3,     // Reserved
    Service_MainLoTimer,    // Service Low Level 5ms Timer
    Service_OEM_LoTimer,    // Service Oem Low Level 5ms Timer
    Service_LPCRST,         // Service LPCRST
    Service_Reserved_1,     // Reserved
    Service_Reserved_2      // Reserved
};
//-----------------------------------------------------------------------------
void main_service(void)
{
    _W1 = ( ( (WORD)Service1 << 8 ) | Service );    /* Load Service */

    while( _W1 != 0 )
    {
        _R1 = 0;
        /* Check Service request Flag */
        while( ( _W1 & 1 ) == 0 )
        {
            _W1 >>= 1;
            _R1++;
        }

        if ( _R1 < 0x08)
        {   /* Clear service request. */
            Service &= Cpl_Byte_Mask( _R1 & 0x07 );
        }
        else
        {
            /* Low Level Check */
            /* Clear service request. */
            Service1 &= Cpl_Byte_Mask( _R1 & 0x07 );
        }

        (service_table[ _R1 ])();   /* Dispatch to service handler. */

        if( IsFlag1(PM1STS, BIT1) )         {   F_Service_PCI2 = 1;     }
        else if( IsFlag1(KBHISR, BIT1) )    {   F_Service_PCI  = 1;     }


        _W1 = ( ( (WORD)Service1 << 8 ) | Service );    /* Re-Load Service */

    }

}

//----------------------------------------------------------------------------
// FUNCTION: Service_UNLOCK
// Unlock aux devices and re-enable Host interface IRQ if it is ok to do so.
//----------------------------------------------------------------------------
static void Service_UNLOCK(void)
{
    /* Unlock_Scan(); */
    Int_Var.Scan_Lock = 0;
}

//----------------------------------------------------------------------------
// FUNCTION: Reserved
//----------------------------------------------------------------------------
static void Service_Reserved_1(void)
{
    ;
}

//----------------------------------------------------------------------------
// FUNCTION: Service polling aux device
//----------------------------------------------------------------------------
static void Service_Reserved_2(void)
{
    ;
}

//----------------------------------------------------------------------------
// FUNCTION: Reserved
//----------------------------------------------------------------------------
static void Service_Reserved_3(void)
{
    ;
}

//-----------------------------------------------------------------------------
// Core 5ms Events 
//-----------------------------------------------------------------------------
void Timer5msEvent(void)
{	  
    ReSendPS2PendingData();
    Hook_Timer5msEvent();
}

//-----------------------------------------------------------------------------
// Core 10ms Events 
//-----------------------------------------------------------------------------
void Timer10msEvent(void)
{
    SetStartScanAUXFlag();
    Hook_Timer10msEvent();
}

//-----------------------------------------------------------------------------
// Core 50ms Events 
//-----------------------------------------------------------------------------
void Timer50msEventA(void)
{
    Hook_Timer50msEventA();
}

//-----------------------------------------------------------------------------
void Timer50msEventB(void)
{
    Hook_Timer50msEventB();
}

//-----------------------------------------------------------------------------
void Timer50msEventC(void)
{
    Hook_Timer50msEventC();
}

//-----------------------------------------------------------------------------
// Core 100ms Events 
//-----------------------------------------------------------------------------
void Timer100msEventA(void)
{
    Hook_Timer100msEventA();
}
//-----------------------------------------------------------------------------
void Timer100msEventB(void)
{
    Hook_Timer100msEventB();

}
//-----------------------------------------------------------------------------
void Timer100msEventC(void)
{ 
    Hook_Timer100msEventC();
}
//-----------------------------------------------------------------------------
// Core 125ms Events 
//-----------------------------------------------------------------------------
void Timer125msEvent(void)
{
    Hook_Timer125msEvent();
}
//-----------------------------------------------------------------------------
// Core 250ms Events 
//-----------------------------------------------------------------------------
void Timer250msEvent(void)
{
    Hook_Timer250msEvent();
}
//-----------------------------------------------------------------------------
// Core 500ms Events 
//-----------------------------------------------------------------------------
void Timer500msEventA(void)
{
    Hook_Timer500msEventA();
}
//-----------------------------------------------------------------------------
void Timer500msEventB(void)
{
    Hook_Timer500msEventB();
}
//-----------------------------------------------------------------------------
void Timer500msEventC(void)
{
    Hook_Timer500msEventC();
}
//-----------------------------------------------------------------------------
// Core 1sec Events/Hook Here
//-----------------------------------------------------------------------------
void Timer1SecEventA(void)
{
    Hook_Timer1SecEventA();
}
//-----------------------------------------------------------------------------
void Timer1SecEventB(void)
{
    Hook_Timer1SecEventB();
}
//-----------------------------------------------------------------------------
void Timer1SecEventC(void)
{
    Hook_Timer1SecEventC();
}

//-----------------------------------------------------------------------------
// 1 min events
//-----------------------------------------------------------------------------
void Timer1MinEvent(void)
{
    Hook_Timer1MinEvent();
}

void Service_MS_5(void)
{
    #if SUPPORT_EC_WDT_RESET
    EWDCNTLR = T_WDTMR_OEM;     // Watch Dog Timeout Values(sec)
    #endif  //SUPPORT_EC_WDT_RESET

    Timer5msEvent();
    Timer5msCnt++;
    Timer125msCnt++;
    if( Timer125msCnt >= 25 )
    {
        Timer125msCnt = 0;
        Timer125msEvent();
    }
    if ( Timer5msCnt & 1 )  // 10ms events
    {
        Timer10msEvent();
        Timer250msCnt++;
        if( Timer250msCnt >= 25 )
        {
            Timer250msCnt = 0;
            Timer250msEvent();
        }
    }
    else
    {
        switch( Timer5msCnt )   // Share Loading Branch Control
        {
            case 2: Timer50msEventA();
                    break;
            case 4: Timer50msEventB();
                    break;
            case 6: Timer50msEventC();
                    break;
            case 8: Timer100msCntB++;
                    if ( Timer100msCntB & 1 )
                    {
                        Timer100msEventA();
                    }
                    else
                    {
                        Timer100msEventB();
                    }
                    break;

            default:    Timer5msCnt=0;
                        break;
        }

        if ( Timer5msCnt == 0x00 )
        {   // 50msec
            Timer100msCnt ++;
            if ( Timer100msCnt & 1 )
            {
                Timer100msEventC();
            }
            else
            {   // 100msec
                switch( Timer100msCnt )
                {
                    case 2:     Timer500msEventA();
                                break;
                    case 4:     Timer500msEventB();
                                break;
                    case 6:     Timer500msEventC();
                                break;
                    case 8:     Timer1SecEventA();
                                break;
                    case 10:    Timer500msEventA();
                                break;
                    case 12:    Timer500msEventB();
                                break;
                    case 14:    Timer500msEventC();
                                break;
                    case 16:    Timer1SecEventB();
                                break;
                    case 18:    Timer1SecEventC();
                                break;
                    default:    // 1 Sec
                                Timer100msCnt = 0;
                                Timer1SecCnt ++;
                                if ( Timer1SecCnt == 10 )
                                {
                                    Timer1MinEvent();
                                    Timer1SecCnt=0;
                                }
                                break;

                }
            }
        }
    }
}

//-----------------------------------------------------------------------------
// Data to RAM Debug function
//-----------------------------------------------------------------------------
void RamDebug(unsigned char dbgcode)
{
    PORT_BYTE_PNTR ClearIndex;
    PORT_BYTE_PNTR byte_register_pntr;
    BYTE index;
    BYTE iLoop;

    byte_register_pntr = DebugRamAddr+DebugRamRange;
    index = *byte_register_pntr;
    *byte_register_pntr +=1;

    if ( *byte_register_pntr == DebugRamRange )
    {
        *byte_register_pntr = 0;
        ClearIndex= DebugRamAddr;

        for (iLoop=0x00;iLoop<DebugRamRange;iLoop++)
        {
            *ClearIndex=0x00;
            ClearIndex++;
        }
    }

    byte_register_pntr = DebugRamAddr + index;
    *byte_register_pntr = dbgcode;
}

//-----------------------------------------------------------------------------


//*****************************************************************************
// CORE MAIN END
//*****************************************************************************

/*-----------------------------------------------------------------------------
 * End
 *---------------------------------------------------------------------------*/
