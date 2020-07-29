/*-----------------------------------------------------------------------------
 * Filename:  CORE_SMBUS.H
 * Function:  CORE_SMBUS.C Function prototype include definition
 *
 * Copyright (c) 2006-2009, ITE Tech. Inc. All Rights Reserved.
 *---------------------------------------------------------------------------*/
#ifndef CORE_SMBUS_H
#define CORE_SMBUS_H

/*-----------------------------------------------------------------------------
 * Data Parameter definition
 *---------------------------------------------------------------------------*/
#define SMBusTOC    200     // SMbus re-try counter

#define SMBusChA    0x00    // SMbus channel A index
#define SMBusChB    0x01    // SMbus channel B index
#define SMBusChC    0x02    // SMbus channel C index
#define SMBusChD    0x03    // SMbus channel D index

#define SMBusWQ     0x80    // Write Quick command
#define SMBusRQ     0x00    // Read Quick command
#define SMBusSBC    0x84    // Send Byte command
#define SMBusRBC    0x04    // Receive Byte command
#define SMBusWB     0x88    // Write byte command
#define SMBusRB     0x08    // Read byte command
#define SMBusWW     0x8C    // Write word command
#define SMBusRW     0x0C    // Read  word command
#define SMBusWBK    0x94    // Write block command
#define SMBusRBK    0x14    // Read block command


/*-----------------------------------------------------------------------------
 * Data Structure definition
 *---------------------------------------------------------------------------*/
typedef struct RSmbusStruct
{
    BYTE    address;
    BYTE    cmd;
    XBYTE   *smbdata;
} sRSmbusStruct;
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
typedef struct WSmbusByteStruct
{
    BYTE    address;
    BYTE    cmd;
    XBYTE   *smbdata01;
    BYTE    data01;
} sWSmbusStruct;
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
typedef struct SMBusStruct
{
    XBYTE   *SMBusCTL;
    XBYTE   *SMBusADR;
    XBYTE   *SMBusCMD;
    XBYTE   *SMBusSTA;
    XBYTE   *SMBusPinCTL;
    XBYTE   *SMBusData0;
    XBYTE   *SMBusData1;
    XBYTE   *SMBusBData;
} sSMBus;
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
typedef struct ResetSMBusS
{
    XBYTE   *SMBusSTA;
    XBYTE   *SMBusCTL2;
    XBYTE   *SMBusPinCTL;
    XBYTE   *SMBusPin0;
    XBYTE   *SMBusPin1;
    XBYTE   *GPIOReg;
    BYTE    GPIOData;
} sResetSMBusS;
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

/*-----------------------------------------------------------------------------
 * External Calling Process
 *---------------------------------------------------------------------------*/
extern bit  bRWSMBus(BYTE Channel,BYTE Protocol,BYTE Addr,BYTE Comd,XBYTE *Var);
extern bit  bRSMBusBlock(BYTE Channel,BYTE Protocol,BYTE Addr,BYTE Comd,XBYTE *Var);
extern bit  bWSMBusBlock(BYTE Channel,BYTE Protocol,BYTE Addr,BYTE Comd,XBYTE *Var,BYTE ByteCont);
extern bit  bSMBusReceiveByte(BYTE Channel,BYTE Addr,XBYTE *Var);
extern bit  bSMBusSendByte(BYTE Channel,BYTE Addr,BYTE SData);

extern bit  vEEPRomRandonRead(BYTE Channel, BYTE addr,BYTE BlockNum, XBYTE *Var);
extern bit  vEEPRomWriteByte(BYTE Channel, BYTE addr, BYTE BlockNum, BYTE Vardata);
extern bit  bWEEPRomPage(BYTE Channel, BYTE Addr,BYTE BlockNum,XBYTE *Var);
extern bit  bREEPRomSEQ(BYTE Channel, BYTE Addr,BYTE BlockNum,XBYTE *Var);

extern void vResetSMBus(BYTE Channel);
extern bit  CheckSMBusStatus( BYTE Channel );
extern void SMBusRecovery(void);
extern void SMBusDelayInact(void);

/*-----------------------------------------------------------------------------
 * Internal Process Definition
 *---------------------------------------------------------------------------*/



//-----------------------------------------------------------------------------
#endif
