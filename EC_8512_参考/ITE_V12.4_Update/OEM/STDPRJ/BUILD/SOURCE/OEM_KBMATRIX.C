/*-----------------------------------------------------------------------------
 * Filename: OEM_KBMATRIX.C
 * Function: 1.Keyboard Matrix RC Table(s) for Scan Matix.
 *           2.Overlay/ExtKey/Numpad table..
 *           3.Function Hot-key handle.
 *
 * Copyright (c) 2006-2009, ITE Tech. Inc. All Rights Reserved.
 *---------------------------------------------------------------------------*/
#include "CORE_INCLUDE.H"
#include "OEM_INCLUDE.H"

/*-----------------------------------------------------------------------------
 * Local Parameter Definition
 *---------------------------------------------------------------------------*/
//#define   XXX     X

/*
;--------------------------------------------------------------------------------------------------
// Keyboard Printing Example
// .----..----..----..----..----..----..----..----..----..----..----..----..----..----..----..----.
// |ESC || F1 || F2 || F3 || F4 || F5 || F6 || F7 || F8 || F9 || F10||F11 || |  ||  - || Ins|| Del|
// |    ||    ||    ||    ||    ||    ||    ||    ||    ||BrDn||BrUp|| F12|| \  ||  _ ||    ||    |
// '----''----''----''----''----''----''----''----''----''----''----''----''----''----''----''----'
// .------..------..------..------..------..------..------..------..------..------..------..------.
// |  1"  ||  2@  ||  3#  ||  4$  ||  5%  ||  6^  ||  7&  ||  8*  ||  9(  ||  0)  ||  }]  || Back |
// '------''------''------''------''------''------''------''------''------''------''------''------'
// .---..------..------..------..------..------..------..------..------..-----..-----..----..-----.
// |Tab||  Q   ||  W   ||  E   ||  R   ||  T   ||  Y   ||  U   ||  I   ||  O  ||  P  || {[ ||     |
// '---''------''------''------''------''------''------''------''------''-----''-----''----'|     |
// .----..------..------..------..------..------..-----..-----..-----..-----..-----..-------+     |
// |Cpas||  A   ||  S   ||  D   ||  F   ||  G   ||  H  ||  J  ||  K  ||  L  ||  ;  ||    Enter    |
// '----''------''------''------''------''------''-----''-----''-----''-----''-----''-------------'
// .------..------..------..------..------..------..------..------..----..----..----..-----..-----.
// |Shift ||  Z   ||  X   ||  C   ||  V   ||  B   ||  N   ||  M   || ,< || .> || /? || Up  ||Shift|
// |      ||      ||      ||      ||      ||      ||      ||      ||    ||    ||    ||PgUp ||     |
// '------''------''------''------''------''------''------''------''----''----''----''-----''-----'
// .----..----..----..----..----..-------------------------..----..----..----..-----..-----..-----.
// | Fn ||Ctrl|| Win|| `~ || Alt||  SPACE                  || Alt|| =+ || "' || <-  || Down|| ->  |
// |    ||    ||    ||    ||    ||                         ||    ||    ||    ||Home || PgDn|| End |
// '----''----''----''----''----''-------------------------''----''----''----''-----''-----''-----'
*/
//-------------------------------------------------------------------------------------------------
const unsigned char code Rc_ROM_Tables[] =
{
    // Scanner Table 1  - 16 x 8 Scan Table
    //00   01   02   03   04   05   06   07   08   09   10   11   12   13   14   15
    0xAD,0xAE,0xB2,0xB3,0x9C,0x00,0x00,0x00,0xB4,0x00,0x00,0x00,0xE4,0xB5,0xB8,0xE1, // 00
    0x0D,0x26,0x25,0x2B,0x33,0x00,0x00,0x00,0xA1,0x00,0x00,0x00,0x9D,0x9E,0x4E,0x55, // 01
    0x58,0x1D,0x24,0x61,0x32,0x00,0x00,0x00,0xA2,0x00,0x00,0x00,0xA3,0x9F,0x94,0x66, // 02
    0x16,0x1E,0x2D,0x21,0x2A,0x00,0x00,0x00,0xA5,0x00,0x00,0x00,0x9B,0x95,0x5B,0x5D, // 03
    0xA8,0x15,0x23,0x22,0x31,0x00,0x00,0x00,0xA6,0x00,0x00,0x00,0xA7,0x54,0xAB,0x98, // 04
    0x8E,0x1C,0x1B,0x1A,0x29,0x00,0x00,0x00,0xA4,0x84,0x00,0x00,0xAA,0x52,0xA0,0x99, // 05
    0x0E,0xAF,0x2E,0x2C,0x34,0x00,0x82,0x8A,0xA9,0x00,0x8C,0x88,0x9A,0x97,0x96,0x00, // 06
    0x76,0xB0,0xB1,0x36,0x35,0x00,0x00,0x8B,0x41,0x00,0x8D,0x89,0xE5,0xB6,0xB7,0x91  // 07
};
//-------------------------------------------------------------------------------------------------
const unsigned char code EtKey_ROM_Tables[] =
{
    //16   17   18  Extended - 19X8 Scan Table
    0x33,0x69,0x70,     // 00
    0x35,0xE3,0x79,     // 01
    0x3C,0x6B,0x7A,     // 02
    0x36,0x7C,0x81,     // 03
    0x3B,0x6C,0x7D,     // 04
    0x3D,0x75,0x7B,     // 05
    0x3A,0x73,0x74,     // 06
    0x31,0x72,0x71,     // 07

};
//-----------------------------------------------------------------------------
//   Key 0[45h] //  Key P[4Dh] //  Key:;[4Ch] // Key ?/[4Ah]
//Numpad *[B3h]//Numpad -[B7h]//Numpad +[BBh]//Numpad /[BEh]
#if NUMPAD_MATH_USE_OEM
const BYTE code ss2_9F[] ={  0x45,  0xB3,  0xB3,  0xB3  };  //[0 ]<->[*]
const BYTE code ss2_A3[] ={  0x4D,  0xB7,  0xB7,  0xB7  };  //[P ]<->[-]
const BYTE code ss2_A7[] ={  0x4C,  0xBB,  0xBB,  0xBB  };  //[:;]<->[+]
const BYTE code ss2_AA[] ={  0x4A,  0xBE,  0xBE,  0xBE  };  //[?/]<->[/]
#endif
//-----------------------------------------------------------------------------
#if NUMPAD_USE_OEM
//Copy from CORE_XLT.C if Oem special request.
const BYTE code ss2_9C[] ={  0x3D,  0x3D,  0x3D,  0xB0  };  //[7 ]<->[7 ]
const BYTE code ss2_9D[] ={  0x3E,  0x3E,  0x3E,  0xB1  };  //[8 ]<->[8 ]
const BYTE code ss2_9E[] ={  0x46,  0x46,  0x46,  0xB2  };  //[9 ]<->[9 ]

