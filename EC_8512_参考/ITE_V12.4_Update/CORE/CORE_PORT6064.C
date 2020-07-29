/*-----------------------------------------------------------------------------
 * TITLE: CORE_PORT6064.C - Standard 8042 Keyboard Controller Commands
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
#define CONTROL_A20_WAY1        0
#define CONTROL_A20_WAY2        0
#define CONTROL_A20_WAY3        1

/*
//-----------------------------------------------------------------------------
 * Keyboard Controller Commands Related to PC Systems  (Port 64h)
 * Command Description
//-----------------------------------------------------------------------------
 * Cmd_00-7F Read/Write KBC RAM Control Bytes (16 Bytes)
 * Cmd_00-1F
 * Cmd_20-3F Read 8042 Command Byte: current 8042 command byte is placed
             in port 60h.
 * Cmd_40-5F
 * Cmd_60-7F Write 8042 Command Byte: next data byte written to port 60h is
             placed in 8042 command register.  format:
            |7|6|5|4|3|2|1|0|  8042 Command Byte
             | | | | | | | +---- 1=enable output register full interrupt
             | | | | | | +----- should be 0
             | | | | | +------ 1=set status register system, 0=clear
             | | | | +------- 1=override keyboard inhibit, 0=allow inhibit
             | | | +-------- disable keyboard I/O by driving clock line low
             | | +--------- disable auxiliary device, drives clock line low
             | +---------- IBM scancode translation 0=AT, 1=PC/XT
             +----------- reserved, should be 0

 * Cmd_90 Synaptics Spec.Active PS/2 Multiplexing
 * Cmd_91 Synaptics Spec.Active PS/2 Multiplexing
 * Cmd_92 Synaptics Spec.Active PS/2 Multiplexing
 * Cmd_93 Synaptics Spec.Active PS/2 Multiplexing

 * Cmd_A1 Get version number: Returns kernel firmware version number.
 * Cmd_A4 Password Installed Test: returned data can be read from port 60h;
          FA=password installed, F1=no password
 * Cmd_A5 Load Security: bytes written to port 60h will be read
          until a null (0) is found.
 * Cmd_A6 Enable Security: works only if a password is already loaded
 * Cmd_A7 Disable Auxiliary Interface: sets Bit 5 of command register
          stopping auxiliary I/O by driving the clock line low
 * Cmd_A8 Enable Auxiliary Interface: clears Bit 5 of command register
 * Cmd_A9 Auxiliary Interface Test: clock and data lines are tested;
          results placed at port 60h are listed below:
            00  no error
            01  keyboard clock line is stuck low
            02  keyboard clock line is stuck high
            03  keyboard data line is stuck low
            04  keyboard data line is stuck high
 * Cmd_AA Self Test: This commands the controller to perform internal
          diagnosic tests. A hex 55 is plased in the output buffer
          if no errors are detected.
 * Cmd_AB Keyboard Interface Test:  clock and data lines are tested;
          results placed at port 60h are listed above with command A9
 * Cmd_AC Diagnostic Dump: sends 16 bytes of 8042's RAM, current input
          port state, current output port state and 8042 program status
          word to port 60h in scan-code format.
 * Cmd_AD Disable Keyboard Interface: sets Bit 4 of command register
          stopping keyboard I/O by driving the clock line low
 * Cmd_AE Enable Keyboard Interface: clears Bit 4 of command register
          enabling keyboard interface.

 * Cmd_C0 Read Input Port: data is read from its input port (which is
          inaccessible to the data bus) and written to output register
          at port 60h;  output register should be empty before call.
             |7|6|5|4|3-0|  8042 Input Port
               | | | | +---- undefined
               | | | +----- 1=enable 2nd 256K of motherboard RAM, 0=disable
               | | +------ 1=manufacturing jumper not installed, 0=installed
               | +------- 1=primary display is MDA, 0=primary display is CGA
               +-------- 1=keyboard not inhibited, 0=keyboard inhibited
 * Cmd_C1 Poll Input Port Low Bits:
          Bits 0-3 of port 1 placed in status Bits 4-7
 * Cmd_C2 Poll Input Port High Bits:
          Bits 4-7 of port 1 placed in status Bits 4-7

 * Cmd_D0 Read Output Port: data is read from 8042 output port
          (which is inaccessible to the data bus) and placed in output
          register; the output register should be empty.
 * Cmd_D1 Write Output Port: next byte written to port 60h is placed in
          the 8042 output port (which is inaccessible to the data bus)
               |7|6|5|4|3|2|1|0|  8042 Output Port
                | | | | | | | +---- system reset line
                | | | | | | +----- gate A20
                | | | | +-------- undefined
                | | | +--------- output buffer full
                | | +---------- input buffer empty
                | +----------- keyboard clock (output)
                +------------ keyboard data (output)
 * Cmd_D2 Write Keyboard Output Register: on PS/2 systems the next data
          byte written to port 60h input register is written to port 60h
          output register as if initiated by a device; invokes interrupt
          if enabled
 * Cmd_D3 Write Auxiliary Output Register: on PS/2 systems the next data
          byte written to port 60h input register is written to port 60h
          output register as if initiated by a device; invokes interrupt
          if enabled
 * Cmd_D4 Write Auxiliary Device: on PS/2 systems the next data byte
          written to input register a port at 60h is sent to the
          auxiliary device

 * Cmd_E0 Read Test Inputs: 8042 reads its T0 and T1 inputs; data is
          placed in output register;  Bit 0 is T0, Bit 1 is T1:
               |1|0|  Test Input Port Bits
                | +---- keyboard clock
                +----- keyboard data

 * Cmd_Fx Pulse Output Port: Bits 0-3 of the 8042 output port can be
          pulsed low for 6 .s;  Bits 0-3 of command indicate which
          Bits should be pulsed; 0=pulse, 1=don't pulse; pulsing
          Bit 0 results in CPU reset since it is connected to system
          reset line.
//-----------------------------------------------------------------------------
*/
/*
//-----------------------------------------------------------------------------
 * Read/Write KBC RAM Control Bytes
//-----------------------------------------------------------------------------
*/
void Cmd_00_3F(void)    // Cmd_00_3F: Read KBC RAM Control Bytes
{
    switch( KBHICmd&0x1F )
    {
        case 0x00:  //Cmd_0x00 & Cmd_0x20
                    Data_To_Host( Ccb42 );
                    break;

        case 0x13:  //Cmd_0x13 & Cmd_0x33 Send Security Code On byte to host
                    Data_To_Host( Pass_On );
                    break;

        case 0x14:  //Cmd_0x14 & Cmd_0x34 Send Security Code Off byte to host
                    Data_To_Host( Pass_Off );
                    break;

        case 0x16:  //Cmd_0x16 & Cmd_0x36 Send Reject make code 1 to host
                    Data_To_Host( Pass_Make1 );
                    break;

        case 0x17:  //Cmd_0x17 & Cmd_0x37 Send Reject make code 2 to host
                    Data_To_Host( Pass_Make2 );
                    break;

        default:    Data_To_Host( 0x00 );
                    KBCUnProcessCnt++;  //Failed Monitor
                    break;
    }


}
//-----------------------------------------------------------------------------
void Cmd_40_7F(void)    // Cmd_40_7F: Write KBC RAM Control Bytes
{
    if ( SetGetPort60Data() )
    {
        KBHIStep = 0x00;    // Set Command Finished

        switch( KBHICmd&0x1F )
        {
            case 0x00:  //Cmd_0x40 & Cmd_0x60
                        Write_KCCB( KBHIData );
                        break;

            case 0x13:  //Cmd_0x53 & Cmd_0x73 Write Security Code On byte
                        Pass_On = KBHIData;     // Write the data.
                        break;

            case 0x14:  //Cmd_0x54 & Cmd_0x74 Write Security Code Off byte
                        Pass_Off = KBHIData;    // Write the data.
                        break;

            case 0x16:  //Cmd_0x56 & Cmd_0x76 Reject make code 1
                        Pass_Make1 = KBHIData;  // Write the data.
                        break;

            case 0x17:  //Cmd_0x57 & Cmd_0x77 Reject make code 2
                        Pass_Make2 = KBHIData;  // Write the data.
                        break;

            default:    KBCUnProcessCnt++;  //Failed Monitor

                        break;
        }
    }


}
//-----------------------------------------------------------------------------
void Write_KCCB(BYTE NewKCCB)
{
    Ccb42 = NewKCCB;    // Write the data.
    //Reconfigure variables/registers that are dependent on Command Byte value.
    //if (Ext_Cb0_PS2_AT)
    {
        if ( Ccb42_DISAB_AUX )  // For PS/2 style 8042 interface.
        {
            Cmd_A7();   // Disable aux device interface.
        }
        else
        {
            Cmd_A8();   // Enable Auxiliary Device Interface.
        }
    }

    if ( Ccb42_DISAB_KEY )
    {
        Cmd_AD();       // Disable keyboard interface.
    }
    else
    {
        Cmd_AE();       // Enable keyboard interface.
    }


    if ( Ccb42_SYS_FLAG )   // Put system flag bit in Status Reg.
    {
        SET_BIT( KBHISR, 2 );
    }
    else
    {
        CLEAR_BIT( KBHISR, 2 );
    }


}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Handle 64 port command 9x.
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Handle command 90 - MUX AUX port #0
// Notice: Synaptics Spec.Active PS/2 Multiplexing
//-----------------------------------------------------------------------------
void Cmd_90(void)
{
    if ( SetGetPort60Data() )
    {
        if(KBHIData==0xFF)
        {
            ScanAUXDevice();
        }
        
	    KBHISR = 0x24;				// set error bit and AUX bit, source bits is 00
        if ( Ccb42_INTR_AUX )		// If AUX IRQ bit of command is present	
        {
       	    SET_BIT(KBHICR,1);		// Enable IRQ
        }
 	    else
 	    {
 		    CLEAR_BIT(KBHICR,1);	// Disable IRQ
 	    }
	    vDelayXms(5);				// Emulate transmission delay times 
 	    KBHIMDOR = 0xFC;			// timeout error  
    }
}

