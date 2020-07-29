/*-----------------------------------------------------------------------------
 * Filename: CORE_MODULE.C
 * Function: Kernel MOdule for chip special function
 *
 * [Mudule Description]
 *
 * 01. ADC Module
 * 02. GPIO Module
 * 03. PWN/TCHX Module
 * 04. One-Wired Access Module
 *
 * Copyright (c) 2006-2009, ITE Tech. Inc. All Rights Reserved.
 *---------------------------------------------------------------------------*/
#include "CORE_INCLUDE.H"
#include "OEM_INCLUDE.H"

/*-----------------------------------------------------------------------------
 * Local Parameter Definition
 *---------------------------------------------------------------------------*/
#define T_EGPC_LOOP         0x100

//-----------------------------------------------------------------------------
/*****************************************************************************/
/* ADC MODULE ****************************************************************/
/*****************************************************************************/
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// Get ADC Pin Data ( 2.2 ~ 2.6mSec )
//-----------------------------------------------------------------------------
void vScanADCInput( BYTE VCHNx, BYTE SELINx )
{
    CLEAR_BIT( ADCCFG,0 );  // ADCCFG.Bit0: ADCEN
    SET_BIT( ADCCFG,7 );    // ADCCFG.Bit7: FIRHIACC
    switch ( VCHNx )
    {
        case 0:     VCH0CTL = ( SELINx | 0x80 );
                    break;
        case 1:     VCH1CTL = ( SELINx | 0x80 );
                    break;
        case 2:     VCH2CTL = ( SELINx | 0x80 );
                    break;
        case 3:     VCH3CTL = ( SELINx | 0x80 );
                    break;
        default:
                    break;

    }

    CLEAR_BIT( ADCSTS,2 );  // ADCSTS.Bit2: ADCPS
    SET_BIT( ADCSTS,1 );    // ADCSTS.Bit1: DOVE
    SET_BIT( ADCSTS,0 );    // ADCSTS.Bit0: EOCE
    SET_BIT( ADCCFG,0 );    // ADCCFG.Bit0: ADCEN

    ADC_Timer2 = 255;
    while ( IS_BIT_CLEAR( ADCSTS, 0 ) ) // ACDSTS.Bit0: EOCE
    {
        ADC_Timer2--;

        if ( VCHNx  == 0x00 )
        {
            if ( IS_BIT_SET( VCH0CTL,7 ) )  // VCHxCTL.Bit7: DATVAL
            {
                break;
            }
            else if ( ADC_Timer2 == 0 )
            {
                VCH0CTL = 0x1F; // Set Disabled if Failed
                break;
            }

        }
        else if ( VCHNx  == 0x01 )
        {
            if ( IS_BIT_SET( VCH1CTL,7 ) )  // VCHxCTL.Bit7: DATVAL
            {
                break;
            }
            else if ( ADC_Timer2 == 0 )
            {
                VCH1CTL = 0x1F; // Set Disabled if Failed
                break;
            }
        }
        else if ( VCHNx  == 0x02 )
        {
            if ( IS_BIT_SET( VCH2CTL,7 ) )  // VCHxCTL.Bit7: DATVAL
            {
                break;
            }
            else if ( ADC_Timer2 == 0 )
            {
                VCH2CTL = 0x1F; // Set Disabled if Failed
                break;
            }
        }
        else if ( VCHNx  == 0x03 )
        {
            if ( IS_BIT_SET( VCH3CTL,7 ) )  // VCHxCTL.Bit7: DATVAL
            {
                break;
            }
            else if ( ADC_Timer2 == 0 ) //Check if timeout
            {
                VCH3CTL = 0x1F; // Set Disabled if Failed
                break;
            }
        }
        else
        {

            break;
        }

    }


}

