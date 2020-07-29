/*-----------------------------------------------------------------------------
 * TITLE: CORE_PECI.C - PECI function  for KBC firmware.
 *
 * Copyright (c) 2010 - , ITE INC. All Rights Reserved.
 *
 * You may not reproduce, distribute, publish, display, perform, modify, adapt,
 * transmit, broadcast, present, recite, release, license or otherwise exploit
 * any part of this publication in any form, by any means, without the prior
 * written permission of ITE INC. .
 *---------------------------------------------------------------------------*/
#include "CORE_INCLUDE.H"
#include "OEM_INCLUDE.H"

//-----------------------------------------------------------------------
// Enable PECI interface function
//-----------------------------------------------------------------------
void Init_PECI(void)
{
    GPCRF6 = PALT;                      
    GCR2 |= PECIE;      // Enable PECI
}

//-----------------------------------------------------------------------
// Init timer1 for interface time-out
//-----------------------------------------------------------------------
void PECI_InitTimeOutTimer(void)
{
    ET1 = 0;                  	// Disable timer1 interrupt
    TH1 = (0xFFFF-23076)>>8;    // Set timer1 counter 30ms
    TL1 = (0xFFFF-23076);       // Set timer1 counter 30ms
    TR1 = 1;                 	// Enable timer1
    TF1 = 0;                  	// Clear overflow flag
}

//-----------------------------------------------------------------------
// CRC-8 polynomial
//-----------------------------------------------------------------------
void PECI_CalcCRC8(BYTE sourcebyte)
{
#if PECI_Softwave_AWFCS    
    BYTE temp;
	PECI_CRC8 ^=sourcebyte;
    temp = PECI_CRC8;

    if(IsFlag0(temp, BIT7))
    {
        temp = temp<<1;
        PECI_CRC8 ^= temp;
    }
    else
    {
        temp = temp<<1;
        PECI_CRC8 ^= 0x09;
        PECI_CRC8 ^= temp;
    }
    
    if(IsFlag0(temp, BIT7))
    {
        temp = temp<<1;
        PECI_CRC8 ^= temp;
    }
    else
    {
        temp = temp<<1;
        PECI_CRC8 ^= 0x07;
        PECI_CRC8 ^= temp;
    }
#endif    
}

//-----------------------------------------------------------------------
// Enables the PECI host controller.
//-----------------------------------------------------------------------
void PECI_HostEnable(void)
{
    HOCTLR |= PECIHEN;
}

//-----------------------------------------------------------------------
// Disable the PECI host controller.
//-----------------------------------------------------------------------
void PECI_HostDisable(void)
{
    HOCTLR = 0x00;
}

//-----------------------------------------------------------------------
// The PECI host controller will perform the desired transaction.
//-----------------------------------------------------------------------
void PECI_HostControl(BYTE control)
{
    HOCTLR |= control;
}

//-----------------------------------------------------------------------
// Check PECI interface busy or not
// return : 
//          0 : 30 ms time out
//          1 : OK
//-----------------------------------------------------------------------
BYTE PECI_CheckHostBusy(void)
{
    PECI_InitTimeOutTimer();    

    while (!TF1)
	{ 
        if(IsFlag0(HOSTAR, HOBY))
        {
            TR1=0;
            TF1=0;
            break;
        }
    }
    
    if(TF1)
	{
        TR1=0;
		TF1=0;
        return(0);
    }
    return(1);
}

//-----------------------------------------------------------------------
// Check PECI interface finish or not
// return : 
//          0 : time-out or error
//          1 : finish
//-----------------------------------------------------------------------
BYTE PECI_CheckHostFinish(void)
{
    BYTE error;
    BYTE status;
    error = 0x00;
    
    PECI_InitTimeOutTimer();
    while (!TF1)
	{ 
        status = HOSTAR;
        if(IsFlag1(status, FINISH))
        {
            TR1=0;
            TF1=0;
            break;
        }
        else if((status&=(BUSERR+EXTERR+WR_FCS_ERR+RD_FCS_ERR))!=0x00)
        {
            error = 0x01;
            break;
        }          
    }
    if((TF1)||(error==0x01))
	{
		TF1=0;
		TR1=0;
        return(0);
    }    
    return(1);
}

