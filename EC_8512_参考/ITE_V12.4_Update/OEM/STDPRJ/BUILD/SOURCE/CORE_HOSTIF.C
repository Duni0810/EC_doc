/*-----------------------------------------------------------------------------
 * TITLE: CORE_HOSTIF.C - Host Interface Handler
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

/*-----------------------------------------------------------------------------
 * Local Parameter Definition
 *---------------------------------------------------------------------------*/
#define T_InitSIO_LOOP          0x100

/* ----------------------------------------------------------------------------
 * FUNCTION: Service_SEND
 *
 * Send data from the scanner keyboard or from multibyte command responses to
 * the Host.
 *
 * Send scan codes from scanner keyboard to the Host.  Also handle multiple
 * byte transmissions for standard commands and extended commands that return
 * more than one byte to the Host.
 *
 * When sending multiple bytes, the 1st byte is sent immediately, but the
 * remaining bytes are sent by generating another send request via the
 * function "handle_unlock" which will call "Start_Scan_Transmission".
 * If more bytes are to be sent, "Start_Scan_Transmission" will start Timer A,
 * and the Timer A interrupt handler will generate the send request when the
 * response timer has expired!
 * ------------------------------------------------------------------------- */
void Service_SEND(void)
{
    //-------------------------------------------------------------------------
    BYTE nKBData;
    FLAG bBreak;
    BYTE KBCmdAck;
    //-------------------------------------------------------------------------
    KeyboardSendTimer++;

    TH1 = (T_Timer_SEND)>>8;
    TL1 = (T_Timer_SEND);       // Reload timer

    TF1 = 0;
    TR1 = 1;
    ET1 = 1;
    Timer_B.fbit.SEND_ENABLE = 1;
    
    //-------------------------------------------------------------------------
    if( IsFlag1(KBHISR,OBF) || IsFlag1(KBHISR,IBF) )
    {
        return;
    }
    if ( KBPendingRXCount > 0 )
    {
        KBCmdAck = GetKB_PendingData();
        KBC_DataToHost(KBCmdAck);
        return;
    }

    /* ------------------------------------------------------------------------
    * Keyboard Buffer Data Send to Host(System)
    * ---------------------------------------------------------------------- */
    //if( Int_Var.Scan_Lock )     return;
    if( Ccb42_DISAB_KEY )       return;

    nKBData = Get_Buffer();
    FORCE_NO_XLATE = 0;

    if ( nKBData == 0xFF )  //0xFF: No key data in buffer
    {
        Timer_B.fbit.SEND_ENABLE = 0;
        return;
    }
    else
    {
        bBreak = Gen_Info_BREAK_SCAN;
        Gen_Info_BREAK_SCAN = 0;
        #if ENABLE_S5_KEYBOARD_INTR
        if( CORE_PMFLAG_S5 )        return;
        #endif
        #if ENABLE_S3_KEYBOARD_INTR
        if( CORE_PMFLAG_S3 )        return;
        #endif
        if ( send_to_pc(nKBData, bBreak) )
        {
            Gen_Info_BREAK_SCAN = 1;    // Break prefix code.
        }
    }
    //-------------------------------------------------------------------------
}

/* ----------------------------------------------------------------------------
 * FUNCTION: Service_Send_PS2
 *
 * ------------------------------------------------------------------------- */
