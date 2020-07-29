/*-----------------------------------------------------------------------------
 * Filename: BBK_IRQ.C         For Chipset: ITE.IT85XX
 *
 * Function: Interrupt Service Routines (ISR).
 *
 * Copyright (c) 2006-2009, ITE Tech. Inc. All Rights Reserved.
 *---------------------------------------------------------------------------*/
const unsigned char code ITE_Signature[] =
{
    0xA5,0xA5,0xA5,0xA5,0xA5,0xA5,0xA5,0xA5,0x85,0x12,0x5A,0x5A,0xAA,0xAA,0x55,0x55,
};

const unsigned char code RomString1[] =  "EC-V12.4 Copyright ITE Tech. Inc.   ";

//-----------------------------------------------------------------------------
// Common Header Include
//-----------------------------------------------------------------------------
#include    "BBK_COMM.H"

//-----------------------------------------------------------------------------
// The function of clearing internal and external ram
//
//-----------------------------------------------------------------------------
void Init_ClearRam(void)
{
    IIBYTE *IdataIndex;
    PORT_BYTE_PNTR byte_register_pntr;

    IdataIndex = 0x20;
    byte_register_pntr=0x04;        //Pass BBK Information

    while( byte_register_pntr < (0x1000-0x04) )
    {   // Clear external ram (0x004~0xFFF)
        *byte_register_pntr = 0;
        byte_register_pntr++;
    }

    while( IdataIndex < 0xD0 )      // Clear internal ram (0x20~0xCF)
    //while(IdataIndex<0x100)       // Clear internal ram (0x20~0xFF)
    {
        *IdataIndex = 0;
        IdataIndex++;
    }

}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// External Link Include
//-----------------------------------------------------------------------------


/* ************************************************************************* */
/* IRQ CODE START                                                            */
/* ************************************************************************* */
#define PS2_InhibitMode         0x11

//-----------------------------------------------------------------------------
// AUX port1 interrupt routine
//-----------------------------------------------------------------------------
void Irq_Aux_Port1(void) using 2
{
	if(IsFlag1(PSSTS1, TDS))	    // Transaction done interrupt 
	{
        CLEAR_BIT(IER2,4);		    // Disable PS2 interrupt 0
		SET_BIT(ISR2,4);		    // Write to clear pending interrupt
        WNCKR = 0x00;               // Delay 15.26 us
        WNCKR = 0x00;               // Delay 15.26 us
		PSCTL1 = PS2_InhibitMode;   // Inhibit clock pin1
		PSCTL2 = PS2_InhibitMode;   // Inhibit clock pin2
		PSCTL3 = PS2_InhibitMode;   // Inhibit clock pin3

	    PS2_IRQ_Channel = 0;        // Set PS2 0 service flag
		F_Service_PS2_IRQ = 1;      // Set main service flag
	    PS2StartBit=0;			    // clear start bit flag
	    PS2_SSIRQ_Channel = 0xFF;   //
		PS2PortxData[0]=PSDAT1;
	}
    else						    // Start bit interrupt 
    {
        PSCTL2 = PS2_InhibitMode;   // Inhibit clock pin2
        PSCTL3 = PS2_InhibitMode;   // Inhibit clock pin3
        IER2&=(~0x1C);			    // Disable all PS2 interrupt
        ISR2|=0x1C;				    // Write to clear all PS2 pending interrupt
        PS2StartBit = 1;		    // Set start bit flag
        PS2_SSIRQ_Channel = 0;      //
        SET_BIT(IER2,4);		    // Enable PS2 interrupt 0
    }	
}

//----------------------------------------------------------------------------
// AUX port2 interrupt routine
//----------------------------------------------------------------------------
void Irq_Aux_Port2(void) using 2
{
	if(IsFlag1(PSSTS2, TDS))	    // Transaction done interrupt 
	{
		CLEAR_BIT(IER2,3);	        // Disable PS2 interrupt 1
		SET_BIT(ISR2,3);	        // Write to clear pending interrupt     
        WNCKR = 0x00;               // Delay 15.26 us
        WNCKR = 0x00;               // Delay 15.26 us
		PSCTL1 = PS2_InhibitMode;   // Inhibit clock pin1
		PSCTL2 = PS2_InhibitMode;   // Inhibit clock pin2
		PSCTL3 = PS2_InhibitMode;   // Inhibit clock pin3
   
	    PS2_IRQ_Channel = 1;        // Set PS2 1 service flag
		F_Service_PS2_IRQ = 1;      // Set main service flag
        PS2StartBit=0;		        // clear start bit flag
        PS2_SSIRQ_Channel = 0xFF;   //
	    PS2PortxData[1]=PSDAT2;
	}
    else					        // Start bit interrupt 
    {
        PSCTL1 = PS2_InhibitMode;   // Inhibit clock pin1
        PSCTL3 = PS2_InhibitMode;   // Inhibit clock pin3
        IER2&=(~0x1C);		        // Disable all PS2 interrupt
        ISR2|=0x1C;			        // Write to clear all PS2 pending interrupt
        PS2StartBit = 1;	        // Set start bit flag
        PS2_SSIRQ_Channel = 1;      //
        SET_BIT(IER2,3);		    // Enable PS2 interrupt 1
    }	
}

