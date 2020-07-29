/*-----------------------------------------------------------------------------
 * Filename: CORE_PS2.C - PS/2 devices handler
 *
 * Copyright (c) 2006-2009, ITE Tech. Inc. All Rights Reserved.
 *
 * You may not reproduce, distribute, publish, display, perform, modify, adapt,
 * transmit, broadcast, present, recite, release, license or otherwise exploit
 * any part of this publication in any form, by any means, without the prior
 * written permission of ITE Tech. Inc.
 *---------------------------------------------------------------------------*/
#include "CORE_INCLUDE.H"
#include "OEM_INCLUDE.H"

const sPS2Struct code asPS2Struct[] = 
{
 	{ &PSCTL1, &PSINT1, &PSSTS1,	&PSDAT1,	&IER2,	&ISR2,	4	},
	{ &PSCTL2, &PSINT2, &PSSTS2,	&PSDAT2,	&IER2,	&ISR2,	3	},
	{ &PSCTL3, &PSINT3, &PSSTS3,	&PSDAT3,	&IER2,	&ISR2,	2	}
};

//-----------------------------------------------------------------
// Send data from aux mouse 
//-----------------------------------------------------------------
void SendFromAux(BYTE auxdata)
{
    KBHISR &= 0x0F;

	if(MULPX_Multiplex)
	{
		if(AUXPortNum==0x00)
		{
			KBHISR = 0x40;				// source bits is 01
		}
		else if(AUXPortNum==0x01)
		{
			KBHISR = 0x80;				// source bits is 10
		}
        else if(AUXPortNum==0x02)
        {
			KBHISR = 0xC0;				// source bits is 11
		}
		CLEAR_BIT(KBHISR,2);			// clear error bit 
	}

	SET_BIT(KBHISR,5);					// set AUX bit
    if ( Ccb42_INTR_AUX )
    {
       	SET_BIT(KBHICR,1);
    }
 	else
 	{
 		CLEAR_BIT(KBHICR,1);
 	}

	if(PS2_MSCMD)
	{   
        #ifdef UART_Debug
            printf("Mouse Port %bX ACK : 0X%bX\n",AUXPortNum, auxdata);
        #else
            RamDebug(auxdata);
        #endif
	}
	KBHIMDOR = auxdata;

#if PS2_Pack_Debug    
    if(PS_Debug_ECToHost==0xFF)
    {
       PS_Debug_ECToHost = 0x00;
    }
    else
    {
        PS_Debug_ECToHost++;
        if(PS_Debug_ECToHost>3)
        {
            PS_Debug_ECToHost = 0x01;
        }
    }
#endif
}

//-----------------------------------------------------------------
// 
//-----------------------------------------------------------------
void SetPS2InhibitTime(BYTE time)
{
    if(AUXInactiveCounter<DeviceResetCmdITTime)
    {
        AUXInactiveCounter = time;
    }
}

//-----------------------------------------------------------------
// Process mouse data from ps2 interface
//-----------------------------------------------------------------
void ProcessMouseData(BYTE channel)
{
    BYTE index;
    
#if PS2_Pack_Debug  
    //RamDebug(PS2_Pack_Debug_Index);
    //RamDebug(channel);
    if(PS_Debug_ECToHost==0xFF)
    {
       PS_Debug_DeviceToEC = 0x00;
    }
    else
    {
        PS_Debug_DeviceToEC++;
        if(PS_Debug_DeviceToEC>3)
        {
            PS_Debug_DeviceToEC = 0x01;
        }
    }
#endif

    if(MS_ID_CUNT!=0x00)                    // Save mouse driver type(3bytes, 4bytes, or 5bytes)
    {
        if(--MS_ID_CUNT==0x00)
        {
            if(PS2PortxData[channel]==0x00||
                PS2PortxData[channel]==0x03||
                 PS2PortxData[channel]==0x04)
            {
                MS_DRIVER_TYPE = PS2PortxData[channel];
            }
        }
    }

    if(IsFlag1(KBHISR,OBF)||IsFlag1(KBHISR,IBF)||(Ccb42_DISAB_AUX))
    {	
        AUXPortNum = channel;
	    MousePendingBuf=PS2PortxData[channel];
	    MSPendingRXCount = 0x33;
    }
    else
    {
        AUXPortNum = channel;
	    SendFromAux(PS2PortxData[channel]);
	    if(TPACKCounter!=0x00)
	    {
		    TPACKCounter--;
	    }

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
                PS2Deviceactive();                  // Enable all ps2 interface
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
                    if(index == channel)
                    {
                        *asPS2Struct[channel].ps2ctrl=PS2_ReceiveMode;
		                SET_BIT(*asPS2Struct[channel].ps2ier, asPS2Struct[channel].ctrlbit);
                    }
                }
		        SetPS2InhibitTime(InactiveTime);    
	        }
        }
    }
} 

//-----------------------------------------------------------------
// Process keybaord data from ps2 interface
//-----------------------------------------------------------------
void ProcessKeyboardData(BYTE channel)
{
    BYTE index;
    if(Main_KB_ACK_CUNT!=0x00)
    {
        Main_KB_ACK_CUNT--;
        if(Main_KB_ACK_CUNT==0x00)
        { 
            SKIP_AUX_SCAN_CUNT = 0x00;
        }

        KBC_DataPending(PS2PortxData[channel]);
        F_Service_SEND = 1;
    }
    else
    {
        Buffer_Key(PS2PortxData[channel]);
        F_Service_SEND = 1;
    }

    if(Main_KB_ACK_CUNT==0x00)
    {   
        PS2Deviceactive();
    }
    else
    {
        *asPS2Struct[channel].ps2ctrl=PS2_ReceiveMode;
        SET_BIT(*asPS2Struct[channel].ps2ier, asPS2Struct[channel].ctrlbit);
        SetPS2InhibitTime(InactiveTime);
    }
}

void ProcessECTestAUXACK(BYTE PortNum)
{
    ECSendtoAUXFlag = 0;
    AUX_SCAN_TIMEOUT_CUNT = 0x00;
    AUX_SCAN_INDEX_HS = 0x00;
    ScanAUXStepIndex = ScanAUXStep_CheckOK + PortNum;

    if(IsFlag1(AuxFlags2[AUX_SCAN_INDEX],DEVICE_NEED_DISABLED))     // Command 0xF5 ACK
    {
        if(PS2PortxData[PortNum]==0xFA)
        {
            //ClearFlag(AuxFlags2[AUX_SCAN_INDEX],DEVICE_NEED_DISABLED);
            AuxFlags2[AUX_SCAN_INDEX] = 0x00;
            SetFlag(AuxFlags2[AUX_SCAN_INDEX],DEVICE_IS_DISABLED);
        }
        else
        {
            PS2_ReTryCunt++;
            if(PS2_ReTryCunt>PS2_OnOffTimeOutCUNT)
            {
                PS2_ReTryCunt = 0x00;
                //ClearFlag(AuxFlags2[AUX_SCAN_INDEX],DEVICE_NEED_DISABLED);
                AuxFlags2[AUX_SCAN_INDEX] = 0x00;
                SetFlag(AuxFlags2[AUX_SCAN_INDEX],DEVICE_DISABLE_TIME_OUT);
            }
        }
    }
    else if(IsFlag1(AuxFlags2[AUX_SCAN_INDEX],DEVICE_NEED_ENABLED))     // Command 0xF4 ACK
    {
        if(PS2PortxData[PortNum]==0xFA)
        {
            //ClearFlag(AuxFlags2[AUX_SCAN_INDEX],DEVICE_NEED_ENABLED);
            AuxFlags2[AUX_SCAN_INDEX] = 0x00;
            SetFlag(AuxFlags2[AUX_SCAN_INDEX],DEVICE_IS_ENABLED);
        }
        else
        {
            PS2_ReTryCunt++;
            if(PS2_ReTryCunt>PS2_OnOffTimeOutCUNT)
            {
                PS2_ReTryCunt = 0x00;
                //ClearFlag(AuxFlags2[AUX_SCAN_INDEX],DEVICE_NEED_ENABLED);
                AuxFlags2[AUX_SCAN_INDEX] = 0x00;
                SetFlag(AuxFlags2[AUX_SCAN_INDEX],DEVICE_ENABLED_TIME_OUT);
            }
        }
    }
    else                                            // Test Command 0xF1 ACK 
    {
        if(AuxFlags[PortNum]==0x00)                 // Device first time plug-in
        {
            AuxFlags[PortNum] = DEVICE_NEED_INIT;
            AUX_SCAN_INDEX_HS = 0xFF;
            ScanAUXStepIndex = ScanAUXStep_CheckType + PortNum; // goto check device is keyboard or mouse
            if(PS2_AUXTypeDebug)
            {
                RamDebug(PS2_AUX_PlugIn);
                RamDebug(PortNum);
            }
        }
        else
        {
            if(IsFlag1(AuxFlags[PortNum], DEVICE_NEED_CONFIG))
            {
                AUX_SCAN_INDEX_HS = 0xFF;                  
                ScanAUXStepIndex = ScanAUXStep_Init + PortNum;  // goto config. device 
            }
            else
            {
                if(AuxFlags_KB_CMD[PortNum]!=0x00)
                {
                    AUX_SCAN_INDEX_HS = 0xFF;
                    ScanAUXStepIndex = ScanAUXStep_UpdateKB + PortNum;
                    //if(PS2_AUXTypeDebug)
                    //{
                    //    RamDebug(PS2_AUX_StatsuUpdate);
                    //    RamDebug(PortNum);
                    //}
                }
            }
        }
    }
    PS2Deviceactive();	
}