//-----------------------------------------------------------------------
// The function of clearing Host Status Register
//-----------------------------------------------------------------------
void ResetPECIStatus(void)
{
    HOSTAR = 0xFE;
}

//-----------------------------------------------------------------------
// PECI Ping function
//  return : 
//              1 : The targeted address of processor is able to respond
//              0 : no respond
//  Input : 
//              (1) addr : The address of processor
//-----------------------------------------------------------------------
BYTE PECI_Ping(BYTE addr)
{
    BYTE done;
    done = 0x00;
    
    PECI_HostEnable();
    HOTRADDR = addr;
    HOWRLR = 0x00;
    HORDLR = 0x00;
    
    if(PECI_CheckHostBusy())
    {
        PECI_HostControl(START+FCSERR_ABT);
        if(PECI_CheckHostFinish())
        {
            done = 0x01;
        }
    }

    PECI_HostDisable();
    ResetPECIStatus();
    return(done);
}

//-----------------------------------------------------------------------
// PECI get temperature command
//  return : 
//              1 : done
//              0 : error
//  Input : 
//              (1) addr : The address of processor
//              (2) *ReadData : the start address of variable to save data
//-----------------------------------------------------------------------
BYTE PECI_GetDIB(BYTE addr, XBYTE *ReadData)
{
    BYTE done, index;
    done = 0x00;
    
    PECI_HostEnable();
    PECI_HostControl(FIFOCLR);
    HOTRADDR = addr;
    HOWRLR = 0x01;
    HORDLR = 0x08;
    HOCMDR = PECI_CMD_GetDIB;
    
    if(PECI_CheckHostBusy())
    {
        //PECI_HostControl(START+FCSERR_ABT+FIFOCLR);
        PECI_HostControl(START+FCSERR_ABT);
        if(PECI_CheckHostFinish())
        {
            for(index=0x00;index<HORDLR;index++)
            {
                *(ReadData+index)=HORDDR;
            }
            done = 0x01;
        }
    }

    PECI_HostDisable();
    ResetPECIStatus();
    return(done);
}

//-----------------------------------------------------------------------
// PECI get temperature command
//  return : 
//              1 : done
//              0 : error
//  Input : 
//              (1) addr : The address of processor
//              (2) *ReadData : the start address of variable to save data
//              (3) Domain : 0 or 1¡C 
//                           0 : Domain 0, 1 : Domain 1¡C
//-----------------------------------------------------------------------
BYTE PECI_GetTemp(BYTE addr, XBYTE *ReadData, BYTE Domain)
{
    BYTE done;
    done = 0x00;
    
    PECI_HostEnable();
    PECI_HostControl(FIFOCLR);
    HOTRADDR = addr;
    HOWRLR = 0x01;
    HORDLR = 0x02;
    if(Domain<2)
    {
        HOCMDR = PECI_CMD_GetTemp+Domain;
    }
    else
    {
        HOCMDR = PECI_CMD_GetTemp;
    }
    
    if(PECI_CheckHostBusy())
    {
        //PECI_HostControl(START+FCSERR_ABT+FIFOCLR);
        PECI_HostControl(START+FCSERR_ABT);
        if(PECI_CheckHostFinish())
        {
            *ReadData=HORDDR;
            *(++ReadData)=HORDDR;
            done = 0x01;
        }
    }

    PECI_HostDisable();
    ResetPECIStatus();
    return(done);
}