const BYTE code ss2_A0[] ={  0x3C,  0x3C,  0x3C,  0xB4  };  //[U ]<->[4 ]
const BYTE code ss2_A1[] ={  0x43,  0x43,  0x43,  0xB5  };  //[I ]<->[5 ]
const BYTE code ss2_A2[] ={  0x44,  0x44,  0x44,  0xB6  };  //[O ]<->[6 ]

const BYTE code ss2_A4[] ={  0x3B,  0x3B,  0x3B,  0xB8  };  //[J ]<->[1 ]
const BYTE code ss2_A5[] ={  0x42,  0x42,  0x42,  0xB9  };  //[K ]<->[2 ]
const BYTE code ss2_A6[] ={  0x4B,  0x4B,  0x4B,  0xBA  };  //[L ]<->[3 ]

const BYTE code ss2_A8[] ={  0x3A,  0x3A,  0x3A,  0xBC  };  //[M ]<->[0 ]
const BYTE code ss2_A9[] ={  0x49,  0x49,  0x49,  0xBD  };  //[>/]<->[. ]
#endif
//-----------------------------------------------------------------------------

/* ----------------------------------------------------------------------------
 * FUNCTION: sskey2_overlay_table
 * ------------------------------------------------------------------------- */
const BYTE code sskey2_overlay_table[] =
{               //  Index   Normal      <-> [Fn]
    0x94, 0x95, //  0Fh     Home        <-> End             // 0xE0
    0x77, 0x7E, //  10h     Num Lk      <-> Scr LK          // 0xE1
    0x98, 0x96, //  11h     Up          <-> PageUp          // 0xE2
    0x99, 0x97, //  12h     Down        <-> PageDn          // 0xE3
    0x9A, 0x94, //  13h     Left        <-> Home            // 0xE4
    0x9B, 0x95, //  14h     Right       <-> End             // 0xE5
    0xC3, 0xC3, //  15h     Insert      <-> PrsSc           // 0xE6
    0x07, 0x91, //  16h     Delete      <-> Sys Rq          // 0xE7
    0x96, 0xA1, //  17h     PageUp      <-> Vol Up          // 0xE8
    0x97, 0xA2, //  18h     PageDown    <-> Vol Down        // 0xE9
    0x95, 0xA0, //  19h     End         <-> Mute            // 0xEA
    0xC3, 0xC1, //  0Bh     PrtSC       <-> SysRq           // 0xEB
    0x00, 0x00, //  0Ch     --------    <-> --------        // 0xEC
    0x00, 0x00, //  0Dh     --------    <-> --------        // 0xED
    0x00, 0x00, //  0Eh     --------    <-> --------        // 0xEE
    0x00, 0x00, //  0Fh     --------    <-> --------        // 0xEF
    0x00, 0x00, //  10h     --------    <-> --------        // 0xF0
    0x00, 0x00, //  11h     --------    <-> --------        // 0xF1
    0x00, 0x00, //  12h     --------    <-> --------        // 0xF2
    0x00, 0x00, //  13h     --------    <-> --------        // 0xF3
    0x00, 0x00, //  14h     --------    <-> --------        // 0xF4
    0x00, 0x00, //  15h     --------    <-> --------        // 0xF5
    0x00, 0x00, //  16h     --------    <-> --------        // 0xF6
    0x00, 0x00, //  17h     --------    <-> --------        // 0xF7
    0x00, 0x00, //  18h     --------    <-> --------        // 0xF8
    0x00, 0x00, //  19h     --------    <-> --------        // 0xF9
    0x00, 0x00, //  1Ah     --------    <-> --------        // 0xFA
    0x00, 0x00, //  1Bh     --------    <-> --------        // 0xFB
    0x00, 0x00, //  1Ch     --------    <-> --------        // 0xFC
    0x00, 0x00, //  1Dh     --------    <-> --------        // 0xFD
    0x00, 0x00, //  1Eh     --------    <-> --------        // 0xFE
    0x00, 0x00, //  1Fh     --------    <-> --------        // 0xFF

};
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Function Key F1 ~ F12 + 2 ExtKey
//-----------------------------------------------------------------------------
// RC = 0xAC ~ 0xBF           Normal  Fn
const BYTE code ss2_AC[] = {  0x00,  0x00  };
const BYTE code ss2_AD[] = {  0x05,  0xD0  };  //  F1<-> HotKey_Fn_F1
const BYTE code ss2_AE[] = {  0x06,  0xD1  };  //  F2<-> HotKey_Fn_F2
const BYTE code ss2_AF[] = {  0x04,  0xD2  };  //  F3<-> HotKey_Fn_F3
const BYTE code ss2_B0[] = {  0x0C,  0xD3  };  //  F4<-> HotKey_Fn_F4
const BYTE code ss2_B1[] = {  0x03,  0xD4  };  //  F5<-> HotKey_Fn_F5
const BYTE code ss2_B2[] = {  0x0B,  0xD5  };  //  F6<-> HotKey_Fn_F6
const BYTE code ss2_B3[] = {  0x80,  0xA0  };  //  F7<-> Mute event
const BYTE code ss2_B4[] = {  0x0A,  0xA2  };  //  F8<-> Volume Down event
const BYTE code ss2_B5[] = {  0x01,  0xA1  };  //  F9<-> Volume Up event
const BYTE code ss2_B6[] = {  0x09,  0xAE  };  // F10<-> HotKey_Fn_F10
const BYTE code ss2_B7[] = {  0x78,  0x78  };  // F11<-> HotKey_Fn_F11
const BYTE code ss2_B8[] = {  0x07,  0x91, };  // F12<-> HotKey_Fn_F12
const BYTE code ss2_B9[] = {  0x98,  0xDC  };
const BYTE code ss2_BA[] = {  0x99,  0xDD  };
const BYTE code ss2_BB[] = {  0x22,  0xC4  };
const BYTE code ss2_BC[] = {  0x76,  0xDE  };
const BYTE code ss2_BD[] = {  0x9B,  0xDF  };
const BYTE code ss2_BE[] = {  0x23,  0xD8  };
const BYTE code ss2_BF[] = {  0x82,  0x83  };

