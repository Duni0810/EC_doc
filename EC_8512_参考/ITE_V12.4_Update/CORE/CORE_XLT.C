/*-----------------------------------------------------------------------------
 * TITLE: CORE_XLT.C - Scan Matrix handler at CORE level
 *
 * The procedure Send_Scan2 of this module is called to translate keyboard
 * scanner contact events into set 2 scan codes.
 *
 * Copyright (c) 1983-2009, Insyde Software Corporation. All Rights Reserved.
 *
 * You may not reproduce, distribute, publish, display, perform, modify, adapt,
 * transmit, broadcast, present, recite, release, license or otherwise exploit
 * any part of this publication in any form, by any means, without the prior
 * written permission of Insyde Software Corporation.
 *---------------------------------------------------------------------------*/
#include "CORE_INCLUDE.H"
#include "OEM_INCLUDE.H"

/*-----------------------------------------------------------------------------
 * Local Parameter Definition
 *---------------------------------------------------------------------------*/
#define ENABLE_SSKEY2_A2_CX          1
#define ENABLE_SSKEY2_A2_DX          0

/* ----------------------------------------------------------------------------
 * FUNCTION: Send_Scan2
 *
 * Input: key   = Row/Column number
 *                key.field.output = 0 - 15
 *                key.field.input  = 0 - 7
 *
 *        event = Contact event (MAKE_EVENT, BREAK_EVENT, or REPEAT_EVENT)
 *
 *        Scanner_State byte
 *
 *        Ext_Cb3_OVL_STATE
 *
 * On return, Scanner_State byte and Ext_Cb3_OVL_STATE are updated.
 *
 * Combination flag for each row/column
 *     0 = Insyde Software key number 2 in Row/Column Table
 *     1 = index for control_ptr_table in Row/Column Table
 *
 * Insyde Software Key Number 2
 *     01-7F = Same as Scan Code, Set 2
 *     80-FF = E0-prefix, cursor, printscreen, pause/break, special key
 * ------------------------------------------------------------------------- */
void Send_Scan2(union KEY key, BYTE event)
{
    #if SUPPORT_MATRIX_UTILITY
    if(IsFlag1(UtilityFlag,KBDebug))
    {
        if(KeyScanACK==0xFF)
        {
            if(event==MAKE_EVENT)
            {
                KeyScanKSI = key.field.input;
                KeyScanKSO = key.field.output;
                KeyScanACK = 0x33;
            }
        }
        return;
    }
    #endif

    #if SUPPORT_KB_RAM_TABLE
    Scanner_RAM_Pntr     = &Rc_RAM_Tables;
    Extendkey_RAM_Pntr   = &EtKey_RAM_Tables;
    Scanner_Table_Pntr   = &Rc_ROM_Tables;
    Extendkey_Table_Pntr = &EtKey_ROM_Tables;

    _R4 = (key.field.input << 4) | key.field.output;
    if( KB_USE_RAM_TABLE )
    {
        _R3 = *(Scanner_RAM_Pntr + _R4);
    }
    else
    {
        _R3 = *(Scanner_Table_Pntr + _R4);
    }
    #else
    Scanner_Table_Pntr   = &Rc_ROM_Tables;
    Extendkey_Table_Pntr = &EtKey_ROM_Tables;
    _R4 = (key.field.input << 4) | key.field.output;
    _R3 = *(Scanner_Table_Pntr + _R4);
    #endif
    if( (DISABLE_WINKEY)  )
    {
        if( (_R3 == 0x82)||(_R3 == 0x83) )
        {
            _R3 = 0x00;
        }
    }
    Send_Key(_R3, event);
}
//-----------------------------------------------------------------------------

/* ----------------------------------------------------------------------------
 * FUNCTION: LoadKBMatrixToRAM
 * ------------------------------------------------------------------------- */
void LoadKBMatrixToRAM(void)
{
    #if SUPPORT_KB_RAM_TABLE
    BYTE i;

    for( i=0; i<0x80; i++ )
    {
        Tmp_XPntr = ( Rc_RAM_Tables + i );
        *Tmp_XPntr = Rc_ROM_Tables[i];
    }
    for( i=0; i<0x18; i++ )
    {
        Tmp_XPntr = ( EtKey_RAM_Tables + i );
        *Tmp_XPntr = EtKey_ROM_Tables[i];
    }
    #endif
}
//-----------------------------------------------------------------------------

/* ----------------------------------------------------------------------------
 * FUNCTION: Send_Key
 * ------------------------------------------------------------------------- */
void Send_Key(BYTE table_entry, BYTE event)
{
    BYTE temp;
    const BYTE code *pntr;

    //Buffer_Mark();    /* Mark Buffer in case of overrun.*/
    temp_scanner_state.byte = Scanner_State;
    if (table_entry >= SSKEY2_OVL_CODE)     /* Fn key + any key.(>=0xE0)*/
    {
        temp = (table_entry - SSKEY2_OVL_CODE);
        temp = temp << 1;
        /* Multiply 2 because each entry takes 2 bytes.*/
        if (temp_scanner_state.field.tOVERLAY ^ temp_scanner_state.field.tFN)
        {                                           /* Fn key */
            temp++; /* Increment index to get the odd byte of table entry.*/
        }
        table_entry = sskey2_overlay_table[temp];   /* Get a sskey2 value.*/
    }
    else if (table_entry >= SSKEY2_SPE_CODE)    /* Combination Key.(>=0x9C)*/
    {
        temp = (table_entry - SSKEY2_SPE_CODE);     /* Get index into table.*/
        table_entry = sskey2_A2_table[temp].comb;
        /* table_entry contains the combination table entry.*/
        pntr = sskey2_A2_table[temp].pntr;
        /* pntr points to the first key byte.*/
        pntr += calc_index_comb(table_entry);
        /* Check combination.  Point to the correct byte.*/
        table_entry = *pntr;                    /* Get a sskey2.*/
    }

    sskey3_proc(table_entry, event);            /* Generate scan code, set 2.*/

    Scanner_State = temp_scanner_state.byte;    /* Update scanner state.*/
    //Ext_Cb3_OVL_STATE = temp_scanner_state.field.tOVERLAY;
}
//-----------------------------------------------------------------------------