//-----------------------------------------------------------------------
// Read to the package configuration space (PCS) within the processor
//  return : 
//              1 : done
//              0 : error
//  Input : 
//              (1) addr : The address of processor
//              (2) *ReadData : the start address of variable to save data
//              (3) Domain : 0 or 1¡C 
//                           0 : Domain 0, 1 : Domain 1¡C
//              (4) Retry   0 or 1
//              (5) Index
//              (6) LSB of parameter
//              (7) MSB of parameter
//              (8) ReadLen read length 2 or 3 or 5
//-----------------------------------------------------------------------
BYTE PECI_RdPkgConfig(BYTE addr, XBYTE *ReadData, BYTE Domain, BYTE Retry, BYTE Index, BYTE LSB, BYTE MSB, BYTE ReadLen)
{
    BYTE done, cunt;
    done = 0x00;
    
    PECI_HostEnable();
    PECI_HostControl(FIFOCLR);
    HOTRADDR = addr;
    HOWRLR = 0x05;
    HORDLR = ReadLen;
    if(Domain<2)
    {
        HOCMDR = PECI_CMD_RdPkgConfig+Domain;
    }
    else
    {
        HOCMDR = PECI_CMD_RdPkgConfig;
    }

    if(Retry<2)
    {
        HOWRDR = (PECI_HostID<<1)+Retry;
    }
    else
    {
        HOWRDR = (PECI_HostID<<1);
    }
    
    HOWRDR = Index;
    HOWRDR = LSB;
    HOWRDR = MSB;
        
    if(PECI_CheckHostBusy())
    {
        //PECI_HostControl(START+FCSERR_ABT+FIFOCLR);
        PECI_HostControl(START+FCSERR_ABT);
        if(PECI_CheckHostFinish())
        {
            for(cunt=0x00;cunt<HORDLR;cunt++)
            {
                if(cunt==0x00)
                {
                    PECI_CompletionCode = HORDDR;
                }
                else
                {
                    *(ReadData+cunt-1)=HORDDR;
                }
            }

            if(PECI_CompletionCode == PECI_CC_Valid)
            {
                done = 0x01;
            }
            else
            {
                done = 0x00;
            }
        }
    }

    PECI_HostDisable();
    ResetPECIStatus();
    return(done);
}

//-----------------------------------------------------------------------
// Write to the package configuration space (PCS) within the processor
//  return : 
//              1 : done
//              0 : error
//  Input : 
//              (1) addr : The address of processor
//              (2) *WriteData : the start address of variable to wirte data
//              (3) Domain : 0 or 1¡C 
//                           0 : Domain 0, 1 : Domain 1¡C
//              (4) Retry   0 or 1
//              (5) Index
//              (6) LSB of parameter
//              (7) MSB of parameter
//              (8) WriteLen write length 0x07 or 0x08 or 0x0A
//-----------------------------------------------------------------------
BYTE PECI_WrPkgConfig(BYTE addr, XBYTE *WriteData, BYTE Domain, BYTE Retry, BYTE Index, BYTE LSB, BYTE MSB, BYTE WriteLen)
{
    BYTE done, cunt;
    done = 0x00;
    
    PECI_CompletionCode = 0x00;
    PECI_HostEnable();
    
    #if PECI_Softwave_AWFCS
    PECI_HostControl(FIFOCLR);
    PECI_CRC8 = 0x00;
    #else
    PECI_HostControl(FIFOCLR+AWFCS_EN);
    #endif
    
    HOTRADDR = addr;
    PECI_CalcCRC8(addr);
    HOWRLR = WriteLen;
    PECI_CalcCRC8(WriteLen);
    HORDLR = 0x01;
    PECI_CalcCRC8(0x01);

    if(Domain<2)
    {
        HOCMDR = PECI_CMD_WrPkgConfig+Domain;
        PECI_CalcCRC8(PECI_CMD_WrPkgConfig+Domain);
    }
    else
    {
        HOCMDR = PECI_CMD_WrPkgConfig;
        PECI_CalcCRC8(PECI_CMD_WrPkgConfig);
    }

    if(Retry<2)
    {
        HOWRDR = (PECI_HostID<<1)+Retry;
        PECI_CalcCRC8((PECI_HostID<<1)+Retry);
    }
    else
    {
        HOWRDR = (PECI_HostID<<1);
        PECI_CalcCRC8(PECI_HostID<<1);
    }

    HOWRDR = Index;
    PECI_CalcCRC8(Index);
    HOWRDR = LSB;
    PECI_CalcCRC8(LSB);
    HOWRDR = MSB;
    PECI_CalcCRC8(MSB);

    for(cunt=0x00; cunt<(WriteLen-6); cunt++)
    {
        HOWRDR = *(WriteData+cunt);
        PECI_CalcCRC8(*(WriteData+cunt));
    }

    #if PECI_Softwave_AWFCS
    PECI_CRC8 ^= 0x80;          // Inverted MSb of preliminary FCS reslut
    HOWRDR = PECI_CRC8;
    #endif
         
    if(PECI_CheckHostBusy())
    {
        PECI_HostControl(START+FCSERR_ABT);
        if(PECI_CheckHostFinish())
        {
            PECI_CompletionCode = HORDDR;
            if(PECI_CompletionCode == PECI_CC_Valid)
            {
                done = 0x01;
            }
            else
            {
                done = 0x00;
            }
        }
    }

    PECI_HostDisable();
    ResetPECIStatus();
    return(done);
}