const BYTE code ss2_C0[] = {  0xC0,  0xC0  };   // Delete <-> Delete
const BYTE code ss2_C1[] = {  0xC1,  0xC1  };   // SysRq  <-> SysRq
const BYTE code ss2_C2[] = {  0xC2,  0xC2  };   // Insert <-> Insert
const BYTE code ss2_C3[] = {  0xC3,  0xC3  };   // PrintScreen <-> PrintScreen
const BYTE code ss2_C4[] = {  0xC4,  0xC4  };   //
const BYTE code ss2_C5[] = {  0xC5,  0xC5  };   //
const BYTE code ss2_C6[] = {  0xC6,  0xC6  };   //
const BYTE code ss2_C7[] = {  0xC7,  0xC7  };   //
const BYTE code ss2_C8[] = {  0xC8,  0xC8  };   //
const BYTE code ss2_C9[] = {  0xC9,  0xC9  };   //
const BYTE code ss2_CA[] = {  0xCA,  0xCA  };   //
const BYTE code ss2_CB[] = {  0xCB,  0xCB  };   //
const BYTE code ss2_CC[] = {  0xCC,  0xCC  };   //
const BYTE code ss2_CD[] = {  0xCD,  0xCD  };   //
const BYTE code ss2_CE[] = {  0xCE,  0xCE  };   //
const BYTE code ss2_CF[] = {  0xCF,  0xCF  };   //

