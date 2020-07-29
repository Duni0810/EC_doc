/*-----------------------------------------------------------------------------
 * Filename: CORE_SMBUS.C
 * Function: System Management SMBus Protocol drivers.
 *
 * Copyright (c) 2006-2009, ITE Tech. Inc. All Rights Reserved.
 *---------------------------------------------------------------------------*/
#include "CORE_INCLUDE.H"
#include "OEM_INCLUDE.H"

/*-----------------------------------------------------------------------------
 * Local Parameter Definition
 *---------------------------------------------------------------------------*/
#define SUPPORT_SMBUS_EEPROM        0

//=============================================================================
// Channel: [SMBusChA] [SMBusChB] [SMBusChC] [SMBusChD]
// Protocol:[SMBusWQ]  -Write Quick command [SMBusRQ]  -Read Quick command
//          [SMBusSBC] -Send Byte command   [SMBusRBC] -Receive Byte command
//          [SMBusWB]  -Write byte command  [SMBusRB]  -Read byte command
//          [SMBusWW]  -Write word command  [SMBusRW]  -Read word command
//          [SMBusWBK] -Write block command [SMBusRBK] -Read block command
//
//=============================================================================
const sSMBus code asSMBus[]=
{
    { &HOCTL_A, &TRASLA_A, &HOCMD_A, &HOSTA_A, &SMBPCTL_A, &D0REG_A, &D1REG_A, &HOBDB_A },
    { &HOCTL_B, &TRASLA_B, &HOCMD_B, &HOSTA_B, &SMBPCTL_B, &D0REG_B, &D1REG_B, &HOBDB_B },
    { &HOCTL_C, &TRASLA_C, &HOCMD_C, &HOSTA_C, &SMBPCTL_C, &D0REG_C, &D1REG_C, &HOBDB_C },
    { &HOCTL_D, &TRASLA_D, &HOCMD_D, &HOSTA_D, &SMBPCTL_D, &D0REG_D, &D1REG_D, &HOBDB_D },
};
//-----------------------------------------------------------------------------
//*****************************************************************************
// SMBus Read/write byte or word function
//
// Channel: [SMBusChA] [SMBusChB] [SMBusChC] [SMBusChD]
// Protocol:[SMBusWB]0x88-Write byte command [SMBusRB]0x08-Read byte command
//          [SMBusWW]0x8C-Write word command [SMBusRW]0x0C-Read word command
// Addr:    device address
// Comd:    access command
// *Var:    Point to data Low Byte and *Var++ is high High Byte
// Return:  A byte in SMBUS_STATUS with [TRUE]:Ok. [FALSE] Failed.
//*****************************************************************************
//-----------------------------------------------------------------------------
bit bRWSMBus( BYTE Channel, BYTE Protocol, BYTE Addr, BYTE Comd, XBYTE *Var )
{
    bit ERROR_FLAG=FALSE;
    //-------------------------------------------------------------------------
    if( (*asSMBus[Channel].SMBusPinCTL&0x07) != 0x07 )
    {
        ERROR_FLAG = TRUE;
        SMBUS_STATUS = 0x40;
    }
    //-------------------------------------------------------------------------
    if( Protocol&0x80 )
    {
        *asSMBus[Channel].SMBusADR = Addr;
        *asSMBus[Channel].SMBusADR &=(~0x01);
        Protocol &=(~0x80);
        *asSMBus[Channel].SMBusData0 = *Var;
        if( Protocol == 0x0C )  //[SMbusWW]0x8C-Write word command
        {
            *asSMBus[Channel].SMBusData1 = *(Var+0x01);
        }
    }
    else
    {
        *asSMBus[Channel].SMBusADR=Addr|0x01;
    }
    //-------------------------------------------------------------------------
    *asSMBus[Channel].SMBusCMD = Comd;              // Set command
    *asSMBus[Channel].SMBusSTA = 0xFE;              // Clear status bits
    *asSMBus[Channel].SMBusCTL=(Protocol|BIT6);     // START transaction
    //-------------------------------------------------------------------------
    ERROR_FLAG = CheckSMBusStatus( Channel );
    //-------------------------------------------------------------------------
    if( !ERROR_FLAG )
    {   // Normal Access
        if ((*asSMBus[Channel].SMBusADR & 0x01 != 0x00))
        {
            *Var = *asSMBus[Channel].SMBusData0;
            if( Protocol==0x0C )
            {
                *(Var+0x01) = *asSMBus[Channel].SMBusData1;
            }
        }
        *asSMBus[Channel].SMBusSTA=0xFE;    // clear bits
        SMBUS_STATUS = 0;
        return(TRUE);
    }
    SMBUS_STATUS |= *asSMBus[Channel].SMBusSTA;
    vResetSMBus(Channel);
    return(FALSE);
    //-------------------------------------------------------------------------
}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//*****************************************************************************
// SMBus send byte function
//
//*****************************************************************************
//-----------------------------------------------------------------------------
bit bSMBusSendByte( BYTE Channel, BYTE Addr, BYTE SData )
{
    bit ERROR_FLAG=FALSE;
    //-------------------------------------------------------------------------
    if( (*asSMBus[Channel].SMBusPinCTL&0x07) != 0x07 )
    {
        ERROR_FLAG = TRUE;
        SMBUS_STATUS = 0x40;
    }
    //-------------------------------------------------------------------------
    *asSMBus[Channel].SMBusADR = Addr;          // Set address with writing bit
    *asSMBus[Channel].SMBusCMD = SData;         // Set command
    *asSMBus[Channel].SMBusSTA = 0xFE;          // Clear bits
    *asSMBus[Channel].SMBusCTL = (0x04|BIT6);   // START transaction
    //-------------------------------------------------------------------------
    ERROR_FLAG = CheckSMBusStatus( Channel );
    //-------------------------------------------------------------------------
    if( !ERROR_FLAG )
    {   // Normal Access
        *asSMBus[Channel].SMBusSTA=0xFE;    // clear bits
        SMBUS_STATUS = 0;
        return(TRUE);
    }
    SMBUS_STATUS |= *asSMBus[Channel].SMBusSTA;
    vResetSMBus(Channel);
    return(FALSE);
    //-------------------------------------------------------------------------
}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//*****************************************************************************
// SMBus receive byte function
//
//*****************************************************************************
//-----------------------------------------------------------------------------
bit bSMBusReceiveByte( BYTE Channel, BYTE Addr, XBYTE *Var )
{
    bit ERROR_FLAG=FALSE;
    //-------------------------------------------------------------------------
    if( *asSMBus[Channel].SMBusPinCTL != 0x07 )
    {
        ERROR_FLAG = TRUE;
        SMBUS_STATUS = 0x40;
    }
    //-------------------------------------------------------------------------
    *asSMBus[Channel].SMBusADR = (Addr|0x01);   // Set address with writing bit
    *asSMBus[Channel].SMBusSTA = 0xFE;          // Clear status bits
    *asSMBus[Channel].SMBusCTL = (0x04|BIT6);   // START transaction
    //-------------------------------------------------------------------------
    if( !ERROR_FLAG )
    {   // Normal Access
        *Var = *asSMBus[Channel].SMBusData0;    // read data
        *asSMBus[Channel].SMBusSTA=0xFE;        // Clear status bits
        SMBUS_STATUS = 0;
        return(TRUE);
    }
    SMBUS_STATUS |= *asSMBus[Channel].SMBusSTA;
    vResetSMBus(Channel);
    return(FALSE);
    //-------------------------------------------------------------------------
}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//*****************************************************************************
// Check SMBus status with timeout status
//
//*****************************************************************************
//-----------------------------------------------------------------------------
bit CheckSMBusStatus( BYTE Channel )
{
    bit bSMB_ERROR=TRUE;
    //-------------------------------------------------------------------------
    ET1 = 0;                    // Disable timer interrupt
    TH1 = T_Timer_25ms>>8;      // Set timer counter 25ms
    TL1 = T_Timer_25ms;         // Set timer counter 25ms
    TR1 = 1;                    // Enable timer
    TF1 = 0;                    // Clear overflow flag
    //-------------------------------------------------------------------------
    while (!TF1)
    {
        if( ( *asSMBus[Channel].SMBusSTA & 0x1C ) != 0x00 )
        {
            break;
        }
        else if( ( *asSMBus[Channel].SMBusSTA & 0x82 ) != 0x00 )
        {
            bSMB_ERROR = FALSE;
            break;
        }
    }

    TR1 = 0;
    TF1 = 0;
    ET1 = 1;
    return bSMB_ERROR;
    //-------------------------------------------------------------------------
}
//-----------------------------------------------------------------------------

