/*-----------------------------------------------------------------------------
 * Filename: CORE_FLASH.C - ITE Flash SPI Service
 * Function: Run program code in RAM space and for SPI flash.
 *           PM1 I/O EC SPI service.
 *
 * Copyright (c) 2006-2009, ITE Tech. Inc. All Rights Reserved.
 *---------------------------------------------------------------------------*/
#include "CORE_INCLUDE.H"
#include "OEM_INCLUDE.H"

//-----------------------------------------------------------------------------
// The function start address always at 0xFE00 (refer to link file)
//-----------------------------------------------------------------------------
void FuncAt_0xFE00(void)
{
	FlashECCode();
}

//-----------------------------------------------------------------------------
// The function start address always at 0xFF00 (refer to link file)
//-----------------------------------------------------------------------------
void FuncAt_0xFF00(void)
{
	RamCode();
}

//-----------------------------------------------------------------------------
// The function of keeping CLKRUN pin low
//-----------------------------------------------------------------------------
void ForceLPCClockRun(void)
{
	GPCRH0 = OUTPUT;
	CLEAR_MASK(GPDRH,BIT(0));
}

//-----------------------------------------------------------------------------
// The function of flashing
//-----------------------------------------------------------------------------
void RamCode(void)
{
    SaveKBHICR = KBHICR;
    KBHICR &= ~0x01;
    SET_BIT( SMECCS, 5 );   // Host write allow (Enabled)

    #if RamCode_useKBC
    if( RamCodePort == 0x64 )
    {
        KBHIKDOR = 0xFA;
    }
    #endif
    #if RamCode_useEC
    if( RamCodePort == 0x66 )
    {
        PM1DO = 0xFA;
    }
    #endif
    #if RamCode_usePM2
    if( RamCodePort == 0x6C )
    {
        PM2DO = 0xFA;
    }
    #endif
    while(1)
    {
        //RunCodeInRamCount++;  //Open For Debug
        if( RamCodePort == 0x64 )
        {
            if ( IS_BIT_CLEAR(KBHISR,1) )   continue;
            HostCMD = KBHIDIR;
            if ( IS_BIT_CLEAR(KBHISR,3) )   continue;
        }
        #if RamCode_useEC
        else if( RamCodePort == 0x66 )
        {
            if ( IS_BIT_CLEAR(PM1STS,1) )   continue;
            HostCMD = PM1DI;
            if ( IS_BIT_CLEAR(PM1STS,3) )   continue;
        }
        #endif
        #if RamCode_usePM2
        else if( RamCodePort == 0x6C )
        {
            if ( IS_BIT_CLEAR(PM2STS,1) )   continue;
            HostCMD = PM2DI;
            if ( IS_BIT_CLEAR(PM2STS,3) )   continue;
        }
        #endif
        if( HostCMD == RamCode_ExitCMD1 )
        {
            ExitRamCodeCommand = 0xAA;
            break;
        }
        else if( HostCMD == RamCode_ExitCMD2 )
        {
            ExitRamCodeCommand = 0xAA;
            break;
        }
        else if ( HostCMD == 0xD7 )
        {
            CLR_BIT( SMECCS, 5 );
            _nop_();
            _nop_();
            SET_BIT( SMECCS, 5 );
        }
        #if Enable_RamCode_B5_B6
        /* 0xB5,0xB6 for BIOS Function */
        else if ( HostCMD == 0xB5 )
        {
            while( IS_BIT_CLEAR(KBHISR,1) );
            HostCMD = KBHIDIR;
            KBHIKDOR = SPIFlashData[KBHIDIR];
        }
        else if ( HostCMD == 0xB6 )
        {
            while( IS_BIT_CLEAR(KBHISR,1) );
            HostCMD = KBHIDIR;
            while( IS_BIT_CLEAR(KBHISR,1) );
            SPIFlashData[HostCMD] = KBHIDIR;
        }
        #endif
        else if ( HostCMD == RamCode_ResetCMD )
        {
            WDTCON |= 0x01;     /* WDTRST = 1 Reset watch dog timer.*/
            WDTCON |= 0x02;     /* WDTEN  = 1 Enable watch dog.     */
            while(1);           /* Wait for watch dog time-out      */
        }
    }
    //-------------------------------------------------------------------------
    CLEAR_BIT(SMECCS,5);        // Host write not allow
    KBHICR = SaveKBHICR;
    _nop_();
    MPREFC = 0x01;
    _nop_();
    MPREFC = 0x01;
    _nop_();
    MPREFC = 0x01;
    _nop_();
    MPREFC = 0x01;
    _nop_();
    //-------------------------------------------------------------------------
    GPCRH0 = PALT;
    main();
    //-------------------------------------------------------------------------
}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// The function of Scratch rom
//-----------------------------------------------------------------------------
void RunCodeInRAM(void)
{
    DisableAllInterrupt();
    ExitRamCodeCommand = 0x00;

    ForceLPCClockRun();
    //#if SUPPORT_PH0_CLKRUN
    ///* Check & Turn-Off CLKRUN# */
    //if( GPIO_H0_CTRL == PALT )
    //{
    //    GPIO_H0_CTRL = OUTPUT;
    //    _GPIOH._Pin0 = _LOW;
    //}
    //#endif

    #if ITE_EC_CHIP_8510
    /* SHARED MEMORY EC OVERRIDE WRITE PROTECT REGISTER */
    SMECOWPR0 = 0;
    SMECOWPR1 = 0;
    SMECOWPR2 = 0;
    SMECOWPR3 = 0;
    SMECOWPR4 = 0;
    SMECOWPR5 = 0;
    SMECOWPR6 = 0;
    SMECOWPR7 = 0;
    SMECOWPR8 = 0;
    SMECOWPR9 = 0;
    /* SHARED MEMORY EC OVERRIDE READ PROTECT REGISTER */
    SMECORPR0 = 0;
    SMECORPR1 = 0;
    SMECORPR2 = 0;
    SMECORPR3 = 0;
    SMECORPR4 = 0;
    SMECORPR5 = 0;
    SMECORPR6 = 0;
    SMECORPR7 = 0;
    SMECORPR8 = 0;
    SMECORPR9 = 0;
    #endif

    Tmp_XPntr = 0x700;
    Tmp_code_pointer = 0xFF00;

    for( _R1=0; _R1<(RamCode_Sizes); _R1++ )
    {
        *Tmp_XPntr = *Tmp_code_pointer;
        Tmp_XPntr++;
        Tmp_code_pointer++;
    }

    #if SUPPORT_EC_WDT_RESET
    EWDCNTLR = T_WDTMR_FLASH;   // Watch Dog Timeout Values(sec)
    #endif  //SUPPORT_EC_WDT_RESET

    while(1)
    {   // Wait OBF Clear
        if( IS_BIT_CLEAR(KBHISR,0) )
            break;
    }

    SET_BIT( FBCFG, 7 );        // Enable scatch ROM

    FuncAt_0xFF00();
}
//-----------------------------------------------------------------------------

