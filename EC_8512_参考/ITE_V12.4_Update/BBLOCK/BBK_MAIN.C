/*-----------------------------------------------------------------------------
 * Filename: BBK_MAIN.C
 *
 * Function: Non-update for Firmware Recovery (Boot-block for crisis)
 *
 * Copyright (c) 2006-2009, ITE Tech. Inc. All Rights Reserved.
 *---------------------------------------------------------------------------*/
/* ----------------------------------------------------------------------------
 * >>>----> ITE Technical Marketing Firmware Center <-----------------------<<<
 *
 *          EC Kernel Code Revision 1.1     Date: July 01, 2009 12:00:00
 * ------------------------------------------------------------------------- */


//-----------------------------------------------------------------------------
// Common Header Include
//-----------------------------------------------------------------------------
#include    "BBK_COMM.H"

//-----------------------------------------------------------------------------
// Externel Link Header
//-----------------------------------------------------------------------------
#include    "BBK_OEM.H"

//=============================================================================
// Local Header
//=============================================================================
//#include    "BBK_MAIN.H"->("BBK_COMM.H")


/* ************************************************************************* */
/* CODE START                                                                */
/* ************************************************************************* */

void BBK_DisableAllInterrupt(void)
{
  	EA = 0;
	_nop_();
	_nop_();
	_nop_();
	_nop_();
}

//-----------------------------------------------------------------------------
// The Boot-block main function
// 4K Bytes Sizes and running after chip power reset.
//-----------------------------------------------------------------------------
void BB_Main(void)
{
    SP = 0xD0;
    BBK_DisableAllInterrupt();
    BB_Timers_Init();
    BB_OemInit();

    while(1)
    {
        BB_Delay1MS(5);     // Delay 5ms
        BB_SysOnSequence();

        if( CtrlPowStep == 0xFF )
        {
             break;
        }
    }

    SET_BIT(KBHISR,4);
    CLEAR_BIT(KBHISR,5);

    while(1)
    {
        TH1=(T_Timer_5ms)>>8;
        TL1=(T_Timer_5ms);
        TR1=1;
        TF1=0;

        while (!TF1);
        {
            BB_Check64PortCmd();

            #if BB_Service_EC
            BB_Check66PortCmd();
            #endif

            #if BB_Service_PM2
            BB_Check6CPortCmd();
            #endif

        }

        BB_OemService();
        TF1=0;
        TR1=0;

    }
}

//----------------------------------------------------------------------------
// Delay 1ms function use timer 1
//----------------------------------------------------------------------------
void BB_Delay1MS(BYTE bMS)
{
    for (;bMS!=0;bMS--)
    {
        TH1=(T_Timer_1ms)>>8;
        TL1=(T_Timer_1ms);
        TR1=1;
        TF1=0;
        while (!TF1);
        TF1=0;
        TR1=0;

    }

}

//-----------------------------------------------------------------------------
// Timer Initialize
//-----------------------------------------------------------------------------
void BB_Timers_Init(void)
{
    TMOD  = 0x11;
    TF0 = TR0 = 0;  // Stop timer 0 and clear interrupt
    TF1 = TR1 = 0;  // Stop timer 1 and clear interrupt

}

//-----------------------------------------------------------------------------
// Boot-Block Init LPC I/O Function Register Table.
//-----------------------------------------------------------------------------
const BYTE code BBsio_table[]=
{
    // Configure and Enable Logical Device 06h(KBD)

    0x07 ,0x06,     // Select Logical Device 06h(KBD)
    0x70 ,0x01,     // Set IRQ=01h for Logical Device 06h(KBD)
    0x30 ,0x01,     // Enable Logical Device 06h(Mouse)

    0x07 ,0x11,     // Select Logical Device 11h(PM1)
    0x70 ,0x00,     // Clear IRQ=0 for  Logical Device 11h(PM1)
    0x30 ,0x01,     // Enable Logical Device 11h(PM1)

    0x07 ,0x12,     // Select Logical Device 12h(PM2)
    0x70 ,0x00,     // Clear IRQ=0 for  Logical Device 12h(PM2)
    0x30 ,0x01,     // Enable Logical Device 12h(PM2)
    /* Enable Logical Device 0Fh(Shared Memory) */
    0x07 ,0x0F,     // Logical Device 0Fh(Shared Memory)
    0x30 ,0x01,     // MSWC Shared Memory
    0xF4 ,0x09,     // MSWC Shared Memory
};