//=============================================================================
//-----------------------------------------------------------------------------
//*****************************************************************************
// Read SMBus block function
//
//*****************************************************************************
//-----------------------------------------------------------------------------
bit bRSMBusBlock( BYTE Channel, BYTE Protocol, BYTE Addr, BYTE Comd, XBYTE *Var )
{
    bit ERROR_FLAG=FALSE;
    //-------------------------------------------------------------------------
    if( (*asSMBus[Channel].SMBusPinCTL&0x07) != 0x07 )
    {
        ERROR_FLAG = TRUE;
        SMBUS_STATUS = 0x40;
    }
    //-------------------------------------------------------------------------
    *asSMBus[Channel].SMBusADR=Addr|0x01;  // Set address with reading bit
    *asSMBus[Channel].SMBusCMD=Comd;       // Set command
    Protocol &= 0x7F;
    *asSMBus[Channel].SMBusCTL=(Protocol|BIT6);    // START transaction
    //-------------------------------------------------------------------------
    ERROR_FLAG = CheckSMBusStatus( Channel );
    //-------------------------------------------------------------------------
    *Var = *asSMBus[Channel].SMBusBData;   // Get first data to buffer
    Var++;
    _R2 = *asSMBus[Channel].SMBusData0;
    if( _R2 > 0 )
    {
        _R2--;
        if( _R2 == 0x01 )
        {   // Set last byte flag
            SetFlag(*asSMBus[Channel].SMBusCTL,BIT5);
        }
    }
    *asSMBus[Channel].SMBusSTA=0xFE;       // Clear bits
    //-------------------------------------------------------------------------
    while( _R2 > 0 && !ERROR_FLAG )
    {
        ERROR_FLAG = CheckSMBusStatus( Channel );
        if( ERROR_FLAG )    break;

        *Var = *asSMBus[Channel].SMBusBData;    // Get block data to buffer
        Var++;
        *asSMBus[Channel].SMBusSTA=0xFE;   // Clear bits
        _R2--;
        if( _R2 == 0x01 )
        {   // Set last byte flag
            SetFlag(*asSMBus[Channel].SMBusCTL,BIT5);
        }
    }
    //-------------------------------------------------------------------------
    ClearFlag(*asSMBus[Channel].SMBusCTL,BIT5);
    if( !ERROR_FLAG )
    {   // Normal Access
        *asSMBus[Channel].SMBusSTA=0xFE;        // Clear status bits
        SMBUS_STATUS = 0;
        return(TRUE);
    }
    SMBUS_STATUS |= *asSMBus[Channel].SMBusSTA;
    vResetSMBus(Channel);
    return(FALSE);
    //-------------------------------------------------------------------------
}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//*****************************************************************************
// Write SMBus block function
//
//*****************************************************************************
//----------------------------------------------------------------------------
bit bWSMBusBlock( BYTE Channel, BYTE Protocol, BYTE Addr, BYTE Comd, XBYTE *Var, BYTE ByteCont )
{
    //-------------------------------------------------------------------------
    bit ERROR_FLAG=FALSE; 
    _R1 = TRUE;
    //-------------------------------------------------------------------------
    if( (*asSMBus[Channel].SMBusPinCTL&0x07) != 0x07 )
    {
        ERROR_FLAG = TRUE;
        SMBUS_STATUS = 0x40;
    }
    //-------------------------------------------------------------------------
    *asSMBus[Channel].SMBusADR   = Addr;            // Set address
    *asSMBus[Channel].SMBusCMD   = Comd;            // Set command
    *asSMBus[Channel].SMBusData0 = ByteCont;        // Set byte count
    *asSMBus[Channel].SMBusBData = *Var;            // Set first data
    *asSMBus[Channel].SMBusCTL   = (Protocol|BIT6); // START transaction
    //-------------------------------------------------------------------------
    if( ByteCont == 0 &&(!ERROR_FLAG) )
    {
        ERROR_FLAG = CheckSMBusStatus( Channel );
        if( ERROR_FLAG )
            {
                _R1 = FALSE;
                vResetSMBus(Channel);
            }
            return(_R1);    // Clear last byte flag
    }
    //-------------------------------------------------------------------------
    ERROR_FLAG = CheckSMBusStatus( Channel );
    //-------------------------------------------------------------------------
    _R2 = ByteCont;
    Var++;                  // Pointing second byte
    //-------------------------------------------------------------------------
    while( (_R2 !=0x00) && !ERROR_FLAG )
    {
        ERROR_FLAG = CheckSMBusStatus( Channel );
        if( ERROR_FLAG )    break;
        *asSMBus[Channel].SMBusBData = *Var;
        Var++;
        *asSMBus[Channel].SMBusSTA = 0xFE;      // Clear bits
        _R2--;
    }
    //-------------------------------------------------------------------------
    ClearFlag(*asSMBus[Channel].SMBusCTL,BIT5);
    if( !ERROR_FLAG )
    {   // Normal Access
        *asSMBus[Channel].SMBusSTA=0xFE;        // Clear status bits
        SMBUS_STATUS = 0;
        return(TRUE);
    }
    SMBUS_STATUS |= *asSMBus[Channel].SMBusSTA;
    vResetSMBus(Channel);
    return(FALSE);
    //-------------------------------------------------------------------------
}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//*****************************************************************************
// SMBus Delay
//*****************************************************************************
//-----------------------------------------------------------------------------
void SMBusDelayInact(void)
{
    BYTE i;
    for( i=0; i<100; i++ )
    {
        WNCKR = 0;  // 15.26 us
    }
}
//-----------------------------------------------------------------------------

