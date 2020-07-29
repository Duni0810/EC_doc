/*----------------------------------------------------------------------------
 * TITLE: CORE_ACPI.C - ACPI Handler
 * FUNCTION:
 * SCI event generation.
 * ACPI SMBus transfer initiated by ACPI command.
 *
 * Copyright(c)1983-2009, Insyde Software Corporation. All Rights Reserved.
 *
 * You may not reproduce,distribute,publish,display,perform,modify,adapt,
 * transmit,broadcast,present,recite,release,license or otherwise exploit
 * any part of this publication in any form,by any means,without the prior
 * written permission of Insyde Software Corporation.
 *---------------------------------------------------------------------------*/
#include "CORE_INCLUDE.H"
#include "OEM_INCLUDE.H"

/*-----------------------------------------------------------------------------
 * Local Parameter Definition
 *---------------------------------------------------------------------------*/
#define T_Burst_Loop        250

//-----------------------------------------------------------------------------
// ACPI Spec.13 ACPI Embedded Controller Interface Specification
// Embedded Controller Command
// Read Embedded Controller (RD_EC) 0x80
// Write Embedded Controller (WR_EC) 0x81
// Burst Enable Embedded Controller (BE_EC) 0x82
// Burst Disable Embedded Controller (BD_EC) 0x83
// Query Embedded Controller (QR_EC) 0x84
// Process Command/Data received from System via the 2nd Host interface
//-----------------------------------------------------------------------------
void Service_PCI2(void)
{
    if ( IsFlag0(PM1STS,IBF1) )  return;
    vSetTotalBurstTime();   // Set Burst mode total time (2ms)
    Service_PCI2_Main();    // Process Command/Data

}

//-----------------------------------------------------------------------------
// ACPI EC Command 0x80:
// ACPI Spec.13.3.1 Read Embedded Controller, RD_EC (0x80)
// Read Command (3 Bytes) -- First Bytes: Port 66H <-0x80
// Byte #1 (Command byte Header) Interrupt on IBF=0
//-----------------------------------------------------------------------------
void EC6266_CMD_80(void)
{
    SCI_RESPONSE();     // Interrupt on IBF=0
    PM1Step = _PM1_STEP_1;

}

//-----------------------------------------------------------------------------
// ACPI Spec.13.3.1 Read Embedded Controller, RD_EC (0x80)
// Second Bytes: Port 62H -> Address Index (EC Space)
// Byte #2 (Address byte to read) No Interrupt
//-----------------------------------------------------------------------------
void EC62_DATA_STEP1(void)
{   // Byte #3 (Data read to host)
    // PM1DO = ACPI Sepc.13.2.3 Embedded Controller Data, EC_DATA (R/W)
    PM1Data1 = Read_MapECSpace(PM1Data);
    PM1DO = PM1Data1;
    SCI_RESPONSE();     // Interrupt on OBF=1
    #if En_Record62
    RamDebug(PM1Data1);
    #endif
    PM1Step = 0;
}

//-----------------------------------------------------------------------------
// ACPI EC Command 0x81:
// ACPI Spec.13.3.2 Write Embedded Controller, WR_EC (0x81)
// Write Command (3 Bytes) -- First Bytes: Port 66H <-0x81
// Byte #1 (Command byte Header) Interrupt on IBF=0
//-----------------------------------------------------------------------------
void EC6266_CMD_81(void)
{
    SCI_RESPONSE();     // Interrupt on IBF=0
    PM1Step = _PM1_STEP_2;

}

//-----------------------------------------------------------------------------
// ACPI Spec.13.3.2 Write Embedded Controller, WR_EC (0x81)
// Second Bytes: Port 62H -> Address Index (EC Space)
// Byte #2 (Address byte to write) Interrupt on IBF=0
//-----------------------------------------------------------------------------
void EC62_DATA_STEP2(void)
{
    PM1Data1=PM1Data;   //  Byte #2 Save Address Index
    SCI_RESPONSE();     // Interrupt on OBF=1
    PM1Step = _PM1_STEP_3;
//AGA-Kernel>>

//AGA-Kernel<<   	
}