void Service_Send_PS2(void)
{
    BYTE index;
	//if(IsFlag1(KBHISR,OBF)||IsFlag1(KBHISR,IBF)||(Ccb42_DISAB_AUX))		// Dino 0202
	if(IsFlag1(KBHISR,OBF)||IsFlag1(KBHISR,IBF))	
	{
        if(SKIP_AUX_SCAN_CUNT<SkipScanAUXTime)
        {
            SKIP_AUX_SCAN_CUNT = 10;    // pr-set to Stop scaning ps2 interface 100ms 
        }
        SetPS2InhibitTime(InactiveTime);
		return;
	}

	if(TPACKCounter!=0x00)
	{
		TPACKCounter--;
	}
	else
	{
		if(Ccb42_DISAB_AUX)		
		{
            if(SKIP_AUX_SCAN_CUNT<SkipScanAUXTime)
            {
                SKIP_AUX_SCAN_CUNT = 10;    // pr-set to Stop scaning ps2 interface 100ms 
            }
            SetPS2InhibitTime(InactiveTime);
			return;
		}
	}

	SendFromAux(MousePendingBuf);

    if(AUXInterfaceBusy)
    {
        AUXInterfaceBusy = 0;
        Send2Port((Main_KB_CHN-1), PS2_KBCmd_BK);
        SetPS2InhibitTime(KBCmdBusyInactiveTime);
    }
    else
    {
	    if(MULPX_Multiplex)
	    {
            PS2Deviceactive();          // Enable all ps2 interface
	    }
	    else
	    {
            for(index=0x00;index<3;index++)     // Check any keyboard is attached?
            {
                if(IsFlag1(AuxFlags[index], DEVICE_IS_KEYBOARD))
                {
                    *asPS2Struct[index].ps2ctrl=PS2_ReceiveMode;
                    SET_BIT(*asPS2Struct[index].ps2ier, asPS2Struct[index].ctrlbit); 
                }
            }
            for(index=0x00;index<3;index++)     // Check any keyboard is attached?
            {
                if(index == AUXPortNum)
                {
                    *asPS2Struct[AUXPortNum].ps2ctrl=PS2_ReceiveMode;
		            SET_BIT(*asPS2Struct[AUXPortNum].ps2ier, asPS2Struct[AUXPortNum].ctrlbit);
                }
            }
		    SetPS2InhibitTime(InactiveTime);  
	    }
	}
    ResetMSPendingData();
}

/* ----------------------------------------------------------------------------
 * FUNCTION: KB_DataToHost
 *
 * Clear error bits in the Host Interface status port and sends a command
 * response byte or a byte of keyboard data to the Host.  Generate Host IRQ1
 * if keyboard interrupts are enabled in controller command byte.
 *
 * Input:  data to send to Host.
 * ------------------------------------------------------------------------- */
BYTE KBC_DataToHost( BYTE nKBData )
{
    WORD  iLOOP;

    iLOOP = WaitKBDataDelay;
    do
    {
        if ( IS_BIT_SET( KBHISR, 1 ) )
        {
            KBC_DataPending( nKBData );
            return 0x00;
        }
        if ( (IS_BIT_CLEAR(KBHISR,1)) && (IS_BIT_CLEAR(KBHISR,0)) )
        {
            break;
        }
        iLOOP--;
    }
    while ( iLOOP != 0 );

    SET_BIT( KBHISR, 4 );      //Keyboard Enabled
    CLEAR_BIT( KBHISR,5 );    //Transmit timeout clear

    if ( Ccb42_INTR_KEY )
    {
        SET_BIT( KBHICR, 0 );
    }
    else
    {
        CLEAR_BIT( KBHICR, 0 );
    }

    if ( IS_BIT_SET( KBHISR, 1 ) )
    {
        KBC_DataPending( nKBData );
        return 0x00;
    }
    KBHIKDOR = nKBData;
    
    if(PS2_KBCMD)
    {
        RamDebug(nKBData);
    }

    #if  SUPPORTED_RECORDER
    if( En_RecordKB60 )     {   RamDebug(0x61);     RamDebug(nKBData);  }
    #endif

    return 0x01;

}


/* ----------------------------------------------------------------------------
 * FUNCTION: Data_To_Host
 *
 * Clear error bits in the Host Interface status port and sends a command
 * response byte or a byte of keyboard data to the Host.  Generate Host IRQ1
 * if keyboard interrupts are enabled in controller command byte.
 *
 * Input:  data to send to Host.
 * ------------------------------------------------------------------------- */
