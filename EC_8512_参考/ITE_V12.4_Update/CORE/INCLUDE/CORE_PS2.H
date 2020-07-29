/*-----------------------------------------------------------------------------
 * TITLE: CORE_PS2.H - Header File for PS/2 device Handler (PS2.C)
 *
 * Copyright (c) 1983-2009, Insyde Software Corporation. All Rights Reserved.
 *
 * You may not reproduce, distribute, publish, display, perform, modify, adapt,
 * transmit, broadcast, present, recite, release, license or otherwise exploit
 * any part of this publication in any form, by any means, without the prior
 * written permission of Insyde Software Corporation.
 *---------------------------------------------------------------------------*/
#ifndef CORE_PS2_H
#define CORE_PS2_H

//-----------------------------------------------------------------------------
// PS2 Interface Debug
//-----------------------------------------------------------------------------
#define PS2_Pack_Debug      0
#define PS2_KBCMD           0           //Enable Debug keyboard command to keyboard and ACK bytes
#define PS2_MSCMD           0           //Enable Debug keyboard command to TP or moues and ACK bytes

#define PS2_AUXTypeDebug    0
//**************************************
#define PS2_Pack_Debug_Index    0x77
#define PS2_AUX_PlugIn          0x88
#define PS2_AUX_StatsuUpdate    0x99

#define PS2_AUXType_MS          0xAA
#define PS2_AUXType_KB          0xAB
#define PS2_KB_Strange          0xFF

#define PS2_AUXType_F2_TimeOut  0xA0
#define FF_ACK_FA_TimeOut       0xA1
#define FF_ACK_AA_TimeOut       0xA2
#define FF_ACK_00_TimeOut       0xA3
#define FF_ACK_00_TimeDone      0xA4
//**************************************

#define PS2_Port0_CLK_CtrlReg	GPCRF0
#define PS2_Port0_Data_CtrlReg	GPCRF1
#define PS2_Port1_CLK_CtrlReg	GPCRF2
#define PS2_Port1_Data_CtrlReg	GPCRF3
#define PS2_Port2_CLK_CtrlReg	GPCRF4
#define PS2_Port2_Data_CtrlReg	GPCRF5

#define ScanAUXStep_ScanPort    0xF0
#define ScanAUXStep_ScanPort0   0xF0
#define ScanAUXStep_ScanPort1   0xF1
#define ScanAUXStep_ScanPort2   0xF2

#define ScanAUXStep_WaitPort    0xE0
#define ScanAUXStep_WaitPort0   0xE0
#define ScanAUXStep_WaitPort1   0xE1 
#define ScanAUXStep_WaitPort2   0xE2

#define ScanAUXStep_Skip        0xD0
#define ScanAUXStep_Skip0       0xD0
#define ScanAUXStep_Skip1       0xD1
#define ScanAUXStep_Skip2       0xD2

#define ScanAUXStep_Init        0xC0
#define ScanAUXStep_Init0       0xC0
#define ScanAUXStep_Init1       0xC1
#define ScanAUXStep_Init2       0xC2

#define ScanAUXStep_InitFail    0xC8
#define ScanAUXStep_InitFail0   0xC8
#define ScanAUXStep_InitFail1   0xC9
#define ScanAUXStep_InitFail2   0xCA

#define ScanAUXStep_CheckType   0xB0
#define ScanAUXStep_CheckType0  0xB0
#define ScanAUXStep_CheckType1  0xB1
#define ScanAUXStep_CheckType2  0xB2

#define ScanAUXStep_UpdateKB    0xA0
#define ScanAUXStep_UpdateKB0   0xA0
#define ScanAUXStep_UpdateKB1   0xA1
#define ScanAUXStep_UpdateKB2   0xA2

#define ScanAUXStep_CheckOK     0x80
#define ScanAUXStep_CheckOK1    0x80
#define ScanAUXStep_CheckOK2    0x81
#define ScanAUXStep_CheckOK3    0x82

#define DeviceResetCmdITTime    100         // 1000 ms
#define KBCmdBusyInactiveTime   10          // 90 ~ 100 ms
#define InactiveTime		    6           // 50 ~ 60 ms
#define CheckDeviceTimeOut      4           // 30 ~ 40 ms
#define SkipScanAUXTime         150         // Skip scan aux devices for keyboard or mouse driver init. (10ms timer base)
#define PS2_OnOffTimeOutCUNT    5

#define PS2_ReceiveMode         0x17
#define PS2_InhibitMode         0x11

#define MouseDataPending        0x33
#define MouseIRQPending         0x44

extern void Ps2_Init(void);
extern void CheckAUXDeviceType(void);
extern void vSendCmdtoMouse(BYTE PortNum);
extern void vSendCmdtoMUX(BYTE port);
extern void ECSend2Port( BYTE PortNum, BYTE PortData);
extern void Send2Port( BYTE PortNum, BYTE PortData);
extern BYTE Send2PortNWait( BYTE PortNum, BYTE cmd, BYTE bytecunt);
extern void PollingAUXDeviceCenter(void);
extern void Service_PS2_IRQ(void);
extern void vPS2DeviceInactive(void);
extern void PS2Deviceactive(void);
extern void vInhibitAUXDevice(void);
extern void SendFromAux(BYTE auxdata);
extern void SetPS2InhibitTime(BYTE time);
extern void ScanAUXDevice(void);
extern void ScanAUXDeviceStep(void);
extern void ScanAUXPortX(void);
extern void EnableAUXDevice(void);
extern void ConfigExtKeyboard(BYTE portnum);
extern BYTE vSetIntellimouse(BYTE DriverType, BYTE port);
extern BYTE ReadMouseID(BYTE port);
extern BYTE ConfigExtMouse(BYTE portnum);
extern void vSendtoExtKeyboaard(BYTE cmd);
extern BYTE bExtAUXTimeOutCheck(BYTE channel);
extern BYTE vCheckAuxMux(void);
extern void ISR_Aux_Port1(void);
extern void ISR_Aux_Port2(void);
extern void ISR_Aux_Port3(void);
extern void KBCmdAckByteCunt(BYTE ackcunt);
extern void SetOtherKBNeedUpdataFlag(BYTE KBCmd);
extern void SetStartScanAUXFlag(void);
extern void AUXData_To_Host_nWait(BYTE data_byte, BYTE portnum);
extern void ReSendPS2PendingData(void);
extern void CheckNSetMainKBCHN(BYTE PortNum);
extern void CheckNSetMainMSCHN(BYTE PortNum);
extern void ResetMSPendingData(void);
extern BYTE CheckMousePS2IFBusy(BYTE PortNum);
extern void DisablePS2Port_0(void);
extern void DisablePS2Port_1(void);
extern void DisablePS2Port_2(void);
extern void EnablePS2Port_0(void);
extern void EnablePS2Port_1(void);
extern void EnablePS2Port_2(void);
extern void KBCmdCheckMouseBusy(BYTE KBCmd);
extern void PS2_PowerOff(void);
extern void PS2_PowerOn(void);

typedef struct PS2Struct
{
    XBYTE 	*ps2ctrl;
    XBYTE 	*ps2int;
    XBYTE 	*ps2status;	
	XBYTE 	*ps2data;
	XBYTE 	*ps2ier;
    XBYTE 	*ps2isr;
	BYTE	ctrlbit;
} sPS2Struct;
extern const sPS2Struct code asPS2Struct[];

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#endif