//-----------------------------------------------------------------------
// Read to Model Specific Registers function
//  return : 
//              1 : done
//              0 : error
//  Input : 
//              (1) addr : The address of processor
//              (2) *ReadData : the start address of variable to save data
//              (3) Domain : 0 or 1¡C 
//                           0 : Domain 0, 1 : Domain 1¡C
//              (4) Retry   0 or 1
//              (5) ProcessorID
//              (6) LSB of parameter
//              (7) MSB of parameter
//              (8) ReadLen read length 0x02 or 0x03 or 0x05 or 0x09
//-----------------------------------------------------------------------
BYTE PECI_RdIAMSR(BYTE addr, XBYTE *ReadData, BYTE Domain, BYTE Retry, BYTE ProcessorID, BYTE LSB, BYTE MSB, BYTE ReadLen)
{
    BYTE done, cunt;
    done = 0x00;
    
    PECI_HostEnable();
    PECI_HostControl(FIFOCLR);
    HOTRADDR = addr;
    HOWRLR = 0x05;
    HORDLR = ReadLen;
    if(Domain<2)
    {
        HOCMDR = PECI_CMD_RdIAMSR+Domain;
    }
    else
    {
        HOCMDR = PECI_CMD_RdIAMSR;
    }

    if(Retry<2)
    {
        HOWRDR = (PECI_HostID<<1)+Retry;
    }
    else
    {
        HOWRDR = (PECI_HostID<<1);
    }
    
    HOWRDR = ProcessorID;
    HOWRDR = LSB;
    HOWRDR = MSB;
        
    if(PECI_CheckHostBusy())
    {
        //PECI_HostControl(START+FCSERR_ABT+FIFOCLR);
        PECI_HostControl(START+FCSERR_ABT);
        if(PECI_CheckHostFinish())
        {
            for(cunt=0x00;cunt<HORDLR;cunt++)
            {
                if(cunt==0x00)
                {
                    PECI_CompletionCode = HORDDR;
                }
                else
                {
                    *(ReadData+cunt-1)=HORDDR;
                }
            }

            if(PECI_CompletionCode == PECI_CC_Valid)
            {
                done = 0x01;
            }
            else
            {
                done = 0x00;
            }
        }
    }

    PECI_HostDisable();
    ResetPECIStatus();
    return(done);
}

