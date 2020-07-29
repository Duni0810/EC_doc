/*-----------------------------------------------------------------------------
 * TITLE: CORE_SCN.C - code to handle local keyboard scanning.
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
#define _KBSCAN_DELAY               1

/*-----------------------------------------------------------------------------
 * Local xdata Definition
 *---------------------------------------------------------------------------*/
const BYTE xdata *Ext_Hotkey_Pntr;

/* ----------------------------------------------------------------------------
 * FUNCTION:   scan_keys
 *
 * Scan key matrix once.
 *
 * Return: value != 0, scan activity detected, scan again later.
 *         value  = 0, no scan activity detected.
 * ------------------------------------------------------------------------- */
BYTE scan_keys(void)
{
    BYTE    SCAN_Flag = FALSE;
    BYTE    SCAN_Cnt;
    BYTE    SCAN_Val;

    scan.saf_make  = 0;
    scan.saf_break = 0;
    scan.saf_keys  = 0;

    if (typematic.byte)
    {
        check_tm(typematic);    // Check typematic.
    }

    if (new_keyh.byte)          // Exist Debounce key?
    {   // Yes, bounding.
        scan.saf_make = 1;      // Set debounce make bit.
        debounce_key();
        SCAN_Flag = TRUE;
    }

    if( !SCAN_Flag )    // Otherwise, scan all.
    {
        for (SCAN_Cnt=0;SCAN_Cnt<16;SCAN_Cnt++)
        {
            Write_Strobe(SCAN_Cnt);
            #if ( _KBSCAN_DELAY > 0 )
            Microsecond_Delay(_KBSCAN_DELAY);
            #endif
            SCAN_Val = KSI;     // Read in KI 0 - 7 sense line data. */
            SCAN_Val = (~SCAN_Val) ^ bscan_matrix[SCAN_Cnt];

            if (SCAN_Val != 0)
            {
                check_scan(SCAN_Val, SCAN_Cnt);
            }

            if (bscan_matrix[SCAN_Cnt])
            {   // Here, if current still active.
                scan.saf_keys = 1;      // Set keys active bits. Check all key release.
            }

            KSOL  = 0xFF;
            KSOH1 = 0xFF;
            KSOH2 = 0xFF;

            #if (ExternMatrixGPO)
            GPOKeyPin1_HI();
            #endif
        }

        // If ghost exist, make key clear.
        if (new_keyl.field.ghost == 1)
        {
            new_keyl.field.ghost = 0;   // Ghost flag is set. Clear it.
            SCAN_Val = new_keyl.byte & 7;
            if (SCAN_Val == 0)
            {
                SCAN_Val = Byte_Mask((BYTE) (new_keyh.field.input));
                SCAN_Val = SCAN_Val & diode_key;
            }

            if (SCAN_Val == 0)
            {
                new_keyh.byte = 0;  // Clear new key.
                new_keyl.byte = 0;
            }
        }
    }

    return((BYTE) (scan.saf_make + scan.saf_break + scan.saf_keys));
}

/* ----------------------------------------------------------------------------
 * FUNCTION: Write_Strobe
 *
 * Writes the scan matrix KSO[15:0] lines based on Scan Line bit number.
 *
 * KSO[15:0] will be all floats or open except for the bit corresponding to
 * the Scan Line number.  This bit will be low to be detected by the sense
 * line input port later.
 *
 * Config.Msk_Strobe_H and Config.Msk_Strobe_L specify which of the 16 bits on
 * the port are used for the scanner.  (1 = Scanner, 0 = Other OEM function).
 * This allows port pins which are not used for the scanner output lines to be
 * used for something else.
 *
 * Input: Scan line number.
 * ------------------------------------------------------------------------- */
void Write_Strobe(BYTE scan_line_num)
{
    if (scan_line_num < 8)
    {
        KSOL  =~ (0x01 << scan_line_num);
        KSOH1 = 0xFF;
        KSOH2 = 0xFF;

        #if (ExternMatrixGPO)
        GPOKeyPin1_HI();
        #endif
    }
    else
    {
        KSOL  = 0xFF;
        KSOH1 =~ (0x01<<(scan_line_num-0x08));
        KSOH2 = 0xFF;

        #if (ExternMatrixGPO)
        GPOKeyPin1_HI();
        #endif
    }
}

/* ----------------------------------------------------------------------------
 * FUNCTION: check_scan
 *
 * Input:
 *     changes = Any bit set indicates there is a difference in state from
 *         last KSI to this KSI.
 *     scan_address = Address of strobe line that was low when changes was
 *         computed.
 *
 * NOTE:  For the shift and bit test of "changes" to work, "changes" MUST be
 * unsigned.  (Right shifting an unsigned quantity fills vacated bits with
 * zeros.  Right shifting a signed quantity will fill vacated bits with sign
 * bits on some machines and zero bits on others.)
 * ------------------------------------------------------------------------- */
static void check_scan(BYTE changes, BYTE scan_address)
{
    BYTE flag;
    BYTE change_make_key;
    BYTE bit_num;

    flag = FALSE;                   // Simple Matrix.
    if (new_keyl.field.ghost == 1)
    {
        flag = TRUE;
    }
    else
    {   /* Find 0 -> 1 's. */
        change_make_key = changes & ~bscan_matrix[scan_address];
        if (change_make_key == 0)
        {
            flag = FALSE;
        }
        else if (find_paths(change_make_key, scan_address))
        {
            flag = TRUE;    /* TRUE if ghost keys. */
        }
    }

    if (flag)
    {   /* Ghost exists. */
        new_keyl.field.ghost = 1;  /* Set ghost flag. */

        /* Only look at 1 -> 0 transition & diode key always no ghost. */
        changes &= bscan_matrix[scan_address] | diode_key;
    }

    bit_num = 0;
    while (changes != 0)
    {
        if (changes & 0x01)     /* Look at changes 1 bit at a time. */
        {
            cscfnd(bit_num, scan_address);
        }
        bit_num++;
        changes = changes >> 1; /* Shift bit out (and a zero bit in) to check next bit. */
    }
}