/* ----------------------------------------------------------------------------
 * FUNCTION: Clear_Fn_Keys
 * ------------------------------------------------------------------------- */
void Clear_Fn_Keys(void)
{
    Fn_Make_Key1 = 0;
}
//-----------------------------------------------------------------------------

/* ----------------------------------------------------------------------------
 * FUNCTION: sskey3_proc
 *
 * Input: sskey2 = Insyde Software Key 2
 *        event =  Contact event (MAKE_EVENT, BREAK_EVENT, or REPEAT_EVENT)
 *
 * Note: FUNCT_PTR_V_BS is a pointer to a function that returns nothing (V for
 *       void) and takes a BYTE and a BYTE for parameters (B for BYTE, S for
 *       BYTE).
 * ------------------------------------------------------------------------- */
// Key Code Process Table.

const FUNCT_PTR_V_BS code kcp_vector_table[] =
{
    simple_code,        // index 00h DO_SIMPLE_CODE IBM key number 83,84
    e0_prefix_code,     // index 01h DO_E0_PREFIX_CODE
    cursor_shift,       // index 02h DO_CURSOR_SHIFT
    cursor_numlock,     // index 03h DO_CURSOR_NUMLOCK
    special_code,       // index 04h DO_SPECIAL_CODE IBM key number 126
    control_effect,     // index 05h DO_CONTROL_EFFECT Shift,Ctrl,Alt,Numlock
    control_effect_e0,  // index 06h DO_CONTROL_EFFECT_E0 Ctrl, Alt
    costomer_function,  // index 07h DO_COSTOMER_FUNCTION costomer function key
    PrintScreen_prefix_code,    // index 08h DO_PrtSc_PREFIX_CODE

};


static void sskey3_proc(BYTE sskey2, BYTE event)
{
    BYTE code_byte;
    BYTE index;
    FLAG quick_key = 0;

    if (sskey2 == 0)                // Null code
    {
        ;
    }
    else if ((sskey2 & 0x80) == 0)
    {
        simple_code(sskey2, event); // 01h through 7Fh = scan code.
    }
    else                            // 80h through FFh.
    {
        sskey2 &= 0x7F;
        sskey2 = sskey2 << 1;
        code_byte = sskey3_80_table[sskey2+0];
        index = sskey3_80_table[sskey2+1];
        (kcp_vector_table[index])(code_byte, event); // Do procedure
    }
}
//-----------------------------------------------------------------------------

/* ----------------------------------------------------------------------------
 * FUNCTION: calc_index_comb
 *
 * Input: combination setting
 *             Bit   Meaning
 *             ---   --------
 *             7     reserved
 *             6     Overlay
 *             5     Fn
 *             4     Num Lock
 *             3     Ctrl
 *             2     Alt
 *             1     Shift
 *             0     reserved
 *
 *       temp_scanner_state
 *
 * Return: offset.
 * ------------------------------------------------------------------------- */
static BYTE calc_index_comb(BYTE comb)
{
    BYTE offset;
    BYTE bit_num;

    offset  = 0;
    bit_num = 0;

    if( comb & BIT(1) )
    {                                       // Combination has Shift.
        if( temp_scanner_state.field.tLEFT || temp_scanner_state.field.tRIGHT )
        {   /* Either left or right shift is pressed.   */
            offset |= Byte_Mask(bit_num);
        }
        bit_num++;                          // Increment bit position.
    }

    if( comb & BIT(2) )
    {                                       // Combination has Alt.
        if( temp_scanner_state.field.tALT )
        {                                   // Alt is pressed.
            offset |= Byte_Mask(bit_num);
        }
        bit_num++;                          // Increment bit position.
    }

    if( comb & BIT(3) )
    {                                       // Combination has Ctrl.
        if( temp_scanner_state.field.tCONTROL )
        {                                   // Ctrl is pressed.
            offset |= Byte_Mask(bit_num);
        }
        bit_num++;                          // Increment bit position.
    }

    if( comb & BIT(4) )
    {   // Combination has Num Lock.
        if( !bNumPad_OnlyWithFn )
        {
            if( temp_scanner_state.field.tNUM_LOCK || bNumPad_UpLayer )
            {   // NumLock has been pressed.
                if( temp_scanner_state.field.tFN )
                {
                    return(offset);
                }
                offset |= Byte_Mask(bit_num);
            }
            bit_num++;                          // Increment bit position.
        }
    }

    if( comb & BIT(5) )      // Combination has Fn.
    {
        if( temp_scanner_state.field.tFN )
        {                                   // Fn is pressed.
            offset |= Byte_Mask(bit_num);
        }
        bit_num++;                          // Increment bit position.
    }

    if( comb & BIT(6) )
    {
        if( temp_scanner_state.field.tOVERLAY ) //Combination has Overlay.
        {
            offset |= Byte_Mask(bit_num);       //Overlay is pressed.
        }
    }
    return(offset);

}
//-----------------------------------------------------------------------------

/* ----------------------------------------------------------------------------
 * FUNCTION: simple_code
 *
 * Input: scan_code, Set 2
 *        event = Contact event (MAKE_EVENT, BREAK_EVENT, or REPEAT_EVENT)
 * ------------------------------------------------------------------------- */
void simple_code(BYTE scan_code, BYTE event)
{
    if (event == BREAK_EVENT)  // Buffer break prefix for break contact.
    {
        Buffer_Key(0xF0);      // F0h is break prefix.
    }
    Buffer_Key(scan_code);  // Buffer base code.
}
//-----------------------------------------------------------------------------