//-----------------------------------------------------------------
// Bypass AUX data to host
//-----------------------------------------------------------------
void SendAUXData(BYTE PortNum)      		// Dino 0907             
{
    BYTE index;

    if(SendtoAUXFlag)				        // Send command to device and transmit done.
    {
        SendtoAUXFlag = 0;		
        *asPS2Struct[PortNum].ps2ctrl=PS2_ReceiveMode;  // Enable Receive mode for ACK byte
        SET_BIT(*asPS2Struct[PortNum].ps2ier, asPS2Struct[PortNum].ctrlbit);  // Enable interrupt for ACK byte
        AUX_SCAN_TIMEOUT_CUNT = 0x00;
    }
    else							        // Receive done and get data
    {	
        if(ECSendtoAUXFlag)                 // If thsi byte is EC scan channel ACK, no send to host
        {
            ProcessECTestAUXACK(PortNum);
        }
        else                                // receive mode transmit done.      
        {
            if(SKIP_AUX_SCAN_CUNT<SkipScanAUXTime)
            {
                SKIP_AUX_SCAN_CUNT = 100;    // pr-set to Stop scaning ps2 interface 1sec 
            }
            AUX_SCAN_INDEX_HS = 0x00;
            AUX_SCAN_TIMEOUT_CUNT = 0x00;
            if(IsFlag1(AuxFlags[PortNum], DEVICE_IS_MOUSE))     // If data from mosue
	        {
                ProcessMouseData(PortNum);      // Process mosue data
	        }
            else if(IsFlag1(AuxFlags[PortNum], DEVICE_IS_KEYBOARD)) // If data from keyboard
	        {	
	            ProcessKeyboardData(PortNum);   // Process keyboard data
	        }
            else
            {
                PS2Deviceactive();          // Enable all ps2 interface
            }
        }
    }
}


//-----------------------------------------------------------------
// 
//-----------------------------------------------------------------
void ClaerPS2IFPendingEvent(void)
{
    vInhibitAUXDevice();
    IER2&=(~0x1C);		        // Disable all PS2 interrupt
    ISR2|=0x1C;				    // Write to clear all PS2 pending interrupt
    PS2StartBit = 0;            // clear ps2 interrupt start bit flag
    F_Service_PS2_IRQ = 0;	    // Clear PS2 interrupt service flag
}

//-----------------------------------------------------------------
// EC Send command to ps2 interface no ack to host
//-----------------------------------------------------------------
void ECSend2Port( BYTE PortNum, BYTE PortData)
{
    ClaerPS2IFPendingEvent();
	ECSendtoAUXFlag = 1;
    SendtoAUXFlag = 1;
	*asPS2Struct[PortNum].ps2ctrl = 0x1D;
	*asPS2Struct[PortNum].ps2data = PortData;
	*asPS2Struct[PortNum].ps2ctrl = 0x1C;
	*asPS2Struct[PortNum].ps2ctrl = 0x1E;
   
    SET_BIT(*asPS2Struct[PortNum].ps2ier, asPS2Struct[PortNum].ctrlbit);    // Enable interrupt   
}

//-----------------------------------------------------------------
// Send data to ps2 interface
//-----------------------------------------------------------------
void Send2Port( BYTE PortNum, BYTE PortData)
{
    if(PortData==0xFF)                              // reset command
    {
        AUXInactiveCounter = DeviceResetCmdITTime;  // 1sec
    }
    else
    {
        SetPS2InhibitTime(InactiveTime);
    }
    
    ClaerPS2IFPendingEvent();

    if(SKIP_AUX_SCAN_CUNT<SkipScanAUXTime)
    {
        SKIP_AUX_SCAN_CUNT = SkipScanAUXTime; 
    }
    AUX_SCAN_INDEX_HS = 0x00;

    ECSendtoAUXFlag = 0;
    SendtoAUXFlag = 1;
	*asPS2Struct[PortNum].ps2ctrl = 0x1D;
	*asPS2Struct[PortNum].ps2data = PortData;
	*asPS2Struct[PortNum].ps2ctrl = 0x1C;
	*asPS2Struct[PortNum].ps2ctrl = 0x1E;
  
    SET_BIT(*asPS2Struct[PortNum].ps2ier, asPS2Struct[PortNum].ctrlbit);    // Enable interrupt 
}

//-----------------------------------------------------------------
// Send command to ps2 interface and check ack or not
//  Note : return   0 --> // Transaction done
//                  1 --> // Transaction fail
//
//                  After this function, all ps2 channel is inhibit and channel interrupt is disabled
//-----------------------------------------------------------------
BYTE Send2PortNWait( BYTE PortNum, BYTE cmd, BYTE bytecunt)
{
    BYTE result;
    BYTE index;

    PS2IFAck = 0x00;

    ClaerPS2IFPendingEvent();
    
    ECSendtoAUXFlag = 0;
    SendtoAUXFlag = 0;
	*asPS2Struct[PortNum].ps2ctrl = 0x1D;
	*asPS2Struct[PortNum].ps2data = cmd;
	*asPS2Struct[PortNum].ps2ctrl = 0x1C;
	*asPS2Struct[PortNum].ps2ctrl = 0x1E;

    result = bExtAUXTimeOutCheck(PortNum);

    if(result==0x00)                                    // Transaction done
    {
        for(index=0x00;index<bytecunt;index++)
        {
            *asPS2Struct[PortNum].ps2ctrl=PS2_ReceiveMode;  // Get Last ACK by bytecunt
            result = bExtAUXTimeOutCheck(PortNum);
            if(result!=0x00)
            {
                break;                                  // Transaction fail
            }
        }
    }

    return(result);		                        
}