//-----------------------------------------------------------------------
// Write to Model Specific Registers function
//  return : 
//              1 : done
//              0 : error
//  Input : 
//              (1) addr : The address of processor
//              (2) *WriteData : the start address of variable to write data
//              (3) Domain : 0 or 1¡C 
//                           0 : Domain 0, 1 : Domain 1¡C
//              (4) Retry   0 or 1
//              (5) ProcessorID
//              (6) LSB of parameter
//              (7) MSB of parameter
//              (8) WriteLen write length 0x07 or 0x08 or 0x0A or 0x0E
//-----------------------------------------------------------------------
BYTE PECI_WrIAMSR(BYTE addr, XBYTE *WriteData, BYTE Domain, BYTE Retry, BYTE ProcessorID, BYTE LSB, BYTE MSB, BYTE WriteLen)
{
    BYTE done, cunt;
    done = 0x00;

    PECI_CompletionCode=0x00;
    PECI_HostEnable();

    #if PECI_Softwave_AWFCS
    PECI_HostControl(FIFOCLR);
    PECI_CRC8 = 0x00;
    #else
    PECI_HostControl(FIFOCLR+AWFCS_EN);
    #endif
    
    HOTRADDR = addr;
    PECI_CalcCRC8(addr);
    HOWRLR = WriteLen;
    PECI_CalcCRC8(WriteLen);
    HORDLR = 0x01;
    PECI_CalcCRC8(0x01);
    if(Domain<2)
    {
        HOCMDR = PECI_CMD_WrIAMSR+Domain;
        PECI_CalcCRC8(PECI_CMD_WrIAMSR+Domain);
    }
    else
    {
        HOCMDR = PECI_CMD_WrIAMSR;
        PECI_CalcCRC8(PECI_CMD_WrIAMSR);
    }

    if(Retry<2)
    {
        HOWRDR = (PECI_HostID<<1)+Retry;
        PECI_CalcCRC8((PECI_HostID<<1)+Retry);
    }
    else
    {
        HOWRDR = (PECI_HostID<<1);
        PECI_CalcCRC8(PECI_HostID<<1);
    }
    
    HOWRDR = ProcessorID;
    PECI_CalcCRC8(ProcessorID);
    HOWRDR = LSB;
    PECI_CalcCRC8(LSB);
    HOWRDR = MSB;
    PECI_CalcCRC8(MSB);

    for(cunt=0x00; cunt<(WriteLen-6); cunt++)
    {
        HOWRDR = *(WriteData+cunt);
        PECI_CalcCRC8(*(WriteData+cunt));
    }

    #if PECI_Softwave_AWFCS
    PECI_CRC8 ^= 0x80;          // Inverted MSb of preliminary FCS reslut
    HOWRDR = PECI_CRC8;
    #endif
        
    if(PECI_CheckHostBusy())
    {
        PECI_HostControl(START+FCSERR_ABT);
        if(PECI_CheckHostFinish())
        {
            PECI_CompletionCode = HORDDR;
            if(PECI_CompletionCode == PECI_CC_Valid)
            {
                done = 0x01;
            }
            else
            {
                done = 0x00;
            }
        }
    }

    PECI_HostDisable();
    ResetPECIStatus();
    return(done);
}

//-----------------------------------------------------------------------
// Read access to the PCI configuration
//  return : 
//              1 : done
//              0 : error
//  Input : 
//              (1) addr : The address of processor
//              (2) *ReadData : the start address of variable to save data
//              (3) Domain : 0 or 1¡C 
//                           0 : Domain 0, 1 : Domain 1¡C
//              (4) Retry   0 or 1
//              (5) external pointer, point to LSB variable
//              (6) ReadLen read length 0x02 or 0x03 or 0x05
//-----------------------------------------------------------------------
BYTE PECI_RdPCIConfig(BYTE addr, XBYTE *ReadData, BYTE Domain, BYTE Retry, XBYTE *pciaddr, BYTE ReadLen)
{
    BYTE done, cunt;
    done = 0x00;
    
    PECI_HostEnable();
    PECI_HostControl(FIFOCLR);
    HOTRADDR = addr;
    HOWRLR = 0x06;
    HORDLR = ReadLen;
    if(Domain<2)
    {
        HOCMDR = PECI_CMD_RdPCIConfig+Domain;
    }
    else
    {
        HOCMDR = PECI_CMD_RdPCIConfig;
    }

    if(Retry<2)
    {
        HOWRDR = (PECI_HostID<<1)+Retry;
    }
    else
    {
        HOWRDR = (PECI_HostID<<1);
    }

    for(cunt=0x00;cunt<4;cunt++)
    {
        HOWRDR = *(pciaddr+cunt);
    }
    
    if(PECI_CheckHostBusy())
    {
        //PECI_HostControl(START+FCSERR_ABT+FIFOCLR);
        PECI_HostControl(START+FCSERR_ABT);
        if(PECI_CheckHostFinish())
        {
            for(cunt=0x00;cunt<HORDLR;cunt++)
            {
                *(ReadData+cunt)=HORDDR;
            }

            if(*ReadData == PECI_CC_Valid)
            {
                done = 0x01;
            }
            else
            {
                done = 0x00;
            }
        }
    }

    PECI_HostDisable();
    ResetPECIStatus();
    return(done);
}