/* ----------------------------------------------------------------------------
 * FUNCTION: e0_prefix_code
 *
 * Input: scan_code, Set 2
 *        event = Contact event (MAKE_EVENT, BREAK_EVENT, or REPEAT_EVENT)
 * ------------------------------------------------------------------------- */
void e0_prefix_code(BYTE scan_code, BYTE event)
{

    //if( (event == MAKE_EVENT) && (Buffer_Key_Space() < 5 )  )
    //{
    //    return;
    //}

    Buffer_Key(0xE0);       // Buffer E0h prefix.
    if (event == BREAK_EVENT)
    {                       // Buffer break prefix for break contact.
        Buffer_Key(0xF0);   // F0h is break prefix.
    }
    Buffer_Key(scan_code);  // Buffer base code.
}
//-----------------------------------------------------------------------------

/* ----------------------------------------------------------------------------
 * FUNCTION: e0_12_prefix_code
 *
 * Input: scan_code, Set 2
 *        event = Contact event (MAKE_EVENT, BREAK_EVENT, or REPEAT_EVENT)
 * ------------------------------------------------------------------------- */
void PrintScreen_prefix_code(BYTE scan_code, BYTE event)
{
    bit bNeed_E0_12=1;

    if( temp_scanner_state.field.tLEFT || temp_scanner_state.field.tRIGHT )
    {
        if( temp_scanner_state.field.tCONTROL )
        {
            bNeed_E0_12 = 0;
        }
    }
    if( temp_scanner_state.field.tALT )
    {
        if (event == BREAK_EVENT)
        {
            Buffer_Key(0xF0);   // F0h is break prefix.
        }
        Buffer_Key(0x84);       // Buffer 84h prefix.
        return;
    }

    if (event == MAKE_EVENT)
    {
        Buffer_Key(0xE0);       // Buffer E0h prefix.
        if( bNeed_E0_12 )
        {
            Buffer_Key(0x12);   // Buffer 12h prefix.
            Buffer_Key(0xE0);   // Buffer E0h prefix.
        }
        Buffer_Key(scan_code);  // Buffer base code.
    }
    if (event == BREAK_EVENT)
    {                           // Buffer break prefix for break contact.
        Buffer_Key(0xE0);       // Buffer E0h prefix.
        Buffer_Key(0xF0);       // F0h is break prefix.
        Buffer_Key(scan_code);  // Buffer base code.
        if( bNeed_E0_12 )
        {
            Buffer_Key(0xE0);   // Buffer E0h prefix.
            Buffer_Key(0xF0);   // F0h is break prefix.
            Buffer_Key(0x12);   // Buffer base code.
        }
    }
}
//-----------------------------------------------------------------------------

/* ----------------------------------------------------------------------------
 * FUNCTION: cursor_shift
 *
 * Input: scan_code, Set 2
 *        event = Contact event (MAKE_EVENT, BREAK_EVENT, or REPEAT_EVENT)
 *        temp_scanner_state
 * ------------------------------------------------------------------------- */
/* BREAK_EVENT key string for cursor_shift and cursor_numlock.               */
const BYTE code csr_brk1[] = {0xE0,0xF0,0x00};

void cursor_shift(BYTE scan_code, BYTE event)
{
    static const BYTE code csr_sftl_mk[]   = {0xE0,0xF0,0x12,0x00};
    static const BYTE code csr_sftr_mk[]   = {0xE0,0xF0,0x59,0x00};
    static const BYTE code csr_sftl_brk2[] = {0xE0,0x12,0x00};
    static const BYTE code csr_sftr_brk2[] = {0xE0,0x59,0x00};

    if( temp_scanner_state.field.tNUM_LOCK )
    {
        if (event == BREAK_EVENT)
        {   /* Key has just been released. This is a "break event". */
            Buffer_String(csr_brk1);            /* Buffer pre-string. */
            Buffer_Key(scan_code);              /* Buffer base code. */
            if( temp_scanner_state.field.tLEFT || temp_scanner_state.field.tRIGHT )
                return;
            Buffer_String(csr_sftl_mk);     /* Buffer pre-string. */
        }
        else if (event == MAKE_EVENT)
        {
            if( !temp_scanner_state.field.tLEFT && !temp_scanner_state.field.tRIGHT )
            {
                Buffer_String(csr_sftl_brk2);   /* Buffer tail-string. */
            }
            Buffer_Key(0xE0);                   /* Buffer end of pre-string. */
            Buffer_Key(scan_code);              /* Buffer base code. */
        }
        else
        {   /* Key is being held pressed for a length of time, a "repeat event". */
            Buffer_Key(0xE0);                   /* Buffer E0h prefix */
            Buffer_Key(scan_code);              /* Buffer base code */
        }
        return;
    }
    if (event == BREAK_EVENT)
    {   /* Key has just been released. This is a "break event". */
        Buffer_String(csr_brk1);            /* Buffer pre-string. */
        Buffer_Key(scan_code);              /* Buffer base code. */

        if (temp_scanner_state.field.tRIGHT)
        {                                   /* Right shift has been pressed. */
            Buffer_String(csr_sftr_brk2);   /* Buffer tail-string. */
        }
        if (temp_scanner_state.field.tLEFT)
        {                                   /* Left shift has been pressed. */
            Buffer_String(csr_sftl_brk2);   /* Buffer tail-string. */
        }
    }
    else if (event == MAKE_EVENT)
    {   /* Key is pressed for the first time, a "make event". */
        /* Left and/or Right SHIFT is pressed.                */
        if (temp_scanner_state.field.tLEFT)
        {                                   /* Left SHIFT is pressed. */
            Buffer_String(csr_sftl_mk);     /* Buffer pre-string. */
        }

        if (temp_scanner_state.field.tRIGHT)
        {                                   /* Right SHIFT is pressed. */
            Buffer_String(csr_sftr_mk);     /* Buffer pre-string. */
        }

        Buffer_Key(0xE0);                   /* Buffer end of pre-string. */
        Buffer_Key(scan_code);              /* Buffer base code. */
    }
    else
    {   /* Key is being held pressed for a length of time, a "repeat event". */
        Buffer_Key(0xE0);                   /* Buffer E0h prefix */
        Buffer_Key(scan_code);              /* Buffer base code */
    }
}


