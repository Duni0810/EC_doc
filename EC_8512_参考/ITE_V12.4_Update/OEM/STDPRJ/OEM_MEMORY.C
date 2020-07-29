/*-----------------------------------------------------------------------------
 * Filename: OEM_MEMORY.C     For Chipset: ITE.IT85XX
 *
 * Function: Memory Definition for extern all code base reference
 *
 * [Memory Map Description]
 *
 * =Chip Internal= 0x00-0xFF
 * 0x00-0xFF     For Kernel and Chip Level use
 *
 ***************************************************
 * Chip 8500       : External ram 0x000-0x7FF
 * Chip Other 85XX : External ram 0x000-0xFFF
 ***************************************************
 * Memory Map :
 * 0x000-0x0FF   For Kernel Core/Module
 * 0x100-0x1FF   OEM RAM        
 * 0x200-0x2FF   OEM RAM
 * 0x300-0x3FF   OEM RAM        
 * 0x400-0x4FF   OEM RAM
 * 0x500-0x5FF   OEM RAM        
 * 0x600-0x6FF   For ITE flash utility,EC SPI rom read write function, and ramdebug function.
 * 0x700-0x7FF   For Keil C Complier and Ram Code Space (for SPI programming).
 * 0x800-0x8FF   OEM RAM 
 * 0x800-0x9FF   OEM RAM 
 * 0xA00-0xAFF   OEM RAM 
 * 0xB00-0xBFF   OEM RAM  
 * 0xC00-0xCFF   OEM RAM 
 * 0xD00-0xDFF   OEM RAM 
 * 0xE00-0xEFF   OEM RAM 
 * 0xF00-0xFFF   OEM RAM    
 *
 * Copyright (c) 2006-2009, ITE Tech. Inc. All Rights Reserved.
 *---------------------------------------------------------------------------*/

//-----------------------------------------------------------------------------
// Common Header Include
//-----------------------------------------------------------------------------
#include "CORE_INCLUDE.H"
#include "OEM_INCLUDE.H"

//-----------------------------------------------------------------------------
/*****************************************************************************/
/* REGISTER MAPPING AREA *****************************************************/
/*****************************************************************************/
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/*****************************************************************************/
/* EXTERNAL RAM AREA      (0x0100 ~ 0x01FF) **********************************/
/*****************************************************************************/
//-----------------------------------------------------------------------------
#define BAT1_MemBase                0x0100
XBYTE   BAT1_ManufacturerAccess     _at_ BAT1_MemBase+0x00;
XBYTE   BAT1_BatteryMode            _at_ BAT1_MemBase+0x02;
XBYTE   BAT1_Temperature            _at_ BAT1_MemBase+0x04;
XBYTE   BAT1_Voltage                _at_ BAT1_MemBase+0x06;
XBYTE   BAT1_Current                _at_ BAT1_MemBase+0x08;
XBYTE   BAT1_AverageCurrent         _at_ BAT1_MemBase+0x0A;
XBYTE   BAT1_MaxError               _at_ BAT1_MemBase+0x0C;
XBYTE   BAT1_RelativeStateOfChg     _at_ BAT1_MemBase+0x0E;
XBYTE   BAT1_AbsoluteOfCharge       _at_ BAT1_MemBase+0x10;
XBYTE   BAT1_RemainingCapacity      _at_ BAT1_MemBase+0x12;
XBYTE   BAT1_FullChargeCapacity     _at_ BAT1_MemBase+0x14;
XBYTE   BAT1_ChargingCurrent        _at_ BAT1_MemBase+0x16;
XBYTE   BAT1_ChargingVoltage        _at_ BAT1_MemBase+0x18;
XBYTE   BAT1_BatteryStatus          _at_ BAT1_MemBase+0x1A;
XBYTE   BAT1_CycleCount             _at_ BAT1_MemBase+0x1C;
XBYTE   BAT1_DesignCapacity         _at_ BAT1_MemBase+0x1E;
XBYTE   BAT1_DesignVoltage          _at_ BAT1_MemBase+0x20;
XBYTE   BAT1_SpecificationInfo      _at_ BAT1_MemBase+0x22;
XBYTE   BAT1_ManufactureDate        _at_ BAT1_MemBase+0x24;
XBYTE   BAT1_SerialNumber           _at_ BAT1_MemBase+0x26;
XBYTE   BAT1_ManufacturerName[14]   _at_ BAT1_MemBase+0x28;
XBYTE   BAT1_DeviceName[14]         _at_ BAT1_MemBase+0x36;
XBYTE   BAT1_DeviceChemistry[6]     _at_ BAT1_MemBase+0x44;
XBYTE   BAT1_ManufacturerData       _at_ BAT1_MemBase+0x4A;



