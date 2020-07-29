/*-----------------------------------------------------------------------------
 * Filename: CORE_FLASH.H
 * Function: CORE_FLASH.C Function prototype include definition
 *
 * Copyright (c) 2006-2009, ITE Tech. Inc. All Rights Reserved.
 *---------------------------------------------------------------------------*/
#ifndef CORE_FLASH_H
#define CORE_FLASH_H

#include "PROJECT.H"

//----------------------------------------------------------------------------
// SPI command 
//----------------------------------------------------------------------------
#define SPICmd_WRSR				0x01	// Write Status Register
#define SPICmd_BYTEProgram		0x02	// To Program One Data Byte
#define SPICmd_WRDI				0x04	// Write diaable

#define SPICmd_ReadStatus		0x05	// Read Status Register
#define SPIStatus_BUSY			BIT(0)	// 1 = Internal Write operation is in progressn
#define SPIStatus_WEL			BIT(1)	// 1 = Device is memory Write enabled
#define SSTSPIStatus_AAI        BIT(6)  // 1 = AAI programming mode    

#define SPICmd_WREN				0x06	// Write Enable
#define SPICmd_HighSpeedRead	0x0B	// High-Speed Read 
#define SPICmd_EWSR				0x50	// Enable Write Status Register
#define SPICmd_RDID			    0xAB	// Read ID
#define SPICmd_DeviceID			0x9F	// Manufacture ID command
#define SPICmd_AAIWordProgram	0xAD	// Auto Address Increment Programming
#define SPICmd_AAIProgram	    0xAF	// Auto Address Increment Programming
#define SPICmd_Erase4KByte		0x20	// Erase 4 KByte block of memory array
#define SPICmd_Erase32KByte		0x52	// Erase 32 KByte block of memory array
#define SPICmd_Erase64KByte		0xD8	// Erase 64 KByte block of memory array

//----------------------------------------------------------------------------
// SPI ID of first byte 
//----------------------------------------------------------------------------
#define SSTID					0xBF
#define SSTID_Old			    0x01

#define WinbondID				0xEF
#define AtmelID					0x9F
#define STID					0x20
#define SpansionID				0x01
#define MXICID					0xC2
#define AMICID					0x37
#define EONID					0x1C
#define ESMTID					0x8C

//----------------------------------------------------------------------------
// SPI status setting 
//----------------------------------------------------------------------------
#define SPIStatus_ProtectAll	0xFC
#define SPIStatus_UnlockAll		0x00
#define SPIStatus_WriteEnable	0x02

extern void FuncAt_0xFE00(void);
extern void FuncAt_0xFF00(void);
extern void ITE_Flash_Utility(void);
extern void RunCodeInRAM(void);
extern void FlashECCode(void);
extern void RamCode(void);
extern void SPI_Write_Enable(void);
extern void LoadSPIFucnToRam(FUNCT_PTR_V_V funcpoint);
extern void Do_SPI_Read_Status(void);
extern void Do_SPI_Write_Status(BYTE statusvalue);
extern void Do_SPI_Read_ID(void);
extern void Do_SPI_Read_ID_CmdAB(void);
extern void Do_SPI_Erase(BYTE EraseCmd, BYTE Addr2, BYTE Addr1, BYTE Addr0);
extern void Do_SPI_Write_256Bytes(XBYTE *DataPointer, BYTE Addr2, BYTE Addr1, BYTE Addr0);
extern void Do_SPI_Read_256Bytes(XBYTE *DataPointer, BYTE Addr2, BYTE Addr1, BYTE Addr0);
extern void Do_SPI_Write_Enable(void);
extern void Do_SPI_Write_Disable(void);

//----------------------------------------------------------------------------
// OEM flash SPI example 
//----------------------------------------------------------------------------
extern void OEM_SPIProtectAll(void);
extern void OEM_Write_SPI_256Bytes(void);
extern void OEM_Read_SPI_256Bytes(void);
extern void OEM_SPI_Read_ID(void);
extern BYTE OEM_SPIReadStatus(void);

//-----------------------------------------------------------------------------
#endif