/* ----------------------------------------------------------------------------
 * FUNCTION: cursor_numlock
 *
 * Input: scan_code, Set 2
 *        event = Contact event (MAKE_EVENT, BREAK_EVENT, or REPEAT_EVENT)
 * ------------------------------------------------------------------------- */
static void cursor_numlock(BYTE scan_code, BYTE event)
{
    static const BYTE code csr_numlock_mk[]   = {0xE0,0x12,0xE0,0x00};
    static const BYTE code csr_numlock_brk2[] = {0xE0,0xF0,0x12,0x00};

    if (event == BREAK_EVENT)
    {
        Buffer_String(csr_brk1);            // Buffer pre-string.
        Buffer_Key(scan_code);              // Buffer base code.
        Buffer_String(csr_numlock_brk2);    // Buffer tail-string.
    }
    else if (event == MAKE_EVENT)
    {
                                            // The Num Lock key is pressed.
        Buffer_String(csr_numlock_mk);      // Buffer pre-string.
        Buffer_Key(scan_code);              // Buffer base code.
    }
    else
    {
        Buffer_Key(0xE0);                   // Buffer E0h prefix.
        Buffer_Key(scan_code);              // Buffer base code.
    }

}
//-----------------------------------------------------------------------------

/* ----------------------------------------------------------------------------
 * FUNCTION: special_code
 *
 * For IBM Key Number 126. This key generates only make code.
 *
 * Input: code
 *        event = Contact event (MAKE_EVENT, BREAK_EVENT, or REPEAT_EVENT)
 * ------------------------------------------------------------------------- */
static void special_code(BYTE code_byte, BYTE event)
{
    static const BYTE code key126_normal_mk[] =
    {0xE1,0x14,0x77,0xE1,0xF0,0x14,0xF0,0x77,0x00};

    static const BYTE code key126_ctrl_mk[] =
    {0xE0,0x7E,0xE0,0xF0,0x7E,0x00};

    static const BYTE code keyBreak_ctrl_mk[] =
    {0x14,0xE0,0x7E,0xE0,0xF0,0x7E,0xF0,0x14,0x00};

    if (event == MAKE_EVENT)
    {
        if( temp_scanner_state.field.tCONTROL || (code_byte == 0x01) )
        {
            if( code_byte == 0x01 )
            {
                Buffer_String(keyBreak_ctrl_mk);
            }
            else
            {
                Buffer_String(key126_ctrl_mk);  // Buffer Ctrl case string.
            }
        }
        else
        {
            Buffer_String(key126_normal_mk);    // Buffer normal code string.
        }
    }
}
//-----------------------------------------------------------------------------

/* ----------------------------------------------------------------------------
 * FUNCTION: control_effect
 *
 * Generate scan code set 2 and update scanner status.
 *
 * Input: state flags
 *        event = Contact event (MAKE_EVENT, BREAK_EVENT, or REPEAT_EVENT)
 *        temp_scanner_state
 *
 *  The following bits are defined for state flags and temp_scanner_state:
 *       NUM_LOCK
 *       LEFT
 *       RIGHT
 *       ALT
 *       CONTROL
 *       OVERLAY
 *       FN
 *
 *
 * Returns: Updated temp_scanner_state
 * ------------------------------------------------------------------------- */
static void control_effect(BYTE state, BYTE event)
{
    if( (event == MAKE_EVENT)||(event == BREAK_EVENT) )
    {
        if (state & LShift)             // First generate scan code set 2.
        {
            simple_code(0x12, event);
        }
        else if (state & LAlt)
        {
            simple_code(0x11, event);
        }
        else if (state & LCtrl)
        {
            simple_code(0x14, event);
        }
        else if (state & RShift)
        {
            simple_code(0x59, event);
        }
    }

    if (event == MAKE_EVENT)            // Then update scanner state.
    {   // Key is pressed for the first time. This is a "make event".
        // Set scanner state for SHIFT, Alt, Ctrl, or Fn.
        temp_scanner_state.byte |= state;
    }
    else if (event == BREAK_EVENT)
    {   // Key has just been released. This is a "break event".
        temp_scanner_state.byte &= ~state;
    }
    else
    {
        ;   // Do nothing for repeat event.
    }
}
//-----------------------------------------------------------------------------

/* ----------------------------------------------------------------------------
 * FUNCTION: control_effect_e0
 *
 * Generate scan code set 2 and update scanner status.
 * Only for Alt-Right and Ctrl-Right.
 *
 * Input: state flags
 *        event = Contact event (MAKE_EVENT, BREAK_EVENT, or REPEAT_EVENT)
 *        temp_scanner_state
 *
 * The following bits are defined for state_flags and temp_scanner_state:
 *       NUM_LOCK
 *       LEFT
 *       RIGHT
 *       ALT
 *       CONTROL
 *       OVERLAY
 *       FN
 *
 * Return: Updated temp_scanner_state
 * ------------------------------------------------------------------------- */
static void control_effect_e0(BYTE state, BYTE event)
{   // This is only called for Alt-Right and Ctrl-Right.
    if ((event == MAKE_EVENT)||(event == BREAK_EVENT))
    {
        // First setup to generate scan code set 2.
        if (state & RAlt)
        {
            e0_prefix_code(0x11, event);
        }
        if (state & RCtrl)
        {
            e0_prefix_code(0x14, event);
        }
    }

    if (event == MAKE_EVENT)
    {       // Key is pressed for the first time. This is a "make event".
            // Set one flag in scanner state flags.
        temp_scanner_state.byte |= state;
    }
    else if (event == BREAK_EVENT)
    {       // Key has just been released. This is a "break event".
            // Clear one flag in scanner state flags.
        temp_scanner_state.byte &= ~state;
    }
    else
    {
        ;   // Do nothing for repeat event.
    }
}
//-----------------------------------------------------------------------------