//-----------------------------------------------------------------------------
// Init super IO function
//-----------------------------------------------------------------------------
void BB_InitSio(void)
{
    BYTE code * data_pntr;
    BYTE nIndex;

    SET_BIT(LSIOHA,0);
    SET_BIT(IBMAE ,0);
    SET_BIT(IBCTL,0);

    nIndex = 0;
    data_pntr=BBsio_table;
    while( nIndex < (sizeof(BBsio_table)/2) )
    {
        IHIOA=0;        // Set indirect Host I/O Address
        IHD=*data_pntr;
        while( IS_BIT_SET(IBCTL,2));
        data_pntr ++;

        IHIOA=1;        // Set indirect Host I/O Address
        IHD=*data_pntr;
        while( IS_BIT_SET(IBCTL,2));
        data_pntr ++;
        nIndex++;
    }

    CLEAR_BIT(LSIOHA,0);
    CLEAR_BIT(IBMAE ,0);
    CLEAR_BIT(IBCTL,0);

}

#if BB_RamCode_useKBC
//-----------------------------------------------------------------------------
// The function of flashing
//-----------------------------------------------------------------------------
void BB_RamCodeKBC(void)
{
    BYTE _Port64Cmd, SaveKBHICR;

    SaveKBHICR = KBHICR;
    KBHICR &= ~0x01;
    SET_BIT( SMECCS, 5 );   // Host write allow (Enabled)

    while(1)
    {
        if( IS_BIT_CLEAR(KBHISR,0) )
            break;
    }
    KBHIKDOR = 0xFA;    //Send to Host ACK.

    while(1)
    {
        if( IS_BIT_CLEAR(KBHISR,1) ) continue;

        if( IS_BIT_CLEAR(KBHISR,3) )
        {
            _Port64Cmd = KBHIDIR;   // Skip not command.
            continue;
        }

        _Port64Cmd = KBHIDIR;   // Received Host KBC Command
        /* RamCode_ExitCMD1,2 <------<< PROJECT.H */
        if ( (_Port64Cmd == RamCode_ExitCMD1) || (_Port64Cmd == RamCode_ExitCMD2) )
        {
            ExitRamCodeCommand = 0xAA;
            break;
        }
        else if ( _Port64Cmd == 0xFE )
        {
            ExitRamCodeCommand = 0xAA;
            break;
        }
        #if (RamCode_ResetCMD)
        else if ( _Port64Cmd == RamCode_ResetCMD )
        {
            WDTCON |= 0x01;     /* WDTRST = 1 Reset watch dog timer.*/
            WDTCON |= 0x02;     /* WDTEN  = 1 Enable watch dog.     */
            while(1);           /* Wait for watch dog time-out      */
            break;
        }
        #endif
        else if ( _Port64Cmd == 0xD7)
        {
            CLR_BIT( SMECCS, 5 );
            _nop_();
            _nop_();
            SET_BIT( SMECCS, 5 );
        }
        else if ( _Port64Cmd == 0xB5 )
        {
            while( IS_BIT_CLEAR(KBHISR,1) );
            _Port64Cmd = KBHIDIR;
            KBHIKDOR = SPIFlashData[_Port64Cmd];
        }
        else if ( _Port64Cmd == 0xB6 )
        {
            while( IS_BIT_CLEAR(KBHISR,1) );
            _Port64Cmd = KBHIDIR;
            while( IS_BIT_CLEAR(KBHISR,1) );
            SPIFlashData[_Port64Cmd] = KBHIDIR;
        }

    }

    CLEAR_BIT(SMECCS,5);    // Host write not allow (Disabled)
    KBHICR = SaveKBHICR;
    _nop_();
    _nop_();

}
#endif