//-----------------------------------------------------------------------------
// Kernel Auto-Scan ADC
//-----------------------------------------------------------------------------
void CoreAutoScanADC(void)
{
    #if SUPPORTED_CORE_ADC
    if ( ADC_Timer == 0x00 )
    {
        SetAutoScanADC();
        ADC_Timer = T_ADC_Timeout;
        //~250us
        return;
    }

    if ( IS_BIT_SET( ADCSTS, 0 ) )  // ACDSTS.Bit0: EOCE
    {
        #if SUPPORTED_VCH0_Scan
        GetVoltChnToRAM( VCH0CTL, VCH0DATM, VCH0DATL );
        #endif

        #if SUPPORTED_VCH1_Scan
        GetVoltChnToRAM( VCH1CTL, VCH1DATM, VCH1DATL );
        #endif

        #if SUPPORTED_VCH2_Scan
        GetVoltChnToRAM( VCH2CTL, VCH2DATM, VCH2DATL );
        #endif

        #if SUPPORTED_VCH3_Scan
        GetVoltChnToRAM( VCH3CTL, VCH3DATM, VCH3DATL );
        #endif

        ADC_Timer = 0;
        //~500us
        return;
    }
    ADC_Timer--;
    #endif

}

//-----------------------------------------------------------------------------
// Store ADC Scan Results
//-----------------------------------------------------------------------------
void GetVoltChnToRAM( BYTE VCHxCTL, BYTE VCHxDATM, BYTE VCHxDATL  )
{
    #if SUPPORTED_CORE_ADC
    if ( IS_BIT_CLEAR( VCHxCTL,7 ) )    // VCHxCTL.Bit7: DATVAL
    {
        return;
    }

    switch( VCHxCTL&0x0F )
    {
        case 0x00:  ADC_0 += ( ((WORD)VCHxDATM << 8) | VCHxDATL );
                    ADC_0 /= 2;     // Average
                    SET_BIT( ADC_FLAG,0 );
                    break;
        case 0x01:  ADC_1 += ( ((WORD)VCHxDATM << 8) | VCHxDATL );
                    ADC_1 /= 2;     // Average
                    SET_BIT( ADC_FLAG,1 );
                    break;
        case 0x02:  ADC_2 += ( ((WORD)VCHxDATM << 8) | VCHxDATL );
                    ADC_2 /= 2;     // Average
                    SET_BIT( ADC_FLAG,2 );
                    break;
        case 0x03:  ADC_3 += ( ((WORD)VCHxDATM << 8) | VCHxDATL );
                    ADC_3 /= 2;     // Average
                    SET_BIT( ADC_FLAG,3 );
                    break;
        case 0x04:  ADC_4 += ( ((WORD)VCHxDATM << 8) | VCHxDATL );
                    ADC_4 /= 2;     // Average
                    SET_BIT( ADC_FLAG,4 );
                    break;
        case 0x05:  ADC_5 += ( ((WORD)VCHxDATM << 8) | VCHxDATL );
                    ADC_5 /= 2;     // Average
                    SET_BIT( ADC_FLAG,5 );
                    break;
        case 0x06:  ADC_6 += ( ((WORD)VCHxDATM << 8) | VCHxDATL );
                    ADC_6 /= 2;     // Average
                    SET_BIT( ADC_FLAG,6 );
                    break;
        case 0x07:  ADC_7 += ( ((WORD)VCHxDATM << 8) | VCHxDATL );
                    ADC_7 /= 2;     // Average
                    SET_BIT( ADC_FLAG,7 );
                    break;
        case 0x0A:  ADC_VCC += ( ((WORD)VCHxDATM << 8) | VCHxDATL );
                    ADC_VCC /= 2;       // Average
                    SET_BIT( ADC_FLAG2,2 );
                    break;
        case 0x0B:  ADC_VSTBY += ( ((WORD)VCHxDATM << 8) | VCHxDATL );
                    ADC_VSTBY /= 2;     // Average
                    SET_BIT( ADC_FLAG2,3 );
                    break;
        case 0x0C:  ADC_AVCC += ( ((WORD)VCHxDATM << 8) | VCHxDATL );
                    ADC_AVCC /= 2;      // Average
                    SET_BIT( ADC_FLAG2,4 );
                    break;
        case 0x0D:  ADC_VBS += ( ((WORD)VCHxDATM << 8) | VCHxDATL );
                    ADC_VBS /= 2;       // Average
                    SET_BIT( ADC_FLAG2,5 );
                    break;
        default:

                    break;

    }
    #endif

}