/* ----------------------------------------------------------------------------
 * FUNCTION: sskey2_A2_table
 *
 * The A2_TABLE structure contains the following.
 *
 * BYTE comb;  Bit   Setting Combination Key
 *            ---   -----------------------
 *             7    Overlay
 *             6    Latched Fn
 *             5    Fn
 *             4    Numlock
 *             3    Ctrl
 *             2    Alt
 *             1    Shift
 *             0    reserved
 *
 * BYTE *pntr; Pointer to array for key.
 * ------------------------------------------------------------------------- */
#if !NUMPAD_USE_OEM
const BYTE code ss2_9C[] =  // Numpad 7 Home
{
    0x3D,   // Normal
    0xB0,   // Numlock
    0xB0,   // Fn
    0xB0    // Fn+Numlock
};

const BYTE code ss2_9D[] =  // Numpad 8 UP
{
    0x3E,   // Normal
    0xB1,   // Numlock
    0xB1,   // Fn
    0xB1    // Fn+Numlock
};

const BYTE code ss2_9E[] =  // Numpad 9 PGUp
{
    0x46,   // Normal
    0xB2,   // Numlock
    0xB2,   // Fn
    0xB2    // Fn+Numlock
};

const BYTE code ss2_A0[] =  // Numpad 4 Left
{
    0x3C,   // Normal
    0xB4,   // Numlock
    0xB4,   // Fn
    0xB4    // Fn+Numlock
};

const BYTE code ss2_A1[] =  // Numpad 5
{
    0x43,   // Normal
    0xB5,   // Numlock
    0xB5,   // Fn
    0xB5    // Fn+Numlock
};

const BYTE code ss2_A2[] =  // Numpad 6 Right
{
    0x44,   // Normal
    0xB6,   // Numlock
    0xB6,   // Fn
    0xB6    // Fn+Numlock
};

const BYTE code ss2_A4[] =  // Numpad 1 End
{
    0x3B,   // Normal
    0xB8,   // Numlock
    0xB8,   // Fn
    0xB8    // Fn+Numlock
};

const BYTE code ss2_A5[] =  // Numpad 2 Down
{
    0x42,   // Normal
    0xB9,   // Numlock
    0xB9,   // Fn
    0xB9    // Fn+Numlock
};

const BYTE code ss2_A6[] =  // Numpad 3 PGDn
{
    0x4B,   // Normal
    0xBA,   // Numlock
    0xBA,   // Fn
    0xBA    // Fn+Numlock
};

const BYTE code ss2_A8[] =  // Numpad 0 Ins
{
    0x3A,   // Normal
    0xBC,   // Numlock
    0xBC,   // Fn
    0xBC    // Fn+Numlock
};

const BYTE code ss2_A9[] =  // Numpad . Del
{
    0x49,   // Normal
    0xBD,   // Numlock
    0xBD,   // Fn
    0xBD    // Fn+Numlock
};
const BYTE code ss2_AB[] =  // Numpad Enter
{
    0x5A,   // Normal
    0x81,   // Numlock
    0x81,   // Fn
    0x81    // Fn+Numlock
};
#endif
//-----------------------------------------------------------------------------
#if !NUMPAD_MATH_USE_OEM
const BYTE code ss2_9F[] =  // Numpad *
{
    0x45,   // Normal
    0xB3,   // Numlock
    0xB3,   // Fn
    0xB3    // Fn+Numlock
};


const BYTE code ss2_A3[] =  // Numpad -
{
    0x4D,   // Normal
    0xB7,   // Numlock
    0xB7,   // Fn
    0xB7    // Fn+Numlock
};

const BYTE code ss2_A7[] =  // Numpad +
{
    0x4C,   // Normal
    0xBB,   // Numlock
    0xBB,   // Fn
    0xBB    // Fn+Numlock
};

const BYTE code ss2_AA[] =  // Numpad /
{
    0x4A,   // Normal
    0xBE,   // Numlock
    0xBE,   // Fn
    0xBE    // Fn+Numlock
};
#endif
//-----------------------------------------------------------------------------

/* ----------------------------------------------------------------------------
 * FUNCTION: sskey2_A2_table
 *
 * The A2_TABLE structure contains the following.
 *
 * BYTE comb;  Bit   Setting Combination Key
 *            ---   -----------------------
 *             7    Overlay
 *             6    Latched Fn
 *             5    Fn
 *             4    Numlock
 *             3    Ctrl
 *             2    Alt
 *             1    Shift
 *             0    reserved
 *
 * BYTE *pntr; Pointer to array for key.
 * ------------------------------------------------------------------------- */