void Data_To_Host(BYTE data_byte)
{
    #if OEM_HOOK_KB_DATA
    if( OemHandle_Data_To_Host(data_byte) )
    {
        return;
    }
    #endif

    SET_BIT( KBHISR,4 );      //Keyboard Enabled
    CLEAR_BIT( KBHISR,5 );    //Transmit timeout

    if ( Ccb42_INTR_KEY )
    {
        SET_BIT( KBHICR, 0 );
    }
    else
    {
        CLEAR_BIT( KBHICR, 0 );
    }

    KBHIKDOR = data_byte;
    #if En_RecordKB60
    RamDebug(data_byte);
    #endif

}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void KBC_DataPending(BYTE nPending)
{
    if( KBPendingRXCount > 3 )  return;

    KBDataPending[(KBPendingRXCount&0x03)] = nPending;
    KBPendingRXCount++;
    EnableSendToHostTimer();
}
//-----------------------------------------------------------------------------
BYTE GetKB_PendingData(void)
{
    BYTE buffer_data;
    buffer_data=KBDataPending[(KBPendingTXCount&0x03)];
    KBPendingTXCount++;
    if( KBPendingTXCount >= KBPendingRXCount )
    {
        KBPendingTXCount = 0;
        KBPendingRXCount = 0;
    }
    return buffer_data;
}
//-----------------------------------------------------------------------------

/* ----------------------------------------------------------------------------
 * FUNCTION: Aux_Data_To_Host - Send auxiliary device (mouse) data to the Host.
 *
 * Clear error bits in the Host Interface status port and sends a byte of
 * aux device (mouse) data to the Host.  Generates Host IRQ12 if aux device
 * (mouse) interrupts are enabled in controller command byte.
 *
 * Input: data to send to Host.
 * ------------------------------------------------------------------------- */
void Aux_Data_To_Host( BYTE data_byte )
{
    if ( Ccb42_SYS_FLAG )       // Put system flag bit in Status Reg.
    {
        SET_BIT( KBHISR, 2 );
    }
    else
    {
        CLEAR_BIT( KBHISR, 2 );
    }

    CLEAR_BIT( KBHISR, 4 );      //Keyboard Disabled
    SET_BIT( KBHISR, 5 );

    if ( Ccb42_INTR_AUX )
    {
        SET_BIT( KBHICR, 1 );
    }
    else
    {
        CLEAR_BIT( KBHICR, 1 );
    }

    KBHIMDOR = data_byte;
}

//-----------------------------------------------------------------------------
#if !(OEM_InitSIO_Table)     /*<------<< PROJECT.H */
//-----------------------------------------------------------------------------
const BYTE code initsio_table[]=
{
    /* Configure and Enable Logical Device 06h(KBD) */
    0x07 ,0x06, // Select Logical Device 06h(KBD)
    0x70 ,0x01, // Set IRQ=01h for Logical Device 06h(KBD)
    0x30 ,0x01, // Enable Logical Device 06h(Mouse)
    /* Configure and Enable Logical Device 05h(Mouse) */
    0x07 ,0x05, // Select Logical Device 05h(Mouse)
    0x70 ,0x0C, // Set IRQ=0Ch for Logical Device 05h(Mouse)
    0x30 ,0x01, // Enable Logical Device 05h(Mouse)
    /* Enable Logical Device 11h(PM1) */
    0x07 ,0x11, // Select Logical Device 11h(PM1)
    0x70 ,0x00, // Clear IRQ=0 for  Logical Device 11h(PM1)
    0x30 ,0x01, // Enable Logical Device 11h(PM1)
    /* Enable Logical Device 12h(PM3) */
    0x07 ,0x12, // Select Logical Device 12h(PM3)
    0x70 ,0x00, // Clear IRQ=0 for  Logical Device 12h(PM3)
    0x30 ,0x01, // Enable Logical Device 12h(PM3)
    /* Enable Logical Device 04h(MSWC) */
    0x07 ,0x04, // Logical Device 04h(MSWC)
    0x30 ,0x00, // Disable MSWC
    /* Enable Logical Device 10h(RTC) */
    0x07 ,0x10, // Logical Device 10h(RTC)
    0x30 ,0x00, // Disable RTC
    /* Enable Logical Device 0Fh(Shared Memory) */
    0x07 ,0x0F, // Logical Device 0Fh(Shared Memory)
    0x30 ,0x01, // MSWC Shared Memory
    0xF4 ,0x09, // MSWC Shared Memory

};