#if SUPPORT_EC_FLASH_CODE
//-----------------------------------------------------------------------------
/*****************************************************************************/
/*****************************************************************************/
//
//      EC Flash Service (Use PM I/O Port62h/66h)
//
/*****************************************************************************/
/*****************************************************************************/
//-----------------------------------------------------------------------------
void FlashECCode(void)
{
	_FlashCMD = 0x00;
	_SEND = 0x00;

	PM1DO = 0x33;		// ACK

 	while(1) 
  	{	
  		if( IsFlag1(KBHISR,IBF) )
  		{
			_FlashCMD = KBHIDIR;
			continue;
		}
		if( IsFlag0(PM1STS,IBF) ) continue;
		if( IsFlag0(PM1STS,C_D) ) continue;
		_FlashCMD = PM1DI;

		if(_SEND==1)
		{
			ECINDDR = _FlashCMD;
			_SEND = 0x00;
			continue;
		}

		if(_FlashCMD==0x01)
		{
			ECINDAR3 = 0x0F;        // Enter follow mode
   			ECINDAR2 = 0xFF;
			ECINDAR1 = 0xFE;
   			ECINDAR0 = 0x00;   		// FFFFExx = 0xFF   
   			ECINDDR = 0x00;			// SCE# high level
		}
		else if(_FlashCMD==0x02)
		{							// Send SPI command
			ECINDAR1 = 0xFD;
			_SEND = 1;
		}
		else if(_FlashCMD==0x03)
		{							// write byte to spi
			_SEND = 1;
		}
		else if(_FlashCMD==0x04)
		{	
			PM1DO=ECINDDR;			// Read byte from spi			
		}
		else if(_FlashCMD==0x05)
		{	
			ECINDAR3 = 0x00;     	//Exit follow mode
			ECINDAR2 = 0x00;
		}
		else if(_FlashCMD==0xFC)
		{	
			ExitRamCodeCommand=0xAA;
			break;
		}
		else if(_FlashCMD==0xFD)
		{
            WDTCON |= 0x01;     /* WDTRST = 1 Reset watch dog timer.*/
            WDTCON |= 0x02;     /* WDTEN  = 1 Enable watch dog.     */
       		while(1);				// Wait for watch dog time-out				
		}
		else if(_FlashCMD==0xFE)
		{	
			//BRAM[63]=0x55;
            WDTCON |= 0x01;     /* WDTRST = 1 Reset watch dog timer.*/
            WDTCON |= 0x02;     /* WDTEN  = 1 Enable watch dog.     */
       		while(1);				// Wait for watch dog time-out
		}
 	}
	main();
}
#endif
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// The function of reading SPI status ( command 0x05)
//	After this function, spi status will save to SPIReadStatus
//-----------------------------------------------------------------------------
void SPI_Read_Status(void)
{
	ECINDAR3 = 0x0F; 
 	ECINDAR2 = 0xFF;
	ECINDAR0 = 0x00;   				// FFFFExx = 0xFF  

	ECINDAR1 = 0xFE;
	ECINDDR = 0xFF;					// SCE# high level
	ECINDAR1 = 0xFD;
	ECINDDR = SPICmd_ReadStatus;	// Read status command

	SPIReadStatus = ECINDDR;		// Save status Register to SPIReadStatus
	
	ECINDAR3 = 0x00;     			//Exit follow mode
	ECINDAR2 = 0x00;
}