const A2_TABLE sskey2_A2_table[] =
{                                           // Keyboard matrix index
    // Keypad -->
    {0x30, ss2_9C },    /* index = 00h */   // 0x9C 7
    {0x30, ss2_9D },    /* index = 01h */   // 0x9D 8
    {0x30, ss2_9E },    /* index = 02h */   // 0x9E 9
    {0x30, ss2_9F },    /* index = 03h */   // 0x9F 0
    {0x30, ss2_A0 },    /* index = 04h */   // 0xA0 U
    {0x30, ss2_A1 },    /* index = 05h */   // 0xA1 I
    {0x30, ss2_A2 },    /* index = 06h */   // 0xA2 O
    {0x30, ss2_A3 },    /* index = 07h */   // 0xA3 P
    {0x30, ss2_A4 },    /* index = 08h */   // 0xA4 J
    {0x30, ss2_A5 },    /* index = 09h */   // 0xA5 K
    {0x30, ss2_A6 },    /* index = 0Ah */   // 0xA6 L
    {0x30, ss2_A7 },    /* index = 0Bh */   // 0xA7 ;:
    {0x30, ss2_A8 },    /* index = 0Ch */   // 0xA8 M
    {0x30, ss2_A9 },    /* index = 0Dh */   // 0xA9 .>
    {0x30, ss2_AA },    /* index = 0Eh */   // 0xAA /?
    {0x20, ss2_AB },    /* index = 0Fh */   // 0xAB Enter
    {0x20, ss2_AC },    /* index = 10h */   // 0xAC
    {0x20, ss2_AD },    /* index = 11h */   // 0xAD F1
    {0x20, ss2_AE },    /* index = 12h */   // 0xAE F2
    {0x20, ss2_AF },    /* index = 13h */   // 0xAF F3
    {0x20, ss2_B0 },    /* index = 14h */   // 0xB0 F4
    {0x20, ss2_B1 },    /* index = 15h */   // 0xB1 F5
    {0x20, ss2_B2 },    /* index = 16h */   // 0xB2 F6
    {0x20, ss2_B3 },    /* index = 17h */   // 0xB3 F7
    {0x20, ss2_B4 },    /* index = 18h */   // 0xB4 F8
    {0x20, ss2_B5 },    /* index = 19h */   // 0xB5 F9
    {0x20, ss2_B6 },    /* index = 1Ah */   // 0xB6 F10
    {0x20, ss2_B7 },    /* index = 1Bh */   // 0xB7 F11
    {0x20, ss2_B8 },    /* index = 1Ch */   // 0xB8 F12
    {0x20, ss2_B9 },    /* index = 1Dh */   // 0xB9
    {0x20, ss2_BA },    /* index = 1Eh */   // 0xBA
    {0x20, ss2_BB },    /* index = 1Fh */   // 0xBB
    {0x20, ss2_BC },    /* index = 20h */   // 0xBC
    {0x20, ss2_BD },    /* index = 21h */   // 0xBD
    {0x20, ss2_BE },    /* index = 22h */   // 0xBE
    {0x20, ss2_BF },    /* index = 23h */   // 0xBF
    #if ENABLE_SSKEY2_A2_CX
    {0x20, ss2_C0 },    /* index = 24h */   // 0xC0
    {0x20, ss2_C1 },    /* index = 25h */   // 0xC1
    {0x20, ss2_C2 },    /* index = 26h */   // 0xC2
    {0x20, ss2_C3 },    /* index = 27h */   // 0xC3
    {0x20, ss2_C4 },    /* index = 28h */   // 0xC4
    {0x20, ss2_C5 },    /* index = 29h */   // 0xC5
    {0x20, ss2_C6 },    /* index = 2Ah */   // 0xC6
    {0x20, ss2_C7 },    /* index = 2Bh */   // 0xC7
    {0x20, ss2_C8 },    /* index = 2Ch */   // 0xC8
    {0x20, ss2_C9 },    /* index = 2Dh */   // 0xC9
    {0x20, ss2_CA },    /* index = 2Eh */   // 0xCA
    {0x20, ss2_CB },    /* index = 2Fh */   // 0xCB
    {0x20, ss2_CC },    /* index = 30h */   // 0xCC
    {0x20, ss2_CD },    /* index = 31h */   // 0xCD
    {0x20, ss2_CE },    /* index = 32h */   // 0xCE
    {0x20, ss2_CF },    /* index = 33h */   // 0xCF
    #endif
    #if ENABLE_SSKEY2_A2_DX
    {0x20, ss2_D0 },    /* index = 34h */   // 0xD0
    {0x20, ss2_D1 },    /* index = 35h */   // 0xD1
    {0x20, ss2_D2 },    /* index = 36h */   // 0xD2
    {0x20, ss2_D3 },    /* index = 37h */   // 0xD3
    {0x20, ss2_D4 },    /* index = 38h */   // 0xD4
    {0x20, ss2_D5 },    /* index = 39h */   // 0xD5
    {0x20, ss2_D6 },    /* index = 3Ah */   // 0xD6
    {0x20, ss2_D7 },    /* index = 3Bh */   // 0xD7
    {0x20, ss2_D8 },    /* index = 3Ch */   // 0xD8
    {0x20, ss2_D9 },    /* index = 3Dh */   // 0xD9
    {0x20, ss2_DA },    /* index = 3Eh */   // 0xDA
    {0x20, ss2_DB },    /* index = 3Fh */   // 0xDB
    {0x20, ss2_DC },    /* index = 40h */   // 0xDC
    {0x20, ss2_DD },    /* index = 41h */   // 0xDD
    {0x20, ss2_DE },    /* index = 42h */   // 0xDE
    {0x20, ss2_DF },    /* index = 43h */   // 0xDF
    #endif
};

/* ----------------------------------------------------------------------------
 * Insyde Software Key Number 2
 *
 * 00h         Null
 * 01h - 7Fh   Same as IBM Scan Code, Set2
 * 80h - BFh   Pre-Index for generation Scan Code, Set2
 * C0h - DFh   Pre-Index for PPK function
 * C0h - DFh   Pre-Index for SMI function
 * ------------------------------------------------------------------------- */
/* ----------------------------------------------------------------------------
 * FUNCTION: sskey3_80_table - Insyde Software Key Number 2
 * 80h - BFh   Pre-Index for generation Scan Code, Set2
 * ------------------------------------------------------------------------- */
