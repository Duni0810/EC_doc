/*-----------------------------------------------------------------------------
 * TITLE: CORE_TIMERS.C
 *
 * Copyright (c) 1983-2009, Insyde Software Corporation. All Rights Reserved.
 *
 * You may not reproduce, distribute, publish, display, perform, modify, adapt,
 * transmit, broadcast, present, recite, release, license or otherwise exploit
 * any part of this publication in any form, by any means, without the prior
 * written permission of Insyde Software Corporation.
 *----------------------------------------------------------------------------*/
#include "CORE_INCLUDE.H"
#include "OEM_INCLUDE.H"


//-----------------------------------------------------------------------------
// FUNCTION: Load_Timer_A - Load Timer A to time-out in 5 mS.
//-----------------------------------------------------------------------------
void Load_Timer_A(void)
{                       // Assume the 8051 clock source is Ring Oscillator
    TH0 = (T_Timer_5ms)>>8;     // Reload timer
    TL0 = (T_Timer_5ms);        // 5ms
    TF0 = 0;
}

//-----------------------------------------------------------------------------
// FUNCTION: Enable_Timer_A - Load the 5 mS timer if not already enabled.
//-----------------------------------------------------------------------------
void Enable_Timer_A(void)
{
    Load_Timer_A();
}

//-----------------------------------------------------------------------------
// FUNCTION: Stop_Timer_A - Stop the 5 mS timer.
//-----------------------------------------------------------------------------
void Stop_Timer_A(void)
{

}

//-----------------------------------------------------------------------------
// FUNCTION: EnableSendToHostTimer
//-----------------------------------------------------------------------------
void EnableSendToHostTimer(void)
{
    TH1 = (T_Timer_SEND)>>8;
    TL1 = (T_Timer_SEND);       // Reload timer

    TF1 = 0;
    TR1 = 1;
    ET1 = 1;
    F_Service_SEND = 1;
}

//-----------------------------------------------------------------------------
// FUNCTION: Load_Timer_B - Load Timer B to time-out in 150 microseconds.
//-----------------------------------------------------------------------------
void Load_Timer_B(void)
{
    TH1 = (T_Timer_1ms)>>8;     // 1ms
    TL1 = (T_Timer_1ms);        // Reload timer

    TF1 = 0;
    TR1 = 1;

}

//-----------------------------------------------------------------------------
// FUNCTION: Stop_Timer_B - Stop the timer B.  Timer 1 is used as Timer B.
//-----------------------------------------------------------------------------
void Stop_Timer_B(void)
{
    TR1=0;

}

//-----------------------------------------------------------------------------
// FUNCTION: Init_Timers
//
// Setup Timer A as a   5 millisecond timer. (Timer 0 is used as Timer A.)
// Setup Timer B as a 150 microsecond timer. (Timer 1 is used as Timer B.)
//-----------------------------------------------------------------------------
void Init_Timers(void)
{
    TF0 = TR0 = 0;              // Stop timer 0 and clear interrupt
    TH0 = (T_Timer_5ms)>>8;     // Reload timer
    TL0 = (T_Timer_5ms);        // 5ms
    TMOD  = 0x11;
    TR0 = 1;                    // Start timer 0 running
    ET0 = 1;

    TF1 = TR1 = 0;              // Stop timer 1 and clear interrupt
    TH1 = (T_Timer_1ms)>>8;     // 1ms
    TL1 = (T_Timer_1ms);        // Reload timer
    TR1 = 1;                    // Start timer 1
    ET1 = 1;                    // Enable timer1 interrupt
}

//-----------------------------------------------------------------------------
// FUNCTION: Microsecond_Delay - Delay a number of microseconds.
// Input:  Code for delay.
//-----------------------------------------------------------------------------
void Microsecond_Delay(WORD delay)
{
    do
    {
        delay--;
    } while (delay);

}