//-----------------------------------------------------------------------------
// ACPI Spec.13.3.2 Write Embedded Controller, WR_EC (0x81)
// Third Bytes: Port 62H -> Data Write to Address Index (EC Space)
// Byte #3 (data byte to write) Interrupt on IBF=0
//-----------------------------------------------------------------------------
void EC62_DATA_STEP3(void)
{   // Byte #3 (Data read to host)
    SCI_RESPONSE(); // Interrupt on OBF=1
    Write_MapECSpace( PM1Data1,PM1Data );
    PM1Step = 0;
}

//-----------------------------------------------------------------------------
// ACPI EC Command 0x82:
// ACPI Spec.13.3.3 Burst Enable Embedded Controller, BE_EC (0x82)
// Burst Enable Command (2 Bytes)
// Byte #1 (Command byte Header) No Interrupt
// The Embedded Controller sets the Burst bit of the Embedded Controller
// Status Register, puts the Burst Acknowledge byte (0x90) into the SCI output
// buffer, sets the OBF bit, and generates an SCI to signal OSPM that it is
// in Burst mode.
//-----------------------------------------------------------------------------
void EC6266_CMD_82(void)
{
    SET_BIT(PM1STS,4);  // PM1STS.4 Set Burst mode flag
    PM1DO = 0x90;       // Byte #2 (Burst acknowledge byte)
    OS_ACPI_Mode = 1;   // Auto Set ACPI Mode if Host Do ECCmd82
    SCI_RESPONSE();     // Interrupt on IBF=0
    #if En_Record62
    RamDebug(0x90);
    #endif

}

//-----------------------------------------------------------------------------
// ACPI EC Command 0x83:
// ACPI Spec.13.3.4 Burst Disable Embedded Controller, BD_EC (0x83)
// Burst Disable Command (1 Byte)
// Byte #1 (Command byte Header) Interrupt on IBF=0
//-----------------------------------------------------------------------------
void EC6266_CMD_83(void)
{
    CLEAR_BIT(PM1STS,4);    /* PM1STS.4 Set Burst mode flag.        */
    /*  Clear Burst bit in secondary Host interface status register.*/
    SCI_RESPONSE();         /* Interrupt on IBF=0.                  */

}

//-----------------------------------------------------------------------------
// ACPI EC Command 0x84:
// ACPI Spec.13.3.5 Query Embedded Controller, QR_EC (0x84)
// Query Command (2 Bytes)
// Byte #1 (Command byte Header) No Interrupt
// Byte #2 (Query value to host) Interrupt on OBF=1
//-----------------------------------------------------------------------------
void EC6266_CMD_84(void)
{
    //OS_ACPI_Mode = 1;   // Auto Set ACPI Mode if Host Do ECCmd84
    /* Byte #2 (Query value to host).*/
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    #if (SUPPORT_ACPI_SMI)
    if( Is_Flag1(PM1STS,5) )
    {   SCI_LastQueryEvent = ReadSCI_QueryValue();  }
    else if( Is_Flag1(PM1STS,6) )
    {   SCI_LastQueryEvent = ReadSMI_QueryValue();  }
    else
    {
        SCI_LastQueryEvent = 0x00;
    }
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    #else
    SCI_LastQueryEvent = ReadSCI_QueryValue();
    #endif
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    PM1DO = SCI_LastQueryEvent;
    SCI_RESPONSE(); /* Interrupt on IBF=0.*/
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    #if SCI_EVENT_LOG
    if( SCI_LastQueryEvent > 0 )
    {
        SCI_SMI_Event_Record[(SCI_SMI_Index_Record&0x0F)] = SCI_LastQueryEvent;
        SCI_SMI_Index_Record++;
    }
    #endif
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    SCI_QueryEvent = 0x00;
    SCI_StepTimer = 0;
    WAIT_SCI_CENTER = 0;
}