//---------------------------------------------------------------------------
// Set Auto Scan Voltage Channel
//---------------------------------------------------------------------------
void SetAutoScanADC(void)
{

    CLEAR_BIT( ADCCFG,0 );  // ADCCFG.Bit0: ADCEN
    ADCSTS = 0x83;
    CLEAR_BIT( ADCCFG, 0 );     // ADCCFG.Bit0: ADCEN
    SET_BIT( KDCTL, 7 );        // KDCTL.Bit7: AHCE
    CLEAR_BIT( ADCCFG, 5 );     // ADCCFG.Bit5: DFILEN

    #if SUPPORTED_VCH0_Scan
    VCH0CTL = SetScanVoltChannel( VCH0CTL, 0 );
    #endif

    #if SUPPORTED_VCH1_Scan
    VCH1CTL = SetScanVoltChannel( VCH1CTL, 1 );
    #endif

    #if SUPPORTED_VCH2_Scan
    VCH2CTL = SetScanVoltChannel( VCH2CTL, 2 );
    #endif

    #if SUPPORTED_VCH3_Scan
    VCH3CTL = SetScanVoltChannel( VCH3CTL, 3 );
    #endif

    CLEAR_BIT( ADCSTS,2 );  // ADCSTS.Bit2: ADCPS
    SET_BIT( ADCSTS,1 );    // ADCSTS.Bit1: DOVE
    SET_BIT( ADCSTS,0 );    // ADCSTS.Bit0: EOCE
    SET_BIT( ADCCFG,0 );    // ADCCFG.Bit0: ADCEN


}

/*-----------------------------------------------------------------------------
 * sScanADC Data Structure
 *---------------------------------------------------------------------------*/
const struct sScanADC code ScanADCSetting[]=
{
    {SetVCH0_ScanA, SetVCH0_ScanB, SetVCH0_ScanC, SetVCH0_ScanD},
    {SetVCH1_ScanA, SetVCH1_ScanB, SetVCH1_ScanC, SetVCH1_ScanD},
    {SetVCH2_ScanA, SetVCH2_ScanB, SetVCH2_ScanC, SetVCH2_ScanD},
    {SetVCH3_ScanA, SetVCH3_ScanB, SetVCH3_ScanC, SetVCH3_ScanD},

};

//---------------------------------------------------------------------------
// Set Scan Voltage Channel
//---------------------------------------------------------------------------
BYTE SetScanVoltChannel( BYTE VCHxCTL, BYTE ADC_VCHx )
{
    #if SUPPORTED_CORE_ADC

    if( ( VCHxCTL&0x0F ) == ( (ScanADCSetting[ADC_VCHx].ADC_ScanA)&0x0F ) )
    {
        if ( ScanADCSetting[ADC_VCHx].ADC_ScanB == 0x9F )
        {
            return 0x9F;
        }
        else
        {
            return ScanADCSetting[ADC_VCHx].ADC_ScanB;
        }

    }
    else if( ( VCHxCTL&0x0F ) == ( (ScanADCSetting[ADC_VCHx].ADC_ScanB)&0x0F ) )
    {
        if ( ScanADCSetting[ADC_VCHx].ADC_ScanC == 0x9F )
        {
            return ScanADCSetting[ADC_VCHx].ADC_ScanA;
        }
        else
        {
            return ScanADCSetting[ADC_VCHx].ADC_ScanC;
        }
    }
    else if( ( VCHxCTL&0x0F ) == ( (ScanADCSetting[ADC_VCHx].ADC_ScanC)&0x0F ) )
    {
        if ( ScanADCSetting[ADC_VCHx].ADC_ScanD == 0x9F )
        {
            return ScanADCSetting[ADC_VCHx].ADC_ScanA;
        }
        else
        {
            return ScanADCSetting[ADC_VCHx].ADC_ScanD;
        }
    }
    else
    {
        return ScanADCSetting[ADC_VCHx].ADC_ScanA;

    }
    #endif

    return  0x9F;
}