//-----------------------------------------------------------------------------
// The function of Write SPI status ( command 0x01)
//	After this function, the value of SPIWriteStatus will write to spi status
//-----------------------------------------------------------------------------
void SPI_Write_Status(void)
{
	ECINDAR3 = 0x0F; 
 	ECINDAR2 = 0xFF;
	ECINDAR0 = 0x00;   				// FFFFExx = 0xFF  

	ECINDAR1 = 0xFE;
	ECINDDR = 0xFF;					// SCE# high level
	ECINDAR1 = 0xFD;
	ECINDDR = SPICmd_ReadStatus;	// Read Status Register
	while(1)						// waiting spi free
	{
		if((ECINDDR&SPIStatus_BUSY)==0x00)
        {
            break;
        } 
	}								

	ECINDAR1 = 0xFE;
	ECINDDR = 0xFF;					// SCE# high level
	ECINDAR1 = 0xFD;
	ECINDDR = SPICmd_WREN;			// Write enable	
	
    if(SPIID==SSTID)
	{
		ECINDAR1 = 0xFE;
		ECINDDR = 0xFF;				// SCE# high level
		ECINDAR1 = 0xFD;
		ECINDDR = SPICmd_EWSR;		// Enable Write Status Register
	}    
	
	ECINDAR1 = 0xFE;
	ECINDDR = 0xFF;					// SCE# high level
	ECINDAR1 = 0xFD;
	ECINDDR = SPICmd_WRSR;			// Write status command
	ECINDDR = SPIWriteStatus;		// Write SPIWriteStatus to spi status register

	ECINDAR1 = 0xFE;
	ECINDDR = 0xFF;					// SCE# high level
	ECINDAR1 = 0xFD;
	ECINDDR = SPICmd_ReadStatus;	// Read Status Register
	while(1)						// waiting spi free
	{
		if((ECINDDR&SPIStatus_BUSY)==0x00)
        {
            break;
        } 
	}
	
	ECINDAR3 = 0x00;     			//Exit follow mode
	ECINDAR2 = 0x00;
}

//-----------------------------------------------------------------------------
// The function of reading SPI ID (command 0x9F)
//	After this function, spi id will save to array SPIIDBuf[] 
//-----------------------------------------------------------------------------
void SPI_Read_ID(void)
{
	ECINDAR3 = 0x0F; 
 	ECINDAR2 = 0xFF;
	ECINDAR0 = 0x00;   				// FFFFExx = 0xFF  
	
	ECINDAR1 = 0xFE;
	ECINDDR = 0xFF;					// SCE# high level
	ECINDAR1 = 0xFD;
	ECINDDR = SPICmd_ReadStatus;	// Read Status Register
	while(1)						// waiting spi free
	{
		if((ECINDDR&SPIStatus_BUSY)==0x00)
        {
            break;
        } 
	}								

	ECINDAR1 = 0xFE;
	ECINDDR = 0xFF;					// SCE# high level
	ECINDAR1 = 0xFD;
	ECINDDR = SPICmd_DeviceID;  // Manufacture ID command
	for(SPIIndex=0x00;SPIIndex<4;SPIIndex++)
	{
		SPIIDBuf[SPIIndex] = ECINDDR;
	}
	SPIID = SPIIDBuf[0];	

	ECINDAR1 = 0xFE;
	ECINDDR = 0xFF;					// SCE# high level
	ECINDAR1 = 0xFD;
	ECINDDR = SPICmd_ReadStatus;	// Read Status Register

	while(1)						// waiting spi free
	{
		if((ECINDDR&SPIStatus_BUSY)==0x00)
        {
            break;
        } 
	}
	ECINDAR3 = 0x00;     	//Exit follow mode
	ECINDAR2 = 0x00;
}

//-----------------------------------------------------------------------------
// The function of reading SPI ID (command 0xAB)
//	After this function, spi id will save to array SPIIDBuf[] 
//-----------------------------------------------------------------------------
void SPI_Read_ID_CmdAB(void)
{
	ECINDAR3 = 0x0F; 
 	ECINDAR2 = 0xFF;
	ECINDAR0 = 0x00;   				// FFFFExx = 0xFF  
	
	ECINDAR1 = 0xFE;
	ECINDDR = 0xFF;					// SCE# high level
	ECINDAR1 = 0xFD;
	ECINDDR = SPICmd_ReadStatus;	// Read Status Register
	while(1)						// waiting spi free
	{
		if((ECINDDR&SPIStatus_BUSY)==0x00)
        {
            break;
        } 
	}								

	ECINDAR1 = 0xFE;
	ECINDDR = 0xFF;					// SCE# high level
	ECINDAR1 = 0xFD;
	ECINDDR = SPICmd_RDID;		    // Read ID command
    ECINDDR = 0x00;		            // Read ID command addr2
    ECINDDR = 0x00;		            // Read ID command addr1
    ECINDDR = 0x00;		            // Read ID command addr0
    
	for(SPIIndex=0x00;SPIIndex<4;SPIIndex++)
	{
		SPIIDBuf[SPIIndex] = ECINDDR; 
	}
	SPIID = SPIIDBuf[0];	

	ECINDAR1 = 0xFE;
	ECINDDR = 0xFF;					// SCE# high level
	ECINDAR1 = 0xFD;
	ECINDDR = SPICmd_ReadStatus;	// Read Status Register

	while(1)						// waiting spi free
	{
		if((ECINDDR&SPIStatus_BUSY)==0x00)
        {
            break;
        } 
	}
	ECINDAR3 = 0x00;     	//Exit follow mode
	ECINDAR2 = 0x00;
}