/* ----------------------------------------------------------------------------
 * FUNCTION:   cscfnd
 *
 * Find changed bit.  This subroutine is called for each bit in this KSI that
 * is different from last KSI.
 * ------------------------------------------------------------------------- */
static void cscfnd(BYTE bit_num, BYTE scan_address)
{
    if (bscan_matrix[scan_address] & Byte_Mask(bit_num))
    {
        if (scan.saf_break == 0)
        {
            scan.saf_break = 1;
            setup_debounce(bit_num, scan_address, BREAK_EVENT);
        }
    }
    else
    {
        if (scan.saf_make == 0)
        {
            scan.saf_make = 1;
            setup_debounce(bit_num, scan_address, MAKE_EVENT);
        }
    }
}

/* ----------------------------------------------------------------------------
 * FUNCTION:   setup_debounce
 *
 * Input:
 *     scan_address = number of bit of KO.
 *     bit_num = number of bit that changed from last KI to this KI for KO.
 *     event = contact event (MAKE_EVENT or BREAK_EVENT).
 * ------------------------------------------------------------------------- */
static void setup_debounce(BYTE bit_num, BYTE scan_address, BYTE event)
{
    new_keyh.field.output = scan_address;
    new_keyh.field.input  = bit_num;
    new_keyh.field.debounce_status = 1;
    new_keyl.field.state = 0;  /* Debounce has not counted out. */
    new_keyl.field.same  = 0;   /* Key activity indication. */

    if (event == MAKE_EVENT)
    {   /* For MAKE key (key pressed). */
        new_keyl.field.trans = 0;
        new_keyl.field.count = Ext_Cb2.field.Break_Count; /* Shouldn't this be Make_Count? */
    }
    else
    {   /* For BREAK key (key released). */
        new_keyl.field.trans = 1;
        new_keyl.field.count = Ext_Cb2.field.Make_Count; /* Shouldn't this be Break_Count? */
    }
}

/* ----------------------------------------------------------------------------
 * FUNCTION: find_paths
 *
 * Input:  DOWN_STATE  scan_address
 *         change_make_key
 *
 * Return: FALSE if no paths found, Otherwise returns TRUE.
 * ------------------------------------------------------------------------- */
static FLAG find_paths(BYTE change_make_key, BYTE scan_address)
{
    FLAG paths_found, done;
    BYTE paths, temp, bits;
    BYTE first_address;

    /* For the shift and bit test to work, "bits" MUST be unsigned.  (Right
       shifting an unsigned quantity fills vacated bits with zeros.  Right
       shifting a signed quantity will fill vacated bits with sign bits on some
       machines and zero bits on others.) */
    done = FALSE;
    paths_found = FALSE;
    first_address = scan_address;

    change_make_key &= ~(diode_key); /* Ignore diode key. */
    /* change_make_key = bKO_BITS. */

    if (change_make_key == 0)
    {
        done = TRUE; /* paths_found == FALSE */
    }

    if (!done)
    {
        paths = bscan_matrix[scan_address] | change_make_key;
        paths &= ~(diode_key);  /* Ignore diode key. */
        /* paths = bKO_PATHS. */
        if (paths == 0)
        {
            done = TRUE;  /* paths_found == FALSE */
        }
    }

    while (!done)
    {
        scan_address++;

        //if(scan_address >= STANDARD_SCAN_LINES)
        if (scan_address >= MAX_SCAN_LINES)
        {
            scan_address = 0;   /* Wrap around */
        }

        if (scan_address == first_address)
        {
            done = TRUE;        /* No scan lines left.  paths_found == FALSE */
        }

        if (!done)
        {   /* Check Path */
            temp  = bscan_matrix[scan_address]; /* Any paths? */
            temp &= ~(diode_key);               /* Ignore diode key */

            if (temp != 0)
            {    /* Paths found */
                temp &= paths;  /* Do paths line up? */
            }

            if (temp != 0)
            {   /* Paths line up */
                if (change_make_key != temp)  /* Only 1 bit set? */
                {   /* No, ghost exists. */
                    paths_found = TRUE;
                    done = TRUE;
                }

                if (!done)
                {   /* The following checks if more than one bit is set to 1.
                    Because "bits" is unsigned, each shift moves the bit
                    under test out and a 0 bit in.  When the first 1 bit is
                    found, if "bits" == 0, this indicates that there is only
                    one bit set. */
                    bits = paths;  /* Only 1 bit set? */
                    temp = FALSE;

                    do
                    {
                        if (bits & 0x01)
                        {
                            temp = TRUE;
                        }

                        /* Shift bit out (and a zero bit in) to check next bit. */
                        bits = bits >> 1;
                    } while (temp == FALSE);

                    if (bits != 0)
                    {   /* No, more than 1 bit set. */
                        paths_found = TRUE;
                        done = TRUE;
                    }
                } /* if (!done) */
            } /* if (temp != 0) */
        } /* if (!done) */
    } /* while (!done) */

    return (paths_found);
}

