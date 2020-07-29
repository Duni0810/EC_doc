/*-----------------------------------------------------------------------------
 * TITLE: CORE_HSPI.C
 *
 * Copyright (c) 2009- , ITE Corporation. All Rights Reserved.
 *
 * You may not reproduce, distribute, publish, display, perform, modify, adapt,
 * transmit, broadcast, present, recite, release, license or otherwise exploit
 * any part of this publication in any form, by any means, without the prior
 * written permission of Insyde Software Corporation.
 *---------------------------------------------------------------------------*/
#include "CORE_INCLUDE.H"
#include "OEM_INCLUDE.H"

//----------------------------------------------------------------------------
// Disable HSPI interface pins 
//----------------------------------------------------------------------------
void GPIO_HSPI_INIT(void)
{
	GPCRH3 = INPUT;                 // Set input mode
	GPCRH4 = INPUT;                 // Set input mode
	GPCRH5 = INPUT;                 // Set input mode
	GPCRH6 = INPUT;                 // Set input mode
    SET_MASK(RSTC4,BIT0);           // Reset SPI
}

//----------------------------------------------------------------------------
// Enable HSPI interface
//----------------------------------------------------------------------------
void Enable_HSPI(void)
{
	GPCRH3 = PALT;                  // Set HSPI mode
	GPCRH4 = PALT;                  // Set HSPI mode
	GPCRH5 = PALT;                  // Set HSPI mode
	GPCRH6 = PALT;                  // Set HSPI mode
    
	FLHCTRL1R = 0x00;			    // Flash Control Register 1
	HCTRL2R = 0x10;				    // Host Control 2 Register
    HINSTC1 = (ENDPI+ENDEI);        // Host Instruction Control 1
    
	R103A = 0x10;				    // set bit4 for not SST SPI ROM
								    // clear bit4 for SST SPI ROM or set bit4 for not SST SPI ROM
                           		    // For phoenix bios    
                           		    
    SET_MASK(IER7,BIT3);            // Enable interrupt 
    SET_MASK(ISR7,BIT3);            // W/C interrupt status
}

//----------------------------------------------------------------------------
// Disable HSPI interface
//----------------------------------------------------------------------------
void Disable_HSPI(void)
{
	GPCRH3 = INPUT;                 // Set input mode
	GPCRH4 = INPUT;                 // Set input mode
	GPCRH5 = INPUT;                 // Set input mode
	GPCRH6 = INPUT;                 // Set input mode

    CLEAR_MASK(IER7,BIT3);          // Disable HSPI interrupt 
    SET_MASK(ISR7,BIT3);            // W/C interrupt status
    HCTRL2R = 0x80;		
    SET_MASK(RSTC4,BIT(0));         // Reset SPI
}

#pragma ot(8, speed)
//----------------------------------------------------------------------------
// Instructino is valid
//----------------------------------------------------------------------------
void HSPI_RamCode(void)
{
    if((DSINST==SPICmd_Erase4KByte)||(DSINST==SPICmd_BYTEProgram)||(DSINST==SPICmd_Erase64KByte))  // Erase 4k 64K and page program command
    {
        if(DSADR2==HSPI_ECCodeSpaceAddr)    // EC code space
        {
            HSPI_SelfRamCode_Index = 0x33;
            return;
        }
        ET1 = 0;			            // Disable timer1 interrupt
        TH1 = (T_Timer_85ms)>>8;	    // set timer1 counter 85 ms 
        TL1 = (T_Timer_85ms);		    // set timer1 counter 85 ms
        TR1 = 1;			            // enable timer1
        TF1 = 0;			            // clear overflow flag

        HINSTC1 |= DISSV+DISS;	        // Allow instruction

        while(!TF1)
        {
            if(IsFlag1(ISR7, BIT3))
            {
                if(DSADR2==HSPI_ECCodeSpaceAddr)    // EC code space
                {
                    HSPI_SelfRamCode_Index = 0x33;
                    TR1 = 0;
                    break;
                }
                HINSTC1 |= DISSV+DISS;	// Allow instruction
                SET_MASK(ISR7,BIT3);
                TH1 = (T_Timer_85ms)>>8;  // set timer1 counter 85 ms 
                TL1 = (T_Timer_85ms);   // set timer1 counter 85 ms
                TR1 = 1;			    // enable timer1
                TF1 = 0;
            }
        }
	    TR1 = 0;			            // disable timer1
 	    TF1 = 0;			            // clear overflow flag
	    ET1 = 1;			            // Enable timer1 interrupt
    }
    else
    {
        if(DSADR2==HSPI_ECCodeSpaceAddr)    // EC code space
        {
            HSPI_SelfRamCode_Index = 0x33;
            return;
        }
        HINSTC1 |= DISSV+DISS;	        // Allow instruction
    }   
}