//-----------------------------------------------------------------------------
// The function of Erasing SPI
//-----------------------------------------------------------------------------
void SPI_Erase(void)
{
	ECINDAR3 = 0x0F; 
 	ECINDAR2 = 0xFF;
	ECINDAR0 = 0x00;   				// FFFFExx = 0xFF  
	
	ECINDAR1 = 0xFE;
	ECINDDR = 0xFF;					// SCE# high level
	ECINDAR1 = 0xFD;
	ECINDDR = SPICmd_ReadStatus;	// Read Status Register
	while(1)						// waiting spi free
	{
		if((ECINDDR&(SPIStatus_BUSY+SPIStatus_WEL))==SPIStatus_WriteEnable)
        {
            break;
        } 
	}								

	ECINDAR1 = 0xFE;
	ECINDDR = 0xFF;					// SCE# high level
	ECINDAR1 = 0xFD;
	ECINDDR = SPIAEraseCmd;			// Send erase command
	for(SPIIndex=0x00;SPIIndex<3;SPIIndex++)
	{
		ECINDDR = SPIAddrCycle[SPIIndex];
	}								// Send address cycle

	ECINDAR1 = 0xFE;
	ECINDDR = 0xFF;					// SCE# high level

	ECINDAR1 = 0xFD;
	ECINDDR = SPICmd_ReadStatus;	// Read Status Register

	while(1)						// waiting spi free
	{
		if((ECINDDR&SPIStatus_BUSY)==0x00)
        {
            break;
        } 
	}
	ECINDAR3 = 0x00;     			//Exit follow mode
	ECINDAR2 = 0x00;
}

//-----------------------------------------------------------------------------
// The function of SPI write enable
//-----------------------------------------------------------------------------
void SPI_Write_Enable(void)
{
	ECINDAR3 = 0x0F; 
 	ECINDAR2 = 0xFF;
	ECINDAR0 = 0x00;   				// FFFFExx = 0xFF  
	
	ECINDAR1 = 0xFE;
	ECINDDR = 0xFF;					// SCE# high level
	ECINDAR1 = 0xFD;
	ECINDDR = SPICmd_ReadStatus;	// Read Status Register
	while(1)						// waiting spi free
	{
		if((ECINDDR&SPIStatus_BUSY)==0x00)
        {
            break;
        } 
	}								

	ECINDAR1 = 0xFE;
	ECINDDR = 0xFF;					// SCE# high level
	ECINDAR1 = 0xFD;
	ECINDDR = SPICmd_WREN;			// Write enable	
	
    if(SPIID==SSTID)
    {
        ECINDAR1 = 0xFE;
        ECINDDR = 0xFF;				// SCE# high level
        ECINDAR1 = 0xFD;
        ECINDDR = SPICmd_EWSR;		// Enable Write Status Register
    }
    
	ECINDAR1 = 0xFE;
	ECINDDR = 0xFF;					// SCE# high level
	ECINDAR1 = 0xFD;
	ECINDDR = SPICmd_ReadStatus;	// Read Status Register
	while(1)						// waiting spi free
	{
		if((ECINDDR&(SPIStatus_WEL+SPIStatus_BUSY))==0x02) // Check write enable and spi not busy
        {
            break;
        } 
	}

	ECINDAR3 = 0x00;     			//Exit follow mode
	ECINDAR2 = 0x00;
}

//-----------------------------------------------------------------------------
// The function of SPI write disable
//-----------------------------------------------------------------------------
void SPI_Write_Disable(void)
{
	ECINDAR3 = 0x0F; 
 	ECINDAR2 = 0xFF;
	ECINDAR0 = 0x00;   				// FFFFExx = 0xFF  
	
	ECINDAR1 = 0xFE;
	ECINDDR = 0xFF;					// SCE# high level
	ECINDAR1 = 0xFD;
	ECINDDR = SPICmd_ReadStatus;	// Read Status Register
	while(1)						// waiting spi free
	{
		if((ECINDDR&SPIStatus_BUSY)==0x00)
        {
            break;
        } 
	}								

	ECINDAR1 = 0xFE;
	ECINDDR = 0xFF;					// SCE# high level
	ECINDAR1 = 0xFD;
	ECINDDR = SPICmd_WRDI;			// Write disable	

	ECINDAR1 = 0xFE;
	ECINDDR = 0xFF;					// SCE# high level
	ECINDAR1 = 0xFD;
	ECINDDR = SPICmd_ReadStatus;	// Read Status Register
	while(1)						// waiting spi free
	{
		if((ECINDDR&(SPIStatus_WEL+SPIStatus_BUSY))==0x00) // Check write disable and spi not busy
        {
            break;
        } 
	}

	ECINDAR3 = 0x00;     			//Exit follow mode
	ECINDAR2 = 0x00;
}