//-----------------------------------------------------------------------------
// Handle command 91 - MUX AUX port #1 (TouchPad)
// Notice: Synaptics Spec.Active PS/2 Multiplexing
//-----------------------------------------------------------------------------
void Cmd_91(void)
{
    if ( SetGetPort60Data() )
    { 
	    if(IsFlag1(AuxFlags[0], DEVICE_IS_MOUSE))
	    {
		    vSendCmdtoMUX(0);		    // Send command to channel 0 mouse
	    }
	    else
	    {
		    KBHISR = 0x40;	
		    vDelayXms(5);				// Emulate transmission delay times 
		    KBHIMDOR = 0xFC;			// timeout error
	    }
    }
}

//-----------------------------------------------------------------------------
// Handle command 92 - MUX AUX port #2
// Notice: Synaptics Spec.Active PS/2 Multiplexing
//-----------------------------------------------------------------------------
void Cmd_92(void)
{
    if ( SetGetPort60Data() )
    {  
	    if(IsFlag1(AuxFlags[1], DEVICE_IS_MOUSE))
	    {
		    vSendCmdtoMUX(1);		    // Send command to channel 0 mouse
	    }
	    else
	    {
		    KBHISR = 0x80;	
		    vDelayXms(5);				// Emulate transmission delay times 
		    KBHIMDOR = 0xFC;			// timeout error
	    }
    }
}

//-----------------------------------------------------------------------------
// Handle command 93 - MUX AUX port #3
// Notice: Synaptics Spec.Active PS/2 Multiplexing
//-----------------------------------------------------------------------------
void Cmd_93(void)
{
    if ( SetGetPort60Data() )
    {
	    if(IsFlag1(AuxFlags[2], DEVICE_IS_MOUSE))
	    {
		    vSendCmdtoMUX(2);	        // Send command to channel 2 mouse
	    }
	    else
	    {
		    KBHISR = 0xC0;				// source bits is 11
		    vDelayXms(5);				// Emulate transmission delay times 
		    KBHIMDOR = 0xFC;
	    }
    }
}