//=============================================================================
const sResetSMBusS code asResetSMBusS[]=
{
    { &HOSTA_A, &HOCTL2_A, &SMBPCTL_A, &GPIO_B3_CTRL, &GPIO_B4_CTRL, &GPIOB, 0x18 },
    { &HOSTA_B, &HOCTL2_B, &SMBPCTL_B, &GPIO_C1_CTRL, &GPIO_C2_CTRL, &GPIOC, 0x06 },
    { &HOSTA_C, &HOCTL2_C, &SMBPCTL_C, &GPIO_F6_CTRL, &GPIO_F7_CTRL, &GPIOF, 0xC0 },
    { &HOSTA_D, &HOCTL2_D, &SMBPCTL_D, &GPIO_H1_CTRL, &GPIO_H2_CTRL, &GPIOF, 0x06 }
};
//=============================================================================
//-----------------------------------------------------------------------------
//*****************************************************************************
// Reset SMBus Function
//*****************************************************************************
//-----------------------------------------------------------------------------
void vResetSMBus( BYTE Channel )
{
    //-------------------------------------------------------------------------
    BYTE i;
    //-------------------------------------------------------------------------
    // Reset Level 1
    *asResetSMBusS[Channel].SMBusPinCTL = 0x03;
    SMBusDelayInact();
    *asResetSMBusS[Channel].SMBusPinCTL = 0x07;
    *asResetSMBusS[Channel].SMBusSTA = 0xFE;    // Clear bits
    SMBusDelayInact();
    i = *asResetSMBusS[Channel].SMBusPinCTL;
    if( (i&0x07) == 0x07 )
    {
    SMBusDelayInact();
        *asResetSMBusS[Channel].SMBusPinCTL = 0x07;
        *asResetSMBusS[Channel].SMBusCTL2   = 0x01;
    return;
    }
    //-------------------------------------------------------------------------
    // Reset Level 2
    *asResetSMBusS[Channel].SMBusPin0 = 0x40;
    *asResetSMBusS[Channel].SMBusPin1 = 0x40;
    *asResetSMBusS[Channel].GPIOReg &= (0xFF-asResetSMBusS[Channel].GPIOData);
    SMBusDelayInact();
    SMBusDelayInact();
    SMBusDelayInact();
    *asResetSMBusS[Channel].GPIOReg |= asResetSMBusS[Channel].GPIOData;
    SMBusDelayInact();
    *asResetSMBusS[Channel].SMBusPin0 = 0x04;
    *asResetSMBusS[Channel].SMBusPin1 = 0x04;
    SMBusDelayInact();
    *asResetSMBusS[Channel].SMBusSTA  = 0xFE;    // Clear bits
    i = *asResetSMBusS[Channel].SMBusPinCTL;
    if( (i&0x07) == 0x07 )
    {
        SMBusDelayInact();
        *asResetSMBusS[Channel].SMBusPinCTL = 0x07;
        *asResetSMBusS[Channel].SMBusCTL2   = 0x01;
    return;
    }
    //-------------------------------------------------------------------------
    // Reset Level 3
    switch( Channel )
    {
        case SMBusChA:  RSTC3 = 0x01;   break;
        case SMBusChB:  RSTC3 = 0x02;   break;
        case SMBusChC:  RSTC3 = 0x04;   break;
        case SMBusChD:  RSTC3 = 0x08;   break;
    }
    SMBusDelayInact();
    *asResetSMBusS[Channel].SMBusPinCTL = 0x07;
    *asResetSMBusS[Channel].SMBusCTL2   = 0x01;
    //-------------------------------------------------------------------------

}
//-----------------------------------------------------------------------------