//-----------------------------------------------------------------------
// Write access to the PCI configuration
//  return : 
//              1 : done
//              0 : error
//  Input : 
//              (1) addr : The address of processor
//              (2) *WriteData : the start address of variable to write data
//              (3) Domain : 0 or 1¡C 
//                           0 : Domain 0, 1 : Domain 1¡C
//              (4) Retry   0 or 1
//              (5) Point to PCI configuration address (LSB)
//              (6) WriteLen write length 0x08 or 0x09 or 0x0B
//-----------------------------------------------------------------------
BYTE PECI_WrPCIConfig(BYTE addr, XBYTE *WriteData, BYTE Domain, BYTE Retry, XBYTE *PCIAddr, BYTE WriteLen)
{
    BYTE done, cunt;
    done = 0x00;

    PECI_CompletionCode=0x00;
    PECI_HostEnable();

    #if PECI_Softwave_AWFCS
    PECI_HostControl(FIFOCLR);
    PECI_CRC8 = 0x00;
    #else
    PECI_HostControl(FIFOCLR+AWFCS_EN);
    #endif
    
    HOTRADDR = addr;
    PECI_CalcCRC8(addr);
    HOWRLR = WriteLen;
    PECI_CalcCRC8(WriteLen);
    HORDLR = 0x01;
    PECI_CalcCRC8(0x01);
    if(Domain<2)
    {
        HOCMDR = PECI_CMD_WrPCIConfig+Domain;
        PECI_CalcCRC8(PECI_CMD_WrPCIConfig+Domain);
    }
    else
    {
        HOCMDR = PECI_CMD_WrPCIConfig;
        PECI_CalcCRC8(PECI_CMD_WrPCIConfig);
    }

    if(Retry<2)
    {
        HOWRDR = (PECI_HostID<<1)+Retry;
        PECI_CalcCRC8((PECI_HostID<<1)+Retry);
    }
    else
    {
        HOWRDR = (PECI_HostID<<1);
        PECI_CalcCRC8(PECI_HostID<<1);
    }
    
    for(cunt=0x00; cunt<4; cunt++)
    {
        HOWRDR = *(PCIAddr+cunt);
        PECI_CalcCRC8(*(PCIAddr+cunt));
    }

    for(cunt=0x00; cunt<(WriteLen-7); cunt++)
    {
        HOWRDR = *(WriteData+cunt);
        PECI_CalcCRC8(*(WriteData+cunt));
    }

    #if PECI_Softwave_AWFCS
    PECI_CRC8 ^= 0x80;          // Inverted MSb of preliminary FCS reslut
    HOWRDR = PECI_CRC8;
    #endif
        
    if(PECI_CheckHostBusy())
    {
        PECI_HostControl(START+FCSERR_ABT);
        if(PECI_CheckHostFinish())
        {
            PECI_CompletionCode = HORDDR;
            if(PECI_CompletionCode == PECI_CC_Valid)
            {
                done = 0x01;
            }
            else
            {
                done = 0x00;
            }
        }
    }

    PECI_HostDisable();
    ResetPECIStatus();
    return(done);
}

//-----------------------------------------------------------------------
// Read access to the PCI configuration space within the processor
//  return : 
//              1 : done
//              0 : error
//  Input : 
//              (1) addr : The address of processor
//              (2) *ReadData : the start address of variable to save data
//              (3) Domain : 0 or 1¡C 
//                           0 : Domain 0, 1 : Domain 1¡C
//              (4) Retry   0 or 1
//              (5) external pointer, point to LSB variable
//              (6) ReadLen read length 0x02 or 0x03 or 0x05
//-----------------------------------------------------------------------
BYTE PECI_RdPCIConfigLocal(BYTE addr, XBYTE *ReadData, BYTE Domain, BYTE Retry, XBYTE *pciaddr, BYTE ReadLen)
{
    BYTE done, cunt;
    done = 0x00;
    
    PECI_HostEnable();
    PECI_HostControl(FIFOCLR);
    HOTRADDR = addr;
    HOWRLR = 0x05;
    HORDLR = ReadLen;
    if(Domain<2)
    {
        HOCMDR = PECI_CMD_RdPCIConfigLocal+Domain;
    }
    else
    {
        HOCMDR = PECI_CMD_RdPCIConfigLocal;
    }

    if(Retry<2)
    {
        HOWRDR = (PECI_HostID<<1)+Retry;
    }
    else
    {
        HOWRDR = (PECI_HostID<<1);
    }

    for(cunt=0x00;cunt<3;cunt++)
    {
        HOWRDR = *(pciaddr+cunt);
    }
    
    if(PECI_CheckHostBusy())
    {
        //PECI_HostControl(START+FCSERR_ABT+FIFOCLR);
        PECI_HostControl(START+FCSERR_ABT);
        if(PECI_CheckHostFinish())
        {
            for(cunt=0x00;cunt<HORDLR;cunt++)
            {
                *(ReadData+cunt)=HORDDR;
            }

            if(*ReadData == PECI_CC_Valid)
            {
                done = 0x01;
            }
            else
            {
                done = 0x00;
            }
        }
    }

    PECI_HostDisable();
    ResetPECIStatus();
    return(done);
}