//-----------------------------------------------------------------------------
// Delay 1ms function use timer 1
//-----------------------------------------------------------------------------
void vDelay1MS(WORD bMS)
{
    #if DELAY_1MS_USE_WKCNR
    bit SaveEA;
    BYTE i;

    SaveEA = EA;    // save Interrupt
    EA = 0;         // clear global interrupt

    while(bMS)      // delay 1ms
    {
        for( i=0; i<66; i++ )
            WNCKR = 0;  // 15.26 us
        bMS--;
    }
    EA = SaveEA;        // restore Interrupt
    #else
    bit SaveEA;
    SaveEA = EA;
    TR0 = 0;
    EA  = 0;
    for( ;bMS!=0; bMS-- )
    {
        TR1 = 0;
        TH1 = (T_Timer_1ms)>>8;
        TL1 = (T_Timer_1ms);
        TR1 = 1;
        TF1 = 0;
        while (!TF1)
        {
            if( !TR1 )
            {
                TH1 = T_Timer_1ms>>8;       // set timer1 counter 85 ms
                TL1 = T_Timer_1ms;          // set timer1 counter 85 ms
                TR1 = 1;                    // enable timer1
                TF1 = 0;                    // clear overflow flag
            }
        }
        TF1 = 0;
        TR1 = 0;
    }
    if( SaveEA )    EA = 1;
    TR0 = 1;
    TR1 = 1;
    #endif

}

//----------------------------------------------------------------------------
// Delay Xms function                  
//----------------------------------------------------------------------------
void vDelayXms(BYTE bMS)
{
	ET1 = 0;			// Disable timer1 interrupt
	for (;bMS!=0;bMS--)
	{
		TH1=(T_Timer_1ms)>>8;
    	TL1=(T_Timer_1ms);
		TR1=1;
    	TF1=0;
		while (!TF1);
		TF1=0;
	}
	ET1 = 1;	
}

//-----------------------------------------------------------------------------
// Low Level Timer 5ms Handle
//-----------------------------------------------------------------------------
void Service_MainLoTimer(void)
{
    bit bTemp=FALSE;

    SMB_Check_Timer++;
    if( SMB_Check_Timer > 20 )
    {
        SMBusRecovery();
        SMB_Check_Timer = 0;
    }

    //-------------------------------------------------------------------------
    if( !CORE_PMFLAG_S0 )   //System Power On Flag
    {
        #if ENABLE_S5_KEYBOARD_INTR
        if( CORE_PMFLAG_S5 )        bTemp = TRUE;
        #endif
        #if ENABLE_S3_KEYBOARD_INTR
        if( CORE_PMFLAG_S3 )        bTemp = TRUE;
        #endif
        if( bTemp )
        {
            IER1 |= 0x08;               //Enable KSI interrupt
            if( Timer_A.fbit.TMR_SCAN )
            {
                F_Service_KEYSCAN = 1;  // Request scanner service.
            }
        }
        return;
    }
    //-------------------------------------------------------------------------
    // CORE S0 (Host Power On)
    //-------------------------------------------------------------------------
    #if USE_SERVICE_EVENT_CENTER
    Service_EventCenter();
    #endif
    //-------------------------------------------------------------------------
    IER1 |= 0x08;               // Enable KSI interrupt
    //-------------------------------------------------------------------------
    if( Timer_A.fbit.TMR_SCAN )
    {
        F_Service_KEYSCAN = 1;  // Request scanner service.
    }
    //-------------------------------------------------------------------------

    if( KBPendingRXCount > 0 )
    {
        F_Service_SEND = 1;
    }
    //-------------------------------------------------------------------------
    if( !( Ccb42_DISAB_KEY )   )
    {
        if( (scan.kbf_head != scan.kbf_tail) )
        {
            F_Service_SEND = 1;
        }
    }

}
//-----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// INIT UART
//----------------------------------------------------------------------------
void Init_UART(void)
{
#ifdef UART_Debug
   	GPCRB0=PALT;
    GPCRB1=PALT;
    SCON=0x40;
    T2CON &= 0xF0;               /* EXEN2=0; TR2=0; C/T2#=0; CP/RL2#=0; */
    T2CON |= 0x30;               /* RCLK = 1; TCLK=1; */
    T2MOD=0x00;
    RCAP2H=R2HV;
    RCAP2L=R2LV;
    TR2=1;
    TI = 1;
#endif    
}

/*-----------------------------------------------------------------------------
 * End
 *---------------------------------------------------------------------------*/