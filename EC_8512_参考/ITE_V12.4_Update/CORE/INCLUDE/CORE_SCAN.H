/*-----------------------------------------------------------------------------
 * TITLE: CORE_SCAN.H
 *
 * Copyright (c) 1983-2009, Insyde Software Corporation. All Rights Reserved.
 *
 * You may not reproduce, distribute, publish, display, perform, modify, adapt,
 * transmit, broadcast, present, recite, release, license or otherwise exploit
 * any part of this publication in any form, by any means, without the prior
 * written permission of Insyde Software Corporation.
 *---------------------------------------------------------------------------*/
#ifndef CORE_SCAN_H
#define CORE_SCAN_H

/*-----------------------------------------------------------------------------
 * Matrix KB Control Parameter
 *---------------------------------------------------------------------------*/
#define KB_TypematicDelay   0x03
#define KB_TypematicRate    0x05
#define KB_PrescaleCounter  0x04
#define TM_SCALE            4
#define TM_REPEAT           4
#define TM_DELAY            30

#define STANDARD_SCAN_LINES 16
#define MAX_SCAN_LINES      19
#define KBF_SIZE            16
#define RESPONSE_TIME       9
#define shiftSCAN_SELECT    6
#define maskSCAN_SELECT     0x03
#define LAST_SCAN_TBL_CNT   0

/*-----------------------------------------------------------------------------
 * External Calling Prototype
 *---------------------------------------------------------------------------*/
extern void Service_KEYSCAN(void);
extern void Lock_Scan(void);
extern void Unlock_Scan(void);
extern bit  Is_Key_Buffer_Ready(void);
extern FLAG Buffer_Key(BYTE row_column);
extern const unsigned char code EtKey_ROM_Tables[];
/* For standard keys */
extern data union KEY new_keyh;
extern data union KEY typematic;
extern data struct Sscan scan;
extern data union Unew_keyl new_keyl;
/* For extend keys */
extern data union KEY new_extendkey;
extern data struct Sscan extendscan;
extern data union Unew_keyl extendstatus;
extern data union KEY etkeytypematic;
extern void Start_Scan_Transmission(void);
extern FLAG Check_Scan_Transmission(void);
extern void Init_Kbd(void);
extern void Setup_Diode_Key(void);
extern void Clear_Key(void);
extern void Clear_Typematic(void);
extern void Set_Typematic(BYTE type_rate);
extern WORD Get_Buffer(void);
extern void Buffer_Mark(void);
extern BYTE Buffer_String(const BYTE *pntr);
extern BYTE scan_keys(void);
extern void Enable_Any_Key_Irq(void);
extern void Write_Strobe(BYTE scan_line_num);
extern BYTE Read_Scan_Lines(void);
extern void Scan_Init(void);
extern void Setup_Scanner_Pntr(void);
extern FLAG Get_Kbd_Type(void);
extern void Setup_Ext_Hotkey_Pntr(void);
extern void Oem_ScanActivity(void);

/*-----------------------------------------------------------------------------
 * Internal Process Prototype
 *---------------------------------------------------------------------------*/
static BYTE *Hookc_Get_Ext_Hotkey_Pntr(void);
static void check_tm(union KEY key);
static void debounce_key(void);
static void ex_change_valid(void);
static void check_scan(BYTE changes, BYTE scan_address);
static void cscfnd(BYTE bit_num, BYTE scan_address);
static void setup_debounce(BYTE bit_num, BYTE scan_address, BYTE event);
static FLAG find_paths(BYTE change_make_key, BYTE scan_address);
static BYTE bScanExtendKeys(void);
static void ex_Write_Strobe(BYTE scan_line);
static void ex_check_scan(BYTE KSI_bit_num, BYTE scan_address);
static void ex_setup_debounce(BYTE KSI_bit_num, BYTE scan_address, BYTE event);
static void ex_debounce_key(void);
static void vCheckExtendKeyValid(void);
static void ex_Send_Scan2(union KEY key, BYTE event);
static void vClearExtendKeys(void);
static void vCheckEtKeystm(union KEY key);
static BYTE ex_scan_keys(void);
static void ex_check_tm(union KEY key);
static void ex_cscfnd(BYTE SCAN_Cnt, BYTE scan_address);
static void change_valid(void);


//-----------------------------------------------------------------------------
#endif