/* ----------------------------------------------------------------------------
 * FUNCTION: InitSio
 *           Init super IO function
 * ------------------------------------------------------------------------- */
void InitSio(void)
{
    BYTE code * data_pntr;
    BYTE cnt;
    WORD Timeout;

    SET_BIT( LSIOHA, 0 );
    SET_BIT( IBMAE,  0 );
    SET_BIT( IBCTL,  0 );

    cnt = 0;
    data_pntr = initsio_table;
    while( cnt < (sizeof(initsio_table)/2) )
    {
        IHIOA = 0;      // Set indirect Host I/O Address
        IHD = *data_pntr;
        Timeout = 0;
        while( IS_BIT_SET(IBCTL,2) )
        {
            Timeout++;
            if( Timeout > T_InitSIO_LOOP )   break;
        }

        data_pntr ++;

        IHIOA = 1;      // Set indirect Host I/O Address
        IHD = *data_pntr;
        Timeout = 0;
        while( IS_BIT_SET(IBCTL,2) )
        {
            Timeout++;
            if( Timeout > T_InitSIO_LOOP )   break;
        }

        data_pntr ++;
        cnt ++;
    }

    CLEAR_BIT( LSIOHA, 0 );
    CLEAR_BIT( IBMAE,  0 );
    CLEAR_BIT( IBCTL,  0 );
}
//-----------------------------------------------------------------------------
#endif      /*<------<< PROJECT.H */
//-----------------------------------------------------------------------------

/* ----------------------------------------------------------------------------
 * FUNCTION: send_ext_to_pc, send_to_pc
 *
 * send_ext_to_pc is just a 'pre-entry' to the original send_to_pc
 * routine.  It's sole purpose is to check auxiliary keyboard entries
 * for external hotkey functions.  This allows it to set flags for
 * CTRL and ALT states, then if BOTH are set, hotkeys can be checked.
 *
 * Send data to Host.  If password is enabled, the data
 * will not be sent until the password is entered correctly.
 *
 * Input: data - Data to send to PC.
 *        break_prefix_flag - TRUE if last scan code was a break prefix.
 *
 * Returns: TRUE if translation mode is enabled and
 *          scan code was a break prefix.
 * ----------------------------------------------------------------------------*/
static BYTE send_to_pc( BYTE data_word, BYTE break_prefix_flag )
{
    return( common_send_to_pc( data_word, break_prefix_flag ) );
}

static BYTE common_send_to_pc( BYTE data_word, BYTE break_prefix_flag )
{
    BYTE send_it = FALSE;

    if ( Ccb42_XLATE_PC == 0 )          // Send data as is.
    {
        send_it = TRUE;
        break_prefix_flag = FALSE;
    }
    else    // Translation mode is enabled.
    {
        data_word = translate_to_pc( data_word, break_prefix_flag );
        if ( data_word == 0xFF )
        {
            break_prefix_flag = TRUE;   // Don't send break code prefix.
        }
        else if( data_word == 0x00 )
        {
            break_prefix_flag = TRUE;;  // Don't send break code prefix.
        }
        else
        {
            break_prefix_flag = FALSE;
            send_it = TRUE;
        }
    }

    if ( send_it ) {    Data_To_Host(data_word);   }

    return( break_prefix_flag );
}