//-----------------------------------------------------------------------------
const FUNCT_PTR_V_B code HotKey_Fn_Fx[] =
{
    HotKey_Fn_F1,       // Fn + F1
    HotKey_Fn_F2,       // Fn + F2
    HotKey_Fn_F3,       // Fn + F3
    HotKey_Fn_F4,       // Fn + F4
    HotKey_Fn_F5,       // Fn + F5
    HotKey_Fn_F6,       // Fn + F6
    HotKey_Fn_F7,       // Fn + F7
    HotKey_Fn_F8,       // Fn + F8
    HotKey_Fn_F9,       // Fn + F9
    HotKey_Fn_F10,      // Fn + F10
    HotKey_Fn_F11,      // Fn + F11
    HotKey_Fn_F12,      // Fn + F12
    HotKey_Fn_DCH,      // Fn + Key
    HotKey_Fn_DDH,      // Fn + Key
    HotKey_Fn_DEH,      // Fn + Key
    HotKey_Fn_DFH,      // Fn + Key
    HotKey_Fn_E0H,      // Fn + Key[5]          //0xE0
    HotKey_Fn_E1H,      // Fn + Key[D]          //0xE1
    HotKey_Fn_E2H,      // Fn + Key[E]          //0xE2
    HotKey_Fn_E3H,      // Fn + Key[6]          //0xE3
    HotKey_Fn_E4H,      // Fn + Key[ ]          //0xE4
    HotKey_Fn_E5H,      // Fn + Key[ ]          //0xE5
    HotKey_Fn_E6H,      // Fn + Key[ ]          //0xE6
    HotKey_Fn_E7H,      // Fn + Key[ ]          //0xE7
    HotKey_Fn_E8H,      // Fn + Key[ ]          //0xE8
    HotKey_Fn_E9H,      // Fn + Key[ ]          //0xE9
    HotKey_Fn_EAH,      // Fn + Key[ ]          //0xEA
    HotKey_Fn_EBH,      // Fn + Key[ ]          //0xEB
    HotKey_Fn_ECH,      // Fn + Key[ ]          //0xEC
    HotKey_Fn_EDH,      // Fn + Key[ ]          //0xED
    HotKey_Fn_EEH,      // Fn + Key[ ]          //0xEE
    HotKey_Fn_EFH,      // Fn + Key[ ]          //0xEF

};
//-----------------------------------------------------------------------------
// The function of hotkey NULL
//-----------------------------------------------------------------------------
void HotKey_NULL(BYTE event)
{
    ;
}
//-----------------------------------------------------------------------------
// The function of hotkey Fn + Fx
//-----------------------------------------------------------------------------
void costomer_function(BYTE code_byte, BYTE event)
{
    (HotKey_Fn_Fx[code_byte])(event);
}

/* ----------------------------------------------------------------------------
 * FUNCTION: Customer Fn + Hotkey Function
 *
 * Input: code
 *        event = Contact event (MAKE_EVENT, BREAK_EVENT, or REPEAT_EVENT)
 * ------------------------------------------------------------------------- */
//-----------------------------------------------------------------------------
// The function of hotkey Fn + F1
//-----------------------------------------------------------------------------
void HotKey_Fn_F1(BYTE event)
{
    //if ((event == MAKE_EVENT)||(event == REPEAT_EVENT))
    if ( event == MAKE_EVENT )
    {
        WriteSCI_QueryValue(_SCIEVT_FnF1);
    }
}