//-----------------------------------------------------------------------------
// The function of other SPI write 256 bytes
//-----------------------------------------------------------------------------
void Other_SPI_Write_256Bytes(void)
{
	ECINDAR3 = 0x0F; 
 	ECINDAR2 = 0xFF;
	ECINDAR0 = 0x00;   				// FFFFExx = 0xFF  
	
	ECINDAR1 = 0xFE;
	ECINDDR = 0xFF;					// SCE# high level
	ECINDAR1 = 0xFD;
	ECINDDR = SPICmd_ReadStatus;	// Read Status Register
	while(1)						// waiting spi free
	{
		if((ECINDDR&SPIStatus_BUSY)==0x00)
        {
            break;
        } 
	}								

	ECINDAR1 = 0xFE;
	ECINDDR = 0xFF;				    // SCE# high level
	ECINDAR1 = 0xFD;
    ECINDDR = SPICmd_BYTEProgram;   // Send Program One Data Byte command

	ECINDDR = SPIAddrCycle[0];	    // For 256 bytes function limite
	ECINDDR = SPIAddrCycle[1];
	ECINDDR = SPIAddrCycle[2];
	
	SPIIndex = 0x00;
	do
	{
		ECINDDR = *SPIDataPointer;
		SPIIndex++;
		SPIDataPointerLowByte++;
	}while(SPIIndex!=0x00);		    // Send 256 bytes

	ECINDAR1 = 0xFE;
	ECINDDR = 0xFF;					// SCE# high level
	ECINDAR1 = 0xFD;
	ECINDDR = SPICmd_ReadStatus;	// Read Status Register
	while(1)						// waiting spi free
	{
		if((ECINDDR&SPIStatus_BUSY)==0x00)
        {
            break;
        } 
	}	

	ECINDAR3 = 0x00;     			// Exit follow mode
	ECINDAR2 = 0x00;				// For 256 bytes function limite
}

//-----------------------------------------------------------------------------
// The function of SST SPI write 256 bytes (for Word AAI command)
//-----------------------------------------------------------------------------
void SST_SPI_Write_256Bytes(void)
{
	ECINDAR3 = 0x0F; 
 	ECINDAR2 = 0xFF;
	ECINDAR0 = 0x00;   				// FFFFExx = 0xFF  
	
	ECINDAR1 = 0xFE;
	ECINDDR = 0xFF;					// SCE# high level
	ECINDAR1 = 0xFD;
	ECINDDR = SPICmd_ReadStatus;	// Read Status Register
	while(1)						// waiting spi free
	{
		if((ECINDDR&SPIStatus_BUSY)==0x00)
        {
            break;
        } 
	}								

	ECINDAR1 = 0xFE;
	ECINDDR = 0xFF;				    // SCE# high level
	ECINDAR1 = 0xFD;
    ECINDDR = SPICmd_AAIWordProgram;    // Auto Address Increment Programming

	ECINDDR = SPIAddrCycle[0];	    // For 256 bytes function limite
	ECINDDR = SPIAddrCycle[1];
	ECINDDR = SPIAddrCycle[2];
	
	SPIIndex = 0x00;
	do
	{ 
		ECINDDR = *SPIDataPointer;
		SPIIndex++;
		SPIDataPointerLowByte++;

	    if((SPIIndex%2)==0x00)
	    {
		    ECINDAR1 = 0xFE;
		    ECINDDR = 0xFF;					// SCE# high level
		    ECINDAR1 = 0xFD;
		    ECINDDR = SPICmd_ReadStatus;	// Read Status Register
		    while(1)						// waiting spi free
		    {
			    if((ECINDDR&SPIStatus_BUSY)==0x00)
        	    {
            	    break;
                } 
		    }	
                
            if(SPIIndex!=0x00)
            {
		        ECINDAR1 = 0xFE;
		        ECINDDR = 0xFF;						    // SCE# high level
		        ECINDAR1 = 0xFD;
		        ECINDDR = SPICmd_AAIWordProgram;		// Auto Address Increment Programming
            }
	    }
	}while(SPIIndex!=0x00);					// Send 256 bytes

	ECINDAR1 = 0xFE;
	ECINDDR = 0xFF;				    // SCE# high level
	ECINDAR1 = 0xFD;
    ECINDDR = SPICmd_WRDI;          // Write disable

    WNCKR = 0x00;                   // Delay 15.26 us
    WNCKR = 0x00;                   // Delay 15.26 us
    WNCKR = 0x00;                   // Delay 15.26 us
    WNCKR = 0x00;                   // Delay 15.26 us

	ECINDAR3 = 0x00;     			// Exit follow mode
	ECINDAR2 = 0x00;				// For 256 bytes function limite
}