//-----------------------------------------------------------------------------
// Generate SCIs in response to related transactions
//-----------------------------------------------------------------------------
void SCI_RESPONSE(void)
{
    if (!OS_ACPI_Mode)  return;

    #if (SCI_IS_PD3_ALT)
    PMCTL = 0x42;
    //Loop_Delay(T_SCI_RESPONSE);
    _nop_();
    PMCTL = 0x00;
    #else
    SCI_ON();
    //Loop_Delay(T_SCI_RESPONSE);
    _nop_();
    SCI_OFF();
    #endif

}

//-----------------------------------------------------------------------------
// Generate SCIs for Query event request
//-----------------------------------------------------------------------------
void SCI_INTERRUPT(void)
{
    #if (SCI_IS_PD3_ALT)
    PMIC = 0x43;
    //Loop_Delay(T_SCI_EVENT);
    _nop_();
    PMIC = 0x47;
    #else
    SCI_ON();
    _nop_();
    //Loop_Delay(T_SCI_EVENT);
    SCI_OFF();
    #endif
}

//-----------------------------------------------------------------------------
// Generate SMIs for Query event request
//-----------------------------------------------------------------------------
void SMI_INTERRUPT(void)
{
    #if (SMI_IS_PD4_ALT)
    PMIC = 0x45;
    Loop_Delay(T_SMI_EVENT);
    PMIC = 0x47;
    #else
    SMI_ON();
    Loop_Delay(T_SMI_EVENT);
    SMI_OFF();
    #endif
}

//-----------------------------------------------------------------------------
// FUNCTION: Loop Delay - Delay a number of microseconds.
// Input:  0x00 - 0xFF for delay timing.
//-----------------------------------------------------------------------------
void Loop_Delay(BYTE delay)
{
    while ( delay > 0 )
    {
        delay--;
    }

}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Upon receipt of the QR_EC command byte, the embedded
// controller places a notification byte with a value between 0-255,
// indicating the cause of the notification. The notification byte
// indicates which interrupt handler operation should be executed
// by OSPM to process the embedded controller SCI.
// The query value of zero is reserved for a spurious query result
// and indicates no outstanding event.
//-----------------------------------------------------------------------------
//Function: ReadSCI_QueryValue
//          Read Query event Value
//-----------------------------------------------------------------------------
BYTE ReadSCI_QueryValue(void)
{
    SCI_QueryEvent = SCI_Event_Buffer[SCI_Event_Out_Index];
    if( SCI_QueryEvent == 0x00 )
    {
        CLEAR_BIT(PM1STS,5);
        return SCI_QueryEvent;
    }

    SCI_Event_Buffer[SCI_Event_Out_Index] = 0x00;
    SCI_Event_Out_Index++;
    if (SCI_Event_Out_Index > EVENT_BUFFER_SIZE)
    {
        SCI_Event_Out_Index = 0x00;
    }
    return SCI_QueryEvent;

}

//-----------------------------------------------------------------------------
//Function: WriteSCI_QueryValue
//          Write Query Value to Buffer
//-----------------------------------------------------------------------------
BYTE WriteSCI_QueryValue(BYTE NewSCI)
{
    if( !CORE_PMFLAG_S0 )       return 0x00;

    if( (SCI_Event_Buffer[SCI_Event_In_Index]) == 0x00  )
    {
        SCI_Event_Buffer[SCI_Event_In_Index] = NewSCI;
        SCI_Event_In_Index++;
    }
    if( SCI_Event_In_Index > EVENT_BUFFER_SIZE )
    {
        SCI_Event_In_Index = 0x00;
    }

    SET_BIT(PM1STS,5);
    if ( WAIT_SCI_CENTER )  {   return 0x00;    }
    WAIT_SCI_CENTER = 1;    //Recovery default SCI center
    SCI_RESPONSE();
    return 0x00;

}

//-----------------------------------------------------------------------------
//Function: ReadSMI_QueryValue
//          Read Query event Value
//-----------------------------------------------------------------------------
BYTE ReadSMI_QueryValue(void)
{
    SMI_QueryEvent = SMI_Event_Buffer[SMI_Event_Out_Index];

    if( SMI_QueryEvent == 0x00 )
    {
        CLEAR_BIT(PM1STS,6);
        return SCI_QueryEvent;
    }

    SMI_Event_Buffer[SMI_Event_Out_Index] = 0x00;
    SMI_Event_Out_Index++;
    if (SMI_Event_Out_Index > EVENT_BUFFER_SIZE)
    {
        SMI_Event_Out_Index = 0x00;
    }
    return SMI_QueryEvent;

}