//-----------------------------------------------------------------------------
/*****************************************************************************/
/* GPIO MODULE ***************************************************************/
/*****************************************************************************/
//-----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// Input Signal Debounce function
// Input Parameter: PIN_STATUS,_ACTIVE,Counter_ID,ReloadCnt
// Return:  ioStatus
//      0x00: No Acitve
//      0x01: Active
//      0x02: Active and Debounced
//      0x03: Repeat Active
// ----------------------------------------------------------------------------
struct inDebounce
{   XBYTE   *REG;
    XBYTE   *Counter;
    BYTE    FLAG_0;
    BYTE    FLAG_1;
};

//-----------------------------------------------------------------------------
const struct inDebounce code inDebounce_TBL[] =
    {
        {&DEBOUNCE_FLAG1    ,&DEBOUNCE_CONT0    ,BIT0   ,BIT1   },
        {&DEBOUNCE_FLAG1    ,&DEBOUNCE_CONT1    ,BIT2   ,BIT3   },
        {&DEBOUNCE_FLAG1    ,&DEBOUNCE_CONT2    ,BIT4   ,BIT5   },
        {&DEBOUNCE_FLAG1    ,&DEBOUNCE_CONT3    ,BIT6   ,BIT7   },
        {&DEBOUNCE_FLAG2    ,&DEBOUNCE_CONT4    ,BIT0   ,BIT1   },
        {&DEBOUNCE_FLAG2    ,&DEBOUNCE_CONT5    ,BIT2   ,BIT3   },
        {&DEBOUNCE_FLAG2    ,&DEBOUNCE_CONT6    ,BIT4   ,BIT5   },
        {&DEBOUNCE_FLAG2    ,&DEBOUNCE_CONT7    ,BIT6   ,BIT7   }
    };

//-----------------------------------------------------------------------------
// Return:  ioStatus
//      0x00: No Acitve
//      0x01: Active
//      0x02: Active and Debounced
//      0x03: Repeat Active
//-----------------------------------------------------------------------------
BYTE InputDebounce(FLAG PIN_STATUS,FLAG _ACTIVE,BYTE Counter_ID,BYTE ReloadCnt)
{
    BYTE    ioStatus = 0x00;
    XBYTE   *in_FlagREG;
    XBYTE   *in_Counter;
    BYTE    in_FLAG_0,in_FLAG_1;
    Counter_ID &= 0x07;
    in_FlagREG = inDebounce_TBL[Counter_ID].REG;
    in_Counter = inDebounce_TBL[Counter_ID].Counter;
    in_FLAG_0  = inDebounce_TBL[Counter_ID].FLAG_0;
    in_FLAG_1  = inDebounce_TBL[Counter_ID].FLAG_1;
    //Check Pin Status
    if( PIN_STATUS == _ACTIVE )
    {   if ( ( (*in_FlagREG) & (in_FLAG_1) ) == 0 )
        {
            if ( ( (*in_FlagREG) & (in_FLAG_0) ) == 0 )
            {   (*in_Counter) = ReloadCnt;
                (*in_FlagREG) |= in_FLAG_0;
                ioStatus = 0x01;
            }
            else
            {   ioStatus = 0x01;
                (*in_Counter)--;

                if( (*in_Counter) == 0 )
                {
                    (*in_FlagREG) |= in_FLAG_1;
                    (*in_FlagREG) &= (~in_FLAG_0);
                    ioStatus = 0x02;
                }
            }
        }
        else
        {
            (*in_FlagREG) |= in_FLAG_0;
            ioStatus = 0x03;
        }

    }
    else
    {
        (*in_Counter) = ReloadCnt;
        (*in_FlagREG) &= ( ~(in_FLAG_0+in_FLAG_1) );
        ioStatus = 0x00;
    }
    return  ioStatus;
}