/*
XBYTE BEC_REG_FLAG0
    bEC_REG_ADP
    bEC_REG_AFLT
    bEC_REG_BAT0
    bEC_REG_BPU0
    bEC_REG_BTC0
    bEC_REG_LIDS
    bEC_REG_LSW0
    bEC_REG_LWKE
XWORD   wEC_REG_BDC0
XWORD   wEC_REG_BFC0
XWORD   wEC_REG_BDV0
XBYTE   xEC_REG_BST0
XWORD   wEC_REG_BPR0
XWORD   wEC_REG_BRC0
XWORD   wEC_REG_BPV0
XWORD   wEC_REG_BTP0
XWORD   wEC_REG_BCW0
XWORD   wEC_REG_BCL0
XWORD   wEC_REG_BCG0
XWORD   wEC_REG_BG20
XBYTE   sEC_REG_BMO0[4]
XBYTE   sEC_REG_BIF0[4]
XWORD   wEC_REG_BSN0
XBYTE   sEC_REG_BTY0[8]
*/

//-----------------------------------------------------------------------------
/*****************************************************************************/
/* EXTERNAL RAM AREA      (0x0200 ~ 0x02FF) **********************************/
/*****************************************************************************/
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/*****************************************************************************/
/* EXTERNAL RAM AREA      (0x0300 ~ 0x03FF) **********************************/
/*****************************************************************************/
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/*****************************************************************************/
/* EXTERNAL RAM AREA      (0x0400 ~ 0x04FF) **********************************/
/*****************************************************************************/