/* ----------------------------------------------------------------------------
 * FUNCTION: debounce_key
 * ------------------------------------------------------------------------- */
static void debounce_key(void)
{
    BYTE    SCAN_Val;

    scan.scan_line = new_keyh.field.output;
    Write_Strobe(scan.scan_line);

    SCAN_Val = Byte_Mask((BYTE) (new_keyh.field.input));
    SCAN_Val &= ~Read_Scan_Lines();

    if (SCAN_Val != 0)
    {
        if (new_keyl.field.trans == 0)
        {
            new_keyl.field.same = 1;    // last key detected as a MAKE,  same = 1.
        }
        else
        {
            new_keyl.field.same = 0;    // last key detected as a BREAK, same = 0.
        }
    }
    else
    {
        if (new_keyl.field.trans == 0)
        {
            new_keyl.field.same = 0;    // last key detected as a MAKE,  same = 0.
        }
        else
        {
            new_keyl.field.same = 1;    // last key detected as a BREAK, same = 1.
        }
    }

    if (new_keyl.field.state == 0)
    {
        if (new_keyl.field.count != 0)
        {
            new_keyl.field.count--;
        }
        else
        {
            if (new_keyl.field.same == 0)
            {
                new_keyh.byte = 0;  // Debounce failed, so claer all data
                new_keyl.byte = 0;
            }
            else
            {
                new_keyl.field.state = 1;
            }
        }
    }
    else
    {
        if (new_keyl.field.same == 0)
        {
            new_keyh.byte = 0;  // Debounce failed.
            new_keyl.byte = 0;
        }
        else
        {
            change_valid();     // Debounce OK
        }
    }
}

/* ----------------------------------------------------------------
 * FUNCTION: Read_Scan_Lines - Read KSI[7:0] of the scan matrix
 *
 * Return: Data from sense lines that are enabled.
 *         The lines that are disabled return 1s.
 *
 * C prototype: BYTE Read_Scan_Lines();
 * ---------------------------------------------------------------- */
BYTE Read_Scan_Lines(void)
{
    return (KSI);
}

/* ---------------------------------------------------------------
 * FUNCTION: change_valid
 * --------------------------------------------------------------- */
void change_valid(void)
{
    BYTE temp, mask, event;

    mask = Byte_Mask((BYTE) (new_keyh.field.input));
    temp = bscan_matrix[new_keyh.field.output];
    temp = temp ^ mask;
    bscan_matrix[new_keyh.field.output] = temp;

    event = BREAK_EVENT;            // Ready for BREAK indication.
    if ((mask & temp) != 0)
    {
        event = MAKE_EVENT;
        typematic.byte = new_keyh.byte; // Set New Typematic Key.
        scan.TMscale = TM_SCALE;
        bTMcount = bTMdelay;

        ex_typematic.byte = 0x00;
    }

    Send_Scan2(new_keyh, event);
    new_keyh.byte = 0;
    new_keyl.byte = 0;
}

/* ----------------------------------------------------------------------------
 * FUNCTION: check_tm - Handle typematic function.
 *
 * Input: Typematic key
 * ------------------------------------------------------------------------- */
static void check_tm(union KEY key)
{
    BYTE    SCAN_flag = FALSE;
    BYTE    SACN_Val;
    SACN_Val = Byte_Mask((BYTE) (key.field.input));
    SACN_Val &= bscan_matrix[key.field.output];

    if (SACN_Val == 0)      // Release Typematic key?
    {
        typematic.byte = 0; // Clear Typematic.
        SCAN_flag = TRUE;
    }

    if (!SCAN_flag)
    {
        scan.TMscale--;         // Count down Prescale.
        if (scan.TMscale != 0)
        {
            SCAN_flag = TRUE;
        }
    }

    if (!SCAN_flag)
    {
        scan.TMscale = TM_SCALE;    // Reload prescale counter.
        bTMcount--;         // Count down TMcount.
        if (bTMcount != 0)
        {
            SCAN_flag = TRUE;
        }
    }

    if (!SCAN_flag)
    {
        bTMcount = bTMrepeat;   // Reload TMcount.
        Send_Scan2(key, REPEAT_EVENT);
    }
}

/* ----------------------------------------------------------------
 * FUNCTION: Scan_Init - Initialize internal keyboard (scanner)
 * ---------------------------------------------------------------- */
void Scan_Init(void)        // Lower all KSO lines for scan matrix
{
    KSOL  = 0x00;
    KSOH1 = 0x00;

    #if ExtendMatrix    // The function of extend keys
    KSOH2 = 0x00;
    #endif
    #if (ExternMatrixGPO)
    GPOKeyPin1Ctl = 0x40;   // output and pull high
    GPOKeyPin1_LO();        // default low
    #endif
}

/* ----------------------------------------------------------------
 * FUNCTION: Enable_Any_Key_Irq
 *
 * Setup for any key from scanner to generate an interrupt.
 *
 * Lower the "strobe" lines so that when any key is pressed at least one input
 * line will go low.  Any bits (out of the possible 16) that are not used for
 * the internal keyboard (scanner) are left alone.  This is done using
 * Config.Msk_Strobe_H and Config.Msk_Strobe_L.
 *
 * Then, if the internal keyboard (scanner) is enabled, allow the interrupt to
 * occur when a key is pressed on the scanner.
 * -------------------------------------------------------------- */