//-----------------------------------------------------------------
// Process AUX command
//-----------------------------------------------------------------
void vSendCmdtoMouse(BYTE PortNum)
{
    BYTE index;
    ResetMSPendingData();
	Cmd_A8();   
	TPACKCounter = 0x00;
    MS_ID_CUNT = 0x00;

	if(AUX2ByteCommand)
	{	
		if(TPResolution == 0xFF)
		{
			TPResolution = KBHIData;
		}
		else if(TPSamplingRate == 0xFF)
		{
			TPSamplingRate = KBHIData;		
		}

		Send2Port(PortNum, KBHIData);	
		AUX2ByteCommand = 0;
		return;
	}

	switch(KBHIData)
	{
		case 0xE8:     			// Set Resolution (2 byte)
			TPResolution = 0xFF;
			AUX2ByteCommand=1;
			break;
		
		case 0xF3:     			// Set Sampling Rate (2 byte)
			TPSamplingRate = 0xFF;	
			AUX2ByteCommand=1;
			break;	
			
		case 0xF4:	
            #if PS2_Pack_Debug
            PS_Debug_ECToHost = 0xFF;
            #endif
			EnableTP = 1;
			MouseDriverIn = 1;
			break;
			
		case 0xF5:
			MouseDriverIn = 0;
			break;	
			
		case 0xE6:
			break;
			
		case 0xE7:
			break;
			
		case 0xEA:
			break;
			
		case 0xEC:
			break;
			
		case 0xEE:
			break;
			
		case 0xF0:
			break;
			
		case 0xF6:		
			break;	
			
		case 0xE9:	
			break;
			
		case 0xF2:	
            MS_ID_CUNT = 2;
			break;
			
		case 0xEB:
		case 0xFE:	
			break;
			
		case 0xFF:
            //ResetMSPendingData();
			MouseDriverIn = 0;
            MULPX_Multiplex = 0;	    // Disable Activate Multiplex mode 
			EnableTP = 0;
			TPACKCounter = 0x03;		// ACK 3 bytes

            for(index=0x00; index<3; index++)
            {
                if((Main_MOUSE_CHN-1)!=index)
                {
                    if(IsFlag1(AuxFlags[index], DEVICE_IS_MOUSE))
                    {
                        AuxFlags[index] = 0x00;
                    }
                }
            }
			break;
			
		default:			            // Invalid command
			break;
	}

	Send2Port(PortNum, KBHIData);

}

//-----------------------------------------------------------------
// Process AUX MUX command
//-----------------------------------------------------------------
void vSendCmdtoMUX(BYTE port)
{
	Cmd_A8();
    ResetMSPendingData();
	if(AUX2ByteCommand)
	{	
		Send2Port(port, KBHIData);	
		AUX2ByteCommand = 0;
		return;
	}

	switch(KBHIData)
	{
		case 0xE8:     		// Set Resolution (2 byte)
		case 0xF3:     		// Set Sampling Rate (2 byte)
			Send2Port(port, KBHIData);	
			AUX2ByteCommand=1;
			break;	
			
		case 0xF4:	
			Send2Port(port, KBHIData);
			break;
			
		case 0xF5:	
			Send2Port(port, KBHIData);
			break;
			
		case 0xE6:
		case 0xE7:
		case 0xEA:
		case 0xEC:
		case 0xEE:
		case 0xF0:	
		case 0xF6:	
		case 0xE9:
		case 0xEB:
		case 0xF2:				
		case 0xFE:	
			Send2Port(port, KBHIData);
			break;
			
		case 0xE1:						// For older Synaptics TouchPads	
			Send2Port(port, KBHIData);
			break;	
			
		case 0xFF:
            //ResetMSPendingData();
			Send2Port(port, KBHIData);
			break;
			
		default:						// Invalid command
			break;
	}
}


//-----------------------------------------------------------------
// FUNCTION: Ps2_Init - Init PS2 registers and variables.  
// Put in receive mode.
//-----------------------------------------------------------------
void Ps2_Init(void)
{

}