//-----------------------------------------------------------------------------
// Handle 64 port command Ax.
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Handle KBC command A1 -
//-----------------------------------------------------------------------------
void Cmd_A1(void)
{


}

//-----------------------------------------------------------------------------
// Handle command A4 - Test Password Installed
// Return: data to send to HOST (FA or F1)
//-----------------------------------------------------------------------------
void Cmd_A4(void)
{
    if (Flag.PASS_READY)
    {
        KBC_DataToHost( 0xFA );     // Return FA if password is loaded
    }
    else
    {
        KBC_DataToHost( 0xF1 );     // Return F1 if password not loaded
    }
}

//-----------------------------------------------------------------------------
// Handle command A5 - Load Password
//-----------------------------------------------------------------------------
void Cmd_A5(void)
{
    if ( SetGetPort60MultiData() )
    {
        Tmp_Load = 0x00;
    }
    else
    {
        Cmd_A5Data();
    }

}
//-----------------------------------------------------------------------------
void Cmd_A5Data(void)
{
    if ( (Tmp_Load == 0) && (KBHIData == 0) )   // The first byte is NULL
    {
        Flag.PASS_READY = 0;    // clear password ready flag and exit
                                // Cmd_Byte has already been set to 0. So
                                // the code will NOT wait for next byte
    }
    else
    {
        if ( Tmp_Load < 8 )     // PASS_SIZE = 8
        {                       // If there is room in the buffer
            Pass_Buff[Tmp_Load] = KBHIData;   // Store scan code
            Tmp_Load++;         // Increment password buffer index
        }

        //Cmd_Byte = 0xA5;      // Set to keep waiting for next byte
        // The last byte (null terminated string) has been stored
        if ( KBHIData == 0 )
        {
            //Cmd_Byte = 0;     // Clear out the command byte.
            Flag.PASS_READY = 1; // Set password ready bit.
        }
    }

}

//-----------------------------------------------------------------------------
// Handle command A6 - Enable Password
//-----------------------------------------------------------------------
void Cmd_A6(void)
{
    if (Flag.PASS_READY)            // At this point, a password is loaded.
    {                               // Enable inhibit switch
        Gen_Info_PASS_ENABLE = 1;   // Enable password
        Pass_Buff_Idx = 0;          // Clear password buffer index
        Flag.SCAN_INH = 0;          // Clear internal keyboard inhibit
        CLEAR_BIT(KBHISR,4);
        KBC_DataToHost(Pass_On);
    }
    else
    {                               // At this point, a password is not loaded.
        KBC_DataToHost( 0x00 );
    }
}

//-----------------------------------------------------------------------------
// Handle command A7 - Disable Aux Device Interface
//-----------------------------------------------------------------------------
void Cmd_A7(void)
{
    Ccb42_DISAB_AUX = 1;  // Disable auxiliary device (mouse)
}

//-----------------------------------------------------------------------------
// Handle command A8 - Enable Auxiliary Device Interface
//-----------------------------------------------------------------------------
void Cmd_A8(void)
{
    Ccb42_DISAB_AUX = 0; // Enable aux device (mouse)
}

//-----------------------------------------------------------------------------
// Handle command A9 - Test Aux Device Interface
// Returns: test error code:
//             0 = no error
//             1 = Clock line stuck low
//             2 = Clock line stuck high
//             3 = Data line stuck low
//             4 = Data line stuck high
//-----------------------------------------------------------------------------
void Cmd_A9(void)
{
    MULPX_Multiplex = 0;	// Reveret to Legacy Mode 
    KBC_DataToHost( 0x00 );
}

//-----------------------------------------------------------------------------
// Handle command AA - Self Test
// Returns: 0x55 to signify that the test passed
//-----------------------------------------------------------------------------
void Cmd_AA(void)
{
    Ccb42_SYS_FLAG = 1;
    MULPX_Multiplex = 0;	// Reveret to Legacy Mode 
    KBC_DataToHost( 0x55 );
    GATEA20_ON();
}

//-----------------------------------------------------------------------------
// Handle command AB - Test Keyboard Interface
// Returns: test error code:
//             0 = no error
//             1 = Clock line stuck low
//             2 = Clock line stuck high
//             3 = Data line stuck low
//             4 = Data line stuck high
//-----------------------------------------------------------------------------
void Cmd_AB(void)
{
    KBC_DataToHost( 0x00 );
}

//-----------------------------------------------------------------------------
// Handle command AC - Diagnostic Dump
//-----------------------------------------------------------------------------
void Cmd_AC(void)
{
    //Kbd_Response = respCMD_AC;// Send multibyte sequence.
    //Tmp_Byte[0] = 0;   Tmp_Byte[0] will be used as the index for data.
}

//-----------------------------------------------------------------------------
// Handle command AD - Disable Keyboard Interface
//-----------------------------------------------------------------------------
void Cmd_AD(void)
{
    Ccb42_DISAB_KEY = 1;        // Disable auxiliary keyboard.
    Flag.SCAN_INH = 1;          // Inhibit scanner (internal keyboard).
    Lock_Scan();                // Lock scanner.
    //Load_Timer_B();           // Start inhibit delay timer.
}

//-----------------------------------------------------------------------------
// Handle command AE - Enable Keyboard Interface
//-----------------------------------------------------------------------------
void Cmd_AE(void)
{
    Ccb42_DISAB_KEY = 0;        // Enable auxiliary keyboard.
    Flag.SCAN_INH = 0;          // Enable scanner (internal keyboard).
}

//-----------------------------------------------------------------------------
// Handle 64 port command Cx.
// Return: Byte from input port.
//     Bit 7 = Keyboard not inhibited via switch.
//     Bit 6 = Monochrome adapter (0 = Color/Graphics adapter).
//     Bit 5 = Manufacturing jumper not installed.
//     Bit 4 = Enable 2nd 256K of system board RAM.
//     Bit 3 =
//     Bit 2 =
//     Bit 1 = Auxiliary data in line (PS/2 only).
//     Bit 0 = Keyboard data in line (PS/2 only).
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// Handle command C0 - Emulate reading the 8042 Input port and send data
//                     to the system
//-----------------------------------------------------------------------------
void Cmd_C0(void)
{
    // Just return the compatibility value for now. //return (PCIN | 0x1F);
    KBC_DataToHost( 0xBF );
}