//-----------------------------------------------------------------------------
// The function of hotkey Fn + F2
//-----------------------------------------------------------------------------
void HotKey_Fn_F2(BYTE event)
{
    //if ((event == MAKE_EVENT)||(event == REPEAT_EVENT))
    if ( event == MAKE_EVENT )
    {
        WAIT_SCI_CENTER = 0;
        //WriteSCI_QueryValue(_SCIEVT_FnF2);
        WriteSCI_QueryValue(_SCIEVT_SLEEP);
    }
}

//-----------------------------------------------------------------------------
// The function of hotkey Fn + F3
//-----------------------------------------------------------------------------
void HotKey_Fn_F3(BYTE event)
{
    //if ((event == MAKE_EVENT)||(event == REPEAT_EVENT))
    if ( event == MAKE_EVENT )
    {
        //WriteSCI_QueryValue(_SCIEVT_FnF3);
        WriteSCI_QueryValue(_SCIEVT_LCD);
    }
}

//-----------------------------------------------------------------------------
// The function of hotkey Fn + F4
//-----------------------------------------------------------------------------
void HotKey_Fn_F4(BYTE event)
{
    //if ((event == MAKE_EVENT)||(event == REPEAT_EVENT))
    if ( event == MAKE_EVENT )
    {
        //WriteSCI_QueryValue(_SCIEVT_FnF4);
        if( DACDAT1 >= 0x10 )   //Brightness
        {
            DACDAT1-=0x10;
        }
    }
}

//-----------------------------------------------------------------------------
// The function of hotkey Fn + F5
//-----------------------------------------------------------------------------
void HotKey_Fn_F5(BYTE event)
{
    //if ((event == MAKE_EVENT)||(event == REPEAT_EVENT))
    if ( event == MAKE_EVENT )
    {
        //WriteSCI_QueryValue(_SCIEVT_FnF5);
        if( DACDAT1 < 0xF0 )    //Brightness
        {
            DACDAT1+=0x10;
        }
    }
}

//-----------------------------------------------------------------------------
// The function of hotkey Fn + F6
//-----------------------------------------------------------------------------
void HotKey_Fn_F6(BYTE event)
{
    //if ((event == MAKE_EVENT)||(event == REPEAT_EVENT))
    if ( event == MAKE_EVENT )
    {

    }
}

//-----------------------------------------------------------------------------
// The function of hotkey Fn + F7
//-----------------------------------------------------------------------------
void HotKey_Fn_F7(BYTE event)
{
    //if ((event == MAKE_EVENT)||(event == REPEAT_EVENT))
    if ( event == MAKE_EVENT )
    {
        WriteSCI_QueryValue(_SCIEVT_FnF7);
    }
}

//-----------------------------------------------------------------------------
// The function of hotkey Fn + F8
//-----------------------------------------------------------------------------
void HotKey_Fn_F8(BYTE event)
{
    //if ((event == MAKE_EVENT)||(event == REPEAT_EVENT))
    if ( event == MAKE_EVENT )
    {
        WAIT_SCI_CENTER = 0;
        WriteSCI_QueryValue(_SCIEVT_FnF8);
    }
}

//-----------------------------------------------------------------------------
// The function of hotkey Fn + F9
//-----------------------------------------------------------------------------
void HotKey_Fn_F9(BYTE event)
{
    //if ((event == MAKE_EVENT)||(event == REPEAT_EVENT))
    if ( event == MAKE_EVENT )
    {
        WAIT_SCI_CENTER = 0;
        WriteSCI_QueryValue(_SCIEVT_FnF9);
    }
}

//-----------------------------------------------------------------------------
// The function of hotkey Fn + F10
//-----------------------------------------------------------------------------
void HotKey_Fn_F10(BYTE event)
{
    //if ((event == MAKE_EVENT)||(event == REPEAT_EVENT))
    if ( event == MAKE_EVENT )
    {
        WriteSCI_QueryValue(_SCIEVT_FnF10);
    }
}

//-----------------------------------------------------------------------------
// The function of hotkey Fn + F11
//-----------------------------------------------------------------------------
void HotKey_Fn_F11(BYTE event)
{
    //if ((event == MAKE_EVENT)||(event == REPEAT_EVENT))
    if ( event == MAKE_EVENT )
    {
        WriteSCI_QueryValue(_SCIEVT_FnF11);
    }
}

