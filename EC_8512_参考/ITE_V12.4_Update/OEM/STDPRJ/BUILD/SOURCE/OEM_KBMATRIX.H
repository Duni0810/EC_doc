/*-----------------------------------------------------------------------------
 * Filename: OEM_KBMATRIX.H
 * Function: OEM_KBMATRIX.C Prototype Include Definition
 *
 * Copyright (c) 2006-2009, ITE Tech. Inc. All Rights Reserved.
 *---------------------------------------------------------------------------*/
#ifndef OEM_KBMATRIX_H
#define OEM_KBMATRIX_H

/*-----------------------------------------------------------------------------
 * External Calling Process
 *---------------------------------------------------------------------------*/
extern void HotKey_NULL(BYTE event);
extern void HotKey_Fn_F1(BYTE event);
extern void HotKey_Fn_F2(BYTE event);
extern void HotKey_Fn_F3(BYTE event);
extern void HotKey_Fn_F4(BYTE event);
extern void HotKey_Fn_F5(BYTE event);
extern void HotKey_Fn_F6(BYTE event);
extern void HotKey_Fn_F7(BYTE event);
extern void HotKey_Fn_F8(BYTE event);
extern void HotKey_Fn_F9(BYTE event);
extern void HotKey_Fn_F10(BYTE event);
extern void HotKey_Fn_F11(BYTE event);
extern void HotKey_Fn_F12(BYTE event);
extern void HotKey_Fn_DCH(BYTE event);
extern void HotKey_Fn_DDH(BYTE event);
extern void HotKey_Fn_DEH(BYTE event);
extern void HotKey_Fn_DFH(BYTE event);
extern void HotKey_Fn_E0H(BYTE event);
extern void HotKey_Fn_E1H(BYTE event);
extern void HotKey_Fn_E2H(BYTE event);
extern void HotKey_Fn_E3H(BYTE event);
extern void HotKey_Fn_E4H(BYTE event);
extern void HotKey_Fn_E5H(BYTE event);
extern void HotKey_Fn_E6H(BYTE event);
extern void HotKey_Fn_E7H(BYTE event);
extern void HotKey_Fn_E8H(BYTE event);
extern void HotKey_Fn_E9H(BYTE event);
extern void HotKey_Fn_EAH(BYTE event);
extern void HotKey_Fn_EBH(BYTE event);
extern void HotKey_Fn_ECH(BYTE event);
extern void HotKey_Fn_EDH(BYTE event);
extern void HotKey_Fn_EEH(BYTE event);
extern void HotKey_Fn_EFH(BYTE event);
extern const BYTE code sskey2_overlay_table[];
extern void costomer_function(BYTE code_byte, BYTE event);
extern bit  OemHandle_Data_To_Host( BYTE nKBData );


/*-----------------------------------------------------------------------------
 * External const Prototype
 *---------------------------------------------------------------------------*/

extern code BYTE Rc_ROM_Tables[];

extern const BYTE code ss2_9C[];
extern const BYTE code ss2_9D[];
extern const BYTE code ss2_9E[];
extern const BYTE code ss2_9F[];

extern const BYTE code ss2_A0[];
extern const BYTE code ss2_A1[];
extern const BYTE code ss2_A2[];
extern const BYTE code ss2_A3[];
extern const BYTE code ss2_A4[];
extern const BYTE code ss2_A5[];
extern const BYTE code ss2_A6[];
extern const BYTE code ss2_A7[];
extern const BYTE code ss2_A8[];
extern const BYTE code ss2_A9[];
extern const BYTE code ss2_AA[];
extern const BYTE code ss2_AB[];
extern const BYTE code ss2_AC[];
extern const BYTE code ss2_AD[];
extern const BYTE code ss2_AE[];
extern const BYTE code ss2_AF[];

extern const BYTE code ss2_B0[];
extern const BYTE code ss2_B1[];
extern const BYTE code ss2_B2[];
extern const BYTE code ss2_B3[];
extern const BYTE code ss2_B4[];
extern const BYTE code ss2_B5[];
extern const BYTE code ss2_B6[];
extern const BYTE code ss2_B7[];
extern const BYTE code ss2_B8[];
extern const BYTE code ss2_B9[];
extern const BYTE code ss2_BA[];
extern const BYTE code ss2_BB[];
extern const BYTE code ss2_BC[];
extern const BYTE code ss2_BD[];
extern const BYTE code ss2_BE[];
extern const BYTE code ss2_BF[];

extern const BYTE code ss2_C0[];
extern const BYTE code ss2_C1[];
extern const BYTE code ss2_C2[];
extern const BYTE code ss2_C3[];
extern const BYTE code ss2_C4[];
extern const BYTE code ss2_C5[];
extern const BYTE code ss2_C6[];
extern const BYTE code ss2_C7[];
extern const BYTE code ss2_C8[];
extern const BYTE code ss2_C9[];
extern const BYTE code ss2_CA[];
extern const BYTE code ss2_CB[];
extern const BYTE code ss2_CC[];
extern const BYTE code ss2_CD[];
extern const BYTE code ss2_CE[];
extern const BYTE code ss2_CF[];

/*-----------------------------------------------------------------------------
 * Internal Calling Process
 *---------------------------------------------------------------------------*/
//-----------------------------------------------------------------------------
#endif