//-----------------------------------------------------------------------------
// The function of old SST SPI write 256 bytes
//-----------------------------------------------------------------------------
void Old_SST_SPI_Write_256Bytes(void)
{
	ECINDAR3 = 0x0F; 
 	ECINDAR2 = 0xFF;
	ECINDAR0 = 0x00;   				// FFFFExx = 0xFF  
	
	ECINDAR1 = 0xFE;
	ECINDDR = 0xFF;					// SCE# high level
	ECINDAR1 = 0xFD;
	ECINDDR = SPICmd_ReadStatus;	// Read Status Register
	while(1)						// waiting spi free
	{
		if((ECINDDR&SPIStatus_BUSY)==0x00)
        {
            break;
        } 
	}								

	ECINDAR1 = 0xFE;
	ECINDDR = 0xFF;				    // SCE# high level
	ECINDAR1 = 0xFD;
    ECINDDR = SPICmd_AAIProgram;    // Auto Address Increment Programming

	ECINDDR = SPIAddrCycle[0];	    // For 256 bytes function limite
	ECINDDR = SPIAddrCycle[1];
	ECINDDR = SPIAddrCycle[2];
	
	SPIIndex = 0x00;
	do
	{
		ECINDDR = *SPIDataPointer;
		SPIIndex++;
		SPIDataPointerLowByte++;

	    ECINDAR1 = 0xFE;
	    ECINDDR = 0xFF;					// SCE# high level
	    ECINDAR1 = 0xFD;
	    ECINDDR = SPICmd_ReadStatus;	// Read Status Register
	    while(1)						// waiting spi free
	    {
		    if((ECINDDR&SPIStatus_BUSY)==0x00)
            {
                break;
            } 
	    }	

        if(SPIIndex!=0x00)
        {
	        ECINDAR1 = 0xFE;
	        ECINDDR = 0xFF;			    // SCE# high level
	        ECINDAR1 = 0xFD;       
            ECINDDR = SPICmd_AAIProgram;    // Auto Address Increment Programming
        }
	}while(SPIIndex!=0x00);		    // Send 256 bytes

	ECINDAR1 = 0xFE;
	ECINDDR = 0xFF;				    // SCE# high level
	ECINDAR1 = 0xFD;
    ECINDDR = SPICmd_WRDI;          // Auto Address Increment Programming

	ECINDAR1 = 0xFE;
	ECINDDR = 0xFF;					// SCE# high level
	ECINDAR1 = 0xFD;
	ECINDDR = SPICmd_ReadStatus;	// Read Status Register
	while(1)						// waiting spi free
	{
		if((ECINDDR&(SPIStatus_BUSY+SSTSPIStatus_AAI))==0x00)
        {
            break;
        } 
	}	

	ECINDAR3 = 0x00;     			// Exit follow mode
	ECINDAR2 = 0x00;				// For 256 bytes function limite
}

//-----------------------------------------------------------------------------
// The function of SPI write 256 bytes
//-----------------------------------------------------------------------------
void SPI_Write_256Bytes(void)
{
    if(SPIID==SSTID)
    {
        if(SSTDeviceID==SSTID_Old)
        {
            LoadSPIFucnToRam(Old_SST_SPI_Write_256Bytes);   // Load function to ram
        }
        else
        {
	        LoadSPIFucnToRam(SST_SPI_Write_256Bytes);	// Load function to ram
        }
    }
    else
    {
	    LoadSPIFucnToRam(Other_SPI_Write_256Bytes);	// Load function to ram
    }  
}


//-----------------------------------------------------------------------------
// The function of SPI read 256 bytes
//-----------------------------------------------------------------------------
void SPI_Read_256Bytes(void)
{
	ECINDAR3 = 0x0F; 
 	ECINDAR2 = 0xFF;
	ECINDAR0 = 0x00;   				// FFFFExx = 0xFF  
	
	ECINDAR1 = 0xFE;
	ECINDDR = 0xFF;					// SCE# high level
	ECINDAR1 = 0xFD;
	ECINDDR = SPICmd_ReadStatus;	// Read Status Register
	while(1)						// waiting spi free
	{
		if((ECINDDR&SPIStatus_BUSY)==0x00)
        {
            break;
        } 
	}								

	ECINDAR1 = 0xFE;
	ECINDDR = 0xFF;						// SCE# high level
	ECINDAR1 = 0xFD;
	ECINDDR = SPICmd_HighSpeedRead;		// High-Speed Read

	ECINDDR = SPIAddrCycle[0];			// For 256 bytes function limite
	ECINDDR = SPIAddrCycle[1];
	ECINDDR = SPIAddrCycle[2];
	ECINDDR = 0x00;						// dummy byte
	
	SPIIndex = 0x00;
	do
	{
		*SPIDataPointer = ECINDDR;;
		SPIIndex++;
		SPIDataPointerLowByte++;
	}while(SPIIndex!=0x00);					// Send 256 bytes

	ECINDAR1 = 0xFE;
	ECINDDR = 0xFF;					// SCE# high level
	ECINDAR1 = 0xFD;
	ECINDDR = SPICmd_ReadStatus;	// Read Status Register
	while(1)						// waiting spi free
	{
		if((ECINDDR&SPIStatus_BUSY)==0x00)
        {
            break;
        } 
	}	

	ECINDAR3 = 0x00;     			// Exit follow mode
	ECINDAR2 = 0x00;				// For 256 bytes function limite
}

//-----------------------------------------------------------------------------
// The function of loading function to external ram 0x600 ~ 0x6FF
//-----------------------------------------------------------------------------
void LoadSPIFucnToRam(FUNCT_PTR_V_V funcpoint)
{
	Tmp_XPntr = 0x600;
	Tmp_code_pointer = funcpoint;

	_R1 = 0x00;
	do
	{
		*Tmp_XPntr = *Tmp_code_pointer;
      	Tmp_XPntr++;
   		Tmp_code_pointer++;
		_R1++;
	}while(_R1!=0x00);
}

//-----------------------------------------------------------------------------
// The function for ITE flash utility
//-----------------------------------------------------------------------------
#pragma ot(7, SIZE)
void ITE_Flash_Utility(void)
{
	DisableAllInterrupt();				// Disable all interrupt 
	LoadSPIFucnToRam(FlashECCode);		// Load function to ram
	SET_MASK(FBCFG,SSMC);       		// enable scatch ROM
	FuncAt_0xFE00();					// do function in extern ram 0x600
}