//-----------------------------------------------------------------------------
// Handle 64 port command Dx.
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// Handle command D0 - Send 8042 Output port value to the system
//                      (emulates data since there¡¦s no real Output port)
// Read Output Port: data is read from 8042 output port
// (which is inaccessible to the data bus) and placed in output
// register; the output register should be empty.
//-----------------------------------------------------------------------------
void Cmd_D0(void)
{
    Emulate8042Port = 0xC1;

    if( STATE_GATEA20() )
    {
        Emulate8042Port |= 0x02;
    }

    KBC_DataToHost( Emulate8042Port );
}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Handle command D1 -
// Only set/reset GateA20 line based on the System Data bit1.
//
// Write Output Port: next byte written to port 60h is placed in
//          the 8042 output port (which is inaccessible to the data bus)
//               |7|6|5|4|3|2|1|0|  8042 Output Port
//                | | | | | | | +---- system reset line
//                | | | | | | +----- gate A20
//                | | | | +-------- undefined
//                | | | +--------- output buffer full
//                | | +---------- input buffer empty
//                | +----------- keyboard clock (output)
//                +------------ keyboard data (output)
//-----------------------------------------------------------------------------
void Cmd_D1(void)
{

    #if CONTROL_A20_WAY3
    FastA20 = 1;
    return;
    #endif

    #if CONTROL_A20_WAY2
    if( WaitAndGetPort60Data() )
    {
        if( (KBHIData & 0x02) != 0x00 )
        {
            GATEA20_ON();
        }
        else
        {
            GATEA20_OFF();
        }
    }
    return;
    #endif

    #if CONTROL_A20_WAY1
    if ( SetGetPort60Data() )
    {
        if ( (KBHIData & 0x02) != 0x00 )
        {
            GATEA20_ON();
        }
        else
        {
            GATEA20_OFF();
        }
    }
    #endif

}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Handle command D2 -
// Send data to the system as if it came from the keyboard.
//
// * Legacy USB Keyboard use this command for simulate PS/2 keyboard
//-----------------------------------------------------------------------------
void Cmd_D2(void)
{
    if ( SetGetPort60Data() )
    {
        Data_To_Host(KBHIData);
    }

}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Handle command D3 -
// Send data to the system as if it came from the auxiliary device.
//-----------------------------------------------------------------------------
void Cmd_D3(void)
{
    if ( SetGetPort60Data() )
    {
        //Ccb42_DISAB_AUX = 0;    // Enable aux device (mouse)
	    Aux_Data_To_Host(vCheckAuxMux());
    }
}

//-----------------------------------------------------------------------------
// Handle command D4 -
// Output next received byte of data from system to auxiliary device.
//-----------------------------------------------------------------------------
void Cmd_D4(void)
{
    BYTE index;
    if ( SetGetPort60Data() )               // Get driver command from host
    {
        if(PS2_MSCMD)
        {
            #ifdef UART_Debug
                printf("\nMouse Command    : 0X%bX\n",KBHIData);
            #else
                RamDebug(0xD4);
                RamDebug(KBHIData);
            #endif
        }
        if(KBHIData==0xFF)                  // if is reset command
        {
            ScanAUXDevice();                // Scan all ps2 channel
        }
              
        if(Main_MOUSE_CHN!=0x00)        // mouse device is attached 
        {
            ClearFlag(AuxFlags[(Main_MOUSE_CHN-1)], DEVICE_NEED_INIT);
            ClearFlag(AuxFlags[(Main_MOUSE_CHN-1)], DEVICE_NEED_CONFIG);
		    vSendCmdtoMouse((Main_MOUSE_CHN-1));
        }    
	    else
	    {
		    //Cmd_A8();
		    vDelayXms(20);				// Emulate transmission delay times 
		    SendFromAux(0xFC);
	    }
    }
    else
    {
    }
}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Handle 64 port command Ex.
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// Handle command E0 - Reports the state of the test inputs
//-----------------------------------------------------------------------------
void Cmd_E0(void)
{
    KBC_DataToHost(0x00);
}

//----------------------------------------------------------------------------
// Set&Get Status for Next Port60 Data Handle
//----------------------------------------------------------------------------
bit SetGetPort60Data(void)
{
    if ( KBHIStep == 0x00 )
    {
        KBHIStep=0x01;  // Set Next Port60 Data Handle again
        return(0);
    }
    else
    {
        KBHIStep=0x00;  // Set Command Finished
        return(1);
    }

}

//-----------------------------------------------------------------------------
// Set&Get Status for Multi Port60 Data Handle
//-----------------------------------------------------------------------------
bit SetGetPort60MultiData(void)
{
    if ( KBHIStep == 0x00 )
    {
        KBHIStep=0x01;  // Set Next Port60 Data Handle again
        return(0);
    }
    else
    {
        KBHIStep++;
        return(1);
    }

}
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Special Handle
// Get 1 Bytes Keyboard Controller Data
//-----------------------------------------------------------------------------
BYTE WaitAndGetPort60Data(void)
{

    WORD  iLOOP;
    iLOOP = WaitKBDataDelay;

    do
    {
        if ( IS_BIT_SET(KBHISR,1) )
        {
            if ( KBHISR & C_D )
            {
                return 0x00;
            }
            else
            {
                KBHIData  = KBHIDIR;
                return 0x01;
            }
        }

        iLOOP--;
    }
    while (iLOOP != 0 );
    KBCUnProcessCnt++;  //Failed Monitor
    return 0x00;

}
//-----------------------------------------------------------------------------
/* ----------------------------------------------------------------------------
 * FUNCTION: vKeyboardCmd
 *
 * Passes data sent from the Host (port 60 write) to the keyboard.
 *
 * Input: data = data byte to send
 * --------------------------------------------------------------------------*/