void Enable_Any_Key_Irq(void)   // Lower all KSO lines for scan matrix
{
    KSOL  = 0x00;
    KSOH1 = 0x00;

    if (ExtendMatrix)
    {
        KSOH2=0x00;

        #if (ExternMatrixGPO)
        GPOKeyPin1_LO();
        #endif
    }

    IER1 |= 0x08;             // enable KSI interrupt
}

/* ----------------------------------------------------------------------------
 * FUNCTION:   Start_Scan_Transmission
 *
 * Lock out other devices so scanner can transmit data to Host, and start
 * Timer B with event "SEND_ENABLE".  When the "RESPONSE_TIME" has expired,
 * the Timer B interrupt handler will post a "SEND" service request to be
 * dispatched by the main service loop.  At that time the data will be sent to
 * the Host via the "SEND" service handler.
 * ------------------------------------------------------------------------- */
void Start_Scan_Transmission(void)
{
    EnableSendToHostTimer();
}

/* ----------------------------------------------------------------------------
 * FUNCTION:   Check_Scan_Transmission
 *
 * See if the scanner keyboard data transmission (to Host) should be allowed.
 *
 * Return: Transmission status indication.  TRUE if ok to transmit.
 * ------------------------------------------------------------------------- */
FLAG Check_Scan_Transmission(void)
{
    FLAG allow_transmission = FALSE;

    if ((Int_Var.Scan_Lock == 0) && (Flag.SCAN_INH == 0))
    {       /* Scanner transmission is locked and inhibited. */
        if (scan.kbf_head != scan.kbf_tail)
        {
            /* Data available in local keyboard buffer */
            allow_transmission = TRUE;  /* Allow transmission */
        }
    }

    return (allow_transmission);
}

/* ----------------------------------------------------------------
 * FUNCTION: Setup_Scanner_Pntr - to define the possible scanner tables
 *
 *     0   Rc_RAM_Table
 *     1   ROM Table 1
 *     ...
 * -------------------------------------------------------------- */
void Setup_Scanner_Pntr(void)
{
    switch( KeyboardMatrixID )
    {
        #if SUPPORT_KB_MATRIX_ID1
        case 0x01:
                Scanner_Table_Pntr = &Rc_ROM_Table1;
                #if ExtendMatrix
                Extendkey_Table_Pntr = &EtKey_ROM_Table1;
                #endif
                break;
        #endif
        #if SUPPORT_KB_MATRIX_ID2
        case 0x02:
                Scanner_Table_Pntr = &Rc_ROM_Table2;
                #if ExtendMatrix
                Extendkey_Table_Pntr = &EtKey_ROM_Table2;
                #endif
                break;
        #endif
        #if SUPPORT_KB_MATRIX_ID3
        case 0x03:
                Scanner_Table_Pntr = &Rc_ROM_Table3;
                #if ExtendMatrix
                Extendkey_Table_Pntr = &EtKey_ROM_Table3;
                #endif
                break;
        #endif
        default:
                Scanner_Table_Pntr = &Rc_ROM_Tables;
                #if ExtendMatrix
                Extendkey_Table_Pntr = &EtKey_ROM_Tables;
                #endif
                break;
    }
}

/* ----------------------------------------------------------------
 * FUNCTION: Get_Kbd_Type
 *
 * Returns the type of the internal keyboard.
 *
 * Returns: 0 = U.S. keyboard, 1 = Japanese keyboard.
 * -------------------------------------------------------------- */
FLAG Get_Kbd_Type(void)
{
    return(0);
}

/* ----------------------------------------------------------------
 * FUNCTION: Setup_Ext_Hotkey_Pntr - to define the possible scanner tables.
 *
 *      0   Rc_RAM_Table
 *      1   ROM Table 1
 *      ...
 *
 * The external keyboard hotkeys are located in the end (the higher addresses)
 * of the Custom Key Table.  After the custom key table is a byte that holds
 * the number of external keyboard hotkeys.  Bits 0 - 3 of this byte defines
 * the number of hotkeys.
 *
 * The first hotkey definition is at the end (the higher addresses) of the
 * Custom Key Table.  The other definitions follow in the lower value
 * addresses.  The order of the hotkey scan code and hotkey function are setup
 * so the hotkey function is at an even address.
 *
 * The table looks like:
 *
 *      ...
 *      WORD holding the hotkey function for  hotkey #4
 *      BYTE holding the hotkey scan code for hotkey #4
 *      BYTE holding the hotkey scan code for hotkey #3
 *      WORD holding the hotkey function for  hotkey #3
 *      WORD holding the hotkey function for  hotkey #2
 *      BYTE holding the hotkey scan code for hotkey #2
 *      BYTE holding the hotkey scan code for hotkey #1
 *      WORD holding the hotkey function for  hotkey #1
 *
 * BYTE with bits 0 - 3 holding the number of hotkeys.
 *
 * The WORDs hold the System Control Function with the high byte of the
 * function in the lower address and the low byte of the function in the
 * higher address.
 *
 * The Ext_Hotkey_Pntr will be set to point to the byte past the end of the
 * Custom Key Table to allow the number of hotkeys to be accessed.
 * -------------------------------------------------------------- */