//----------------------------------------------------------------------------
// AUX port3 interrupt routine
//----------------------------------------------------------------------------
void Irq_Aux_Port3(void) using 2
{
	if(IsFlag1(PSSTS3, TDS))	    // Transaction done interrupt 
	{
		CLEAR_BIT(IER2,2);	        // Disable PS2 interrupt 2
		SET_BIT(ISR2,2);	        // Write to clear pending interrupt        
        WNCKR = 0x00;               // Delay 15.26 us
        WNCKR = 0x00;               // Delay 15.26 us
		PSCTL1 = PS2_InhibitMode;   // Inhibit clock pin1
		PSCTL2 = PS2_InhibitMode;   // Inhibit clock pin2
		PSCTL3 = PS2_InhibitMode;   // Inhibit clock pin3

		PS2_IRQ_Channel = 2;        // Set PS2 2 service flag
		F_Service_PS2_IRQ = 1;      // Set main service flag
		PS2StartBit=0;		        // clear start bit flag
	    PS2_SSIRQ_Channel = 0xFF;   //
	    PS2PortxData[2]=PSDAT3;
	}
    else					        // Start bit interrupt 
    {
        PSCTL1 = PS2_InhibitMode;   // Inhibit clock pin1
        PSCTL2 = PS2_InhibitMode;   // Inhibit clock pin2
        IER2&=(~0x1C);		        // Disable all PS2 interrupt
        ISR2|=0x1C;			        // Write to clear all PS2 pending interrupt
        PS2StartBit = 1;	        // Set start bit flag
        PS2_SSIRQ_Channel = 2;      //
        SET_BIT(IER2,2);		    // Enable PS2 interrupt 2
    }	
}  


//-----------------------------------------------------------------------------
// Port 60/64 input buffer full interrupt routine
//-----------------------------------------------------------------------------
void Irq_Ibf1(void)  using 2
{
    Int_Var.Scan_Lock = 1;
    CLEAR_BIT(IER3,0);
    SET_BIT(ISR3,0);
    F_Service_PCI = 1;
}

//-----------------------------------------------------------------------------
// Port 62/66, 68/6C input buffer full interrupt routine
//-----------------------------------------------------------------------------
void Irq_Ibf2(void) using 2
{
    if(IsFlag1(PM2STS, BIT1))   // 68/6C input buffer full
    {
        F_Service_PCI3 = 1;
    }
    else                        // 62/66 input buffer full
    {
        F_Service_PCI2 = 1;
    }
    CLEAR_BIT(IER3,1);
    SET_BIT(ISR3,1);
}

//-----------------------------------------------------------------------------
// ADC interrupt routine
//-----------------------------------------------------------------------------
void Irq_ADC(void)  using 2
{

}

//-----------------------------------------------------------------------------
// SMBus0 interrupt routine
//-----------------------------------------------------------------------------
void Irq_SMB0(void) using 2
{

}

//-----------------------------------------------------------------------------
// SMBus1 interrupt routine
//-----------------------------------------------------------------------------
void Irq_SMB1(void) using 2
{

}

//-----------------------------------------------------------------------------
// CIR interrupt routine
//-----------------------------------------------------------------------------
void Irq_CIR(void) using 2
{
    ClearFlag(IER1, BIT7);      // Disable INT15 CIR Interrupt
    SetFlag(ISR1, BIT7);        // Clear INT15 pending Status
    F_Service_CIR = 1;
}

//----------------------------------------------------------------------------
// FUNCTION:   Irq_HPSI
// ISR for INT59 WUC Interrupt  for HSPI Process
//----------------------------------------------------------------------------
#ifdef HSPI
void Irq_HSPI(void) using 2
{
	CLEAR_MASK(IER7,BIT3);
    SET_MASK(ISR7,BIT3);
    Service_HSPI=0x01;  
}
#endif

//-----------------------------------------------------------------------------
// Null function
//-----------------------------------------------------------------------------
void IrqNull(void) using 2
{
    ;
}

//-----------------------------------------------------------------------------
// Irq_Anykey - Direct Keyboard Scan key press interrupt handler.
// Disable further key interrupts and post key service request.
//-----------------------------------------------------------------------------
void Irq_Anykey(void)  using 2
{
    F_Service_KEYSCAN = 1;  // Post service request to scan internal keyboard.
    IER1&=(~0x08);          // Disable interrupt of keyboard
    CLEAR_BIT(IER1, 3);
    SET_BIT(ISR1, 3);
}