/*
//-----------------------------------------------------------------------------
 * Standard Keyboard Commands
 * Command Description
//-----------------------------------------------------------------------------
 * 0xFF Reset - Keyboard responds with "ack" (0xFA), then enters "Reset" mode.
 * 0xFE Resend - Keyboard responds by resending the last-sent byte.
        The exception to this is if the last-sent byte was "resend"
        (0xFE). If this is the case, the keyboard resends the last non
 * 0xFE byte. This command is used by the host to indicate an error in
        reception. The next six commands can be issued when the keyboard is
        in any mode, but it only effects the behavior of the keyboard when in
        "mode 3" (ie, set to scan code set 3.)
 * 0xFD (Set Key Type Make) - Disable break codes and typematic repeat for
        specified keys. Keyboard responds with "ack" (0xFA), then disables
        scanning (if enabled) and reads a list of keys from the host. These
        keys are specified by their set 3 make codes. Keyboard responds to
        each make code with "ack". Host terminates this list by sending an
        invalid set 3 make code (eg, a valid command.) The keyboard then
        re-enables scanning (if previously disabled).
 * 0xFC (Set Key Type Make/Break) - Similar to previous command, except this
        one only disables typematic repeat.
 * 0xFB (Set Key Type Typematic) - Similar to previous two, except this one
        only disables break codes.
 * 0xFA (Set All Keys Typematic/Make/Break) - Keyboard responds with "ack"
        (0xFA). Sets all keys to their normal setting (generate scan codes on
        make, break, and typematic repeat)
 * 0xF9 (Set All Keys Make) - Keyboard responds with "ack" (0xFA). Similar to
        0xFD, except applies to all keys.
 * 0xF8 (Set All Keys Make/Break) - Keyboard responds with "ack" (0xFA).
        Similar to 0xFC,except applies to all keys.
 * 0xF7 (Set All Keys Typematic) - Keyboard responds with "ack" (0xFA).
        Similar to 0xFB, except applies to all keys.
 * 0xF6 (Set Default) - Load default typematic rate/delay (10.9cps / 500ms),
        key types (all keys typematic/make/break), and scan code set (2).
 * 0xF5 (Disable) - Keyboard stops scanning, loads default values
        (see "Set Default" command), and waits further instructions.
 * 0xF4 (Enable) - Re-enables keyboard after disabled using previous command.
 * 0xF3 (Set Typematic Rate/Delay) - Host follows this command with one
        argument byte that defines the typematic rate.
 * 0xF2 (Read ID) - The keyboard responds by sending a two-byte device
        ID of 0xAB, 0x83. (0xAB is sent first, followed by 0x83.)
 * 0xF0 (Set Scan Code Set) - Keyboard responds with "ack", then reads
        argument byte from the host. This argument byte may be 0x01, 0x02,
        or 0x03 to select scan code set 1, 2, or 3, respectively.
        The keyboard responds to this argument byte with "ack".
        If the argument byte is 0x00, the keyboard responds with "ack"
        followed by the current scan code set.
 * 0xEE (Echo) - The keyboard responds with "Echo" (0xEE).
 * 0xED (Set/Reset LEDs) - The host follows this command with one argument
        byte, that specifies the state of the keyboard's Num Lock, Caps Lock,
        and Scroll Lock LEDs.
//----------------------------------------------------------------------------
*/

/* ----------------------------------------------------------------------------
 * FUNCTION: vKeyboardCmd
 *
 * Passes data sent from the Host (port 60 write) to the keyboard.
 *
 * Input: data = data byte to send
 * ------------------------------------------------------------------------- */
