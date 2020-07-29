/*-----------------------------------------------------------------------------
 * Filename: OEM_CIR.C
 * Function: OEM CIR HANDLE
 *
 * Copyright (c) 2006-2009, ITE Tech. Inc. All Rights Reserved.
 *---------------------------------------------------------------------------*/
#include "CORE_INCLUDE.H"
#include "OEM_INCLUDE.H"

#if SUPPORT_CIR_DEVICE
#define CIRCompareOffset	7

const BYTE code RC6PowerKey[]={	0x95,0x55,0xA5,0xF5 }; 	// RC6 power key

//----------------------------------------------------------------------------
// The function of CIR wake up system 
//----------------------------------------------------------------------------
void Hook_CIRWakeUp(void)
{
    BYTE index;
    BYTE cirdata;
	if(IsFlag1(POWER_FLAG1,enter_S3)||IsFlag0(POWER_FLAG1,system_on))
	{
		CLEAR_MASK(C0SCK, BIT0);  		// CIR unselect slow clock
		for(index=0x00;index<CIRCompareOffset;index++)
		{
			cirdata=C0DR;
			_nop_();
		}

		for(index=0x00;index<sizeof(RC6PowerKey);index++)
		{
			cirdata=C0DR;
			if(cirdata != RC6PowerKey[index])
			{
				ClearCIR();
				return;
			}
		}
	
		if(IsFlag1(POWER_FLAG1,enter_S3))
		{
			CIRS3WakeUp();
		}
		else
		{
			CIRS4S5WakeUp();
		}
	}
}

//----------------------------------------------------------------------------
// The function of CIR S4/S5 wake up system 
//----------------------------------------------------------------------------
void CIRS4S5WakeUp(void)
{
	//Oem_TriggerS5S0();
	SET_MASK(C0MSTCR, BIT0);		//CIR Reset
}

//----------------------------------------------------------------------------
// The function of CIR S3 wake up system 
//----------------------------------------------------------------------------
void CIRS3WakeUp(void)
{
	//Oem_TriggerS3S0();
	SET_MASK(C0MSTCR, BIT0);		//CIR Reset
}
#endif

//----------------------------------------------------------------------------
// The function of CIR Initialization
//----------------------------------------------------------------------------
void Init_CIR(void)
{ 	
	GPCRC0 = 0X00;				// CIR RX (8512 only) Set GPIO to ALT function.
	CLEAR_MASK(C0RCR, BIT7);	// RX Disable     		
	C0BDHR = 0x00;				// BaudRate 
	C0BDLR = 0x33;
	C0MSTCR = 0x08;    			// FIFO Threshold = 17 Bytes
	C0CFR = 0x0B;				// Carrier Freqency = 38KHz 	

	bConfirmCirRIM();  			// Write receiver enter an inactive mode
	bClearCirFIFO();		  	// Clear FIFO

	C0IER = 0x82;				// CIR Interrupt Enable
  								// CIR receive available interrupt   
	SET_MASK(IELMR1, BIT7);		// CIR Interrupt must use Edge-Trig
	SET_MASK(IER1, BIT7);		// Enable INT15 CIR Interrupt
	SET_MASK(ISR1, BIT7);		// Clear INT15 pending Status
	SET_MASK(C0RCR, BIT7);		// RX enable         
}

//----------------------------------------------------------------------------
// The function of CIR module reset
//----------------------------------------------------------------------------
void Reset_CIR(void)
{
	SET_MASK(RSTC2, BIT1);		// Reset CIR module
	CLEAR_MASK(IER1, BIT7);		// Disable INT15 CIR Interrupt
	SET_MASK(ISR1, BIT7);		// Clear INT15 pending Status		     
}

//----------------------------------------------------------------------------
// The function of clearing CIR FIFO
//----------------------------------------------------------------------------
void bClearCirFIFO(void)
{
	SET_MASK(C0MSTCR, BIT1);			// Write 1 for Clearing FIFO data
	_nop_();
	while(IsFlag1(C0MSTCR, BIT1))	    // Wait FIFO Clear Done
	{
		;					
	}
}

//----------------------------------------------------------------------------
// The function of confirming CIR receiver inactive mode
//----------------------------------------------------------------------------
void bConfirmCirRIM(void)
{
	SET_MASK(C0RCR, BIT3);		// Write 1 for clearing Receive Active
	_nop_();
	while(IsFlag1(C0RCR, BIT3))	// Wait CIR receiver enter an inactive mode
	{
		;					
	}
}

//----------------------------------------------------------------------------
// The function of clearing CIR
//----------------------------------------------------------------------------
void ClearCIR(void) 
{
	CLEAR_MASK(C0RCR, BIT7);    // RX Disable   
	bConfirmCirRIM();  			// Write receiver enter an inactive mode
 	bClearCirFIFO();		  	// Clear FIFO
 	SET_MASK(C0RCR, BIT7);		// RX enable 
} 

//----------------------------------------------------------------------------
// The function of CIR Interrupt service routine
//----------------------------------------------------------------------------
void Service_CIR(void)
{
    #if SUPPORT_CIR_DEVICE
    Hook_CIRWakeUp();
    #endif
}

//=============================================================================

