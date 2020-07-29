/*-----------------------------------------------------------------------------
 * TITLE: CORE_XLT.H - prototypes for CORE_XLT.C
 *
 * Copyright (c) 1983-2009, Insyde Software Corporation. All Rights Reserved.
 *
 * You may not reproduce, distribute, publish, display, perform, modify, adapt,
 * transmit, broadcast, present, recite, release, license or otherwise exploit
 * any part of this publication in any form, by any means, without the prior
 * written permission of Insyde Software Corporation.
 *---------------------------------------------------------------------------*/
#ifndef CORE_XLT_H
#define CORE_XLT_H

/*-----------------------------------------------------------------------------
 * Data Parameter definition
 *---------------------------------------------------------------------------*/
#define NUM_LOCK    BIT(0)
#define LShift      BIT(1)
#define RShift      BIT(2)
#define LAlt        BIT(3)
#define RAlt        BIT(3)
#define LCtrl       BIT(4)
#define RCtrl       BIT(4)
#define OVLAY       BIT(5)
#define FN          BIT(6)

#define SSKEY2_SPE_CODE 0x9C
#define SSKEY2_OVL_CODE 0xE0

#define MAKE_EVENT   0
#define BREAK_EVENT  1
#define REPEAT_EVENT 2

// Definitions for indices into Key Code Process Table.
#define DO_SIMPLE_CODE          0
#define DO_E0_PREFIX_CODE       1
#define DO_CURSOR_SHIFT         2
#define DO_CURSOR_NUMLOCK       3
#define DO_SPECIAL_CODE         4
#define DO_CONTROL_EFFECT       5
#define DO_CONTROL_EFFECT_E0    6
#define DO_COSTOMER_FUNCTION    7
#define DO_PrtSC_PREFIX_CODE    8

#define RC_TABLE_SIZE           128
#define COMB_TABLE_SIZE         16
#define RC_TABLE_INDEX          0
#define CUSTOM_KEY_TABLE_INDEX  (COMB_TABLE_INDEX + COMB_TABLE_SIZE)
#define COMB_TABLE_INDEX        (RC_TABLE_INDEX + RC_TABLE_SIZE)

/*-----------------------------------------------------------------------------
 * Data Structure definition
 *---------------------------------------------------------------------------*/
/* ----------------------------------------------------------------------------
 * FUNCTION: A2_TABLE
 * BYTE comb        - Bit  Combination Key Setting
 *                    ---  -----------------------
 *                     7
 *                     6
 *                     5   Fn
 *                     4   Numlock
 *                     3   Ctrl
 *                     2   Alt
 *                     1   Shift
 *                     0   reserved
 *
 * const BYTE *pntr - Pointer to array for key
 * ------------------------------------------------------------------------- */
typedef struct _A2_TABLE
{
    BYTE comb;              // Combination Key Setting
    const BYTE code *pntr;  // Pointer to array for key
} code A2_TABLE;
//-----------------------------------------------------------------------------

/*-----------------------------------------------------------------------------
 * External const Prototype
 *---------------------------------------------------------------------------*/
extern const A2_TABLE sskey2_A2_table[];
extern const BYTE code sskey2_overlay_table[];
extern const BYTE code sskey3_80_table[];
extern const BYTE code SS_Qkey_table[];
extern void  LoadKBMatrixToRAM(void);

/*-----------------------------------------------------------------------------
 * External Calling Process
 *---------------------------------------------------------------------------*/
extern void Send_Scan2(union KEY key, BYTE event);
extern void Send_Key(BYTE table_entry, BYTE event);
extern void Clear_Fn_Keys(void);
extern void ex_Clear_Key(void);
extern void e0_prefix_code(BYTE scan_code, BYTE event);
extern void simple_code(BYTE scan_code, BYTE event);
extern void cursor_shift(BYTE scan_code, BYTE event);

/*-----------------------------------------------------------------------------
 * Internal Process Definition
 *---------------------------------------------------------------------------*/
static BYTE fn_ov_comb_proc(BYTE index);
static void sskey3_proc(BYTE sskey2, BYTE event);
static BYTE custom_key_proc(BYTE index, BYTE event);
static BYTE calc_index_comb(BYTE comb);
static void cursor_numlock(BYTE scan_code, BYTE event);
static void special_code(BYTE code_byte, BYTE event);
static void control_effect(BYTE state, BYTE event);
static void control_effect_e0(BYTE state, BYTE event);
static void PrintScreen_prefix_code(BYTE scan_code, BYTE event);

static void custom_code_proc(const BYTE *pntr, BYTE event);
static BYTE any_combination(WORD data_word, const BYTE *pntr);


//-----------------------------------------------------------------------------
#endif