static void vKeyboardCmd(BYTE nKB60DAT)
{
    BYTE ack,ack1,ack2,index,cmdbk;

    KBCmdAckByteCunt(0x00);         // New keyboard command, clear ack counter
    ack = 0x00;
    ack1 = 0x00;
    ack2 = 0x00;
    
    //-------------------------------------------------------------------------
    //Keyboard Command Received Data Process
    //-------------------------------------------------------------------------
    if ( KB_Command )
    {
        cmdbk = KB_Command;
        
        switch(KB_Command)
        {
            case 0xED:  /* Update LEDs command. */
                        Led_Data = nKB60DAT;
                        ack = 0xFA;
                        KB_Command = 0x00;
                        OEM_Write_Leds();   //Hook Oem On-board LED control

                        /* Update scanner numlock state. */
                        Scanner_State_NUM_LOCK = Led_Data_NUM;
                        break;
            case 0xF0:  /* Set alternate scan codes. */
                        KB_CodeSet = nKB60DAT;
                        ack = 0xFA;
                        KB_Command = 0x00;
                        break;
            case 0xF3:  /* Set typematic rate/delay. */
                        PS2KB_Typematic = nKB60DAT;
                        Set_Typematic(PS2KB_Typematic);
                        ack = 0xFA;
                        KB_Command = 0x00;
                        break;
            default:    //Unknown command request system resend   
                        ack = 0xFE;         //Resend         
                        KBCUnProcessCnt++;
                        break;
        }

        if(PS2_KBCMD)
        {
            RamDebug(0x60);
            RamDebug(nKB60DAT);
        }

        if((cmdbk==0xF3)||(cmdbk==0xED))
        {
            KeyboardDriverIn = 1;
        } 
        
        if( Main_KB_CHN ==0x00)             // no any external keyboard
        {
            if(ack != 0x00)                 // if need send ack to host
            {
                KBC_DataToHost(ack);        // send to host
            }
        }
        else                                // Send command to external keyboard
        {
            KBCmdAckByteCunt(0x01);         // set ec need send one byte to host
            KBCmdCheckMouseBusy(cmdbk);
            if(!AUXInterfaceBusy)
            {
                if(cmdbk==0xED)
                {
	                Send2Port((Main_KB_CHN-1), (nKB60DAT&0x07));
                }
                else
                {
	                Send2Port((Main_KB_CHN-1), nKB60DAT);
                }

                if((cmdbk==0xF3)||(cmdbk==0xED))
                {
                    SetOtherKBNeedUpdataFlag(cmdbk);   
                } 
            }
        }
        return;
    }
    
    //-------------------------------------------------------------------------
    //Keyboard Command Process
    //-------------------------------------------------------------------------
    switch(nKB60DAT)
    {
        KB_Command = 0x00;
        case 0xED:  /* Update LEDs command. */
                    ack = 0xFA;
                    KB_Command = 0xED;
                    break;
        case 0xEC:
                    ack = 0xFA;
                    break;

        case 0xEE:  /* ECHO command. */
                    ack = 0xEE;
                    break;

        case 0xF0:  /* Set alternate scan codes. */
                    ack = 0xFA;
                    KB_Command = 0xF0;
                    break;

        case 0xF2:  /* Read manufacturers ID */
                    ack = 0xFA;
                    ack1 = 0xAB;
                    if( Ccb42_XLATE_PC )
                    {
                        ack2 = 0x83;
                    }
                    else
                    {
                        ack2 = 0x41;
                    }
                    break;

        case 0xF3:  /* Set typematic rate/delay. */
                    ack = 0xFA;
                    KB_Command = 0xF3;
                    break;

        case 0xF4:  /* Enable scanning. */
                    KeyboardDriverIn = 1;
                    Clear_Key();
                    ack = 0xFA;
                    break;

        case 0xF5:  /* Default disable. */
                    KeyboardDriverIn = 0;
                    Clear_Key();
                    Clear_Typematic();
                    ack = 0xFA;
                    break;

        case 0xF6:  /* Set defaults. */
                    Clear_Key();
                    Clear_Typematic();
                    ack = 0xFA;
                    break;
        case 0xF7:
        case 0xF8:
        case 0xF9:
        case 0xFA:
        case 0xFB:
                    ack = 0xFA;
                    break;

        case 0xFF:  /* Reset keyboard. */
                    #ifdef ITE_EVBoard
                    //InitShareMemory();
                    InitSio();
                    #endif
                    KeyboardDriverIn = 0;
                    ScanAUXDevice();            // Scan all ps2 channel    
                    Clear_Key();
                    Clear_Typematic();
                    Scanner_State = 0;
                    Clear_Fn_Keys();
                    ack = 0xFA;
                    ack1 = 0xAA;
                    break;

        default:    //Unknown command request system resend
                    ack = 0xFE;
                    KBCUnProcessCnt++;
                    KBCUnProcessRec = nKB60DAT;
                    break;

    }

    if(PS2_KBCMD)                       // for keybaord command debug
    {
        RamDebug(0x60);
        RamDebug(nKB60DAT);
    }

    if( Main_KB_CHN ==0x00)             // no any external keyboard
    {
        KBCmdAckByteCunt(0x00);
        if(ack != 0x00)
        {
            vDelayXms(20);
            KBC_DataToHost(ack);

            if(ack1 != 0x00)
            {
                vDelayXms(20);
                KBC_DataPending(ack1);

                if(ack2 != 0x00)
                {
                    vDelayXms(20);
                    KBC_DataPending(ack2);
                }
            }
        }
    }
    else                                    // Send command to external keyboard
    {
        if(nKB60DAT==0xFF)                  // reset command
        {
            KBCmdAckByteCunt(0x02);
        }
        else                             
        {
            if(nKB60DAT==0xF2)              // read keyboard ID command
            {
                KBCmdAckByteCunt(0x03);
            }
            else if((nKB60DAT==0xF3)||(nKB60DAT==0xED))
            {
                KBCmdAckByteCunt(0x02);
            }
            else                            // other
            {
                KBCmdAckByteCunt(0x01);
            }
        }
        
        KBCmdCheckMouseBusy(nKB60DAT);
        if(!AUXInterfaceBusy)
        {
	        Send2Port((Main_KB_CHN-1), nKB60DAT);
            if((nKB60DAT==0xF4)||(nKB60DAT==0xF5))
            {
                SetOtherKBNeedUpdataFlag(nKB60DAT);   
            }  
        }
    }
}

/* ----------------------------------------------------------------------------
 * FUNCTION: Read_Input_Port_1
 *
 * Read 8042 Input Port 1 and return its contents.
 *
 * Return: Byte from input port.
 *     Bit 7 = Keyboard not inhibited via switch.
 *     Bit 6 = Monochrome adapter (0 = Color/Graphics adapter).
 *     Bit 5 = Manufacturing jumper not installed.
 *     Bit 4 = Enable 2nd 256K of system board RAM.
 *     Bit 3 =
 *     Bit 2 =
 *     Bit 1 = Auxiliary data in line (PS/2 only).
 *     Bit 0 = Keyboard data in line (PS/2 only).
 * ------------------------------------------------------------------------- */
//BYTE Read_Input_Port_1(void)
//{
//   Just return the compatibility value for now. //return (PCIN | 0x1F);
//   return(0xBF);
//}


/* ----------------------------------------------------------------------------
 * FUNCTION: Read_Output_Port_2
 *
 * Read 8042 output port 2.
 *
 * Return: Byte from output port.
 *     Bit 7 = Keyboard data output line (inverted on PS/2).
 *     Bit 6 = Keyboard clock output line.
 *     Bit 5 = Input buffer empty (auxiliary interrupt IRQ12 on PS/2).
 *     Bit 4 = Output buffer full (Generates IRQ1).
 *     Bit 3 = Reserved (inverted auxiliary clock output line on PS/2).
 *     Bit 2 = Reserved (inverted auxiliary data output line on PS/2).
 *     Bit 1 = Gate A20.
 *     Bit 0 = System reset.
 * ------------------------------------------------------------------------- */
BYTE Read_Output_Port_2(void)
{
#if 1
    BYTE p2byte;

    /* "data" will hold bits read from different inputs.
        Start with bit 0 set and set bits 2 and 3 for AT mode.
        If PS2 mode is being used, bits 2 and 3 will be changed. */
    p2byte = (1 << 3) | (1 << 2) | (1 << 0);

    if (Ext_Cb0_PS2_AT)
    {   /* If PS2 mode is enabled */
        /* Invert auxiliary keyboard data line (bit 7) and clear bits 2 and 3.
           (Since bits 2 and 3 were set before, they can be cleared by inverting.) */
        p2byte^= 0x8C;
    }

    return (p2byte);
#else

    BYTE data_byte;

    /* "data" will hold bits read from different inputs.
       Start with bit 0 set and set bits 2 and 3 for AT mode.
       If PS2 mode is being used, bits 2 and 3 will be changed. */
    data_byte = (1 << 3) | (1 << 2) | (1 << 0);

    /* Put Gate A20 bit value into bit 1. */

    if (Ext_Cb0_PS2_AT)
    {   /* PS/2 mode is enabled 8/
        /* Invert aux keyboard data line (bit 7) and clear bit[3:2]. (Since
           bits 2 and 3 were set before, they can be cleared by inverting.) */
        data_byte ^= 0x8C;

        /* Put inverted auxiliary device (mouse) clock line in bit 3. */

        /* Put inverted auxiliary device (mouse) data line in bit 2. */
    } /* if (Ext_Cb0_PS2_AT) */

    return (data_byte);
#endif
}