void Setup_Ext_Hotkey_Pntr(void)
{
    BYTE table_number;
    const BYTE code *cpntr;

    //table_number = (Wakeup3.byte >> shiftSCAN_SELECT) & maskSCAN_SELECT;

    Ext_Hotkey_Pntr = Hookc_Get_Ext_Hotkey_Pntr();

    if (Ext_Hotkey_Pntr == (BYTE *)0)
    {   /* If hook set the pointer to NULL, point to the end of external
           keyboard hotkey table table here.
           If the RAM_SCAN_TABLE option is not used, 4 tables can be in ROM */
//#if RAM_SCAN_TABLE
//      if (table_number == 0)
//      {
//          Ext_Hotkey_Pntr = Rc_RAM_Table + (MAX_SCAN_TABLE_SIZE - 1);
//      }
//      else
//#endif /* RAM_SCAN_TABLE */
        {   /* Point to start of ROM tables. */
            cpntr = (BYTE *) &Rc_ROM_Tables;

            while (table_number > LAST_SCAN_TBL_CNT)
            /* E-MCHT, 04/10/04 */
            {   /* Move pointer past first ROM table. */
                cpntr += 1 +            /* past size of Custom Table, */
                CUSTOM_KEY_TABLE_INDEX +    /* up to start of Custom Table, */
                cpntr[0] +          /* past Custom Table, */
                1;              /* past internal keyboard type and number of external keyboard hotkeys byte. */
                table_number--;         /* Bump table number. */
            } /* while (table_number > LAST_SCAN_TBL_CNT) */

            /* Move pointer to end of table. */
            cpntr +=                /* Move pointer */
            1 +                 /* past size of Custom Table, */
            CUSTOM_KEY_TABLE_INDEX +        /* up to start of Custom Table, */
            cpntr[0];               /* past Custom Table. */
            Ext_Hotkey_Pntr = (BYTE *) &cpntr;
        } /* if (table_number == 0) */
    } /* if (Ext_Hotkey_Pntr == (BYTE *)0) */
}

/* ----------------------------------------------------------------------------
 * FUNCTION: Lock_Scan
 *
 * Lock out the scanner via "Scan_Lock" and clear pending scanner "SEND"
 * events.
 * ------------------------------------------------------------------------- */
void Lock_Scan(void)
{
    Int_Var.Scan_Lock = 1;
}

/* ----------------------------------------------------------------------------
 * FUNCTION: Unlock_Scan - Unlock Scanner Keyboard Function
 * ------------------------------------------------------------------------- */
void Unlock_Scan(void)
{
    Int_Var.Scan_Lock = 0;
}

/* ----------------------------------------------------------------------------
 * FUNCTION: Init_Kbd - Scanner Keyboard Initialization.
 * ------------------------------------------------------------------------- */
void Init_Kbd(void)
{
    Setup_Scanner_Pntr();
    Setup_Ext_Hotkey_Pntr();
    //Scanner_Strobe_Delay = Calculate_Microsecond_Delay(Config.Scan_Read_Delay);
    Setup_Diode_Key();

    /* Clear debounce logic */

    new_keyh.byte = 0;
    new_keyl.byte = 0;

    //vClearExtendKeys();   // for extend keys

    Clear_Fn_Keys();
    Clear_Key();        /* Clear key matrix/buffer */
    ex_Clear_Key();     // for extend keys
    Clear_Typematic();

    Ccb42 = 0x38;
}

/* ----------------------------------------------------------------------------
 * FUNCTION:   Setup_Diode_Key
 * ------------------------------------------------------------------------- */
void Setup_Diode_Key(void)
{
    if(1)
    {
        //if (Ext_Cb3_DISABLE_NKEY) { /* Simple matrix (NO diodes) */
        diode_key = 0x00;
    }
    else
    {                      /* N-key (diodes in keyboard) */
        diode_key = 0xFF;
    }
}


/* ----------------------------------------------------------------------------
 * FUNCTION: Clear_Key - Clear local keyboard buffer and related variables.
 * ------------------------------------------------------------------------- */
void Clear_Key(void)
{
    BYTE i;
    WORD msk_strobe;

    /* Clear scan matrix. */
    for(i = 0; i < MAX_SCAN_LINES; i++)
    {
        bscan_matrix[i] = 0;
    }

    /* Clear key buffer. */
    for(i = 0; i < KBF_SIZE; i++)
    {
        bKEY_BUFF[i] = 0;
    }

    scan.kbf_head = 0;
    scan.kbf_tail = 0;

    typematic.byte = 0;

    scan.saf_make = 0;
    scan.saf_break = 0;
    scan.saf_keys = 0;

    /* Initialize scan.scan_line variable */
    //   msk_strobe = (Config.Msk_Strobe_H << 8) | Config.Msk_Strobe_L;
    msk_strobe = 0xFFFF;

    /* msk_strobe contains a flag for each of the valid scanner output lines.
    Flag set = output line is used. */
    scan.scan_line = 0;

    while ((msk_strobe & 0x0001) == 0)
    {   /* Loop until valid scan line found */
        scan.scan_line++;
        msk_strobe = msk_strobe >> 1;
    }
}


/* ----------------------------------------------------------------------------
 * FUNCTION: Clear_Typematic - Set default typematic delay and clear type-matic action.
 * ------------------------------------------------------------------------- */
void Clear_Typematic(void)
{
    typematic.byte = 0;
    bTMdelay = TM_DELAY;
    bTMrepeat = TM_REPEAT;
    PS2KB_Typematic = 0x6F;
}

/* ----------------------------------------------------------------------------
 * FUNCTION: Set_Typematic - Set the typematic rate.
 *
 * Input: typematic rate
 *        Bit[4:0] Typematic repeat interval
 *        Bit[6:5] Delay time
 * ------------------------------------------------------------------------- */
const BYTE code repeat_tbl[] =
{
     3,  4,  4,  5,  5,  6,  6,  7,  7,  8,  9, 10, 11, 12, 13, 14,
    15, 16, 18, 20, 22, 24, 25, 27, 30, 33, 37, 41, 44, 48, 52, 55
};