//-----------------------------------------------------------------------
// Write access to the PCI configuration space within the processor
//  return : 
//              1 : done
//              0 : error
//  Input : 
//              (1) addr : The address of processor
//              (2) *WriteData : the start address of variable to write data
//              (3) Domain : 0 or 1¡C 
//                           0 : Domain 0, 1 : Domain 1¡C
//              (4) Retry   0 or 1
//              (5) Point to PCI configuration address (LSB)
//              (6) WriteLen write length 0x07 or 0x08 or 0x0A
//-----------------------------------------------------------------------
BYTE PECI_WrPCIConfigLocal(BYTE addr, XBYTE *WriteData, BYTE Domain, BYTE Retry, XBYTE *PCIAddr, BYTE WriteLen)
{
    BYTE done, cunt;
    done = 0x00;

    PECI_CompletionCode=0x00;
    PECI_HostEnable();

    #if PECI_Softwave_AWFCS
    PECI_HostControl(FIFOCLR);
    PECI_CRC8 = 0x00;
    #else
    PECI_HostControl(FIFOCLR+AWFCS_EN);
    #endif
    
    HOTRADDR = addr;
    PECI_CalcCRC8(addr);
    HOWRLR = WriteLen;
    PECI_CalcCRC8(WriteLen);
    HORDLR = 0x01;
    PECI_CalcCRC8(0x01);
    if(Domain<2)
    {
        HOCMDR = PECI_CMD_WrPCIConfigLocal+Domain;
        PECI_CalcCRC8(PECI_CMD_WrPCIConfigLocal+Domain);
    }
    else
    {
        HOCMDR = PECI_CMD_WrPCIConfigLocal;
        PECI_CalcCRC8(PECI_CMD_WrPCIConfigLocal);
    }

    if(Retry<2)
    {
        HOWRDR = (PECI_HostID<<1)+Retry;
        PECI_CalcCRC8((PECI_HostID<<1)+Retry);
    }
    else
    {
        HOWRDR = (PECI_HostID<<1);
        PECI_CalcCRC8(PECI_HostID<<1);
    }
    
    for(cunt=0x00; cunt<3; cunt++)
    {
        HOWRDR = *(PCIAddr+cunt);
        PECI_CalcCRC8(*(PCIAddr+cunt));
    }

    for(cunt=0x00; cunt<(WriteLen-6); cunt++)
    {
        HOWRDR = *(WriteData+cunt);
        PECI_CalcCRC8(*(WriteData+cunt));
    }

    #if PECI_Softwave_AWFCS
    PECI_CRC8 ^= 0x80;          // Inverted MSb of preliminary FCS reslut
    HOWRDR = PECI_CRC8;
    #endif
        
    if(PECI_CheckHostBusy())
    {
        PECI_HostControl(START+FCSERR_ABT);
        if(PECI_CheckHostFinish())
        {
            PECI_CompletionCode = HORDDR;
            if(PECI_CompletionCode == PECI_CC_Valid)
            {
                done = 0x01;
            }
            else
            {
                done = 0x00;
            }
        }
    }

    PECI_HostDisable();
    ResetPECIStatus();
    return(done);
}