/* ----------------------------------------------------------------------------
 * FUNCTION:   Isr_LPCRST
 * ISR for INT17 WUC Interrupt WUI4 for LPCRST Process
 * ------------------------------------------------------------------------- */
void Irq_LPCRST(void)  using 2
{
    CLEAR_BIT(IER2, 1);
    SET_BIT(ISR2, 1);
    F_Service_LPCRST  = 1;
}

//-----------------------------------------------------------------------------
// FUNCTION:   Isr_Int0
// ISR for INT0 (Key Scan Interrupt).
//-----------------------------------------------------------------------------
void Isr_Int0(void) interrupt 0 using 2
{

}

//-----------------------------------------------------------------------------
// Isr_Tmr0
// ISR for TF0 (Timer 0 Interrupt).  Timer 0 is the 1mS Timer (Timer A).
//-----------------------------------------------------------------------------
/* ----------------------------------------------------------------------------
 * FUNCTION: Irq_Timer_A - Handle Timer A interrupts.
 *
 * Timer A has a time base of 1 mS.
 *
 * Input: On entry, "TIMER Timer_A" has the event(s) to handle. Valid Events:
 *     TMR_SCAN   -
 *     TMR_GENTMO - This timer is a timer that is used for timeout detection
 *                  of serial data transmissions to/from the auxiliary
 *                  keyboard/pointing devices.
 * ------------------------------------------------------------------------- */
void Isr_Tmr0(void) interrupt 1 using 2
{
    TH0 = (T_Timer_5ms)>>8;     // Reload timer
    TL0 = (T_Timer_5ms);        // 5ms
    TF0 = 0;
    if( !(F_Service_MS_5 ) )
    {
        F_Service_MS_5 = 1;     // Request 5 mS timer service.
        F_Service_MainLoTimer = 1;
        #if SUPPORT_OEM_LOW_TIMER
        F_Service_OEM_LoTimer = 1;
        #endif
    }
}


/* ----------------------------------------------------------------------------
 * FUNCTION:   Isr_Int1
 * ISR for INT1 (Interrupt Source A and B Interrupts).
 *
 * Interrupt Source A:
 * ------------------------------------------------------------------------- */