const BYTE code delay_tbl[] = {27, 55, 83, 111};

void Set_Typematic(BYTE type_rate)
{
    KB_Typematic = type_rate;   /* Save for suspend/resume. */

    /* Bit 4 - 0 typematic repeat interval index. */
    bTMrepeat = repeat_tbl[type_rate & 0x1F];

    /* Bit 6 - 5 delay time index. */
    bTMdelay = delay_tbl[(type_rate >> 5) & 0x03];
}


/* ----------------------------------------------------------------------------
 * FUNCTION: Handle_Scan - Service the local keyboard in response to a key press.
 * ------------------------------------------------------------------------- */
void Service_KEYSCAN(void)
{
    BYTE scan_activity;

    #if SUPPORT_KB_MATRIX_SCAN
    if (Timer_A.fbit.TMR_SCAN == 0)
    {
        Timer_A.fbit.TMR_SCAN = 1;  // Activate scan timer.
    }

    #if ExtendMatrix    // extend keys
    scan_activity = scan_keys()|ex_scan_keys();
    #else
    scan_activity = scan_keys();    // Scan keyboard matrix one time.
    #endif

    if (scan_activity)  // There is scan activity.
    {
        bKB_MATRIX_SENSE = 1;
        Oem_ScanActivity();
    }
    else            // No scan activity
    {
        Timer_A.fbit.TMR_SCAN = 0;  // Disable the scan timer.
        F_Service_KEYSCAN = 0;      // Clear any pending service request.
        Enable_Any_Key_Irq();       // Enable Any Key interrupt.
    }
    #endif

}


/*****************************************************************************/
/********** SCANNER KEY BUFFER ROUTINES **************************************/
/*****************************************************************************/

/* ----------------------------------------------------------------------------
 * FUNCTION:   Get_Buffer
 *
 * Get data byte from keyboard buffer (if not empty) and update "scan.kbf_head"
 * with new index into keyboard buffer.
 *
 * Return: Data from buffer (WORD of all 1's if buffer is empty).
 * ------------------------------------------------------------------------- */
WORD Get_Buffer(void)
{
    WORD buffer_data;

    if (scan.kbf_head != scan.kbf_tail)
    {
        buffer_data = (WORD) bKEY_BUFF[scan.kbf_head++];
        if (scan.kbf_head >= KBF_SIZE)
        {
            scan.kbf_head = 0;
        }
    }
    else
    {
        /* Buffer is empty. */
        buffer_data = (WORD) ~0;
    }

    return (buffer_data);
}

/* ----------------------------------------------------------------------------
 * FUNCTION:   Buffer_Mark - Mark local keyboard buffer tail.
 * ------------------------------------------------------------------------- */
void Buffer_Mark(void)
{
    scan.kbf_mark = scan.kbf_tail;   /* Copy scan.kbf_tail to scan.kbf_mark. */
}

/* ----------------------------------------------------------------------------
 * FUNCTION:   Buffer_Key
 *
 * Input:  Row/Column (0iii,oooo) to put in buffer.
 * Return: TRUE operation successful, FALSE unsuccessful.
 * ------------------------------------------------------------------------- */