/* ----------------------------------------------------------------------------
 * FUNCTION: translate_to_pc
 *
 * Translate scan code from "set 2 scan code" to "set 1 scan code".
 *
 * Input: data - scan code received from aux keyboard or local keyboard.
 *        break_prefix_flag - TRUE if last scan code was a break prefix.
 *
 * Return: a value with all bits set if data is break prefix (0xF0);
 *         otherwise, returns translated key.
 * ------------------------------------------------------------------------- */
static BYTE translate_to_pc( BYTE data_word, BYTE break_prefix_flag )
{
    /* Scan code set 2 to scan code set 1 translation table.  First byte is a
       dummy entry because scan code "0" is not translated. */
    static const BYTE code scan2_table[] =
    {
        0x00, 0x43, 0x41, 0x3F, 0x3D, 0x3B, 0x3C, 0x58,
        0x64, 0x44, 0x42, 0x40, 0x3E, 0x0F, 0x29, 0x59,
        0x65, 0x38, 0x2A, 0x70, 0x1D, 0x10, 0x02, 0x5A,
        0x66, 0x71, 0x2C, 0x1F, 0x1E, 0x11, 0x03, 0x5B,
        0x67, 0x2E, 0x2D, 0x20, 0x12, 0x05, 0x04, 0x5C,
        0x68, 0x39, 0x2F, 0x21, 0x14, 0x13, 0x06, 0x5D,
        0x69, 0x31, 0x30, 0x23, 0x22, 0x15, 0x07, 0x5E,
        0x6A, 0x72, 0x32, 0x24, 0x16, 0x08, 0x09, 0x5F,
        0x6B, 0x33, 0x25, 0x17, 0x18, 0x0B, 0x0A, 0x60,
        0x6C, 0x34, 0x35, 0x26, 0x27, 0x19, 0x0C, 0x61,
        0x6D, 0x73, 0x28, 0x74, 0x1A, 0x0D, 0x62, 0x6E,
        0x3A, 0x36, 0x1C, 0x1B, 0x75, 0x2B, 0x63, 0x76,
        0x55, 0x56, 0x77, 0x78, 0x79, 0x7A, 0x0E, 0x7B,
        0x7C, 0x4F, 0x7D, 0x4B, 0x47, 0x7E, 0x7F, 0x6F,
        0x52, 0x53, 0x50, 0x4C, 0x4D, 0x48, 0x01, 0x45,
        0x57, 0x4E, 0x51, 0x4A, 0x37, 0x49, 0x46, 0x54
    };

    BYTE check_break_bit = FALSE;

    if ( data_word == 0xF0 )
    {   /* Signify that break code prefix was encountered. */
        data_word = 0xFF;
    }
    else if ( data_word == 0x00 )
    {
        data_word = 0x00;           /* Key detection error/overrun. */
    }
    else if ( (data_word & 0x80) == 0 )
    {   /* Translate codes 01 thru 7F. */
        /* The variable "data" has scan code (set 2) to translate.
           Set "data" to the translated (to set 1) scan code. */
        data_word = scan2_table[data_word];
        check_break_bit = TRUE;
    }
    else if ( data_word == 0x83 )   /* ID code for 101/102 keys. */
    {
        data_word = 0x41;           /* Translate ID code. */
        check_break_bit = TRUE;
    }
    else if ( data_word == 0x84 )   /* ID code for 84 keys. */
    {
        data_word = 0x54;           /* Translate ID code. */
        check_break_bit = TRUE;
    }

    if ( check_break_bit && break_prefix_flag )
    {   /* Last code received by this routine was the break prefix.This must be
           a break code. Set high bit to indicate that this is a break code. */
        data_word |= 0x80;
    }

    return(data_word);
}


/*-----------------------------------------------------------------------------
 * End
 *---------------------------------------------------------------------------*/