//-----------------------------------------------------------------------------
/*****************************************************************************/
/* PWM/TACHX MODULE **********************************************************/
/*****************************************************************************/
//-----------------------------------------------------------------------------
// Calculation Fan Tachometer to Fan Speed (R.P.M)
// Fan Speed (R.P.M)=60/(1/fs sec * {FnTMRR,FnTLRR) * P )
//-----------------------------------------------------------------------------
WORD GetFanRPM(BYTE FnTMRR,BYTE FnTLRR)
{
    WORD  FanRPMSpd, FnTWRR, FanRPMDiv;

    FnTWRR = FnTMRR*0x100+FnTLRR;
    // Minimun filter
    if ( FnTWRR == 0x00 || FnTWRR > 0x3FFF )
    {  return 0;       }
    // Maximun filter
    if ( FnTWRR < 128 )
    {  return 9999;  }

    FanRPMDiv = _FAN_TACH_BASE1;
    FanRPMSpd = ( FanRPMDiv / FnTWRR )*0x1000;
    FanRPMDiv = ( FanRPMDiv % FnTWRR )*0x10+_FAN_TACH_BASE2;
    FanRPMSpd+= ( FanRPMDiv / FnTWRR )*0x100;
    FanRPMDiv = ( FanRPMDiv % FnTWRR )*0x10+_FAN_TACH_BASE3;
    FanRPMSpd+= ( FanRPMDiv / FnTWRR )*0x10;
    FanRPMDiv = ( FanRPMDiv % FnTWRR )*0x10+_FAN_TACH_BASE4;
    FanRPMSpd+= ( FanRPMDiv / FnTWRR );

    return FanRPMSpd;

}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/*****************************************************************************/
/* EGPC MODULE ***************************************************************/
/*****************************************************************************/
//-----------------------------------------------------------------------------
#if ExternalGPIO_8301
//-----------------------------------------------------------------------------
// Read External GPIO
//-----------------------------------------------------------------------------
bit EGPC_Read( BYTE Addr )
{
    EADDR = Addr;

    if( (EDAT >> 5) & 0x01 )
        return  TRUE;

    return  FALSE;
}
//-----------------------------------------------------------------------------
// Write External GPIO
//-----------------------------------------------------------------------------
void EGPC_Write( BYTE Addr, BYTE Data )
{
    BYTE EGPC_TimeOut;

    EADDR = Addr;
    EDAT  = Data;

    EGPC_TimeOut = T_EGPC_LOOP;
    while( (ESTS & CDS) == 0 )
    {
        EGPC_TimeOut--;
        if( EGPC_TimeOut == 0 )     break;
    }   // Check transfer cycle done
}
//-----------------------------------------------------------------------------
void Set_EGPC( BYTE Addr )
{
    BYTE EGPC_TimeOut;

    EADDR = Addr;
    EDAT |= 0x01;

    EGPC_TimeOut = T_EGPC_LOOP;
    while( (ESTS & CDS) == 0 )
    {
        EGPC_TimeOut--;
        if( EGPC_TimeOut == 0 )     break;
    }   // Check transfer cycle done

}
//-----------------------------------------------------------------------------
void Clr_EGPC(BYTE Addr)
{
    BYTE EGPC_TimeOut;

    EADDR = Addr;           // Address
    EDAT &= ~0x01;          // Data

    EGPC_TimeOut = T_EGPC_LOOP;
    while( (ESTS & CDS) == 0 )
    {
        EGPC_TimeOut--;
        if( EGPC_TimeOut == 0 )     break;
    }   // Check transfer cycle done
}
//-----------------------------------------------------------------------------
#endif  //ExternalGPIO_8301
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/*****************************************************************************/
/* ONE-WIRE ACCESS MODULE ****************************************************/
/*****************************************************************************/
//-----------------------------------------------------------------------------