//-----------------------------------------------------------------------------
//Function: WriteSMI_QueryValue
//          Write Query Value to Buffer
//-----------------------------------------------------------------------------
BYTE WriteSMI_QueryValue(BYTE NewSMI)
{

    if( NewSMI == 0x00 )    return 0x00;

    if( (SMI_Event_Buffer[SMI_Event_In_Index]) == 0x00  )
    {
        SMI_Event_Buffer[SMI_Event_In_Index] = NewSMI;
        SMI_Event_In_Index++;
    }
    if( SMI_Event_In_Index > EVENT_BUFFER_SIZE )
    {
        SMI_Event_In_Index = 0x00;
    }
    return 0x00;

}

//-----------------------------------------------------------------------------
//Function: ClearEventBuffer
//          Clear SCI/SMI Event Buffer
//-----------------------------------------------------------------------------
void ClearEventBuffer(void)
{
    BYTE  i;
    for( i=0; i<EVENT_BUFFER_SIZE; i++ )
    {
        SMI_Event_Buffer[i] = 0;
        SCI_Event_Buffer[i] = 0;
    }
    SCI_Event_In_Index  = 0x00;
    SMI_Event_In_Index  = 0x00;
    SCI_Event_Out_Index = 0x00;
    SMI_Event_Out_Index = 0x00;
}

//-----------------------------------------------------------------------------
//Function: Service_EventCenter
//
//-----------------------------------------------------------------------------
void Service_EventCenter(void)
{
    //-------------------------------------------------------------------------
    SCI_StepTimer++;
    if( SCI_StepTimer > T_SCI_STEP )
    {
        SCI_StepTimer = 0;

        SCI_QueryEvent = SCI_Event_Buffer[SCI_Event_Out_Index];
        if( SCI_QueryEvent > 0x00 && CORE_PMFLAG_S0 )
        {
            if( OS_ACPI_Mode )
            {
                SET_BIT(PM1STS,5);
                CLR_BIT(PM1STS,6);
                SCI_INTERRUPT();
                return ;
            }
            #if (SUPPORT_ACPI_SMI)
            else
            {
                SMI_INTERRUPT();
            }
            #endif
        }
    }
    //------------------------------------------------------------------------
    #if (SUPPORT_ACPI_SMI)
    SMI_StepTimer++;
    if( SMI_StepTimer > T_SMI_STEP )
    {
        SMI_QueryEvent = SMI_Event_Buffer[SMI_Event_Out_Index];
        if( SMI_QueryEvent > 0x00 && CORE_PMFLAG_S0 )
        {
            if( OS_ACPI_Mode )
            {
                if( !(Is_Flag1(PM1STS,5) ) )    //SCI Service First
                {
                    SET_BIT(PM1STS,6);
                    SMI_INTERRUPT();
                    return ;
                }
            }
            else
            {
                SMI_INTERRUPT();
            }
        }
    }
    #endif
    //------------------------------------------------------------------------
}

