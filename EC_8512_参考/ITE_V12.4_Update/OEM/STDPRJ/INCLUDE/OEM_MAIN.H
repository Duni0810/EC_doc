/*-----------------------------------------------------------------------------
 * Filename: OEM_MAIN.H
 * Function: OEM_MAIN.C Include Definition
 *
 * Copyright (c) 2006-2009, ITE Tech. Inc. All Rights Reserved.
 *---------------------------------------------------------------------------*/
#ifndef OEM_MAIN_H
#define OEM_MAIN_H

/*-----------------------------------------------------------------------------
 * OEM SCI/SMI Event Code Define
 *---------------------------------------------------------------------------*/
#define _SCIEVT_NULL                0x00    //Can't use
#define _SCIEVT_01H                 0x01    //Reserved
#define _SCIEVT_02H                 0x02    //Reserved
#define _SCIEVT_03H                 0x03    //Reserved
#define _SCIEVT_04H                 0x04    //Reserved
#define _SCIEVT_05H                 0x05    //Reserved
#define _SCIEVT_BATTERY             0x06    //Battery status changed
#define _SCIEVT_ADAPTER             0x07    //Adapter status changed
#define _SCIEVT_08H                 0x08    //Reserved
#define _SCIEVT_09H                 0x09    //Reserved
#define _SCIEVT_FnF1                0x0A    //Hotkey Fn+ F1 Event
#define _SCIEVT_FnF2                0x0B    //Hotkey Fn+ F2 Event
#define _SCIEVT_FnF3                0x0C    //Hotkey Fn+ F3 Event
#define _SCIEVT_FnF4                0x0D    //Hotkey Fn+ F4 Event
#define _SCIEVT_FnF5                0x0E    //Hotkey Fn+ F5 Event
#define _SCIEVT_FnF6                0x0F    //Hotkey Fn+ F6 Event
#define _SCIEVT_FnF7                0x10    //Hotkey Fn+ F7 Event
#define _SCIEVT_FnF8                0x11    //Hotkey Fn+ F8 Event
#define _SCIEVT_FnF9                0x12    //Hotkey Fn+ F9 Event
#define _SCIEVT_FnF10               0x13    //Hotkey Fn+F10 Event
#define _SCIEVT_FnF11               0x14    //Hotkey Fn+F11 Event
#define _SCIEVT_FnF12               0x15    //Hotkey Fn+F12 Event

#define _SCIEVT_SLEEP               0x31
#define _SCIEVT_LCD                 0x32

/*-----------------------------------------------------------------------------
 * External Calling Process
 *---------------------------------------------------------------------------*/
extern void Service_OEM_LoTimer(void);
extern void Service_LPCRST(void);
extern void Service_WaitPS2_Handle(void);
extern bit  OemSetWakeUpSource(void);
extern void InitWakeFromDeepSleep(void);
extern void InitEnterDeepSleep(void);
extern bit  vCheckChipCanDeepSleep(void);
extern void OEM_ColdResetInit(void);
extern void OEM_Init_SMBus(void);
extern void OEM_INIT_ACPI(void);
extern void LCD_BackLightBrightness(void);

/*-----------------------------------------------------------------------------
 * Internal Process Definition
 *---------------------------------------------------------------------------*/
static void Hook_Timer5msEvent(void);
static void Hook_Timer10msEvent(void);
static void Hook_Timer50msEventA(void);
static void Hook_Timer50msEventB(void);
static void Hook_Timer50msEventC(void);
static void Hook_Timer100msEventA(void);
static void Hook_Timer100msEventB(void);
static void Hook_Timer100msEventC(void);
static void Hook_Timer125msEvent(void);
static void Hook_Timer250msEvent(void);
static void Hook_Timer500msEventA(void);
static void Hook_Timer500msEventB(void);
static void Hook_Timer500msEventC(void);
static void Hook_Timer1SecEventA(void);
static void Hook_Timer1SecEventB(void);
static void Hook_Timer1SecEventC(void);
static void Hook_Timer1MinEvent(void);

//-----------------------------------------------------------------------------
#endif