//*****************************************************************************
// OEM FUNCTION CALL
//*****************************************************************************
//-----------------------------------------------------------------------------
// The function for oem code write spi status
//-----------------------------------------------------------------------------
#pragma ot(7, SIZE)
void Do_SPI_Write_Status(BYTE statusvalue)
{
	SPIWriteStatus = statusvalue;
	
	DisableAllInterrupt();				// Disable all interrupt 
	LoadSPIFucnToRam(SPI_Write_Status);	// Load function to ram
	SET_MASK(FBCFG,SSMC);       		// enable scatch ROM
	FuncAt_0xFE00();					// do function in extern ram 0x600
	EnableAllInterrupt();				// Enable all interrupt 
	CLEAR_MASK(FBCFG,SSMC);       		// disable scatch ROM
    HSPI_SetReloadRamCodeFlag();        // Set flag for re-load HSPI ram code function
}

//-----------------------------------------------------------------------------
// The function for oem code read spi status
//		Note : SPI status always save to SPIReadStatus
//-----------------------------------------------------------------------------
#pragma ot(7, SIZE)
void Do_SPI_Read_Status(void)
{
	DisableAllInterrupt();				// Disable all interrupt 
	LoadSPIFucnToRam(SPI_Read_Status);	// Load function to ram
	SET_MASK(FBCFG,SSMC);       		// enable scatch ROM
	FuncAt_0xFE00();					// do function in extern ram 0x600
	EnableAllInterrupt();				// Enable all interrupt 
	CLEAR_MASK(FBCFG,SSMC);       		// disable scatch ROM
	HSPI_SetReloadRamCodeFlag();        // Set flag for re-load HSPI ram code function
}

//-----------------------------------------------------------------------------
// The function for oem code read spi ID (command 0x9F)   
//		Note : SPI ID always save to array SPIIDBuf[]
//-----------------------------------------------------------------------------
#pragma ot(7, SIZE)
void Do_SPI_Read_ID(void)
{
	DisableAllInterrupt();			// Disable all interrupt 
	LoadSPIFucnToRam(SPI_Read_ID);	// Load function to ram
	SET_MASK(FBCFG,SSMC);       	// enable scatch ROM
	FuncAt_0xFE00();				// do function in extern ram 0x600
	EnableAllInterrupt();		    // Enable all interrupt 
	CLEAR_MASK(FBCFG,SSMC);         // disable scatch ROM
	HSPI_SetReloadRamCodeFlag();    // Set flag for re-load HSPI ram code function
}

//-----------------------------------------------------------------------------
// The function for oem code read spi ID (command 0xAB)
//		Note : SPI ID always save to array SPIIDBuf[]
//-----------------------------------------------------------------------------
#pragma ot(7, SIZE)
void Do_SPI_Read_ID_CmdAB(void)
{
	DisableAllInterrupt();			// Disable all interrupt 
	LoadSPIFucnToRam(SPI_Read_ID_CmdAB);	// Load function to ram
	SET_MASK(FBCFG,SSMC);       	// enable scatch ROM
	FuncAt_0xFE00();				// do function in extern ram 0x600
	EnableAllInterrupt();		    // Enable all interrupt 
	CLEAR_MASK(FBCFG,SSMC);         // disable scatch ROM
	HSPI_SetReloadRamCodeFlag();    // Set flag for re-load HSPI ram code function
}

//-----------------------------------------------------------------------------
// The function for oem code erase spi
//		Input : 1 EraseCmd --> Only support SPICmd_Erase4KByte, 
//								SPICmd_Erase32KByte, and SPICmd_Erase64KByte.
//				2 Addr2	   --> A23 ~ A16
//				3 Addr1	   --> A15 ~ A8
//				4 Addr0	   --> A7  ~ A0
//-----------------------------------------------------------------------------
#pragma ot(7, SIZE)
void Do_SPI_Erase(BYTE EraseCmd, BYTE Addr2, BYTE Addr1, BYTE Addr0)
{
	SPIAEraseCmd = EraseCmd;
	SPIAddrCycle[0] = Addr2;
	SPIAddrCycle[1] = Addr1;
	SPIAddrCycle[2] = Addr0;
			
	DisableAllInterrupt();			// Disable all interrupt 
	LoadSPIFucnToRam(SPI_Erase);	// Load function to ram
	SET_MASK(FBCFG,SSMC);       	// enable scatch ROM
	FuncAt_0xFE00();				// do function in extern ram 0x600
	EnableAllInterrupt();		    // Enable all interrupt 
	CLEAR_MASK(FBCFG,SSMC);         // disable scatch ROM
	HSPI_SetReloadRamCodeFlag();    // Set flag for re-load HSPI ram code function
}

//-----------------------------------------------------------------------------
// The function for oem code write spi 256 bytes
//		Input : 1 DataPointer --> Pointe to external memory
//				2 Addr2	   --> A23 ~ A16
//				3 Addr1	   --> A15 ~ A8
//				4 Addr0	   --> A7  ~ A0
//-----------------------------------------------------------------------------
#pragma ot(7, SIZE)
void Do_SPI_Write_256Bytes(XBYTE *DataPointer, BYTE Addr2, BYTE Addr1, BYTE Addr0)
{
	SPIDataPointer = DataPointer;
	SPIAddrCycle[0] = Addr2;
	SPIAddrCycle[1] = Addr1;
	SPIAddrCycle[2] = Addr0;
			
	DisableAllInterrupt();		    // Disable all interrupt 
	SPI_Write_256Bytes();           // Load function to ram
	SET_MASK(FBCFG,SSMC);           // enable scatch ROM
	FuncAt_0xFE00();			    // do function in extern ram 0x600
	EnableAllInterrupt();		    // Enable all interrupt 
	CLEAR_MASK(FBCFG,SSMC);         // disable scatch ROM
	HSPI_SetReloadRamCodeFlag();    // Set flag for re-load HSPI ram code function
}