//-----------------------------------------------------------------------------
const FUNCT_PTR_V_V code Cmd8X_table[16] =
{
    Cmd_80,             // Process 64 port command 80
    Cmd_81,             // Process 64 port command 81
    Cmd_82,             // Process 64 port command 82
    Cmd_83,             // Process 64 port command 83
    Cmd_84,             // Process 64 port command 84
    Cmd_85,             // Process 64 port command 85
    Cmd_86,             // Process 64 port command 86
    Cmd_87,             // Process 64 port command 87
    Cmd_88,             // Process 64 port command 88
    Cmd_89,             // Process 64 port command 89
    Cmd_8A,             // Process 64 port command 8A
    Cmd_8B,             // Process 64 port command 8B
    Cmd_8C,             // Process 64 port command 8C
    Cmd_8D,             // Process 64 port command 8D
    Cmd_8E,             // Process 64 port command 8E
    Cmd_8F              // Process 64 port command 8F
};
//-----------------------------------------------------------------------------
const FUNCT_PTR_V_V code Cmd9X_table[16] =
{
    Cmd_90,             // Process 64 port command 90
    Cmd_91,             // Process 64 port command 91
    Cmd_92,             // Process 64 port command 92
    Cmd_93,             // Process 64 port command 93
    Cmd_94,             // Process 64 port command 94
    Cmd_95,             // Process 64 port command 95
    Cmd_96,             // Process 64 port command 96
    Cmd_97,             // Process 64 port command 97
    Cmd_98,             // Process 64 port command 98
    Cmd_99,             // Process 64 port command 99
    Cmd_9A,             // Process 64 port command 9A
    Cmd_9B,             // Process 64 port command 9B
    Cmd_9C,             // Process 64 port command 9C
    Cmd_9D,             // Process 64 port command 9D
    Cmd_9E,             // Process 64 port command 9E
    Cmd_9F              // Process 64 port command 9F
};
//-----------------------------------------------------------------------------
const FUNCT_PTR_V_V code CmdAX_table[16] =
{
    Cmd_A0,             // Process 64 port command A0
    Cmd_A1,             // Process 64 port command A1
    Cmd_A2,             // Process 64 port command A2
    Cmd_A3,             // Process 64 port command A3
    Cmd_A4,             // Process 64 port command A4
    Cmd_A5,             // Process 64 port command A5
    Cmd_A6,             // Process 64 port command A6
    Cmd_A7,             // Process 64 port command A7
    Cmd_A8,             // Process 64 port command A8
    Cmd_A9,             // Process 64 port command A9
    Cmd_AA,             // Process 64 port command AA
    Cmd_AB,             // Process 64 port command AB
    Cmd_AC,             // Process 64 port command AC
    Cmd_AD,             // Process 64 port command AD
    Cmd_AE,             // Process 64 port command AE
    Cmd_AF              // Process 64 port command AF
};
//-----------------------------------------------------------------------------
const FUNCT_PTR_V_V code CmdBX_table[16] =
{
    Cmd_B0,             // Process 64 port command B0
    Cmd_B1,             // Process 64 port command B1
    Cmd_B2,             // Process 64 port command B2
    Cmd_B3,             // Process 64 port command B3
    Cmd_B4,             // Process 64 port command B4
    Cmd_B5,             // Process 64 port command B5
    Cmd_B6,             // Process 64 port command B6
    Cmd_B7,             // Process 64 port command B7
    Cmd_B8,             // Process 64 port command B8
    Cmd_B9,             // Process 64 port command B9
    Cmd_BA,             // Process 64 port command BA
    Cmd_BB,             // Process 64 port command BB
    Cmd_BC,             // Process 64 port command BC
    Cmd_BD,             // Process 64 port command BD
    Cmd_BE,             // Process 64 port command BE
    Cmd_BF              // Process 64 port command BF
};
//-----------------------------------------------------------------------------
const FUNCT_PTR_V_V code CmdCX_table[16] =
{
    Cmd_C0,             // Process 64 port command C0
    Cmd_C1,             // Process 64 port command C1
    Cmd_C2,             // Process 64 port command C2
    Cmd_C3,             // Process 64 port command C3
    Cmd_C4,             // Process 64 port command C4
    Cmd_C5,             // Process 64 port command C5
    Cmd_C6,             // Process 64 port command C6
    Cmd_C7,             // Process 64 port command C7
    Cmd_C8,             // Process 64 port command C8
    Cmd_C9,             // Process 64 port command C9
    Cmd_CA,             // Process 64 port command CA
    Cmd_CB,             // Process 64 port command CB
    Cmd_CC,             // Process 64 port command CC
    Cmd_CD,             // Process 64 port command CD
    Cmd_CE,             // Process 64 port command CE
    Cmd_CF              // Process 64 port command CF
};
//-----------------------------------------------------------------------------
const FUNCT_PTR_V_V code CmdDX_table[16] =
{
    Cmd_D0,             // Process 64 port command D0
    Cmd_D1,             // Process 64 port command D1
    Cmd_D2,             // Process 64 port command D2
    Cmd_D3,             // Process 64 port command D3
    Cmd_D4,             // Process 64 port command D4
    Cmd_D5,             // Process 64 port command D5
    Cmd_D6,             // Process 64 port command D6
    Cmd_D7,             // Process 64 port command D7
    Cmd_D8,             // Process 64 port command D8
    Cmd_D9,             // Process 64 port command D9
    Cmd_DA,             // Process 64 port command DA
    Cmd_DB,             // Process 64 port command DB
    Cmd_DC,             // Process 64 port command DC
    Cmd_DD,             // Process 64 port command DD
    Cmd_DE,             // Process 64 port command DE
    Cmd_DF              // Process 64 port command DF
};
//-----------------------------------------------------------------------------
#if SUPPORTED_KBC_EX
const FUNCT_PTR_V_V code CmdEX_table[16] =
{
    Cmd_E0,             // Process 64 port command E0
    Cmd_E1,             // Process 64 port command E1
    Cmd_E2,             // Process 64 port command E2
    Cmd_E3,             // Process 64 port command E3
    Cmd_E4,             // Process 64 port command E4
    Cmd_E5,             // Process 64 port command E5
    Cmd_E6,             // Process 64 port command E6
    Cmd_E7,             // Process 64 port command E7
    Cmd_E8,             // Process 64 port command E8
    Cmd_E9,             // Process 64 port command E9
    Cmd_EA,             // Process 64 port command EA
    Cmd_EB,             // Process 64 port command EB
    Cmd_EC,             // Process 64 port command EC
    Cmd_ED,             // Process 64 port command ED
    Cmd_EE,             // Process 64 port command EE
    Cmd_EF              // Process 64 port command EF
};
#endif
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void Cmd_8X(void)
{
    (Cmd8X_table[KBHICmd&0x0F])();
}
//-----------------------------------------------------------------------------
void Cmd_9X(void)
{
    (Cmd9X_table[KBHICmd&0x0F])();
}
//-----------------------------------------------------------------------------
void Cmd_AX(void)
{
    (CmdAX_table[KBHICmd&0x0F])();
}
//-----------------------------------------------------------------------------
void Cmd_BX(void)
{
    (CmdBX_table[KBHICmd&0x0F])();
}
//-----------------------------------------------------------------------------
void Cmd_CX(void)
{
    (CmdCX_table[KBHICmd&0x0F])();
}
//-----------------------------------------------------------------------------
void Cmd_DX(void)
{
    (CmdDX_table[KBHICmd&0x0F])();
}
//-----------------------------------------------------------------------------
void Cmd_EX(void)
{
    #if SUPPORTED_KBC_EX
    (CmdEX_table[KBHICmd&0x0F])();
    #endif
}
//-----------------------------------------------------------------------------
//void Cmd_FE(void)
//void Cmd_FF(void)
void Cmd_FX(void)
{
    //(CmdFX_table[KBHICmd&0x0F])();
    if( (KBHICmd % 2) == 0x00 )    // Even command
    {
        GATEA20_OFF();
        KBRST_ON();
        Microsecond_Delay(64);  // Delay.
        GATEA20_ON();
        KBRST_OFF();
    }                           // Odd command do no thing
    CLEAR_BIT( Ccb42, 2 );
    CLEAR_BIT( KBHISR, 2 );
}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Process Command/Data received from System via the KBC interface
//
//-----------------------------------------------------------------------------
const FUNCT_PTR_V_V code Port64_Table[16] =
{
    Cmd_00_3F,          // Process command 0x
    Cmd_00_3F,          // Process command 1x
    Cmd_00_3F,          // Process command 2x
    Cmd_00_3F,          // Process command 3x
    Cmd_40_7F,          // Process command 4x
    Cmd_40_7F,          // Process command 5x
    Cmd_40_7F,          // Process command 6x
    Cmd_40_7F,          // Process command 7x
    Cmd_8X,             // Process command 8x
    Cmd_9X,             // Process command 9x
    Cmd_AX,             // Process command Ax
    Cmd_BX,             // Process command Bx
    Cmd_CX,             // Process command Cx
    Cmd_DX,             // Process command Dx
    Cmd_EX,             // Process command Ex
    Cmd_FX,             // Process command Fx
};
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Service Host Port 60/64 Keyboard Controller or Keyboard Command
//
//-----------------------------------------------------------------------------
void Service_PCI(void)
{
    //-------------------------------------------------------------------------
    if ( IsFlag0(KBHISR,IBF) )  return;
    //-------------------------------------------------------------------------
    if ( KBHISR & C_D )         // Command
    {
        FastA20  = 0;
        if( KBHIStep > 0 )      // Command Start
        {
            KBHIStep = 0;
            KBCUnProcessCnt++;
        }
        KBHICmd = KBHIDIR;
        //---------------------------------------------------------------------
        #if  SUPPORTED_RECORDER
        if( En_Record64 && KBHICmd!=0xAD && KBHICmd!=0xAE )
        {
            #ifdef UART_Debug
            printf("Port 64 : 0X%bX\n",KBHICmd);
            #else
            RamDebug(0x64);
            RamDebug(KBHICmd);
            #endif
        }
        #endif
        //---------------------------------------------------------------------
        (Port64_Table[(KBHICmd >>4)])();
        //---------------------------------------------------------------------
    }
    else    // Data
    {
        if( KBHIStep || FastA20 )   // If need data
        {
            KBHIData  = KBHIDIR;

            #if  SUPPORTED_RECORDER
            if( En_Record60 )
            {   
                #ifdef UART_Debug
                printf("Port 60 : 0X%bX\n",KBHIData);
                #else    
                RamDebug(0x60);     
                RamDebug(KBHIData); 
                #endif
            }
            #endif

            #if CONTROL_A20_WAY3
            if( FastA20 )
            {
                FastA20 = 0;
                if( (KBHIData & 0x02) != 0x00 )
                {
                    GATEA20_ON();
                }
                else
                {
                    GATEA20_OFF();
                }
                return;
            }
            #endif

            (Port64_Table[(KBHICmd >>4)])();

        }
        else
        {
            KBHIData  = KBHIDIR;

            #if  SUPPORTED_RECORDER
            if( En_RecordKB60 )
            {   
                #ifdef UART_Debug
                printf("Port 60 KB Command : 0X%bX\n",KBHIData);
                #else  
                RamDebug(0x60);    
                RamDebug(KBHIData);  
                #endif
            }
            #endif

            vKeyboardCmd(KBHIData);     // Keyboard Command
        }
    }

}

//-----------------------------------------------------------------------------

/*-----------------------------------------------------------------------------
 * End
 *---------------------------------------------------------------------------*/