#if SUPPORT_SMBUS_EEPROM
//-----------------------------------------------------------------------------
//*****************************************************************************
// EEPRom accessing functions
//*****************************************************************************
//-----------------------------------------------------------------------------
#define EEPRomAddr  0xA0
#define EEPRBlcok1  0x00
#define EEPRBlcok2  0x02
#define EEPRBlcok3  0x04
#define EEPRBlcok4  0x06
#define EEPRRead    0x01
#define EEPRWrite   0x00
//-----------------------------------------------------------------------------
// Read EEPRom Byte function
//-----------------------------------------------------------------------------
typedef struct SMBusEEPRomStruct
{
    XBYTE   *SMBusCTL   ;
    XBYTE   *SMBusADR   ;
    XBYTE   *SMBusCMD   ;
    XBYTE   *SMBusSTA   ;
    XBYTE   *SMBusData0 ;
} sSMBusEEPRom;
//-----------------------------------------------------------------------------
const sSMBusEEPRom code asSMBusEEPRom[]=
{
    { &HOCTL_A, &TRASLA_A, &HOCMD_A, &HOSTA_A, &D0REG_A, },
    { &HOCTL_B, &TRASLA_B, &HOCMD_B, &HOSTA_B, &D0REG_B, }
};
//-----------------------------------------------------------------------------
bit vEEPRomRandonRead( BYTE Channel, BYTE addr, BYTE BlockNum, XBYTE *Var )
{
    _R1 = TRUE;
    _R2 = 0x00;
    _R3 = BlockNum;
    _R3 *= 0x02;

    *asSMBusEEPRom[Channel].SMBusADR = EEPRomAddr|_R3|EEPRWrite;
    *asSMBusEEPRom[Channel].SMBusCMD = addr;
    *asSMBusEEPRom[Channel].SMBusCTL = 0x04|BIT6;
    do
    {
        _R2++;
        if(_R2>=SMBusTOC)
        {
            _R1 = FALSE;
            return(_R1);
        }
    }while((*asSMBusEEPRom[Channel].SMBusSTA & 0x02 )==0x00);
    *asSMBusEEPRom[Channel].SMBusSTA = 0xFE;

    *asSMBusEEPRom[Channel].SMBusData0 = 0x00;
    _R2 = 0x00;
    *asSMBusEEPRom[Channel].SMBusADR = EEPRomAddr|_R3|EEPRRead;
    *asSMBusEEPRom[Channel].SMBusCTL = 0x04|BIT6;
    do
    {
        _R2++;
        if(_R2>=SMBusTOC)
        {
            _R1 = FALSE;
            return(_R1);
        }
    }while((*asSMBusEEPRom[Channel].SMBusSTA & 0x02 )==0x00);
    *asSMBusEEPRom[Channel].SMBusSTA = 0xFE;

    *Var = *asSMBusEEPRom[Channel].SMBusData0;

    return(_R1);
}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//Write EEPRom Byte function
//-----------------------------------------------------------------------------
bit vEEPRomWriteByte( BYTE Channel, BYTE addr, BYTE BlockNum, BYTE Vardata )
{
    _R1 = TRUE;
    _R2 = 0x00;
    _R3 = BlockNum;
    _R3 *= 0x02;

    *asSMBusEEPRom[Channel].SMBusADR = EEPRomAddr|_R3|EEPRWrite;
    *asSMBusEEPRom[Channel].SMBusCMD = addr;
    *asSMBusEEPRom[Channel].SMBusData0 = Vardata;
    *asSMBusEEPRom[Channel].SMBusCTL = 0x08|BIT6;

    do
    {
        _R2++;
        if(_R2>=SMBusTOC)
        {
            _R1 = FALSE;
            return(_R1);
        }
    }while((*asSMBusEEPRom[Channel].SMBusSTA & 0x02 )==0x00);

    *asSMBusEEPRom[Channel].SMBusSTA = 0xFE;
    return(_R1);
}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Write EEPRom page (16 bytes) function
//-----------------------------------------------------------------------------
#define ByteNumber  16
typedef struct SMBusEEPRomBStruct
{
    XBYTE   *SMBusCTL   ;
    XBYTE   *SMBusCTL2  ;
    XBYTE   *SMBusADR   ;
    XBYTE   *SMBusCMD   ;
    XBYTE   *SMBusSTA   ;
    XBYTE   *SMBusBData ;
    XBYTE   *SMBusData0 ;
} sSMBusEEPRomB;
//-----------------------------------------------------------------------------
const sSMBusEEPRomB code asSMBusEEPRomB[]=
{
    { &HOCTL_A, &HOCTL2_A, &TRASLA_A, &HOCMD_A, &HOSTA_A, &HOBDB_A, &D0REG_A },
    { &HOCTL_B, &HOCTL2_B, &TRASLA_B, &HOCMD_B, &HOSTA_B, &HOBDB_B, &D0REG_B }
};
//-----------------------------------------------------------------------------
bit bWEEPRomPage( BYTE Channel, BYTE Addr, BYTE BlockNum, XBYTE *Var )
{
    _R1 = TRUE;
    _R2 = ByteNumber;
    _R3 = BlockNum;
    _R3*= 0x02;
    _R4 = 0x00;

    SetFlag(*asSMBusEEPRomB[Channel].SMBusCTL2,BIT1);
    *asSMBusEEPRomB[Channel].SMBusADR = EEPRomAddr|_R3|EEPRWrite;
    *asSMBusEEPRomB[Channel].SMBusCMD=Addr;
    *asSMBusEEPRomB[Channel].SMBusData0=ByteNumber;
    *asSMBusEEPRomB[Channel].SMBusBData=*Var;
    *asSMBusEEPRomB[Channel].SMBusCTL=(0x14|BIT6);
Loop:
    do
    {
        _R4++;
        if(_R4>=SMBusTOC)
        {
            _R1 = FALSE;
            ClearFlag(*asSMBusEEPRomB[Channel].SMBusCTL2,BIT1);
            return(_R1);
        }
    }while((*asSMBusEEPRomB[Channel].SMBusSTA & 0x80 )==0x00);
    _R2--;
    Var++;

    if(_R2 !=0x00)
    {
        *asSMBusEEPRomB[Channel].SMBusBData=*Var;
        *asSMBusEEPRomB[Channel].SMBusSTA=0xFE;
        goto Loop;
    }

    *asSMBusEEPRomB[Channel].SMBusSTA=0xFE;
    ClearFlag(*asSMBusEEPRomB[Channel].SMBusCTL2,BIT1);

    return(_R1);
}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Read EEPRom sequential function
//-----------------------------------------------------------------------------
bit bREEPRomSEQ( BYTE Channel, BYTE Addr, BYTE BlockNum, XBYTE *Var )
{
    _R1 = TRUE;
    _R2 = ByteNumber;
    _R3 = BlockNum;
    _R3 *= 0x02;
    _R4 = 0x00;

    *asSMBusEEPRomB[Channel].SMBusADR = EEPRomAddr|_R3|EEPRRead;
    *asSMBusEEPRomB[Channel].SMBusCMD = Addr;
    *asSMBusEEPRomB[Channel].SMBusCTL = (0x14|BIT6);
Loop:
    do
    {
        _R4++;
        if(_R4>=SMBusTOC)
        {
            _R1 = FALSE;
            return(_R1);
        }
    }while((*asSMBusEEPRomB[Channel].SMBusSTA & 0x80 )==0x00);

    if(_R2 == ByteNumber)
    {
        *Var = *asSMBusEEPRomB[Channel].SMBusData0;
        _R2--;
    }

    _R2--;
    Var++;
    *Var = *asSMBusEEPRomB[Channel].SMBusBData;
    *asSMBusEEPRomB[Channel].SMBusSTA=0xFE;

    if(_R2 ==0x01)
    {
        SetFlag(*asSMBusEEPRomB[Channel].SMBusCTL,BIT5);
    }
    if(_R2 !=0x00)
    {
        goto Loop;
    }
    ClearFlag(*asSMBusEEPRomB[Channel].SMBusCTL,BIT5);
    return(_R1);

}
//-----------------------------------------------------------------------------
#endif

