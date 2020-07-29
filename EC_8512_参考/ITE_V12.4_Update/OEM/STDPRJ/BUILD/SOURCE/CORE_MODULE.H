/*-----------------------------------------------------------------------------
 * Filename: CORE_MODULE.H
 * Function: CORE_MODULE.C prototype include definition
 *
 * Copyright (c) 2006-2009, ITE Tech. Inc. All Rights Reserved.
 *---------------------------------------------------------------------------*/
#ifndef CORE_MODULE_H
#define CORE_MODULE_H

/*-----------------------------------------------------------------------------
 * ExternalGPIO_8301. Code Parameter
 *---------------------------------------------------------------------------*/
#define TMB         5   // Transmitted Data Bits (n+1)
#define CDIE        0   // Cycle Done Interrupt 0-> disable, 1->Enable
#define CREN        1   // Channel Ready Enable
#define EGPC_CTRL   ((TMB<<2) + (CDIE<<1) + CREN)

#define EGPC_ID0            0x00
#define EGPC_ID1            0x01
#define EGPC_ID2            0x02
#define EGPC_IN             0x00
#define EGPC_OUT            0x02
#define EGPC_PULL_UP        0x04
#define EGPC_PULL_DN        0x08
#define EGPC_PUSH_PULL      0x00
#define EGPC_OPEN_DRAIN     0x10

#define GPIO00  (0x00<<2)   // Pin08
#define GPIO01  (0x01<<2)   // Pin09
#define GPIO02  (0x02<<2)   // Pin10
#define GPIO03  (0x03<<2)   // Pin11
#define GPIO04  (0x04<<2)   // Pin12
#define GPIO05  (0x05<<2)   // Pin13
#define GPIO06  (0x06<<2)   // Pin14
#define GPIO07  (0x07<<2)   // Pin15
#define GPIO08  (0x08<<2)   // Pin16
#define GPIO09  (0x09<<2)   // Pin17
#define GPIO10  (0x0A<<2)   // Pin18
#define GPIO11  (0x0B<<2)   // Pin19
#define GPIO12  (0x0C<<2)   // Pin20
#define GPIO13  (0x0D<<2)   // Pin21
#define GPIO14  (0x0E<<2)   // Pin22
#define GPIO15  (0x0F<<2)   // Pin23
#define GPIO16  (0x10<<2)   // Pin26
#define GPIO17  (0x11<<2)   // Pin27
#define GPIO18  (0x12<<2)   // Pin28
#define GPIO19  (0x13<<2)   // Pin29
#define GPIO20  (0x14<<2)   // Pin30
#define GPIO21  (0x15<<2)   // Pin31
#define GPIO22  (0x16<<2)   // Pin32
#define GPIO23  (0x17<<2)   // Pin33
#define GPIO24  (0x18<<2)   // Pin34
#define GPIO25  (0x19<<2)   // Pin35
#define GPIO26  (0x1A<<2)   // Pin36
#define GPIO27  (0x1B<<2)   // Pin37
#define GPIO28  (0x1C<<2)   // Pin38
#define GPIO29  (0x1D<<2)   // Pin39
#define GPIO30  (0x1E<<2)   // Pin40
#define GPIO31  (0x1F<<2)   // Pin41
#define GPIO32  (0x20<<2)   // Pin42
#define GPIO33  (0x21<<2)   // Pin43
#define GPIO34  (0x22<<2)   // Pin44
#define GPIO35  (0x23<<2)   // Pin45
#define GPIO36  (0x24<<2)   // Pin46
#define GPIO37  (0x25<<2)   // Pin47

/*-----------------------------------------------------------------------------
 * Data Structure Definition
 *---------------------------------------------------------------------------*/
struct sScanADC
{
    BYTE    ADC_ScanA;
    BYTE    ADC_ScanB;
    BYTE    ADC_ScanC;
    BYTE    ADC_ScanD;

};

typedef struct
{
    BYTE    Addr;
    BYTE    Data;
} EXGPIO;

/*-----------------------------------------------------------------------------
 * External Calling Process
 *---------------------------------------------------------------------------*/
extern void vScanADCInput( BYTE VCHNx, BYTE SELINx );
extern void CoreAutoScanADC(void);

extern BYTE InputDebounce( FLAG PIN_STATUS,
                            FLAG _ACTIVE, BYTE Counter_ID, BYTE ReloadCnt );

extern WORD GetFanRPM( BYTE FnTMRR, BYTE FnTLRR );

#if ExternalGPIO_8301
extern bit  EGPC_Read( BYTE Addr );
extern void EGPC_Write( BYTE Addr, BYTE Data );
extern void Set_EGPC( BYTE Addr );
extern void Clr_EGPC( BYTE Addr );
#endif  //ExternalGPIO_8301

extern void vOneWireDelayuS(BYTE Timer);
extern BYTE bResetOneWireDevice(void);
extern void vOneWireWriteBit(bit OneWirebit);
extern void vOneWireWriteByte(BYTE OneWireByte);
extern bit  vOneWireReadBit(void);
extern BYTE vOneWireReadByte(void);

/*-----------------------------------------------------------------------------
 * Internal Calling Process
 *---------------------------------------------------------------------------*/
static void GetVoltChnToRAM( BYTE VCHxCTL, BYTE VCHxDATM, BYTE VCHxDATL  );
static void SetAutoScanADC(void);
static BYTE SetScanVoltChannel( BYTE VCHxCTL, BYTE ADC_VCHx );


//-----------------------------------------------------------------------------
#endif