/*****************************************************************************/
/* EXTERNAL RAM AREA      (0x0500 ~ 0x05FF) **********************************/
/*****************************************************************************/
//-----------------------------------------------------------------------------
#define SMBA_MemBase                0x0500
XBYTE       SMBA_DEV_DIR            _at_ SMBA_MemBase+0x00; 
XBYTE       SMBA_DEV_CTL            _at_ SMBA_MemBase+0x01;
XBYTE       SMBA_DEV_CMD            _at_ SMBA_MemBase+0x02;
XWORD       SMBA_DEV_MEM            _at_ SMBA_MemBase+0x03;
XBYTE       SMBA_DEV_NO             _at_ SMBA_MemBase+0x05;
FLAGReg8    SMBA_DEV_STATUS         _at_ SMBA_MemBase+0x06;
XBYTE       SMBA_BYTE_CNT           _at_ SMBA_MemBase+0x07;
XBYTE       SMBA_GET_TIMER          _at_ SMBA_MemBase+0x08;
XBYTE       SMBA_Dev1_POLL_DIR      _at_ SMBA_MemBase+0x09;
XBYTE       SMBA_Dev1_POLL_SPEED    _at_ SMBA_MemBase+0x0A;
XBYTE       SMBA_Dev2_POLL_DIR      _at_ SMBA_MemBase+0x0B;
XBYTE       SMBA_Dev2_POLL_SPEED    _at_ SMBA_MemBase+0x0C;
XBYTE       SMBA_Dev3_POLL_DIR      _at_ SMBA_MemBase+0x0D;
XBYTE       SMBA_Dev3_POLL_SPEED    _at_ SMBA_MemBase+0x0E;
FLAGReg8    SMBA_DEBUG_FLAG         _at_ SMBA_MemBase+0x0F;
//-----------------------------------------------------------------------------
#define SMBB_MemBase                0x0510
XBYTE       SMBB_DEV_DIR            _at_ SMBB_MemBase+0x00;
XBYTE       SMBB_DEV_CTL            _at_ SMBB_MemBase+0x01;
XBYTE       SMBB_DEV_CMD            _at_ SMBB_MemBase+0x02;
XWORD       SMBB_DEV_MEM            _at_ SMBB_MemBase+0x03;
XBYTE       SMBB_DEV_NO             _at_ SMBB_MemBase+0x05;
FLAGReg8    SMBB_DEV_STATUS         _at_ SMBB_MemBase+0x06;
XBYTE       SMBB_BYTE_CNT           _at_ SMBB_MemBase+0x07;
XBYTE       SMBB_GET_TIMER          _at_ SMBB_MemBase+0x08;
XBYTE       SMBB_Dev1_POLL_DIR      _at_ SMBB_MemBase+0x09;
XBYTE       SMBB_Dev1_POLL_SPEED    _at_ SMBB_MemBase+0x0A;
XBYTE       SMBB_Dev2_POLL_DIR      _at_ SMBB_MemBase+0x0B;
XBYTE       SMBB_Dev2_POLL_SPEED    _at_ SMBB_MemBase+0x0C;
XBYTE       SMBB_Dev3_POLL_DIR      _at_ SMBB_MemBase+0x0D;
XBYTE       SMBB_Dev3_POLL_SPEED    _at_ SMBB_MemBase+0x0E;
FLAGReg8    SMBB_DEBUG_FLAG         _at_ SMBB_MemBase+0x0F;
//-----------------------------------------------------------------------------
#define SMBC_MemBase                0x0520
XBYTE       SMBC_DEV_DIR            _at_ SMBC_MemBase+0x00;
XBYTE       SMBC_DEV_CTL            _at_ SMBC_MemBase+0x01;
XBYTE       SMBC_DEV_CMD            _at_ SMBC_MemBase+0x02;
XWORD       SMBC_DEV_MEM            _at_ SMBC_MemBase+0x03;
XBYTE       SMBC_DEV_NO             _at_ SMBC_MemBase+0x05;
FLAGReg8    SMBC_DEV_STATUS         _at_ SMBC_MemBase+0x06;
XBYTE       SMBC_BYTE_CNT           _at_ SMBC_MemBase+0x07;
XBYTE       SMBC_GET_TIMER          _at_ SMBC_MemBase+0x08;
XBYTE       SMBC_Dev1_POLL_DIR      _at_ SMBC_MemBase+0x09;
XBYTE       SMBC_Dev1_POLL_SPEED    _at_ SMBC_MemBase+0x0A;
XBYTE       SMBC_Dev2_POLL_DIR      _at_ SMBC_MemBase+0x0B;
XBYTE       SMBC_Dev2_POLL_SPEED    _at_ SMBC_MemBase+0x0C;
XBYTE       SMBC_Dev3_POLL_DIR      _at_ SMBC_MemBase+0x0D;
XBYTE       SMBC_Dev3_POLL_SPEED    _at_ SMBC_MemBase+0x0E;
FLAGReg8    SMBC_DEBUG_FLAG         _at_ SMBC_MemBase+0x0F;
//-----------------------------------------------------------------------------
#define SMBD_MemBase                0x0530
XBYTE       SMBA_Dev1_FailCount     _at_ SMBD_MemBase+0x00;
XBYTE       SMBA_Dev2_FailCount     _at_ SMBD_MemBase+0x01;
XBYTE       SMBA_Dev3_FailCount     _at_ SMBD_MemBase+0x02;
XBYTE       SMBB_Dev1_FailCount     _at_ SMBD_MemBase+0x03;
XBYTE       SMBB_Dev2_FailCount     _at_ SMBD_MemBase+0x04;
XBYTE       SMBB_Dev3_FailCount     _at_ SMBD_MemBase+0x05;
XBYTE       SMBC_Dev1_FailCount     _at_ SMBD_MemBase+0x06;
XBYTE       SMBC_Dev2_FailCount     _at_ SMBD_MemBase+0x07;
XBYTE       SMBC_Dev3_FailCount     _at_ SMBD_MemBase+0x08;
//-----------------------------------------------------------------------------
XBYTE       SMBX_ACCESS_CHANNEL     _at_ SMBD_MemBase+0x09;
XBYTE       SMBX_ACCESS_DEV_ADDR    _at_ SMBD_MemBase+0x0A;
XBYTE       SMBX_ACCESS_DEV_CTL     _at_ SMBD_MemBase+0x0B;
XBYTE       SMBX_ACCESS_DEV_CMD     _at_ SMBD_MemBase+0x0C;
XWORD       SMBX_ACCESS_DEV_MEM     _at_ SMBD_MemBase+0x0D;
XBYTE       SMBX_DEBUG_MODE_ENABLE  _at_ SMBD_MemBase+0x0F;
//-----------------------------------------------------------------------------
#define SMBX_MemBase                0x0540
XBYTE   SMB_BUF[32]                 _at_ SMBX_MemBase+0x00;
XBYTE   SMB_BUF2[32]                _at_ SMBX_MemBase+0x20;
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
/*****************************************************************************/
/* EXTERNAL RAM AREA      (0x0600 ~ 0x06FF) **********************************/
/*****************************************************************************/
//-----------------------------------------------------------------------------
// For ITE flash utility,EC SPI rom read write function, and ramdebug function.