//-----------------------------------------------------------------------------
/*****************************************************************************/
/*****************************************************************************/
//
//      SMBUS Recovery Module
//
/*****************************************************************************/
/*****************************************************************************/
//-----------------------------------------------------------------------------
void SMBusRecovery(void)
{
    //-------------------------------------------------------------------------
    #if USE_SMBUS_CHANNEL_A
    if( (SMBPCTL_A&0x03) != 0x03 )
    {
        #if EN_S5_SMBUS_CHANNEL_A
        SMBA_PinError++;
        #else
        if( CORE_PMFLAG_S0 )
        {
            SMBA_PinError++;
        }
        #endif
    }
    //-------------------------------------------------------------------------
    if( SMBA_PinError > 10 )
    {
        SMBPCTL_A = 0x00;
        RSTC3 = 0x01;       //Reset SMBus-A
        SMBusDelayInact();
        SMBA_PinError = 0;
        bSMBA_GetData = 0;
        SMBPCTL_A = 0x07;
        HOCTL2_A  = 0x01;
        return;
    }
    #endif
    //-------------------------------------------------------------------------
    #if USE_SMBUS_CHANNEL_B
    if( (SMBPCTL_B&0x03) != 0x03 )
    {
        #if EN_S5_SMBUS_CHANNEL_B
        SMBB_PinError++;
        #else
        if( CORE_PMFLAG_S0 )
        {
            SMBB_PinError++;
        }
        #endif
    }
    //-------------------------------------------------------------------------
    if( SMBB_PinError > 10 )
    {
        SMBPCTL_B = 0x00;
        RSTC3 = 0x02;       //Reset SMBus-B
        SMBusDelayInact();
        SMBB_PinError = 0;
        bSMBB_GetData = 0;
        SMBPCTL_B = 0x07;
        HOCTL2_B  = 0x01;
        return;
    }
    #endif
    //-------------------------------------------------------------------------
    #if USE_SMBUS_CHANNEL_C
    if( (SMBPCTL_C&0x03) != 0x03 )
    {
        #if EN_S5_SMBUS_CHANNEL_C
        SMBC_PinError++;
        #else
        if( CORE_PMFLAG_S0 )
        {
            SMBC_PinError++;
        }
        #endif
    }
    //-------------------------------------------------------------------------
    if( SMBC_PinError > 10 )
    {
        SMBPCTL_C = 0x00;
        RSTC3 = 0x04;       //Reset SMBus-C
        SMBusDelayInact();
        SMBC_PinError = 0;
        bSMBC_GetData = 0;
        SMBPCTL_C = 0x07;
        HOCTL2_C  = 0x01;
        return;
    }
    #endif
    //-------------------------------------------------------------------------
    #if USE_SMBUS_CHANNEL_D
    if( (SMBPCTL_D&0x03) != 0x03 )
    {
        #if EN_S5_SMBUS_CHANNEL_D
        SMBD_PinError++;
        #else
        if( CORE_PMFLAG_S0 )
        {
            SMBD_PinError++;
        }
        #endif
    }
    //-------------------------------------------------------------------------
    if( SMBD_PinError > 10 )
    {
        SMBPCTL_D = 0x00;
        RSTC3 = 0x08;       //Reset SMBus-D
        SMBusDelayInact();
        SMBD_PinError = 0;
        bSMBD_GetData = 0;
        SMBPCTL_D = 0x07;
        HOCTL2_D  = 0x01;
        return;
    }
    #endif
    //-------------------------------------------------------------------------
}
//-----------------------------------------------------------------------------

/*-----------------------------------------------------------------------------
 * End
 *---------------------------------------------------------------------------*/