//-----------------------------------------------------------------------------
// 13.9 SMBus Host Controller In terface via Embedded Controller
// Process OS SMBus command PROTOCOL:
// 0x00_Controller Not In Use   0x01_Reserved
// 0x02_Write Quick Command     0x03_Read Quick Command
// 0x04_Send Byte               0x05_Receive Byte
// 0x06_Write Byte              0x07_Read Byte
// 0x08_Write Word              0x09_Read Word
// 0x0A_Write Block             0x0B_Read Block
// 0x0C_Process Call            0x0D_Block Write-Block Read Process Call
//-----------------------------------------------------------------------------
const FUNCT_PTR_B_V code ACPISMBusviaEC_Table[16] =
{
    // ACPI response to writes to SMBus Protocol register. (SMB_PRTC)
    ACPI_SMB_NULL,          //0x00 Controller Not In Use
    ACPI_SMB_NULL,          //0x01 Reserved
    ACPI_SMB_WQuickCMD,     //0x02 Write Quick Command
    ACPI_SMB_RQuickCMD,     //0x03 Read Quick Command
    ACPI_SMB_SendBYTE,      //0x04 Send Byte
    ACPI_SMB_ReciveBYTE,    //0x05 Receive Byte
    ACPI_SMB_WriteBYTE,     //0x06 Write Byte
    ACPI_SMB_ReadBYTE,      //0x07 Read Byte
    ACPI_SMB_WriteWORD,     //0x08 Write Word
    ACPI_SMB_ReadWORD,      //0x09 Read Word
    ACPI_SMB_WriteBLOCK,    //0x0A Write Block
    ACPI_SMB_ReadBLOCK,     //0x0B Read Block
    ACPI_SMB_ProcessCall,   //0x0C Process Call
    ACPI_SMB_BLOCKCall,     //0x0D Write Block-Read Block Process Call
    ACPI_SMB_NULL,          //0x0E Reserved
    ACPI_SMB_NULL           //0x0F Reserved
};
//-----------------------------------------------------------------------------
BYTE ACPI_SMB_NULL(void)
{
    return  0x00;
}

//-----------------------------------------------------------------------------
void ACPISMBusviaEC(void)
{
    #if SUPPORTED_ACPI_SMB_EC

    BYTE  RUNCODE;
    RUNCODE = Oem_ACPISMBusviaEC();
    if( (SMB_PRTC != 0x00) && (RUNCODE == 0x00) )
    {
        SMB_STS = 0x00;
        SMB_PRTC &= 0x0F;
        // ACPI response to writes to SMBus Protocol register. (SMB_PRTC)
        if( (ACPISMBusviaEC_Table[SMB_PRTC])() )
        {
            SMB_STS = 0x80;
        }
        else
        {
            SMB_STS = 0x5A;
        }

        SMB_PRTC = 0x00;
        WAIT_SCI_CENTER = 0;
        WriteSCI_QueryValue(EC_SMB_SCI_Number);
    }

    #endif
}
//-----------------------------------------------------------------------------
// 0x02 Write Quick Command
//-----------------------------------------------------------------------------
BYTE ACPI_SMB_WQuickCMD(void)
{
    return 0x00;
}

//-----------------------------------------------------------------------------
// 0x03 Read Quick Command
//-----------------------------------------------------------------------------
BYTE ACPI_SMB_RQuickCMD(void)
{
    return 0x00;
}

//-----------------------------------------------------------------------------
// 0x04 Send Byte - ACPI Spec.13.9.2.3 Send Byte
//-----------------------------------------------------------------------------
BYTE ACPI_SMB_SendBYTE(void)
{
    return 0x00;
}

//-----------------------------------------------------------------------------
// 0x05 Receive Byte
//-----------------------------------------------------------------------------
BYTE ACPI_SMB_ReciveBYTE(void)
{
    return 0x00;
}

//-----------------------------------------------------------------------------
// 0x06 Write Byte -  ACPI Spec.13.9.2.5 Write Byte
//-----------------------------------------------------------------------------
BYTE ACPI_SMB_WriteBYTE(void)
{
    #if SUPPORTED_ACPI_SMB_EC
    return (bRWSMBus(SMBusChA, SMBusWB, SMB_ADDR, SMB_CMD, &SMB_DATA));
    #else
    return  0x00;
    #endif
}

//-----------------------------------------------------------------------------
// 0x07 Read Byte
//-----------------------------------------------------------------------------
BYTE ACPI_SMB_ReadBYTE(void)
{
    #if SUPPORTED_ACPI_SMB_EC
    return (bRWSMBus(SMBusChA, SMBusRB, SMB_ADDR, SMB_CMD, &SMB_DATA));
    #else
    return  0x00;
    #endif
}