#if BB_RamCode_useEC
//-----------------------------------------------------------------------------
// The function of flashing
//-----------------------------------------------------------------------------
void BB_RamCodeEC(void)
{
    BYTE _Port66Cmd, SaveKBHICR;

    SaveKBHICR = KBHICR;
    KBHICR &= ~0x01;
    SET_BIT( SMECCS, 5 );   // Host write allow (Enabled)

    while(1)
    {
        if( IS_BIT_CLEAR(PM1STS,0) )
            break;
    }

    PM1DO = 0xFA;       //Send to Host ACK.

    while(1)
    {
        if( IS_BIT_CLEAR(PM1STS,1) ) continue;

        if( IS_BIT_CLEAR(PM1STS,3) )
        {
            _Port66Cmd = PM1DI;   // Skip not command.
            continue;
        }

        _Port66Cmd = PM1DI;     // Received Host KBC Command
        /* RamCode_ExitCMD1,2 <------<< PROJECT.H */
        if ( (_Port66Cmd == RamCode_ExitCMD1) || (_Port66Cmd == RamCode_ExitCMD2) )
        {
            ExitRamCodeCommand = 0xAA;
            break;
        }
        else if ( _Port66Cmd == 0xFE )
        {
            ExitRamCodeCommand = 0xAA;
            break;
        }
        #if (RamCode_ResetCMD)
        else if ( _Port66Cmd == RamCode_ResetCMD )
        {
            WDTCON |= 0x01;     /* WDTRST = 1 Reset watch dog timer.*/
            WDTCON |= 0x02;     /* WDTEN  = 1 Enable watch dog.     */
            while(1);           /* Wait for watch dog time-out      */
            break;
        }
        #endif
        else if ( _Port66Cmd == 0xD7)
        {
            CLR_BIT( SMECCS, 5 );
            _nop_();
            _nop_();
            SET_BIT( SMECCS, 5 );
        }
        else if ( _Port66Cmd == 0xB5 )
        {
            while( IS_BIT_CLEAR(PM1STS,1) );
            _Port66Cmd = PM1DI;
            PM1DO = SPIFlashData[_Port66Cmd];
        }
        else if ( _Port66Cmd == 0xB6 )
        {
            while( IS_BIT_CLEAR(PM1STS,1) );
            _Port66Cmd = PM1DI;
            while( IS_BIT_CLEAR(PM1STS,1) );
            SPIFlashData[_Port66Cmd] = PM1DI;
        }

    }

    CLEAR_BIT( SMECCS, 5 );     // Host write not allow (Disabled)
    KBHICR = SaveKBHICR;
    _nop_();
    _nop_();
}
#endif