const BYTE code sskey3_80_table[] =
{
//-----------------------------------------------------------------------------
// Index from keyboard matrix
//-----------------------------------------------------------------------------
                                    // Keyboard matrix index
    0x83,   DO_SIMPLE_CODE,         // [80h] F7 (118)
    0x5A,   DO_E0_PREFIX_CODE,      // [81h] Numpad Enter (108)
    0x1F,   DO_E0_PREFIX_CODE,      // [82h] Windows Left
    0x27,   DO_E0_PREFIX_CODE,      // [83h] Windows Right
    0x2F,   DO_E0_PREFIX_CODE,      // [84h] Application
    0x37,   DO_E0_PREFIX_CODE,      // [85h] Power event
    0x3F,   DO_E0_PREFIX_CODE,      // [86h] Sleep event
    0x5E,   DO_E0_PREFIX_CODE,      // [87h] Wake event
    LShift, DO_CONTROL_EFFECT,      // [88h] 12 Shift(L)
    RShift, DO_CONTROL_EFFECT,      // [89h] 59 Shift(R)
    LAlt,   DO_CONTROL_EFFECT,      // [8Ah] 11 Alt(L)
    RAlt,   DO_CONTROL_EFFECT_E0,   // [8Bh] E0 11 Alt(R)
    LCtrl,  DO_CONTROL_EFFECT,      // [8Ch] 14 Crtl(L)
    RCtrl,  DO_CONTROL_EFFECT_E0,   // [8Dh] E0 14 Ctrl(R)
    FN,     DO_CONTROL_EFFECT,      // [8Eh] Fn
    OVLAY,  DO_CONTROL_EFFECT,      // [8Fh] Overlay
    0x00,   DO_SIMPLE_CODE,         // [90h] Reserved
    0x00,   DO_SPECIAL_CODE,        // [91h] Pause (126)
    0x01,   DO_SPECIAL_CODE,        // [92h] Break
    0x84,   DO_SIMPLE_CODE,         // [93h] (124) Alt-Case (SysRq)
    0x6C,   DO_CURSOR_SHIFT,        // [94h] Home
    0x69,   DO_CURSOR_SHIFT,        // [95h] End
    0x7D,   DO_CURSOR_SHIFT,        // [96h] Page up
    0x7A,   DO_CURSOR_SHIFT,        // [97h] Page down
    0x75,   DO_CURSOR_SHIFT,        // [98h] UP arrow
    0x72,   DO_CURSOR_SHIFT,        // [99h] Down arrow
    0x6B,   DO_CURSOR_SHIFT,        // [9Ah] Left arrow
    0x74,   DO_CURSOR_SHIFT,        // [9Bh] Right arrow

//-----------------------------------------------------------------------------
// For MicroSoft enhance keyboard feature.
//-----------------------------------------------------------------------------
    0x4D, DO_E0_PREFIX_CODE,        // [9Ch] Next Track event
    0x15, DO_E0_PREFIX_CODE,        // [9Dh] Prev Track event
    0x3B, DO_E0_PREFIX_CODE,        // [9Eh] Stop event
    0x34, DO_E0_PREFIX_CODE,        // [9Fh] Play/Pause event
    0x23, DO_E0_PREFIX_CODE,        // [A0h] Mute event
    0x32, DO_E0_PREFIX_CODE,        // [A1h] Volume Up event
    0x21, DO_E0_PREFIX_CODE,        // [A2h] Volume Down event
    0x48, DO_E0_PREFIX_CODE,        // [A3h] Mail event
    0x10, DO_E0_PREFIX_CODE,        // [A4h] Search event
    0x3A, DO_E0_PREFIX_CODE,        // [A5h] Web/Home event
    0x38, DO_E0_PREFIX_CODE,        // [A6h] Back event
    0x30, DO_E0_PREFIX_CODE,        // [A7h] Forward event
    0x28, DO_E0_PREFIX_CODE,        // [A8h] Stop event
    0x20, DO_E0_PREFIX_CODE,        // [A9h] Refresh event
    0x18, DO_E0_PREFIX_CODE,        // [AAh] Favorites event
    0x2B, DO_E0_PREFIX_CODE,        // [ABh] Caluator event
    0x40, DO_E0_PREFIX_CODE,        // [ACh] My Computer event
    0x50, DO_E0_PREFIX_CODE,        // [ADh] Media event
    0x00, DO_SIMPLE_CODE,           // [AEh] Reserved
    0x00, DO_SIMPLE_CODE,           // [AFh] Reserved

//-----------------------------------------------------------------------------
// Index from sskey2_A2_table
//-----------------------------------------------------------------------------
    0x6C, DO_SIMPLE_CODE,           // Numpad 7 Home    [B0h]
    0x75, DO_SIMPLE_CODE,           // Numpad 8 UP      [B1h]
    0x7D, DO_SIMPLE_CODE,           // Numpad 9 PGUp    [B2h]
    0x7C, DO_SIMPLE_CODE,           // Numpad *         [B3h]
    0x6B, DO_SIMPLE_CODE,           // Numpad 4 Left    [B4h]
    0x73, DO_SIMPLE_CODE,           // Numpad 5         [B5h]
    0x74, DO_SIMPLE_CODE,           // Numpad 6 Right   [B6h]
    0x7B, DO_SIMPLE_CODE,           // Numpad -         [B7h]
    0x69, DO_SIMPLE_CODE,           // Numpad 1 End     [B8h]
    0x72, DO_SIMPLE_CODE,           // Numpad 2 Down    [B9h]
    0x7A, DO_SIMPLE_CODE,           // Numpad 3 PGDn    [BAh]
    0x79, DO_SIMPLE_CODE,           // Numpad +         [BBh]
    0x70, DO_SIMPLE_CODE,           // Numpad 0 Ins     [BCh]
    0x71, DO_SIMPLE_CODE,           // Numpad . Del     [BDh]
    0x4A, DO_CURSOR_SHIFT,          // Numpad /         [BEh]
    0x00, DO_SIMPLE_CODE,           // Reserved         [BFh]

    0x71, DO_CURSOR_SHIFT,          // Delete           [C0h]
    0x84, DO_SIMPLE_CODE,           // SysRq            [C1h]
    0x70, DO_CURSOR_SHIFT,          // Insert           [C2h]
    0x7C, DO_PrtSC_PREFIX_CODE,     // Print Screen     [C3h]
    0x00, DO_SIMPLE_CODE,           // Reserved         [C4h]
    0x00, DO_SIMPLE_CODE,           // Reserved         [C5h]
    0x00, DO_SIMPLE_CODE,           // Reserved         [C6h]
    0x00, DO_SIMPLE_CODE,           // Reserved         [C7h]
    0x00, DO_SIMPLE_CODE,           // Reserved         [C8h]
    0x00, DO_SIMPLE_CODE,           // Reserved         [C9h]
    0x00, DO_SIMPLE_CODE,           // Reserved         [CAh]
    0x00, DO_SIMPLE_CODE,           // Reserved         [CBh]
    0x00, DO_SIMPLE_CODE,           // Reserved         [CCh]
    0x00, DO_SIMPLE_CODE,           // Reserved         [CDh]
    0x00, DO_SIMPLE_CODE,           // Reserved         [CEh]
    0x00, DO_SIMPLE_CODE,           // Reserved         [CFh]

    // Function Key Fn + Key
    0x00,   DO_COSTOMER_FUNCTION,     // Function key F1    [D0h]
    0x01,   DO_COSTOMER_FUNCTION,     // Function key F2    [D1h]
    0x02,   DO_COSTOMER_FUNCTION,     // Function key F3    [D2h]
    0x03,   DO_COSTOMER_FUNCTION,     // Function key F4    [D3h]
    0x04,   DO_COSTOMER_FUNCTION,     // Function key F5    [D4h]
    0x05,   DO_COSTOMER_FUNCTION,     // Function key F6    [D5h]
    0x06,   DO_COSTOMER_FUNCTION,     // Function key F7    [D6h]
    0x07,   DO_COSTOMER_FUNCTION,     // Function key F8    [D7h]
    0x08,   DO_COSTOMER_FUNCTION,     // Function key F9    [D8h]
    0x09,   DO_COSTOMER_FUNCTION,     // Function key F10   [D9h]
    0x0A,   DO_COSTOMER_FUNCTION,     // Function key F11   [DAh]
    0x0B,   DO_COSTOMER_FUNCTION,     // Function key F12   [DBh]
    0x0C,   DO_COSTOMER_FUNCTION,     // Function key       [DCh]
    0x0D,   DO_COSTOMER_FUNCTION,     // Function key       [DDh]
    0x0E,   DO_COSTOMER_FUNCTION,     // Function Key       [DEh]
    0x0F,   DO_COSTOMER_FUNCTION,     // Reserved key       [DFh]
    0x10,   DO_COSTOMER_FUNCTION,     // Reserved key       [E0h]
    0x11,   DO_COSTOMER_FUNCTION,     // Reserved key       [E1h]
    0x12,   DO_COSTOMER_FUNCTION,     // Reserved key       [E2h]
    0x13,   DO_COSTOMER_FUNCTION,     // Reserved key       [E3h]
    0x14,   DO_COSTOMER_FUNCTION,     // Reserved key       [E4h]
    0x15,   DO_COSTOMER_FUNCTION,     // Reserved key       [E5h]
    0x16,   DO_COSTOMER_FUNCTION,     // Reserved key       [E6h]
    0x17,   DO_COSTOMER_FUNCTION,     // Reserved key       [E7h]
    // Special "Break" Function Key Fn + Key
    0x18,   DO_COSTOMER_FUNCTION,     // Special Break [D0h][E8h]
    0x19,   DO_COSTOMER_FUNCTION,     // Special Break [D1h][E9h]
    0x1A,   DO_COSTOMER_FUNCTION,     // Special Break [D2h][EAh]
    0x1B,   DO_COSTOMER_FUNCTION,     // Special Break [D3h][EBh]
    0x1C,   DO_COSTOMER_FUNCTION,     // Special Break [D4h][ECh]
    0x1D,   DO_COSTOMER_FUNCTION,     // Special Break [D5h][EDh]
    0x1E,   DO_COSTOMER_FUNCTION,     // Special Break [D6h][EEh]
    0x1F,   DO_COSTOMER_FUNCTION,     // Special Break [D7h][EFh]
    0x20,   DO_COSTOMER_FUNCTION,     // Special Break [D8h][F0h]
    0x21,   DO_COSTOMER_FUNCTION,     // Special Break [D9h][F1h]
    0x22,   DO_COSTOMER_FUNCTION,     // Special Break [DAh][F2h]
    0x23,   DO_COSTOMER_FUNCTION,     // Special Break [DBh][F3h]
    0x24,   DO_COSTOMER_FUNCTION,     // Special Break [DCh][F4h]
    0x25,   DO_COSTOMER_FUNCTION,     // Special Break [DDh][F5h]
    0x26,   DO_COSTOMER_FUNCTION,     // Special Break [DEh][F6h]
    0x27,   DO_COSTOMER_FUNCTION,     // Special Break [DFh][F7h]
    0x28,   DO_COSTOMER_FUNCTION,     // Special Break [E0h][F8h]
    0x29,   DO_COSTOMER_FUNCTION,     // Special Break [E1h][F9h]
    0x2A,   DO_COSTOMER_FUNCTION,     // Special Break [E2h][FAh]
    0x2B,   DO_COSTOMER_FUNCTION,     // Special Break [E3h][FBh]
    0x2C,   DO_COSTOMER_FUNCTION,     // Special Break [E4h][FCh]
    0x2D,   DO_COSTOMER_FUNCTION,     // Special Break [E5h][FDh]
    0x2E,   DO_COSTOMER_FUNCTION,     // Special Break [E6h][FEh]
    0x2F,   DO_COSTOMER_FUNCTION,     // Special Break [E7h][FFh]

};
//-----------------------------------------------------------------------------

/*-----------------------------------------------------------------------------
 * End
 *---------------------------------------------------------------------------*/