//-----------------------------------------------------------------------------
// The function of hotkey Fn + F12
//-----------------------------------------------------------------------------
void HotKey_Fn_F12(BYTE event)
{
    //if ((event == MAKE_EVENT)||(event == REPEAT_EVENT))
    if ( event == MAKE_EVENT )
    {
        WriteSCI_QueryValue(_SCIEVT_FnF12);
    }
}

//-----------------------------------------------------------------------------
// The function of hotkey Fn + Key
//-----------------------------------------------------------------------------
void HotKey_Fn_DCH(BYTE event)
{
    if ((event == MAKE_EVENT)||(event == REPEAT_EVENT))
    {

    }
}

//-----------------------------------------------------------------------------
// The function of hotkey Fn + Key
//-----------------------------------------------------------------------------
void HotKey_Fn_DDH(BYTE event)
{
    if ((event == MAKE_EVENT)||(event == REPEAT_EVENT))
    {

    }
}

//-----------------------------------------------------------------------------
// The function of hotkey Fn + Key
//-----------------------------------------------------------------------------
void HotKey_Fn_DEH(BYTE event)
{
    if ((event == MAKE_EVENT)||(event == REPEAT_EVENT))
    {

    }
}

//-----------------------------------------------------------------------------
// The function of hotkey Fn + Key
//-----------------------------------------------------------------------------
void HotKey_Fn_DFH(BYTE event)
{
    if ((event == MAKE_EVENT)||(event == REPEAT_EVENT))
    {

    }
}

//-----------------------------------------------------------------------------
// The function of hotkey Fn + Key
//-----------------------------------------------------------------------------
void HotKey_Fn_E0H(BYTE event)
{
//
}

//-----------------------------------------------------------------------------
// The function of hotkey Fn + Key
//-----------------------------------------------------------------------------
void HotKey_Fn_E1H(BYTE event)
{
}

//-----------------------------------------------------------------------------
// The function of hotkey Fn + Key
//-----------------------------------------------------------------------------
void HotKey_Fn_E2H(BYTE event)
{
}

//-----------------------------------------------------------------------------
// The function of hotkey Fn + Key
//-----------------------------------------------------------------------------
void HotKey_Fn_E3H(BYTE event)
{

}

//-----------------------------------------------------------------------------
// The function of hotkey Fn + Key
//-----------------------------------------------------------------------------
void HotKey_Fn_E4H(BYTE event)
{

}
//-----------------------------------------------------------------------------
// The function of hotkey Fn + Key
//-----------------------------------------------------------------------------
void HotKey_Fn_E5H(BYTE event)
{

}
//-----------------------------------------------------------------------------
// The function of hotkey Fn + Key
//-----------------------------------------------------------------------------
void HotKey_Fn_E6H(BYTE event)
{

}
//-----------------------------------------------------------------------------
// The function of hotkey Fn + Key
//-----------------------------------------------------------------------------
void HotKey_Fn_E7H(BYTE event)
{

}
//-----------------------------------------------------------------------------
void HotKey_Fn_E8H(BYTE event)
{

}
//-----------------------------------------------------------------------------
void HotKey_Fn_E9H(BYTE event)
{

}
//-----------------------------------------------------------------------------
void HotKey_Fn_EAH(BYTE event)
{

}
//-----------------------------------------------------------------------------
void HotKey_Fn_EBH(BYTE event)
{

}
//-----------------------------------------------------------------------------
void HotKey_Fn_ECH(BYTE event)
{

}
//-----------------------------------------------------------------------------
void HotKey_Fn_EDH(BYTE event)
{
    //TEST INVERSE_REG(UtilityFlag,2);
}
//-----------------------------------------------------------------------------
void HotKey_Fn_EEH(BYTE event)
{
     //TEST INVERSE_REG(UtilityFlag,1);
}
//-----------------------------------------------------------------------------
void HotKey_Fn_EFH(BYTE event)
{
     //TEST INVERSE_REG(UtilityFlag,0);
}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Oem_ScanActivity Hook
//-----------------------------------------------------------------------------
void Oem_ScanActivity(void)
{
    if( !CORE_PMFLAG_S3 )   return;

    if( STATE_PM_SLPS3() )  return;

    PM_PWRBTN_LO();
    vDelay1MS(80);
    PM_PWRBTN_HI();
}
//-----------------------------------------------------------------------------

/*-----------------------------------------------------------------------------
 * End
 *---------------------------------------------------------------------------*/