//-----------------------------------------------------------------
// The function of PS2 interface interrupt service routine
//-----------------------------------------------------------------
void Service_PS2_IRQ(void)
{	
    if(PS2_IRQ_Channel!=0xFF)
    {
        SendAUXData(PS2_IRQ_Channel);
        PS2_IRQ_Channel = 0xFF;
    }
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
// The function of stopping ps2 devices send data to KBC
//----------------------------------------------------------------------------
void vPS2DeviceInactive(void)
{
	PSCTL1 = PS2_InhibitMode;
	PSCTL2 = PS2_InhibitMode;
	PSCTL3 = PS2_InhibitMode;
}

//----------------------------------------------------------------------------
// The function of starting ps2 devices send data to KBC
//----------------------------------------------------------------------------
void PS2Deviceactive(void)
{	 
    BYTE index;
    for(index=0x00;index<3;index++)
    {
        if(IsFlag1(AuxFlags[index],DEVICE_IS_KEYBOARD))
        {
            *asPS2Struct[index].ps2ctrl=PS2_ReceiveMode;
            SET_BIT(*asPS2Struct[index].ps2ier, asPS2Struct[index].ctrlbit); 
        }
    }
    for(index=0x00;index<3;index++)
    {
        if(IsFlag1(AuxFlags[index],DEVICE_IS_MOUSE))
        {
            *asPS2Struct[index].ps2ctrl=PS2_ReceiveMode;
            SET_BIT(*asPS2Struct[index].ps2ier, asPS2Struct[index].ctrlbit); 
        }
    }     
}

//----------------------------------------------------------------------------
// The function of inhibit AUX device
//----------------------------------------------------------------------------
void vInhibitAUXDevice(void)
{ 
	vPS2DeviceInactive();

	ET1 = 0;		// Disable timer1 interrupt
    TL1 = 0x8C;     // set timer1 counter 150 us
    TH1 = 0xFF;     // set timer1 counter 150 us	
	TR1 = 1;		// enable timer1
 	TF1 = 0;		// clear overflow flag
	while(!TF1);	// waitting overflow flag
	TF1 = 0;		// clear overflow flag
	ET1 = 1;		// Enable timer1 interrupt
}

//----------------------------------------------------------------------------
// The function of scaning AUX device
//----------------------------------------------------------------------------
void ScanAUXDevice(void)
{
	BYTE index;
    BYTE timeout;
   
	for(index=0x00;index<3;index++)
	{
        if(IsFlag1(AuxFlags[index], DEVICE_IS_MOUSE)||IsFlag1(AuxFlags[index], DEVICE_IS_KEYBOARD))
        {
            continue;
        }
		AuxFlags[index] = 0x00;                 

		if(index==0x00)
		{
			if((PS2_Port0_CLK_CtrlReg&0xFB)!=0x00 || (PS2_Port0_Data_CtrlReg&0xFB)!=0x00)
				continue;                   // If this channel isn't PS2 ALT function, scan next.             
		}
		else if(index==0x01)
		{
			if((PS2_Port1_CLK_CtrlReg&0xFB)!=0x00 || (PS2_Port1_Data_CtrlReg&0xFB)!=0x00)
				continue;                   // If this channel isn't PS2 ALT function, scan next.    
		}
		else if(index==0x02)
		{
			if((PS2_Port2_CLK_CtrlReg&0xFB)!=0x00 || (PS2_Port2_Data_CtrlReg&0xFB)!=0x00)
				continue;                   // If this channel isn't PS2 ALT function, scan next.    
		}
        
        if(Send2PortNWait(index,0xFF,1)==0x00)
        {
            SetFlag(AuxFlags[index], DEVICE_IS_ATTACHED);

            for(timeout=0x00;timeout<=20;timeout++) // 600 ms time-out
            {
                *asPS2Struct[index].ps2ctrl=PS2_ReceiveMode;
                if(!bExtAUXTimeOutCheck(index))
                {
                    break;
                }
            }
                
            if(timeout>20)                      // wait aa time-out
            {
                if(PS2_AUXTypeDebug)
                {
                    RamDebug(FF_ACK_AA_TimeOut);
                }
            }
            else
            {
                for(timeout=0x00;timeout<=10;timeout++)
                {
                    *asPS2Struct[index].ps2ctrl=PS2_ReceiveMode;
                    if(!bExtAUXTimeOutCheck(index))
                    {
                        break;
                    }
                }
                    
                if(timeout>10)          // wait 00 time-out
                {
                    SetFlag(AuxFlags[index], DEVICE_IS_KEYBOARD);
                    CheckNSetMainKBCHN(index);
        
                    if(PS2_AUXTypeDebug)
                    {
                        RamDebug(FF_ACK_00_TimeOut);
                        RamDebug(PS2_AUXType_KB);
                    }        
                }
                else
                {
                    if(PS2_AUXTypeDebug)
                    {
                        RamDebug(FF_ACK_00_TimeDone);
                    }
                        
                    SetFlag(AuxFlags[index], DEVICE_IS_MOUSE);

                    CheckNSetMainMSCHN(index);

                    if(PS2_AUXTypeDebug)
                    {
                        RamDebug(PS2_AUXType_KB);
                        RamDebug(PS2_KB_Strange);
                    } 
                }
            }
        }
        else
        {
			continue;           // Device non-ack
        }       
	}

    if((Main_MOUSE_CHN==0x00)&&(Main_KB_CHN==0x00)) // No any device
    {
        AUX_SCAN_TIMEOUT_CUNT = 0x00;
        AUX_SCAN_INDEX_HS = 0x00;
    }
    
    #if TouchPad_only
        Main_KB_CHN = 0x00;
    #endif
}

//----------------------------------------------------------------------------
// The function of reading device ID
//----------------------------------------------------------------------------
void ReadDeviceID(void)
{
    PS2IFAck = 0x00;
    if(Send2PortNWait(AUX_SCAN_INDEX,0xF2,2)==0x00)
    {
	    if(PS2IFAck ==0xAB)                                         // Device is keyboard
	    {
            SetFlag(AuxFlags[AUX_SCAN_INDEX], DEVICE_IS_KEYBOARD);
            CheckNSetMainKBCHN(AUX_SCAN_INDEX);
            *asPS2Struct[AUX_SCAN_INDEX].ps2ctrl=PS2_ReceiveMode;   // Get ID
            bExtAUXTimeOutCheck(AUX_SCAN_INDEX);
            if(PS2_AUXTypeDebug)
            {
                RamDebug(PS2_AUXType_KB);
            }
        }
        else
        {
            SetFlag(AuxFlags[AUX_SCAN_INDEX], DEVICE_IS_MOUSE);      // Device is mouse
            CheckNSetMainMSCHN(AUX_SCAN_INDEX);
            if(PS2_AUXTypeDebug)
            {
                RamDebug(PS2_AUXType_MS);
            }
        }
    }
    else
    {
        if(PS2_AUXTypeDebug)
        {
            RamDebug(PS2_AUXType_F2_TimeOut);
        }
    }
}


//----------------------------------------------------------------------------
// The function of checking AUX device is keyborad or mouse
//----------------------------------------------------------------------------
void CheckAUXDeviceType(void)
{
    BYTE timeout;
    
    if(AuxFlags[AUX_SCAN_INDEX] == DEVICE_NEED_INIT)
    {
        AuxFlags[AUX_SCAN_INDEX]=0x00;
        PS2IFAck = 0x00;

        if(PS2_AUXTypeDebug)
        {
            RamDebug((AUX_SCAN_INDEX+1));
        }

        if(Send2PortNWait(AUX_SCAN_INDEX,0xFF,1)==0x00)
        {
            SetFlag(AuxFlags[AUX_SCAN_INDEX], DEVICE_IS_ATTACHED);

            for(timeout=0x00;timeout<=20;timeout++) // 600 ms time-out
            {
                *asPS2Struct[AUX_SCAN_INDEX].ps2ctrl=PS2_ReceiveMode;
                if(!bExtAUXTimeOutCheck(AUX_SCAN_INDEX))
                {
                    break;
                }
            }
                
            if(timeout>20)          // wait aa time-out
            {
                if(PS2_AUXTypeDebug)
                {
                    RamDebug(FF_ACK_AA_TimeOut);
                }
            }
            else
            {
                for(timeout=0x00;timeout<=10;timeout++)
                {
                    *asPS2Struct[AUX_SCAN_INDEX].ps2ctrl=PS2_ReceiveMode;
                    if(!bExtAUXTimeOutCheck(AUX_SCAN_INDEX))
                    {
                         break;
                    }
                }
                    
                if(timeout>10)          // wait 00 time-out
                {
                    SetFlag(AuxFlags[AUX_SCAN_INDEX], DEVICE_IS_KEYBOARD);
                    CheckNSetMainKBCHN(AUX_SCAN_INDEX);
                        
                    if(PS2_AUXTypeDebug)
                    {
                        RamDebug(FF_ACK_00_TimeOut);
                        RamDebug(PS2_AUXType_KB);
                    }        
                }
                else
                {
                    if(PS2_AUXTypeDebug)
                    {
                        RamDebug(FF_ACK_00_TimeDone);
                    }
                        
                    if(PS2IFAck==0x00)
                    {
                        ReadDeviceID();
                    }
                    else
                    {
                        SetFlag(AuxFlags[AUX_SCAN_INDEX], DEVICE_IS_KEYBOARD);
                        CheckNSetMainKBCHN(AUX_SCAN_INDEX);
                        if(PS2_AUXTypeDebug)
                        {
                            RamDebug(PS2_AUXType_KB);
                            RamDebug(PS2_KB_Strange);
                        } 
                    }
                }
            }
            SetFlag(AuxFlags[AUX_SCAN_INDEX], DEVICE_NEED_CONFIG);
            ScanAUXStepIndex = ScanAUXStep_Init + AUX_SCAN_INDEX;
        }
        else
        {
            if(PS2_AUXTypeDebug)
            {
                RamDebug(FF_ACK_FA_TimeOut);
            }  
            AUX_SCAN_INDEX_HS = 0x00;
            ScanAUXStepIndex = ScanAUXStep_InitFail + AUX_SCAN_INDEX;
        }
        PS2Deviceactive();
    } 
}

//----------------------------------------------------------------------------
// The polling aux device center
//----------------------------------------------------------------------------
void PollingAUXDeviceCenter(void)
{
    BYTE index;
 
    if(SKIP_AUX_SCAN_CUNT!=0x00)
    {
        SKIP_AUX_SCAN_CUNT--;
    }
    else
    {
        if(AUX_SCAN_INDEX_HS!=0xFF)
        {
            AUX_SCAN_INDEX_HS = 0xFF;
            AUX_SCAN_INDEX++;
            if(AUX_SCAN_INDEX>2)
            {
                AUX_SCAN_INDEX=0x00;
            }
            ScanAUXStepIndex = ScanAUXStep_ScanPort+AUX_SCAN_INDEX;
        }
        else
        {
            if(ECSendtoAUXFlag)
            {
                AUX_SCAN_TIMEOUT_CUNT++;
                if(AUX_SCAN_TIMEOUT_CUNT>=CheckDeviceTimeOut)
                {
                    AUX_SCAN_TIMEOUT_CUNT = 0x00;
                    AUX_SCAN_INDEX_HS = 0x00;
                    AuxFlags[AUX_SCAN_INDEX] = 0x00;
                    AuxFlags2[AUX_SCAN_INDEX] = 0x00;
                    PS2_ReTryCunt = 0x00;
                    SendtoAUXFlag = 0;	
                    ECSendtoAUXFlag = 0;

                    if(Main_MOUSE_CHN == (AUX_SCAN_INDEX+1))
                    {
                        Main_MOUSE_CHN = 0x00;
                        for(index=0x00;index<3;index++)
                        {
                            if(IsFlag1(AuxFlags[index], DEVICE_IS_MOUSE))
                            {
                                SetFlag(AuxFlags[index], DEVICE_IS_MAIN_MOUSE);
                                Main_MOUSE_CHN = index + 1;
                                break;
                            }
                        }   
                    }
                    
                    if(Main_KB_CHN == (AUX_SCAN_INDEX+1))
                    {
                        Main_KB_CHN = 0x00;
                        for(index=0x00;index<3;index++)
                        {
                            if(IsFlag1(AuxFlags[index], DEVICE_IS_KEYBOARD))
                            {
                                SetFlag(AuxFlags[index], DEVICE_IS_MAIN_KB);
                                Main_KB_CHN = index + 1;
                                break;
                            }
                        }
                    }
                    PS2Deviceactive();	
                }
            }
        }
    }
}

//----------------------------------------------------------------------------
// The function of update external keyboard device
//----------------------------------------------------------------------------
const BYTE code UpdateConfigAUXKB[] = {0xF5, 0xF4, 0xF3, 0xED};	// Command table

void UpdateExtKeyboard(BYTE portnum)
{   
	BYTE index;
    
    for(index=0x00; index<sizeof(UpdateConfigAUXKB); index++)	
    {
        if(IS_BIT_SET(AuxFlags_KB_CMD[AUX_SCAN_INDEX], index))
        {
            if(!Send2PortNWait(portnum,UpdateConfigAUXKB[index],1))
            {
                if(UpdateConfigAUXKB[index]==0xF3)
                {
                    Send2PortNWait(portnum,Save_Typematic,1);
                }
                else if(UpdateConfigAUXKB[index]==0xED)
                {
                    Send2PortNWait(portnum,(Led_Data&0x07),1);
                }
                CLR_BIT(AuxFlags_KB_CMD[AUX_SCAN_INDEX], index);
            }
        }
    }
}

//----------------------------------------------------------------------------
// The function of update aux keyboard device
//----------------------------------------------------------------------------
void UpdateAUXKBDevice(void)
{
    UpdateExtKeyboard(AUX_SCAN_INDEX);
    AUX_SCAN_INDEX_HS = 0x00;
}


//----------------------------------------------------------------------------
// The function of init, AUX device
//----------------------------------------------------------------------------
void InitAUXDevice(void)
{
    if(IsFlag1(AuxFlags[AUX_SCAN_INDEX], DEVICE_IS_KEYBOARD))   // device is keyboard
    {
        if(KeyboardDriverIn)
        {
            if(AuxFlags_KB_CMD[AUX_SCAN_INDEX]==0x00)
            {
                ConfigExtKeyboard(AUX_SCAN_INDEX);
            }
            else
            {
                UpdateExtKeyboard(AUX_SCAN_INDEX);
            }
            ClearFlag(AuxFlags[AUX_SCAN_INDEX], DEVICE_NEED_CONFIG);
        }
        else
        {
            
        }
    }
    else                                                // device is mouse
    {
        if(MouseDriverIn)
        {
	        if(MULPX_Multiplex)
	        {
                AUXData_To_Host_nWait(0xAA,AUX_SCAN_INDEX);
                AUXData_To_Host_nWait(0x00,AUX_SCAN_INDEX);
                ClearFlag(AuxFlags[AUX_SCAN_INDEX], DEVICE_NEED_CONFIG);
	        }
            else
            {            
                if(vSetIntellimouse(MS_DRIVER_TYPE,AUX_SCAN_INDEX))
                {
                    if(ReadMouseID(AUX_SCAN_INDEX)==MS_DRIVER_TYPE)
                    {
                        if(ConfigExtMouse(AUX_SCAN_INDEX))
                        {
                            ClearFlag(AuxFlags[AUX_SCAN_INDEX], DEVICE_NEED_CONFIG);
                        }
                        else
                        {
                            AuxFlags[AUX_SCAN_INDEX] = 0x00;        // device error
                        }
                    }
                    else
                    {
                        SetFlag(AuxFlags[AUX_SCAN_INDEX],DEVICE_MS_DRIVER_NO_MATCH);
                        ClearFlag(AuxFlags[AUX_SCAN_INDEX], DEVICE_NEED_CONFIG);
                    }
                }
                else
                {
                    AuxFlags[AUX_SCAN_INDEX] = 0x00;                // device error
                }
            }
        }
        else
        {
	        if(MULPX_Multiplex)
	        {
                AUXData_To_Host_nWait(0xAA,AUX_SCAN_INDEX);
                AUXData_To_Host_nWait(0x00,AUX_SCAN_INDEX);
                ClearFlag(AuxFlags[AUX_SCAN_INDEX], DEVICE_NEED_CONFIG);
	        }
        }
    }
    PS2Deviceactive();
    AUX_SCAN_INDEX_HS = 0x00;
}

//----------------------------------------------------------------------------
// The function of checking on/off TP.
// Just for "TouchPad_only" selection set.
//----------------------------------------------------------------------------
void TPOnly_CheckOnOffTP(void)
{
    BYTE index;
    
	for(index=0x00;index<3;index++)
	{
		if(IsFlag1(AuxFlags2[index], DEVICE_NEED_DISABLED))
		{
    		if(!Send2PortNWait(index,0xF5,1))
    		{
        		AuxFlags2[index] = 0x00;
            	SetFlag(AuxFlags2[index],DEVICE_IS_DISABLED);
    		}
            else
            {
                PS2_ReTryCunt++;
                if(PS2_ReTryCunt>PS2_OnOffTimeOutCUNT)
                {
                    PS2_ReTryCunt = 0x00;
                    AuxFlags2[index] = 0x00;
                    SetFlag(AuxFlags2[index],DEVICE_DISABLE_TIME_OUT);
                }
            }
		}
		else if(IsFlag1(AuxFlags2[index], DEVICE_NEED_ENABLED))
		{
    		if(!Send2PortNWait(index,0xF4,1))
    		{
        		AuxFlags2[index] = 0x00;
            	SetFlag(AuxFlags2[index],DEVICE_IS_ENABLED);
				*asPS2Struct[index].ps2ctrl=PS2_ReceiveMode;
    		}
            else
            {
                PS2_ReTryCunt++;
                if(PS2_ReTryCunt>PS2_OnOffTimeOutCUNT)
                {
                    PS2_ReTryCunt = 0x00;
                    AuxFlags2[index] = 0x00;
                    SetFlag(AuxFlags2[index],DEVICE_DISABLE_TIME_OUT);
                }
            }            
		}
	}    
}

//----------------------------------------------------------------------------
// The function of scaning AUX device step
//----------------------------------------------------------------------------
void ScanAUXDeviceStep(void)
{
#if TouchPad_only  
    if(IsFlag1(PSSTS1, TDS)&&IS_BIT_SET(ISR2,4))	// Transaction done  
    {
        ISR_Aux_Port1();
    }
    if(IsFlag1(PSSTS2, TDS)&&IS_BIT_SET(ISR2,3))	// Transaction done 
    {   
	    ISR_Aux_Port2();
    }	
    if(IsFlag1(PSSTS3, TDS)&&IS_BIT_SET(ISR2,2))	// Transaction done 
    {
	    ISR_Aux_Port3();
    }   
    TPOnly_CheckOnOffTP();
    
#else
    if(StartENAUXDevice)
    {
        StartENAUXDevice = 0;
        EnableAUXDevice();
        if(IsFlag1(PSSTS1, SS)||IsFlag1(PSSTS2, SS)||IsFlag1(PSSTS3, SS))
        {
            AuxScanWDT++;
            if(AuxScanWDT>250)      // 2.5 sec
            {
                AuxScanWDT = 0x00;
                ScanAUXPortX();
            }
        }
        else
        {
            AuxScanWDT = 0x00;
        }
    }
        
    if(IsFlag1(PSSTS1, TDS)&&IS_BIT_SET(ISR2,4))	// Transaction done  
	{
		ISR_Aux_Port1();
	}
    if(IsFlag1(PSSTS2, TDS)&&IS_BIT_SET(ISR2,3))	// Transaction done 
	{   
		ISR_Aux_Port2();
	}	
    if(IsFlag1(PSSTS3, TDS)&&IS_BIT_SET(ISR2,2))	// Transaction done 
	{
		ISR_Aux_Port3();
	}
    if((Service != 0x00)||(Service1 != 0x00))
    {
        return;
    }
    
    if(IsFlag1(PSSTS1, SS)||IsFlag1(PSSTS2, SS)||IsFlag1(PSSTS3, SS))
    {
        return;
    }

    if(StartScanAUXDevice)
    {
        StartScanAUXDevice = 0;
        PollingAUXDeviceCenter();
    }

    if(SKIP_AUX_SCAN_CUNT!=0x00)
    {
        return;
    }

    if(MSPendingRXCount==0x33)
    {
        return;
    }
       
    switch(ScanAUXStepIndex)
    {
        case ScanAUXStep_ScanPort0:   
        case ScanAUXStep_ScanPort1:
        case ScanAUXStep_ScanPort2:
            if(AUXInactiveCounter==0x00)
	        {
                ScanAUXPortX();
            }
            break;      
            
        case ScanAUXStep_CheckType0:
        case ScanAUXStep_CheckType1:
        case ScanAUXStep_CheckType2:
            CheckAUXDeviceType();
            break;
            
        case ScanAUXStep_Init0:
        case ScanAUXStep_Init1:
        case ScanAUXStep_Init2:
            InitAUXDevice();
            break;

        case ScanAUXStep_UpdateKB0:
        case ScanAUXStep_UpdateKB1:
        case ScanAUXStep_UpdateKB2:
            UpdateAUXKBDevice();
            break;

        case ScanAUXStep_CheckOK1:
        case ScanAUXStep_CheckOK2:
        case ScanAUXStep_CheckOK3:
            
        case ScanAUXStep_InitFail0:
        case ScanAUXStep_InitFail1:
        case ScanAUXStep_InitFail2:            
       
        case ScanAUXStep_Skip0:
        case ScanAUXStep_Skip1:
        case ScanAUXStep_Skip2:
            
        case ScanAUXStep_WaitPort0:
        case ScanAUXStep_WaitPort1:
        case ScanAUXStep_WaitPort2:
            
        default:
            break;
    }   
#endif    
}

//----------------------------------------------------------------------------
// The function of checking AUX device
//----------------------------------------------------------------------------
void ScanAUXPortX(void)
{
    if(AUX_SCAN_INDEX_HS==0xFF)
    {
        if(AUX_SCAN_INDEX==0x00)
        {
            if((PS2_Port0_CLK_CtrlReg&0xFB)!=0x00 || (PS2_Port0_Data_CtrlReg&0xFB)!=0x00)
            {
                 AUX_SCAN_INDEX_HS = 0x00;
            }
        }
        else if(AUX_SCAN_INDEX==0x01)
        {
            if((PS2_Port1_CLK_CtrlReg&0xFB)!=0x00 || (PS2_Port1_Data_CtrlReg&0xFB)!=0x00)
            {
                AUX_SCAN_INDEX_HS = 0x00;
            }
        }
        else if(AUX_SCAN_INDEX==0x02)
        {
            if((PS2_Port2_CLK_CtrlReg&0xFB)!=0x00 || (PS2_Port2_Data_CtrlReg&0xFB)!=0x00)
            {
                 AUX_SCAN_INDEX_HS = 0x00;
            }
        }
            
        if(AUX_SCAN_INDEX_HS == 0x00)
        {
            ScanAUXStepIndex = ScanAUXStep_Skip + AUX_SCAN_INDEX;
        }
        else
        {
            AUX_SCAN_TIMEOUT_CUNT = 0x00;
            if(IsFlag1(AuxFlags2[AUX_SCAN_INDEX],DEVICE_NEED_DISABLED))
            {
                 ECSend2Port(AUX_SCAN_INDEX, 0xF5);              
            }
            else if(IsFlag1(AuxFlags2[AUX_SCAN_INDEX],DEVICE_NEED_ENABLED))
            {
                 ECSend2Port(AUX_SCAN_INDEX, 0xF4);              
            }
            else
            {
                ECSend2Port(AUX_SCAN_INDEX, 0xF1);
            }
            ScanAUXStepIndex = ScanAUXStep_WaitPort + AUX_SCAN_INDEX;
        }
    }
}

//----------------------------------------------------------------------------
// The function of enabling AUX device
//----------------------------------------------------------------------------
void EnableAUXDevice(void)
{
	if(AUXInactiveCounter!=0x00)
	{
		AUXInactiveCounter--;
		if(AUXInactiveCounter==0x00)
		{
			PS2Deviceactive();
		}
	}
}

//----------------------------------------------------------------------------
// The function of starting to check AUX device
//----------------------------------------------------------------------------
BYTE bExtAUXTimeOutCheck(BYTE channel)
{
    BYTE result;
    result = 0x01;      // pre-set resutl is fail
    
    TR1 = 0;            // Disable timer 1
	ET1 = 0;			// Disable timer1 interrupt
	TH1 = 0xA6;			// set timer1 counter 30 ms 
	TL1 = 0x21;			// set timer1 counter 30 ms
    TF1 = 0;			// clear overflow flag
	TR1 = 1;			// enable timer1

	do
	{                   // Wait PS2 transaction Done Status
        if(IsFlag1(*asPS2Struct[channel].ps2status, TDS))
        {
            *asPS2Struct[channel].ps2ctrl=PS2_InhibitMode;
            PS2IFAck=*asPS2Struct[channel].ps2data;
            result = 0x00;
            break;
        }
	}while(!TF1);					// waitting for overflow flag

    *asPS2Struct[channel].ps2ctrl=PS2_InhibitMode;
    TR1 = 0;			            // disable timer1
    TF1 = 0;						// clear overflow flag
    ET1 = 1;	
	return(result);
}

//----------------------------------------------------------------------------
// The function of setting external keyboard device
//----------------------------------------------------------------------------
const BYTE code ConfigAUXKB[] = {0xF3, 0xED, 0xF4 };	// Command table

void ConfigExtKeyboard(BYTE portnum)
{   
	BYTE index;

    for(index=0x00; index<sizeof(ConfigAUXKB); index++)	
    {
        if(!Send2PortNWait(portnum,ConfigAUXKB[index],1))
        {
            if(ConfigAUXKB[index]==0xF3)
            {
                Send2PortNWait(portnum,Save_Typematic,1);
            }
            else if(ConfigAUXKB[index]==0xED)
            {
                Send2PortNWait(portnum,(Led_Data&0x07),1);
            }
        }
    }
}

//----------------------------------------------------------------------------
// The function of setting Resolution
//  return : 1 OK, 0 fail
//----------------------------------------------------------------------------
BYTE EnableMouse(BYTE port)
{
    BYTE resault;
    if(!Send2PortNWait(port,0xF4,1))
    {
        resault = 1;
    }
    else
    {
        resault = 0;
    }
    return(resault);
}

//----------------------------------------------------------------------------
// The function of setting Resolution
//  return : 1 OK, 0 fail
//----------------------------------------------------------------------------
BYTE SetResolution(BYTE port, BYTE resolution)
{
    BYTE resault;
    if(!Send2PortNWait(port,0xE8,1))
    {
        if(!Send2PortNWait(port,resolution,1))
        {
            resault = 1;
        }
        else
        {
            resault = 0;
        }
    }
    else
    {
        resault = 0;
    }
    return(resault);
}

//----------------------------------------------------------------------------
// The function of setting samplerate
//  return : 1 OK, 0 fail
//----------------------------------------------------------------------------
BYTE Setsamplerate(BYTE port, BYTE samplerate)
{
    BYTE resault;
    if(!Send2PortNWait(port,0xF3,1))
    {
        if(!Send2PortNWait(port,samplerate,1))
        {
            resault = 1;
        }
        else
        {
            resault = 0;
        }
    }
    else
    {
        resault = 0;
    }
    return(resault);
}

//----------------------------------------------------------------------------
// The Microsoft Intellimouse
//----------------------------------------------------------------------------
const BYTE code Intellimouse_5Byte[3] = 
{	
    200,    200,    80
};	

const BYTE code Intellimouse_4Byte[3] = 
{	
    200,    100,    80
};

BYTE vSetIntellimouse(BYTE DriverType, BYTE port)
{
    BYTE index;
    BYTE result;
    result = 0x01;   // pr-set OK
    
    if(DriverType==3)
    {
        for(index=0x00;index<sizeof(Intellimouse_4Byte);index++)
        {
            if(Setsamplerate(port, Intellimouse_4Byte[index])==0x00)
            {
                result = 0x00;
                break;
            }
        }
    }
    else if(DriverType==4)
    {
        for(index=0x00;index<sizeof(Intellimouse_5Byte);index++)
        {
            if(Setsamplerate(port, Intellimouse_5Byte[index])==0x00)
            {
                result = 0x00;
                break;
            }
        }
    }
    return(result);
}

BYTE ReadMouseID(BYTE port)
{
    if(Send2PortNWait(port,0xF2,2)==0x00)
    {
        return(PS2IFAck);
    }
    return(0xFF);
}

//----------------------------------------------------------------------------
// The function of setting external mouse device
//----------------------------------------------------------------------------
BYTE ConfigExtMouse(BYTE portnum)
{  
    BYTE result;
    result = 0x00;
    
    if(SetResolution(portnum,TPResolution))
    {
        if(EnableMouse(portnum))
        {
            result = 0x01;
        }
    }
    return(result);
}

//----------------------------------------------------------------------------
// The function of checking AUX Multiplexed modes
//----------------------------------------------------------------------------
BYTE vCheckAuxMux(void)
{
	BYTE current,data_byte,index;
 	current = 1;
    data_byte = KBHIData;           // For Dos mouse
	if (KBHIData == 0xF0)		 	// Data from Host is F0h 
 	{  
       	MULPX_Mx_state2 = 1;
      	MULPX_Mx_state3 = 0;
      	current = 0;    			// Do NOT clear State2 and State3 
      	data_byte = 0xF0;
	}
  	else if (KBHIData == 0x56)		// Data from Host is 56h
	{ 
     	if (MULPX_Mx_state2) 		// Current is State2
		{ 
         	MULPX_Mx_state2 = 0;
         	MULPX_Mx_state3 = 1;
        	current = 0;
			data_byte = 0x56;
     	}
	}
	else if (KBHIData == 0xA4)		// Data from Host is A4h 
  	{  
   		if (MULPX_Mx_state3) 		// Current is State3 
		{  
       		MULPX_Multiplex = 1;	// Enable Activate Multiplex mode 
       		SKIP_AUX_SCAN_CUNT = 250;
          	data_byte = 0x11; 		// Return Version number 
     	}
	}
 	else if (KBHIData == 0xA5)		// Data from Host is A5h. 
	{   
   		if (MULPX_Mx_state3) 		// Current is State3
		{  
          	MULPX_Multiplex = 0;	// Reveret to Legacy Mode 
           	data_byte = 0x11;       // Return version number 
      	}
  	} 

 	if (current) 
	{
      	MULPX_Mx_state2 = 0;		// Clear State2 and State3 
		MULPX_Mx_state3 = 0;
   	}
	return(data_byte);
}

// ----------------------------------------------------------------------------
// The function of disable ps2 channel x
// Input :  portnum --> ps2 channel
//          active  --> 1 : On, 0 : Off
//
//  Note : If want to enable mouse device. Please ensrue "MouseDriverIn" is set.
// ----------------------------------------------------------------------------
void OnOffPS2Channel_X(BYTE portnum, BYTE active)
{
    if(portnum==0x00)
    {
        if((PS2_Port0_CLK_CtrlReg&0xFB)!=0x00 || (PS2_Port0_Data_CtrlReg&0xFB)!=0x00)
        {
            return;// If this channel isn't PS2 ALT function. return;  
        }                  
    }
    else if(portnum==0x01)
    {
	    if((PS2_Port1_CLK_CtrlReg&0xFB)!=0x00 || (PS2_Port1_Data_CtrlReg&0xFB)!=0x00)
        {
            return;// If this channel isn't PS2 ALT function. return;  
        }    
    }
    else if(portnum==0x02)
    {
	    if((PS2_Port2_CLK_CtrlReg&0xFB)!=0x00 || (PS2_Port2_Data_CtrlReg&0xFB)!=0x00)
        {
            return;// If this channel isn't PS2 ALT function. return;  
        }     
    }

    if(active == 0x00)
    {
        SetFlag(AuxFlags2[portnum], DEVICE_NEED_DISABLED);
    }
    else if(active == 0x01)
    {
        SetFlag(AuxFlags2[portnum], DEVICE_NEED_ENABLED);
    }
    PS2_ReTryCunt = 0x00;
}

// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
void ISR_Aux_Port1(void)
{
    CLEAR_BIT(IER2,4);		// Disable PS2 interrupt 0
    SET_BIT(ISR2,4);		// Write to clear pending interrupt
    WNCKR = 0x00;               // Delay 15.26 us
    WNCKR = 0x00;               // Delay 15.26 us
    PSCTL1 = PS2_InhibitMode;   // Inhibit clock pin1
    PSCTL2 = PS2_InhibitMode;   // Inhibit clock pin2
    PSCTL3 = PS2_InhibitMode;   // Inhibit clock pin3

    PS2_IRQ_Channel = 0;    // Set PS2 0 service flag
    F_Service_PS2_IRQ = 1;  // Set main service flag
    PS2StartBit=0;	    // clear start bit flag
    PS2PortxData[0]=PSDAT1;
}

// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
void ISR_Aux_Port2(void)
{
    CLEAR_BIT(IER2,3);	    // Disable PS2 interrupt 1
    SET_BIT(ISR2,3);	    // Write to clear pending interrupt
    WNCKR = 0x00;               // Delay 15.26 us
    WNCKR = 0x00;               // Delay 15.26 us
    PSCTL1 = PS2_InhibitMode;   // Inhibit clock pin1
    PSCTL2 = PS2_InhibitMode;   // Inhibit clock pin2
    PSCTL3 = PS2_InhibitMode;   // Inhibit clock pin3

    PS2_IRQ_Channel = 1;    // Set PS2 1 service flag
    F_Service_PS2_IRQ = 1;  // Set main service flag
    PS2StartBit=0;	    // clear start bit flag	
    PS2PortxData[1]=PSDAT2;
}

// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
void ISR_Aux_Port3(void)
{
    CLEAR_BIT(IER2,2);	    // Disable PS2 interrupt 2
    SET_BIT(ISR2,2);	    // Write to clear pending interrupt
    WNCKR = 0x00;               // Delay 15.26 us
    WNCKR = 0x00;               // Delay 15.26 us
    PSCTL1 = PS2_InhibitMode;   // Inhibit clock pin1
    PSCTL2 = PS2_InhibitMode;   // Inhibit clock pin2
    PSCTL3 = PS2_InhibitMode;   // Inhibit clock pin3

    PS2_IRQ_Channel = 2;    // Set PS2 2 service flag    
    F_Service_PS2_IRQ = 1;  // Set main service flag
    PS2StartBit=0;	    // clear start bit flag
    PS2PortxData[2]=PSDAT3;		
} 

// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
void KBCmdAckByteCunt(BYTE ackcunt)
{
    Main_KB_ACK_CUNT = ackcunt;
}

// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
void SetOtherKBNeedUpdataFlag(BYTE KBCmd)
{
    BYTE index;
    
    for(index=0x00;index<3;index++)
    {
        if(IsFlag1(AuxFlags[index], DEVICE_IS_KEYBOARD))
        {
            if(Main_KB_CHN != (index+1))
            {
                if(IsFlag0(AuxFlags[index], DEVICE_NEED_CONFIG))
                {
                    if(KBCmd==0xED)
                    {
                        AuxFlags_KB_CMD[index] |= KB_CMD_0xED;
                    }
                    else if(KBCmd==0xF3)
                    {
                        AuxFlags_KB_CMD[index] |= KB_CMD_0xF3;
                    }
                    else if(KBCmd==0xF4)
                    {
                        AuxFlags_KB_CMD[index] |= KB_CMD_0xF4;
                    }
                    else if(KBCmd==0xF5)
                    {
                        AuxFlags_KB_CMD[index] |= KB_CMD_0xF5;
                    }      
                }
            }
        }
    } 
}

// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
void SetStartScanAUXFlag(void)
{
    StartScanAUXDevice = 1;
    StartENAUXDevice = 1;
}

// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
void AUXData_To_Host_nWait(BYTE data_byte, BYTE portnum)
{
    AUXPortNum = portnum;
    SendFromAux(data_byte);

    TR1 = 0;                 	// Disable timer1
    ET1 = 0;                  	// Disable timer1 interrupt
    TH1 = (0xFFFF-19942)>>8;    // Set timer1 counter 26ms
    TL1 = (0xFFFF-19942);       // Set timer1 counter 26ms
    TF1 = 0;                  	// Clear overflow flag
    TR1 = 1;                 	// Enable timer1

	while (!TF1)
	{ 
	    if(IsFlag0(KBHISR, OBF))
        {
            break;
        }   
        if(IsFlag1(KBHISR, IBF))
        {
            break;
        }  
	}
}

// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
void ReSendPS2PendingData(void)
{
    if(MSPendingRXCount==0x33)
    {
        F_Service_Send_PS2 = 1;
    }
}

// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
void CheckNSetMainKBCHN(BYTE PortNum)
{
    if(Main_KB_CHN == 0x00)
    {
        SetFlag(AuxFlags[PortNum], DEVICE_IS_MAIN_KB);
        Main_KB_CHN = PortNum+1;
    }
}

// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
void CheckNSetMainMSCHN(BYTE PortNum)
{
    if(Main_MOUSE_CHN == 0x00)
    {
        SetFlag(AuxFlags[PortNum], DEVICE_IS_MAIN_MOUSE);
        Main_MOUSE_CHN = PortNum+1;
    }
}

// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
void ResetMSPendingData(void)
{
    MSPendingRXCount = 0x88;
    F_Service_Send_PS2 = 0;
}

// ----------------------------------------------------------------------------
// The function of checking ps2 port busy
//  Return : 1: The port is busy
//           0: no busy   
// ----------------------------------------------------------------------------
BYTE CheckMousePS2IFBusy(BYTE PortNum)
{
    BYTE result,auxstatus;

    auxstatus = *asPS2Struct[PortNum].ps2status;
    if(IsFlag1(auxstatus,TDS)||
        IsFlag1(auxstatus,SS)||
        ((F_Service_PS2_IRQ == 1)&&(PS2_IRQ_Channel == PortNum))||
        ((PS2StartBit == 1)&&(PS2_SSIRQ_Channel == PortNum)))
    {
        result = 0x01;
    }
    else
    {
        *asPS2Struct[PortNum].ps2ctrl = PS2_InhibitMode;
        result = 0x00;
    }

    return(result);
}


// ----------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------
void KBCmdCheckMouseBusy(BYTE KBCmd)
{
    BYTE index;
    
    if(MouseDriverIn)
    {
        for(index=0x00;index<3;index++)
        {
            if(index == (Main_KB_CHN-1))
            {
                continue;
            }
            
            if(IsFlag1(AuxFlags[index], DEVICE_IS_MOUSE))
            {
                if(CheckMousePS2IFBusy(index)==1)
                {
                    if(ECSendtoAUXFlag==0)
                    {
                        AUXInterfaceBusy = 1;
                        PS2_KBCmd_BK = KBCmd;
                    }
                    break;
                }
                        
                if(MSPendingRXCount==0x33)
                {
                    AUXInterfaceBusy = 1;
                    PS2_KBCmd_BK = KBCmd;
                    break;
                }
            }
        }
    }
}

//----------------------------------------------------------------------------
// PS2 interface enable/disable OEM function call
//----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// Disable port 0 ( send 0xF5 command to device)
// ----------------------------------------------------------------------------
void DisablePS2Port_0(void)
{
    OnOffPS2Channel_X(0,0);
}

// ----------------------------------------------------------------------------
// Disable port 1 ( send 0xF5 command to device)
// ----------------------------------------------------------------------------
void DisablePS2Port_1(void)
{
    OnOffPS2Channel_X(1,0);
}

// ----------------------------------------------------------------------------
// Disable port 2 ( send 0xF5 command to device) 
// ----------------------------------------------------------------------------
void DisablePS2Port_2(void)
{
    OnOffPS2Channel_X(2,0);
}

// ----------------------------------------------------------------------------
// Enable port 0 ( send 0xF4 command to device) 
// ----------------------------------------------------------------------------
void EnablePS2Port_0(void)
{
    OnOffPS2Channel_X(0,1);
}

// ----------------------------------------------------------------------------
// Enable port 1 ( send 0xF4 command to device)  
// ----------------------------------------------------------------------------
void EnablePS2Port_1(void)
{
    OnOffPS2Channel_X(1,1);
}

// ----------------------------------------------------------------------------
// Enable port 2 ( send 0xF4 command to device)  
// ----------------------------------------------------------------------------
void EnablePS2Port_2(void)
{
    OnOffPS2Channel_X(2,1);
}

// ----------------------------------------------------------------------------
// Power off clock and data pins of all supported channel
// ----------------------------------------------------------------------------
void PS2_PowerOff(void)
{
    BYTE index;
    ResetMSPendingData();
    ClaerPS2IFPendingEvent();

    AUX_SCAN_TIMEOUT_CUNT = 0x00;
    AUX_SCAN_INDEX_HS = 0x00;
    PS2_ReTryCunt = 0x00;
    SendtoAUXFlag = 0;	
    ECSendtoAUXFlag = 0;
    AUXInactiveCounter = 0;
    StartScanAUXDevice = 0;
    StartENAUXDevice = 0;
    AUXInterfaceBusy = 0;

    for(index=0x00;index<3;index++)
    {
        AuxFlags[index] = 0x00;
        AuxFlags2[index] = 0x00;
    }
    
    if((PS2_Port0_CLK_CtrlReg&0xFB)==0x00 && (PS2_Port0_Data_CtrlReg&0xFB)==0x00)
    {
        PS2_Port0_CLK_CtrlReg = OUTPUT+PULL_DN;
        PS2_Port0_Data_CtrlReg = OUTPUT+PULL_DN;
        SetFlag(AuxFlags2[0], DEVICE_POWER_DOWN);
    }
           
    if((PS2_Port1_CLK_CtrlReg&0xFB)==0x00 && (PS2_Port1_Data_CtrlReg&0xFB)==0x00)
    {
        PS2_Port1_CLK_CtrlReg = OUTPUT+PULL_DN;
        PS2_Port1_Data_CtrlReg = OUTPUT+PULL_DN;
        SetFlag(AuxFlags2[1], DEVICE_POWER_DOWN);
    }
    
    if((PS2_Port2_CLK_CtrlReg&0xFB)==0x00 && (PS2_Port2_Data_CtrlReg&0xFB)==0x00)
    {
        PS2_Port2_CLK_CtrlReg = OUTPUT+PULL_DN;
        PS2_Port2_Data_CtrlReg = OUTPUT+PULL_DN;
        SetFlag(AuxFlags2[2], DEVICE_POWER_DOWN);
    }
}

// ----------------------------------------------------------------------------
// Power on clock and data pins of all supported channel
// ----------------------------------------------------------------------------
void PS2_PowerOn(void)
{
    vPS2DeviceInactive();
    if(IsFlag1(AuxFlags2[0], DEVICE_POWER_DOWN))
    {
        ClearFlag(AuxFlags2[0], DEVICE_POWER_DOWN);
        PS2_Port0_CLK_CtrlReg = PALT;
        PS2_Port0_Data_CtrlReg = PALT;
    }
    
    if(IsFlag1(AuxFlags2[1], DEVICE_POWER_DOWN))
    {
        ClearFlag(AuxFlags2[1], DEVICE_POWER_DOWN);
        PS2_Port1_CLK_CtrlReg = PALT;
        PS2_Port1_Data_CtrlReg = PALT;
    }

    if(IsFlag1(AuxFlags2[2], DEVICE_POWER_DOWN))
    {
        ClearFlag(AuxFlags2[2], DEVICE_POWER_DOWN);
        PS2_Port2_CLK_CtrlReg = PALT;
        PS2_Port2_Data_CtrlReg = PALT;
    }    
}

/*-----------------------------------------------------------------------------
 * End
 *---------------------------------------------------------------------------*/