//-----------------------------------------------------------------------------
/*****************************************************************************/
/* EXTERNAL RAM AREA      (0x0700 ~ 0x07FF) **********************************/
/*****************************************************************************/
//-----------------------------------------------------------------------------
// For Keil C Complier and Ram Code Space (for SPI programming).

//-----------------------------------------------------------------------------
/*****************************************************************************/
/* EXTERNAL RAM AREA      (0x0800 ~ 0x08FF) **********************************/
/*****************************************************************************/
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/*****************************************************************************/
/* EXTERNAL RAM AREA      (0x0900 ~ 0x09FF) **********************************/
/*****************************************************************************/
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/*****************************************************************************/
/* EXTERNAL RAM AREA      (0x0A00 ~ 0x0AFF) **********************************/
/*****************************************************************************/
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/*****************************************************************************/
/* EXTERNAL RAM AREA      (0x0B00 ~ 0x0BFF) **********************************/
/*****************************************************************************/
//-----------------------------------------------------------------------------

/*****************************************************************************/
/* EXTERNAL RAM AREA      (0x0C00 ~ 0x0CFF) **********************************/
/*****************************************************************************/
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/*****************************************************************************/
/* EXTERNAL RAM AREA      (0x0D00 ~ 0x0DFF) **********************************/
/*****************************************************************************/
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/*****************************************************************************/
/* EXTERNAL RAM AREA      (0x0E00 ~ 0x0EFF) **********************************/
/*****************************************************************************/
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/*****************************************************************************/
/* EXTERNAL RAM AREA      (0x0F00 ~ 0x0FFF) **********************************/
/*****************************************************************************/
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/*****************************************************************************/
/* BRAM AREA      (0x2280 ~ 0x22BF) ******************************************/
/*****************************************************************************/
//-----------------------------------------------------------------------------
#define BRAM_BASE                   0x2280
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//=============================================================================

/*****************************************************************************/
// FUNCTION CALL AREA
/*****************************************************************************/
void Clear_RAM_Range(WORD nAddr, WORD nSize)
{
    WORD i;
    Tmp_XPntr = (unsigned int*)nAddr;
    for( i=0; i<=nSize; i++ )
    {
        *Tmp_XPntr = 0;
        Tmp_XPntr++;
    }

}
//----------------------------------------------------------------------------

/*****************************************************************************/
// END
/*****************************************************************************/

//-----------------------------------------------------------------------------