FLAG Buffer_Key(BYTE row_column)
{
    FLAG ready = TRUE;  /* Ready for successful operation */

    bKEY_BUFF[scan.kbf_tail] = row_column;  /* Store Data to Buffer Tail */
    scan.kbf_tail++;                        /* Increment Buffer Tail (pointer) */

    if (scan.kbf_tail >= KBF_SIZE)
    {
        scan.kbf_tail = 0;  /* Wrap pointer if too large. */
    }

    /* Check Overflow */
    if (scan.kbf_tail == scan.kbf_head)
    {
        /* Overflow */
        scan.kbf_tail = scan.kbf_mark;  /* Restore buffer marker. */
        bKEY_BUFF[scan.kbf_tail] = 0;   /* Set OverFlow Mark. */
        ready = FALSE;                  /* Overflow Indication !!! */
    }

    return (ready);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bit Is_Key_Buffer_Ready(void)
{
    BYTE CheckTail;
    CheckTail = scan.kbf_tail;
    CheckTail++;
    if (CheckTail >= KBF_SIZE)
    {
        CheckTail = 0;  /* Check pointer if too large. */
    }
    /* Check Overflow */
    if ( CheckTail == scan.kbf_head )
    {
        return FALSE;   /* Overflow Indication !!! */
    }
    return TRUE;

}

/* ----------------------------------------------------------------------------
 * FUNCTION:   Buffer_String
 *
 * Places a string in the local keyboard buffer (without a terminating NULL).
 * Call Buffer_Key to place data in key buffer.
 *
 * Input:  Pointer to null terminated string.
 * Return: 0x80 if buffer overflow error, else 0.
 * ------------------------------------------------------------------------- */
BYTE Buffer_String(const BYTE *pntr)
{
    register BYTE error = 0;

    while ((*pntr != 0) && (error == 0))
    {
        if (Buffer_Key(*pntr++) == 0)
        {
        error = 0x80;   /* Indicate Overflow */
        }
    }

    return (error);
}

/* ----------------------------------------------------------------
 * FUNCTION:   Hookc_Get_Kbd_Type
 *
 * Return: 0 for a U.S. keyboard, 1 for a Japanese keyboard,
 *         anything else to allow CORE to determine keyboard type.
 * -------------------------------------------------------------- */
BYTE Hookc_Get_Kbd_Type(void)
{
    return(0xFF);
}

//=============================================================================
// FUNCTION: Hookc_Get_Ext_Hotkey_Pntr
//-----------------------------------------------------------------
BYTE *Hookc_Get_Ext_Hotkey_Pntr(void)
{
    return((void *) 0);
}

//=============================================================================

//*****************************************************************
// The functions of extend keys
//*****************************************************************
//-----------------------------------------------------------------
// The function of Scaning extend keys
//-----------------------------------------------------------------
BYTE ex_scan_keys(void)
{
    BYTE    SCAN_Flag = FALSE;
    BYTE    SCAN_Cnt;
    BYTE    SCAN_Val;

    ex_scan.saf_make= 0;
    ex_scan.saf_break = 0;
    ex_scan.saf_keys= 0;

    if (ex_typematic.byte)
    {
        ex_check_tm(ex_typematic);  // Check typematic.
    }

    if(ex_new_keyh.byte)
    {
        // Yes, bounding.
        ex_scan.saf_make = 1;
        ex_debounce_key();
        SCAN_Flag = TRUE;
    }

    if(!SCAN_Flag)
    {
        for(SCAN_Cnt=0; SCAN_Cnt<ExtendScanPin; SCAN_Cnt++)
        {
            ex_Write_Strobe(SCAN_Cnt);
            SCAN_Val = KSI;
            SCAN_Val = (~SCAN_Val)^bscan_matrix[SCAN_Cnt + STANDARD_SCAN_LINES];

            if (SCAN_Val != 0)
            {
                ex_check_scan(SCAN_Val, (SCAN_Cnt + STANDARD_SCAN_LINES));
            }

            if (bscan_matrix[SCAN_Cnt + STANDARD_SCAN_LINES])
            {   // Here, if current still active.
                ex_scan.saf_keys = 1;       // Set keys active bits. Check all key release.
            }
        }
    }

    if (ex_new_keyl.field.ghost == 1)
    {
        ex_new_keyl.field.ghost = 0;    // Ghost flag is set.
        SCAN_Val = ex_new_keyl.byte & 7;
        if (SCAN_Val == 0)
        {
            SCAN_Val = Byte_Mask((BYTE)(ex_new_keyh.field.input));
            SCAN_Val &= diode_key;
        }
        if (SCAN_Val == 0)
        {
            ex_new_keyh.byte = 0;       // Clear new key.
            ex_new_keyl.byte = 0;
        }
    }
    KSOH2 = 0x03;

    return((BYTE) (ex_scan.saf_make + ex_scan.saf_break + ex_scan.saf_keys));
}

//-----------------------------------------------------------------
// The function of Setting extend scan lines
//-----------------------------------------------------------------
void ex_Write_Strobe(BYTE scan_line)
{
    if(scan_line < 0x02)
    {
        KSOL  = 0xFF;
        KSOH1 = 0xFF;
        KSOH2 = ~(0x01<<scan_line);
        #if (ExternMatrixGPO)
        GPOKeyPin1_HI();
        #endif
    }
    else
    {
        KSOL  = 0xFF;
        KSOH1 = 0xFF;
        KSOH2 = 0xFF;
        #if (ExternMatrixGPO)
        GPOKeyPin1_LO();
        #endif
    }
}

//-----------------------------------------------------------------
// The function of Setting extend scan lines
//-----------------------------------------------------------------
void ex_check_scan(BYTE KSI_bit_num, BYTE scan_address)
{
    BYTE    flag;
    BYTE    change_make_key;
    BYTE    SCAN_Cnt;

    flag = FALSE;
    if (ex_new_keyl.field.ghost == 1)
    {
        flag = TRUE;
    }
    else
    {   // Find 0 -> 1 's.
        change_make_key = KSI_bit_num & ~bscan_matrix[scan_address];
        if (change_make_key == 0)
        {
            flag = FALSE;
        }
        else if (find_paths(change_make_key, scan_address))
        {
            flag = TRUE;                // TRUE if ghost keys.
        }
    }

    if (flag)
    {   // Ghost exists.
        ex_new_keyl.field.ghost = 1;    // Set ghost flag.
        KSI_bit_num &= bscan_matrix[scan_address] | diode_key;
    }

    SCAN_Cnt = 0x00;
    while (KSI_bit_num != 0)
    {
        if (KSI_bit_num & 0x01)
        {
            ex_cscfnd(SCAN_Cnt, scan_address);
        }
        SCAN_Cnt++;
        KSI_bit_num = KSI_bit_num >> 1; /* Shift bit out (and a zero bit in) to check next bit. */
    }
}

//=============================================================================
void ex_cscfnd(BYTE SCAN_Cnt, BYTE scan_address)
{
    if (bscan_matrix[scan_address] & Byte_Mask(SCAN_Cnt))
    {
        if (ex_scan.saf_break == 0)
        {
            ex_scan.saf_break = 1;
            ex_setup_debounce(SCAN_Cnt, scan_address, BREAK_EVENT);
        }
    }
    else
    {
        if (ex_scan.saf_make == 0)
        {
            ex_scan.saf_make = 1;
            ex_setup_debounce(SCAN_Cnt, scan_address, MAKE_EVENT);
        }
    }
}

//-----------------------------------------------------------------
// The function of extend keys debounce
//-----------------------------------------------------------------
void ex_setup_debounce(BYTE KSI_bit_num, BYTE scan_address, BYTE event)
{
    ex_new_keyh.field.output = scan_address-STANDARD_SCAN_LINES;
    ex_new_keyh.field.input = KSI_bit_num;
    ex_new_keyh.field.debounce_status = 1;
    ex_new_keyl.field.state = 0;        // Debounce has not counted out. */
    ex_new_keyl.field.same = 0;     // Key activity indication. */

    if (event == MAKE_EVENT)
    {
        ex_new_keyl.field.trans = 0;
        ex_new_keyl.field.count = 1;    //Ext_Cb2.field.Break_Count;
    }
    else
    {
        ex_new_keyl.field.trans = 1;
        ex_new_keyl.field.count = 1;    //Ext_Cb2.field.Make_Count;
    }
}

//=============================================================================
void ex_debounce_key(void)
{
    BYTE    SCAN_Val, SCAN_Tmp;

    ex_Write_Strobe(ex_new_keyh.field.output);

    SCAN_Tmp = ~Read_Scan_Lines();
    SCAN_Val = Byte_Mask((BYTE)(ex_new_keyh.field.input));
    SCAN_Val = SCAN_Val & SCAN_Tmp;

    if (SCAN_Val != 0)
    {
        if (ex_new_keyl.field.trans == 0)   // key make
        {
            ex_new_keyl.field.same = 1;     // last key detected as a MAKE, same = 1.
        }
        else
        {
            ex_new_keyl.field.same = 0;    // last key detected as a BREAK, same = 0.
        }
    }
    else
    {
        if (ex_new_keyl.field.trans == 0)
        {
            ex_new_keyl.field.same = 0;    // last key detected as a MAKE, same = 0.
        }
        else
        {
            ex_new_keyl.field.same = 1;    // last key detected as a BREAK, same = 1.
        }
    }

    if (ex_new_keyl.field.state == 0)   // 1 means debounce counted out.
    {
        if (ex_new_keyl.field.count != 0)
        {
            ex_new_keyl.field.count--;
        }
        else
        {
            if (ex_new_keyl.field.same == 0)
            {
                ex_new_keyh.byte = 0;   // Debounce failed, so claer all data
                ex_new_keyl.byte = 0;
            }
            else
            {
                ex_new_keyl.field.state = 1;
            }
        }
    }
    else
    {
        if (ex_new_keyl.field.same == 0)
        {
            ex_new_keyh.byte = 0;   /* Debounce failed. */
            ex_new_keyl.byte = 0;
        }
        else
        {
            ex_change_valid();
        }
    }
}

//-----------------------------------------------------------------
// The function of checking extend key valid
//-----------------------------------------------------------------
void ex_change_valid(void)
{
    BYTE    temp, mask, event;

    mask = Byte_Mask((BYTE) (ex_new_keyh.field.input));
    temp = bscan_matrix[(ex_new_keyh.field.output + STANDARD_SCAN_LINES)];
    temp = temp ^ mask;
    bscan_matrix[(ex_new_keyh.field.output + STANDARD_SCAN_LINES)] = temp;

    event = BREAK_EVENT;
    if ((mask & temp) != 0)
    {
        event = MAKE_EVENT;
        ex_typematic.byte = ex_new_keyh.byte;
        ex_scan.TMscale = TM_SCALE;
        bTMcount = bTMdelay;

        typematic.byte = 0x00;
    }

    ex_Send_Scan2(ex_new_keyh, event);
    ex_new_keyh.byte = 0;
    ex_new_keyl.byte = 0;
}

//-----------------------------------------------------------------
// The function of
//-----------------------------------------------------------------
void ex_Send_Scan2(union KEY key, BYTE event)
{
    BYTE    table_entry;
    BYTE    temp;

    #if SUPPORT_MATRIX_UTILITY
    if(IsFlag1(UtilityFlag,KBDebug))
    {
		if(KeyScanACK==0xFF)
		{
			if(event==MAKE_EVENT)
			{
				KeyScanKSI = key.field.input;
				KeyScanKSO = key.field.output;
				KeyScanACK = 0x44;
			}
		}
        return;
    }
    #endif

    temp = (key.field.input * 3) + key.field.output;
    table_entry = *(Extendkey_Table_Pntr + temp);
    Send_Key(table_entry, event);
}

//-----------------------------------------------------------------
// The function of clearing extend keys
//-----------------------------------------------------------------
void ex_Clear_Key(void)
{
    ex_new_keyh.byte = 0;
    ex_new_keyl.byte = 0;
    ex_typematic.byte = 0;
}

//-----------------------------------------------------------------
// The function of checking extend keys Typematic
//-----------------------------------------------------------------
void ex_check_tm(union KEY key)
{
    BYTE    SCAN_flag = FALSE;
    BYTE    SCAN_Val;
    SCAN_Val = Byte_Mask((BYTE) (key.field.input));
    SCAN_Val = SCAN_Val & bscan_matrix[key.field.output+STANDARD_SCAN_LINES];

    if (SCAN_Val == 0)
    {
        ex_typematic.byte = 0;
        SCAN_flag = TRUE;
    }

    if (!SCAN_flag)
    {
        ex_scan.TMscale--;
        if (ex_scan.TMscale != 0)
        {
            SCAN_flag = TRUE;
        }
    }

    if (!SCAN_flag)
    {
        ex_scan.TMscale = TM_SCALE;
        bTMcount--;
        if (bTMcount != 0)
        {
            SCAN_flag = TRUE;
        }
    }

    if (!SCAN_flag)
    {
        bTMcount = bTMrepeat;
        ex_Send_Scan2(key, REPEAT_EVENT);
    }
}

//=============================================================================