//-----------------------------------------------------------------------------
// The function for oem code read spi 256 bytes to external memory
//		Input : 1 DataPointer --> Pointe to external memory
//				2 Addr2	   --> A23 ~ A16
//				3 Addr1	   --> A15 ~ A8
//				4 Addr0	   --> A7  ~ A0
//-----------------------------------------------------------------------------
#pragma ot(7, SIZE)
void Do_SPI_Read_256Bytes(XBYTE *DataPointer, BYTE Addr2, BYTE Addr1, BYTE Addr0)
{
	SPIDataPointer = DataPointer;
	SPIAddrCycle[0] = Addr2;
	SPIAddrCycle[1] = Addr1;
	SPIAddrCycle[2] = Addr0;
			
	DisableAllInterrupt();				// Disable all interrupt 
	LoadSPIFucnToRam(SPI_Read_256Bytes);	// Load function to ram
	SET_MASK(FBCFG,SSMC);       		// enable scatch ROM
	FuncAt_0xFE00();					// do function in extern ram 0x600
	EnableAllInterrupt();				// Enable all interrupt 
	CLEAR_MASK(FBCFG,SSMC);       		// disable scatch ROM
	HSPI_SetReloadRamCodeFlag();        // Set flag for re-load HSPI ram code function
}

//-----------------------------------------------------------------------------
// The function for oem code spi write enable
//-----------------------------------------------------------------------------
#pragma ot(7, SIZE)
void Do_SPI_Write_Enable(void)
{
	DisableAllInterrupt();				// Disable all interrupt 
	LoadSPIFucnToRam(SPI_Write_Enable);	// Load function to ram
	SET_MASK(FBCFG,SSMC);       		// enable scatch ROM
	FuncAt_0xFE00();					// do function in extern ram 0x600
	EnableAllInterrupt();				// Enable all interrupt 
	CLEAR_MASK(FBCFG,SSMC);       		// disable scatch ROM
	HSPI_SetReloadRamCodeFlag();        // Set flag for re-load HSPI ram code function
}

//-----------------------------------------------------------------------------
// The function for oem code spi write disable
//-----------------------------------------------------------------------------
#pragma ot(7, SIZE)
void Do_SPI_Write_Disable(void)
{
	DisableAllInterrupt();				// Disable all interrupt 
	LoadSPIFucnToRam(SPI_Write_Disable);	// Load function to ram
	SET_MASK(FBCFG,SSMC);       		// enable scatch ROM
	FuncAt_0xFE00();					// do function in extern ram 0x600
	EnableAllInterrupt();				// Enable all interrupt 
	CLEAR_MASK(FBCFG,SSMC);       		// disable scatch ROM
	HSPI_SetReloadRamCodeFlag();        // Set flag for re-load HSPI ram code function
}


//----------------------------------------------------------------------------
// SPI read & write example
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
// The example of SPI protect all
//----------------------------------------------------------------------------
void OEM_SPIProtectAll(void)
{
	Do_SPI_Write_Status(SPIStatus_ProtectAll);
	Do_SPI_Write_Disable();
}

//----------------------------------------------------------------------------
// The example of writing external ram 0x300 ~ 0x3FF to SPI ROM 0x010000 ~ 0x0100FF
//----------------------------------------------------------------------------
void OEM_Write_SPI_256Bytes(void)
{
	Do_SPI_Write_Status(SPIStatus_UnlockAll);
	Do_SPI_Write_Enable();
	Do_SPI_Erase(SPICmd_Erase4KByte,0x00,0xF0,0x00);
	Do_SPI_Write_Enable();
	Do_SPI_Write_256Bytes(0x300,0x00,0xF0,0x00);
	Do_SPI_Write_Disable();
}

//----------------------------------------------------------------------------
// The example of reading SPI ROM 0x010000 ~ 0x0100FF to external ram 0x800 ~ 0x8FF
//----------------------------------------------------------------------------
void OEM_Read_SPI_256Bytes(void)
{
	Do_SPI_Read_256Bytes(0x800,0x01,0x00,0x00);
}

//-----------------------------------------------------------------------------
// The example of reading SPI ID
//		Note : SPI ID always save to array SPIIDBuf[]
//-----------------------------------------------------------------------------
void OEM_SPI_Read_ID(void)
{
    SPIID = 0x00;
	Do_SPI_Read_ID();
    if(SPIID==0x00)                     
    {
        Do_SPI_Read_ID_CmdAB();
        if(SPIID==SSTID)
        {
            SSTDeviceID = SSTID_Old;
        }
    }
}

//----------------------------------------------------------------------------
// The example of SPI read status
//----------------------------------------------------------------------------
BYTE OEM_SPIReadStatus(void)
{
	Do_SPI_Read_Status();
	return(SPIReadStatus);
}
/*-----------------------------------------------------------------------------
 * End
 *---------------------------------------------------------------------------*/