#if BB_RamCode_usePM2
//-----------------------------------------------------------------------------
// The function of flashing
//-----------------------------------------------------------------------------
void BB_RamCodePM2(void)
{
    BYTE _Port6CCmd, SaveKBHICR;

    SaveKBHICR = KBHICR;
    KBHICR &= ~0x01;
    SET_BIT( SMECCS, 5 );   // Host write allow (Enabled)

    while(1)
    {
        if( IS_BIT_CLEAR(PM2STS,0) )
            break;
    }

    PM2DO = 0xFA;       //Send to Host ACK.

    while(1)
    {
        if( IS_BIT_CLEAR(PM2STS,1) ) continue;

        if( IS_BIT_CLEAR(PM2STS,3) )
        {
            _Port6CCmd = PM2DI;   // Skip not command.
            continue;
        }

        _Port6CCmd = PM2DI;     // Received Host KBC Command
        /* RamCode_ExitCMD1,2 <------<< PROJECT.H */
        if ( (_Port6CCmd == RamCode_ExitCMD1) || (_Port6CCmd == RamCode_ExitCMD2) )
        {
            ExitRamCodeCommand = 0xAA;
            break;
        }
        else if ( _Port6CCmd == 0xFE )
        {
            ExitRamCodeCommand = 0xAA;
            break;
        }
        #if (RamCode_ResetCMD)
        else if ( _Port6CCmd == RamCode_ResetCMD )
        {
            WDTCON |= 0x01;     /* WDTRST = 1 Reset watch dog timer.*/
            WDTCON |= 0x02;     /* WDTEN  = 1 Enable watch dog.     */
            while(1);           /* Wait for watch dog time-out      */
            break;
        }
        #endif
        else if ( _Port6CCmd == 0xD7)
        {
            CLR_BIT( SMECCS, 5 );
            _nop_();
            _nop_();
            SET_BIT( SMECCS, 5 );
        }
        else if ( _Port6CCmd == 0xB5 )
        {
            while( IS_BIT_CLEAR(PM2STS,1) );
            _Port6CCmd = PM2DI;
            PM2DO = SPIFlashData[_Port6CCmd];
        }
        else if ( _Port6CCmd == 0xB6 )
        {
            while( IS_BIT_CLEAR(PM2STS,1) );
            _Port6CCmd = PM2DI;
            while( IS_BIT_CLEAR(PM2STS,1) );
            SPIFlashData[_Port6CCmd] = PM2DI;
        }

    }

    CLEAR_BIT( SMECCS, 5 );     // Host write not allow (Disabled)
    KBHICR = SaveKBHICR;
    _nop_();
    _nop_();
}
#endif

//-----------------------------------------------------------------------------
// The function of Scratch rom
//-----------------------------------------------------------------------------
void BB_RunCodeInRAM(void)
{
    PORT_BYTE_PNTR register_pntr;
    BYTE code * data_pntr;
    //BYTE iLoop;
    FUNCT_PTR_V_V code BB_FlashFunc = 0xFD00;

    SET_BIT(SMECCS,5);  // Host write allow

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

    register_pntr = 0x0500;

    #if BB_RamCode_usePM2
    data_pntr = (void *) BB_RamCodePM2;
    #endif

    #if BB_RamCode_useEC
    data_pntr = (void *) BB_RamCodeEC;
    #endif

    #if BB_RamCode_useKBC
    data_pntr = (void *) BB_RamCodeKBC;
    #endif

    for( _R1=0;_R1<255;_R1++ )
    {
        *register_pntr= *data_pntr;
        register_pntr +=1 ;
        data_pntr +=1;
    }
    DACDAT1  = 0xFF;    // Fan Fully On
    DACDAT2  = 0xFF;
    CLEAR_BIT(KBHICR,0);
    EA = 0;
    SET_BIT(FBCFG,7);   // enable scatch ROM
    BB_FlashFunc();
    CLEAR_BIT(FBCFG,7); // disable scatch ROM

}


//-----------------------------------------------------------------------------
// The boot block process 64 port command function
//-----------------------------------------------------------------------------
void BB_Check64PortCmd(void)
{
    if ( IS_BIT_CLEAR(KBHISR,1) ) return;

    if ( KBHISR & C_D )     // Bit3 - 1:Command 0:Data
    {
        KBHICmd  = KBHIDIR;
        KBHIStep = 0;

        #if En_RecordBB64
        BB_RamDebug(0x64);
        BB_RamDebug(KBHICmd);
        #endif

        switch (KBHICmd)
        {
            case 0x20:
                        KBHIKDOR = 0x03;
                        break;
            case 0xAA:
                        KBHIKDOR = 0x55;
                        break;
            case 0xD2:
                        KBHIStep = 1;
                        break;
            default:
                        break;
        }
        BB_OemCheckKBCmd();

    }
    else        //PMSTS.3=0
    {
        KBHIData  = KBHIDIR;

        #if En_RecordBB60
        BB_RamDebug(0x60);
        BB_RamDebug(KBHIData);
        #endif

        if ( KBHIStep )
        {
            switch (KBHICmd)
            {
                case 0xD2:
                            KBHIKDOR = KBHIData;
                            break;
                default:
                            break;
            }

            BB_OemProcessKBData();

        }
        else
        {
            KBHIKDOR = 0xFA;
        }
    }

}