//-----------------------------------------------------------------------------
// 0x08 Write Word
//-----------------------------------------------------------------------------
BYTE ACPI_SMB_WriteWORD(void)
{
    #if SUPPORTED_ACPI_SMB_EC
    return (bRWSMBus(SMBusChA, SMBusWW, SMB_ADDR, SMB_CMD, &SMB_DATA));
    #else
    return  0x00;
    #endif
}

//-----------------------------------------------------------------------------
// 0x09 Read Word
//-----------------------------------------------------------------------------
BYTE ACPI_SMB_ReadWORD(void)
{
    #if SUPPORTED_ACPI_SMB_EC
    return (bRWSMBus(SMBusChA, SMBusRW, SMB_ADDR, SMB_CMD, &SMB_DATA));
    #else
    return  0x00;
    #endif
}

//-----------------------------------------------------------------------------
// 0x0A Write Block
//-----------------------------------------------------------------------------
BYTE ACPI_SMB_WriteBLOCK(void)
{
    #if SUPPORTED_ACPI_SMB_EC
    return (bRSMBusBlock(SMBusChA, SMBusRBK, SMB_ADDR, SMB_CMD, &SMB_DATA));
    #else
    return  0x00;
    #endif
}

//-----------------------------------------------------------------------------
// 0x0B Read Block
//-----------------------------------------------------------------------------
BYTE ACPI_SMB_ReadBLOCK(void)
{
    #if SUPPORTED_ACPI_SMB_EC
    return (bRSMBusBlock(SMBusChA, SMBusRBK, SMB_ADDR, SMB_CMD, &SMB_DATA));
    #else
    return  0x00;
    #endif
}

//-----------------------------------------------------------------------------
// 0x0C Process Call
//-----------------------------------------------------------------------------
BYTE ACPI_SMB_ProcessCall(void)
{
    return 0x00;
}

//-----------------------------------------------------------------------------
// 0x0D Block Write-Block Read Process Call
//-----------------------------------------------------------------------------
BYTE ACPI_SMB_BLOCKCall(void)
{
    return 0x00;
}

//-----------------------------------------------------------------------------
// Burst mode or normal mode
//-----------------------------------------------------------------------------
bit CheckBurstMode(void)
{
    WORD BurstLoopOut;

    BurstLoopOut = T_Burst_Loop;

    while((PM1STS&0x02)==0x00)
    {
        BurstLoopOut--;
        if( TF1 || (BurstLoopOut==0) )  // Time-Out
        {
            ClearFlag( PM1STS,BURST );
            SCI_RESPONSE();     // Generate Interrupt
            TF1=0;
            return(0);
        }
    }
    return(1);

}