const FUNCT_PTR_V_V code IRQ_Service[] =
{
    IrqNull,            //0x10 INT0  Reserved
    IrqNull,            //0x11 INT1  WUC interrupt WUI0
    IrqNull,            //0x12 INT2  KBC output buffer empty interrupt
    IrqNull,            //0x13 INT3  PMC output buffer empty interrupt
    IrqNull,            //0x14 INT4  ------------------------------------------
    IrqNull,            //0x15 INT5  WUC interrupt (WU10 ~ WU15)(WU40 ~ WU47)
    IrqNull,            //0x16 INT6  WUC interrupt WUI3
    IrqNull,            //0x17 INT7  PWM interrupt
    Irq_ADC,            //0x18 INT8  ADC Interrupt
    Irq_SMB0,           //0x19 INT9  SMB0 Interrupt
    Irq_SMB1,           //0x1A INT10 SMB1 Interrupt
    Irq_Anykey,         //0x1B INT11 Key matrix scan Int
    IrqNull,            //0x1C INT12 WUC interrupt SWUC wake up
    IrqNull,            //0x1D INT13 WUC interrupt KSI wake-up (WU30 ~ WU37)
    IrqNull,            //0x1E INT14 WUC interrupt Power switch
    Irq_CIR,            //0x1F INT15 CIR interrupt
    IrqNull,            //0x20 INT16 SMB2 Interrupt
    Irq_LPCRST,         //0x21 INT17 WUC Interrupt WUI4 for LPCRST Process
    Irq_Aux_Port3,      //0x22 INT18 PS2 P2 Interrupt
    Irq_Aux_Port2,      //0x23 INT19 PS2 P1 Interrupt
    Irq_Aux_Port1,      //0x24 INT20 PS2 P0 Interrupt
    IrqNull,            //0x25 INT21 WUC Interrupt WUI2
    IrqNull,            //0x26 INT22 SMFI Semaphore Interrupt
    IrqNull,            //0x27 INT23 SMFI Lock Error Interrupt
    Irq_Ibf1,           //0x28 INT24 KBC input buffer empty interrupt
    Irq_Ibf2,           //0x29 INT25 PMC/PMC1 input buffer empty interrupt
    IrqNull,            //0x2A INT26 ------------------------------------------
    IrqNull,            //0x2B INT27 PMC2 input buffer empty interrupt
    IrqNull,            //0x2C INT28 GINT from function 1 of GPD5
    IrqNull,            //0x2D INT29 EGPC interrupt
    IrqNull,            //0x2E INT30 External timer interrupt
    IrqNull,            //0x2F INT31 WKO[21]->WU21->WUI1 WUC interrupt WUI1
    IrqNull,            //0x30 INT32 KBC output buffer empty interrupt
    IrqNull,            //0x31 INT33 PMC output buffer empty interrupt
    IrqNull,            //0x32 INT34 GPINT0
    IrqNull,            //0x33 INT35 GPINT1
    IrqNull,            //0x34 INT36 GPINT2
    IrqNull,            //0x35 INT37 GPINT3
    IrqNull,            //0x36 INT38
    IrqNull,            //0x37 INT39
    IrqNull,            //0x38 INT40
    IrqNull,            //0x39 INT41
    IrqNull,            //0x3A INT42
    IrqNull,            //0x3B INT43
    IrqNull,            //0x3C INT44
    IrqNull,            //0x3D INT45
    IrqNull,            //0x3E INT46
    IrqNull,            //0x3F INT47
    IrqNull,            //0x40 INT48 WKO[60]->WU60->WUI16
    IrqNull,            //0x41 INT49 WKO[61]->WU61->WUI16
    IrqNull,            //0x42 INT50 WKO[62]->WU62->WUI17
    IrqNull,            //0x43 INT51 WKO[63]->WU63->WUI18
    IrqNull,            //0x44 INT52 WKO[64]->WU64->WUI19
    IrqNull,            //0x45 INT53 WKO[65]->WU65->WUI20
    IrqNull,            //0x46 INT54 WKO[66]->WU66->WUI21
    IrqNull,            //0x47 INT55 WKO[67]->WU67->WUI22
    IrqNull,            //0x48 INT56
    IrqNull,            //0x49 INT57
    IrqNull,            //0x4A INT58
#ifdef HSPI    
 	Irq_HSPI,			//0x4B INT59
#else
    IrqNull,	        //0x4B INT59
#endif 
    IrqNull,            //0x4C INT60
    IrqNull,            //0x4D INT61
    IrqNull,            //0x4E INT62
    IrqNull,            //0x4F INT63
};
//-----------------------------------------------------------------------------
const FUNCT_PTR_V_V code IRQ_Service2[] =
{
    IrqNull,            //0x50 INT64
    IrqNull,            //0x51 INT65
    IrqNull,            //0x52 INT66
    IrqNull,            //0x53 INT67
    IrqNull,            //0x54 INT68
    IrqNull,            //0x55 INT69
    IrqNull,            //0x56 INT70
    IrqNull,            //0x57 INT71
    IrqNull,            //0x58 INT72
    IrqNull,            //0x59 INT73
    IrqNull,            //0x5A INT74
    IrqNull,            //0x5B INT75
    IrqNull,            //0x5C INT76
    IrqNull,            //0x5D INT77
    IrqNull,            //0x5E INT78
    IrqNull,            //0x5F INT79
};
//-----------------------------------------------------------------------------
void Isr_Int1 (void) interrupt 2 using 2
{
    if( IVECT < 0x50 )
    {
        (IRQ_Service[(IVECT-0x10)&0x3F])();     // Dispatch to service handler.
    }
    else if( IVECT < 0x60 )
    {
        (IRQ_Service2[(IVECT-0x50)&0x0F])();    // Dispatch to service handler.
    }
    else
    {
        IrqNull();
    }
}

/* ----------------------------------------------------------------------------
 * FUNCTION:   Isr_Tmr1
 * ISR for TF1 (Timer 1 Interrupt).  Timer 1 is the 300uS Timer (Timer B).
 * ------------------------------------------------------------------------- */
void Isr_Tmr1(void) interrupt 3 using 2
{
    //Stop_Timer_B();
    TR1=0;

    if ( Timer_B.fbit.SEND_ENABLE )
    {
        F_Service_SEND = 1;         // Post service request.
        Timer_B.fbit.SEND_ENABLE = 0;
        //Load_Timer_B();
        TH1 = (T_Timer_SEND)>>8;    // Reload timer
        TL1 = (T_Timer_SEND);

        TF1 = 0;
        TR1 = 1;
    }

}

/* ----------------------------------------------------------------------------
 * FUNCTION:   Isr_UART
 * ISR for TI or RI (UART Interrupt).
 * ------------------------------------------------------------------------- */
void Isr_UART(void) interrupt 4 using 2
{
    ;
}


/* ----------------------------------------------------------------------------
 * FUNCTION:   Isr_Tmr2
 * ISR for TF2 or EXF2 (Timer 2 Interrupt).
 * ------------------------------------------------------------------------- */
void Isr_Tmr2(void) interrupt 5 using 2
{
   
}
//-----------------------------------------------------------------------------

/* ************************************************************************* */
// CODE END