//-----------------------------------------------------------------------------
// The boot block Send KB data to Host and wait received
//-----------------------------------------------------------------------------
void BB_KBDataToHostWaitRx(BYTE nDATA)
{
    WORD  iLOOP;

    KBHIKDOR = nDATA;

    iLOOP = WaitKBDataDelay;

    while (iLOOP != 0 )
    {
        iLOOP--;
        if ( IS_BIT_CLEAR(KBHISR,0)  || IS_BIT_SET(KBHISR,1)  )
        {
            iLOOP = 0;
        }

    }

}
//----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// The boot block process 68 port command function
//-----------------------------------------------------------------------------
void BB_Check66PortCmd(void)
{
    if ( IS_BIT_CLEAR(PM1STS,1) ) return;

    if ( PM1STS & BIT3)      // Bit3 - 1:Command 0:Data
    {
        PM1Cmd  = PM1DI;
        PM1Step = 0;

        #if En_RecordBB66
        BB_RamDebug(0x66);
        BB_RamDebug(PM1Cmd);
        #endif
        BB_OemCheckECCmd();

    }
    else    //PMSTS.3=0
    {
        PM1Data = PM1DI;

        #if En_RecordBB62
        BB_RamDebug(0x62);
        BB_RamDebug(PM1Data);
        #endif

        if (PM1Step)
        {
            BB_OemProcessECData();
        }
    }

}

//-----------------------------------------------------------------------------
// The boot block process 6C port command function
//-----------------------------------------------------------------------------
void BB_Check6CPortCmd(void)
{
    if ( IS_BIT_CLEAR(PM2STS,1) ) return;

    if ( PM2STS & BIT3 )     // Bit3 - 1:Command 0:Data
    {
        PM2Cmd  = PM2DI;
        PM2Step = 0;

        #if En_RecordBB6C
        BB_RamDebug(0x6C);
        BB_RamDebug(PM2Cmd);
        #endif
        BB_OemCheckPM2Cmd();

    }
    else    //PM2STS.3=0
    {
        PM2Data = PM2DI;

        #if En_RecordBB68
        BB_RamDebug(0x68);
        BB_RamDebug(PM2Data);
        #endif

        if (PM2Step)
        {
            BB_OemProcessPM2Data();
        }
    }


}

//-----------------------------------------------------------------------------
// Boot block Debug Recorder function
//-----------------------------------------------------------------------------
void BB_RamDebug(unsigned char dbgcode)
{
    #if SUPPORTED_BB_RECORDER
    PORT_BYTE_PNTR ClearIndex;
    PORT_BYTE_PNTR byte_register_pntr;
    BYTE index;
    BYTE iLoop;

    byte_register_pntr = BB_DebugRamAddr+BB_DebugRamRange;
    index = *byte_register_pntr;
    *byte_register_pntr +=1;

    if ( *byte_register_pntr == BB_DebugRamRange )
    {
        *byte_register_pntr = 0;
        ClearIndex= BB_DebugRamAddr;

        for ( iLoop=0x00; iLoop<BB_DebugRamRange; iLoop++ )
        {
            *ClearIndex=0x00;
            ClearIndex++;
        }
    }

    byte_register_pntr = BB_DebugRamAddr + index;
    *byte_register_pntr = dbgcode;

    #endif
}

//-----------------------------------------------------------------------------

/* ************************************************************************* */
// CODE END