#pragma ot(8, speed)
//----------------------------------------------------------------------------
// Instructino is valid
//----------------------------------------------------------------------------
void HSPI_SelfRamCode(void)
{
    HSPI_FPT_timeout = 0x00;
    
    HINSTC1 |= DISSV+DISS;	        // Allow instruction

    ET1 = 0;			// Disable timer1 interrupt
    TH1 = 0x00;			// set timer1 counter 85 ms 
    TL1 = 0x00;			// set timer1 counter 85 ms
    TR1 = 1;			// enable timer1
    TF1 = 0;			// clear overflow flag
    
    while(1)
    {
        if (IsFlag1(KBHISR,IBF))
        {
			HSPI_tempcmd = KBHIDIR;
        }
        if(IsFlag1(ISR7, BIT3))
        {
            HINSTC1 |= DISSV+DISS;	// Allow instruction
            HSPI_FPT_timeout = 0x00;
            SET_MASK(ISR7,BIT3);
        }
        else
        {
            if(HSPI_FPT_timeout>=200)// > 17 sec
            {
                TR1 = 0;			// disable timer1
                TF1 = 0;			// clear overflow flag
                ET1 = 0;			// enable timer1 interrupt
                HSPI_FPT_timeout = 0xff;

                #if 1
	            GPCRH3 = INPUT;
	            GPCRH4 = INPUT;
	            GPCRH5 = INPUT;
	            GPCRH6 = INPUT;
  	            CLEAR_MASK(IER7,BIT3);
  	            SET_MASK(ISR7,BIT3);
                HCTRL2R = 0x80;		
                SET_MASK(RSTC4,BIT0);   // Reset SPI
                WDTCON |= 0x01;         // WDTRST = 1 Reset watch dog timer.
                WDTCON |= 0x02;         // WDTEN  = 1 Enable watch dog.     
       		    while(1);		        // Wait for watch dog time-out
       		    #else
                ExitRamCodeCommand = 0xAA;
                break;
                #endif
            }
            if(TF1)
            {
                HSPI_FPT_timeout++;
                TH1 = 0x00;			    // set timer1 counter 85 ms 
                TL1 = 0x00;			    // set timer1 counter 85 ms
                TF1 = 0;			    // clear overflow flag
            }
        }
    }
    main();
}

//-----------------------------------------------------------------------------
// The function will long jmp to 0xFE00
//-----------------------------------------------------------------------------
void HSPI_JmpTo0XFE00(void)
{
    FlashECCode();                          // Jmp to 0xFE00
}

//-----------------------------------------------------------------------------
// Process HSPI ISR
//-----------------------------------------------------------------------------
void HSPI_Process(void)
{
    DisableAllInterrupt();                  // Disable all interrupt 
    SET_MASK(FBCFG,SSMC);       	        // enable scatch RAM
    HSPI_JmpTo0XFE00();                     // Go to HSPI_RamCode();
    CLEAR_MASK(FBCFG,SSMC);                 // disable scatch RAM
    if(HSPI_SelfRamCode_Index==0x33)        
    {
        HSPI_SelfRamCode_Index = 0x00;
        LoadSPIFucnToRam(HSPI_SelfRamCode);	// Load function to ram
        SET_MASK(FBCFG,SSMC);       	    // enable scatch RAM
        HSPI_JmpTo0XFE00();                 // Go to HSPI_SelfRamCode();
    }
    SET_MASK(IER7,BIT3);                    // Enable HSPI interrupt
}

//-----------------------------------------------------------------------------
// Need re-load HSPI ram code function to memory
//-----------------------------------------------------------------------------
void HSPI_SetReloadRamCodeFlag(void)
{
    #ifdef HSPI
    SET_MASK(HSPI_Misc1, HSPI_ReLoadHSPIRamCode);
    #endif
}