//-----------------------------------------------------------------------------
// Set Burst mode total time (1ms)
//-----------------------------------------------------------------------------
void vSetTotalBurstTime(void)
{
    ET1 = 0;                        // Disable timer1 interrupt
    TH1 = (T_ACPI_BurstTime)>>8;    // Set timer1 counter 2ms
    TL1 = (T_ACPI_BurstTime);       // Set timer1 counter 2ms
    TR1 = 1;                        // Enable timer1
    TF1 = 0;                        // Clear overflow flag
}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
const FUNCT_PTR_V_V code EC6266Cmd8X_Table[16] =
{
    EC6266_CMD_80,          // Process ACPI command 80
    EC6266_CMD_81,          // Process ACPI command 81
    EC6266_CMD_82,          // Process ACPI command 82
    EC6266_CMD_83,          // Process ACPI command 83
    EC6266_CMD_84,          // Process ACPI command 84
    EC6266_CMD_85,          // Process ACPI command 85
    EC6266_CMD_86,          // Process ACPI command 86
    EC6266_CMD_87,          // Process ACPI command 87
    EC6266_CMD_88,          // Process ACPI command 88
    EC6266_CMD_89,          // Process ACPI command 89
    EC6266_CMD_8A,          // Process ACPI command 8A
    EC6266_CMD_8B,          // Process ACPI command 8B
    EC6266_CMD_8C,          // Process ACPI command 8C
    EC6266_CMD_8D,          // Process ACPI command 8D
    EC6266_CMD_8E,          // Process ACPI command 8E
    EC6266_CMD_8F           // Process ACPI command 8F
};
//-----------------------------------------------------------------------------
void EC_Cmd_8X(void)
{
    (EC6266Cmd8X_Table[PM1Cmd&0x0F])();
}
//-----------------------------------------------------------------------------
// Process Command/Data received from System via the 2nd Host interface
//-----------------------------------------------------------------------------
const FUNCT_PTR_V_V code Port66_Table[16] =
{
    EC_Cmd_0X,          // Process ACPI command 0x
    EC_Cmd_1X,          // Process ACPI command 1x
    EC_Cmd_2X,          // Process ACPI command 2x
    EC_Cmd_3X,          // Process ACPI command 3x
    EC_Cmd_4X,          // Process ACPI command 4x
    EC_Cmd_5X,          // Process ACPI command 5x
    EC_Cmd_6X,          // Process ACPI command 6x
    EC_Cmd_7X,          // Process ACPI command 7x
    EC_Cmd_8X,          // Process ACPI command 8x
    EC_Cmd_9X,          // Process ACPI command 9x
    EC_Cmd_AX,          // Process ACPI command Ax
    EC_Cmd_BX,          // Process ACPI command Bx
    EC_Cmd_CX,          // Process ACPI command Cx
    EC_Cmd_DX,          // Process ACPI command Dx
    EC_Cmd_EX,          // Process ACPI command Ex
    EC_Cmd_FX,          // Process ACPI command Fx
};
//-----------------------------------------------------------------------------
const FUNCT_PTR_V_V code Port62_Table[16] =
{
    EC62_DATA_STEP0,    // PM1Step=0 Process ACPI EC data
    EC62_DATA_STEP1,    // PM1Step=1 Process ACPI EC data
    EC62_DATA_STEP2,    // PM1Step=2 Process ACPI EC data
    EC62_DATA_STEP3,    // PM1Step=3 Process ACPI EC data
    EC62_DATA_STEP4,    // PM1Step=4 Process ACPI EC data
    EC62_DATA_STEP5,    // PM1Step=5 Process ACPI EC data
    EC62_DATA_STEP6,    // PM1Step=6 Process ACPI EC data
    EC62_DATA_STEP7     // PM1Step=7 Process ACPI EC data
};
//-----------------------------------------------------------------------------
// Service Embedded Controller Interface
//-----------------------------------------------------------------------------
void Service_PCI2_Main(void)
{

    do
    {   // PM1STS = Embedded Controller Status, EC_SC (R)
        if ( PM1STS & C_D1 )  // CMD:1=Byte in data register is a command byte
        {
            PM1Cmd  = PM1DI; // Load command from Port Buffer
            PM1Step = 0;

            #if SUPPORTED_RECORDER
            if(En_Record66 )
            {
                RamDebug(0x66); RamDebug(PM1Cmd);
            }
            #endif

            (Port66_Table[PM1Cmd>>4])(); // Handle command

        }
        else    // CMD:0=Byte in data register is a data byte
        {
            //PM1DI,PM1DO=ACPI Sepc.13.2.3 Embedded Controller Data,EC_DATA(R/W)
            PM1Data = PM1DI;         // Load data
            if (PM1Step!=0x00)
            {
                #if SUPPORTED_RECORDER
                if(En_Record62)
                {
                    RamDebug(0x62); RamDebug(PM1Data);
                }
                #endif

                (Port62_Table[PM1Step&0x07])(); // Handle command data

            }
            else
            {
                while( ECUnProcessCnt < 0xFF )
                {
                    ECUnProcessCnt++;
                }
            }
        }

    }
    while( IsFlag1(PM1STS,BURST) && CheckBurstMode() );

    ET1 = 1;    // Enable timer1 interrupt

    #if  SUPPORTED_ACPI_SMB_EC
    // ACPI Spec.13.9 SMBus Host Controller In terface via Embedded Controller
    ACPISMBusviaEC();   // ACPI SMBus command  process
    #endif
}
//-----------------------------------------------------------------------------

/*-----------------------------------------------------------------------------
 * End
 *---------------------------------------------------------------------------*/