#if SUPPORT_ONE_WIRE_MODULE
//-----------------------------------------------------------------------------
// OneWire Delay
//-----------------------------------------------------------------------------
void vOneWireDelayuS(BYTE Timer)
{
    for (;Timer!=0;Timer--)
    {   ;   }

}
//-----------------------------------------------------------------------------
// Reset device function
//-----------------------------------------------------------------------------
BYTE bResetOneWireDevice(void)
{
    BYTE timeout  = 0x00;
    BYTE loopcunt = 0x00;

    DCache = 0x03;

    OneWireOutput;
    OneWireLow;
    vOneWireDelayuS( _T_ONE_500 );  // Delay 500 us
    OneWireInput;

    while( OneWirePin == 0 )
    {
        timeout++;
        if( timeout > 0xFE )
        {
            DCache = 0x00;
            return(11);
        }
    }

    for( loopcunt=0x00; loopcunt<1; loopcunt++ )
    {
        if( OneWirePin == 0 )
        {
            DCache = 0x00;
            return(12);
        }
    }
    timeout = 0x00;
    while( OneWirePin == 1 )
    {
        timeout++;
        if( timeout > 0xFE )
        {
            DCache = 0x00;
            return(13);
        }
    }

    for( loopcunt=0x00; loopcunt<4; loopcunt++ )    // 80 us loop
    {
        if( OneWirePin == 1 )
        {
            DCache = 0x00;
            return(14);
        }
        vOneWireDelayuS( _T_ONE_1us );
    }
    timeout = 0x00;
    while( OneWirePin == 0 )
    {
        timeout++;
        if( timeout > 0xFE )
        {
            DCache = 0x00;
            return(15);
        }
    }

    vOneWireDelayuS( _T_ONE_500 );  // Delay 500 us
    DCache = 0x00;
    return(0xFF);
}

//-----------------------------------------------------------------------------
// Write one bit function
//-----------------------------------------------------------------------------
void vOneWireWriteBit( bit OneWirebit )
{
    OneWireOutput;
    OneWireLow;
    OneWirePin=OneWirebit;
    vOneWireDelayuS( _T_ONE_WRB );
    OneWireInput;
    vOneWireDelayuS( _T_ONE_1us );
}

//-----------------------------------------------------------------------------
// Write one byte function
//-----------------------------------------------------------------------------
void vOneWireWriteByte( BYTE OneWireByte )
{
    BYTE bitcunt;

    DCache = 0x03;

    for( bitcunt=0x00; bitcunt<8; bitcunt++ )
    {
        if( (OneWireByte>>bitcunt) & 0x01 )
        vOneWireWriteBit(1);
        else
        vOneWireWriteBit(0);
    }

    DCache = 0x00;
}

//-----------------------------------------------------------------------------
// Read one bit function
//-----------------------------------------------------------------------------
bit vOneWireReadBit(void)
{
    bit tempdata;

    OneWireOutput;
    OneWireLow;
    OneWireInput;
    tempdata=OneWirePin;
    vOneWireDelayuS( _T_ONE_RDB );
    return( tempdata );
}

//-----------------------------------------------------------------------------
// Read one byte function
//-----------------------------------------------------------------------------
BYTE vOneWireReadByte(void)
{
    BYTE bitcout;
    BYTE bitdata;
    BYTE result=0x00;

    DCache = 0x03;

    for( bitcout=0x00; bitcout < 8; bitcout++ )
    {
        bitdata = vOneWireReadBit();
        result |= (bitdata<<bitcout);
    }

    DCache = 0x00;
    return(result);
}
//-----------------------------------------------------------------------------
#endif  //SUPPORT_ONE_WIRE_MODULE
//-----------------------------------------------------------------------------

/*-----------------------------------------------------------------------------
 * End
 *---------------------------------------------------------------------------*/